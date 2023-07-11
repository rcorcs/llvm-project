#!/bin/bash

DIR=$(dirname $(readlink -f $0))
echo $DIR
source $DIR/config

${LLVMDIR}clang $1 -Os -emit-llvm -S -o $1.ll -fno-vectorize -fno-slp-vectorize 
#../release/bin/opt -loop-reroll  $1.ll -o $1.bl.ll -S
echo "LoopRolling"
${LLVMDIR}opt -loop-rolling -loop-rolling-extensions=true $1.ll -o $1.opt.ll -S
${LLVMDIR}clang $1.ll -Os -c -o $1.o -fno-vectorize -fno-slp-vectorize
#../release/bin/clang $1.bl.ll -Os -c -o $1.bl.o -fno-vectorize -fno-slp-vectorize
${LLVMDIR}clang $1.opt.ll -Os -c -o $1.opt.o -fno-vectorize -fno-slp-vectorize
${LLVMDIR}llvm-size $1.o
${LLVMDIR}llvm-size $1.opt.o
