LLVM_PATH=/home/rodrigo/tmp/build/bin/

OPT="-O3 -march=native -mtune=skylake"
#OPT="-O3 -march=native"
#OPT="-O3"

${LLVM_PATH}clang $1 ${OPT} -fno-vectorize -fslp-vectorize -funroll-loops -fno-inline-functions -mllvm -slp-unroll-loops=false -emit-llvm -S -o $(basename $1).unroll-slp.ll
${LLVM_PATH}clang $1 ${OPT} -fno-vectorize -fslp-vectorize -fno-unroll-loops -fno-inline-functions -mllvm -slp-unroll-loops=false -emit-llvm -S -o $(basename $1).slp.ll
${LLVM_PATH}clang $1 ${OPT} -fno-vectorize -fslp-vectorize -fno-unroll-loops -fno-inline-functions -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true -emit-llvm -S -o $(basename $1).luslp.ll
${LLVM_PATH}clang $1 ${OPT} -fvectorize -fno-slp-vectorize -fno-unroll-loops -fno-inline-functions -emit-llvm -S -o $(basename $1).vec.ll


${LLVM_PATH}clang dummy.c $1 ${OPT} -fno-vectorize -fno-slp-vectorize -fno-unroll-loops -fno-inline-functions -o $(basename $1).baseline
${LLVM_PATH}clang dummy.c $1 ${OPT} -fno-vectorize -fslp-vectorize -funroll-loops -fno-inline-functions -mllvm -slp-unroll-loops=false -o $(basename $1).unroll-slp
${LLVM_PATH}clang dummy.c $1 ${OPT} -fno-vectorize -fslp-vectorize -fno-unroll-loops -fno-inline-functions -mllvm -slp-unroll-loops=false -o $(basename $1).slp
${LLVM_PATH}clang dummy.c $1 ${OPT} -fno-vectorize -fslp-vectorize -fno-unroll-loops -fno-inline-functions -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true -o $(basename $1).luslp
${LLVM_PATH}clang dummy.c $1 ${OPT} -fvectorize -fno-slp-vectorize -fno-unroll-loops -fno-inline-functions -o $(basename $1).vec

