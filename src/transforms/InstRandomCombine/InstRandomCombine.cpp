#include "InstRandomCombine.h"
#include "utils/Random.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/OptimizationRemarkEmitter.h"
#include "llvm/Analysis/ProfileSummaryInfo.h"
#include "llvm/Analysis/TargetFolder.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include <llvm/Analysis/InstructionSimplify.h>
#include <llvm/Analysis/SimplifyQuery.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Transforms/Utils/Local.h>

using namespace llvm;

void InstRandomCombinePass::printPipeline(
    raw_ostream &OS, function_ref<StringRef(StringRef)> MapClassName2PassName) {
}

static void randomCombine(Function &F) {
  for (auto &BB : F) {
    for (auto &Inst : BB) {
      auto *I = &Inst;

      if (whether()) {
        SimplifyQuery Q(F.getParent()->getDataLayout(), I);
        if (Value *V = simplifyInstruction(I, Q))
          I->replaceAllUsesWith(V);
      }
    }
  }
}

PreservedAnalyses InstRandomCombinePass::run(Function &F,
                                             FunctionAnalysisManager &AM) {

  randomCombine(F);
  PreservedAnalyses PA;
  PA.preserveSet<CFGAnalyses>();
  return PA;
}
