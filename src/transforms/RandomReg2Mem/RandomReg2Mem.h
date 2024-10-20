#pragma once

#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/PassManager.h>

#define DEBUG_TYPE "random-reg2mem"

namespace llvm {

class RandomRegToMemPass : public PassInfoMixin<RandomRegToMemPass> {

public:
  explicit RandomRegToMemPass() {}
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm
