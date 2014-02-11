import os
import sys

mTh = [ 150, 200, 250, 300, 350, 400,450,500,550,600,650,700,750,800,850]
xsTh = [  53.3, 11.9, 3.47, 1.21, 0.477, .205,.0949,.0463,.0236,.0124,.00676,.00377,.00215,.00124,.000732]

nodo = 0
if "--nodo" in str(sys.argv):
	nodo = 1
cdir = sys.argv[1]
if cdir[-1] != '/':
	cdir += '/'

person = (os.popen('whoami').readlines())[0].replace('\n','')
tmpdir1 = '/tmp/' + person+'/'+cdir.split('/')[-2]

if nodo == 0:
	os.system('rm -r '+tmpdir1)
	os.system('mkdir '+tmpdir1)
	
if 'castor' in cdir:
	cdirinfo = os.popen('nsls '+cdir).readlines() 
	
	dirinfo = []
	tmpdirs = []
	
	for x in cdirinfo:
		print x
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
else:
	os.system('cp -r '+cdir+'/* '+tmpdir1+'/')
	cdirinfo = os.listdir(cdir)
	
	dirinfo = []
	tmpdirs = []
	
	for x in cdirinfo:
		if '.' not in x:
			dirinfo.append(cdir+'/'+x.replace('\n',''))
			tmpdirs.append(tmpdir1+'/'+x.replace('\n',''))
	files = []	


mThnew = []
xsThnew = []
for n in range(len(mTh)):
	m = str(mTh[n])

	mkeep=0
	
	if '--combo' not in sys.argv:
		for d in dirinfo:
			if 'M_'+m in d:
				mkeep=True
	if '--combo' in sys.argv:
		for d in dirinfo:
			dcont = os.listdir(d)
			for df in dcont:
				if 'M_'+m in df:
					mkeep=True
			#print dcont
			#print d
	if mkeep==1:
		xsThnew.append(xsTh[n])
		mThnew.append(mTh[n])
		
mTh = mThnew
xsTh = xsThnew

print mTh
print xsTh
	
hadds = []
for x in tmpdirs:
	if '--combo' in sys.argv:
		continue
	if '--betahalf' in sys.argv:
		if 'BetaHalf' not in x:
			continue
	if '--betaone' in sys.argv:
		if 'BetaHalf'  in x:
			continue
	hadds.append('hadd '+x+'.root'+' '+x+'/*root')

if '--combo' in sys.argv:
	for x in tmpdirs:
		dtypes = []
		dcont = os.listdir(x)
		for df in dcont:
			df=df.split('_R_')[0]
			if df not in dtypes:
				dtypes.append(df)
		for d in dtypes:
			#print d
			hadds.append('hadd '+tmpdir1+'/'+d+'.root '+x+'/'+d+'*root')

if '--nohadd' not in sys.argv:
	for x in hadds:
		os.system(x)

mainfiles = []
for x in hadds:
	mainfiles.append((x.split(' '))[1])

from ROOT import *

limits = []
masses = []

def combiner(conf,grid,quantile):
	comstring = 'combine -v 0 -M HybridNew --rule CLs --frequentist '
	comstring += conf
	comstring += ' --grid='+grid
	if quantile > 0 and quantile < 1:
		comstring += ' --expectedFromGrid '+str(quantile)
	return comstring




