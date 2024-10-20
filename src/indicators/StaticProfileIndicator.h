#pragma once

#include "indicators/Indicator.h"
#include <llvm/ADT/DenseMap.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>

namespace llvm {
/*! \class StaticProfileIndicator
 *  \brief Brief class description
 *
 *  Compare the performance of IRs based on LLVM's static profiling technique
 *  like BranchProbabilityInfo
 */
class StaticProfileIndicator : public Indicator {
public:
  int64_t worth(Function &L, Function &R);

protected:
  DenseMap<BasicBlock *, uint64_t> BBCost;
};
} // namespace llvm
