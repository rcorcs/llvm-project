DIR=$(dirname $0)
BENCH=483.xalancbmk
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 ${DIR}/data/ref/input/t5.xml ${DIR}/data/ref/input/xalanc.xsl 1>/tmp/${BENCH}.${FILENAME}.out"

