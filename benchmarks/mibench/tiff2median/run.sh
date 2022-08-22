DIR=$(dirname $0)
BENCH=tiff2median
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 -f lsb2msb ${DIR}/data/data.tiff /tmp/${BENCH}.${FILENAME}.out 1>/dev/null 2>&1"

