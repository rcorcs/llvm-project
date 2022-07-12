DIR=$(dirname $0)
BENCH=susan_e
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 ${DIR}/data/data.pgm /tmp/${BENCH}.${FILENAME}.out -e 1>/dev/null 2>&1"