if '--combo' in sys.argv:


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


	tmpfiles = os.listdir(tmpdir1)
	cfgfiles = []
	for x in tmpfiles:
		if '.cfg' in x:
			cfgfiles.append(x)

	betas = []
	sbetas = []
	
	
	maincfgs = []

	for x in mainfiles:
		shortfile = x.split('/')[-1]
		style = 'BetaHalf'*('BetaHalf' in x) + 'BetaOne'*('BetaOne' in x)+'COMBO_M'*('Combo_' in x)
		mass = ''
		beta = ''
		splitfile = shortfile.split('_')
		for s in range(len(splitfile)):
			if splitfile[s] == 'beta':
				beta = splitfile[s]+'_'+splitfile[s+1]+'_'+splitfile[s+2]
			if splitfile[s]=='M':
				mass = splitfile[s]+'_'+splitfile[s+1]
				mass = mass.split('.')[0]
		cfgfile = ''
		sbval = beta
		bval = float((beta.split('beta_')[-1]).replace('_','.'))
		
		if bval not in betas:
			betas.append(bval)
			sbetas.append(sbval)
		
		betas, sbetas = zip(*sorted(zip(betas, sbetas)))
		betas = list(betas)
		sbetas = list(sbetas)
		
		mval = int((mass.split('_'))[-1])
		if mval not in masses:
			masses.append(mval)
		
		for c in cfgfiles:
			if mass in c and style in c and beta in c:
				cfgfile = tmpdir1+'/'+c
		maincfgs.append(cfgfile)
	
	mlist = []
	for m in masses:
		mlist.append(0)
	
	for beta in betas:
		ComboObs.append(mlist)
		Combo95down.append(mlist)
		Combo68down.append(mlist)
		ComboExp.append(mlist)
		Combo68up.append(mlist)
		Combo95up.append(mlist)

		ComboBetaOneObs.append(mlist)
		ComboBetaOne95down.append(mlist)
		ComboBetaOne68down.append(mlist)
		ComboBetaOneExp.append(mlist)
		ComboBetaOne68up.append(mlist)
		ComboBetaOne95up.append(mlist)
		

		ComboBetaHalfObs.append(mlist)
		ComboBetaHalf95down.append(mlist)
		ComboBetaHalf68down.append(mlist)
		ComboBetaHalfExp.append(mlist)
		ComboBetaHalf68up.append(mlist)
		ComboBetaHalf95up.append(mlist)
	
	fillstrings = ['Obs','95down','68down','Exp','68up','95up']
	for n in range(len(mainfiles)):
		x = mainfiles[n]
		shortfile = x.split('/')[-1]
		print shortfile
		style = 'ComboBetaHalf'*('BetaHalf' in x) + 'ComboBetaOne'*('BetaOne' in x)+'Combo'*('Combo_' in x)

		mass = ''
		beta = ''
		splitfile = shortfile.split('_')
		for s in range(len(splitfile)):
			if splitfile[s] == 'beta':
				beta = splitfile[s]+'_'+splitfile[s+1]+'_'+splitfile[s+2]
			if splitfile[s]=='M':
				mass = splitfile[s]+'_'+splitfile[s+1]
				mass = mass.split('.')[0]
				mass = mass.split('_')[-1]
		massindex = 999
		betaindex = 999
		
		for n in range(len(mTh)):
			if int(mTh[n]) == int(mass):
				massindex = n
		
		for n in range(len(sbetas)):
			if beta == sbetas[n]:
				betaindex = n
		
		listindex = '['+str(betaindex)+']['+str(massindex)+']'
		
		
		conf = maincfgs[n]
		comcommands = []
		comcommands.append( combiner(conf,x,-1) )
		comcommands.append( combiner(conf,x,0.025) )
		comcommands.append( combiner(conf,x,0.16) )
		comcommands.append( combiner(conf,x,0.5) )
		comcommands.append( combiner(conf,x,0.84) )
		comcommands.append( combiner(conf,x,0.975) )		

		finalcoms = []
		for com in comcommands:
			comoutput = os.popen(com).readlines()
			for c in comoutput:
				if 'r <' in c:
					c = c.split('<')[-1]
					c = c.split('+')[0]
					c = c.replace(' ','')
					c = float(c)
					finalcoms.append(c)
					
		print finalcoms
	
		for n in range(len(finalcoms)):
			exec(style+fillstrings[n]+listindex+'='+str(finalcoms[n]))







	print "*"*40 + '\n COMBINATION CLS RESULTS\n\n' +"*"*40

					
	mTh = [ 150, 200, 250, 300, 350, 400,450,500,550,600,650,700,750,800,850]
	xsTh = [  53.3, 11.9, 3.47, 1.21, 0.477, .205,.0949,.0463,.0236,.0124,.00676,.00377,.00215,.00124,.000732]
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
		
	


	def fill_mlists(clist):
		mlist = []
		for limit_set in clist:
			fitted_limits = logspline(masses,limit_set)
			goodm = get_intersection(logtheory,fitted_limits,250,850)
			mlist.append(goodm[0])
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
	
	
	excurve_combo += str(m_ComboExp).replace('[','').replace(']','')+'};'
	obcurve_combo += str(m_ComboObs).replace('[','').replace(']','')+'};'

	excurve_lljj += str(m_ComboBetaOneExp).replace('[','').replace(']','')+'};'
	obcurve_lljj += str(m_ComboBetaOneObs).replace('[','').replace(']','')+'};'
	
	excurve_lvjj += str(m_ComboBetaHalfExp).replace('[','').replace(']','')+'};'
	obcurve_lvjj += str(m_ComboBetaHalfObs).replace('[','').replace(']','')+'};'	

	m_Combo68up.reverse()
	m_Combo95up.reverse()

	m_ComboBetaOne68up.reverse()
	m_ComboBetaOne95up.reverse()
	
	m_ComboBetaHalf68up.reverse()
	m_ComboBetaHalf95up.reverse()
	
		
	band1sigma_combo += str(m_Combo68down).replace('[','').replace(']','') + ', '+  str(m_Combo68up).replace('[','').replace(']','')+'};'
	band2sigma_combo += str(m_Combo95down).replace('[','').replace(']','') + ', '+ str(m_Combo95up).replace('[','').replace(']','')+'};'

	band1sigma_lljj += str(m_ComboBetaOne68down).replace('[','').replace(']','') + ', '+  str(m_ComboBetaOne68up).replace('[','').replace(']','')+'};'
	band2sigma_lljj += str(m_ComboBetaOne95down).replace('[','').replace(']','') + ', '+ str(m_ComboBetaOne95up).replace('[','').replace(']','')+'};'
	
	band1sigma_lvjj += str(m_ComboBetaHalf68down).replace('[','').replace(']','') + ', '+  str(m_ComboBetaHalf68up).replace('[','').replace(']','')+'};'
	band2sigma_lvjj += str(m_ComboBetaHalf95down).replace('[','').replace(']','') + ', '+ str(m_ComboBetaHalf95up).replace('[','').replace(']','')+'};'
	
	
	
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



















