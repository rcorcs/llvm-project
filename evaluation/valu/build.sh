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

echo "==BASELINE"
/usr/bin/time -f "${1},O3,%E" sh -c "${CC} $1 dummy.c ${OPT} -fno-vectorize -fno-slp-vectorize -fno-unroll-loops -fno-inline-functions -o $(basename $1).baseline ${CFLAG} &> $(basename $1).baseline.txt"
echo "==LV"
/usr/bin/time -f "${1},LV,%E" sh -c "${CC} $1 dummy.c ${OPT} -fvectorize -fno-slp-vectorize -fno-unroll-loops -fno-inline-functions -o $(basename $1).lv ${CFLAG} &> $(basename $1).lv.txt"
echo "==SLP"
/usr/bin/time -f "${1},SLP,%E" sh -c "${CC} $1 dummy.c ${OPT} -fno-vectorize -fslp-vectorize -fno-unroll-loops -fno-inline-functions -mllvm -slp-unroll-loops=false ${SLPSETTINGS} -o $(basename $1).slp ${CFLAG} &> $(basename $1).slp.txt"
echo "==DU+SLP"
/usr/bin/time -f "${1},DU+SLP,%E" sh -c "${CC} $1 dummy.c ${OPT} -fno-vectorize -fslp-vectorize -funroll-loops -fno-inline-functions -mllvm -slp-unroll-loops=false ${SLPSETTINGS} -o $(basename $1).du-slp ${CFLAG} &> $(basename $1).du-slp.txt"
#for Count in 2 4; do
#${CC} $1 dummy.c ${OPT} -fno-vectorize -fslp-vectorize -funroll-loops -fno-inline-functions -mllvm -slp-unroll-loops=false -mllvm -unroll-count=${Count} -mllvm -unroll-allow-remainder=true -mllvm -unroll-runtime=1 ${SLPSETTINGS} -o $(basename $1).unroll-${Count}-slp ${CFLAG}
#done
echo "==VALU+SLP"
/usr/bin/time -f "${1},VALU+SLP,%E" sh -c "${CC} $1 dummy.c ${OPT} -fno-vectorize -fslp-vectorize -fno-unroll-loops -fno-inline-functions -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true -mllvm -slp-unrolled-seeds=false ${SLPSETTINGS} -o $(basename $1).valu ${CFLAG} &> $(basename $1).valu.txt"
echo "==VALU/S+SLP"
/usr/bin/time -f "${1},VALU/S+SLP,%E" sh -c "${CC} $1 dummy.c ${OPT} -fno-vectorize -fslp-vectorize -fno-unroll-loops -fno-inline-functions -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true -mllvm -slp-unrolled-seeds=true ${SLPSETTINGS} -o $(basename $1).valu-seeds ${CFLAG} &> $(basename $1).valu-seeds.txt"
