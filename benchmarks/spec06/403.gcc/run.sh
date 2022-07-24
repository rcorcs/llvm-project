DIR=$(dirname $0)
BENCH=403.gcc
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 ${DIR}/data/ref/input/scilab.i -o /tmp/${BENCH}.${FILENAME}.out"
