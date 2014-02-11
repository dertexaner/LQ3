import os
import sys
import subprocess
import matplotlib.pyplot
import math 

import random
 #betas = [0.01,0.02,0.03,0.04,0.05,0.06,0.07,0.08,0.09,0.1,0.11,0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.19,0.2,0.21,0.22,0.23,0.24,0.25,0.26,0.27,0.28,0.29,0.3,0.31,0.32,0.33,0.34,0.35,0.36,0.37,0.38,0.39,0.4,0.41,0.42,0.43,0.44,0.45,0.46,0.47,0.48,0.49,0.5,0.51,0.52,0.53,0.54,0.55,0.56,0.57,0.58,0.59,0.6,0.61,0.62,0.63,0.64,0.65,0.66,0.67,0.68,0.69,0.7,0.71,0.72,0.73,0.74,0.75,0.76,0.77,0.78,0.79,0.8,0.81,0.82,0.83,0.84,0.85,0.86,0.87,0.88,0.89,0.9,0.91,0.92,0.93,0.94,0.95,0.96,0.97,0.98,0.99,0.99999]
#betas = [.3,0.5]
#betas = [0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,0.99]
betas = [0.02,0.04,0.06,0.08,0.1,0.12,0.14,0.18,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.82,0.84,0.86,0.88,0.90,0.92,0.94,0.96,0.98,0.9995]
ESTIMATIONMETHOD = ' -M Asymptotic '
METHOD = '-M HybridNew --rule CLs --frequentist CONFIGURATION --clsAcc=0 -s -1 -T 70 -i 70 --singlePoint SINGLEPOINT --saveToys --saveHybridResult'
person = (os.popen('whoami').readlines())[0].replace('\n','')



masses = []
do_BetaOne = 0
do_BetaHalf = 0 
do_combo = 0
do_observedonly = 0
cdir = ''
if 'do_BetaOne' in str(sys.argv):
	do_BetaOne = 1
if 'do_BetaHalf' in str(sys.argv):
	do_BetaHalf = 1
if 'do_Combo' in str(sys.argv):
	do_combo = 1
if 'just_observed' in str(sys.argv):
	do_observedonly = 1	
singlebeta = -1

numdo = 1	
queue = '1nd'
if 'CLSLimits' not in os.listdir('.'):
	os.system('mkdir CLSLimits')
if 'ShellScriptsForBatch' not in os.listdir('.'):
	os.system('mkdir ShellScriptsForBatch')
os.system('rfmkdir /castor/cern.ch/user/'+person[0]+'/'+person+'/CLSLimits'+cdir)

dobatch = True
for x in range(len(sys.argv)):
	if sys.argv[x] == '-c':
		cdir = sys.argv[x+1]
		os.system('rfmkdir /castor/cern.ch/user/'+person[0]+'/'+person+'/CLSLimits/BetaOne'+cdir)
		os.system('rfmkdir /castor/cern.ch/user/'+person[0]+'/'+person+'/CLSLimits/BetaHalf'+cdir)
		os.system('rfmkdir /castor/cern.ch/user/'+person[0]+'/'+person+'/CLSLimits/Combo'+cdir)

		os.system('mkdir CLSLimits/BetaOne'+cdir)
		os.system('mkdir CLSLimits/BetaHalf'+cdir)
		os.system('mkdir CLSLimits/Combo'+cdir)

	if sys.argv[x] == '-n':
		numdo = int(sys.argv[x+1])
	if sys.argv[x] == '-q':
		queue = str(sys.argv[x+1])
	if sys.argv[x] == '--single_beta':
		singlebeta = float(sys.argv[x+1])
	if '--Asymptotic_Only' in sys.argv[x]:
		dobatch = False 
from ROOT import *
from array import array


if singlebeta>0:
	betas = []
	betas.append(singlebeta)
beta_combo = []
m_combo = []
dif_combo = []
cr = '  \n'

fullcards = open('FinalCards.txt','r')
mycards = []
for line in fullcards:
	mycards.append(line.replace('\n',''))

digis = '0123456789'
name = []
for x in mycards:
	if '.txt' in x:
		name.append((x.replace('.txt','')).replace('\n','')) 
		mm = ''
		for a in x:
			if a in digis:
				mm += a
		if int(mm) not in masses:
			masses.append(int(mm))

BetaOneObs = []
BetaOne95down = []
BetaOne95up = []
BetaOne68down = []
BetaOne68up = []
BetaOneExp = []

BetaHalfObs = []
BetaHalf95down = []
BetaHalf95up = []
BetaHalf68down = []
BetaHalf68up = []
BetaHalfExp = []

ComboObs = []
Combo95down = []
Combo95up = []
Combo68down = []
Combo68up = []
ComboExp = []

ComboBetaOneObs = []
ComboBetaOne95down = []
ComboBetaOne68down = []
ComboBetaOneExp = []
ComboBetaOne68up = []
ComboBetaOne95up = []


ComboBetaHalfObs = []
ComboBetaHalf95down = []
ComboBetaHalf68down = []
ComboBetaHalfExp = []
ComboBetaHalf68up = []
ComboBetaHalf95up = []
						
