#include "InstFeature.h"
#include <fstream>
#include <llvm/ADT/MapVector.h>
#include <llvm/IR/InstIterator.h>

using namespace llvm;

typedef DenseMap<uint, uint> Opcode2Num;

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

bool InstFeature::PutDiff(Function &L, Function &R, std::ostream &Out) {
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

  for (auto [Opcode, Sign] : Ret)
    Out << Opcode << " " << Sign << "\n";

  return !Ret.empty();
}
