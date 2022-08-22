DIR=$(dirname $0)
BENCH=ghostscript
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 -sDEVICE=ppm -dNOPAUSE -q -sOutputFile=/tmp/${BENCH}.${FILENAME}.out -- ${DIR}/data/large.ps 1>/dev/null 2>&1"
