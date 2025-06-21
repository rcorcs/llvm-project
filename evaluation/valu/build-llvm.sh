#LLVM_PATH=/home/rodrigo/tmp/build/bin/
#LLVM_PATH=/home/rodrigo/tmp/bld/bin/
LLVM_PATH=/home/rodrigo/vec-research/supervec/build/bin/

#OPT="-O3 -march=skylake -mtune=skylake"
OPT="-O3 -march=native -mtune=native -ffast-math"
#OPT="-O3"
CFLAG=-lm
#SLPSETTINGS="-mllvm -slp-min-tree-size=2 -mllvm -slp-vectorize-hor-store=true -mllvm -slp-vectorize-hor=true"
SLPSETTINGS="-mllvm -slp-vectorize-hor-store=true -mllvm -slp-vectorize-hor=true"

EXT="${1##*.}"
if [ $EXT == "c" ]; then
 CC=${LLVM_PATH}clang
else
 CC=${LLVM_PATH}clang++
fi

echo "BASELINE"
${CC} $1 ${OPT} -fno-vectorize -fno-slp-vectorize -fno-unroll-loops -mllvm -slp-unroll-loops=false -emit-llvm -S -o $(basename $1).baseline.ll 1>/dev/null 2>/dev/null
echo "SLP"
${CC} $1 ${OPT} -fno-vectorize -fslp-vectorize -fno-unroll-loops -mllvm -slp-unroll-loops=false ${SLPSETTINGS} -emit-llvm -S -o $(basename $1).slp.ll 1>/dev/null 2>/dev/null
echo "DU+SLP"
${CC} $1 ${OPT} -fno-vectorize -fslp-vectorize -funroll-loops -mllvm -slp-unroll-loops=false ${SLPSETTINGS} -emit-llvm -S -o $(basename $1).du-slp.ll 1>/dev/null 2>/dev/null
#for Count in 8; do
# ${CC} $1 ${OPT} -fno-vectorize -fno-slp-vectorize -funroll-loops -mllvm -slp-unroll-loops=false -mllvm -unroll-count=${Count} -mllvm -unroll-allow-remainder=true -mllvm -unroll-runtime=1 ${SLPSETTINGS} -emit-llvm -S -o $(basename $1).unroll-${Count}-slp.ll 1>/dev/null 2>/dev/null
#done
echo "VALU+SLP"
${CC} $1 ${OPT} -fno-vectorize -fslp-vectorize -fno-unroll-loops -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true -mllvm -slp-unrolled-seeds=false ${SLPSETTINGS} -emit-llvm -S -o $(basename $1).valu.ll
echo "VALU/S+SLP"
${CC} $1 ${OPT} -fno-vectorize -fslp-vectorize -fno-unroll-loops -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true -mllvm -slp-unrolled-seeds=true ${SLPSETTINGS} -emit-llvm -S -o $(basename $1).valu-seeds.ll
echo "LV"
${CC} $1 ${OPT} -fvectorize -fno-slp-vectorize -fno-unroll-loops -emit-llvm -S -o $(basename $1).lv.ll 1>/dev/null 2>/dev/null
