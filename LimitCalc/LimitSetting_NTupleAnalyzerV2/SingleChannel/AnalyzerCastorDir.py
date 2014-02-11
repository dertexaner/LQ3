import os
import sys

mTh = [ 150, 200, 250, 300, 350, 400,450,500,550,600,650,700,750,800,850]
xsTh = [  53.3, 11.9, 3.47, 1.21, 0.477, .205,.0949,.0463,.0236,.0124,.00676,.00377,.00215,.00124,.000732]

nodo = 0
if "--nodo" in str(sys.argv):
	nodo = 1
cdir = sys.argv[1]

person = (os.popen('whoami').readlines())[0].replace('\n','')
tmpdir1 = '/tmp/' + person+'/'+cdir.split('/')[-1]
if nodo == 1:
	os.system('rm -r '+tmpdir1)
	os.system('mkdir '+tmpdir1)

cdirinfo = os.popen('nsls '+cdir).readlines() 

dirinfo = []
tmpdirs = []

for x in cdirinfo:
	if '.' not in x:
		dirinfo.append(cdir+'/'+x.replace('\n',''))
		tmpdirs.append(tmpdir1+'/'+x.replace('\n',''))

files = []
for x in dirinfo:
	f= os.popen('nsls '+x).readlines() 
	for y in f:
		files.append(x+'/'+y.replace('\n',''))
if nodo ==  0:
	for x in tmpdirs:
		os.system('rm -r '+x)
		os.system('mkdir '+x)

rfcps = []
for x in files:
	begin = 'rfcp '+x+' '
	for t in tmpdirs:

		if t.split('/')[-1] in x:
			d = t
			rfcps.append(begin + t + ' & ')

if nodo==0:
	for x in rfcps:
		os.system(x)
		os.system('sleep .3')

	do = 0
	while do ==0:
		total1 = len(os.popen('ls '+tmpdir1+'/*/*root').readlines())
		total2 = len(files)
		if total1==total2:
			do = 1
		os.system('sleep 10')
	os.system('sleep 10')

#os.system('sleep 200')

hadds = []
for x in tmpdirs:
	hadds.append('hadd '+x+'.root'+' '+x+'/*root')

for x in hadds:
	os.system(x)

mainfiles = []
for x in hadds:
	mainfiles.append((x.split(' '))[1])

from ROOT import *

limits = []

masses = []
for x in mainfiles:
	print x
	m = x.split('_')[-1]
	m = m.split('.')[0]
	masses.append(m)
	
for x in mainfiles:
	print x
	theselimits = []
	check = (os.popen('du '+x).readlines())
	check = check[0].split('\t')
	check = float(check[0])
	if check<9:
		limits.append(theselimits)
		continue

	f = TFile.Open(x)
	t = f.Get("limit")
	N = (t.GetEntries())
	for iev in range(0,N) :
		t.GetEntry(iev)
		lim = t.limit
		theselimits.append(lim)
	f.Close()
	del t
	limits.append(theselimits)
	print theselimits

mean = []
median = []
sig1up = []
sig1down = []
sig2up = []
sig2down = []

def lmean(v):
	tot = sum(v)
	tot = tot/float(len(v))
	return tot
def lband(v):
	v.sort()
	length = len(v)
	b0 = int(round(.0275*length))
	b1 = int(round(.16*length))
	b2 = int(round(.5*length))
	b3 = int(round(.84*length))
	b4 = int(round(.975*length))
	if b0==length:
		b0 = length-1
	if b1==length:
		b1 = length-1
	if b2==length:
		b2 = length-1	
	if b3==length:
		b3 = length-1
	if b4==length:
		b4 = length-1
	b0 = v[b0]
	b1 = v[b1]
	b2 = v[b2]
	b3 = v[b3]
	b4 = v[b4]
	band = [b0,b1,b2,b3,b4]
	return band

n = 0
for h in limits:
	#print masses[n]
	n = n +1
	#print "starting ... "
	#print h
	h.sort()
	if len(h)==0:
		h = [0,0]
	m = lmean(h)
	b = lband(h)
	mean.append(m)
	sig2down.append(b[0])
	sig1down.append(b[1])
	median.append(b[2])
	sig1up.append(b[3])
	sig2up.append(b[4])

med = median
up2 = sig2up
up1 = sig1up
down2 = sig2down
down1 = sig1down
print '\n\n\n\n'
s = '    '
for x in range(len(masses)):
	print masses[x] + s + str(down2[x]) + s + str(down1[x])+ s + str(med[x]) + s+ str(up1[x]) + s+ str(up2[x])

band1sigma = 'Double_t y_1sigma[20]={'
band2sigma = 'Double_t y_2sigma[20]={'
excurve = 'Double_t xsUp_expected[10] = {' 

fac = 1.0
if "--munu" in str(sys.argv):
	fac = 0.5
sigma = []
for x in range(len(mTh)):
	if str(mTh[x]) in masses: 
		sigma.append(xsTh[x]*fac)


print masses
print sigma		
for x in range(len(masses)):
	excurve += str(float(med[x])*float(sigma[x])) + ' , ' 
	band1sigma += str(float(down1[x])*float(sigma[x])) + ' , ' 
	band2sigma += str(float(down2[x])*float(sigma[x])) + ' , ' 

for x in range(len(masses)):
	band1sigma += str(float(up1[-(x+1)])*float(sigma[-(x+1)])) + ' , ' 
	band2sigma += str(float(up2[-(x+1)])*float(sigma[-(x+1)])) + ' , ' 
excurve += '}'
band1sigma += '}'
band2sigma += '}'
excurve = excurve.replace(' , }',' }; ' )
band1sigma = band1sigma.replace(' , }',' }; ' )
band2sigma = band2sigma.replace(' , }',' }; ' )

print '\n'
print excurve
print '\n'
print band1sigma
print '\n'
print band2sigma

