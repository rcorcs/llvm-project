
import sys

with open(sys.argv[1]) as f:
  for line in f:
    tmp = line.split()
    print(tmp[0],tmp[2])

