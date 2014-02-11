#!/usr/bin/env python
from ROOT import *
import os
import sys
from array import array

print "MyLimitPlotter.py - Initializing"

input1=sys.argv[1] #higgsCombineLQ3_Sep9opt
input2=sys.argv[2] #Asymptotic
#
#debug = False
debug = True

#get current working directory
pwd=os.getcwd() 

print pwd
rootfilepath = str(pwd)+'/'
rootfilelist = os.listdir(rootfilepath)
InputFileArray = [ ]

for file in rootfilelist:
    if "root" not in file: continue
    if input1 not in file: continue
    if input2 not in file: continue
    InputFileArray.append(file)
    
print "Found ",len(InputFileArray)," files."

print "Input Files: "
for ifile in xrange(0,len(InputFileArray)):
    print " -> ",InputFileArray[ifile]
print

#--------------------------
def sFactor( MLQ ):
    Mtau=1.777 #GeV
    Mtop=173.5 #GeV
    d1=pow(Mtop/MLQ,2)
    d2=pow(Mtau/MLQ,2)
    if MLQ<Mtop:
        return 0
    #suppression factor due to phase space constraints
    sFactor_=pow( (pow((1+d1-d2),2) - 4*d1), 0.5 )*(1-d1-d2)  
    #return sFactor_
    return 1
#--------------------------
SSmutauBR=1
#--------------------------
print
print "LQ->MuTau+X (SS) Branching Fraction: ",SSmutauBR
print "Suppression Factors:  sqrt( pow((1+d1-d2),2) -4*d1 )*(1-d1-d2)"
for iM in xrange(0,14):
    print "LQ3M: ",iM*50+100,"  sFactor: ",sFactor(iM*50+100),"      (1-d1-d2): ",1-pow(173.5/(iM*50+100),2)-pow(1.777/(iM*50+100),2)
print

LQxsecDict = dict( M100=521.0*SSmutauBR*sFactor(100),     M150=74.9*SSmutauBR*sFactor(150), 
                   M200=17.4*SSmutauBR*sFactor(200),      M250=5.26*SSmutauBR*sFactor(250),
                   M300=1.89*SSmutauBR*sFactor(300),      M350=0.769*SSmutauBR*sFactor(350),
                   M400=0.342*SSmutauBR*sFactor(400),     M450=0.163*SSmutauBR*sFactor(450),
                   M500=0.082*SSmutauBR*sFactor(500),     M550=0.0431*SSmutauBR*sFactor(550),
                   M600=0.0235*SSmutauBR*sFactor(600),    M650=0.0132*SSmutauBR*sFactor(650),
                   M700=0.00761*SSmutauBR*sFactor(700),   M750=0.00448*SSmutauBR*sFactor(750),
                   M800=0.00269*SSmutauBR*sFactor(800),   M850=0.00164*SSmutauBR*sFactor(850),
                   M900=0.00101*SSmutauBR*sFactor(900),   M950=0.000635*SSmutauBR*sFactor(950),
                   M1000=0.000401*SSmutauBR*sFactor(1000)
                   )

LQxsecSysDict =  dict ( M100=0,        M150=0,
                        M200=0.1527,   M250=0.1621,
                        M300=0.1741,   M350=0.1869,
                        M400=0.2011,   M450=0.2156,
                        M500=0.2311,   M550=0.2464,
                        M600=0.2635,   M650=0.2795,
                        M700=0.2968,   M750=0.3139,
                        M800=0,        M850=0,
                        M900=0,        M950=0,
                        M1000=0
                        )
    

LQxsecArray        = [ ]
Up1LQxsecArray     = [ ]
Down1LQxsecArray   = [ ]
ObsLimitArray      = [ ]
ExpLimitArray      = [ ]
Up1ExpLimitArray   = [ ]
Down1ExpLimitArray = [ ]
Up2ExpLimitArray   = [ ]
Down2ExpLimitArray = [ ]
MassArray          = [ ]

