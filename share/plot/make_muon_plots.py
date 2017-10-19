#!/usr/bin/python
import os, sys
from ROOT import *
from math import log10

# loading ATLAS style
gROOT.LoadMacro("AtlasStyle.C")
gROOT.LoadMacro("AtlasUtils.C")
SetAtlasStyle()

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
f_data = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.data.root", 'READ') 
f_lllvm = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.lllvm.root", 'READ') 
f_m250t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.301911.root", 'READ') 
f_m250t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.301913.root", 'READ') 
f_m1000t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.301917.root", 'READ') 
f_m1000t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.301919.root", 'READ') 

#=============================================
# define colors
#=============================================

color_data      =   kBlack
color_data_pt0  =   kAzure-4
color_data_pt50 =   kAzure-5
color_data_pt60 =   kAzure-6

color_lllvm     =   kGreen-5

color_m1000t500 =   kRed-5
color_m1000t100 =   kRed+2    # dark
color_m250t100  =   kRed-4    
color_m250t500  =   kRed-9    # light


#=============================================
# muon distribution
#=============================================

parameters = ['pt', 'pt_low','trig_matched_pt', 'trig_matched_pt_low','trig_matched_pt_msonly', 'trig_matched_pt_low_msonly']
parameters_label = [
                    'p_{T} [GeV]',
                    'p_{T} [GeV]',
                    'p_{T} [GeV]',
                    'p_{T} [GeV]',
                    'p_{T} [GeV]',
                    'p_{T} [GeV]'
                   ]

for p in range(len(parameters)):

    # define legends
    legends = [ '2016 Data, 33.1 fb^{-1}, normalized',
                'MC W^{-}Z, normalized',
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 100 mm, normalized',
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 500 mm, normalized',
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 100 mm, normalized',
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 500 mm, normalized',
                ]

    # initialize legend
    legend = TLegend(0.23,0.68,0.79,0.93)
 
    # initialize canvas
    c= TCanvas("m_muon_%s" % parameters[p], "", 600, 600)


    # set log
    log = False

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_data.Get('muons/reconstructed/m_mu_%s' % parameters[p] ))
    histograms.append(f_lllvm.Get('muons/reconstructed/m_mu_%s' % parameters[p] ))
    histograms.append(f_m250t100.Get('muons/reconstructed/m_mu_%s' % parameters[p] ))
    histograms.append(f_m250t500.Get('muons/reconstructed/m_mu_%s' % parameters[p] ))
    histograms.append(f_m1000t100.Get('muons/reconstructed/m_mu_%s' % parameters[p] ))
    histograms.append(f_m1000t500.Get('muons/reconstructed/m_mu_%s' % parameters[p] ))

    # set histogram color 
    histograms[0].SetLineColor(color_data_pt0)
    histograms[1].SetLineColor(color_lllvm)
    histograms[2].SetLineColor(color_m250t100)
    histograms[3].SetLineColor(color_m250t500)
    histograms[4].SetLineColor(color_m1000t100)
    histograms[5].SetLineColor(color_m1000t500)

    # set fill color
    #histograms[0].SetFillColor(color_data_pt0)
    #histograms[3].SetFillColor(color_m250t100) 
    #histograms[4].SetFillColor(color_m250t500) 
    #histograms[5].SetFillColor(color_m1000t100) 
    #histograms[6].SetFillColor(color_m1000t500) 
    
    # scale
    #histograms[1].Scale(2.78)

    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        # scale
        if (histograms[i].Integral() > 0):
            histograms[i].Scale( 1. / histograms[i].Integral())

        hmax = getMaximum(histograms[i])
        if (hmax > hmax_global):
            hmax_global = hmax
        hmin = getMinimumNonzero(histograms[i])
        if (hmin < hmin_global):
            hmin_global = hmin
 
    for i in range(len(histograms)):
        if (log ==False):
            histograms[i].SetMaximum(hmax_global*1.5)
            histograms[i].SetMinimum(0)
        if (log ==True):
            histograms[i].SetMaximum(hmax_global*10e1)
            #histograms[i].SetMinimum(hmin_global*10e-1)
            histograms[i].SetMinimum(1)
        histograms[i].SetLineWidth(3)
        histograms[i].GetXaxis().SetTitle(parameters_label[p])
        histograms[i].GetYaxis().SetTitle("Entries")
        histograms[i].GetXaxis().SetNdivisions(6)
        legend.AddEntry(histograms[i], legends[i], "L")

    # custom drawing
    histograms[0].Draw("hist same")
    histograms[1].Draw("hist same")
    histograms[2].Draw("hist same")
    histograms[4].Draw("hist same")
    histograms[3].Draw("hist same")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/m_muon_%s.pdf" % parameters[p])
