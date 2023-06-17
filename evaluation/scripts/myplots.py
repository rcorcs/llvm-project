
import matplotlib

import os
if os.environ.get('DISPLAY','') == '':
  matplotlib.use('Agg')

import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import t as student_t
from scipy.stats import norm

def getVal(x,summary=np.mean):
   if isinstance(x, dict):
      return x['mean']
   elif isinstance(x, list):
      return summary(x)
   else:
      return x

def bars(dataset, ylabel, plotAverage=True, groups=None, entries=None, decimals=2,filename=None,summary=np.mean,palette = ['black','green','blue','red','yellow','orange','brown','pink'],edgecolor=None,labelAverage=True,title=None,legendPosition='best',horizontalLines=None,showXAxis=True,FilterDiffNum=None):
   global fig, ax
   if not entries:
      entries = list(sorted(dataset.keys()))
   else:
      entries = list(entries)
   if not groups:
      if isinstance(dataset[entries[0]], dict):
         groups = list(sorted(dataset[entries[0]].keys()))
   else:
      groups = list(groups)
   if groups:
      width = 1.0/(len(groups)+1.0)
   else:
      width = 0.5
   index = np.arange(len(entries))
   #sns.set_style('whitegrid', {'legend.frameon': True, 'font.family': [u'serif']})
   opacity = 0.75
   error_config = {'ecolor': 'black'}
   #fig, ax = plt.subplots(figsize=(4, 3))
   #fig, ax = plt.subplots(figsize=(6, 5))
   fig, ax = plt.subplots(figsize=(16, 4))
   #fig, ax = plt.subplots(figsize=(15, 3))
   maxHeight = float('-inf')
   minHeight = float('inf')
   idx=0


   if horizontalLines:
     for hline in horizontalLines:
       plt.axhline(hline, color='grey', linestyle='dashed', linewidth=1)

   if groups:
      avgs = {}
      for group in groups:
         avgs[group] = summary([ getVal(dataset[key][group],np.mean) for key in entries ])
      if FilterDiffNum!=None:
         entries = [key for key in entries if np.any([ getVal(dataset[key][g],np.mean)!=FilterDiffNum for g in groups ])]
         index = np.arange(len(entries))
      for group in groups:
         intervals = None
         if isinstance(dataset[entries[0]][groups[0]], dict):
            ys = [ dataset[key][group]['mean'] for key in entries]
            intervals = [norm.interval(0.99, loc=dataset[key][group]['mean'], scale=dataset[key][group]['std']/np.sqrt(dataset[key][group]['n'])) for key in entries]
         elif isinstance(dataset[entries[0]][groups[0]], list):
            ys = [ (np.mean(dataset[key][group])) for key in entries]
            intervals = [student_t.interval(0.99, df=len(dataset[key][group])-1, loc=np.mean(dataset[key][group]), scale=np.std(dataset[key][group])/np.sqrt(len(dataset[key][group]))) for key in entries]
         else:
            ys = [ dataset[key][group] for key in entries ]
         maxHeight = max(maxHeight, max(ys))
         minHeight = min(minHeight, min(ys))
         if intervals:
            intervals = [(y[0], y[1]) for y in intervals]
            plt.bar(index+idx*width, ys, width,
                 alpha=opacity,
                 color=palette[idx], edgecolor=(edgecolor if edgecolor else palette[idx]),
                 yerr=[(y[1]-y[0])*0.5 for y in intervals],
                 error_kw=error_config,
                 label=group)
         else:
            plt.bar(index+idx*width, ys, width,
                 alpha=opacity,
                 color=palette[idx], edgecolor=(edgecolor if edgecolor else palette[idx]),
                 label=group)
         if plotAverage:
            plt.bar([len(entries)+idx*width*1.25], [avgs[group]], width*1.25,
                 alpha=min(opacity*1.20,1.0),
                 color=palette[idx], edgecolor=(edgecolor if edgecolor else palette[idx]))
            height = avgs[group]
            #print height
            avg = np.round(avgs[group],decimals=decimals)
            if decimals==0:
               avg = int(avg)
            label = str(avg)
            if labelAverage:
               plt.text(len(entries) + idx*width*1.25, height, label, fontsize=12, ha='center', va='bottom',rotation=90)

         idx += 1
      if plotAverage:
         plt.axvline((len(entries)-1) + len(groups)*width - width/4, color='grey', linestyle='dashed', linewidth=1)
   else:
      if isinstance(dataset[entries[0]], float):
         ys = [ dataset[key] for key in entries ]
         maxHeight = max(maxHeight, max(ys))
         minHeight = min(minHeight, min(ys))
         plt.bar(index, ys, width,
                 alpha=opacity,
                 color=palette[idx], edgecolor=(edgecolor if edgecolor else palette[idx]))
         if plotAverage:
            plt.bar([len(entries)], [summary(ys)], width*1.25,
                 alpha=min(opacity*1.20,1.0),
                 color=palette[idx], edgecolor=(edgecolor if edgecolor else palette[idx]))
            height = summary(ys)
         
            avg = np.round(summary(ys),decimals=decimals)
            if decimals==0:
               avg = int(avg)
            label = str(avg)
            if labelAverage:
               plt.text(len(entries), height, label, fontsize=12, ha='center', va='bottom',rotation=90)

         if plotAverage:
            plt.axvline((len(entries)-1) + width, color='grey', linestyle='dashed', linewidth=1)

   ax.yaxis.grid(True) #horizontal grid
   ax.xaxis.grid(False) #horizontal grid

   if minHeight>=0:
      plt.ylim(0, maxHeight*1.1)
   else:
      plt.ylim(minHeight*1.1, maxHeight*1.1)

   #plt.ylim(0, 10.5)
   #plt.ylim(-1, 6.25)

   plt.ylabel(ylabel,fontsize=16)
   #plt.ylabel(ylabel)

   if plotAverage:
      entries.append('Mean')
      index = np.arange(len(entries),dtype=float)
      #index[len(entries)-1] += width*len(groups)*0.5

   
   if groups:
      if showXAxis:
          plt.xticks(index + (len(groups)-1)*width*0.5, entries, rotation=90, ha='center')
      else:
          plt.xticks([], [])
      #plt.xticks(index + (len(groups)-1)*width*0.5, entries, rotation=30, ha='right')
      #plt.legend(loc='lower right',fontsize=14)
      #plt.legend(loc='upper left',fontsize=14)
      #plt.legend(loc=legendPosition,fontsize=14, ncol=3)
      plt.legend(loc=legendPosition,fontsize=14)
      #plt.legend(loc='best',fontsize=10)
   else:
      if showXAxis:
         plt.xticks(index, entries, rotation=30, ha='right')
      else:
         plt.xticks([], [])

   #plt.yticks([-20,-15,-10,-5,0,5,10,15,20])
   #plt.yticks([-10,-5,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70])
   #plt.yticks([-1,0,1,2,3,4,5,6])

   box = ax.get_position()
   ax.set_position([box.x0, box.y0*1.5, box.width, box.height])

   plt.tick_params(labelsize=12)

   if title:
      plt.title(title)

   #plt.tight_layout()
   if filename:
      plt.savefig(filename)
   else:
      plt.show()
   plt.clf()
   plt.close('all')





