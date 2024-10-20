#pragma once

#include "utils/Random.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PatternMatch.h>
#include <utils/Marcos.h>

using namespace llvm;
using namespace llvm::PatternMatch;

namespace llvm {

typedef llvm::function_ref<Value *(BinaryOperator *, Value *, Value *,
                                   IRBuilder<> &)>
    BinOpVisitor;

class ExprExpander {
public:
  ExprExpander(IRBuilder<> &Builder) : Builder(Builder) {}

  void run(Function &F);

  Value *expandInst(Instruction *I);
  Value *expandAddSub(BinaryOperator *I);
  Value *expandMulDivRem(BinaryOperator *I);
  Value *expandBitwise(BinaryOperator *I);
  Value *expandShift(BinaryOperator *I);
  Value *expandICmp(ICmpInst *I);

  void reorder(Instruction *I);

private:
  IRBuilder<> &Builder;
  RandomExecutor<BinOpVisitor> AddSubVisitors;
};

} // namespace llvm
