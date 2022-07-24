DIR=$(dirname $0)
BENCH=447.dealII
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 10 1>/tmp/${BENCH}.${FILENAME}.out"
