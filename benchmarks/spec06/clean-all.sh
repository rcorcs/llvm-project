DIR=$(dirname $0)
cd ${DIR}

BENCHMARKS=$(cat BenchNames)

for BENCH in ${BENCHMARKS}; do
  rm ${BENCH}/build/*
  mkdir -p ${BENCH}/build/
  cd ${BENCH}
  make clean >/dev/null
  cd ..
done
