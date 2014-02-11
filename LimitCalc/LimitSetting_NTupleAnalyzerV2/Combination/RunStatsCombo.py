import os
import sys
import subprocess
import matplotlib.pyplot

import random

METHOD = ' -M MarkovChainMC --tries 100 -i 20000 -s -1 -H MarkovChainMC '
#betas = [0.01,0.03,0.05,0.07,0.09,0.11,0.13,0.15,0.17,0.19,0.21,0.23,0.25,0.27,0.29,0.31,0.33,0.35,0.37,0.39,0.41,0.43,0.45,0.47,0.49,0.5,0.52,0.54,0.56,0.58,0.6,0.62,0.64,0.66,0.68,0.7,0.72,0.74,0.76,0.78,0.8,0.82,0.84,0.86,0.88,0.9,0.92,0.94,0.96,0.98,0.9999]
betas = [0.01,0.02,0.03,0.04,0.05,0.06,0.07,0.08,0.09,0.1,0.11,0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.19,0.2,0.21,0.22,0.23,0.24,0.25,0.26,0.27,0.28,0.29,0.3,0.31,0.32,0.33,0.34,0.35,0.36,0.37,0.38,0.39,0.4,0.41,0.42,0.43,0.44,0.45,0.46,0.47,0.48,0.49,0.5,0.51,0.52,0.53,0.54,0.55,0.56,0.57,0.58,0.59,0.6,0.61,0.62,0.63,0.64,0.65,0.66,0.67,0.68,0.69,0.7,0.71,0.72,0.73,0.74,0.75,0.76,0.77,0.78,0.79,0.8,0.81,0.82,0.83,0.84,0.85,0.86,0.87,0.88,0.89,0.9,0.91,0.92,0.93,0.94,0.95,0.96,0.97,0.98,0.99,0.99999]
#betas = [.01,.03,.05,.07,.09,.11,.13,.15,.17,.19,.23,.27,.31,.35,.39,.43,.45,.47,.49,.51,.53,.56,.59,.63,.67,.71,.74,.76,.78,.80,.82,.84,.86,.88,.90,.92,.94,.96,.98,.99999]
#betas = [0.01,0.02,0.04,0.06,0.08,0.1,0.12,0.14,0.16,0.18,0.2,0.22,0.24,0.26,0.28,0.3,0.32,0.34,0.36,0.38,0.4,0.42,0.44,0.46,0.48,0.5,0.52,0.54,0.56,0.58,0.6,0.62,0.64,0.66,0.68,0.7,0.72,0.74,0.76,0.78,0.8,0.82,0.84,0.86,0.88,0.9,0.92,0.94,0.96,0.98,1]
#betas = [0.99999999999]
#betas = [0.1,0.5,0.7,0.9999]
#betas = [0.5]
#betas = [0.20]

if '--quick' in sys.argv:
	METHOD = ' -M MarkovChainMC --tries 10 -i 3000 -s -1 -H MarkovChainMC '


myrand= random.randrange(1,100)
myrand = (1.0*myrand)/10.0
os.system('sleep '+str(myrand))


cards = '/afs/cern.ch/user/d/darinb/scratch0/LimitCombinationHiggs/CMSSW_4_1_3/src/FinalCards.txt'
tagname = 'test'
for x in range(len(sys.argv)):
	if '-cards' ==sys.argv[x]:
		cardfile = str(sys.argv[x+1])
	if '-tag'==sys.argv[x]:
		tagname = str(sys.argv[x+1])

os.system('cat '+cards+' > mycards.txt')

ctag = ''
for x in range(len(sys.argv)):
	if sys.argv[x] == '-c':
		ctag = sys.argv[x+1]


dcont = os.listdir('.')
cardcont = []
for x in dcont:
	if x not in cardcont and ctag in x and '.cfg' in x:
		cardcont.append(x)

flog = open('mycards.txt','r')
digits = '0123456789'

cards = []
cardmasses = []
cardcontent = []
card = ''


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
	pair = []
	for x in combocards:
		if m in x:
			pair.append(x)
	os.system('combineCards.py '+pair[0]+ ' '+pair[1]+ '  > combocard_COMBO_M_'+m+'.cfg ' )
	print pair
for m in uniquecardmasses:
	combocards.append('combocard_COMBO_M_'+m+'.cfg')



do_mumu = 0
do_munu = 0 
do_combo = 0
if 'do_mumu' in str(sys.argv):
	do_mumu = 1
if 'do_munu' in str(sys.argv):
	do_munu = 1
if 'do_combo' in str(sys.argv):
	do_combo = 1
	
