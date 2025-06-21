#CCBIN=/home/rcor/dev/CSLPinSLP/build/bin/
#${CCBIN}clang *.c -w -O3 -fno-vectorize -ffast-math -fslp-vectorize -mllvm -cslp -mllvm -cslp-fnsize-limit=$1 -lm -mavx2 -o main -g -mllvm -cslp-counters
CCBIN=/home/rcor/dev/vwslp/build/bin/
${CCBIN}clang *.c -I./include -w -g -O3 -funroll-loops -fno-vectorize -ffast-math -fslp-vectorize -ffast-math -march=skylake -mtune=skylake -mavx2 -mllvm -vwslp-both -mllvm -unroll-count=4 -lm -mllvm -vwslp-debug=1
#${CCBIN}clang *.c -w -g -O3 -fno-vectorize -ffast-math -fslp-vectorize -ffast-math -march=skylake -mtune=skylake -mavx2 -mllvm -slp-regional -lm

