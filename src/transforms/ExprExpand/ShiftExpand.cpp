#include "ExprExpander.h"
#include <llvm/IR/PatternMatch.h>

Value *visitShl(BinaryOperator *I, IRBuilder<> &Builder) {
  std::vector<BinOpVisitor> expanders;

  add_patten_binop(expanders, { return nullptr; });

  int index = choose(expanders.size());
  auto *LHS = I->getOperand(0);
  auto *RHS = I->getOperand(1);
  return whether() ? expanders[index](I, LHS, RHS, Builder) : nullptr;
}

Value *visitAShr(BinaryOperator *I, IRBuilder<> &Builder) {
  std::vector<BinOpVisitor> expanders;

  add_patten_binop(expanders, {
    // A a>> (BW - 1) --> SExt(A < 0)
    if (match(B, m_SpecificInt(A->getType()->getScalarSizeInBits() - 1))) {
      auto *Zero = Constant::getNullValue(A->getType());
      return Builder.CreateSExt(Builder.CreateICmpSLT(A, Zero), A->getType());
    }
    return nullptr;
  });

  int index = choose(expanders.size());
  auto *LHS = I->getOperand(0);
  auto *RHS = I->getOperand(1);
  return whether() ? expanders[index](I, LHS, RHS, Builder) : nullptr;
}

Value *visitLShr(BinaryOperator *I, IRBuilder<> &Builder) {
  std::vector<BinOpVisitor> expanders;

  add_patten_binop(expanders, {
    // A l>> (BW - 1) --> ZExt(A < 0)
    if (match(B, m_SpecificInt(A->getType()->getScalarSizeInBits() - 1))) {
      auto *Zero = Constant::getNullValue(A->getType());
      return Builder.CreateZExt(Builder.CreateICmpSLT(A, Zero), A->getType());
    }
    return nullptr;
  });

  int index = choose(expanders.size());
  auto *LHS = I->getOperand(0);
  auto *RHS = I->getOperand(1);
  return whether() ? expanders[index](I, LHS, RHS, Builder) : nullptr;
}

Value *ExprExpander::expandShift(BinaryOperator *I) {
  if (I->getOpcode() == Instruction::Shl)
    return visitShl(I, Builder);
  if (I->getOpcode() == Instruction::AShr)
    return visitAShr(I, Builder);
  if (I->getOpcode() == Instruction::LShr)
    return visitLShr(I, Builder);
  return nullptr;
}
