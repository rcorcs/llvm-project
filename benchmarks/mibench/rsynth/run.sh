DIR=$(dirname $0)
BENCH=rsynth
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 -a -q -o /tmp/${BENCH}.${FILENAME}.out < ${DIR}/data/data.txt 1>/dev/null 2>&1"
