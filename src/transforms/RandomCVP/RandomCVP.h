#pragma once
#include "llvm/IR/PassManager.h"

namespace llvm {

class Function;

struct RandomCVPPass
    : PassInfoMixin<RandomCVPPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // end namespace llvm
