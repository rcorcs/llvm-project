DIR=$(dirname $0)
cd ${DIR}

BENCHMARKS=$(cat BenchNames)

for BENCH in ${BENCHMARKS}; do
  rm ${BENCH}/build/*
  cd ${BENCH}
  make clean >/dev/null
  cd ..
done