from ROOT import *
from array import array
	
mTh = array("d",[ 150, 200, 250, 300, 350, 400,450,500,550,600,650,700,750,800,850])
xsTh = array("d",[  53.3, 11.9, 3.47, 1.21, 0.477, .205,.0949,.0463,.0236,.0124,.00676,.00377,.00215,.00124,.000732])

g = TGraph(len(mTh),mTh,xsTh);
spline = TSpline3("xsection",g) 
#xx = (spline.Eval(310));
M_th=[ 150, 200, 250, 300, 350, 400,450,500,550,600,650,700,750,800,850]
X_th=[  53.3, 11.9, 3.47, 1.21, 0.477, .205,.0949,.0463,.0236,.0124,.00676,.00377,.00215,.00124,.000732]
	
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
def logspline(inputarrayX,inputarrayY):
	logarray = []
	for x in inputarrayY:
		logarray.append(math.log(x))
	x = array("d",inputarrayX)
	y = array("d",logarray)
	g = TGraph(len(x),x,y)
	outspline = TSpline3("",g)
	return outspline
logtheory = logspline(M_th,X_th)

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
		if '--quick' in sys.argv:
			print (str(x)) + '   ' + str(thisdif)
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
		if '--quick' in sys.argv:
			print str(xvals[x+1]) +'    '+str(a)  + '     ' +str(b) 
		if ((a/abs(a))*(b/abs(b))) < 0.0 and a >0.0 :
			print 'Limit found at: '+ (str(xvals[x]))
			bestmass = xvals[x]
			#break;
					
	return [bestmass,mindif]

cr = ' \n'	


masses_comb = 'Double_t masses_comb['
masses_mumu = 'Double_t masses_one['
masses_munu = 'Double_t masses_half['

b_comb = 'Double_t beta_comb['
b_mumu = 'Double_t beta_one['
b_munu= 'Double_t beta_half['


theoryratios = []
theoryratioerrors = []

beta_combo = []
m_combo = []
dif_combo = []

rat = -99.0
precision = .001

name = []
for x in combocards:
	name.append(x.replace('.cfg',''))

if do_combo == 1:
	kill = 0 
	for beta in betas:
		beta_combo.append(beta)
		these_masses = []
		these_ratios = []
		these_limits = []
		
		for x in range(len(name)):
			print name[x]
			if 'COMBO' not in name[x]:
				continue
			m = ''
			for n in name[x]:
				if n in digits:
					m+= n
			these_masses.append(float(m))


			fnorm = open(name[x]+'.cfg','r')
			ftmp = open(name[x]+'_temporarynonsense.cfg','w')
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
					for nsp in range(len(linesplit)):
						if 'BetaHalf' not in line and nsp == betaoneplace+1:
							repsold =  str(linesplit[nsp+1])
							repsnew = str(float(repsold)*beta*beta)
							line = line.replace(repsold,repsnew)
						if 'BetaHalf' in line and nsp == betahalfplace+1:
							repsold =  str(linesplit[nsp+1])
							respsnew = str(float(repsold)*beta*(1.0-beta)*4.0)
							line = line.replace(repsold,repsnew)
				ftmp.write(line)
			ftmp.close()
			
			limit = os.popen('combine -v 0 -d '+name[x]+'_temporarynonsense.cfg '+METHOD).readlines()

			for line in limit:
				if 'r <' in line:
					oline = line
					line = line.split(' ')
					for i in range(len(line)):
						if '<' in line[i]:
							print name[x]+', beta = '+str(beta)+' :  ' +oline
							goodline = line
			line = goodline	
			for i in range(len(line)):
				if  line[i]=='r' and line[i+1] == '<' :
					rat = float(line[i+2].replace(';',''))
					these_ratios.append(rat)
			print these_masses
			print these_ratios

			for i in range(len(M_th)):
				if (float(m) == M_th[i]):
					these_limits.append(X_th[i]*rat)
			#print these_limits

		fitted_limits = logspline(these_masses,these_limits)

		goodm = get_intersection(logtheory,fitted_limits,250,850)
		gooddif = goodm[-1]
		goodm  = goodm[0]

		#if float(these_ratios[0])>1.0:
			#goodm = 0
		
		print '\n\n\n***************   Mass Point Found: beta = ' + str(beta) + '    m = ' + str(goodm) + ',   dif = '+  str(gooddif) +'\n\n'
		if goodm > 0:
		
			m_combo.append(goodm)
			dif_combo.append(gooddif)

	masses_comb += str(len(m_combo))+'] = {'
	b_comb += str(len(m_combo))+'] = {'
	for x in range(len(m_combo)):
		masses_comb += str(m_combo[x]) +','
		b_comb += str(beta_combo[x]) +','
	masses_comb += '};\n'
	b_comb += '};\n'
	masses_comb = masses_comb.replace(',}','}')
	b_comb = b_comb.replace(',}','}')
	print 2*cr + masses_comb +cr+ b_comb + 2*cr
	print dif_combo



