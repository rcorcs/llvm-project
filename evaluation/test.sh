#!/bin/bash

function run() {
  
  #../build/release/bin/clang -Os $1.c -S -emit-llvm -o $1.ll
  #../build/release/bin/opt --func-cloning -funcspec-constants-only=true $1.ll -S -o $1.opt.ll
  ../build/release/bin/opt --func-cloning -funcspec-constants-only=false $1 -S -o $1.opt.ll
  ../build/release/bin/clang -Os -c $1 -o $1.o
  #../build/release/bin/clang -Os $1.ll -o $1.out
  ../build/release/bin/clang -Os -c $1.opt.ll -o $1.opt.o
  #../build/release/bin/clang -Os $1.opt.ll -o $1.opt.out
  ../build/release/bin/llvm-size $1.o
  ../build/release/bin/llvm-size $1.opt.o
  #echo "ORIGINAL"
  #./$1.out
  #echo "OPTIMIZED"
  #./$1.opt.out
}

#run test-0
#run test-1
#run test-2
#run test-3
#run test-4
#run test-5
#run test-6

#run multinode-1

run /mnt/d/f3m_exp/benchmarks/linux/_main_._all_._files_._linked_.bc
