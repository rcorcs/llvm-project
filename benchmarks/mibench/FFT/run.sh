DIR=$(dirname $0)
BENCH=FFT
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 8 32768 > /tmp/${BENCH}.${FILENAME}.out 2>&1; $1 8 32768 -i >> /tmp/${BENCH}.${FILENAME}.out 2>&1;"
