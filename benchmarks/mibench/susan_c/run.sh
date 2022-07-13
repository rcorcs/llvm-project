DIR=$(dirname $0)
BENCH=susan_c
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 ${DIR}/data/data.pgm /tmp/${BENCH}.${FILENAME}.out -c 1>/dev/null 2>&1"

