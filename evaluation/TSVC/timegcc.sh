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
