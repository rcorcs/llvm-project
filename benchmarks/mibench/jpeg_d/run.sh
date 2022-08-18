DIR=$(dirname $0)
BENCH=jpeg_d
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 -dct int -ppm -outfile /tmp/${BENCH}.${FILENAME}.out ${DIR}/data/data.jpg 1>/dev/null 2>&1"
