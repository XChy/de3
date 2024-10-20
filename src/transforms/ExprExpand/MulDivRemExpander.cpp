#include "ExprExpander.h"

using namespace llvm;

static Value *visitMul(BinaryOperator *I, IRBuilder<> &Builder) {
  std::vector<BinOpVisitor> expanders;

  add_patten_binop(expanders, {
    // A * (B + C) -> A * B + A * C
    Value *C;
    if (match(BinOp, m_c_Mul(m_Value(A), m_Add(m_Value(B), m_Value(C)))))
      return Builder.CreateAdd(Builder.CreateMul(A, B),
                               Builder.CreateMul(A, C));
    return nullptr;
  });

  int index = choose(expanders.size());
  auto *LHS = I->getOperand(0);
  auto *RHS = I->getOperand(1);
  return whether() ? expanders[index](I, LHS, RHS, Builder) : nullptr;
}

static Value *visitUDiv(BinaryOperator *I, IRBuilder<> &Builder) {
  std::vector<BinOpVisitor> expanders;

  add_patten_binop(expanders, { return nullptr; });

  int index = choose(expanders.size());
  auto *LHS = I->getOperand(0);
  auto *RHS = I->getOperand(1);
  return whether() ? expanders[index](I, LHS, RHS, Builder) : nullptr;
}

static Value *visitURem(BinaryOperator *I, IRBuilder<> &Builder) {
  std::vector<BinOpVisitor> expanders;

  add_patten_binop(expanders, {
    // A u% B --> A - B * (A / B)
    return Builder.CreateSub(A, Builder.CreateMul(B, Builder.CreateUDiv(A, B)));
  });

  int index = choose(expanders.size());
  auto *LHS = I->getOperand(0);
  auto *RHS = I->getOperand(1);
  return whether() ? expanders[index](I, LHS, RHS, Builder) : nullptr;
}

Value *ExprExpander::expandMulDivRem(BinaryOperator *I) {
  if (I->getOpcode() == Instruction::Mul)
    return visitMul(I, Builder);
  if (I->getOpcode() == Instruction::UDiv)
    return visitUDiv(I, Builder);
  if (I->getOpcode() == Instruction::URem)
    return visitURem(I, Builder);
  return nullptr;
}
