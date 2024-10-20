#include "CFGExpander.h"
#include "utils/Marcos.h"
#include "utils/Random.h"
#include <llvm/ADT/STLExtras.h>
#include <llvm/ADT/ilist.h>
#include <llvm/Analysis/ValueTracking.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/PatternMatch.h>
#include <llvm/Support/Casting.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>

using namespace llvm;

void CFGExpander::run(Function &F) {
  for (auto &BB : F)
    if (whether())
      expandBB(&BB);
}

static bool sinkSelectOperand(Value *V) {
  auto *I = dyn_cast<Instruction>(V);
  // If it's safe to speculatively execute, then it should not have side
  // effects; therefore, it's safe to sink and possibly *not* execute.
  return I && I->hasOneUse() && isSafeToSpeculativelyExecute(I);
}

void unfoldSelectInst(SelectInst *SI) {
  // Transform a sequence like this:
  //    start:
  //       %cmp = cmp uge i32 %a, %b
  //       %sel = select i1 %cmp, i32 %c, i32 %d
  //
  // Into:
  //    start:
  //       %cmp = cmp uge i32 %a, %b
  //       %cmp.frozen = freeze %cmp
  //       br i1 %cmp.frozen, label %select.true, label %select.false
  //    select.true:
  //       br label %select.end
  //    select.false:
  //       br label %select.end
  //    select.end:
  //       %sel = phi i32 [ %c, %select.true ], [ %d, %select.false ]
  //
  // %cmp should be frozen, otherwise it may introduce undefined behavior.
  // In addition, we may sink instructions that produce %c or %d from
  // the entry block into the destination(s) of the new branch.
  // If the true or false blocks do not contain a sunken instruction, that
  // block and its branch may be optimized away. In that case, one side of the
  // first branch will point directly to select.end, and the corresponding PHI
  // predecessor block will be the start block.

  // Collect values that go on the true side and the values that go on the false
  // side.
  SmallVector<Instruction *> TrueInstrs, FalseInstrs;
  if (Value *V = SI->getTrueValue(); sinkSelectOperand(V))
    TrueInstrs.push_back(cast<Instruction>(V));
  if (Value *V = SI->getFalseValue(); sinkSelectOperand(V))
    FalseInstrs.push_back(cast<Instruction>(V));

  // Split the select block, according to how many (if any) values go on each
  // side.
  BasicBlock *StartBlock = SI->getParent();
  BasicBlock::iterator SplitPt = std::next(BasicBlock::iterator(SI));
  // We should split before any debug-info.
  SplitPt.setHeadBit(true);

  IRBuilder<> IB(SI);
  auto *CondFr = IB.CreateFreeze(SI->getCondition(), SI->getName() + ".frozen");

  BasicBlock *TrueBlock = nullptr;
  BasicBlock *FalseBlock = nullptr;
  BasicBlock *EndBlock = nullptr;
  BranchInst *TrueBranch = nullptr;
  BranchInst *FalseBranch = nullptr;
  if (TrueInstrs.size() == 0) {
    FalseBranch = cast<BranchInst>(SplitBlockAndInsertIfElse(
        CondFr, SplitPt, false, nullptr, nullptr, nullptr));
    FalseBlock = FalseBranch->getParent();
    EndBlock = cast<BasicBlock>(FalseBranch->getOperand(0));
  } else if (FalseInstrs.size() == 0) {
    TrueBranch = cast<BranchInst>(SplitBlockAndInsertIfThen(
        CondFr, SplitPt, false, nullptr, nullptr, nullptr));
    TrueBlock = TrueBranch->getParent();
    EndBlock = cast<BasicBlock>(TrueBranch->getOperand(0));
  } else {
    Instruction *ThenTerm = nullptr;
    Instruction *ElseTerm = nullptr;
    SplitBlockAndInsertIfThenElse(CondFr, SplitPt, &ThenTerm, &ElseTerm,
                                  nullptr, nullptr, nullptr);
    TrueBranch = cast<BranchInst>(ThenTerm);
    FalseBranch = cast<BranchInst>(ElseTerm);
    TrueBlock = TrueBranch->getParent();
    FalseBlock = FalseBranch->getParent();
    EndBlock = cast<BasicBlock>(TrueBranch->getOperand(0));
  }

  EndBlock->setName("select.end");
  if (TrueBlock)
    TrueBlock->setName("select.true.sink");
  if (FalseBlock)
    FalseBlock->setName(FalseInstrs.size() == 0 ? "select.false"
                                                : "select.false.sink");
  // Sink expensive instructions into the conditional blocks to avoid executing
  // them speculatively.
  for (Instruction *I : TrueInstrs)
    I->moveBefore(TrueBranch);
  for (Instruction *I : FalseInstrs)
    I->moveBefore(FalseBranch);

  // If we did not create a new block for one of the 'true' or 'false' paths
  // of the condition, it means that side of the branch goes to the end block
  // directly and the path originates from the start block from the point of
  // view of the new PHI.
  if (TrueBlock == nullptr)
    TrueBlock = StartBlock;
  else if (FalseBlock == nullptr)
    FalseBlock = StartBlock;

  // Use reverse iterator because later select may use the value of the
  // earlier select, and we need to propagate value through earlier select
  // to get the PHI operand.
  // The select itself is replaced with a PHI Node.
  PHINode *PN = PHINode::Create(SI->getType(), 2, "");
  PN->insertBefore(EndBlock->begin());
  PN->takeName(SI);
  PN->addIncoming(SI->getTrueValue(), TrueBlock);
  PN->addIncoming(SI->getFalseValue(), FalseBlock);
  PN->setDebugLoc(SI->getDebugLoc());

  SI->replaceAllUsesWith(PN);
  SI->eraseFromParent();
}

