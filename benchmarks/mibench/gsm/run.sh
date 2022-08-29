DIR=$(dirname $0)
BENCH=gsm
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 -fps -c ${DIR}/data/large.au > /tmp/${BENCH}.${FILENAME}.out 2>&1;"
