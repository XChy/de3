#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PatternMatch.h>
#include <memory>
#include <utils/Marcos.h>

using namespace llvm;
using namespace llvm::PatternMatch;

namespace llvm {
class CFGExpander {
public:
  void run(Function &F);
  void expandBB(BasicBlock *BB);

private:
};
} // namespace llvm
