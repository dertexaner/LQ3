import sys
import os

infos = []
tag =sys.argv[1]
tag = '_'+tag+'_'
files = []
allfiles = os.listdir('.')

for f in allfiles:
	if tag in f:
		files.append(f)

def getcontent(ifile):
	ifile = open(ifile,'r')
	content = []
	for line in ifile:
		if '[1]' in line:
			content.append(line)
	return content

contents = []
for f in files:
	contents.append(getcontent(f))

arrays = []
for x in contents:
	for y in x:
		y=y.split('=')[0]
		if y not in arrays:
			arrays.append(y)

newarrays = []	
for a in arrays:
	newarray = a + ' = '
	for x in contents:
		for y in x:
			if a in y:
				newarray += ((y.split('=')[-1]).replace('\n','')).replace(';','')
	newarray = newarray.replace('} {',' , ')
	newarrays.append(newarray)
	
for x in newarrays:
	x = x.replace('[1]','')
	x = x.replace('{','[')
	x = x.replace('}',']')
	x = x.replace('Double_t ','')
	exec(x)

combinfo = zip (beta_comb,masses_comb)
combinfo.sort()
oneinfo = zip (beta_one,masses_one)
oneinfo.sort()
halfinfo = zip (beta_half,masses_half)
halfinfo.sort()
	
def infoparse(alist):
	list1 = []
	list2 = []
	for x in alist:
		for y in range(len(x)):
			if y == 0:
				list1.append(str(round(float(x[y]),2)))
			if y == 1:
				list2.append((x[y]))			
	return list1,list2


c1,c2 = infoparse(combinfo)	
bo1,bo2 = infoparse(oneinfo)
bh1,bh2 = infoparse(halfinfo)

def stringup(alist):
	out = 'name['+str(len(alist))+'] = ' 
	a = str(alist)
	a = a.replace('[','{')
	a = a.replace(']','}; \n')
	a = a.replace('\'','')	
	out += a
	return out
	
r = [c1,c2,bo1,bo2,bh1,bh2]

c1 = stringup(c1)
c2 = stringup(c2)
bo1 = stringup(bo1)
bo2 = stringup(bo2)
bh1 = stringup(bh1)
bh2 = stringup(bh2)

c1 = c1.replace('name','Double_t beta_comb')
c2 = c2.replace('name','Double_t masses_comb')
bo1 = bo1.replace('name','Double_t beta_one')
bo2 = bo2.replace('name','Double_t masses_one')
bh1 = bh1.replace('name','Double_t beta_half')
bh2 = bh2.replace('name','Double_t masses_half')

print c1
print c2
print bo1
print bo2
print bh1
print bh2



#def castarray(cstringarray):
	#name = ((cstringarray.split()[1]).split('['))[0]
	#thearray = cstringarray.split('{')[-1]
	#thearray = '['+thearray
	#thearray  = thearray.replace('};',']')
	#thearray = thearray.replace('\n','')
	#outstring = name + ' = array("d",' + thearray + ')\n'
	#exec(outstring)
#r = [c1,c2,bo1,bo2,bh1,bh2]
#for x in r:
	#castarray(x)
#def makespline(inputarrayX,inputarrayY):
	#g = TGraph(len(inputarrayX),inputarrayX,inputarrayY)
	#outspline = TSpline3("",g)
	#return outspline
	
#combspline = makespline(masses_comb,beta_comb)
#bonespline = makespline(masses_one, beta_one)	



#from ROOT import *
