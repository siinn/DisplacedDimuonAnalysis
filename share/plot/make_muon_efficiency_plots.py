#!/usr/bin/python

#=============================================
# making plots for the following algorithms
# * MuonEfficiency
#=============================================

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
f_m250t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.301911.Truth.root", 'READ') 
f_m250t250 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.301912.Truth.root", 'READ') 
f_m250t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.301913.Truth.root", 'READ') 
f_m500t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.301914.Truth.root", 'READ') 
f_m500t250 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.301915.Truth.root", 'READ') 
f_m500t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.301916.Truth.root", 'READ') 
f_m1000t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.301917.Truth.root", 'READ') 
f_m1000t250 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.301918.Truth.root", 'READ') 
f_m1000t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output.301919.Truth.root", 'READ') 

#=============================================
# define colors
#=============================================

color_m250t100  =   kBlue+2
color_m250t250  =   kBlue-7
color_m250t500  =   kBlue-10
color_m500t100  =   kGreen+2
color_m500t250  =   kGreen-5
color_m500t500  =   kGreen-10
color_m1000t100 =   kRed+1
color_m1000t250 =   kRed-7
color_m1000t500 =   kRed-10

#=============================================
# Truth muon eta, l
#=============================================

parameters = ['eta', 'pt', 'R', 'd0']
parameters_label = [
                    '#eta_{#mu}',
                    'p_{T}^{#mu}',
                    'R_{DV}',
                    'd_{0}^{#mu}',
                   ]


for p in range(len(parameters)):

    # define legends
    legends = [
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 100 mm',
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 500 mm',
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 100 mm',
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 500 mm',
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 100 mm',
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 500 mm',
                ]

    # initialize legend
    legend = TLegend(0.35,0.72,0.82,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_muon_efficiency_%s" % parameters[p], "", 600, 600)

    # set log
    log = False

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_m250t100.Get('Muons/Efficiency/m_eff_%s' % parameters[p]))
    histograms.append(f_m250t250.Get('Muons/Efficiency/m_eff_%s' % parameters[p]))
    histograms.append(f_m250t500.Get('Muons/Efficiency/m_eff_%s' % parameters[p]))
    histograms.append(f_m500t100.Get('Muons/Efficiency/m_eff_%s' % parameters[p]))
    histograms.append(f_m500t250.Get('Muons/Efficiency/m_eff_%s' % parameters[p]))
    histograms.append(f_m500t500.Get('Muons/Efficiency/m_eff_%s' % parameters[p]))
    histograms.append(f_m1000t100.Get('Muons/Efficiency/m_eff_%s' % parameters[p]))
    histograms.append(f_m1000t250.Get('Muons/Efficiency/m_eff_%s' % parameters[p]))
    histograms.append(f_m1000t500.Get('Muons/Efficiency/m_eff_%s' % parameters[p]))

    # set histogram color 
    histograms[0].SetLineColor(color_m250t100)
    histograms[1].SetLineColor(color_m250t250)
    histograms[2].SetLineColor(color_m250t500)
    histograms[3].SetLineColor(color_m500t100)
    histograms[4].SetLineColor(color_m500t250)
    histograms[5].SetLineColor(color_m500t500)
    histograms[6].SetLineColor(color_m1000t100)
    histograms[7].SetLineColor(color_m1000t250)
    histograms[8].SetLineColor(color_m1000t500)

    # set histogram marker color
    histograms[0].SetMarkerColor(color_m250t100)
    histograms[1].SetMarkerColor(color_m250t250)
    histograms[2].SetMarkerColor(color_m250t500)
    histograms[3].SetMarkerColor(color_m500t100)
    histograms[4].SetMarkerColor(color_m500t250)
    histograms[5].SetMarkerColor(color_m500t500)
    histograms[6].SetMarkerColor(color_m1000t100)
    histograms[7].SetMarkerColor(color_m1000t250)
    histograms[8].SetMarkerColor(color_m1000t500)

    # set fill color

    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        histograms[i].Sumw2()
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
            #histograms[i].SetMaximum(hmax_global*1.5)
            #histograms[i].SetMinimum(0)
            histograms[i].SetMaximum(1.3)
            histograms[i].SetMinimum(0.6)
        if (log ==True):
            histograms[i].SetMaximum(hmax_global*10e1)
            histograms[i].SetMinimum(hmin_global*10e-2)
        histograms[i].SetMarkerSize(0.4)
        histograms[i].SetLineWidth(2)
        histograms[i].GetXaxis().SetTitle(parameters_label[p])
        histograms[i].GetYaxis().SetTitle("Efficiency")
        histograms[i].GetXaxis().SetNdivisions(6)
        histograms[i].Draw("same e")
        legend.AddEntry(histograms[i], legends[i], "L")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/muon_efficiency_%s.pdf" % parameters[p])


