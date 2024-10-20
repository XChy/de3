#pragma once
#include <llvm/IR/Module.h>

using namespace llvm;

class DiffFeature {
public:
  virtual bool PutDiff(Function &L, Function &R, std::ostream &Out) = 0;
  virtual ~DiffFeature() = default;
};
