#include "ExprExpander.h"
#include <llvm/IR/Instructions.h>

Value *ExprExpander::expandICmp(ICmpInst *I) {
  std::vector<
      function_ref<Value *(ICmpInst *, Value *, Value *, IRBuilder<> &)>>
      expanders;

  add_patten_icmp(expanders, {
    // A == B -> A - B == 0
    // A != B -> A - B != 0
    if (!A->getType()->isIntOrIntVectorTy())
      return nullptr;

    if (I->getPredicate() == ICmpInst::ICMP_EQ)
      return Builder.CreateICmpEQ(Builder.CreateSub(A, B),
                                  Constant::getNullValue(A->getType()));

    if (I->getPredicate() == ICmpInst::ICMP_NE)
      return Builder.CreateICmpNE(Builder.CreateSub(A, B),
                                  Constant::getNullValue(A->getType()));
    return nullptr;
  });

  int index = choose(expanders.size());
  auto *LHS = I->getOperand(0);
  auto *RHS = I->getOperand(1);
  return whether() ? expanders[index](I, LHS, RHS, Builder) : nullptr;
}
