
import sys
import os

import string

class Instruction:
  def __init__(self, encoding, text):
    self.encoding = encoding
    self.text = text

  def size(self):
    return len(self.encoding)

class Function:
  def __init__(self, name, addr):
    self.name = name
    self.addr = addr
    self.content = []

  def size(self):
    return sum([inst.size() for inst in self.content])

def is_hex(s):
  hex_digits = set(string.hexdigits)
  # if s is long, then it is faster to check against a set
  return all(c in hex_digits for c in s)

def binarySize(content):
  count = 0
  for line in content.split('\n'):
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

def extractFunctions(path, filterNames=None):
  allFuncs = []
  with open(path) as f:
    func = None
    inst = None
    for line in f:
      if len(line.strip())==0:
        if func!=None and inst!=None:
          func.content.append(inst)
          inst = None
        func = None
        inst = None
      else:
        entries = line.split()
        if is_hex(entries[0]) and  entries[1].startswith('<') and entries[1].endswith('>:'):
          if func!=None and inst!=None:
            func.content.append(inst)
            inst = None
          name = entries[1][1:-2]
          func = Function(name, entries[0])
          allFuncs.append(func)
        entries = line.split('\t')
        if len(entries)>=1 and entries[0].strip()[-1]==':' and is_hex(entries[0].strip()[:-1]):
          if len(entries)==2 and inst!=None:
            inst.encoding.extend(entries[1].split())
          elif len(entries)==3:
            if inst!=None:
              func.content.append(inst)
              inst = None
            inst = Instruction(entries[1].split(), entries[2].strip())
    if func!=None and inst!=None:
      func.content.append(inst)
      inst = None
  return allFuncs

name = sys.argv[1].strip()
funcs = extractFunctions(sys.argv[2])
for f in funcs:
  print(f.name,',',name,',',f.size())
  #for inst in f.content:
  #  print(inst.text,'\t',inst.encoding)
