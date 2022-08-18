DIR=$(dirname $0)
BENCH=blowfish_d
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 d ${DIR}/data/data.enc /tmp/${BENCH}.${FILENAME}.out ABC123 1>/tmp/${BENCH}.${FILENAME}.out 2>&1"


