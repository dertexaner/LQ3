#!/usr/bin/env python
from ROOT import *
import ROOT as r
import os
import sys
#
print
print     "Produce a Ratio Plot from 2 hisograms:"
print     "        Usage: ./RatioOf2Histos.py <inputFile> <numHiso> <denomHisto> <OPT::rebinValue> <OPT::XaxisLabel> <OPT::YaxisLabel> <OPT::OutputName.Format>" 
if len(sys.argv)<4 or len(sys.argv)>8 :
    print "        Exiting.. "
    print
    exit(1)
print
#
filename=sys.argv[1]
histoname1=sys.argv[2]
histoname2=sys.argv[3]
rebinValue=1
xlabel="xlabel"
ylabel="ylabel"
outputName="test.root"
if len(sys.argv)>4: rebinValue=(int)(sys.argv[4])
if len(sys.argv)>5: xlabel=sys.argv[5]
if len(sys.argv)>6: ylabel=sys.argv[6]
if len(sys.argv)>7: outputName=sys.argv[7]
filepath= str(filename)
file=r.TFile(filepath)
file.ls()
print
histo1=r.TH1D(file.Get(histoname1))
histo2=r.TH1D(file.Get(histoname2))
#
print "  Numerator.GetEntries(): ",histo1.GetEntries()
print "Denominator.GetEntries(): ",histo2.GetEntries()
print "  Numerator.GetNbinsX() : ",histo1.GetNbinsX()
print "Denominator.GetNbinsX() : ",histo2.GetNbinsX()
#
ratio=r.TGraphAsymmErrors()
ratio.Divide( histo1.Rebin(rebinValue), histo2.Rebin(rebinValue), "vcp" )
for ibin in xrange(1,ratio.GetN()+1):
    print "Bin(",ibin,") -> ",ratio.GetX()[ibin],": ",ratio.GetY()[ibin]
#
can=r.TCanvas()
can.cd()
ratio.GetXaxis().SetTitle(xlabel)
ratio.GetYaxis().SetTitle(ylabel)
ratio.SetTitle("")
ratio.Draw("AP")
can.SaveAs(outputName)
#
#
exit(0)

