DIR=$(dirname $0)
cd ${DIR}

BENCHMARKS=$(cat BenchNames)

:>num-profitable-branches.csv
for BENCH in ${BENCHMARKS}; do
  for v in baseline soa brfusion-pa brfusion-nw cfmelder cfmelder-brfusion-pa cfmelder-brfusion-nw; do
  echo "${BENCH},${v},$(grep "Profitable Branch Fusion" ${BENCH}.${v}.txt | wc -l)" >> num-profitable-branches.csv
  done
done