if '--combo' not in sys.argv:
	for x in mainfiles:
		print x
		m = x.split('_')[-1]
		m = m.split('.')[0]
		masses.append(m)
		n = 0
		
	
	mean = []
	median = []
	sig1up = []
	sig1down = []
	sig2up = []
	sig2down = []
	obs = []
		
	for x in mainfiles:
		conf = 'conf'
		if '--betahalf' not in sys.argv:
			conf += 'betaone_'
			thistag = cdir.split('BetaOne')[-1]
			thistype = 'LQ_M_'	+ (x.replace('.root','')).split('_')[-1]
		if '--betahalf' in sys.argv:
			conf += 'betahalf_'
			thistag = cdir.split('BetaHalf')[-1]	
			thistype = 'LQ_BetaHalf_M_'	+ (x.replace('.root','')).split('_')[-1]
			
		conf += thistag
		conf += thistype
		conf+= '.cfg'
		conf = conf.replace('/','_')
			
		if 'castor' not in cdir:
			conf = 'CLSLimits/'+cdir.split('/')[-2]+'/'+conf
		else:
			os.system('mkdir tmpcastorconfigs')
			os.system('rfcp '+cdir+'/'+conf+' tmpcastorconfigs/')
			conf = 'tmpcastorconfigs/'+conf
		
		comcommands = []
		comcommands.append( combiner(conf,x,-1) )
		comcommands.append( combiner(conf,x,0.025) )
		comcommands.append( combiner(conf,x,0.16) )
		comcommands.append( combiner(conf,x,0.5) )
		comcommands.append( combiner(conf,x,0.84) )
		comcommands.append( combiner(conf,x,0.975) )
		
		finalcoms = []
		for com in comcommands:
			comoutput = os.popen(com).readlines()
			for c in comoutput:
				if 'r <' in c:
					c = c.split('<')[-1]
					c = c.split('+')[0]
					c = c.replace(' ','')
					c = float(c)
					finalcoms.append(c)
					
		print finalcoms
	
	
		obs.append(finalcoms[0])		
		sig2down.append(finalcoms[1])
		sig1down.append(finalcoms[2])
		median.append(finalcoms[3])
		sig1up.append(finalcoms[4])
		sig2up.append(finalcoms[5])
		
	
	med = median
	up2 = sig2up
	up1 = sig1up
	down2 = sig2down
	down1 = sig1down
	ob = obs
	print '\n\n\n\n'
	s = '    '
	for x in range(len(masses)):
		print masses[x] + s + str(down2[x]) + s + str(down1[x])+ s + str(med[x]) + s+ str(up1[x]) + s+ str(up2[x])
	
	band1sigma = 'Double_t y_1sigma[20]={'
	band2sigma = 'Double_t y_2sigma[20]={'
	excurve = 'Double_t xsUp_expected[10] = {' 
	obcurve = 'Double_t xsUp_observed[10] = {'  
	
	fac = 1.0
	if "--betahalf" in str(sys.argv):
		fac = 0.5
	sigma = []
	for x in range(len(mTh)):
		if str(mTh[x]) in masses: 
			sigma.append(xsTh[x]*fac)
	
	
	print masses
	print sigma		
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

