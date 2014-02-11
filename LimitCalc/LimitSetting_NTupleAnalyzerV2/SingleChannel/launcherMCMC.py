import sys
import os
inputa = sys.argv[1]
typea = sys.argv[2]

dostring = 'combine -v 0 -d conf'+typea+'_'+inputa+'.cfg -M MarkovChainMC -t 50 --tries 50 -s -1 -H MarkovChainMC'

out = os.popen(dostring).readlines()
import random
r = str(random.randint(1,10000000))
f = open('log'+typea+inputa+'_'+r+'.txt','w')
f.write('Beginning log file \n\n')
for x in out:
	f.write(x)
f.close()

