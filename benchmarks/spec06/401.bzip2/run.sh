DIR=$(dirname $0)
BENCH=401.bzip2
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 ${DIR}/data/ref/input/text.html >/tmp/${BENCH}.${FILENAME}.out"
