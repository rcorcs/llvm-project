DIR=$(dirname $0)
BENCH=blowfish_e
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 e ${DIR}/data/data.wav /tmp/${BENCH}.${FILENAME}.out ABC123 1>/tmp/${BENCH}.${FILENAME}.out 2>&1"


