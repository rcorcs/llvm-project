LLVM_PATH=/home/rodrigo/tmp/build/bin/

#OPT="-O3 -march=skylake -mtune=skylake"
OPT="-O3 -march=native -mtune=native -ffast-math" # -fno-inline-functions"
#OPT="-O3"
CFLAG=-lm
#SLPSETTINGS=
#SLPSETTINGS="-mllvm -slp-min-tree-size=2 -mllvm -slp-vectorize-hor-store=true -mllvm -slp-vectorize-hor=true"
SLPSETTINGS="-mllvm -slp-vectorize-hor-store=true -mllvm -slp-vectorize-hor=true"
LVSETTINGS= #"-mllvm -enable-cond-stores-vec=false -mllvm -vectorize-num-stores-pred=0 "
#EXT="${1##*.}"
#if [ $EXT == "c" ]; then
CC=${LLVM_PATH}clang
#else
# CC=${LLVM_PATH}clang++
#fi

#${CC} tsc.c ${OPT} ${SLPSETTINGS} -o tsvc.std.ll ${CFLAG} -c -emit-llvm -S
#${CC} tsc.c ${OPT} ${SLPSETTINGS} -o tsvc.std-valu.ll -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true ${CFLAG} -c -emit-llvm -S 2>tmp-std.txt 1>/dev/null

${CC} tsc.c ${OPT} -fno-vectorize -fno-slp-vectorize -funroll-loops -o tsvc.o3novec.ll ${CFLAG} -c -emit-llvm -S
#${CC} tsc.c ${OPT} -fno-vectorize -fslp-vectorize -fno-unroll-loops -mllvm -slp-unroll-loops=false ${SLPSETTINGS} -o tsvc.slp-nounroll.ll ${CFLAG} -c -emit-llvm -S
#${CC} tsc.c ${OPT} -fno-vectorize -fslp-vectorize -funroll-loops -mllvm -slp-unroll-loops=false ${SLPSETTINGS} -o tsvc.slp-du.ll ${CFLAG} -c -emit-llvm -S
#${CC} tsc.c ${OPT} -fno-vectorize -fslp-vectorize -funroll-loops -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true -mllvm -slp-unrolled-seeds=false ${SLPSETTINGS} -o tsvc.slp-valu.ll ${CFLAG} -c -emit-llvm -S 2>slp-valu.txt 1>/dev/null
${CC} tsc.c ${OPT} -fno-vectorize -fslp-vectorize -funroll-loops -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true -mllvm -slp-unrolled-seeds=true ${SLPSETTINGS} -o tsvc.slp-valu-seeds.ll ${CFLAG} -c -emit-llvm -S 2>slp-valu-seeds.txt 1>/dev/null


#${CC} tsc.c ${OPT} -fvectorize -fno-slp-vectorize -funroll-loops -o tsvc.vec.ll ${CFLAG} -c -emit-llvm -S
