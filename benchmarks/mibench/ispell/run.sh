DIR=$(dirname $0)
BENCH=ispell
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 -a -d ${DIR}/data/americanmed+ ${DIR}/data/large.txt > /tmp/${BENCH}.${FILENAME}.out 2>&1"
