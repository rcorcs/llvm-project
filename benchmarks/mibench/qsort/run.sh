DIR=$(dirname $0)
BENCH=qsort
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 ${DIR}/data/input_large.dat 1>/tmp/${BENCH}.${FILENAME}.out 2>&1"

