#pragma once

#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/PassManager.h>

#define DEBUG_TYPE "inst-random-combine"

namespace llvm {

class InstRandomCombinePass : public PassInfoMixin<InstRandomCombinePass> {

public:
  explicit InstRandomCombinePass() {}
  void printPipeline(raw_ostream &OS,
                     function_ref<StringRef(StringRef)> MapClassName2PassName);

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

};

} // namespace llvm
