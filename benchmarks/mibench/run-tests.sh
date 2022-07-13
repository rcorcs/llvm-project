DIR=$(dirname $0)
cd ${DIR}

BENCHMARKS=$(cat BenchNames)

BUILDS="baseline inline trivial tro"
for BENCH in ${BENCHMARKS}; do
  for VERSION in ${BUILDS}; do
    bash ${BENCH}/run.sh ${BENCH}/build/main.${VERSION}
  done
done
