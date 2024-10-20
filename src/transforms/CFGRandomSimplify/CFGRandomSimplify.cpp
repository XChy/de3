#include "CFGRandomSimplify.h"
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

void CFGRandomSimplifyPass::printPipeline(
    raw_ostream &OS, function_ref<StringRef(StringRef)> MapClassName2PassName) {
}

PreservedAnalyses CFGRandomSimplifyPass::run(Function &F,
                                             FunctionAnalysisManager &AM) {

  auto &TLI = AM.getResult<TargetLibraryAnalysis>(F);
  auto &TTI = AM.getResult<TargetIRAnalysis>(F);
  // Combine begin from here.
  for (BasicBlock &BB : make_early_inc_range(F)) {
    if (whether())
      simplifyCFG(&BB, TTI);
  }

  return PreservedAnalyses::none();
}
