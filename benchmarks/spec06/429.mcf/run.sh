DIR=$(dirname $0)
BENCH=429.mcf
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 ${DIR}/data/ref/input/inp.in >/tmp/${BENCH}.${FILENAME}.out"
