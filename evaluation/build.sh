#!/bin/bash

OPT="-Os -fno-vectorize -fno-slp-vectorize -fno-unroll-loops -fno-inline-functions"
function run() {
  
  ../build/release/bin/clang $OPT  -isysroot $(xcrun --show-sdk-path) -B /usr/bin $1.c -S -emit-llvm -o $1.ll
  #../build/release/bin/opt --func-cloning -funcspec-constants-only=true $1.ll -S -o $1.opt.ll
  ../build/release/bin/opt -passes=func-cloning -fe-constants-only=false -fe-profit-threshold=-1 $1.ll -S -o $1.opt.ll
  ../build/release/bin/clang  $OPT -isysroot $(xcrun --show-sdk-path) -B /usr/bin -c $1.ll -o $1.o
  #../build/release/bin/clang  $OPT -isysroot $(xcrun --show-sdk-path) -B /usr/bin $1.ll -o $1.out
  ../build/release/bin/clang  $OPT -isysroot $(xcrun --show-sdk-path) -B /usr/bin -c $1.opt.ll -o $1.opt.o
  ../build/release/bin/clang  $OPT -isysroot $(xcrun --show-sdk-path) -B /usr/bin $1.opt.ll -o $1.opt.out
  ../build/release/bin/llvm-size $1.o
  ../build/release/bin/llvm-size $1.opt.o
  #echo "ORIGINAL"
  #./$1.out
  #echo "OPTIMIZED"
  #./$1.opt.out
}

#run test-0
#run test-1
run test-2
#run test-3
#run test-4
#run test-5
#run test-6

#run multinode-1
#run legality-1
#run legality-2
