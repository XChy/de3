#pragma once

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Debug.h>

namespace llvm {

extern cl::opt<bool> MODebug;

#define MODEBUG(X)                                                             \
  do {                                                                         \
    if (MODebug) {                                                             \
      X;                                                                       \
    }                                                                          \
  } while (false)

} // namespace llvm
