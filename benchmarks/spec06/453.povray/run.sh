DIR=$(dirname $0)
BENCH=453.povray
cp $1 ${DIR}/data/all/input/main.tmp
cd ${DIR}/data/all/input
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "./main.tmp  SPEC-benchmark-train.ini 1>/tmp/${BENCH}.${FILENAME}.out 2>&1"
