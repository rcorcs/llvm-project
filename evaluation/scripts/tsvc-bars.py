
import sys

import numpy as np

import myplots as plts

path = sys.argv[1]
data = {}

def formatName(n):
  m = {'reroll':'LLVM', 'llvm':'LLVM', 'rolled':'RoLAG', 'rolling':'RoLAG', 'oracle':'Oracle'}
  if n in m.keys():
    return m[n]
  return n

with open(path) as f:
  for line in f:
    vals = [val.strip() for val in line.strip().split(',')]
    if vals[0] not in data.keys():
      data[vals[0]] = {}
    ftype=''
    name = formatName(vals[1])
    data[vals[0]][ftype+name] = float(vals[2])

#print data

gorder = ['LLVM', 'RoLAG']
BlueRedPallete = ['black','red']

ftype = ''

countpos = {}

pdata = {}

ignoreKs = ['s151','test','f','time_function','main']

for k in data.keys():
  if not np.all([ ((ftype+name) in data[k].keys()) for name in gorder]):
    continue
  for name in gorder:
    print(k)
    val = data[k][ftype+name]
    if sum([ (1 if (data[k][ftype+'baseline']!=data[k][ftype+n]) else 0) for n in ['Oracle','RoLAG'] ])==0:
      continue
    if k in ignoreKs:
      continue
    if k not in pdata.keys():
      pdata[k] = {}
    pdata[k][name] = float(data[k][ftype+'baseline']-val)/float(data[k][ftype+'baseline'])*100
    if pdata[k][name]>0:
      if name not in countpos.keys():
        countpos[name] = 0
      countpos[name] += 1
    #if pdata[k][name]<0:
    print(k, name, pdata[k][name])

print(countpos)
entries = list(sorted(list(pdata.keys()), key=lambda k: pdata[k]['RoLAG']))
#entries = list(sorted(list(pdata.keys()), key=lambda k: pdata[k]['LLVM']))

plts.bars(pdata,'Reduction (%)',groups=gorder,entries=entries,palette = BlueRedPallete,edgecolor='black',labelAverage=True,decimals=2,legendPosition='upper left',showXAxis=True, FilterDiffNum=0,filename='../results/tsvc-bars-code-reduction.pdf')
