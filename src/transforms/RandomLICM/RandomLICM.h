
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/IR/PassManager.h"

namespace llvm {

class LPMUpdater;
class Loop;
class LoopNest;


/// Performs Loop Invariant Code Motion Pass.
class RandomLICMPass : public PassInfoMixin<RandomLICMPass> {

public:
  PreservedAnalyses run(Loop &L, LoopAnalysisManager &AM,
                        LoopStandardAnalysisResults &AR, LPMUpdater &U);
};

/// Performs LoopNest Invariant Code Motion Pass.
class RandomLNICMPass : public PassInfoMixin<RandomLNICMPass> {

public:
  PreservedAnalyses run(LoopNest &L, LoopAnalysisManager &AM,
                        LoopStandardAnalysisResults &AR, LPMUpdater &U);
};
} // end namespace llvm
