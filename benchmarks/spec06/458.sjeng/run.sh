DIR=$(dirname $0)
BENCH=458.sjeng
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 ${DIR}/data/ref/input/ref.txt 1>/tmp/${BENCH}.${FILENAME}.out"
