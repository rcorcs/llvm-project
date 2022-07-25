DIR=$(dirname $0)
BENCH=450.soplex
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 ${DIR}/data/test/input/test.mps 1>/tmp/${BENCH}.${FILENAME}.out 2>&1"

