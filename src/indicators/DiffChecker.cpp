#include "DiffChecker.h"
#include <llvm/ADT/MapVector.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instructions.h>

using namespace llvm;

typedef DenseMap<uint, uint> Opcode2Num;
typedef SmallVector<std::pair<uint, uint>, 4> Opcode2NumVec;

static Opcode2Num NumOp(Function &F) {
  Opcode2Num Ret;
  for (auto &I : instructions(F)) {
    if (Ret.contains(I.getOpcode()))
      Ret[I.getOpcode()]++;
    else
      Ret.insert({I.getOpcode(), 1});
  }
  return Ret;
}

int64_t DiffChecker::worth(Function &L, Function &R) {
  Opcode2Num LS = NumOp(L);
  Opcode2Num RS = NumOp(R);

  // Be deterministic
  MapVector<uint, uint> Ret;

  for (auto [Opcode, Num] : LS) {
    if (RS.contains(Opcode)) {
      if (LS[Opcode] > RS[Opcode])
        Ret.insert({Opcode, 1});
      else if (LS[Opcode] < RS[Opcode])
        Ret.insert({Opcode, -1});
    } else {
      Ret.insert({Opcode, 1});
    }
  }

  for (auto [Opcode, Num] : RS) {
    if (!LS.contains(Opcode))
      Ret.insert({Opcode, -1});
  }

  auto Diff = Ret.takeVector();

  SmallVector<Opcode2NumVec, 8> Uninteresting = {
      {{Instruction::TermOps::Br, 1}}, {{Instruction::TermOps::Br, -1}}};
  if (llvm::any_of(Uninteresting,
                   [&Diff](const Opcode2NumVec &Elt) { return Diff == Elt; })) {
    return -1;
  }

  return 1;
}
