DIR=$(dirname $0)
BENCH=473.astar
cp $1 ${DIR}/data/ref/input/main.tmp
cd ${DIR}/data/ref/input
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "./main.tmp BigLakes2048.cfg 1>/tmp/${BENCH}.${FILENAME}.out"
