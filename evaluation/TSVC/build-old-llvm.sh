LLVM_PATH=/home/rodrigo/tmp/build/bin/

#OPT="-O3 -march=skylake -mtune=skylake"
OPT="-O3 -march=native -mtune=native -ffast-math"
#OPT="-O3"
CFLAG=-lm
#SLPSETTINGS="-mllvm -slp-min-tree-size=2 -mllvm -slp-vectorize-hor-store=true -mllvm -slp-vectorize-hor=true"
SLPSETTINGS="-mllvm -slp-vectorize-hor-store=true -mllvm -slp-vectorize-hor=true"

#EXT="${1##*.}"
#if [ $EXT == "c" ]; then
CC=${LLVM_PATH}clang
#else
# CC=${LLVM_PATH}clang++
#fi


${CC} tsc.c ${OPT} -fno-vectorize -fslp-vectorize -fno-unroll-loops -fno-inline-functions -mllvm -slp-unroll-loops=false ${SLPSETTINGS} -emit-llvm -S -o $(basename $1).slp.ll #1>/dev/null 2>/dev/null
${CC} tsc.c ${OPT} -fno-vectorize -fslp-vectorize -funroll-loops -fno-inline-functions -mllvm -slp-unroll-loops=false ${SLPSETTINGS} -emit-llvm -S -o $(basename $1).unroll-slp.ll 1>/dev/null 2>/dev/null
#for Count in 2 4 8; do
# ${CC} $1 ${OPT} -fno-vectorize -fslp-vectorize -funroll-loops -fno-inline-functions -mllvm -slp-unroll-loops=false -mllvm -unroll-count=${Count} -mllvm -unroll-allow-remainder=true -mllvm -unroll-runtime=1 ${SLPSETTINGS} -emit-llvm -S -o $(basename $1).unroll-${Count}-slp.ll 1>/dev/null 2>/dev/null
#done
${CC} tsc.c ${OPT} -fno-vectorize -fslp-vectorize -fno-unroll-loops -fno-inline-functions -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true ${SLPSETTINGS} -emit-llvm -S -o $(basename $1).luslp.ll 1>/dev/null 2>tmp.valu.txt
${CC} tsc.c ${OPT} -fvectorize -fno-slp-vectorize -fno-unroll-loops -fno-inline-functions -emit-llvm -S -o $(basename $1).vec.ll 1>/dev/null 2>/dev/null