beta_mumu = []
m_mumu = []
dif_mumu = []
rat = -99.0
precision = .001

if do_mumu == 1:
	kill = 0 
	for beta in betas:
		beta_mumu.append(beta)
		these_masses = []
		these_ratios = []
		these_limits = []
		
		for x in range(len(name)):
			print name[x]
			
			if 'BetaHalf' in name[x] or 'COMBO' in name[x]:
				continue
			m = ''
			for n in name[x]:
				if n in digits:
					m+= n
			these_masses.append(float(m))
			fnorm = open(name[x]+'.cfg','r')
			ftmp = open(name[x]+'_temporarynonsense.cfg','w')
			betahalfplace = 99
			betaoneplace = 99
			for line in fnorm:

				if ('LQ' in line and 'process' in line):
					linesplit = line.split()
					for place in range(len(linesplit)):
						if 'LQ' in linesplit[place] and 'BetaHalf' not in linesplit[place]:
							betaoneplace = place
							
				if ( 'rate' in line):

					linesplit = line.split()
					linesplit2 = []
					for place in range(len(linesplit)):
						arg = linesplit[place]
						
						if betaoneplace == place:						
							arg = str(float(arg)*beta*beta)
						linesplit2.append(arg)
					print linesplit2
					line2 = ''
					for xpart in linesplit2:
						line2 += xpart + '    '
					line2 += '\n'
					line = line2
				if  'stat' in line and 'sig' in line and 'gmN' in line:
					linesplit = line.split()
					for nsp in range(len(linesplit)):
						if 'BetaHalf' not in line and nsp == betaoneplace+1:
							repsold =  str(linesplit[nsp+1])
							repsnew = str(float(repsold)*beta*beta)
							line = line.replace(repsold,repsnew)
						if 'BetaHalf' in line and nsp == betahalfplace+1:
							repsold =  str(linesplit[nsp+1])
							respsnew = str(float(repsold)*beta*(1.0-beta)*4.0)
							line = line.replace(repsold,repsnew)					
				ftmp.write(line)
			ftmp.close()
			limit = os.popen('combine -v 0 -d '+name[x]+'_temporarynonsense.cfg '+METHOD).readlines()
			
			for line in limit:
				if 'r <' in line:
					oline = line
					line = line.split(' ')
					for i in range(len(line)):
						if '<' in line[i]:
							print name[x]+', beta = '+str(beta)+' :  ' +oline
							goodline = line
			line = goodline	
			for i in range(len(line)):
				if  line[i]=='r' and line[i+1] == '<' :
					rat = float(line[i+2].replace(';',''))
					these_ratios.append(rat)
			print these_masses
			print these_ratios

			for i in range(len(M_th)):
				if (float(m)  == M_th[i]):
					these_limits.append(X_th[i]*rat)
			#print these_limits

		fitted_limits = logspline(these_masses,these_limits)

		goodm = get_intersection(logtheory,fitted_limits,250,850)
		gooddif = goodm[-1]
		goodm  = goodm[0]

		#if float(these_ratios[0])>1.0:
			#goodm = 0		
		
		print '\n\n\n***************   Mass Point Found: beta = ' + str(beta) + '    m = ' + str(goodm) + ',   dif = '+  str(gooddif) +'\n\n'
		if goodm > 0:
			m_mumu.append(goodm)
			dif_mumu.append(gooddif)
		

	masses_mumu += str(len(m_mumu))+'] = {'
	b_mumu += str(len(m_mumu))+'] = {'
	for x in range(len(m_mumu)):
		masses_mumu += str(m_mumu[x]) +','
		b_mumu += str(beta_mumu[x]) +','
	masses_mumu += '};\n'
	b_mumu += '};\n'
	masses_mumu = masses_mumu.replace(',}','}')
	b_mumu = b_mumu.replace(',}','}')
	print 2*cr + masses_mumu +cr+ b_mumu + 2*cr
	print dif_mumu



beta_munu = []
m_munu = []
dif_munu = []
rat = -99.0
precision = .001

