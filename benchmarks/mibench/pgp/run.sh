DIR=$(dirname $0)
BENCH=pgp
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 -sa ${DIR}/data/testin.txt test -o /tmp/${BENCH}.${FILENAME}.out 1>/tmp/${BENCH}.${FILENAME}.err 2>&1"
