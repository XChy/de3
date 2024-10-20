#pragma once

#include "llvm/IR/PassManager.h"

namespace llvm {

class Function;

/// This class implements a trivial dead store elimination. We consider
/// only the redundant stores that are local to a single Basic Block.
class RandomDSEPass : public PassInfoMixin<RandomDSEPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);
};

} // end namespace llvm
