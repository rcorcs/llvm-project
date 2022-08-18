DIR=$(dirname $0)
BENCH=patricia
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 ${DIR}/data/data.udp" # 1>/tmp/${BENCH}.${FILENAME}.out 2>&1"

