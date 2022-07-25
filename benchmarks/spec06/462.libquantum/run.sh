DIR=$(dirname $0)
BENCH=462.libquantum
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 256 8 1>/tmp/${BENCH}.${FILENAME}.out"
