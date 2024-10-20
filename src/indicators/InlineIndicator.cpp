#include "InlineIndicator.h"
#include <llvm/ADT/SmallVector.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/IntrinsicInst.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Value.h>
#include <llvm/Passes/PassBuilder.h>
using namespace llvm;

static void FunctionNames(Function &F, DenseSet<StringRef> &Names) {
  for (auto &I : instructions(F)) {
    if (auto CI = dyn_cast<CallBase>(&I))
      if (!isa<IntrinsicInst>(&I))
        if (auto Callee = CI->getCalledFunction())
          Names.insert(Callee->getName());
  }
}

int64_t InlineIndicator::worth(Function &L, Function &R) {
  DenseSet<StringRef> LNames;
  DenseSet<StringRef> RNames;
  FunctionNames(L, LNames);
  FunctionNames(R, RNames);

  if (LNames == RNames)
    return 1;

  return 0;
}
