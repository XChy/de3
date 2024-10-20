#include "Mutator.h"
#include "transforms/CFGExpand/CFGExpand.h"
#include "transforms/CFGRandomSimplify/CFGRandomSimplify.h"
#include "transforms/ExprExpand/ExprExpandPass.h"
#include "transforms/InstRandomCombine/InstRandomCombine.h"
#include "transforms/RandomCVP/RandomCVP.h"
#include "transforms/RandomDSE/RandomDSE.h"
#include "transforms/RandomLICM/RandomLICM.h"
#include "transforms/RandomLoopSink/RandomLoopSink.h"
#include "transforms/RandomMem2Reg/RandomMem2Reg.h"
#include "transforms/RandomReg2Mem/RandomReg2Mem.h"
#include "transforms/RandomScalarizer/RandomScalarizer.h"
#include "transforms/RandomSink/RandomSink.h"
#include "utils/Debug.h"
#include "utils/Files.h"
#include "utils/Random.h"
#include <cassert>
#include <llvm/ADT/StringExtras.h>
#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/Analysis/TargetTransformInfo.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/PatternMatch.h>
#include <llvm/IRPrinter/IRPrintingPasses.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Pass.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar/CorrelatedValuePropagation.h>
#include <llvm/Transforms/Scalar/DeadStoreElimination.h>
#include <llvm/Transforms/Scalar/LoopPassManager.h>
#include <llvm/Transforms/Scalar/LoopSimplifyCFG.h>
#include <llvm/Transforms/Scalar/Reassociate.h>
#include <llvm/Transforms/Scalar/SCCP.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>
#include <llvm/Transforms/Utils/Cloning.h>
#include <llvm/Transforms/Utils/LCSSA.h>
#include <llvm/Transforms/Utils/Mem2Reg.h>
#include <llvm/Transforms/Vectorize/LoopVectorize.h>
#include <llvm/Transforms/Vectorize/SLPVectorizer.h>
#include <random>
#include <string>

using namespace llvm;

static cl::opt<int> PrintBefore("print-before-mo", cl::desc("<print-before>"),
                                cl::init(-1));

// 0: comlete random
// 1: deoptimize
// 2: deterministic
static cl::opt<int> PipelineType("pipeline-type", cl::desc("<pipeline-type>"),
                                 cl::init(0));

static cl::opt<int> NumLastRemoved("remove-last",
                                   cl::desc("<remove-last-n-passes>"),
                                   cl::init(0));

#define PASS_CREATOR(Pass, Weight, Name)                                       \
  {                                                                            \
    [](FunctionPassManager &FPM) { FPM.addPass(Pass); }, (Weight), (Name)      \
  }

typedef std::function<void(FunctionPassManager &)> FuncPassBuilder;
typedef std::tuple<FuncPassBuilder, int, std::string> PassEntry;

static std::vector<PassEntry> RandomizedPasses = {
    PASS_CREATOR(ExprExpandPass(), 50, "random-inst-expand"),
    PASS_CREATOR(InstRandomCombinePass(), 50, "random-inst-combine"),
    PASS_CREATOR(CFGRandomSimplifyPass(), 50, "random-cfg-simplify"),
    PASS_CREATOR(CFGExpandPass(), 50, "random-cfg-expand"),
    PASS_CREATOR(RandomMemToRegPass(), 50, "random-mem2reg"),
    PASS_CREATOR(RandomRegToMemPass(), 50, "random-reg2mem"),
    PASS_CREATOR(RandomLoopSinkPass(), 50, "random-loopsink"),
    // TODO: randomize slp vectorizer
    PASS_CREATOR(SLPVectorizerPass(), 50, "slp-vectorizer"),
    PASS_CREATOR(LoopVectorizePass(), 50, "loop-vectorizer"),
    PASS_CREATOR(SCCPPass(), 50, "sccp"),
    {[](FunctionPassManager &FPM) {
       // LICM requires LCSSA guard
       FPM.addPass(LCSSAPass());
       FPM.addPass(createFunctionToLoopPassAdaptor(RandomLICMPass(), true));
     },
     50, "random-licm"},
    PASS_CREATOR(RandomScalarizerPass(), 50, "random-scalarizer"),
    PASS_CREATOR(RandomCVPPass(), 50, "random-cvp"),
    PASS_CREATOR(RandomDSEPass(), 50, "random-dse"),
};

