LLVM_PATH=/home/rodrigo/tmp/bld/bin/
OPT="-O3 -march=native -mtune=native -ffast-math" # -fno-inline-functions"
CFLAG=-lm
SLPSETTINGS="-mllvm -slp-vectorize-hor-store=true -mllvm -slp-vectorize-hor=true"
LVSETTINGS= #"-mllvm -enable-cond-stores-vec=false -mllvm -vectorize-num-stores-pred=0 "

CC=${LLVM_PATH}clang

for UT in 100 200 300 400 500 750 1000; do
  for PT in 100 200 300 400 500 1000; do
    for Boost in 100 400 500 600 700; do
      for ItSim in 5 10 20; do
        FILENAME=tsvc.unroll-${UT}-${PT}-${Boost}-${ItSim}-slp
        ${CC} *.c ${OPT} -fno-vectorize -fslp-vectorize -funroll-loops -mllvm -slp-unroll-loops=false ${SLPSETTINGS} -mllvm -unroll-allow-partial=true -mllvm -unroll-allow-remainder=true -mllvm -unroll-runtime=true -mllvm -unroll-max-iteration-count-to-analyze=${ItSim} -mllvm -unroll-max-percent-threshold-boost=${Boost} -mllvm -unroll-partial-threshold=${PT} -mllvm -unroll-threshold=${UT} -o ${FILENAME} ${CFLAG}
        for i in 1 2 3; do
          ./${FILENAME} 1> DU-${UT}-${PT}-${Boost}-${ItSim}+SLP.${i}.txt
        done
      done
    done
  done
done

