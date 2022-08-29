DIR=$(dirname $0)
BENCH=pgp
FILENAME=$(basename $1)
/usr/bin/time -f "${BENCH},${FILENAME},%E" sh -c "$1 -sa -z \"this is a test\" -u taustin@eecs.umich.edu ${DIR}/data/testin.txt austin@umich.edu 1>/tmp/${BENCH}.${FILENAME}.out 2>&1"
