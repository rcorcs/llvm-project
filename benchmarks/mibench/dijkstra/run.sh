DIR=$(dirname $0)
BENCH=dijkstra
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 ${DIR}/data/data.txt 1>/tmp/${BENCH}.${FILENAME}.out 2>&1"

