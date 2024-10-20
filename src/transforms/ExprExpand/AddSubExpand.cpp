#include "ExprExpander.h"
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Instruction.h>

using namespace llvm;

static Value *visitAdd(BinaryOperator *I, IRBuilder<> &Builder) {
  std::vector<BinOpVisitor> expanders;

  add_patten_binop(expanders,
                   // A + B -> A - (-B)
                   return Builder.CreateSub(A, Builder.CreateNeg(B)););
  add_patten_binop(expanders,
                   // A + B -> A - (-B)
                   return Builder.CreateSub(A, Builder.CreateNeg(B)););

  int index = choose(expanders.size());
  auto *LHS = I->getOperand(0);
  auto *RHS = I->getOperand(1);
  return (whether() ? expanders[index](I, LHS, RHS, Builder) : nullptr);
}

static Value *visitSub(BinaryOperator *I, IRBuilder<> &Builder) {
  std::vector<BinOpVisitor> expanders;

  add_patten_binop(expanders,
                   // A - B -> A + (-B)
                   return Builder.CreateAdd(A, Builder.CreateNeg(B)););
  add_patten_binop(
      expanders,
      // A - B -> ~B - ~A
      return Builder.CreateSub(Builder.CreateNot(B), Builder.CreateNot(A)););

  int index = choose(expanders.size());
  auto *LHS = I->getOperand(0);
  auto *RHS = I->getOperand(1);
  return (whether() ? expanders[index](I, LHS, RHS, Builder) : nullptr);
}

Value *ExprExpander::expandAddSub(BinaryOperator *I) {
  if (I->getOpcode() == Instruction::Add)
    return visitAdd(I, Builder);
  if (I->getOpcode() == Instruction::Sub)
    return visitSub(I, Builder);
  return nullptr;
}
