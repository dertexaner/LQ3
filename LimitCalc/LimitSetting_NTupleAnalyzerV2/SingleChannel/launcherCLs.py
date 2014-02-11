import sys
import os
inputa = sys.argv[1]
typea = sys.argv[2]

#dostring = 'combine -v 0 -d conf'+typea+'_'+inputa+'.cfg -M MarkovChainMC -t 25 --tries 200 -b 5000 -i 20000 --rMin 0.0 --rMax 200.0' '
#dostring = 'combine -v 0 -d conf'+typea+'_'+inputa+'.cfg -M MarkovChainMC -t 50 --tries 70 -s -1 -H ProfileLikelihood '
#dostring = 'combine -v 0 -M HybridNew --rule CLs  --testStat LHC conf'+typea+'_'+inputa+'.cfg  -H ProfileLikelihood -t 50 -T 1 -s -1'

#dostring = 'combine -v 0 -M HybridNew --rule CLs  --testStat LEP conf'+typea+'_'+inputa+'.cfg -t 1 -s -1 -H ProfileLikelihood '

dostring = 'combine -v 0 -M HybridNew --rule CLs --testStat LEP conf'+typea+'_'+inputa+'.cfg --clsAcc=0 -s -1 -T 100 -i 10 -H MarkovChainMC --saveToys --saveHybridResult '


#combine -M HybridNew --rule CLs --testStat LEP confmunuMCMC1000PseudoRMaxOverRideFixedV4_LQ600.cfg -t 3 -s -1
out = os.popen(dostring).readlines()
import random
r = str(random.randint(1,10000000))
f = open('log'+typea+inputa+'_'+r+'.txt','w')
f.write('Beginning log file \n\n')
for x in out:
	f.write(x)
f.close()

