
import sys

import numpy as np

import myplots as plts

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
  for name in gorder:
    if name=='Oracle':
      val = min([data[k][ftype+n] for n in gorder])
    else:
      val = data[k][ftype+name]
    if sum([ (1 if (data[k][ftype+'baseline']!=data[k][ftype+n]) else 0) for n in gorder ])==0:
      continue
    if k not in pdata.keys():
      pdata[k] = {}
    pdata[k][name] = float(data[k][ftype+'baseline']-val)/float(data[k][ftype+'baseline'])*100

ys = {}
#entries = list(sorted(list(pdata.keys()), key=lambda k: (pdata[k]['RoLAG'],pdata[k]['Oracle']) ))
entries = list( sorted(list(pdata.keys()), key=lambda k: (pdata[k]['region'],pdata[k]['RoLAG'])) )
for k in entries:
  if not np.all([ ((ftype+name) in data[k].keys()) for name in gorder]):
    print('skipping',name)
    continue
  for name in gorder:
    if name not in ys.keys():
      ys[name] = []
    ys[name].append(pdata[k][name])

for e in entries:
  print(e,pdata[e]['RoLAG'])

#plts.bars(pdata,'Reduction (%)',groups=gorder,entries=entries,palette = BlueRedPallete,edgecolor='black',labelAverage=True,decimals=2,legendPosition='upper left',showXAxis=False)#,filename='code-size-reduction.pdf')

#plt.plot(range(len(ys['RoLAG'])), sorted(ys['RoLAG']))


fig, ax = plt.subplots(figsize=(8, 6))


for i in range(len(gorder)):
  version = gorder[i]
  fcolor = BlueRedPallete[i] #''
  #plt.fill_between(range(len(ys[version])), sorted(ys[version]), y2=0, facecolor=fcolor, edgecolor='black', linewidth=2)
  plt.fill_between(range(len(ys[version])), ys[version], y2=0, facecolor=fcolor, edgecolor='black', linewidth=2,label=version)
  my = np.mean(ys[version])
  print('Average:',my)
  plt.plot(range(len(ys[version])), [my]*len(ys[version]), color='black', linewidth=2, linestyle='--')

plt.xlim(0,len(ys[version])-1)
plt.ylim(-10,80)

ax.yaxis.grid(True) #horizontal grid
ax.xaxis.grid(False) #horizontal grid

plt.ylabel('Reduction (%)',fontsize=16)
plt.tick_params(labelsize=14)

plt.legend(loc='best', fontsize=16)
plt.savefig('../results/tsvc-curve-code-reduction.pdf')


