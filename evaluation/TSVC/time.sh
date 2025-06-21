LLVM_PATH=/home/rodrigo/tmp/bld/bin/

#OPT="-O3 -march=skylake -mtune=skylake"
OPT="-O3 -march=native -mtune=native -ffast-math -s -g0" # -fno-inline-functions"
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

/usr/bin/time -f "GCC,%E" sh -c "${CC} *.c ${OPT} ${SLPSETTINGS} -o tsvc.std-gcc ${CFLAG} 2>/dev/null 1>/dev/null"
/usr/bin/time -f "STD,%E" sh -c "${CC} *.c ${OPT} ${SLPSETTINGS} -o tsvc.std ${CFLAG} 2>/dev/null 1>/dev/null"
  #/usr/bin/time -f "STD+DU=8,%E" sh -c "${CC} *.c ${OPT} ${SLPSETTINGS} -mllvm -unroll-count=8 -o tsvc.std-du8 ${CFLAG} 2>/dev/null 1>/dev/null"

/usr/bin/time -f "STD+VALU,%E" sh -c "${CC} *.c ${OPT} ${SLPSETTINGS} -o tsvc.std-valu -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true -mllvm -slp-unrolled-seeds=false  ${CFLAG} 2>/dev/null 1>/dev/null"
/usr/bin/time -f "STD+VALU/S,%E" sh -c "${CC} *.c ${OPT} ${SLPSETTINGS} -o tsvc.std-valu-seeds -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true -mllvm -slp-unrolled-seeds=true ${CFLAG} 1>/dev/null 2>std-valu-seeds.txt"

  #${CC} *.c ${OPT} -fno-vectorize -fno-slp-vectorize -fno-unroll-loops -o tsvc.o3novec ${CFLAG}
/usr/bin/time -f "O3,%E" sh -c "${CC} *.c ${OPT} -fno-vectorize -fno-slp-vectorize -funroll-loops -o tsvc.o3novec ${CFLAG} 2>/dev/null 1>/dev/null"
/usr/bin/time -f "SLP,%E" sh -c "${CC} *.c ${OPT} -fno-vectorize -fslp-vectorize -fno-unroll-loops -mllvm -slp-unroll-loops=false ${SLPSETTINGS} -o tsvc.slp-nounroll ${CFLAG} 2>/dev/null 1>/dev/null"
/usr/bin/time -f "DU+SLP,%E" sh -c "${CC} *.c ${OPT} -fno-vectorize -fslp-vectorize -funroll-loops -mllvm -slp-unroll-loops=false ${SLPSETTINGS} -o tsvc.slp-du ${CFLAG} 2>/dev/null 1>/dev/null"

/usr/bin/time -f "VALU+SLP,%E" sh -c "${CC} *.c ${OPT} -fno-vectorize -fslp-vectorize -funroll-loops -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true -mllvm -slp-unrolled-seeds=false ${SLPSETTINGS} -o tsvc.slp-valu ${CFLAG} 2>/dev/null 1>/dev/null"
/usr/bin/time -f "VALU/S+SLP,%E" sh -c "${CC} *.c ${OPT} -fno-vectorize -fslp-vectorize -funroll-loops -mllvm -slp-unroll-loops=true -mllvm -slp-unroll-runtime=true -mllvm -slp-unrolled-seeds=true ${SLPSETTINGS} -o tsvc.slp-valu-seeds ${CFLAG} 2>/dev/null 1>/dev/null 2>slp-valu-seeds.txt"

/usr/bin/time -f "LV,%E" sh -c "${CC} *.c ${OPT} -fvectorize -fno-slp-vectorize -funroll-loops -o tsvc.vec ${CFLAG} 2>/dev/null 1>/dev/null"
  #${CC} *.c ${OPT} -fvectorize -fno-slp-vectorize -fno-unroll-loops -o tsvc.vec ${CFLAG}
