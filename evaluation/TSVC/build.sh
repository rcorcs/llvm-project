#LLVM_PATH=/home/rodrigo/tmp/build/bin/
LLVM_PATH=/home/rodrigo/vec-research/supervec/build/bin/

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
CC="${LLVM_PATH}clang -ansi -std=gnu99"
#else
# CC=${LLVM_PATH}clang++
#fi


gcc -ansi -std=gnu99 *.c ${OPT} -o tsvc.std-gcc ${CFLAG}
${CC} *.c ${OPT} ${SLPSETTINGS} -o tsvc.std ${CFLAG}
${CC} *.c ${OPT} ${SLPSETTINGS} -o tsvc.std-valu -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true -mllvm -slp-unrolled-seeds=false ${CFLAG} 2> std-valu.txt
${CC} *.c ${OPT} ${SLPSETTINGS} -o tsvc.std-valu-seeds -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true -mllvm -slp-unrolled-seeds=true ${CFLAG} 2> std-valu-seeds.txt

#${CC} *.c ${OPT} -fno-vectorize -fno-slp-vectorize -fno-unroll-loops -o tsvc.o3novec ${CFLAG}
${CC} *.c ${OPT} -fno-vectorize -fno-slp-vectorize -funroll-loops -o tsvc.o3novec ${CFLAG}
${CC} *.c ${OPT} -fno-vectorize -fslp-vectorize -fno-unroll-loops -mllvm -slp-unroll-loops=false ${SLPSETTINGS} -o tsvc.slp-nounroll ${CFLAG}
${CC} *.c ${OPT} -fno-vectorize -fslp-vectorize -funroll-loops -mllvm -slp-unroll-loops=false ${SLPSETTINGS} -o tsvc.slp-du ${CFLAG}
${CC} *.c ${OPT} -fno-vectorize -fslp-vectorize -funroll-loops -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true -mllvm -slp-unrolled-seeds=false ${SLPSETTINGS} -o tsvc.slp-valu ${CFLAG}
${CC} *.c ${OPT} -fno-vectorize -fslp-vectorize -funroll-loops -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true -mllvm -slp-unrolled-seeds=true ${SLPSETTINGS} -o tsvc.slp-valu-seeds ${CFLAG}
${CC} *.c ${OPT} -fvectorize -fno-slp-vectorize -funroll-loops -o tsvc.vec ${CFLAG}
#${CC} *.c ${OPT} -fvectorize -fno-slp-vectorize -fno-unroll-loops -o tsvc.vec ${CFLAG}