if do_BetaOne == 1:
	for x in range(len(name)):
		if 'BetaHalf' in name[x]:
			continue
		print 'Calculating limit for: ' + name[x]
		f = open('CLSLimits/BetaOne'+cdir+'/confbetaone_'+cdir+'_'+name[x]+'.cfg','w')
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
	
		f.close()

		os.system('rfmkdir /castor/cern.ch/user/'+person[0]+'/'+person+'/CLSLimits/BetaOne'+cdir+'/'+name[x])
		os.system('rfcp CLSLimits/BetaOne'+cdir+'/confbetaone_'+cdir+'_'+name[x]+'.cfg /castor/cern.ch/user/'+person[0]+'/'+person+'/CLSLimits/BetaOne'+cdir+'/')
		os.system('mkdir CLSLimits/BetaOne'+cdir+'/'+name[x])

		mdir = (os.popen('pwd').readlines())[0]
		mdir = mdir.replace('\n','')
		fsub = open('ShellScriptsForBatch/subbetaone_'+cdir+name[x]+'.csh','w')
		fsub.write('#!/bin/csh'+ cr)
		fsub.write('cd ' + mdir+ cr)
		fsub.write('eval `scramv1 runtime -csh`'+ cr)
		fsub.write('cd -'+ cr)
		fsub.write('cp '+mdir+'/CLSLimits/BetaOne'+cdir+'/confbetaone_'+cdir+'_'+name[x]+ '.cfg . '+ cr)
		fsub.write('SUBCOMMAND'+'\n')
		if '--castor_only' not in sys.argv:
			fsub.write('cp *root '+mdir+'/CLSLimits/BetaOne'+cdir+'/'+name[x]+'/'+ cr +cr )					
		fsub.write('rfcp *root /castor/cern.ch/user/'+person[0]+'/'+person+'/CLSLimits/BetaOne'+cdir+'/'+name[x]+'/'+ cr +cr )			
		fsub.close()
		
		## Estimate the r values with Asymptotic CLs
		EstimationInformation = [' r < 0.0000']
		rmax = 1000.0
		breaker = False 
		while 'r < 0.0000' in str(EstimationInformation):
			EstimationInformation = os.popen('combine '+ESTIMATIONMETHOD+' CLSLimits/BetaOne'+cdir+'/confbetaone_'+cdir+'_'+name[x]+'.cfg --rMax '+str(rmax)).readlines()
			if breaker ==True:
				break
			if 'r < 0.0000' not in str(EstimationInformation):
				effrmax = -999999
				for e in EstimationInformation:
					if 'r <'  in e and 'Expected' in e:
						thisrval = e.split('<')[-1]
						thisrval = thisrval.replace('\n','')
						thisrval = float(thisrval)
						if thisrval>effrmax:
							effrmax = thisrval
				rmax = effrmax*15.0
				EstimationInformation = [' r < 0.0000']
				breaker = True
			rmax = rmax/5.0
		## Estimation Complete

		
		expectedlines = []
		for line in EstimationInformation:
			if 'Expected' in line and 'r <' in line:
				expectedlines.append(line.replace('\n',''))
		values = []
		for e in expectedlines:
			print e
			values.append(float(e.split()[-1]))
		
		## Fill the arrays of Asymptotic Values
		for line in EstimationInformation:
			if 'Observed' in line and '<' in line:
				BetaOneObs.append((line.split('<')[-1]).replace('\n',''))
			if 'Expected' in line and '<' in line:
				if '2.5%' in line:
					BetaOne95down.append((line.split('<')[-1]).replace('\n',''))
				if '16.0%' in line:
					BetaOne68down.append((line.split('<')[-1]).replace('\n',''))
				if '50.0%' in line:
					BetaOneExp.append((line.split('<')[-1]).replace('\n',''))
				if '84.0%' in line:
					BetaOne68up.append((line.split('<')[-1]).replace('\n',''))
				if '97.5%' in line:
					BetaOne95up.append((line.split('<')[-1]).replace('\n',''))

		vstart = round((min(values)/3),5)
		vstop = round((max(values)*3),5)
		rvalues = []
		interval = abs(vstop-vstart)/100.0
		
		nindex = 0
		thisr = 0
		while thisr<vstop:
			thisr = vstart*1.05**(float(nindex))
			rvalues.append(thisr)
			nindex +=1
		strRvalues = []
		for r in rvalues:
			strRvalues.append(str(round(r,5)))
		print strRvalues
		
		for r in strRvalues:
			command = 'combine '+METHOD.replace('SINGLEPOINT',r).replace('CONFIGURATION','confbetaone_'+cdir+'_'+name[x]+'.cfg')
			strR = r.replace('.','_')
			os.system('cat ShellScriptsForBatch/subbetaone_'+cdir+name[x]+'.csh | sed  \'s/SUBCOMMAND/'+command+'/g\'  > ShellScriptsForBatch/subbetaone_'+strR+'_'+cdir+name[x]+'.csh')
			os.system('chmod 777 ShellScriptsForBatch/subbetaone_'+strR+'_'+cdir+name[x]+'.csh')

			for nn in range(numdo):
				if (dobatch):
					os.system('bsub -o /dev/null -e /dev/null -q '+queue+' -J jobbetaone'+str(nn)+'_R_'+strR+'_'+name[x]+' < ShellScriptsForBatch/subbetaone_'+strR+'_'+cdir+name[x]+'.csh')

if do_BetaHalf == 1:
	
	for x in range(len(name)):
		if 'BetaHalf' not in name[x]:
			continue		
		print 'Calculating limit for: ' + name[x]			
		f = open('CLSLimits/BetaHalf'+cdir+'/confbetahalf_'+cdir+'_'+name[x]+'.cfg','w')
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
		f.close()

		os.system('rfmkdir /castor/cern.ch/user/'+person[0]+'/'+person+'/CLSLimits/BetaHalf'+cdir+'/'+name[x])
		os.system('rfcp CLSLimits/BetaHalf'+cdir+'/confbetahalf_'+cdir+'_'+name[x]+'.cfg /castor/cern.ch/user/'+person[0]+'/'+person+'/CLSLimits/BetaHalf'+cdir+'/')
	
		os.system('mkdir CLSLimits/BetaHalf'+cdir+'/'+name[x])

		mdir = (os.popen('pwd').readlines())[0]
		mdir = mdir.replace('\n','')
		fsub = open('ShellScriptsForBatch/subbetahalf_'+cdir+name[x]+'.csh','w')
		fsub.write('#!/bin/csh'+ cr)
		fsub.write('cd ' + mdir+ cr)
		fsub.write('eval `scramv1 runtime -csh`'+ cr)
		fsub.write('cd -'+ cr)
		fsub.write('cp '+mdir+'/CLSLimits/BetaHalf'+cdir+'/confbetahalf_'+cdir+'_'+name[x]+ '.cfg . '+ cr)
		fsub.write('SUBCOMMAND'+'\n')
		if '--castor_only' not in sys.argv:
			fsub.write('cp *root '+mdir+'/CLSLimits/BetaHalf'+cdir+'/'+name[x]+'/'+ cr +cr )					
		fsub.write('rfcp *root /castor/cern.ch/user/'+person[0]+'/'+person+'/CLSLimits/BetaHalf'+cdir+'/'+name[x]+'/'+ cr +cr )			
		fsub.close()
		
		## Estimate the r values with Asymptotic CLs
		EstimationInformation = [' r < 0.0000']
		rmax = 1000.0
		breaker = False 
		while 'r < 0.0000' in str(EstimationInformation):
			EstimationInformation = os.popen('combine '+ESTIMATIONMETHOD+' CLSLimits/BetaHalf'+cdir+'/confbetahalf_'+cdir+'_'+name[x]+'.cfg --rMax '+str(rmax)).readlines()
			if breaker ==True:
				break
			if 'r < 0.0000' not in str(EstimationInformation):
				effrmax = -999999
				for e in EstimationInformation:
					if 'r <'  in e and 'Expected' in e:
						thisrval = e.split('<')[-1]
						thisrval = thisrval.replace('\n','')
						thisrval = float(thisrval)
						if thisrval>effrmax:
							effrmax = thisrval
				rmax = effrmax*15.0
				EstimationInformation = [' r < 0.0000']
				breaker = True
			rmax = rmax/5.0
		## Estimation Complete
		
		expectedlines = []
		for line in EstimationInformation:
			if 'Expected' in line and 'r <' in line:
				expectedlines.append(line.replace('\n',''))
		values = []
		for e in expectedlines:
			print e
			values.append(float(e.split()[-1]))

		## Fill the arrays of Asymptotic Values
		for line in EstimationInformation:
			if 'Observed' in line and '<' in line:
				BetaHalfObs.append((line.split('<')[-1]).replace('\n',''))
			if 'Expected' in line and '<' in line:
				if '2.5%' in line:
					BetaHalf95down.append((line.split('<')[-1]).replace('\n',''))
				if '16.0%' in line:
					BetaHalf68down.append((line.split('<')[-1]).replace('\n',''))
				if '50.0%' in line:
					BetaHalfExp.append((line.split('<')[-1]).replace('\n',''))
				if '84.0%' in line:
					BetaHalf68up.append((line.split('<')[-1]).replace('\n',''))
				if '97.5%' in line:
					BetaHalf95up.append((line.split('<')[-1]).replace('\n',''))
		
		vstart = round((min(values)/3),5)
		vstop = round((max(values)*3),5)
		rvalues = []
		interval = abs(vstop-vstart)/100.0
		
		nindex = 0
		thisr = 0
		while thisr<vstop:
			thisr = vstart*1.05**(float(nindex))
			rvalues.append(thisr)
			nindex += 1
		strRvalues = []
		for r in rvalues:
			strRvalues.append(str(round(r,5)))
		print strRvalues
		
		for r in strRvalues:
			command = 'combine '+METHOD.replace('SINGLEPOINT',r).replace('CONFIGURATION','confbetahalf_'+cdir+'_'+name[x]+'.cfg')
			strR = r.replace('.','_')
			os.system('cat ShellScriptsForBatch/subbetahalf_'+cdir+name[x]+'.csh | sed  \'s/SUBCOMMAND/'+command+'/g\'  > ShellScriptsForBatch/subbetahalf_'+strR+'_'+cdir+name[x]+'.csh')
			os.system('chmod 777 ShellScriptsForBatch/subbetahalf_'+strR+'_'+cdir+name[x]+'.csh')

			for nn in range(numdo):
				if (dobatch):
					os.system('bsub -o /dev/null -e /dev/null -q '+queue+' -J jobbetahalf'+str(nn)+'_R_'+strR+'_'+name[x]+' < ShellScriptsForBatch/subbetahalf_'+strR+'_'+cdir+name[x]+'.csh')
					

