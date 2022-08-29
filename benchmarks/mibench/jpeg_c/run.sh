DIR=$(dirname $0)
BENCH=jpeg_c
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 -dct int -progressive -opt -outfile /tmp/${BENCH}.${FILENAME}.out ${DIR}/data/data.ppm 1>/dev/null 2>&1"