for ifile in xrange(0,len(InputFileArray)):
    if debug :
        print "Open ",InputFileArray[ifile]
    f=TFile(InputFileArray[ifile],"READ")
    if debug :
        f.ls()
        print
    Tlimit=f.Get("limit")
    limit = array('d',[0])
    mass  = array('d',[0])
    Tlimit.SetBranchAddress( "limit", limit )
    Tlimit.SetBranchAddress( "mh", mass )
    if debug :
        for row in Tlimit:
            print row.limit,"  (",row.quantileExpected,")   Mass:",row.mh
    print
    #
    # Get LQ tag
    Tlimit.GetEntry(2); MassArray.append(mass[0])
    lq3massTag=str(MassArray[-1]); lq3massTag=lq3massTag[:-2]; lq3massTag="M"+lq3massTag
    print "lq3massTag: ",lq3massTag
    #
    Tlimit.GetEntry(0); Down2ExpLimitArray.append(limit[0]*LQxsecDict[lq3massTag])
    Tlimit.GetEntry(1); Down1ExpLimitArray.append(limit[0]*LQxsecDict[lq3massTag])
    Tlimit.GetEntry(2); ExpLimitArray.append(limit[0]*LQxsecDict[lq3massTag])
    Tlimit.GetEntry(3); Up1ExpLimitArray.append(limit[0]*LQxsecDict[lq3massTag])
    Tlimit.GetEntry(4); Up2ExpLimitArray.append(limit[0]*LQxsecDict[lq3massTag])
    Tlimit.GetEntry(5); ObsLimitArray.append(limit[0]*LQxsecDict[lq3massTag])
    LQxsecArray.append(LQxsecDict[lq3massTag])
    Up1LQxsecArray.append(LQxsecDict[lq3massTag]*(1.0+LQxsecSysDict[lq3massTag]))
    Down1LQxsecArray.append(LQxsecDict[lq3massTag]*(1.0-LQxsecSysDict[lq3massTag]))
    # ------------------------
    #root [13] tr->Show(3)
    #======> EVENT:3
    #limit           = 1.77145
    #limitErr        = 0
    #mh              = 600
    #syst            = 1
    #iToy            = -1
    #iSeed           = 0
    #iChannel        = 0
    #t_cpu           = 0
    #t_real          = 0
    #quantileExpected = 0.84
    # ------------------------
print
print "Input Parameters are:  Mass ExpLimit + Upper - Lower"
for i in xrange(0,len(ExpLimitArray)):
    print MassArray[i]," ",ExpLimitArray[i]," +",Up1ExpLimitArray[i],"(",Up2ExpLimitArray[i],") -",Down1ExpLimitArray[i],"(",Down2ExpLimitArray[i],")"
print

#MeanExpLimitHisto = TH1D("MeanExpLimitHisto","MeanExpLimitHisto",7,array('d',[150,250,350,450,525,575,650,750])) # DOES NOT WORK!
#ObsLimitHisto = TH1D("ObsLimitHisto","ObsLimitHisto",11,175,725)#mass steps: 50GeV
#MeanExpLimitHisto = TH1D("MeanExpLimitHisto","MeanExpLimitHisto",11,175,725)#mass steps: 50GeV
#MeanExpLimitHisto = TH1D("MeanExpLimitHisto","MeanExpLimitHisto",6,150,750)#mass steps: 100GeV
MeanExpLimit1SigmaHisto = TGraphAsymmErrors()
MeanExpLimit2SigmaHisto = TGraphAsymmErrors()

MeanExpLimitHisto = TGraph(len(MassArray),array('d',MassArray),array('d',ExpLimitArray))
ObsLimitHisto = TGraph(len(MassArray),array('d',MassArray),array('d',ObsLimitArray))


