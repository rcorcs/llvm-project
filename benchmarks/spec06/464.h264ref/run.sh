DIR=$(dirname $0)
BENCH=464.h264ref
cp $1 ${DIR}/data/all/input/main.tmp
cd ${DIR}/data/all/input/
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "./main.tmp 1>/tmp/${BENCH}.${FILENAME}.out"
