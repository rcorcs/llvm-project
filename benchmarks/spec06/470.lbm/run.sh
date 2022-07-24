DIR=$(dirname $0)
BENCH=470.lbm
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 1024 /dev/null 4 0 ${DIR}/data/train/input/100_100_130_cf_b.of 1>/tmp/${BENCH}.${FILENAME}.out"

