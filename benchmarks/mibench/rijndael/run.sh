DIR=$(dirname $0)
BENCH=rijndael
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 ${DIR}/data/data.asc /tmp/${BENCH}.${FILENAME}.1.out e 1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321 1>/dev/null 2>&1;"
