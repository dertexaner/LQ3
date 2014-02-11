#!/usr/bin/env python
from ROOT import *
import sys
#from Style import *
from array import array
import os

#dir="/afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_SIGNAL_CutFlowEfficiency_V1/"
#dir="/afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_SIGNAL_CutFlowEfficiency_V2/"
dir="/afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_SIGNAL_CutFlowEfficiency_V4/"

F250 = TFile(dir+"analysisClass_SIGNAL_CutFlowEfficiency___inputList_RootNtuple-V00-03-11_LQToTTau_M-250.root","READ")
F350 = TFile(dir+"analysisClass_SIGNAL_CutFlowEfficiency___inputList_RootNtuple-V00-03-11_LQToTTau_M-350.root","READ")
F450 = TFile(dir+"analysisClass_SIGNAL_CutFlowEfficiency___inputList_RootNtuple-V00-03-11_LQToTTau_M-450.root","READ")
F550 = TFile(dir+"analysisClass_SIGNAL_CutFlowEfficiency___inputList_RootNtuple-V00-03-11_LQToTTau_M-550.root","READ")
F650 = TFile(dir+"analysisClass_SIGNAL_CutFlowEfficiency___inputList_RootNtuple-V00-03-11_LQToTTau_M-650.root","READ")

H250 = F250.Get("cutFlowRatioHisto")
H350 = F350.Get("cutFlowRatioHisto")
H450 = F450.Get("cutFlowRatioHisto")
H550 = F550.Get("cutFlowRatioHisto")
H650 = F650.Get("cutFlowRatioHisto")

gStyle.SetOptStat(0)

can1 = TCanvas()
can1.SetLogy(1)
can1.SetGridy(1)
can1.SetTicky(1)
can1.SetTickx(1)
H250.SetLineColor(1); H250.SetMarkerColor(1); H250.SetMarkerStyle(9); H250.Draw("PL");
H250.GetXaxis().SetBinLabel(1,"Gen Mu+Tau (SS)")
#H250.GetXaxis().SetBinLabel(4,"Mu+Tau (OS+SS)")
#H250.GetXaxis().SetBinLabel(6,"Mu+Tau (SS)")
H250.GetXaxis().SetLabelSize(0.04)
H250.GetYaxis().SetRangeUser(0.009,1.199); H250.GetYaxis().SetTitle("Signal Efficiency");  H250.GetYaxis().CenterTitle(); H250.SetTitle("")
H350.SetLineColor(2); H350.SetMarkerColor(2); H350.SetMarkerStyle(9); H350.Draw("PLsame")
H450.SetLineColor(3); H450.SetMarkerColor(3); H450.SetMarkerStyle(9); H450.Draw("PLsame")
H550.SetLineColor(4); H550.SetMarkerColor(4); H550.SetMarkerStyle(9); H550.Draw("PLsame")
H650.SetLineColor(6); H650.SetMarkerColor(6); H650.SetMarkerStyle(9); H650.Draw("PLsame")

legend=TLegend(0.14,0.14,0.33,0.4)
legend.SetFillColor(0); legend.SetTextSize(0.04)
legend.AddEntry(H250,"LQ3 M250","P")
legend.AddEntry(H350,"LQ3 M350","P")
legend.AddEntry(H450,"LQ3 M450","P")
legend.AddEntry(H550,"LQ3 M550","P")
legend.AddEntry(H650,"LQ3 M650","P")
legend.Draw()

can1.SaveAs("PlotCutFlowEfficiecy.pdf")
can1.SaveAs("PlotCutFlowEfficiecy.root")