for i in xrange(0,len(ExpLimitArray)):
    print i," ",MassArray[i]," ",ExpLimitArray[i]," ",Up1ExpLimitArray[i]," ",Down1ExpLimitArray[i]
    MeanExpLimit1SigmaHisto.SetPoint(i,MassArray[i],ExpLimitArray[i])
    MeanExpLimit1SigmaHisto.SetPointEYhigh(i,Up1ExpLimitArray[i]-ExpLimitArray[i])
    MeanExpLimit1SigmaHisto.SetPointEYlow(i,ExpLimitArray[i]-Down1ExpLimitArray[i])
    #MeanExpLimitHisto.Fill(MassArray[i],ExpLimitArray[i])
    #ObsLimitHisto.Fill(MassArray[i],ObsLimitArray[i])
    #
    MeanExpLimit2SigmaHisto.SetPoint(i,MassArray[i],ExpLimitArray[i])
    MeanExpLimit2SigmaHisto.SetPointEYhigh(i,Up2ExpLimitArray[i]-ExpLimitArray[i])
    MeanExpLimit2SigmaHisto.SetPointEYlow(i,ExpLimitArray[i]-Down2ExpLimitArray[i])

"""
for i in xrange(2,MeanExpLimitHisto.GetNbinsX()):
    if MeanExpLimitHisto.GetBinContent(i)==0 and MeanExpLimitHisto.GetBinContent(i+1)!=0 and MeanExpLimitHisto.GetBinContent(i-1)!=0 :
        print "MeanExpLimitHisto:: Filling in the gaps: >>>  M",MeanExpLimitHisto.GetBinCenter(i)
        MeanExpLimitHisto.SetBinContent(i,(MeanExpLimitHisto.GetBinContent(i+1)+MeanExpLimitHisto.GetBinContent(i-1))*0.5)
for i in xrange(2,ObsLimitHisto.GetNbinsX()):
    if ObsLimitHisto.GetBinContent(i)==0 and ObsLimitHisto.GetBinContent(i+1)!=0 and ObsLimitHisto.GetBinContent(i-1)!=0 :
        print "ObsLimitHisto:: Filling in the gaps: >>>  M",ObsLimitHisto.GetBinCenter(i)
        ObsLimitHisto.SetBinContent(i,(ObsLimitHisto.GetBinContent(i+1)+ObsLimitHisto.GetBinContent(i-1))*0.5)
"""

# Plotting

# Define Colors
sig1Col=kGreen+3
sig2Col=kOrange-2

#Set Canvas
can=TCanvas("can","can",0,0,650,650)
can.cd()
can.SetLogy(1)
gStyle.SetOptStat(0)
can.SetLeftMargin(0.115); can.SetRightMargin(0.085); can.SetTopMargin(0.085); can.SetBottomMargin(0.085)
can.SetTickx(1); can.SetTicky(1)

#Set CMS text
CMStext = TPaveText(0.21,0.12,0.37,0.220,"NDC");
#CMStext = TPaveText(0.6,0.785,0.77,0.89,"NDC");
CMStext.SetFillStyle(0)
CMStext.SetFillColor(0)
CMStext.SetLineColor(0)
CMStext.SetBorderSize(1)
CMStext.AddText("CMS Preliminary")
CMStext.AddText("#sqrt{s}=8 TeV")
#CMStext.SetTextSize(0.042)
CMStext.SetTextSize(0.04)
CMStext.SetTextFont(42)


#Set Legend
#legend=TLegend(0.5,0.63,0.76,0.87)
legend=TLegend(0.38,0.63,0.76,0.87)
legend.SetFillColor(0)
legend.SetLineColor(0)
legend.SetFillColor(0)
legend.SetTextSize(0.037)

#LQ3 xsec line
LQ3xsecLine = TGraph(len(MassArray),array('d',MassArray),array('d',LQxsecArray))
LQ3xsecLine.SetLineColor(2)
LQ3xsecLine.SetLineWidth(2)
Up1LQ3xsecLine = TGraph(len(MassArray),array('d',MassArray),array('d',Up1LQxsecArray))
Up1LQ3xsecLine.SetLineColor(2)
Up1LQ3xsecLine.SetLineWidth(2)
Up1LQ3xsecLine.SetLineStyle(2)
Down1LQ3xsecLine = TGraph(len(MassArray),array('d',MassArray),array('d',Down1LQxsecArray))
Down1LQ3xsecLine.SetLineColor(2)
Down1LQ3xsecLine.SetLineWidth(2)
Down1LQ3xsecLine.SetLineStyle(2)


