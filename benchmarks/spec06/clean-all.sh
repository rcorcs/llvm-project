DIR=$(dirname $0)
cd ${DIR}

BENCHMARKS=$(cat BenchNames)

for BENCH in ${BENCHMARKS}; do
  rm -f ${BENCH}/build/*
  mkdir -p ${BENCH}/build/
  cd ${BENCH}
  make clean >/dev/null
  cd ..
done

# clean up the space
rm -f *.baseline.txt *.soa.txt *.brfusion-pa.txt *.brfusion-nw.txt *.cfmelder.txt *.cfmelder-brfusion-pa.txt *.cfmelder-brfusion-nw.txt
rm -f results.csv compilation.csv
