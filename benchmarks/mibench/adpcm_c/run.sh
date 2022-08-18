DIR=$(dirname $0)
BENCH=adpcm_c
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 < ${DIR}/data/large.pcm 1>/tmp/${BENCH}.${FILENAME}.out 2>&1"


