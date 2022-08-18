DIR=$(dirname $0)
BENCH=stringsearch
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 1>/tmp/${BENCH}.${FILENAME}.out 2>&1"
