#!/usr/bin/env python
from ROOT import *
import ROOT as r
import sys
#from Style import *
from array import array
import os
import copy

def HistoStyle(histo,icol):
    xLabelSize=0.045
    yLabelSize=0.045
    xTitleSize=0.045
    yTitleSize=0.045
    histo.GetXaxis().SetTitle(xLabel)
    histo.GetYaxis().SetTitle(yLabel)
    histo.GetXaxis().CenterTitle()
    histo.GetYaxis().CenterTitle()
    histo.GetXaxis().SetLabelSize(xLabelSize)
    histo.GetYaxis().SetLabelSize(yLabelSize)
    histo.GetXaxis().SetTitleSize(xTitleSize)
    histo.GetYaxis().SetTitleSize(yTitleSize)
    histo.SetMarkerColor(icol)
    histo.SetLineColor(icol)
    histo.SetMarkerStyle(9)
    histo.SetMarkerSize(0.3)

def DoRebinArrayV2( binarray ):
    binarray.append(0)
    binarray.append(20)
    for ibin in xrange(1,24):
        binarray.append(20+ibin*20)
    binarray.append(500)

def DoRebinArray(histo,binarray,target):
    #
    binarray.append(18.5)
    binarray.append(20)
    binContent=0
    #
    # high pt binning
    #binarray.append(100)
    #
    for ibin in xrange(1,histo.GetNbinsX()+1):
        #
        # high pt binning
        #if histo.GetXaxis().GetBinLowEdge(ibin)<100 : continue
        #
        binContent+=histo.GetBinContent(ibin)
        if ibin==histo.GetNbinsX() :
            continue
        if binContent>=target:
            binarray.append(histo.GetXaxis().GetBinLowEdge(ibin)+histo.GetXaxis().GetBinWidth(ibin))
            binContent=0            
            
    #binarray.append(300)
    binarray.append(500)


r.TH1.SetDefaultSumw2()

#dir="Jul04Backup_VLooseToTight_Tau_PROMPT/"#isojet>=0 #RelXsec for TTJets and WJets
dir="Jul24Backup_VLooseToTight_Tau_PROMPT/"#isojet>=0 #RelXsec for TTJets and WJets
#
#dir="Jul19Backup_VLooseToTight_MuTauSS_FAKE/"#RelXsec for TTJets and WJets

filett = TFile(dir+"DR0toInf_Tau_TTJets_RelXsec_Histo1D2D.root","READ")
filew  = TFile(dir+"DR0toInf_Tau_W1234Jets_RelXsec_Histo1D2D.root","READ")
filelq = TFile(dir+"DR0toInf_Tau_LQ3_Histo1D2D.root","READ")

filett.ls()

ttcolor=4
wcolor=3
lqcolor=2
mcolor=1

logxon=0

etareg="B"
#etareg="E"
#param="JetDeltaR"; xLabel="#DeltaR(#tau,j)_{min}"; xlow=0; xhigh=4; ylow=0.5; yhigh=500000; binbucket=2
#param="rhoJets"; xLabel="#rho_{jets} "; xlow=0; xhigh=50; ylow=0.5; yhigh=500000; binbucket=10
#param="PU"; xLabel="Pile-up Vertices"; xlow=-0.5; xhigh=50.5; ylow=0.5; yhigh=500000; binbucket=5
#param="Pt"; xLabel="Tau p_{T} (GeV)";xlow=18.5; xhigh=80; ylow=0.05; yhigh=500000; binbucket=1;logxon=0
param="Pt"; xLabel="Tau p_{T} (GeV)";xlow=18.5; xhigh=510; ylow=0.05; yhigh=500000; binbucket=1;logxon=1 # for Rates 2000 Events each
#param="Pt"; xLabel="Tau p_{T} (GeV)";xlow=10; xhigh=510; ylow=0.05; yhigh=500000; binbucket=1;logxon=0 # for Distribution
#param="Pt"; xLabel="Tau p_{T} (GeV)";xlow=18.5; xhigh=310; ylow=0.05; yhigh=500000; binbucket=1;logxon=1 # for Rates 1000 Events each
#param="Pt"; xLabel="Tau p_{T} (GeV)";xlow=10; xhigh=510; ylow=0.05; yhigh=500000; binbucket=1;logxon=0 # for Distribution
#param="Eta"; xLabel="Tau |#eta|"; xlow=-0.1; xhigh=1.6; ylow=0.05; yhigh=5000000; binbucket=1;
#param="Eta"; xLabel="Tau |#eta|"; xlow=1.4; xhigh=2.2; ylow=0.05; yhigh=5000000; binbucket=1
#param="IsoST"; xLabel="S_{T} (GeV)"; xlow=00; xhigh=2000; ylow=0.05; yhigh=500000; binbucket=20;
#param="IsoST"; xLabel="S_{T} (GeV)"; xlow=00; xhigh=2000; ylow=0.05; yhigh=500000; binbucket=40;
#param="JetN"; xLabel="Jet Multiplicity"; xlow=-0.5; xhigh=10.5; ylow=0.05; yhigh=500000; binbucket=1;
#param="Flavor";xLabel="TauJet Flavour (pdgID)"; xlow=-8; xhigh=25; ylow=0; yhigh=500000; binbucket=1;
tautype="Prompt"; yLabel="p_{#tau}"
#tautype="Fake"; yLabel="f_{#tau}"
param=param+etareg
if etareg=="B" : yLabel=yLabel+"  (Barrel)"
if etareg=="E" : yLabel=yLabel+"  (Endcap)"

