DIR=$(dirname $0)
BENCH=471.omnetpp
cp $1 ${DIR}/data/ref/input/main.tmp
cd ${DIR}/data/ref/input/
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c ":>/tmp/${BENCH}.${FILENAME}.out"
