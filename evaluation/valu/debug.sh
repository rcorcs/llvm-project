LLVM_PATH=/home/rodrigo/tmp/build/bin/

OPT="-O3 -march=skylake -mtune=skylake"

${LLVM_PATH}clang $1  -mllvm -unroll-allow-partial ${OPT} -fno-vectorize -fslp-vectorize -funroll-loops -fno-inline-functions -mllvm -slp-unroll-loops=false -mllvm -debug -emit-llvm -S -o $(basename $1).unroll-slp.ll