static std::vector<PassEntry> DeoptimizePasses = {
    PASS_CREATOR(ExprExpandPass(), 50, "random-inst-expand"),
    PASS_CREATOR(CFGExpandPass(), 50, "random-cfg-expand"),
    PASS_CREATOR(RandomSinkPass(), 50, "random-sink"),
    PASS_CREATOR(RandomRegToMemPass(), 50, "random-mem2reg"),
    PASS_CREATOR(RandomLoopSinkPass(), 50, "random-loopsink"),
    PASS_CREATOR(RandomScalarizerPass(), 50, "random-scalarizer"),
};

static std::vector<PassEntry> DeterministicPasses = {
    PASS_CREATOR(PromotePass(), 50, "mem2reg"),
    // TODO: randomize slp vectorizer
    PASS_CREATOR(SLPVectorizerPass(), 50, "slp-vectorizer"),
    PASS_CREATOR(SimplifyCFGPass(), 50, "simplify-cfg"),
    PASS_CREATOR(LoopVectorizePass(), 50, "loop-vectorizer"),
    PASS_CREATOR(SCCPPass(), 50, "sccp"),
    PASS_CREATOR(InstCombinePass(), 50, "instcombine"),
};

static std::vector<
    std::tuple<std::function<void(FunctionPassManager &)>, int, std::string>>
    PipelineGen = RandomizedPasses;

void Mutator::generateOrReadPipeline() {
  std::vector<std::string> Ret;
  if (!PipelineFile.empty()) {
    // Read from file.
    Ret = ReadPipeline(PipelineFile);
  }

  bool NeedToUpdate;
  if (Ret.empty()) {
    // Generate pipeline randomly.

    // Between 1 and MaxPasses.

    std::random_device RD;
    std::mt19937 Gen(RD());

    uint PassesNum = std::uniform_int_distribution<>(0, MaxPassesNum)(Gen);

    std::vector<int> Weights;
    for (auto [_, Weight, __] : PipelineGen)
      Weights.push_back(Weight);

    // Random Distribution of passes.
    std::discrete_distribution<> d(Weights.begin(), Weights.end());

    for (int i = 0; i < PassesNum; ++i) {
      auto index = d(Gen);
      auto [_, __, name] = PipelineGen[index];
      Ret.push_back(name);
    }
  }

  // Update old version of pipeline.
  if (!Ret.empty() && isDigit(Ret.front()[0])) {
    for (auto &Name : Ret) {
      int index = std::stoi(Name);
      auto [_, __, newName] = PipelineGen[index];
      Name = newName;
    }
    WritePipeline(PipelineFile, Ret);
  }

  Pipeline = Ret;

  for (auto Name : Ret)
    MODEBUG(errs() << Name << " ");
  MODEBUG(errs() << "\n");
}

void Mutator::storeConfig() {
  if (!PipelineFile.empty())
    WritePipeline(PipelineFile, Pipeline);
}

int Mutator::mutate(Module &M) {
  assert(!Pipeline.empty() && "");

  switch (PipelineType) {
  default:
  case 0:
    PipelineGen = RandomizedPasses;
    break;
  case 1:
    PipelineGen = DeoptimizePasses;
    break;
  case 2:
    PipelineGen = DeterministicPasses;
    break;
  }

  LoopAnalysisManager LAM;
  FunctionAnalysisManager FAM;
  CGSCCAnalysisManager CGAM;
  ModuleAnalysisManager MAM;

  PassBuilder PB;
  PB.registerModuleAnalyses(MAM);
  PB.registerCGSCCAnalyses(CGAM);
  PB.registerFunctionAnalyses(FAM);
  PB.registerLoopAnalyses(LAM);
  PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

  FunctionPassManager FPM;
  ModulePassManager MPM;

  int SequenceLen = Pipeline.size() - NumLastRemoved;
  if (SequenceLen <= 0)
    return 0;

  for (int i = 0; i < SequenceLen; i++) {
    auto CurName = Pipeline[i];

    if (PrintBefore == i)
      FPM.addPass(PrintFunctionPass());

    for (auto &[Builder, _, Name] : PipelineGen) {
      if (Name == CurName) {
        Builder(FPM);
        break;
      }
    }
  }

  MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
  MPM.run(M, MAM);
  return 0;
}
