#include "UBChecker.h"
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/PatternMatch.h>
#include <llvm/IR/Value.h>
#include <llvm/Passes/PassBuilder.h>
using namespace llvm;
using namespace llvm::PatternMatch;

static bool hasObivousUB(Function &F) {
  for (auto &I : instructions(F)) {

    if (match(&I, m_Store(m_Value(), m_Zero())))
      return true;

    if (match(&I, m_Load(m_Zero())))
      return true;

    if (CallBase *CB = dyn_cast<CallBase>(&I))
      if (match(CB->getCalledOperand(), m_Zero()))
        return true;

    for (Use &U : I.operands())
      if (match(U.get(), m_Undef()))
        return true;
  }
  return false;
}

int64_t UBChecker::worth(Function &L, Function &R) {
  if (!hasObivousUB(L) && !hasObivousUB(R))
    return 1;
  else
    return -1;
}