# ---> 80,0,4 <--- 0.05 per bin, 
#
JetDeltaRminBoundaryLow   = [ 11,15,17,21,1 ,1, 1, 1 ,1 ,1 ,1 ,1  ]
JetDeltaRminBoundaryHigh  = [ 14,16,20,80,80,80,80,80,80,80,80,80 ]


labelArray = [ ]
legendLArray = [ ]
legendLRArray = [ ]
legendHArray = [ ]
#
ttTotal = [ ]; ttPass  = [ ]; ttRatio = [ ]
wTotal  = [ ]; wPass   = [ ]; wRatio  = [ ]
lqTotal = [ ]; lqPass  = [ ]; lqRatio = [ ]

ttTotalJetDeltaR   = [ ]; ttPassJetDeltaR    = [ ]; ttRatioJetDeltaR   = [ ]
wTotalJetDeltaR    = [ ]; wPassJetDeltaR     = [ ]; wRatioJetDeltaR    = [ ]
lqTotalJetDeltaR   = [ ]; lqPassJetDeltaR    = [ ]; lqRatioJetDeltaR   = [ ]
ResultsArray = [ ]

H2DttTotal = filett.Get(tautype+"TotalTau1"+param+"Histo")
H2DttPass  = filett.Get(tautype+"PassTau1"+param+"Histo")
H2DwTotal  = filew.Get(tautype+"TotalTau1"+param+"Histo")
H2DwPass   = filew.Get(tautype+"PassTau1"+param+"Histo")
H2DlqTotal = filelq.Get(tautype+"TotalTau1"+param+"Histo")
H2DlqPass  = filelq.Get(tautype+"PassTau1"+param+"Histo")

H2DttTotalJetDeltaR = filett.Get(tautype+"TotalTau1JetDeltaR"+etareg+"Histo")
H2DttPassJetDeltaR  = filett.Get(tautype+"PassTau1JetDeltaR"+etareg+"Histo")
H2DwTotalJetDeltaR  = filew.Get(tautype+"TotalTau1JetDeltaR"+etareg+"Histo")
H2DwPassJetDeltaR   = filew.Get(tautype+"PassTau1JetDeltaR"+etareg+"Histo")
H2DlqTotalJetDeltaR = filelq.Get(tautype+"TotalTau1JetDeltaR"+etareg+"Histo")
H2DlqPassJetDeltaR  = filelq.Get(tautype+"PassTau1JetDeltaR"+etareg+"Histo")

r.gStyle.SetTitleFont(22,"T"); 

