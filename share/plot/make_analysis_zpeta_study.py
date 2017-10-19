#!/usr/bin/python
import os, sys
from ROOT import *
from math import log10

# loading ATLAS style
gROOT.LoadMacro("AtlasStyle.C")
gROOT.LoadMacro("AtlasUtils.C")
SetAtlasStyle()

# set text option
gStyle.SetPaintTextFormat(".0f");

#=============================================
# Convenient little functions
#=============================================

def getMaximum(h):
    hmax = 0
    for i in range(h.GetXaxis().GetNbins()):
        hmax = max(hmax, h.GetBinContent(i + 1))
        pass
    return hmax

def getMinimumNonzero(h):
    hmin = float('Inf')
    for i in range(h.GetXaxis().GetNbins()):
        c = h.GetBinContent(i + 1)
        if c > 0:
            hmin = min(hmin, c)
            pass
        pass
    return hmin


#=============================================
# input
#=============================================
#f_data = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.full.overlap.root", 'READ') 

f_m250t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.zpeta.root", 'READ') 
f_m250t100_truth = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.301911.Truth.root", 'READ') 
#f_m1000t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.301917.root", 'READ') 
#f_m1000t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.301919.root", 'READ') 
#
#f_lllvm = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.lllvm.root", 'READ') 
#f_lllvp = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.lllvp.root", 'READ') 
#f_llvv = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.llvv.root", 'READ') 
#f_llll = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.llll.root", 'READ') 
#f_ttbar = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.ttbar.noTrigMatching.root", 'READ') 

#=============================================
# define colors
#=============================================

color_data      =   kBlack
color_data_pt0  =   kAzure-4
color_data_pt50 =   kAzure-5
color_data_pt60 =   kAzure-6

color_lllvm     =   kBlue-7
color_lllvp     =   kBlue-9
color_llvv      =   kBlue-10
color_llll      =   kOrange+1
color_ttbar     =   kRed-7

color_m1000t100 =   kRed-4
color_m250t100  =   kRed-9
color_m250t500  =   kRed+2
color_m1000t500 =   kRed-5

#=============================================
# Event cutflow
#=============================================

#if False:
if True:

    # define legends
    legends = [
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 100 mm (Reconstructed)',
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 100 mm (Generated)',
                ]

    # initialize legend
    legend = TLegend(0.2,0.87,0.50,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_dv_matched_eta", "", 600, 600)

    # set log
    log = False

    if (log == True):
        c.SetLogy()


    # read histograms
    histograms = []

    histograms.append(f_m250t100.Get('dv_mumu/truth-matched/dv_mumu_eta'))
    #histograms.append(f_m250t100_truth.Get('Truth/signal_zp/eta/m_zp_eta'))

    # set histogram color 
    histograms[0].SetLineColor(color_m250t100)
    #histograms[1].SetLineColor(kBlack)

    # set fill color
    histograms[0].SetFillColor(color_m250t100)

    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        # scale
        #if (histograms[i].Integral() > 0):
        #    histograms[i].Scale( 1. / histograms[i].Integral())

        hmax = getMaximum(histograms[i])
        if (hmax > hmax_global):
            hmax_global = hmax
        hmin = getMinimumNonzero(histograms[i])
        if (hmin < hmin_global):
            hmin_global = hmin
 
    for i in range(len(histograms)):
        if (log ==False):
            histograms[i].SetMaximum(hmax_global*2.0)
            histograms[i].SetMinimum(0)
        if (log ==True):
            histograms[i].SetMaximum(10e5)
            histograms[i].SetMinimum(1e-1)
        histograms[i].GetYaxis().SetTitle("N")
        histograms[i].GetXaxis().SetTitle("#eta")
        #histograms[i].Draw("HIST SAME")
        print histograms[i].Integral(9,34)
        print histograms[i].Integral()
        legend.AddEntry(histograms[i], legends[i], "F")


    histograms[0].Draw("HIST SAME")
    #histograms[1].Draw("HIST SAME")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/truth_matched_dv_mumu_eta.pdf")


