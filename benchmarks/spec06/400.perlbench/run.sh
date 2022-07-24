DIR=$(dirname $0)
BENCH=400.perlbench
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 ${DIR}/data/test/input/test.pl 1>/tmp/${BENCH}.${FILENAME}.out 2>/dev/null" # 2>&1"
