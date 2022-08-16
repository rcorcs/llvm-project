
import matplotlib

import os
#if os.environ.get('DISPLAY','') == '':
#  matplotlib.use('Agg')

#import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import t as student_t
from scipy.stats import norm


def bars(dataset, ylabel, plotAverage=True, groups=None, entries=None, decimals=2,filename=None,summary=np.mean,palette = ['#AF1515','#15AF3E','#FFAA32','#DC75FF','#32AAFF','#C45454','#54C472','#FFC169','#E59AFF','#69C1FF'],edgecolor=None,labelAverage=False,title=None,legendPosition='best'):
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
   avgWidth = width*1.2
   index = np.arange(len(entries))
   #sns.set_style('whitegrid', {'legend.frameon': True, 'font.family': [u'serif']})
   opacity = 0.95
   error_config = {'ecolor': 'black'}
   #fig, ax = plt.subplots(figsize=(4, 3))
   fig, ax = plt.subplots(figsize=(16, 4))
   maxHeight = float('-inf')
   minHeight = float('inf')
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
            plt.bar([len(entries)+idx*avgWidth], [np.mean(ys)], avgWidth,
                 alpha=min(opacity*1.20,1.0),
                 color=palette[idx], edgecolor=(edgecolor if edgecolor else palette[idx]))
            height = np.mean(ys)
            print(group, height)
            avg = np.round(np.mean(ys),decimals=decimals)
            if decimals==0:
               avg = int(avg)
            label = str(avg)
            if labelAverage:
               plt.text(len(entries) + idx*avgWidth, height, label, fontsize=10, ha='center', va='bottom',rotation=90)

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
            plt.bar([len(entries)], [np.mean(ys)], width*1.5,
                 alpha=min(opacity*1.20,1.0),
                 color=palette[idx], edgecolor=(edgecolor if edgecolor else palette[idx]))
            height = np.mean(ys)
         
            avg = np.round(np.mean(ys),decimals=decimals)
            if decimals==0:
               avg = int(avg)
            label = str(avg)
            if labelAverage:
               plt.text(len(entries), height, label, fontsize=10, ha='center', va='bottom',rotation=45)

         if plotAverage:
            plt.axvline((len(entries)-1) + width, color='grey', linestyle='dashed', linewidth=1)

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
      index = np.arange(len(entries),dtype=float)
      index[len(entries)-1] += width*len(groups)*0.5

   if groups:
      plt.xticks(index + (len(groups)-1)*width*0.5, entries, rotation=90, ha='center')
      #plt.legend(loc='lower right',fontsize=14)
      #plt.legend(loc='upper left',fontsize=14)
      #plt.legend(loc=legendPosition,fontsize=14, ncol=3)
      plt.legend(loc=legendPosition,fontsize=14)
      #plt.legend(loc='best',fontsize=10)
   else:
      plt.xticks(index, entries, rotation=90, ha='center')

   box = ax.get_position()
   ax.set_position([box.x0, box.y0*1, box.width/3, box.height])

   plt.tick_params(labelsize=14)

   if title:
      plt.title(title)

   plt.tight_layout()
   if filename:
      plt.savefig(filename)
   else:
      plt.show()
   plt.clf()
   plt.close('all')





def onTopBars(dataset, ylabel, plotAverage=True, groups=None, entries=None, decimals=2,filename=None,summary=np.mean,palette = ['#AF1515','#15AF3E','#FFAA32','#DC75FF','#32AAFF','#C45454','#54C472','#FFC169','#E59AFF','#69C1FF'],edgecolor=None,labelAverage=False):
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
      index[len(entries)-1] += 1

   if groups:
      #plt.xticks(index + (len(groups)-1)*width*0.5, entries, rotation=90)
      plt.xticks(index, entries, rotation=30)
      plt.legend(loc='lower left',fontsize=14)
      #plt.legend(loc='upper left',fontsize=14)
      #plt.legend(loc='best',fontsize=14)
      #plt.legend(loc='best',fontsize=10)
   else:
      plt.xticks(index, entries, rotation=90)

   box = ax.get_position()
   ax.set_position([box.x0, box.y0*1, box.width/3, box.height])

   plt.tick_params(labelsize=14)

   plt.tight_layout()
   if filename:
      plt.savefig(filename)
   else:
      plt.show()
   plt.clf()
   plt.close('all')


