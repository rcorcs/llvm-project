DIR=$(dirname $0)
cd ${DIR}

BENCHMARKS=$(cat BenchNames)

for BENCH in ${BENCHMARKS}; do
  rm ${BENCH}/build/*
  mkdir -p ${BENCH}/build/
  cd ${BENCH}
  echo ${BENCH}
  make clean >/dev/null
  /usr/bin/time -f "${BENCH},baseline,%E" sh -c "make BIN=build/baseline 2>../${BENCH}.baseline.txt 1>/dev/null" 2>> ../compilation.csv
  make clean >/dev/null
  /usr/bin/time -f "${BENCH},soa,%E" sh -c "make ENABLE_BRFUSION=true ENABLE_SOA=true BIN=build/soa 2>../${BENCH}.soa.txt 1>/dev/null" 2>> ../compilation.csv
  make clean >/dev/null
  /usr/bin/time -f "${BENCH},brfusion-pa,%E" sh -c "make ENABLE_BRFUSION=true USE_HYFMPA=true  BIN=build/brfusion-pa 2>../${BENCH}.brfusion-pa.txt 1>/dev/null" 2>> ../compilation.csv
  #make clean >/dev/null
  #/usr/bin/time -f "${BENCH},brfusion-nw,%E" sh -c "make ENABLE_BRFUSION=true USE_HYFMNW=true BIN=build/brfusion-nw 2>../${BENCH}.brfusion-nw.txt 1>/dev/null" 2>> ../compilation.csv
  make clean >/dev/null
  /usr/bin/time -f "${BENCH},cfmelder,%E" sh -c "make ENABLE_CFMELDER=true BIN=build/cfmelder 2>../${BENCH}.cfmelder.txt 1>/dev/null" 2>> ../compilation.csv
  make clean >/dev/null
  /usr/bin/time -f "${BENCH},cfmelder-brfusion-pa,%E" sh -c "make ENABLE_CFMELDER=true ENABLE_BRFUSION=true USE_HYFMPA=true  BIN=build/cfmelder-brfusion-pa 2>../${BENCH}.cfmelder-brfusion-pa.txt 1>/dev/null" 2>> ../compilation.csv
  #make clean >/dev/null
  #/usr/bin/time -f "${BENCH},cfmelder-brfusion-nw,%E" sh -c "make ENABLE_CFMELDER=true ENABLE_BRFUSION=true USE_HYFMNW=true BIN=build/cfmelder-brfusion-nw 2>../${BENCH}.cfmelder-brfusion-nw.txt 1>/dev/null" 2>> ../compilation.csv
  cd ..

  #for VERSION in baseline soa brfusion-pa brfusion-nw cfmelder cfmelder-brfusion-pa cfmelder-brfusion-nw; do
  for VERSION in baseline soa brfusion-pa cfmelder cfmelder-brfusion-pa; do
    python results.py ${BENCH} ../../build/release/bin/llvm-size ${VERSION} >> ./results.csv
  done
done
