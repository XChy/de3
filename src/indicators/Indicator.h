#pragma once

#include <llvm/IR/Module.h>
namespace llvm {

class Indicator {
public:
  // If check(L, R) > 0, it it worthwhile to transform L to R.
  virtual int64_t worth(Function &L, Function &R) = 0;
  virtual ~Indicator() = default;
};
} // namespace llvm
