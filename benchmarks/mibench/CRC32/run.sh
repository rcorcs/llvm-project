DIR=$(dirname $0)
BENCH=CRC32
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 ${DIR}/data/large.pcm > /tmp/${BENCH}.${FILENAME}.out 2>&1"
