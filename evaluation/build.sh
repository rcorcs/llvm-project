#!/bin/bash

function run() {
  
  ../build/release/bin/clang -O1 $1.c -S -emit-llvm -o $1.ll
  ../build/release/bin/opt --func-cloning $1.ll -S -o $1.opt.ll
  ../build/release/bin/clang -Os -c $1.ll -o $1.o
  ../build/release/bin/clang -Os -c $1.opt.ll -o $1.opt.o
  ../build/release/bin/llvm-size $1.o
  ../build/release/bin/llvm-size $1.opt.o
}

#run test-1
#run test-2
run test-3


