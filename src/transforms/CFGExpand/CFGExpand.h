#pragma once

#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/PassManager.h>

#define DEBUG_TYPE "cfg-expand"

namespace llvm {

class CFGExpandPass : public PassInfoMixin<CFGExpandPass> {

public:
  explicit CFGExpandPass() {}
  void printPipeline(raw_ostream &OS,
                     function_ref<StringRef(StringRef)> MapClassName2PassName);

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm
