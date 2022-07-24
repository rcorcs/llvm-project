DIR=$(dirname $0)
BENCH=998.specrand
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 324342 24239 1>/tmp/${BENCH}.${FILENAME}.out"
