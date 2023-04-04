OPT="-Os -fno-vectorize -fno-slp-vectorize -fno-unroll-loops" 

DIR=$(dirname $0)
cd ${DIR}

LLPATH=../loop-rolling/build/release/bin/

CC=${LLPATH}clang
LLOPT=${LLPATH}opt
#OBJDUMP=${LLPATH}llvm-objdump
OBJDUMP="objdump -M intel "


${CC} tsvc.original.c ${OPT} -emit-llvm -S -o tsvc.original.ll
${CC} tsvc.unrolled.c ${OPT} -emit-llvm -S -o tsvc.unrolled.ll

${LLOPT} -loop-reroll tsvc.unrolled.ll -o tsvc.reroll.ll -S 2>tsvc.reroll.txt
${LLOPT} -loop-rolling -simplifycfg -loop-simplify -loop-flatten -licm tsvc.unrolled.ll -o tsvc.rolled.ll -S 2>tsvc.rolled.txt

${CC} -x ir tsvc.original.ll ${OPT} -c -o tsvc.oracle.o
${CC} -x ir tsvc.unrolled.ll ${OPT} -c -o tsvc.baseline.o
${CC} -x ir tsvc.reroll.ll ${OPT} -c -o tsvc.reroll.o
${CC} -x ir tsvc.rolled.ll ${OPT} -c -o tsvc.rolled.o

${OBJDUMP} -d tsvc.oracle.o > tsvc.oracle.dump
${OBJDUMP} -d tsvc.baseline.o > tsvc.baseline.dump
${OBJDUMP} -d tsvc.reroll.o > tsvc.reroll.dump
${OBJDUMP} -d tsvc.rolled.o > tsvc.rolled.dump

:>tsvc.csv
python3 extract.py oracle tsvc.oracle.dump >> tsvc.csv
python3 extract.py baseline tsvc.baseline.dump >> tsvc.csv
python3 extract.py reroll tsvc.reroll.dump >> tsvc.csv
python3 extract.py rolled tsvc.rolled.dump >> tsvc.csv

${CC} tsvc.oracle.o common.c dummy.c ${OPT} -o tsvc.oracle -lm 
${CC} tsvc.baseline.o common.c dummy.c ${OPT} -o tsvc.baseline -lm 
${CC} tsvc.reroll.o common.c dummy.c ${OPT} -o tsvc.reroll -lm
${CC} tsvc.rolled.o common.c dummy.c  ${OPT} -o tsvc.rolled -lm
