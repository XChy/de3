#include "RandomReg2Mem.h"
#include "utils/Random.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <llvm/ADT/STLExtras.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Transforms/Utils/Local.h>

using namespace llvm;

static bool valueEscapes(const Instruction &Inst) {
  if (!Inst.getType()->isSized())
    return false;

  const BasicBlock *BB = Inst.getParent();
  for (const User *U : Inst.users()) {
    const Instruction *UI = cast<Instruction>(U);
    if (UI->getParent() != BB || isa<PHINode>(UI))
      return true;
  }
  return false;
}

static bool runPass(Function &F) {
  // Insert all new allocas into entry block.
  BasicBlock *BBEntry = &F.getEntryBlock();
  assert(pred_empty(BBEntry) &&
         "Entry block to function must not have predecessors!");

  // Find first non-alloca instruction and create insertion point. This is
  // safe if block is well-formed: it always have terminator, otherwise
  // we'll get and assertion.
  BasicBlock::iterator I = BBEntry->begin();
  while (isa<AllocaInst>(I))
    ++I;

  CastInst *AllocaInsertionPoint = new BitCastInst(
      Constant::getNullValue(Type::getInt32Ty(F.getContext())),
      Type::getInt32Ty(F.getContext()), "reg2mem alloca point", &*I);

  // Find the escaped instructions. But don't create stack slots for
  // allocas in entry block.
  std::list<Instruction *> WorkList;
  for (Instruction &I : instructions(F))
    if (!(isa<AllocaInst>(I) && I.getParent() == BBEntry) && valueEscapes(I))
      WorkList.push_front(&I);

  // Demote escaped instructions
  for (Instruction *I : WorkList)
    if (whether())
      DemoteRegToStack(*I, false, AllocaInsertionPoint->getIterator());

  WorkList.clear();

  // Find all phi's
  for (BasicBlock &BB : F)
    for (auto &Phi : BB.phis())
      WorkList.push_front(&Phi);

  // Demote phi nodes
  for (Instruction *I : WorkList)
    if (whether())
      DemotePHIToStack(cast<PHINode>(I), AllocaInsertionPoint->getIterator());

  return true;
}

PreservedAnalyses RandomRegToMemPass::run(Function &F,
                                          FunctionAnalysisManager &AM) {
  auto *DT = &AM.getResult<DominatorTreeAnalysis>(F);
  auto *LI = &AM.getResult<LoopAnalysis>(F);
  unsigned N = SplitAllCriticalEdges(F, CriticalEdgeSplittingOptions(DT, LI));
  bool Changed = runPass(F);
  if (N == 0 && !Changed)
    return PreservedAnalyses::all();
  PreservedAnalyses PA;
  PA.preserve<DominatorTreeAnalysis>();
  PA.preserve<LoopAnalysis>();
  return PA;
}
