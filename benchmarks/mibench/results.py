
import sys
import os

import string
def is_hex(s):
     hex_digits = set(string.hexdigits)
     # if s is long, then it is faster to check against a set
     return all(c in hex_digits for c in s)

def binarySize(filename):
  count = 0
  with open(filename) as f:
    for line in f:
      if ':' in line:
        tmp = line.split(':')
        offset = tmp[0].strip()
        if not is_hex(offset):
          continue
        instruction = tmp[1].strip().split('\t')
        if len(instruction) == 0:
          continue
        binary = instruction[0].strip().split()
        valid = all([is_hex(byte.strip()) for byte in binary])
        if valid:
          count += len(binary)
  return count

bench = sys.argv[1]
path = bench+'/build'

sizeexec = sys.argv[2]

ftypes = sys.argv[3:]

import os
stream = os.popen('echo Returned output')
output = stream.read()


for ftype in ftypes:
  filename = path+'/'+ftype
  #stream = os.popen('/home/rodrigo/llvm/size-inlining/build/release/bin/llvm-size '+filename)
  stream = os.popen(sizeexec+' '+filename)
  data = stream.read().strip().split('\n')[1]
  text_size = data.split()[0]
  print str(bench)+', '+str(ftype)+', '+str(text_size)
