#include "ExprExpander.h"
#include <llvm/IR/PatternMatch.h>

Value *visitAnd(BinaryOperator *I, IRBuilder<> &Builder) {
  std::vector<BinOpVisitor> expanders;

  add_patten_binop(expanders, {
    // A & B -> ~(~A | ~B)
    return Builder.CreateNot(
        Builder.CreateOr(Builder.CreateNot(A), Builder.CreateNot(B)));
  });
  add_patten_binop(expanders, {
    Value *C;
    // A & (B | C) -> (A & B) | (A & C)
    if (match(BinOp, m_And(m_Value(A), m_Or(m_Value(B), m_Value(C)))))
      return Builder.CreateOr(Builder.CreateAnd(A, B), Builder.CreateAnd(A, C));

    return nullptr;
  });
  add_patten_binop(expanders, {
    // (and A, B) --> (sub (add A, B) (or A, B))
    return Builder.CreateSub(Builder.CreateAdd(A, B), Builder.CreateOr(A, B));
  });

  add_patten_binop(expanders, {
    // (and A, B) --> (sub (or A, B) (xor A, B))
    return Builder.CreateSub(Builder.CreateOr(A, B), Builder.CreateXor(A, B));
  });

  add_patten_binop(expanders, {
    //  (~X & B) --> ((X | B) - X)
    Value *X;
    if (match(A, m_Not(m_Value(X))))
      return Builder.CreateSub(Builder.CreateOr(X, B), X);
    return nullptr;
  });

  // add_patten_binop(expanders, {
  ////  (A & ~Y) --> (A - (A & Y))
  // Value *Y;
  // if (match(B, m_Not(m_Value(Y))))
  // return Builder.CreateSub(A, Builder.CreateAnd(A, Y));
  // return nullptr;
  //});

  int index = choose(expanders.size());
  auto *LHS = I->getOperand(0);
  auto *RHS = I->getOperand(1);
  return whether() ? expanders[index](I, LHS, RHS, Builder) : nullptr;
}

Value *visitOr(BinaryOperator *I, IRBuilder<> &Builder) {
  std::vector<BinOpVisitor> expanders;

  add_patten_binop(expanders, {
    // A | B -> ~(~A & ~B)
    return Builder.CreateNot(
        Builder.CreateAnd(Builder.CreateNot(A), Builder.CreateNot(B)));
  });

  add_patten_binop(expanders, {
    Value *C;
    // A | (B & C) -> (A | B) & (A | C)
    if (match(BinOp, m_Or(m_Value(A), m_And(m_Value(B), m_Value(C)))))
      return Builder.CreateAnd(Builder.CreateOr(A, B), Builder.CreateOr(A, C));

    return nullptr;
  });

  add_patten_binop(expanders, {
    // (or A, B) --> (sub (add A, B) (and A, B))
    return Builder.CreateSub(Builder.CreateAdd(A, B), Builder.CreateAnd(A, B));
  });

  add_patten_binop(expanders, {
    // (or disjoint A, B) --> (add nsw nuw A, B)
    if (match(BinOp, m_DisjointOr(m_Value(), m_Value())))
      return Builder.CreateAdd(A, B, BinOp->getName(), true, true);
    return nullptr;
  });

  add_patten_binop(expanders, {
    // (or disjoint A, B) --> (xor A, B)
    if (match(BinOp, m_DisjointOr(m_Value(), m_Value())))
      return Builder.CreateXor(A, B);
    return nullptr;
  });

  int index = choose(expanders.size());
  auto *LHS = I->getOperand(0);
  auto *RHS = I->getOperand(1);
  return whether() ? expanders[index](I, LHS, RHS, Builder) : nullptr;
}

Value *visitXor(BinaryOperator *I, IRBuilder<> &Builder) {
  std::vector<BinOpVisitor> expanders;

  add_patten_binop(expanders, {
    // A ^ B -> (!A & B) | (A & !B)
    return Builder.CreateOr(Builder.CreateAnd(Builder.CreateNot(A), B),
                            Builder.CreateAnd(A, Builder.CreateNot(B)));
  });

  add_patten_binop(expanders, {
    // (xor A, B) --> (sub (or A, B) (and A, B))
    return Builder.CreateSub(Builder.CreateOr(A, B), Builder.CreateAnd(A, B));
  });

  int index = choose(expanders.size());
  auto *LHS = I->getOperand(0);
  auto *RHS = I->getOperand(1);
  return whether() ? expanders[index](I, LHS, RHS, Builder) : nullptr;
}

Value *ExprExpander::expandBitwise(BinaryOperator *I) {
  if (I->getOpcode() == Instruction::And)
    return visitAnd(I, Builder);
  if (I->getOpcode() == Instruction::Or)
    return visitOr(I, Builder);
  if (I->getOpcode() == Instruction::Xor)
    return visitXor(I, Builder);
  return nullptr;
}