################################################################################################################
################################################################################################################

if do_combo == 1:
	digits = '0123456789'

	cards = []
	cardmasses = []
	cardcontent = []
	card = ''

	flog = open('FinalCards.txt','r')
	os.system('rm -r TMPComboCards/; mkdir TMPComboCards')	
	for line in flog:
		if '.txt' not in line:
			card += line
		if '.txt' in line:
			cardcontent.append(card)
	
			card  = ''
			
			line = line.replace('.txt\n','')
			cards.append(line)
			m = ''
			for x in line:
				if x in digits:
					m+=(x)
			cardmasses.append(m)
			
	cardcontent.append(card)
	
	cardcontent = cardcontent[1:]
	combocards = []
	for x in cardcontent:
		for y in cards:
			if y in x:
				fout = open('combocard_'+y+'.cfg','w')
				x = x.replace('stat_','stat_'+y)
				fout.write(x)
				fout.close()
				combocards.append('combocard_'+y+'.cfg')
	uniquecardmasses = []
	for x in cardmasses:
		if x not in uniquecardmasses:
			uniquecardmasses.append(x)
	for m in uniquecardmasses:
		print 'Printing cards for mass' +str(m)
		pair = []
		for x in combocards:
			if m in x:
				pair.append(x)
		if 'BetaHalf' in pair[0]:
			bcard = pair[0]
			ocard = pair[1]
		if 'BetaHalf' not in pair[0]:
			bcard = pair[1]
			ocard = pair[0]			

		if '900' not in m:
			os.system('combineCards.py '+pair[0]+ ' '+pair[1]+ '  > TMPComboCards/combocard_COMBO_M_'+m+'.cfg ' )
			os.system('combineCards.py '+bcard+' > TMPComboCards/combocard_COMBO_BetaHalf_M_'+m+'.cfg ' )
			os.system('combineCards.py '+ocard+' > TMPComboCards/combocard_COMBO_BetaOne_M_'+m+'.cfg ' )

		if '900' not in m:
			os.system('combineCards.py '+ocard+ '  > TMPComboCards/combocard_COMBO_M_'+m+'.cfg ' )
			os.system('combineCards.py '+bcard+' > TMPComboCards/combocard_COMBO_BetaHalf_M_'+m+'.cfg ' )
			os.system('combineCards.py '+ocard+' > TMPComboCards/combocard_COMBO_BetaOne_M_'+m+'.cfg ' )

		print pair
	for m in uniquecardmasses:
		combocards.append('TMPComboCards/combocard_COMBO_M_'+m+'.cfg')

	betaind = -1

	for beta in betas:
		betaind += 1
		ComboObs.append([])
		Combo95down.append([])
		Combo68down.append([])
		ComboExp.append([])
		Combo68up.append([])
		Combo95up.append([])

		ComboBetaOneObs.append([])
		ComboBetaOne95down.append([])
		ComboBetaOne68down.append([])
		ComboBetaOneExp.append([])
		ComboBetaOne68up.append([])
		ComboBetaOne95up.append([])
		

		ComboBetaHalfObs.append([])
		ComboBetaHalf95down.append([])
		ComboBetaHalf68down.append([])
		ComboBetaHalfExp.append([])
		ComboBetaHalf68up.append([])
		ComboBetaHalf95up.append([])
						
								
		betaval = str(beta).replace('.','_')
		os.system('mkdir CLSLimits/Combo'+cdir+'/Combo_beta_'+betaval)
		os.system('rfmkdir /castor/cern.ch/user/'+person[0]+'/'+person+'/CLSLimits/Combo'+cdir+'/Combo_beta_'+betaval)


		for x in range(len(uniquecardmasses)):	
			print 'Calculating limit for combination mass: ' + str(uniquecardmasses[x]) + ' , beta = '+str(beta)			
			newcard = ('CLSLimits/Combo'+cdir+'/combocard_COMBO_M_'+uniquecardmasses[x]+'.cfg').replace('COMBO','beta_'+betaval+'_COMBO')
			newcastorcard = ( '/castor/cern.ch/user/'+person[0]+'/'+person+'/CLSLimits/Combo'+cdir+'/combocard_COMBO_M_'+uniquecardmasses[x]+'.cfg').replace('COMBO','beta_'+betaval+'_COMBO')

			newcard_BetaOne = ('CLSLimits/Combo'+cdir+'/combocard_COMBO_BetaOne_M_'+uniquecardmasses[x]+'.cfg').replace('COMBO_BetaOne','beta_'+betaval+'_COMBO_BetaOne')
			newcastorcard_BetaOne = ( '/castor/cern.ch/user/'+person[0]+'/'+person+'/CLSLimits/Combo'+cdir+'/combocard_COMBO_BetaOne_M_'+uniquecardmasses[x]+'.cfg').replace('COMBO_BetaOne','beta_'+betaval+'_COMBO_BetaOne')
			
			newcard_BetaHalf = ('CLSLimits/Combo'+cdir+'/combocard_COMBO_BetaHalf_M_'+uniquecardmasses[x]+'.cfg').replace('COMBO_BetaHalf','beta_'+betaval+'_COMBO_BetaHalf')
			newcastorcard_BetaHalf = ( '/castor/cern.ch/user/'+person[0]+'/'+person+'/CLSLimits/Combo'+cdir+'/combocard_COMBO_BetaHalf_M_'+uniquecardmasses[x]+'.cfg').replace('COMBO_BetaHalf','beta_'+betaval+'_COMBO_BetaHalf')
			
			ctypes = ['','_BetaOne','_BetaHalf']
			for c in ctypes:
				tcard = newcard
				if 'BetaHalf' in c:
					tcard = newcard_BetaHalf
				if 'BetaOne' in c:
					tcard = newcard_BetaOne
				ftmp = open(tcard,'w')
				fnorm = open('TMPComboCards/combocard_COMBO'+c+'_M_'+str(uniquecardmasses[x])+'.cfg','r')
				
				betahalfplace = 99
				betaoneplace = 99
				for line in fnorm:
	
					if ('LQ' in line and 'process' in line):
						linesplit = line.split()
						for place in range(len(linesplit)):
							if 'LQ' in linesplit[place] and 'BetaHalf' in linesplit[place]:
								betahalfplace = place
							if 'LQ' in linesplit[place] and 'BetaHalf' not in linesplit[place]:
								betaoneplace = place
								
					if ( 'rate' in line):
	
						linesplit = line.split()
						linesplit2 = []
						for place in range(len(linesplit)):
							arg = linesplit[place]
							if betahalfplace == place:
								arg = str(float(arg)*beta*(1.0-beta)*4.0)
							
							if betaoneplace == place:						
								arg = str(float(arg)*beta*beta)
							linesplit2.append(arg)
						line2 = ''
						for xpart in linesplit2:
							line2 += xpart + '    '
						line2 += '\n'
						line = line2
					if  'stat' in line and 'sig' in line and 'gmN' in line:
						linesplit = line.split()
						print linesplit
						for nsp in range(len(linesplit)):
							if 'BetaHalf' not in line and nsp == betaoneplace+1:
								repsold =  str(linesplit[nsp+1])
								repsnew = str(float(repsold)*beta*beta)
								line = line.replace(repsold,repsnew)
							if 'BetaHalf' in line and nsp == betahalfplace+1:
								repsold =  str(linesplit[nsp+1])
								repsnew = str(float(repsold)*beta*(1.0-beta)*4.0)
								line = line.replace(repsold,repsnew)
					ftmp.write(line)
				ftmp.close()
			if (dobatch==True):
				os.system('rfcp '+newcard+' '+newcastorcard)
				os.system('rfcp '+newcard_BetaOne+' '+newcastorcard_BetaOne)
				os.system('rfcp '+newcard_BetaHalf+' '+newcastorcard_BetaHalf)

		
			thisname ='beta_'+ betaval + '_M_'+str(uniquecardmasses[x])
			mdir = (os.popen('pwd').readlines())[0]
			mdir = mdir.replace('\n','')
			fsub = open('ShellScriptsForBatch/subcombo_'+cdir+thisname+'.csh','w')
			fsub.write('#!/bin/csh'+ cr)
			fsub.write('cd ' + mdir+ cr)
			fsub.write('eval `scramv1 runtime -csh`'+ cr)
			fsub.write('cd -'+ cr)
			fsub.write('cp '+mdir+'/'+newcard+' .'+ cr)
			fsub.write('cp '+mdir+'/'+newcard_BetaOne+' .'+ cr)
			fsub.write('cp '+mdir+'/'+newcard_BetaHalf+' .'+ cr)

			fsub.write('SUBCOMMAND0'+'\n')
			fsub.write('mv higgs*root Combo_'+thisname+'_R_RVALUE0_ind_`bash -c \'echo $RANDOM\'`.root \n')			
			fsub.write('SUBCOMMAND1'+'\n')
			fsub.write('mv higgs*root ComboBetaOne_'+thisname+'_R_RVALUE1_ind_`bash -c \'echo $RANDOM\'`.root \n')			
			fsub.write('SUBCOMMAND2'+'\n')
			fsub.write('mv higgs*root ComboBetaHalf_'+thisname+'_R_RVALUE2_ind_`bash -c \'echo $RANDOM\'`.root \n')						
			
			if '--castor_only' not in sys.argv:
				fsub.write('cp *root '+mdir+'/CLSLimits/Combo'+cdir+'/Combo_beta_'+betaval+'/ \n' )					
			fsub.write('rfcp *root /castor/cern.ch/user/'+person[0]+'/'+person+'/CLSLimits/Combo'+cdir+'/Combo_beta_'+betaval+'/' +cr +cr )			

			fsub.close()
			
			## Estimate the r values with Asymptotic CLs
			EstimationInformation0 = [' r < 0.0000']
			rmax = 1000.0
			breaker = False 
			while 'r < 0.0000' in str(EstimationInformation0):
				print 'combine '+ESTIMATIONMETHOD+' '+newcard +' --rMax '+str(rmax)
				EstimationInformation0 = os.popen('combine '+ESTIMATIONMETHOD+' '+newcard +' --rMax '+str(rmax)).readlines()
				
				if breaker ==True:
					break
				if 'r < 0.0000' not in str(EstimationInformation0):
					effrmax = -999999
					for e in EstimationInformation0:
						if 'r <'  in e and 'Expected' in e:
							thisrval = e.split('<')[-1]
							thisrval = thisrval.replace('\n','')
							thisrval = float(thisrval)
							if thisrval>effrmax:
								effrmax = thisrval
					rmax = effrmax*15.0
					EstimationInformation0 = [' r < 0.0000']
					breaker = True
				rmax = rmax/5.0


			EstimationInformation1 = [' r < 0.0000']
			rmax = 1000.0
			breaker = False 
			while 'r < 0.0000' in str(EstimationInformation1):
				print 'combine '+ESTIMATIONMETHOD+' '+newcard_BetaOne +' --rMax '+str(rmax)
				EstimationInformation1 = os.popen('combine '+ESTIMATIONMETHOD+' '+newcard_BetaOne +' --rMax '+str(rmax)).readlines()
				
				if breaker ==True:
					break
				if 'r < 0.0000' not in str(EstimationInformation1):
					effrmax = -999999
					for e in EstimationInformation1:
						if 'r <'  in e and 'Expected' in e:
							thisrval = e.split('<')[-1]
							thisrval = thisrval.replace('\n','')
							thisrval = float(thisrval)
							if thisrval>effrmax:
								effrmax = thisrval
					rmax = effrmax*15.0
					EstimationInformation1 = [' r < 0.0000']
					breaker = True
				rmax = rmax/5.0
				
				
			EstimationInformation2 = [' r < 0.0000']
			rmax = 1000.0
			breaker = False 
			while 'r < 0.0000' in str(EstimationInformation2):
				print 'combine '+ESTIMATIONMETHOD+' '+newcard_BetaHalf +' --rMax '+str(rmax)
				EstimationInformation2 = os.popen('combine '+ESTIMATIONMETHOD+' '+newcard_BetaHalf +' --rMax '+str(rmax)).readlines()
				
				if breaker ==True:
					break
				if 'r < 0.0000' not in str(EstimationInformation2):
					effrmax = -999999
					for e in EstimationInformation2:
						if 'r <'  in e and 'Expected' in e:
							thisrval = e.split('<')[-1]
							thisrval = thisrval.replace('\n','')
							thisrval = float(thisrval)
							if thisrval>effrmax:
								effrmax = thisrval
					rmax = effrmax*15.0
					EstimationInformation2 = [' r < 0.0000']
					breaker = True
				rmax = rmax/5.0				

			
			expectedlines0 = []
			for line in EstimationInformation0:
				if 'Expected' in line and 'r <' in line:
					expectedlines0.append(line.replace('\n',''))
			values0 = []
			for e in expectedlines0:
				print e
				values0.append(float(e.split()[-1]))


			expectedlines1 = []
			for line in EstimationInformation1:
				if 'Expected' in line and 'r <' in line:
					expectedlines1.append(line.replace('\n',''))
			values1 = []
			for e in expectedlines1:
				print e
				values1.append(float(e.split()[-1]))
				
				
			expectedlines2 = []
			for line in EstimationInformation2:
				if 'Expected' in line and 'r <' in line:
					expectedlines2.append(line.replace('\n',''))
			values2 = []
			for e in expectedlines2:
				print e
				values2.append(float(e.split()[-1]))				

			## Estimation Complete


			## Fill the arrays of Asymptotic Values
			for line in EstimationInformation0:
				if 'Observed' in line and '<' in line:
					ComboObs[betaind].append((line.split('<')[-1]).replace('\n',''))
				if 'Expected' in line and '<' in line:
					if '2.5%' in line:
						Combo95down[betaind].append((line.split('<')[-1]).replace('\n',''))
					if '16.0%' in line:
						Combo68down[betaind].append((line.split('<')[-1]).replace('\n',''))
					if '50.0%' in line:
						ComboExp[betaind].append((line.split('<')[-1]).replace('\n',''))
					if '84.0%' in line:
						Combo68up[betaind].append((line.split('<')[-1]).replace('\n',''))
					if '97.5%' in line:
						Combo95up[betaind].append((line.split('<')[-1]).replace('\n',''))

			for line in EstimationInformation1:
				if 'Observed' in line and '<' in line:
					ComboBetaOneObs[betaind].append((line.split('<')[-1]).replace('\n',''))
				if 'Expected' in line and '<' in line:
					if '2.5%' in line:
						ComboBetaOne95down[betaind].append((line.split('<')[-1]).replace('\n',''))
					if '16.0%' in line:
						ComboBetaOne68down[betaind].append((line.split('<')[-1]).replace('\n',''))
					if '50.0%' in line:
						ComboBetaOneExp[betaind].append((line.split('<')[-1]).replace('\n',''))
					if '84.0%' in line:
						ComboBetaOne68up[betaind].append((line.split('<')[-1]).replace('\n',''))
					if '97.5%' in line:
						ComboBetaOne95up[betaind].append((line.split('<')[-1]).replace('\n',''))

			for line in EstimationInformation2:
				if 'Observed' in line and '<' in line:
					ComboBetaHalfObs[betaind].append((line.split('<')[-1]).replace('\n',''))
				if 'Expected' in line and '<' in line:
					if '2.5%' in line:
						ComboBetaHalf95down[betaind].append((line.split('<')[-1]).replace('\n',''))
					if '16.0%' in line:
						ComboBetaHalf68down[betaind].append((line.split('<')[-1]).replace('\n',''))
					if '50.0%' in line:
						ComboBetaHalfExp[betaind].append((line.split('<')[-1]).replace('\n',''))
					if '84.0%' in line:
						ComboBetaHalf68up[betaind].append((line.split('<')[-1]).replace('\n',''))
					if '97.5%' in line:
						ComboBetaHalf95up[betaind].append((line.split('<')[-1]).replace('\n',''))
			
			## Asymptotic Information Filled
			
			
			vstart0 = round((min(values0)/2),5)
			vstop0 = round((max(values0)*2),5)
			rvalues0 = []


			vstart1 = round((min(values1)/2),5)
			vstop1 = round((max(values1)*2),5)
			rvalues1 = []
			
			vstart2 = round((min(values2)/2),5)
			vstop2 = round((max(values2)*2),5)
			rvalues2 = []			
			
			nindex0 = 0
			thisr0 = 0

			nindex1 = 0
			thisr1= 0
			
			nindex2 = 0
			thisr2 = 0
			
			print 'vstop0 '+str(vstop0) + '   vstart0  '+str(vstart0)
			print 'vstop1 '+str(vstop1) + '   vstart1  '+str(vstart1)
			print 'vstop2 '+str(vstop2) + '   vstart2  '+str(vstart2)

			expinc0 = 0.9999*(2.718281828**((0.0666666666667*(math.log(vstop0/vstart0)))))
			expinc1 = 0.9999*(2.718281828**((0.0666666666667*(math.log(vstop1/vstart1)))))
			expinc2 = 0.9999*(2.718281828**((0.0666666666667*(math.log(vstop2/vstart2)))))
			
			
			while thisr0<vstop0:
				thisr0 = vstart0*expinc0**(float(nindex0))
				rvalues0.append(thisr0)
				nindex0 += 1
			strRvalues0 = []
			for r in rvalues0:
				strRvalues0.append(str(round(r,5)))
			#print strRvalues0


			while thisr1<vstop1:
				thisr1 = vstart1*expinc1**(float(nindex1))
				rvalues1.append(thisr1)
				nindex1 += 1
			strRvalues1 = []
			for r in rvalues1:
				strRvalues1.append(str(round(r,5)))
			#print strRvalues1

			while thisr2<vstop2:
				thisr2 = vstart2*expinc2**(float(nindex2))
				rvalues2.append(thisr2)
				nindex2 += 1
			strRvalues2 = []
			for r in rvalues2:
				strRvalues2.append(str(round(r,5)))
			#print strRvalues2
			

			
			for rind in range(len(strRvalues0)):
				r0 = strRvalues0[rind]
				r1 = strRvalues1[rind]
				r2 = strRvalues2[rind]

				command0 = 'combine '+METHOD.replace('SINGLEPOINT',r0).replace('CONFIGURATION',newcard.split('/')[-1])
				command1 = 'combine '+METHOD.replace('SINGLEPOINT',r1).replace('CONFIGURATION',newcard_BetaOne.split('/')[-1])
				command2 = 'combine '+METHOD.replace('SINGLEPOINT',r2).replace('CONFIGURATION',newcard_BetaHalf.split('/')[-1])

				strR0 = r0.replace('.','_')
				strR1 = r1.replace('.','_')
				strR2 = r2.replace('.','_')

				os.system('cat ShellScriptsForBatch/subcombo_'+cdir+thisname+'.csh | sed  \'s/SUBCOMMAND0/'+command0+'/g\' | sed  \'s/RVALUE0/'+strR0+'/g\' | sed  \'s/SUBCOMMAND1/'+command1+'/g\' | sed  \'s/RVALUE1/'+strR1+'/g\' | sed  \'s/SUBCOMMAND2/'+command2+'/g\' | sed  \'s/RVALUE2/'+strR2+'/g\' > ShellScriptsForBatch/subcombo_R_'+str(rind)+'_'+cdir+thisname+'.csh')
				os.system('chmod 777 ShellScriptsForBatch/subcombo_R_'+str(rind)+'_'+cdir+thisname+'.csh')
	
				for nn in range(numdo):
					if (dobatch):
						os.system('bsub -o /dev/null -e /dev/null -q '+queue+' -J jobcombo'+str(nn)+'_R_'+str(rind)+'_'+thisname+' < ShellScriptsForBatch/subcombo_R_'+str(rind)+'_'+cdir+thisname+'.csh')


