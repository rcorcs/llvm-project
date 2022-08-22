DIR=$(dirname $0)
BENCH=adpcm_d
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 < ${DIR}/data/large.adpcm 1>/tmp/${BENCH}.${FILENAME}.out 2>&1"


