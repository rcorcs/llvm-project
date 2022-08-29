DIR=$(dirname $0)
BENCH=tiff2dither
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 -c g4 ${DIR}/data/data.tiff /tmp/${BENCH}.${FILENAME}.out 1>/dev/null 2>&1"