################################################################################################################
################################################################################################################






os.system('rm higgsCombineTest*root')

print '\n\n\n'

#### ASYMPTOTIC CLS PRINTOUT ###


mTh = [ 150, 200, 250, 300, 350, 400,450,500,550,600,650,700,750,800,850,900]
xsTh = [  53.3, 11.9, 3.47, 1.21, 0.477, .205,.0949,.0463,.0236,.0124,.00676,.00377,.00215,.00124,.000732,.000436]

mThold= [ 150, 200, 250, 300, 350, 400,450,500,550,600,650,700,750,800,850]
xsThold = [  53.3, 11.9, 3.47, 1.21, 0.477, .205,.0949,.0463,.0236,.0124,.00676,.00377,.00215,.00124,.000732]





#### BETA ONE CHANNEL
if do_BetaOne == 1:

	print "*"*40 + '\n BETA ONE ASYMPTOTIC CLS RESULTS\n\n' +"*"*40
	
	band1sigma = 'Double_t y_1sigma[22]={'
	band2sigma = 'Double_t y_2sigma[22]={'
	excurve = 'Double_t xsUp_expected[11] = {' 
	obcurve = 'Double_t xsUp_observed[11] = {'  
	
	ob = BetaOneObs 
	down2 = BetaOne95down 
	up2 = BetaOne95up 
	down1 = BetaOne68down 
	up1 = BetaOne68up 
	med = BetaOneExp 
	
	fac = 1.0
	sigma = []
	for x in range(len(mTh)):
		if (mTh[x]) in masses: 
			sigma.append(xsTh[x]*fac)
	
	for x in range(len(masses)):
		excurve += str(float(med[x])*float(sigma[x])) + ' , ' 
		obcurve += str(float(ob[x])*float(sigma[x])) + ' , ' 
		band1sigma += str(float(down1[x])*float(sigma[x])) + ' , ' 
		band2sigma += str(float(down2[x])*float(sigma[x])) + ' , ' 
	
	for x in range(len(masses)):
		band1sigma += str(float(up1[-(x+1)])*float(sigma[-(x+1)])) + ' , ' 
		band2sigma += str(float(up2[-(x+1)])*float(sigma[-(x+1)])) + ' , ' 
	excurve += '}'
	obcurve += '}'
	band1sigma += '}'
	band2sigma += '}'
	excurve = excurve.replace(' , }',' }; ' )
	obcurve = obcurve.replace(' , }',' }; ' )
	band1sigma = band1sigma.replace(' , }',' }; ' )
	band2sigma = band2sigma.replace(' , }',' }; ' )
	
	print '\n'
	print excurve
	print '\n'
	print obcurve
	print '\n'
	print band1sigma
	print '\n'
	print band2sigma
	print '\n'
	print '\n'


