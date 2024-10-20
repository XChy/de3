#pragma once

#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/PassManager.h>

#define DEBUG_TYPE "cfg-random-simplify"

namespace llvm {

class CFGRandomSimplifyPass : public PassInfoMixin<CFGRandomSimplifyPass> {

public:
  explicit CFGRandomSimplifyPass() {}
  void printPipeline(raw_ostream &OS,
                     function_ref<StringRef(StringRef)> MapClassName2PassName);

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm
