LLVM_PATH=/home/rodrigo/vec-research/supervec/build/bin/

#OPT="-O3 -march=skylake -mtune=skylake"
OPT="-O3 -march=native -mtune=native -ffast-math"
CFLAG=-lm
#SLPSETTINGS="-mllvm -slp-min-tree-size=2 -mllvm -slp-vectorize-hor-store=true -mllvm -slp-vectorize-hor=true"
SLPSETTINGS="-mllvm -slp-vectorize-hor-store=true -mllvm -slp-vectorize-hor=true"

EXT="${1##*.}"
if [ $EXT == "c" ]; then
 CC=${LLVM_PATH}clang
else
 CC=${LLVM_PATH}clang++
fi

echo "VALU/S+SLP"
${CC} $1 ${OPT} -fno-vectorize -fslp-vectorize -fno-unroll-loops -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true -mllvm -slp-unrolled-seeds=true ${SLPSETTINGS} -emit-llvm -S -o $(basename $1).valu-seeds.ll
