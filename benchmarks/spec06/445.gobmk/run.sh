DIR=$(dirname $0)
BENCH=445.gobmk
cp $1 ${DIR}/data/all/input/main.tmp
cd ${DIR}/data/all/input/
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "./main.tmp  --mode gtp <connect.tst 1>/tmp/${BENCH}.${FILENAME}.out"
