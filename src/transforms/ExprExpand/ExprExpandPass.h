#pragma once

#include <llvm/IR/Function.h>
#include <llvm/IR/PassManager.h>

#define DEBUG_TYPE "instcombine"

namespace llvm {

class ExprExpandPass : public PassInfoMixin<ExprExpandPass> {

public:
  explicit ExprExpandPass() {}
  void printPipeline(raw_ostream &OS,
                     function_ref<StringRef(StringRef)> MapClassName2PassName);

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm
