DIR=$(dirname $0)
BENCH=444.namd
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 --input ${DIR}/data/all/input/namd.input --iterations 2 1>/tmp/${BENCH}.${FILENAME}.out"