if do_munu == 1:
	kill = 0 
	for beta in betas:
		beta_munu.append(beta)
		these_masses = []
		these_ratios = []
		these_limits = []
		
		for x in range(len(name)):
			print name[x]
			
			if 'BetaHalf' not in name[x]:
				continue
			m = ''
			
			for n in name[x]:
				if n in digits:
					m+= n
			these_masses.append(float(m))	
			
					
			fnorm = open(name[x]+'.cfg','r')
			ftmp = open(name[x]+'_temporarynonsense.cfg','w')
			betahalfplace = 99
			betaoneplace = 99
			for line in fnorm:

				if ('LQ' in line and 'process' in line):
					linesplit = line.split()
					for place in range(len(linesplit)):
						if 'LQ' in linesplit[place] and 'BetaHalf' in linesplit[place]:
							betahalfplace = place
							
				if ( 'rate' in line):

					linesplit = line.split()
					linesplit2 = []
					for place in range(len(linesplit)):
						arg = linesplit[place]
						if betahalfplace == place:
							arg = str(float(arg)*beta*(1.0-beta)*4.0)
						linesplit2.append(arg)
					line2 = ''
					for xpart in linesplit2:
						line2 += xpart + '    '
					line2 += '\n'
					line = line2
				if  'stat' in line and 'sig' in line and 'gmN' in line:
					linesplit = line.split()
					for nsp in range(len(linesplit)):
						if 'BetaHalf' not in line and nsp == betaoneplace+1:
							repsold =  str(linesplit[nsp+1])
							repsnew = str(float(repsold)*beta*beta)
							line = line.replace(repsold,repsnew)
						if 'BetaHalf' in line and nsp == betahalfplace+1:
							repsold =  str(linesplit[nsp+1])
							respsnew = str(float(repsold)*beta*(1.0-beta)*4.0)
							line = line.replace(repsold,repsnew)					
				ftmp.write(line)
			ftmp.close()
			limit = os.popen('combine -v 0 -d '+name[x]+'_temporarynonsense.cfg '+METHOD).readlines()
			

			for line in limit:
				if 'r <' in line:
					oline = line
					line = line.split(' ')
					for i in range(len(line)):
						if '<' in line[i]:
							print name[x]+', beta = '+str(beta)+' :  ' +oline
							goodline = line
			line = goodline	
			for i in range(len(line)):
				if  line[i]=='r' and line[i+1] == '<' :
					rat = float(line[i+2].replace(';',''))
					these_ratios.append(rat)
			print these_masses
			print these_ratios

			for i in range(len(M_th)):
				if (float(m)  == M_th[i]):
					these_limits.append(X_th[i]*rat)
			#print these_limits

		fitted_limits = logspline(these_masses,these_limits)

		goodm = get_intersection(logtheory,fitted_limits,250,850)
		gooddif = goodm[-1]
		goodm  = goodm[0]
		
		#if float(these_ratios[0])>1.0:
			#goodm = 0
		print '\n\n\n***************   Mass Point Found: beta = ' + str(beta) + '    m = ' + str(goodm) + ',   dif = '+  str(gooddif) +'\n\n'
		
		if goodm > 0:
			m_munu.append(goodm)
			dif_munu.append(gooddif)
		

	masses_munu += str(len(m_munu))+'] = {'
	b_munu += str(len(m_munu))+'] = {'
	for x in range(len(m_munu)):
		masses_munu += str(m_munu[x]) +','
		b_munu += str(beta_munu[x]) +','
	masses_munu += '};\n'
	b_munu += '};\n'
	masses_munu = masses_munu.replace(',}','}')
	b_munu = b_munu.replace(',}','}')
	print 2*cr + masses_munu +cr+ b_munu + 2*cr
	print dif_munu

#os.system('rm *temporarynonsense*.cfg')



print "\n\n"+'+'*40 + '\n\n'
print 2*cr + masses_comb +cr+ b_comb + 2*cr
print dif_combo
print 2*cr + masses_mumu +cr+ b_mumu + 2*cr
print dif_mumu
print 2*cr + masses_munu +cr+ b_munu + 2*cr
print dif_munu

import random
rannum = str(random.randrange(1,10000000))

flogout = open('ComboLog_'+tagname+'_'+rannum+'.txt','w')


flogout.write( "\n\n"+'+'*40 + '\n\n')
flogout.write( 2*cr + masses_comb +cr+ b_comb + 2*cr)
flogout.write( 2*cr + masses_mumu +cr+ b_mumu + 2*cr)
flogout.write( 2*cr + masses_munu +cr+ b_munu + 2*cr)

flogout.close()