#Set Base with MeanExpLimitHisto (TGraph)
MeanExpLimitHisto.SetLineColor(1)
MeanExpLimitHisto.SetLineWidth(2)
MeanExpLimitHisto.SetLineStyle(2)
MeanExpLimitHisto.SetTitle("")
MeanExpLimitHisto.GetYaxis().SetTitleOffset(1.5)
MeanExpLimitHisto.GetYaxis().CenterTitle()
#MeanExpLimitHisto.GetYaxis().SetTitle("95% CL limit on #sigma/#sigma_{LQ_{3}}")
MeanExpLimitHisto.GetYaxis().SetTitle("#sigma_{LQ_{3}}#times#beta^{2} (pb)")
#MeanExpLimitHisto.GetYaxis().SetRangeUser(0.005,3)
MeanExpLimitHisto.GetYaxis().SetRangeUser(0.005,40.)
MeanExpLimitHisto.GetXaxis().SetTitleOffset(1.2)
MeanExpLimitHisto.GetXaxis().CenterTitle()
MeanExpLimitHisto.GetXaxis().SetTitle("LQ_{3} Mass (GeV)")
#MeanExpLimitHisto.GetXaxis().SetRangeUser(300,650)
MeanExpLimitHisto.GetXaxis().SetRangeUser(150,850)
MeanExpLimitHisto.SetMarkerStyle(24)
#
ObsLimitHisto.SetLineColor(1)
ObsLimitHisto.SetLineWidth(2)
ObsLimitHisto.SetLineStyle(1)
ObsLimitHisto.SetMarkerStyle(9)

#Set 1,2 Sigma bands
MeanExpLimit2SigmaHisto.SetFillColor(sig2Col)
MeanExpLimit2SigmaHisto.SetLineColor(sig2Col)
MeanExpLimit1SigmaHisto.SetFillColor(sig1Col)
MeanExpLimit1SigmaHisto.SetLineColor(sig1Col)


#Draw Histograms
MeanExpLimitHisto.Draw("AL")
MeanExpLimit2SigmaHisto.Draw("E3Lsame")
MeanExpLimit1SigmaHisto.Draw("E3Lsame")
MeanExpLimitHisto.Draw("Lsame")
ObsLimitHisto.SetMarkerStyle(9)
ObsLimitHisto.Draw("PLsame")
LQ3xsecLine.Draw("LSame")
Up1LQ3xsecLine.Draw("LSame")
Down1LQ3xsecLine.Draw("LSame")
legend.SetHeader("Barrel Channel: |#tilde{#eta}|<0.9")
legend.AddEntry(ObsLimitHisto,"Observed Limit (19.5/fb)","PL")
#legend.AddEntry(ObsLimitHisto,"Observed Limit (MC - rounded Up)","L")
#legend.AddEntry(ObsLimitHisto,"Observed Limit (MC - rounded Down)","L")
legend.AddEntry(MeanExpLimitHisto,"Expected Limit","L")
#legend.AddEntry(MeanExpLimitHisto,"Expected Limit (MC)","L")
legend.AddEntry(MeanExpLimit1SigmaHisto,"Expected 68%","F")
legend.AddEntry(MeanExpLimit2SigmaHisto,"Expected 95%","F")
#legend.AddEntry(LQ3xsecLine,"#sigma_{LQ_{3}}#times#beta^{2} #pm #sigma_{pdf}#oplus#sigma_{theory}","L")
legend.AddEntry(LQ3xsecLine,"#sigma_{LQ_{3}}#times#scale[0.5]{ }#beta^{2}#scale[0.5]{  }#pm#scale[0.5]{  }#sigma_{theory}  #scale[0.8]{(#beta=1)}","L")
CMStext.Draw();
legend.Draw();
gPad.RedrawAxis();

can.SaveAs("MyLimitPlotter_"+input1+"_"+input2+"_BarrelWShapeSys.pdf")
     