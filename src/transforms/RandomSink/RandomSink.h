#pragma once
#include "llvm/IR/PassManager.h"

namespace llvm {

class Function;

/// Move instructions into successor blocks when possible.
class RandomSinkPass : public PassInfoMixin<RandomSinkPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};
} // namespace llvm
