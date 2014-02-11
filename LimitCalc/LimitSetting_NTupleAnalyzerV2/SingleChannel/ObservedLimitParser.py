import os
import sys

f = open('ObservedLimitLog.txt','r')

types = []
Limits = []
count = 0
nline = 0
for line in f:
	if 'Limit:' in line:
		print line
		nline += 1
		print nline
		count += 1
		if count == 2:
			Limits.append(line)
			count = 0
	
limitfigs = []
for limit in Limits:
	limit = (limit.split()[3])
	limitfigs.append(limit)

length = len(limitfigs)
length = length/2
ignoremumu=0
ignoremunu=0
if '--ignore_mumu' in sys.argv:
	ignoremumu=1
if '--ignore_munu' in sys.argv:
	ignoremunu=1

mumu=[]
munu=[]

if (ignoremumu==0 and ignoremumu==0):
	mumu = limitfigs[0:length]
if (ignoremumu==1 and ignoremunu==0):
        munu = limitfigs
	
if (ignoremunu==0 and ignoremumu==0):
	munu = limitfigs[length:]
if (ignoremunu==1 and ignoremumu==0):
	mumu=limitfigs
X_th=[ 3.47, 0.477, .205,.0949,.0463,.0236,.0124,.00676,.00215,.000732]
print mumu
print munu
for x in range(len(mumu)):
	mumu[x] = str(float(mumu[x])*X_th[x])

for x in range(len(munu)):
        munu[x] = str(float(munu[x])*0.5*X_th[x])


print mumu
print munu
mumu = str(mumu).replace('\'','')
mumu = mumu.replace('[','{')
mumu = mumu.replace(']','}')

munu = str(munu).replace('\'','')
munu = munu.replace('[','{')
munu = munu.replace(']','}')
if (ignoremumu+ignoremunu==1):
	length *=2

print '\nObserved limits for MuMu:\n'

print ' Double_t xsUp_observed['+str(length)+'] = '+mumu+' ; ' 


print '\n\nObserved limits for MuNu:\n'

print ' Double_t xsUp_observed['+str(length)+'] = '+munu+' ; ' 

print '\n\n'