ttRebinArrayALL= [ ]
wRebinArrayALL = [ ]
lqRebinArrayALL = [ ]
print " --- Rebinning  - begin ---"
for iS in xrange(0,len(JetDeltaRminBoundaryLow)):
    ttTotalforRebin=TH1D(H2DttTotal.ProjectionX("ttTotal",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone())
    wTotalforRebin=TH1D(H2DwTotal.ProjectionX("wTotal",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone())
    lqTotalforRebin=TH1D(H2DlqTotal.ProjectionX("lqTotal",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone())
    #
    ttRebinArray = [ ]
    wRebinArray = [ ]
    lqRebinArray = [ ]
    #
    #DoRebinArrayV2( wRebinArray ); DoRebinArrayV2( ttRebinArray ); DoRebinArrayV2( lqRebinArray )
    #DoRebinArray( wTotalforRebin,wRebinArray,1000 ); DoRebinArray(ttTotalforRebin,ttRebinArray,1000 ); DoRebinArray( lqTotalforRebin,lqRebinArray,1000 )
    #DoRebinArray( wTotalforRebin,wRebinArray,1500 ); DoRebinArray(ttTotalforRebin,ttRebinArray,1500 ); DoRebinArray( lqTotalforRebin,lqRebinArray,1500 )
    #DoRebinArray( wTotalforRebin,wRebinArray,2000 ); DoRebinArray(ttTotalforRebin,ttRebinArray,2000 ); DoRebinArray( lqTotalforRebin,lqRebinArray,2000 )
    #DoRebinArray( wTotalforRebin,wRebinArray,200 ); DoRebinArray(ttTotalforRebin,ttRebinArray,200 ); DoRebinArray( lqTotalforRebin,lqRebinArray,200 )
    #
    #DoRebinArray( wTotalforRebin,wRebinArray,800 ); DoRebinArray(ttTotalforRebin,ttRebinArray,800 ); DoRebinArray( lqTotalforRebin,lqRebinArray,800 )
    #DoRebinArray( wTotalforRebin,wRebinArray,250 ); DoRebinArray(ttTotalforRebin,ttRebinArray,250 ); DoRebinArray( lqTotalforRebin,lqRebinArray,250 )
    DoRebinArray( wTotalforRebin,wRebinArray,500 ); DoRebinArray(ttTotalforRebin,ttRebinArray,500 ); DoRebinArray( lqTotalforRebin,lqRebinArray,500 )
    #DoRebinArray( wTotalforRebin,wRebinArray,750 ); DoRebinArray(ttTotalforRebin,ttRebinArray,750 ); DoRebinArray( lqTotalforRebin,lqRebinArray,750 )
    #
    ttRebinArrayALL.append(copy.deepcopy(ttRebinArray))
    wRebinArrayALL.append(copy.deepcopy(wRebinArray))
    lqRebinArrayALL.append(copy.deepcopy(lqRebinArray))
    #
    ttRebinArray = [ ]
    wRebinArray = [ ]
    lqRebinArray = [ ]
###----------
for  iS in xrange(0,len(ttRebinArrayALL)) :
    print "ttRebinArray: ",JetDeltaRminBoundaryLow[iS]," - ",JetDeltaRminBoundaryHigh[iS]
    for ibin in xrange(0,len(ttRebinArrayALL[iS])):
        print ttRebinArrayALL[iS][ibin]
    print "wRebinArray: ",JetDeltaRminBoundaryLow[iS]," - ",JetDeltaRminBoundaryHigh[iS]
    for ibin in xrange(0,len(wRebinArrayALL[iS])):
        print wRebinArrayALL[iS][ibin]
print " --- Rebinning  - end   ---"




#Loop over slices in Lep-JetDeltaRmin
for iS in xrange(0,len(JetDeltaRminBoundaryLow)):
    #
    #special for PT binning
    if "Pt" in param :
        ttptbins = array('d',ttRebinArrayALL[iS])
        wptbins = array('d',wRebinArrayALL[iS])
        lqptbins = array('d',lqRebinArrayALL[iS])
        lenttptbins=len(ttptbins)-1
        lenwptbins=len(wptbins)-1
        lenlqptbins=len(lqptbins)-1
        ttTotal.append((H2DttTotal.ProjectionX("ttTotal",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone()).Rebin(lenttptbins,"ttTotal",ttptbins))
        ttPass.append((H2DttPass.ProjectionX("ttPass",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone()).Rebin(lenttptbins,"ttPass",ttptbins))
        wTotal.append((H2DwTotal.ProjectionX("wTotal",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone()).Rebin(lenwptbins,"wTotal",wptbins))
        wPass.append((H2DwPass.ProjectionX("wPass",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone()).Rebin(lenwptbins,"wPass",wptbins))
        lqTotal.append((H2DlqTotal.ProjectionX("lqTotal",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone()).Rebin(lenlqptbins,"lqTotal",lqptbins))
        lqPass.append( (H2DlqPass.ProjectionX("lqPass",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone()).Rebin(lenlqptbins,"lqPass",lqptbins))
    #
    if "Pt" not in param :
        ttTotal.append( (H2DttTotal.ProjectionX("ttTotal",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone()).Rebin(binbucket) )
        ttPass.append( (H2DttPass.ProjectionX("ttPass",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone()).Rebin(binbucket) )
        wTotal.append( (H2DwTotal.ProjectionX("wTotal",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone()).Rebin(binbucket) )
        wPass.append( (H2DwPass.ProjectionX("wPass",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone()).Rebin(binbucket) )
        lqTotal.append( (H2DlqTotal.ProjectionX("lqTotal",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone()).Rebin(binbucket) )
        lqPass.append( (H2DlqPass.ProjectionX("lqPass",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone()).Rebin(binbucket) )
    #
    dRbucket=1
    ttTotalJetDeltaR.append( (H2DttTotalJetDeltaR.ProjectionX("ttTotalJDR",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone()).Rebin(dRbucket) )
    ttPassJetDeltaR.append( (H2DttPassJetDeltaR.ProjectionX("ttPassJDR",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone()).Rebin(dRbucket) )
    wTotalJetDeltaR.append( (H2DwTotalJetDeltaR.ProjectionX("wTotalJDR",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone()).Rebin(dRbucket) )
    wPassJetDeltaR.append( (H2DwPassJetDeltaR.ProjectionX("wPassJDR",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone()).Rebin(dRbucket) )
    lqTotalJetDeltaR.append( (H2DlqTotalJetDeltaR.ProjectionX("lqTotalJDR",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone()).Rebin(dRbucket) )
    lqPassJetDeltaR.append( (H2DlqPassJetDeltaR.ProjectionX("lqPassJDR",JetDeltaRminBoundaryLow[iS],JetDeltaRminBoundaryHigh[iS],"e").Clone()).Rebin(dRbucket) )
    #
    asymRatioPROXY = r.TGraphAsymmErrors()
    asymRatioPROXY.Divide(ttPass[iS],ttTotal[iS],"cl=0.683 b(1,1) mode")
    ttRatio.append(asymRatioPROXY.Clone())
    asymRatioPROXY.Divide(wPass[iS],wTotal[iS],"cl=0.683 b(1,1) mode")
    wRatio.append(asymRatioPROXY.Clone())
    asymRatioPROXY.Divide(lqPass[iS],lqTotal[iS],"cl=0.683 b(1,1) mode")
    lqRatio.append(asymRatioPROXY.Clone())
    #
    lowEdge=H2DttTotal.ProjectionY("",1,1,"e").GetBinLowEdge(JetDeltaRminBoundaryLow[iS])
    binwidth=H2DttTotal.ProjectionY("",1,1,"e").GetBinWidth(JetDeltaRminBoundaryHigh[iS])
    highEdge=H2DttTotal.ProjectionY("",1,1,"e").GetBinLowEdge(JetDeltaRminBoundaryHigh[iS])+binwidth
    labelPROXY="#DeltaR: "+str(lowEdge)+"-"+str(highEdge)+""
    #
    labelArray.append(labelPROXY)
    #
    HistoStyle(ttTotal[iS],ttcolor)
    HistoStyle(ttPass[iS],ttcolor)
    HistoStyle(ttRatio[iS],ttcolor)
    HistoStyle(wTotal[iS],wcolor)
    HistoStyle(wPass[iS],wcolor)
    HistoStyle(wRatio[iS],wcolor)
    HistoStyle(lqTotal[iS],lqcolor)
    HistoStyle(lqPass[iS],lqcolor)
    HistoStyle(lqRatio[iS],lqcolor)

#Define Legend
#if "Prompt" in tautype     : legendL=r.TLegend(0.13,0.64,0.35,0.91)
#if "Fake" in tautype    : legendL=r.TLegend(0.13,0.64,0.35,0.91)
if "Fake" in tautype     : legendL=r.TLegend(0.70,0.64,0.92,0.91)
if "Prompt" in tautype     : legendL=r.TLegend(0.70,0.64,0.92,0.91)
legendLR=r.TLegend(0.70,0.64,0.92,0.91)
#legendH=r.TLegend(0.13,0.14,0.35,0.43)
legendH=r.TLegend(0.70,0.64,0.92,0.91)
legendL.SetFillColor(0); legendL.SetTextSize(0.053)
legendLR.SetFillColor(0); legendLR.SetTextSize(0.053)
legendH.SetFillColor(0); legendH.SetTextSize(0.053)

for iS in xrange(0,len(JetDeltaRminBoundaryLow)) :
    legendL.Clear()
    legendL.SetHeader(" "+labelArray[iS])
    legendL.AddEntry(ttTotal[0],"tt+jets","PL");    legendL.AddEntry(wTotal[0], "W+jets","PL");    legendL.AddEntry(lqTotal[0],"LQ3","PL")
    legendLArray.append(legendL.Clone())
    legendLR.Clear()
    legendLR.SetHeader(" "+labelArray[iS])
    legendLR.AddEntry(ttTotal[0],"tt+jets","PL");    legendLR.AddEntry(wTotal[0], "W+jets","PL");    legendLR.AddEntry(lqTotal[0],"LQ3","PL")
    legendLRArray.append(legendLR.Clone())
    legendH.Clear()
    legendH.SetHeader(" "+labelArray[iS])
    legendH.AddEntry(ttTotal[0],"tt+jets","PL");    legendH.AddEntry(wTotal[0], "W+jets","PL");    legendH.AddEntry(lqTotal[0],"LQ3","PL")
    legendHArray.append(legendH.Clone())
    
#Define Canvas
r.gStyle.SetOptStat(0)
can1 = TCanvas()
#can1.Divide(3,3,0.00001,0.00001)
can1.Divide(2,2,0.00001,0.00001)
for ican1 in xrange(0,len(JetDeltaRminBoundaryLow)) :
    can1.cd(ican1+1)
    can1.cd(ican1+1).SetGridy(1)
    can1.cd(ican1+1).SetLogx(logxon)
    can1.cd(ican1+1).SetTicky(1)
    can1.cd(ican1+1).SetTickx(1)
    can1.cd(ican1+1).SetLeftMargin(0.15)
    can1.cd(ican1+1).SetRightMargin(0.05)

can2 = TCanvas()
can2.Divide(1,1,0,0)
can2.SetGridy(1)
can2.SetTicky(1)
can2.SetTickx(1)
can2.SetLeftMargin(0.1)
can2.SetRightMargin(0.05)


#Draw
#for ican1 in xrange(0,9) :
for ican1 in xrange(0,4) :
    can1.cd(ican1+1)
    can1.cd(ican1+1).SetLeftMargin(0.1)
    can1.cd(ican1+1).SetTopMargin(0.05)
    #
    if "Pt" in param :
        #linear2tt   = TF1("linear2tt",'pol0',20,300); linear2w    = TF1("linear2w",'pol0',20,300)
        linear2tt   = TF1("linear2tt",'pol0',20,500); linear2w    = TF1("linear2w",'pol0',20,500)
        #
        # high pt binning
        #linear2tt   = TF1("linear2tt",'pol0',100,500); linear2w    = TF1("linear2w",'pol0',100,500)
        #
    if  "JetN" in param:
        linear2tt   = TF1("linear2tt",'pol0',0,6); linear2w    = TF1("linear2w",'pol0',0,3)
    linear2tt.SetLineColor(ttcolor); linear2w.SetLineColor(wcolor);
    linear2tt.SetLineStyle(7); linear2w.SetLineStyle(7);
    #
    ttRatio[ican1].GetXaxis().SetTickLength(0.04)
    ttRatio[ican1].GetYaxis().SetRangeUser(0,1)
    ttRatio[ican1].GetXaxis().SetLabelSize(0.045)
    ttRatio[ican1].GetXaxis().SetMoreLogLabels()
    ttRatio[ican1].GetXaxis().SetNoExponent()
    ttRatio[ican1].Draw("AP")
    ttRatio[ican1].SetMarkerStyle(9)
    ttRatio[ican1].SetMarkerSize(0.7)
    ttRatio[ican1].GetXaxis().SetLimits(xlow,xhigh)
    if tautype=="Fake": ttRatio[ican1].GetYaxis().SetRangeUser(0,0.5)
    #if tautype=="Fake": ttRatio[ican1].GetYaxis().SetRangeUser(0,1.0)
    wRatio[ican1].Draw("Psame")
    wRatio[ican1].SetMarkerStyle(9)
    wRatio[ican1].SetMarkerSize(0.7)
    lqRatio[ican1].Draw("PSame")
    lqRatio[ican1].SetMarkerStyle(9)
    lqRatio[ican1].SetMarkerSize(0.7)
    #
    #
    if tautype == "Prompt" :
        if "JetN" in param :
            if ican1>0 : legendHArray[ican1].Draw()
            if ican1<1 : legendLArray[ican1].Draw()
        if "JetN" not in param :
            if ican1>1 : legendHArray[ican1].Draw()
            if ican1<2 : legendLArray[ican1].Draw()
    if tautype == "Fake"   : legendLArray[ican1].Draw()
    can1.cd(ican1+1).Update()
    can1.cd(ican1+1).RedrawAxis()
    #
    #if ican1>0 and ( "Pt" in param or "JetN" in param):
    if ican1>0 and  "Pt" in param :
        ttRatio[ican1].Fit(linear2tt,"R0LM")
        wRatio[ican1].Fit(linear2w,"R0LM")
        ttErr2=0; ttBins=0
        wErr2=0;  wBins=0
        wRatioForRMS=wPass[ican1].Clone(); wRatioForRMS.Reset("MICES")
        wRatioForRMS.Divide(wPass[ican1],wTotal[ican1],1,1)
        ttRatioForRMS=ttPass[ican1].Clone(); ttRatioForRMS.Reset("MICES")
        ttRatioForRMS.Divide(ttPass[ican1],ttTotal[ican1],1,1)
        for ibin in xrange(1,wRatioForRMS.GetNbinsX()+1):
            # high pt binning
            #if wRatioForRMS.GetXaxis().GetBinLowEdge(ibin)<100 :continue
            #
            if wTotal[ican1].GetBinContent(ibin)>0:
                print "w: ",linear2w.GetParameter(0),"  ",wRatioForRMS.GetBinContent(ibin),"   X:",wRatioForRMS.GetBinCenter(ibin)
                wErr2+=pow( (linear2w.GetParameter(0)-wRatioForRMS.GetBinContent(ibin)), 2 )
                wBins+=1
        for ibin in xrange(1,ttRatioForRMS.GetNbinsX()+1):
            # high pt binning
            #if ttRatioForRMS.GetXaxis().GetBinLowEdge(ibin)<100 :continue
            #
            if ttTotal[ican1].GetBinContent(ibin)>0:
                print "tt: ",linear2tt.GetParameter(0),"  ",ttRatioForRMS.GetBinContent(ibin),"   X:",ttRatioForRMS.GetBinCenter(ibin)
                ttErr2+=pow( (linear2tt.GetParameter(0)-ttRatioForRMS.GetBinContent(ibin)), 2 )
                ttBins+=1
        """
        for ibin in xrange(0,len(ttptbins)-1) :
            if ttRatio[ican1].GetX()[ibin]>ttptbins[1] and ttRatio[ican1].GetX()[ibin]<ttptbins[len(ttptbins)-1] :
                if ttRatio[ican1].GetY()[ibin]>0.01 and ttRatio[ican1].GetY()[ibin]<=1.0 :
                    print "tt: ",linear2tt.GetParameter(0),"  ",ttRatio[ican1].GetY()[ibin],"   X:",ttRatio[ican1].GetX()[ibin]
                    ttErr2+=pow( (linear2tt.GetParameter(0)-ttRatio[ican1].GetY()[ibin]), 2 )
                    ttBins+=1
        for ibin in xrange(0,len(wptbins)-1) :
            if wRatio[ican1].GetX()[ibin]>wptbins[1] and wRatio[ican1].GetX()[ibin]<wptbins[len(wptbins)-1] :
                if wRatio[ican1].GetY()[ibin]>0.01 and wRatio[ican1].GetY()[ibin]<=1.0 :
                    print "w: ",linear2w.GetParameter(0),"  ",wRatio[ican1].GetY()[ibin],"   X:",wRatio[ican1].GetX()[ibin]
                    wErr2+=pow( (linear2w.GetParameter(0)-wRatio[ican1].GetY()[ibin]), 2 )
                    wBins+=1
        """        
        print " ---- Results 4x4----  "
        print JetDeltaRminBoundaryLow[ican1]," - ",JetDeltaRminBoundaryHigh[ican1]
        print "tt p0:",linear2tt.GetParameter(0)," +/- ",pow( ttErr2/ttBins, 0.5 )
        print "w  p0:",linear2w.GetParameter(0)," +/- ",pow( wErr2/wBins, 0.5 )
        print "mean MC p0:",(linear2tt.GetParameter(0)+linear2w.GetParameter(0))/2.0," +/- ",pow( ((ttErr2/ttBins)+(wErr2/wBins))/2.0, 0.5 ),
        print " +/- ",abs((linear2tt.GetParameter(0)-linear2w.GetParameter(0))/2)
        print "pure fit tt: ",linear2tt.GetParameter(0)," +/- ",linear2tt.GetParError(0),"  prob:",linear2tt.GetProb()
        print "pure fit w: ",linear2w.GetParameter(0)," +/- ",linear2w.GetParError(0),"  prob:",linear2w.GetProb()
        ResultsArray.append(str(JetDeltaRminBoundaryLow[ican1])+"-"+str(JetDeltaRminBoundaryHigh[ican1])+" W fit rms    : "+str(linear2w.GetParameter(0))+" +/- "+str(pow(wErr2/wBins,0.5)))
        wNDF=linear2w.GetNDF();
        ttNDF=linear2tt.GetNDF();
        if wNDF==0  : wNDF=1;
        if ttNDF==0 : ttNDF=1;
        ResultsArray.append(str(JetDeltaRminBoundaryLow[ican1])+"-"+str(JetDeltaRminBoundaryHigh[ican1])+" W fit fiterr : "+str(linear2w.GetParameter(0))+" +/- "+str(linear2w.GetParError(0))+"  prob:"+str(linear2w.GetProb())+" ndf:"+str(linear2w.GetNDF())+" Chi2/ndf:"+str(linear2w.GetChisquare()/wNDF))
        ResultsArray.append("")
        ResultsArray.append(str(JetDeltaRminBoundaryLow[ican1])+"-"+str(JetDeltaRminBoundaryHigh[ican1])+" tt fit rms   : "+str(linear2tt.GetParameter(0))+" +/- "+str(pow(ttErr2/ttBins,0.5)))
        ResultsArray.append(str(JetDeltaRminBoundaryLow[ican1])+"-"+str(JetDeltaRminBoundaryHigh[ican1])+" tt fit fiterr: "+str(linear2tt.GetParameter(0))+" +/- "+str(linear2tt.GetParError(0))+"  prob:"+str(linear2tt.GetProb())+" ndf:"+str(linear2tt.GetNDF())+" Chi2/ndf:"+str(linear2tt.GetChisquare()/ttNDF))
        ResultsArray.append("")
        ResultsArray.append(str(JetDeltaRminBoundaryLow[ican1])+"-"+str(JetDeltaRminBoundaryHigh[ican1])+" Ave(tt,W) DeltaMC : "+str((linear2tt.GetParameter(0)+linear2w.GetParameter(0))/2.0)+" +/- "+str(pow( ((ttErr2/ttBins)+(wErr2/wBins))/2.0, 0.5 ))+" +/- "+str(abs((linear2tt.GetParameter(0)-linear2w.GetParameter(0))/2)))
        ResultsArray.append("")
        ResultsArray.append("")
        linear2tt.Draw("same")
        linear2w.Draw("same")


can1.Print("Tau"+tautype+"Rate_"+param+".pdf(")

#Draw
for ican2 in xrange(0,len(JetDeltaRminBoundaryLow)):
    can2.cd()
    can2.cd().SetTopMargin(0.05) #added for the fake plots
    #can2.cd().SetLogy(1)
    can2.cd().SetLogx(logxon)
    ttRatio[ican2].GetXaxis().SetLimits(xlow,xhigh)
    ttRatio[ican2].GetYaxis().SetRangeUser(0,1)
    ttRatio[ican2].GetXaxis().SetTickLength(0.04)
    ttRatio[ican2].GetXaxis().SetLabelSize(0.045)
    ttRatio[ican2].GetXaxis().SetMoreLogLabels()
    ttRatio[ican2].GetXaxis().SetNoExponent()
    if tautype=="Fake": ttRatio[ican2].GetYaxis().SetRangeUser(0,0.5)
    ttRatio[ican2].SetMarkerSize(0.75)
    wRatio[ican2].SetMarkerSize(0.75)
    lqRatio[ican2].SetMarkerSize(0.75)
    lqRatio[ican2].SetMarkerStyle(24)
    ttRatio[ican2].Draw("APE")
    wRatio[ican2].Draw("Psame")
    lqRatio[ican2].Draw("PSame")
    #
    #linear3tt   = TF1("linear3tt",'pol0',20,300); linear3w    = TF1("linear3w",'pol0',20,300)
    linear3tt   = TF1("linear3tt",'pol0',20,500); linear3w    = TF1("linear3w",'pol0',20,500)
    #
    # high pt binning
    #linear3tt   = TF1("linear3tt",'pol0',100,500); linear3w    = TF1("linear3w",'pol0',100,500)
    #
    linear3tt.SetLineColor(ttcolor); linear3w.SetLineColor(wcolor);
    linear3tt.SetLineStyle(7); linear3w.SetLineStyle(7);
    #
    #linear2ttA   = TF1("linear2ttA",'pol0',20,120);  linear2wA    = TF1("linear2wA",'pol0',20,120)
    #linear2ttB   = TF1("linear2ttB",'pol0',120,500); linear2wB    = TF1("linear2wB",'pol0',120,500)
    #linear2ttA.SetLineColor(ttcolor); linear2wA.SetLineColor(wcolor);
    #linear2ttA.SetLineStyle(7);       linear2wA.SetLineStyle(7);
    #linear2ttB.SetLineColor(ttcolor); linear2wB.SetLineColor(wcolor);
    #linear2ttB.SetLineStyle(7);       linear2wB.SetLineStyle(7);
    #wRatio[ican2].Fit(linear2wA,"R0M")
    #wRatio[ican2].Fit(linear2wB,"R0M")
    #ttRatio[ican2].Fit(linear2ttA,"R0M")
    #ttRatio[ican2].Fit(linear2ttB,"R0M")
    #
    if ican2>=0 and "Pt" in param:
        print "Fitting ican2: ",ican2
        ttRatio[ican2].Fit(linear3tt,"R0M")
        wRatio[ican2].Fit(linear3w,"R0M")
        linear3tt.Draw("same")
        linear3w.Draw("same")
        #linear2ttA.Draw("same"); linear2ttB.Draw("same");
        #linear2wA.Draw("same");  linear2wB.Draw("same");
    #
    if tautype == "Prompt" and ican2>1 : legendHArray[ican2].Draw()
    if tautype == "Prompt" and ican2<2 : legendLArray[ican2].Draw()
    if tautype == "Fake"   : legendLArray[ican2].Draw()
    can2.cd(ican2+1).Update()
    can2.cd(ican2+1).RedrawAxis()
    can2.Print("Tau"+tautype+"Rate_"+param+".pdf")



#Draw
for ican2 in xrange(0,len(JetDeltaRminBoundaryLow)):
    can2.cd()
    can2.SetLogy(1)
    ttTotal[ican2].GetXaxis().SetTickLength(0.04)
    ttTotal[ican2].GetXaxis().SetLabelSize(0.045)
    ttTotal[ican2].GetXaxis().SetMoreLogLabels()
    ttTotal[ican2].GetXaxis().SetNoExponent()
    ttTotal[ican2].Draw("HIST")
    ttTotal[ican2].SetTitle("")
    ttTotal[ican2].GetYaxis().SetRangeUser(0.5,500000)
    #ttTotal[ican2].GetYaxis().SetRangeUser(0.5,5000000)
    wTotal[ican2].Draw("HIST same")
    lqTotal[ican2].Draw("HIST same")
    ttTotal[ican2].Draw("HIST sameaxis")
    can2.cd(ican2+1).Update()
    can2.cd(ican2+1).RedrawAxis()
    legendLArray[ican2].Draw()
    can2.Print("Tau"+tautype+"Rate_"+param+".pdf")


#Draw
for ican1 in xrange(0,4) :
    can1.cd(ican1+1)
    can1.cd(ican1+1).SetLogy(1)
    #
    ttTotal[ican1].GetXaxis().SetTickLength(0.04)
    ttTotal[ican1].GetXaxis().SetLabelSize(0.045)
    ttTotal[ican1].GetXaxis().SetMoreLogLabels()
    ttTotal[ican1].GetXaxis().SetNoExponent()
    ttTotal[ican1].Draw("HIST")
    ttTotal[ican1].SetTitle("")
    #ttTotal[ican1].GetYaxis().SetRangeUser(0.5,20000000)
    ttTotal[ican1].GetYaxis().SetRangeUser(0.5,500000)
    wTotal[ican1].Draw("HIST same")
    lqTotal[ican1].Draw("HIST same")
    ttTotal[ican1].Draw("HIST sameaxis")
    can1.cd(ican1+1).Update()
    can1.cd(ican1+1).RedrawAxis()
    #
    legendLRArray[ican1].Draw()

can1.Print("Tau"+tautype+"Rate_"+param+".pdf")


#Draw TEXT
can3 = TCanvas()
t =r.TText();
for ibin in xrange(0,len(ResultsArray)):
    t.SetTextSize(0.02);
    t.DrawText(0.2,0.9-ibin*0.035,ResultsArray[ibin]);

can3.Print("Tau"+tautype+"Rate_"+param+".pdf)")



print
print
print "Results"
print
for ibin in xrange(0,len(ResultsArray)):
    print ResultsArray[ibin]
print
print

