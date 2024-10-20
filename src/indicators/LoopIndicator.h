#pragma once

#include "indicators/Indicator.h"
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>

namespace llvm {

class LoopIndicator : public Indicator {
public:
  int64_t worth(Function &L, Function &R);
};
} // namespace llvm