def onTopBars(dataset, ylabel, plotAverage=True, groups=None, entries=None, decimals=2,filename=None,summary=np.mean,palette = ['black','green','blue','red','yellow','orange','brown','pink'],edgecolor=None,labelAverage=False,horizontalLines=None,legend=True):
   global fig, ax
   if not entries:
      entries = list(sorted(dataset.keys()))
   else:
      entries = list(entries)
   if not groups:
      if isinstance(dataset[entries[0]], dict):
         groups = list(sorted(dataset[entries[0]].keys()))
   else:
      groups = list(groups)
   width = 0.5
   index = np.arange(len(entries))
   #sns.set_style('whitegrid', {'legend.frameon': True, 'font.family': [u'serif']})
   opacity = 1
   opacity = min(opacity,1)
   error_config = {'ecolor': 'black'}
   #fig, ax = plt.subplots(figsize=(4, 3))
   fig, ax = plt.subplots(figsize=(12, 3))
   maxHeight = float('-inf')
   minHeight = float('inf')

   if horizontalLines:
     for hline in horizontalLines:
       plt.axhline(hline, color='grey', linestyle='dashed', linewidth=1)

   idx=0
   if groups:
      for group in groups:
         intervals = None
         if isinstance(dataset[entries[0]][groups[0]], dict):
            ys = [ dataset[key][group]['mean'] for key in entries]
            intervals = [norm.interval(0.99, loc=dataset[key][group]['mean'], scale=dataset[key][group]['std']/np.sqrt(dataset[key][group]['n'])) for key in entries]
         elif isinstance(dataset[entries[0]][groups[0]], list):
            ys = [ (summary(dataset[key][group])) for key in entries]
            intervals = [student_t.interval(0.99, df=len(dataset[key][group])-1, loc=np.mean(dataset[key][group]), scale=np.std(dataset[key][group])/np.sqrt(len(dataset[key][group]))) for key in entries]
         else:
            ys = [ dataset[key][group] for key in entries ]
         maxHeight = max(maxHeight, max(ys))
         minHeight = min(minHeight, min(ys))
         if intervals:
            intervals = [(y[0], y[1]) for y in intervals]
            plt.bar(index, ys, width,
                 alpha=opacity,
                 color=palette[idx], edgecolor=(edgecolor if edgecolor else palette[idx]),
                 yerr=[(y[1]-y[0])*0.5 for y in intervals],
                 error_kw=error_config,
                 label=group)
         else:
            plt.bar(index, ys, width,
                 alpha=opacity,
                 color=palette[idx], edgecolor=(edgecolor if edgecolor else palette[idx]),
                 label=group)
         if plotAverage:
            plt.bar([len(entries)], [np.mean(ys)], width*1.0,
                 alpha=min(opacity*1.20,1.0),
                 color=palette[idx], edgecolor=(edgecolor if edgecolor else palette[idx]))
            height = np.mean(ys)
         
            avg = np.round(np.mean(ys),decimals=decimals)
            if decimals==0:
               avg = int(avg)
            label = str(avg)
            if labelAverage:
               plt.text(len(entries) + width*1.5, height, label, fontsize=11, ha='center', va='bottom')

         idx += 1
      if plotAverage:
         plt.axvline((len(entries)-1) + width, color='grey', linestyle='dashed', linewidth=1)
   else:
      if isinstance(dataset[entries[0]], float):
         ys = [ dataset[key] for key in entries ]
         maxHeight = max(maxHeight, max(ys))
         minHeight = min(minHeight, min(ys))
         plt.bar(index, ys, width,
                 alpha=opacity,
                 color=palette[idx], edgecolor=(edgecolor if edgecolor else palette[idx]))
         if plotAverage:
            plt.bar([len(entries)], [np.mean(ys)], width*1.5,
                 alpha=min(opacity*1.20,1.0),
                 color=palette[idx], edgecolor=(edgecolor if edgecolor else palette[idx]))
            height = np.mean(ys)
         
            avg = np.round(np.mean(ys),decimals=decimals)
            if decimals==0:
               avg = int(avg)
            label = str(avg)
            if labelAverage:
               plt.text(len(entries)+width*1.5, height, label, fontsize=9, ha='center', va='bottom')

         if plotAverage:
            plt.axvline((len(entries)-1), color='grey', linestyle='dashed', linewidth=1)

   ax.yaxis.grid(True) #horizontal grid
   ax.xaxis.grid(False) #horizontal grid

   if minHeight>=0:
      plt.ylim(0, maxHeight*1.1)
   else:
      plt.ylim(minHeight*1.1, maxHeight*1.1)

   plt.ylabel(ylabel,fontsize=16)
   #plt.ylabel(ylabel)

   if plotAverage:
      entries.append('Mean')
      index = np.arange(len(entries))
      #index[len(entries)-1] += 1

   if groups:
      #plt.xticks(index + (len(groups)-1)*width*0.5, entries, rotation=30)
      plt.xticks(index, entries, rotation=90, ha='center')
      if legend:
        plt.legend(loc='best',fontsize=14)
        #plt.legend(loc='upper left',fontsize=14)
        #plt.legend(loc='best',fontsize=14)
        #plt.legend(loc='best',fontsize=10)
   else:
      plt.xticks(index, entries, rotation=30)

   box = ax.get_position()
   ax.set_position([box.x0, box.y0*1, box.width/3, box.height])

   plt.tick_params(labelsize=14)

   #plt.tight_layout()
   if filename:
      plt.savefig(filename)
   else:
      plt.show()
   plt.clf()
   plt.close('all')

