#!/usr/bin/env python
from ROOT import *
import os
import sys
from array import array

print "MyLimitPlotter.py - Initializing"

input1=sys.argv[1] #higgsCombineLQ3_FinalResults17Nov2013_Barrel
input2=sys.argv[2] #ProfileLikelihood
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



LQxsecDict = dict( M100=521.0,     M150=74.9,
                   M200=17.4,      M250=5.26,
                   M300=1.89,      M350=0.769,
                   M400=0.342,     M450=0.163,
                   M500=0.082,     M550=0.0431,
                   M600=0.0235,    M650=0.0132,
                   M700=0.00761,   M750=0.00448,
                   M800=0.00269,   M850=0.00164,
                   M900=0.00101,   M950=0.000635,
                   M1000=0.000401
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
    Tlimit.GetEntry(0); Down2ExpLimitArray.append(limit[0])#*LQxsecDict[lq3massTag])
    #Tlimit.GetEntry(1); Down1ExpLimitArray.append(limit[0])#*LQxsecDict[lq3massTag])
    #Tlimit.GetEntry(2); ExpLimitArray.append(limit[0])#*LQxsecDict[lq3massTag])
    #Tlimit.GetEntry(3); Up1ExpLimitArray.append(limit[0])#*LQxsecDict[lq3massTag])
    #Tlimit.GetEntry(4); Up2ExpLimitArray.append(limit[0])#*LQxsecDict[lq3massTag])
    #Tlimit.GetEntry(5); ObsLimitArray.append(limit[0])#*LQxsecDict[lq3massTag])
    #LQxsecArray.append(LQxsecDict[lq3massTag])
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
for i in xrange(0,len(Down2ExpLimitArray)):
    print MassArray[i]," ",Down2ExpLimitArray[i]
print

#exit(0)
# Plotting

# Define Colors
#sig1Col=kGreen+3
#sig2Col=kOrange-2

#Set Canvas
can=TCanvas("can","can",0,0,650,650)
can.cd()
can.SetLogy(0)
gStyle.SetOptStat(0)
can.SetLeftMargin(0.115); can.SetRightMargin(0.085); can.SetTopMargin(0.085); can.SetBottomMargin(0.085)
can.SetTickx(1); can.SetTicky(1)

#Set CMS text
CMStext = TPaveText(0.63,0.735,0.845,0.84,"NDC");
#CMStext = TPaveText(0.21,0.785,0.37,0.89,"NDC");
#CMStext = TPaveText(0.21,0.12,0.37,0.220,"NDC");
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
#legend=TLegend(0.38,0.63,0.76,0.87)
#legend=TLegend(0.58,0.80,0.86,0.88)
#legend=TLegend(0.54,0.85,0.84,0.89)
legend=TLegend(0.43,0.845,0.835,0.89)
legend.SetFillColor(0)
legend.SetLineColor(0)
legend.SetFillColor(0)
legend.SetTextSize(0.0375)


LQ3xsecLine = TGraph(len(MassArray),array('d',MassArray),array('d',Down2ExpLimitArray))
LQ3xsecLine.SetLineColor(2)
LQ3xsecLine.SetLineWidth(2)

#sigma3line= TF1("sigma3line",'3',0,1000)
sigma3line = TH1D("sigma3line","sigma3line",1000,0,1000)
for ibin in xrange(1,1000):
    sigma3line.SetBinContent(ibin,3)
sigma3line.SetLineColor(2)
sigma3line.SetLineWidth(2)

#Set Base with MeanExpLimitHisto (TGraph)
LQ3xsecLine.SetLineColor(1)
LQ3xsecLine.SetLineWidth(2)
LQ3xsecLine.SetLineStyle(2)
LQ3xsecLine.SetMarkerStyle(9)
LQ3xsecLine.SetTitle("")
LQ3xsecLine.GetYaxis().SetTitleOffset(1.5)
LQ3xsecLine.GetYaxis().CenterTitle()
#LQ3xsecLine.GetYaxis().SetTitle("#sigma_{LQ_{3}}#times#beta^{2} (pb)")
LQ3xsecLine.GetYaxis().SetTitle("Expected Significance")# (a posteriori)")
#LQ3xsecLine.GetYaxis().SetRangeUser(0.005,3)
LQ3xsecLine.GetYaxis().SetRangeUser(0,9)
LQ3xsecLine.GetXaxis().SetTitleOffset(1.2)
LQ3xsecLine.GetXaxis().CenterTitle()
LQ3xsecLine.GetXaxis().SetTitle("LQ_{3} Mass (GeV)")
#LQ3xsecLine.GetXaxis().SetRangeUser(300,650)
LQ3xsecLine.GetXaxis().SetRangeUser(150,850)
#LQ3xsecLine.SetMarkerStyle(24)

#Draw Histograms
LQ3xsecLine.Draw("PAL")
sigma3line.Draw("HIST ][same")
LQ3xsecLine.Draw("PLsame")
legend.SetHeader("Combination: Barrel & Endcap")
#legend.SetHeader("Barrel Channel: |#tilde{#eta}|<0.9")
#legend.AddEntry(ObsLimitHisto,"Observed Limit (19.5/fb)","PL")
#legend.AddEntry(MeanExpLimitHisto,"Expected Limit","L")
#legend.AddEntry(MeanExpLimit1SigmaHisto,"Expected 68%","F")
#legend.AddEntry(MeanExpLimit2SigmaHisto,"Expected 95%","F")
#legend.AddEntry(LQ3xsecLine,"#sigma_{LQ_{3}}#times#scale[0.5]{ }#beta^{2}#scale[0.5]{  }#pm#scale[0.5]{  }#sigma_{theory}  #scale[0.8]{(#beta=1)}","L")
CMStext.Draw();
legend.Draw();
gPad.RedrawAxis();

can.SaveAs("MyLimitPlotter_"+input1+"_"+input2+"_Combo_aPostSignif.pdf")
     
