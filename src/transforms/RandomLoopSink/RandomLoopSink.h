#pragma once

#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/PassManager.h>

#define DEBUG_TYPE "random-mem2reg"

namespace llvm {

class RandomLoopSinkPass : public PassInfoMixin<RandomLoopSinkPass> {

public:
  explicit RandomLoopSinkPass() {}
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm
