import os
import sys
import subprocess
import matplotlib.pyplot
# Check the root version

METHOD = ' -M MarkovChainMC --tries 40 -i 5000 -s -1 -H MarkovChainMC '


rootinfo = os.popen('root -b -q').readlines()
hostinfo = os.popen('hostname').readline()
hostinfo = str(hostinfo)

rootinfo2 = str(rootinfo)

rootinfo = rootinfo[4].split()[2].split('/')[0]
rootinfo = float(rootinfo)

do_mumu = 0
do_munu = 0 
do_combo = 0
do_observedonly = 0
cdir = ''
if 'do_mumu' in str(sys.argv):
	do_mumu = 1
if 'do_munu' in str(sys.argv):
	do_munu = 1
if 'do_combo' in str(sys.argv):
	do_combo = 1
if 'just_observed' in str(sys.argv):
	do_observedonly = 1	
numdo = 1	
queue = '1nd'
launcher = 'launcherMCMC.py'
iters = 1
for x in range(len(sys.argv)):
	if sys.argv[x] == '-c':
		cdir = sys.argv[x+1]
		os.system('rfmkdir /castor/cern.ch/user/d/darinb/MuMu'+cdir)
		os.system('rfmkdir /castor/cern.ch/user/d/darinb/MuNu'+cdir)
		os.system('mkdir '+cdir)
	if sys.argv[x] == '-n':
		numdo = int(sys.argv[x+1])
	if sys.argv[x] == '-q':
		queue = str(sys.argv[x+1])
	if sys.argv[x] == '-l':
		launcher = str(sys.argv[x+1])	
	if sys.argv[x] == '--iters':
		iters = int(sys.argv[x+1])		
from ROOT import *
from array import array
	
beta_combo = []
m_combo = []
dif_combo = []
cr = '  \n'

fullcards = open('FinalCards.txt','r')
mycards = []
for line in fullcards:
	mycards.append(line.replace('\n',''))

name = []
for x in mycards:
	if '.txt' in x:
		name.append((x.replace('.txt','')).replace('\n','')) 




if do_mumu == 1:
	for x in range(len(name)):
		if 'BetaHalf' in name[x]:
			continue
		print 'Calculating limit for: ' + name[x]
		f = open('confmumu'+cdir+'_'+name[x]+'.cfg','w')
		count = 0
		print name[x]
		for l in mycards:
			if count ==1:
				f.write(l+'\n')
			if 'BetaHalf' not in l and '.txt' in l:
				newname = l
				if name[x] in newname:
					print newname
					count = 1
				else:
					count = 0
		os.system('rfmkdir /castor/cern.ch/user/d/darinb/MuMu'+cdir+'/'+name[x])
					
		f.close()
		if (do_observedonly == 0):
			

			mdir = (os.popen('pwd').readlines())[0]
			mdir = mdir.replace('\n','')
			fsub = open('submumu_'+cdir+name[x]+'.csh','w')
			fsub.write('#!/bin/csh'+ cr)
			fsub.write('cd ' + mdir+ cr)
			fsub.write('eval `scramv1 runtime -csh`'+ cr)
			fsub.write('cd -'+ cr)
			fsub.write('cp '+mdir+'/'+launcher+' . '+ cr)
			fsub.write('cp '+mdir+'/confmumu'+cdir+'_'+name[x]+ '.cfg . '+ cr)
			for ii in range(iters):
				fsub.write('python '+launcher+' '+name[x]+' mumu'+cdir+cr)
			fsub.write('cp log*.txt '+mdir+'/'+cdir+'/'+ cr +cr +cr)
			fsub.write('rfcp log*.txt /castor/cern.ch/user/d/darinb/MuMu'+cdir+'/'+ cr +cr +cr)
			fsub.write('rfcp *root /castor/cern.ch/user/d/darinb/MuMu'+cdir+'/'+name[x]+'/'+ cr +cr +cr)			
			fsub.close()
			os.system('chmod 777 *csh')
			for nn in range(numdo):
	
				os.system('bsub -o /dev/null -e /dev/null -q '+queue+' -J jobmumu'+str(nn)+'_'+name[x]+' < submumu_'+cdir+name[x]+'.csh')
	
		if (do_observedonly == 1):
			os.system('combine -v 0 -d confmumu'+'_'+cdir+name[x]+'.cfg '+METHOD) 
			#os.system('combine -v -1 -d confmumu'+'_'+cdir+name[x]+'.cfg -M HybridNew --rule CLs -s -1 --testStat LHC -H ProfileLikelihood --fork 16') 
	



if do_munu == 1:
	
	for x in range(len(name)):
		if 'BetaHalf' not in name[x]:
			continue		
		print 'Calculating limit for: ' + name[x]			
		f = open('confmunu'+cdir+'_'+name[x]+'.cfg','w')
		count = 0
		print name[x]
		for l in mycards:
			if '.txt' in l and 'BetaHalf' not in l:
				break			
			if count ==1:
				f.write(l+'\n')
			if 'BetaHalf' in l and '.txt' in l:
				newname = l
				if name[x].replace('LQ','') in newname:
					print newname
					count = 1
				else:
					count = 0
		os.system('rfmkdir /castor/cern.ch/user/d/darinb/MuNu'+cdir+'/'+name[x])

					
		f.close()
		if (do_observedonly == 0):
					
			mdir = (os.popen('pwd').readlines())[0]
			mdir = mdir.replace('\n','')
			fsub = open('submumu_'+cdir+name[x]+'.csh','w')
			fsub.write('#!/bin/csh'+ cr)
			fsub.write('cd ' + mdir+ cr)
			fsub.write('eval `scramv1 runtime -csh`'+ cr)
			fsub.write('cd -'+ cr)
			fsub.write('cp '+mdir+'/'+launcher+' . '+ cr)
			fsub.write('cp '+mdir+'/confmunu'+cdir+'_'+name[x]+ '.cfg . '+ cr)
			for ii in range(iters):
				fsub.write('python '+launcher+' '+name[x]+' munu'+cdir+cr)
			fsub.write('cp log.txt '+mdir+'/'+cdir+'/'+ cr +cr +cr)
			fsub.write('rfcp log*.txt /castor/cern.ch/user/d/darinb/MuNu'+cdir+'/'+ cr +cr +cr)
			fsub.write('rfcp *root /castor/cern.ch/user/d/darinb/MuNu'+cdir+'/'+name[x]+'/'+ cr +cr +cr)	
			fsub.close()
			os.system('chmod 777 *csh')
			for nn in range(numdo):
	
				os.system('bsub -o /dev/null -e /dev/null -q '+queue+' -J jobmunu'+str(nn)+'_'+name[x]+' < submumu_'+cdir+name[x]+'.csh')			
			
		if (do_observedonly == 1):
			os.system('combine -v 0 -d confmunu'+'_'+cdir+name[x]+'.cfg '+METHOD)
			#os.system('combine -v -1 -d confmunu'+'_'+cdir+name[x]+'.cfg -M HybridNew --rule CLs -s -1 --testStat LEP -H ProfileLikelihood --fork 16') 