#### BETA HALF CHANNEL
if do_BetaHalf == 1:

	print "*"*40 + '\n BETA HALF ASYMPTOTIC CLS RESULTS\n\n' +"*"*40
	
	band1sigma = 'Double_t y_1sigma[20]={'
	band2sigma = 'Double_t y_2sigma[20]={'
	excurve = 'Double_t xsUp_expected[10] = {' 
	obcurve = 'Double_t xsUp_observed[10] = {'  
	
	ob = BetaHalfObs 
	down2 = BetaHalf95down 
	up2 = BetaHalf95up 
	down1 = BetaHalf68down 
	up1 = BetaHalf68up 
	med = BetaHalfExp 
	
	fac = 0.5
	sigma = []
	for x in range(len(mThold)):
		if (mTh[x]) in masses: 
			sigma.append(xsTh[x]*fac)
	
	for x in range(len(masses)):
		excurve += str(float(med[x])*float(sigma[x])) + ' , ' 
		obcurve += str(float(ob[x])*float(sigma[x])) + ' , ' 
		band1sigma += str(float(down1[x])*float(sigma[x])) + ' , ' 
		band2sigma += str(float(down2[x])*float(sigma[x])) + ' , ' 
	
	for x in range(len(masses)):
		band1sigma += str(float(up1[-(x+1)])*float(sigma[-(x+1)])) + ' , ' 
		band2sigma += str(float(up2[-(x+1)])*float(sigma[-(x+1)])) + ' , ' 
	excurve += '}'
	obcurve += '}'
	band1sigma += '}'
	band2sigma += '}'
	excurve = excurve.replace(' , }',' }; ' )
	obcurve = obcurve.replace(' , }',' }; ' )
	band1sigma = band1sigma.replace(' , }',' }; ' )
	band2sigma = band2sigma.replace(' , }',' }; ' )
	
	print '\n'
	print excurve
	print '\n'
	print obcurve
	print '\n'
	print band1sigma
	print '\n'
	print band2sigma
	print '\n'
	print '\n'
	
	
	


