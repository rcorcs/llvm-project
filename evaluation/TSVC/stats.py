
import sys

filename = sys.argv[1]

keys = 'MATCH IDENTICAL BINOP GEPSEQ INTSEQ ALTSEQ CONSTEXPR REDUCTION RECURRENCE MISMATCH MULTI'.split()

data = {}
for k in keys:
  data[k] = 0

with open(filename) as f:
  for line in f:
    line = line.strip()
    for k in keys:
      if line.startswith(k+':'):
        tmp = line.split(':')
        if len(tmp)==2:
            data[k] += int(tmp[1])

for k in keys:
  print(k,data[k])
