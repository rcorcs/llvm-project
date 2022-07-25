DIR=$(dirname $0)
BENCH=456.hmmer
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 --seed 101 ${DIR}/data/ref/input/nph3.hmm 1>/tmp/${BENCH}.${FILENAME}.out 2>&1"

