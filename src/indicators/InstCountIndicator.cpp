#include "InstCountIndicator.h"
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Value.h>
#include <llvm/Passes/PassBuilder.h>
using namespace llvm;

static int64_t instcount(Function &L) { return range_size(instructions(L)); }

int64_t InstCountIndicator::worth(Function &L, Function &R) {
  return instcount(R) - instcount(L);
}
