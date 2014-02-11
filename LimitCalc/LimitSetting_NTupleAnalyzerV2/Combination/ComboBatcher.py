import os
import sys

mypwd = ((os.popen('pwd').readlines())[0]).replace('\n','')

f = open('RunStatsCombo.py','r')

fb = open('betasinfo.py','w')

ft = open('RunStatsComboBatcher.py','w')

cards = '/afs/cern.ch/user/d/darinb/scratch0/LimitCombinationHiggs/CMSSW_4_1_3/src/FinalCards.txt'

tagname = 'test'
for x in range(len(sys.argv)):
	if '-cards' ==sys.argv[x]:
		cardfile = str(sys.argv[x+1])
	if '-tag'==sys.argv[x]:
		tagname = str(sys.argv[x+1])
if mypwd not in cards:
	cards = mypwd + cards
for line in f:
	if 'betas' in line and '=' in line:
		fb.write(line)
		continue
	ft.write(line)
ft.close()
fb.close()

from betasinfo import *

print betas

n = 0
os.system('rm combosub_*_*sh')
for x in betas:
	if x > 0.9999:
		x = 0.999
	n += 1
	os.system('echo betas = "['+str(round(x,4))+']">RunStatsComboBatcher_'+str(n)+'.py')
	os.system('echo "  " >>RunStatsComboBatcher_'+str(n)+'.py')
	os.system('echo "  " >>RunStatsComboBatcher_'+str(n)+'.py')
	os.system('cat RunStatsComboBatcher.py >> RunStatsComboBatcher_'+str(n)+'.py')
	os.system('sed -i \'s/ComboLog/ComboLog_'+str(n)+'/g\' RunStatsComboBatcher_'+str(n)+'.py')
	subs = ['combosub_mumu.csh','combosub_munu.csh','combosub_combo.csh']
	for sub in subs:
		os.system('sed \'s/RunStatsCombo/RunStatsComboBatcher_'+str(n)+'/g\' '+sub + ' > '+sub.replace('.','_'+str(n)+'.'))
		os.system('sed -i \'s/MYPWD/'+mypwd.replace('/','\/')+'/g\' '+sub.replace('.','_'+str(n)+'.') )

		opts = '-cards '+cardfile + ' -tag '+tagname
		os.system ('sed -i \'s/MYOPTIONS/'+(opts.replace('-','\-')).replace(' ','\ ')+'/g\' '+sub.replace('.','_'+str(n)+'.') )
		
	fcom = open('combosub_alljobs.sh','r')
	for line in fcom:
		if 'bsub' not in line:
			continue
		if '#' in line:
			continue
		line = line.replace('.','_'+str(n)+'.')

		line = line.replace('jobcombo','jobcombo_'+str(n))
		#line = line.replace('--tries 100','--tries 10')
		
#		line = line.replace('-o /dev/null','' )
		line = line.replace('-R "pool>10000" -o /dev/null -e /dev/null','' )

#		line = line.replace('2nd','1nh')
		print line
		os.system(line)
		os.system('sleep .3')
