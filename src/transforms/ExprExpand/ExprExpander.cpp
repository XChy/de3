#include "ExprExpander.h"
#include "utils/Random.h"
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/PatternMatch.h>
#include <llvm/Transforms/Scalar/Reassociate.h>

using namespace llvm;
using namespace llvm::PatternMatch;

void ExprExpander::reorder(Instruction *I) {
  if (auto *BinOp = dyn_cast<BinaryOperator>(I)) {
    Value *LHS, *RHS;

    Value *A, *B, *C;
    BinaryOperator *SubI;
    Instruction::BinaryOps Opcode = BinOp->getOpcode();

    // Randomly reassciate expressions (A op B) op C or A op (B op C)
    if (whether() && BinOp->isAssociative() &&
        match(BinOp, m_BinOp(m_CombineAnd(m_BinOp(m_Value(A), m_Value(B)),
                                          m_BinOp(SubI)),
                             m_Value(C))) &&
        SubI->getOpcode() == Opcode) {

      auto reordered =
          whether() ? Builder.CreateBinOp(Opcode, A,
                                          Builder.CreateBinOp(Opcode, B, C))
                    : Builder.CreateBinOp(Opcode,
                                          Builder.CreateBinOp(Opcode, A, B), C);

      I->replaceAllUsesWith(reordered);
    }

    if (whether() && BinOp->isCommutative())
      BinOp->swapOperands();
  }
}

void ExprExpander::run(Function &F) {
  // TODO: Maybe we can expand an expression multiple times?
  int MaxIteration = 1;

  for (auto &BB : F) {
    for (auto &Inst : BB) {
      Builder.SetInsertPoint(&Inst);

      if (auto *Ret = expandInst(&Inst))
        Inst.replaceAllUsesWith(Ret);
    }

    for (auto &Inst : BB) {
      Builder.SetInsertPoint(&Inst);
      reorder(&Inst);
    }

  }
}
