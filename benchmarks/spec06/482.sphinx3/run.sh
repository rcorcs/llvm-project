DIR=$(dirname $0)
BENCH=482.sphinx3
cp $1 ${DIR}/data/all/input/main.tmp
cd ${DIR}/data/all/input/
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "./main.tmp ctlfile . args.an4 1>/tmp/${BENCH}.${FILENAME}.out"

