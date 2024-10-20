#pragma once

#include "Random.h"
#include <llvm/ADT/FunctionExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

using namespace llvm;

typedef llvm::function_ref<Value *(Instruction *, IRBuilder<> &)> InstVisitor;

typedef llvm::function_ref<Value *(BinaryOperator *, Value *, Value *,
                                   IRBuilder<> &)>
    BinOpVisitor;

typedef llvm::function_ref<void(BasicBlock *, BranchInst *)> BBVisitor;

#define randif(x) if ((x) && whether())

#define add_patten_I(expanders, code)                                          \
  expanders.push_back(                                                         \
      [](Instruction *I, IRBuilder<> &Builder) -> Instruction * { code })

#define add_patten_binop(expanders, code)                                      \
  expanders.push_back([](BinaryOperator *BinOp, Value *A, Value *B,            \
                         IRBuilder<> &Builder) -> Value * { code })

#define add_patten_icmp(expanders, code)                                       \
  expanders.push_back([](ICmpInst *I, Value *A, Value *B,                      \
                         IRBuilder<> &Builder) -> Value * { code })

#define add_patten_bb(expanders, code)                                         \
  expanders.push_back([&](BasicBlock *BB, BranchInst *BI) -> void { code })
