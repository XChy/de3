#!/usr/bin/bash
clang -O3 -Xclang -disable-llvm-optzns -fno-discard-value-names -emit-llvm -S $1 -o $2
