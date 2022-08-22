DIR=$(dirname $0)
BENCH=sha
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 ${DIR}/data/input_large.asc 1>/tmp/${BENCH}.${FILENAME}.out 2>&1"
