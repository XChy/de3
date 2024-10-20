#include "StaticProfileIndicator.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include <llvm/Analysis/AssumptionCache.h>
#include <llvm/Analysis/BlockFrequencyInfo.h>
#include <llvm/Analysis/CodeMetrics.h>
#include <llvm/Analysis/TargetLibraryInfo.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Value.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/Casting.h>
#include <utils/Debug.h>
using namespace llvm;

constexpr uint32_t CostBitwidth = 256;

static uint64_t costOfBB(BasicBlock &BB, TargetTransformInfo &TTI,
                         const SmallPtrSetImpl<const Value *> &EphValues) {
  uint64_t CostVal = 0;
  auto CostKind = TargetTransformInfo::TCK_SizeAndLatency;
  bool TypeBasedIntrinsicCost = true;

  for (Instruction &Inst : BB) {
    InstructionCost Cost;
    auto *II = dyn_cast<IntrinsicInst>(&Inst);
    BranchInst *BI = dyn_cast<BranchInst>(&Inst);
    SwitchInst *SI = dyn_cast<SwitchInst>(&Inst);

    // Don't take the cost of ephemeral instructions into account.
    if (EphValues.contains(&Inst))
      continue;

    if (II && TypeBasedIntrinsicCost) {
      IntrinsicCostAttributes ICA(II->getIntrinsicID(), *II,
                                  InstructionCost::getInvalid(), true);
      Cost = TTI.getIntrinsicInstrCost(ICA, CostKind);
    } else if (BI && BI->isConditional()) {
      Cost = 2;
    } else if (SI) {
      Cost = SI->getNumCases() * 3;
    } else if (isa<SelectInst>(&Inst)) {
      Cost = 2;
    } else {
      Cost = TTI.getInstructionCost(&Inst, CostKind);
    }

    CostVal += Cost.getValue().value_or(0);
  }

  return CostVal;
}

static APInt cost(Function &F, APInt &EntryFreq) {
  PassBuilder PB;
  FunctionAnalysisManager FM;
  APInt TotalCost(CostBitwidth, 0);
  SmallPtrSet<const Value *, 32> EphValues;

  PB.registerFunctionAnalyses(FM);
  auto &TTI = FM.getResult<TargetIRAnalysis>(F);
  auto &BFI = FM.getResult<BlockFrequencyAnalysis>(F);
  auto &AC = FM.getResult<AssumptionAnalysis>(F);

  CodeMetrics::collectEphemeralValues(&F, &AC, EphValues);

  EntryFreq = BFI.getEntryFreq().getFrequency();

  for (auto &BB : F) {
    APInt Freq(CostBitwidth, BFI.getBlockFreq(&BB).getFrequency());
    APInt Cost(CostBitwidth, costOfBB(BB, TTI, EphValues));
    MODEBUG(dbgs() << "[Cost] " << BB.getName() << " : " << Cost * Freq
                   << "\n");
    TotalCost += Freq * Cost;
  }

  return TotalCost;
}

int64_t StaticProfileIndicator::worth(Function &L, Function &R) {
  APInt LEntryFreq(CostBitwidth, 0);
  APInt REntryFreq(CostBitwidth, 0);
  APInt LTotal = cost(L, LEntryFreq);
  APInt RTotal = cost(R, REntryFreq);

  APInt GCD = APIntOps::GreatestCommonDivisor(LEntryFreq, REntryFreq);
  LTotal *= REntryFreq.udiv(GCD);
  RTotal *= LEntryFreq.udiv(GCD);

  MODEBUG(dbgs() << "[Total] " << LTotal << " : " << RTotal << "\n");

  if (LTotal == RTotal)
    return 0;
  else if (LTotal.ult(RTotal))
    return 1;
  else
    return -1;
}
