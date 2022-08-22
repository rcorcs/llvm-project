DIR=$(dirname $0)
BENCH=blowfish_d
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 d ${DIR}/data/data.enc /tmp/${BENCH}.${FILENAME}.out 1234567890abcdeffedcba0987654321 1>/dev/null 2>&1"
