
import sys

import numpy as np


import matplotlib.pyplot as plt


#sns.set_style('whitegrid', {'legend.frameon': True, 'font.family': [u'serif']})

path = sys.argv[1]
data = {}

def formatName(n):
  m = {'reroll':'LLVM', 'llvm':'LLVM', 'rolled':'RoLAG', 'rolling':'RoLAG', 'oracle':'Oracle'}
  if n in m.keys():
    return m[n]
  return n

ignoreKs = ['s151','test','f','time_function','main']

with open(path) as f:
  for line in f:
    vals = line.split(',')
    key = ','.join(vals[:-2])
    if key in ignoreKs:
      continue
    if key not in data.keys():
      data[key] = {}
    ftype=''
    name = formatName(vals[-2].strip())
    data[key][ftype+name] = float(vals[-1].strip())

#print data

gorder = ['Oracle','region','RoLAG']
BlueRedPallete = ['#79cb42','#ff6868','blue']

ftype = ''

pdata = {}
for k in data.keys():
  if not np.all([ ((ftype+name) in data[k].keys()) for name in gorder]):
    continue
  #if data[k][ftype+'baseline']==data[k][ftype+'region'] and data[k][ftype+'Oracle']<data[k][ftype+'baseline']:
  #  print('Investigate:',k)

  if data[k]['Oracle'] < data[k]['baseline'] and data[k]['RoLAG']==data[k]['region'] and data[k]['baseline']==data[k]['region']:
    print(k,data[k]['baseline'],data[k]['Oracle'])
