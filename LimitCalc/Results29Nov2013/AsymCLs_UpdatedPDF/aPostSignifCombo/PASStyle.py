#!/usr/bin/env python
from ROOT import *
import os
import sys
#from array import array

def CreateCanvasPAS( ) :
    can=TCanvas("can","can",0,0,650,650)
    can.cd()
    can.SetLogy(1)
    gStyle.SetOptStat(0)
    can.SetLeftMargin(0.16) 
    can.SetRightMargin(0.02)
    can.SetTopMargin(0.05)
    can.SetBottomMargin(0.13)
    can.SetTickx(1)
    can.SetTicky(1)
    return can

def SetHistoStylePAS( histo ) :
    #histo.SetEndErrorSize(2)
    #histo.SetErrorX(0.)
    #histo.SetMarkerStyle(20)
    #histo.SetOptTitle(0);
    histo.GetXaxis().SetTitleFont(42);
    histo.GetYaxis().SetTitleFont(42);
    histo.GetXaxis().SetTitleSize(0.05);
    histo.GetYaxis().SetTitleSize(0.05);
    histo.GetXaxis().SetTitleFont(42)
    histo.GetYaxis().SetTitleFont(42)
    histo.GetXaxis().SetLabelFont(42)
    histo.GetYaxis().SetLabelFont(42)
    histo.GetXaxis().SetTitleSize(0.055)
    histo.GetYaxis().SetTitleSize(0.055)
    histo.GetXaxis().SetTitleOffset(1.1)
    histo.GetYaxis().SetTitleOffset(1.25)
    histo.GetXaxis().SetLabelOffset(0.007)
    histo.GetYaxis().SetLabelOffset(0.007)
    histo.GetXaxis().SetLabelSize(0.045)
    histo.GetYaxis().SetLabelSize(0.045)
    #For the axis:
    #histo.SetAxisColor(1, "XYZ");
    #histo.SetStripDecimals(kTRUE);
    #histo.SetTickLength(0.03, "XYZ");
    #histo.SetNdivisions(510, "XYZ");

def SetLegendStylePAS( legend ) :
    legend.SetFillColor(0)
    legend.SetLineColor(0)
    legend.SetFillColor(0)
    #legend.SetTextSize(0.037)
    #legend.SetTextSize(0.04)
    legend.SetTextSize(0.045)
    legend.SetTextFont(42)

def SetTextStylePAS( text ) :
    text.SetFillStyle(0)
    text.SetFillColor(0)
    text.SetLineColor(0)
    text.SetBorderSize(1)
    #text.AddText("CMS Preliminary")
    #text.AddText("#sqrt{s}=8 TeV")
    text.AddText("CMS Preliminary, #sqrt{s}=8 TeV")
    text.SetTextSize(0.045)
    #text.SetTextSize(0.04)
    text.SetTextFont(42)

