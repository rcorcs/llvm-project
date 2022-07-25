DIR=$(dirname $0)
BENCH=433.milc
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 <${DIR}/data/ref/input/su3imp.in 1>/tmp/${BENCH}.${FILENAME}.out"
