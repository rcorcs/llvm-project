
import sys
import numpy as np
import myplots as plts

path = sys.argv[1]
data = {}

filename = path
with open(filename) as f:
  for line in f:
    vals = [val.strip() for val in line.strip().split(',')]
    if vals[0] not in data.keys():
      data[vals[0]] = {}
    ftype=''
    name = vals[1]
    if '.' in name:
      tmp = name.split('.')
      ftype = tmp[0]+'.'
      name = tmp[1]
    if name=='fm3':
      name = 'Oracle [t='+str(t)+']'
    if name=='fm2':
      name = 'SalSSA [t='+str(t)+']'
    if name=='fm':
      name = 'FMSA [t='+str(t)+']'
    if name=='llvm':
      name = 'Identical'
    if name=='llfm':
      name = 'Identical'
    if name=='soa':
      name = 'SOA'
    data[vals[0]][ftype+name] = float(vals[2])

gorder = ['soa', 'cfmelder', 'brfusion-pa', 'cfmelder-brfusion-pa']
#gorder = ['soa', 'cfmelder', 'brfusion-pa', 'brfusion-nw', 'cfmelder-brfusion-pa', 'cfmelder-brfusion-nw']
#colors = ['black',   '#8bb9fa', '#1270f4',   '#fa8072','#ff3e20']
colors = ['#1A0841',  '#4F9DA6', '#FFAD5A', '#FF5959']

mapname = {}
mapname['soa'] = 'Single-Block'
mapname['cfmelder'] = 'CFMelder'
mapname['brfusion-pa'] = 'SEME-Fusion'
mapname['cfmelder-brfusion-pa'] = 'Combined'


pdata = {}
for k in data.keys():
  if 'baseline' not in data[k].keys():
    continue
  
  #if len(data[k].keys())!=max([len(data[ek].keys()) for ek in data.keys()]):
  #  print 'skipping',k
  #  continue
  pdata[k] = {}
  for name in gorder:
    if name not in data[k].keys():
      data[k][name] = data[k]['baseline']
    val = data[k][name]
    pdata[k][mapname[name]] = (data[k]['baseline']-val) #/data[k]['baseline'] * 100
    #print(k, name, pdata[k][mapname[name]])
  if sum([pdata[k][mapname[name]] for name in gorder])==0:
    del pdata[k]

plts.bars(pdata,'Reduction (Bytes)',groups=[mapname[n] for n in gorder],palette =colors,edgecolor='black',labelAverage=True,decimals=1,legendPosition='upper left',filename='absolute-reduction.pdf')


pdata = {}
for k in data.keys():
  if 'baseline' not in data[k].keys():
    continue
  #if len(data[k].keys())!=max([len(data[ek].keys()) for ek in data.keys()]):
  #  print 'skipping',k
  #  continue
  pdata[k] = {}
  for name in gorder:
    if name not in data[k].keys():
      data[k][name] = data[k]['baseline']
    val = data[k][name]
    pdata[k][mapname[name]] = (data[k]['baseline']-val)/data[k]['baseline'] * 100
    print(k, name, pdata[k][mapname[name]])

plts.bars(pdata,'Reduction (%)',groups=[mapname[n] for n in gorder],palette =colors,edgecolor='black',labelAverage=True,decimals=1,legendPosition='upper left',filename='percentage-reduction.pdf')


