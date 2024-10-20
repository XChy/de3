#include "CFGExpand.h"
#include "transforms/CFGExpand/CFGExpander.h"
#include "utils/Random.h"
#include "llvm/Analysis/TargetFolder.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Transforms/Utils/Local.h>

using namespace llvm;

void CFGExpandPass::printPipeline(
    raw_ostream &OS, function_ref<StringRef(StringRef)> MapClassName2PassName) {
  OS << "cfg-expand";
}

PreservedAnalyses CFGExpandPass::run(Function &F, FunctionAnalysisManager &AM) {

  auto &TLI = AM.getResult<TargetLibraryAnalysis>(F);
  auto &TTI = AM.getResult<TargetIRAnalysis>(F);
  // Combine begin from here.
  CFGExpander Expander;
  Expander.run(F);

  return PreservedAnalyses::none();
}
