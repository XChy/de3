#include "LoopIndicator.h"
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/PassBuilder.h>
using namespace llvm;

static int64_t loopNumOf(Function &L) {
  PassBuilder PB;
  FunctionAnalysisManager FM;
  PB.registerFunctionAnalyses(FM);
  auto &analysis = FM.getResult<LoopAnalysis>(L);
  return range_size(analysis);
}

int64_t LoopIndicator::worth(Function &L, Function &R) {
  return loopNumOf(R) - loopNumOf(L);
}