void expandUncondBB(BasicBlock *BB, BranchInst *BI) {
  Value *A, *B;
  std::vector<BBVisitor> expanders;
  IRBuilder<> Builder(BB->getContext());

  add_patten_bb(expanders, {
    for (auto &I : make_early_inc_range(*BB)) {
      if (auto SI = dyn_cast<SelectInst>(&I)) {
        if (choose(4) == 1) {
          unfoldSelectInst(SI);
          break;
        }
      }
    }
  });

  int index = choose(expanders.size());
  if (choose(6) == 1)
    expanders[index](BB, BI);
}

void expandCondBB(BasicBlock *BB, BranchInst *BI) {
  Value *A, *B;
  std::vector<BBVisitor> expanders;
  IRBuilder<> Builder(BB->getContext());

  add_patten_bb(expanders, {
    BasicBlock *SuccThen;
    BasicBlock *SuccElse;

    if (match(BI->getCondition(),
              m_Br(m_Or(m_Value(A), m_Value(B)), m_BasicBlock(SuccThen),
                   m_BasicBlock(SuccElse))) &&
        A->getType()->isIntegerTy(1)) {

      // TODO: handle those with phis
      if (!(SuccThen->phis().empty() && SuccElse->phis().empty()))
        return;

      BasicBlock *Split = BasicBlock::Create(BB->getContext());
      Split->insertInto(SuccThen->getParent());

      BI->setCondition(A);
      BI->setSuccessor(0, SuccThen);
      BI->setSuccessor(1, Split);

      Builder.SetInsertPoint(Split);
      Builder.CreateCondBr(B, SuccThen, SuccElse);
    }
  });

  add_patten_bb(expanders, {
    for (auto &I : make_early_inc_range(*BB)) {
      if (auto SI = dyn_cast<SelectInst>(&I)) {
        if (choose(4) == 1) {
          unfoldSelectInst(SI);
          break;
        }
      }
    }
  });

  int index = choose(expanders.size());
  if (whether())
    expanders[index](BB, BI);
}

void CFGExpander::expandBB(BasicBlock *BB) {
  if (auto BI = dyn_cast<BranchInst>(BB->getTerminator())) {
    if (BI->isUnconditional())
      expandUncondBB(BB, BI);
    else
      expandCondBB(BB, BI);
  }
}