#### COMBINATION CHANNEL
if do_combo == 1:

	print "*"*40 + '\n COMBINATION ASYMPTOTIC CLS RESULTS\n\n' +"*"*40

					
	mTh = [ 150, 200, 250, 300, 350, 400,450,500,550,600,650,700,750,800,850,900]
	xsTh = [  53.3, 11.9, 3.47, 1.21, 0.477, .205,.0949,.0463,.0236,.0124,.00676,.00377,.00215,.00124,.000732,.000436]
	sigma = []
	fac = 1.0
	for x in range(len(mTh)):
		if (mTh[x]) in masses: 
			sigma.append(xsTh[x]*fac)
	
	def re_eval(rlist):
		outlist = []
		for mset in rlist:
			newset = []
			for r_ind in range(len(mset)):
				newset.append(sigma[r_ind]*float(mset[r_ind]))		
			outlist.append(newset)
		return outlist
	
	s_ComboObs = re_eval(ComboObs)
	s_Combo95down = re_eval(Combo95down)
	s_Combo68down = re_eval(Combo68down)
	s_ComboExp = re_eval(ComboExp)
	s_Combo68up = re_eval(Combo68up)
	s_Combo95up = re_eval(Combo95up)
	
	s_ComboBetaOneObs = re_eval(ComboBetaOneObs)
	s_ComboBetaOne95down = re_eval(ComboBetaOne95down)
	s_ComboBetaOne68down = re_eval(ComboBetaOne68down)
	s_ComboBetaOneExp = re_eval(ComboBetaOneExp)
	s_ComboBetaOne68up = re_eval(ComboBetaOne68up)
	s_ComboBetaOne95up = re_eval(ComboBetaOne95up)
	
	s_ComboBetaHalfObs = re_eval(ComboBetaHalfObs)
	s_ComboBetaHalf95down = re_eval(ComboBetaHalf95down)
	s_ComboBetaHalf68down = re_eval(ComboBetaHalf68down)
	s_ComboBetaHalfExp = re_eval(ComboBetaHalfExp)
	s_ComboBetaHalf68up = re_eval(ComboBetaHalf68up)
	s_ComboBetaHalf95up = re_eval(ComboBetaHalf95up)


	from ROOT import *
	from array import array
		
	mTh = array("d",[ 150, 200, 250, 300, 350, 400,450,500,550,600,650,700,750,800,850,900])
	xsTh = array("d",[  53.3, 11.9, 3.47, 1.21, 0.477, .205,.0949,.0463,.0236,.0124,.00676,.00377,.00215,.00124,.000732,.000436])
	
	
	g = TGraph(len(mTh),mTh,xsTh);
	spline = TSpline3("xsection",g) 
	#xx = (spline.Eval(310));
	M_th=[ 150, 200, 250, 300, 350, 400,450,500,550,600,650,700,750,800,850,900]
	X_th=[  53.3, 11.9, 3.47, 1.21, 0.477, .205,.0949,.0463,.0236,.0124,.00676,.00377,.00215,.00124,.000732,.000436]
		
		
	def sigmaval(mval):
		return spline.Eval(mval)
		
	
	def mval(sigma):
		testm = 150
		oldtestm = 800
		inc = 50
		dif = 55
		olddif = 000
		while abs(oldtestm - testm)>0.01:
			testsigma = sigmaval(testm)
			olddif = dif
			dif = testsigma -sigma
			if testm>1000:
				break
			if dif*olddif <= 0.0:
				inc = -inc/2.3
			oldtestm = testm
			#print '**' + str(testm) + '  ' + str(testsigma) +'  ' +str(dif) + '   ' + str(dif*olddif)
	
			testm = testm + inc
		return testm
	import math
	inputarrayX = []
	inputarrayY = []

	def loggraph(inputarrayX,inputarrayY):
		logarray = []
		for j in inputarrayY:
			logarray.append(math.log10(j))
		x = array("d",inputarrayX)
		y = array("d",logarray)
		g = TGraph(len(x),x,y)
		return g
		
	logtheory = loggraph(M_th,X_th)

	def logspline(inputarrayX,inputarrayY):
		logarray = []
		for j in inputarrayY:
			logarray.append(math.log(j))
		x = array("d",inputarrayX)
		y = array("d",logarray)
		g = TGraph(len(x),x,y)
		outspline = TSpline3("",g)
		return outspline
	
	from math import exp
	def get_intersection(spline1, spline2, xmin,xmax):
		num = xmax-xmin
		inc = (xmax - xmin)/num
		dif = []
		sdif = []
		x = xmin
		xvals = []
		xx = []
		yy = []
		xvals = []
		while x<xmax:
			thisdif = (exp(spline1.Eval(x)) - exp(spline2.Eval(x)))
			#print (str(x)) + '   ' + str(thisdif)
			xx.append(exp(spline1.Eval(x)))
			yy.append(exp(spline2.Eval(x)))
			sdif.append(thisdif)
			dif.append(abs(thisdif))
			xvals.append(x)
			#print  str(x) + '   ' +str(exp(spline1.Eval(x))) + '    '+str(exp(spline2.Eval(x))) + '    ' + str(thisdif)
			x = x+inc
		mindif = min(dif)
		bestmass = 0	
		
	
		for x in range(len(dif)-2):
			a = sdif[x]
			b = sdif[x+1]	
			#print str(xvals[x+1]) +'    '+str(a)  + '     ' +str(b) 
			if ((a/abs(a))*(b/abs(b))) < 0.0 and a >0.0 :
				#print 'Limit found at: '+ (str(xvals[x]))
				bestmass = xvals[x]
				break;
						
		return [bestmass,mindif]
		
	def get_simple_intersection(graph1, graph2, xmin,xmax):
		num = (xmax-xmin)*10
		inc = (xmax - xmin)/(1.0*num)

		dif = []
		sdif = []
		x = xmin +0.1
		xvals = []
		xx = []
		yy = []
		xvals = []
		while x<(xmax-.1):
			thisdif = (exp(graph1.Eval(x)) - exp(graph2.Eval(x)))
			#print (str(x)) + '   '+ str(xmax-.1) +'   '+ str(thisdif)
			xx.append(exp(graph1.Eval(x)))
			yy.append(exp(graph2.Eval(x)))
			sdif.append(thisdif)
			dif.append(abs(thisdif))
			xvals.append(x)
			#print  str(x) + '   ' +str(exp(graph1.Eval(x))) + '    '+str(exp(graph2.Eval(x))) + '    ' + str(thisdif)
			x = x+inc
		#print 'Done Looping for Difs'
		mindif = min(dif)
		bestmass = 0	
		
	
		for x in range(len(dif)-2):
			a = sdif[x]
			b = sdif[x+1]	
			#print str(xvals[x+1]) +'    '+str(a)  + '     ' +str(b) 
			if ((a/abs(a))*(b/abs(b))) < 0.0 and a >0.0 :
				print 'Limit found at: '+ (str(xvals[x]))
				bestmass = xvals[x]
				break;
						
		return [bestmass,mindif]
		


	def fill_mlists(clist):
		mlist = []
		for limit_set in clist:
			fitted_limits = loggraph(masses,limit_set)
			goodm = get_simple_intersection(logtheory,fitted_limits,250,850)
			mlist.append(str(round(goodm[0],2)))
		return mlist
		
	m_ComboObs = fill_mlists(s_ComboObs)
	m_Combo95down = fill_mlists(s_Combo95down)
	m_Combo68down = fill_mlists(s_Combo68down)
	m_ComboExp = fill_mlists(s_ComboExp)
	m_Combo68up = fill_mlists(s_Combo68up)
	m_Combo95up = fill_mlists(s_Combo95up)

	m_ComboBetaOneObs = fill_mlists(s_ComboBetaOneObs)
	m_ComboBetaOne95down = fill_mlists(s_ComboBetaOne95down)
	m_ComboBetaOne68down = fill_mlists(s_ComboBetaOne68down)
	m_ComboBetaOneExp = fill_mlists(s_ComboBetaOneExp)
	m_ComboBetaOne68up = fill_mlists(s_ComboBetaOne68up)
	m_ComboBetaOne95up = fill_mlists(s_ComboBetaOne95up)

	m_ComboBetaHalfObs = fill_mlists(s_ComboBetaHalfObs)
	m_ComboBetaHalf95down = fill_mlists(s_ComboBetaHalf95down)
	m_ComboBetaHalf68down = fill_mlists(s_ComboBetaHalf68down)
	m_ComboBetaHalfExp = fill_mlists(s_ComboBetaHalfExp)
	m_ComboBetaHalf68up = fill_mlists(s_ComboBetaHalf68up)
	m_ComboBetaHalf95up = fill_mlists(s_ComboBetaHalf95up)
	
	
	betav = []
	for x in betas:
		betav.append(str(round(x,4)))
	betastring = 'static int numbetas = '+str(len(betas))+';\n'+'Double_t beta_vals['+str(len(betas))+'] = {' +str(betav).replace('[','').replace(']','').replace('\'','')+'};'

	band1sigma_combo = 'Double_t m_1sigma_combo['+str(2*len(betas))+']={'
	band2sigma_combo = 'Double_t m_2sigma_combo['+str(2*len(betas))+']={'
	excurve_combo = 'Double_t m_expected_combo['+str(len(betas))+'] = {' 
	obcurve_combo = 'Double_t m_observed_combo['+str(len(betas))+'] = {'  

	band1sigma_lljj = 'Double_t m_1sigma_lljj['+str(2*len(betas))+']={'
	band2sigma_lljj = 'Double_t m_2sigma_lljj['+str(2*len(betas))+']={'
	excurve_lljj = 'Double_t m_expected_lljj['+str(len(betas))+'] = {' 
	obcurve_lljj = 'Double_t m_observed_lljj['+str(len(betas))+'] = {'  
	
	band1sigma_lvjj = 'Double_t m_1sigma_lvjj['+str(2*len(betas))+']={'
	band2sigma_lvjj = 'Double_t m_2sigma_lvjj['+str(2*len(betas))+']={'
	excurve_lvjj = 'Double_t m_expected_lvjj['+str(len(betas))+'] = {' 
	obcurve_lvjj = 'Double_t m_observed_lvjj['+str(len(betas))+'] = {'  
	
	
	excurve_combo += str(m_ComboExp).replace('[','').replace(']','').replace('\'','')+'};'
	obcurve_combo += str(m_ComboObs).replace('[','').replace(']','').replace('\'','')+'};'

	excurve_lljj += str(m_ComboBetaOneExp).replace('[','').replace(']','').replace('\'','')+'};'
	obcurve_lljj += str(m_ComboBetaOneObs).replace('[','').replace(']','').replace('\'','')+'};'
	
	excurve_lvjj += str(m_ComboBetaHalfExp).replace('[','').replace(']','').replace('\'','')+'};'
	obcurve_lvjj += str(m_ComboBetaHalfObs).replace('[','').replace(']','').replace('\'','')+'};'	

	m_Combo68up.reverse()
	m_Combo95up.reverse()

	m_ComboBetaOne68up.reverse()
	m_ComboBetaOne95up.reverse()
	
	m_ComboBetaHalf68up.reverse()
	m_ComboBetaHalf95up.reverse()
	
		
	band1sigma_combo += str(m_Combo68down).replace('[','').replace(']','').replace('\'','')+ ', '+  str(m_Combo68up).replace('[','').replace(']','').replace('\'','')+'};'
	band2sigma_combo += str(m_Combo95down).replace('[','').replace(']','').replace('\'','') + ', '+ str(m_Combo95up).replace('[','').replace(']','').replace('\'','')+'};'

	band1sigma_lljj += str(m_ComboBetaOne68down).replace('[','').replace(']','').replace('\'','') + ', '+  str(m_ComboBetaOne68up).replace('[','').replace(']','').replace('\'','')+'};'
	band2sigma_lljj += str(m_ComboBetaOne95down).replace('[','').replace(']','').replace('\'','') + ', '+ str(m_ComboBetaOne95up).replace('[','').replace(']','').replace('\'','')+'};'
	
	band1sigma_lvjj += str(m_ComboBetaHalf68down).replace('[','').replace(']','').replace('\'','') + ', '+  str(m_ComboBetaHalf68up).replace('[','').replace(']','').replace('\'','')+'};'
	band2sigma_lvjj += str(m_ComboBetaHalf95down).replace('[','').replace(']','').replace('\'','') + ', '+ str(m_ComboBetaHalf95up).replace('[','').replace(']','').replace('\'','')+'};'
	
	
	
	print '\n'
	print betastring
	print '\n'
	print excurve_combo
	print obcurve_combo
	print band1sigma_combo
	print band2sigma_combo

	print '\n'
	print excurve_lljj
	print obcurve_lljj
	print band1sigma_lljj
	print band2sigma_lljj
	
	print '\n'
	print excurve_lvjj
	print obcurve_lvjj
	print band1sigma_lvjj
	print band2sigma_lvjj	
	print '\n'
	print '\n'
	
	
