DIR=$(dirname $0)
BENCH=cjpeg
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 -outfile /tmp/${BENCH}.${FILENAME}.out ${DIR}/data/data.ppm 1>/dev/null 2>&1"

