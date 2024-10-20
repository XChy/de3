#include "ExprExpander.h"
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Operator.h>
#include <llvm/Support/Casting.h>
#include <utils/Random.h>

#define DEBUG_TYPE "instexpander"

using namespace llvm;

Value *ExprExpander::expandInst(Instruction *I) {
  LLVM_DEBUG(dbgs() << "Try to mutate:" << *I << '\n');
  if (auto *BinOp = dyn_cast<BinaryOperator>(I)) {
    switch (BinOp->getOpcode()) {
    case Instruction::Add:
    case Instruction::Sub:
      return expandAddSub(BinOp);
    case Instruction::Mul:
    case Instruction::UDiv:
    case Instruction::URem:
    case Instruction::SDiv:
    case Instruction::SRem:
      return expandMulDivRem(BinOp);
    case Instruction::Xor:
    case Instruction::And:
    case Instruction::Or:
      return expandBitwise(BinOp);
    case Instruction::Shl:
    case Instruction::AShr:
    case Instruction::LShr:
      return expandShift(BinOp);
    default:
      return nullptr;
    }
  }

  if (auto *ICmp = dyn_cast<ICmpInst>(I))
    return expandICmp(ICmp);

  return nullptr;
}
