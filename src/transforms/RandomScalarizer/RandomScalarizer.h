#pragma once

#include "llvm/IR/PassManager.h"
#include "llvm/Transforms/Scalar/Scalarizer.h"
#include <optional>

namespace llvm {

class Function;
class FunctionPass;

class RandomScalarizerPass : public PassInfoMixin<RandomScalarizerPass> {
  ScalarizerPassOptions Options;

public:
  RandomScalarizerPass() = default;
  RandomScalarizerPass(const ScalarizerPassOptions &Options)
      : Options(Options) {}

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

  void setScalarizeVariableInsertExtract(bool Value) {
    Options.ScalarizeVariableInsertExtract = Value;
  }
  void setScalarizeLoadStore(bool Value) { Options.ScalarizeLoadStore = Value; }
  void setScalarizeMinBits(unsigned Value) { Options.ScalarizeMinBits = Value; }
};

} // namespace llvm
