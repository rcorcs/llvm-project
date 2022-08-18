DIR=$(dirname $0)
BENCH=typeset
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 -I ${DIR}/src/include -D ${DIR}/src/data -F ${DIR}/src/font -C ${DIR}/src/maps -H ${DIR}/src/hyph data/data.lout > /tmp/${BENCH}.${FILENAME}.out 2>/dev/null"
rm lout.li
