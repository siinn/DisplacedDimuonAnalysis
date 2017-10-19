#!/usr/bin/python

#=============================================
# making plots for the following algorithms
# * TruthPlots
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
f_ee_m100t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.ee.m100t100.root", 'READ')
f_ee_m100t250 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.ee.m100t250.root", 'READ')
f_ee_m100t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.ee.m100t500.root", 'READ')
f_ee_m250t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.ee.m250t100.root", 'READ')
f_ee_m250t250 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.ee.m250t250.root", 'READ')
f_ee_m250t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.ee.m250t500.root", 'READ')
f_ee_m500t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.ee.m500t100.root", 'READ')
f_ee_m500t250 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.ee.m500t250.root", 'READ')
f_ee_m500t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.ee.m500t500.root", 'READ')
f_ee_m750t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.ee.m750t100.root", 'READ')
f_ee_m750t250 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.ee.m750t250.root", 'READ')
f_ee_m750t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.ee.m750t500.root", 'READ')
f_ee_m1000t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.ee.m1000t100.root", 'READ')
f_ee_m1000t250 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.ee.m1000t250.root", 'READ')
f_ee_m1000t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.ee.m1000t500.root", 'READ')

f_emu_m100t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.emu.m100t100.root", 'READ')
f_emu_m100t250 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.emu.m100t250.root", 'READ')
f_emu_m100t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.emu.m100t500.root", 'READ')
f_emu_m250t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.emu.m250t100.root", 'READ')
f_emu_m250t250 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.emu.m250t250.root", 'READ')
f_emu_m250t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.emu.m250t500.root", 'READ')
f_emu_m500t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.emu.m500t100.root", 'READ')
f_emu_m500t250 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.emu.m500t250.root", 'READ')
f_emu_m500t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.emu.m500t500.root", 'READ')
f_emu_m750t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.emu.m750t100.root", 'READ')
f_emu_m750t250 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.emu.m750t250.root", 'READ')
f_emu_m750t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.emu.m750t500.root", 'READ')
f_emu_m1000t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.emu.m1000t100.root", 'READ')
f_emu_m1000t250 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.emu.m1000t250.root", 'READ')
f_emu_m1000t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.emu.m1000t500.root", 'READ')

#=============================================
# define decay channel
#=============================================

ch = 'e#mu'
#ch = 'ee'


#=============================================
# define colors
#=============================================

color_m100t100  =   kGreen+1
color_m100t250  =   kGreen+2
color_m100t500  =   kGreen+3
color_m250t100  =   kCyan+1
color_m250t250  =   kCyan+2
color_m250t500  =   kCyan+3
color_m500t100  =   kBlue+1
color_m500t250  =   kBlue+2
color_m500t500  =   kBlue+3
color_m750t100  =   kMagenta+1
color_m750t250  =   kMagenta+2
color_m750t500  =   kMagenta+3
color_m1000t100 =   kRed+1
color_m1000t250 =   kRed+2
color_m1000t500 =   kRed+3


#=============================================
# signal muon plots
#=============================================

parameters = ['m','pt','eta','phi','R','z','pdgId' ]
parameters_label = [
                    'm_{#mu} [GeV]',
                    'p_{T}^{#mu} [GeV]',
                    '#eta_{#mu}',
                    '#phi_{#mu}',
                    'R_{#mu}',
                    'z_{#mu}',
                    'pdgId_{#mu}',
                   ]


for p in range(len(parameters)):

    # define legends
    legends = [
                'MC Z\'#rightarrow%s, m_{Z\'} = 100 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 100 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 100 GeV, c#tau = 500 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 250 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 250 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 250 GeV, c#tau = 500 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 500 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 500 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 500 GeV, c#tau = 500 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 750 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 750 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 750 GeV, c#tau = 500 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 1000 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 1000 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 1000 GeV, c#tau = 500 mm' % ch,
                ]

    # initialize legend
    legend = TLegend(0.35,0.72,0.82,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_signal_muon_%s" %parameters[p], "", 600, 600)

    # set log
    log = False

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    if (ch == 'e#mu'):
        histograms.append(f_emu_m100t100.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m100t250.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m100t500.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m250t100.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m250t250.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m250t500.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m500t100.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m500t250.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m500t500.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m750t100.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m750t250.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m750t500.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m1000t100.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m1000t250.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m1000t500.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))

    if (ch == 'ee'):
        histograms.append(f_ee_m100t100.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m100t250.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m100t500.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m250t100.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m250t250.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m250t500.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m500t100.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m500t250.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m500t500.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m750t100.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m750t250.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m750t500.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m1000t100.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m1000t250.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m1000t500.Get('Truth/signal_muon/%s/m_muon_%s' % (parameters[p], parameters[p]) ))


    # set histogram color 
    histograms[0].SetLineColor(color_m100t100)
    histograms[1].SetLineColor(color_m100t250)
    histograms[2].SetLineColor(color_m100t500)
    histograms[3].SetLineColor(color_m250t100)
    histograms[4].SetLineColor(color_m250t250)
    histograms[5].SetLineColor(color_m250t500)
    histograms[6].SetLineColor(color_m500t100)
    histograms[7].SetLineColor(color_m500t250)
    histograms[8].SetLineColor(color_m500t500)
    histograms[9].SetLineColor(color_m750t100)
    histograms[10].SetLineColor(color_m750t250)
    histograms[11].SetLineColor(color_m750t500)
    histograms[12].SetLineColor(color_m1000t100)
    histograms[13].SetLineColor(color_m1000t250)
    histograms[14].SetLineColor(color_m1000t500)


    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        histograms[i].Sumw2()
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
            histograms[i].SetMinimum(hmin_global*10e-2)
        histograms[i].SetLineWidth(2)
        #histograms[i].SetMarkerSize(0.4)
        histograms[i].GetXaxis().SetTitle(parameters_label[p])
        histograms[i].GetYaxis().SetTitle("Entries")
        histograms[i].GetXaxis().SetNdivisions(6)
        histograms[i].Draw("hist same")
        legend.AddEntry(histograms[i], legends[i], "L")

    # draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/truth_signal_muon_%s.pdf" % parameters[p])


#=============================================
# signal electron plots
#=============================================

parameters = ['m','pt','eta','phi','R','z','pdgId' ]
parameters_label = [
                    'm_{e} [MeV]',
                    'p_{T}^{e} [GeV]',
                    '#eta_{e}',
                    '#phi_{e}',
                    'R_{e}',
                    'z_{e}',
                    'pdgId_{e}',
                   ]


for p in range(len(parameters)):

    # define legends
    legends = [
                'MC Z\'#rightarrow%s, m_{Z\'} = 100 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 100 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 100 GeV, c#tau = 500 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 250 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 250 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 250 GeV, c#tau = 500 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 500 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 500 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 500 GeV, c#tau = 500 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 750 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 750 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 750 GeV, c#tau = 500 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 1000 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 1000 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 1000 GeV, c#tau = 500 mm' % ch,
                ]

    # initialize legend
    legend = TLegend(0.35,0.72,0.82,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_signal_el_%s" %parameters[p], "", 600, 600)

    # set log
    log = False

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    if (ch == 'e#mu'):
        histograms.append(f_emu_m100t100.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m100t250.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m100t500.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m250t100.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m250t250.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m250t500.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m500t100.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m500t250.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m500t500.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m750t100.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m750t250.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m750t500.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m1000t100.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m1000t250.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m1000t500.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))

    if (ch == 'ee'):
        histograms.append(f_ee_m100t100.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m100t250.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m100t500.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m250t100.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m250t250.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m250t500.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m500t100.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m500t250.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m500t500.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m750t100.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m750t250.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m750t500.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m1000t100.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m1000t250.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m1000t500.Get('Truth/signal_el/%s/m_el_%s' % (parameters[p], parameters[p]) ))


    # set histogram color 
    histograms[0].SetLineColor(color_m100t100)
    histograms[1].SetLineColor(color_m100t250)
    histograms[2].SetLineColor(color_m100t500)
    histograms[3].SetLineColor(color_m250t100)
    histograms[4].SetLineColor(color_m250t250)
    histograms[5].SetLineColor(color_m250t500)
    histograms[6].SetLineColor(color_m500t100)
    histograms[7].SetLineColor(color_m500t250)
    histograms[8].SetLineColor(color_m500t500)
    histograms[9].SetLineColor(color_m750t100)
    histograms[10].SetLineColor(color_m750t250)
    histograms[11].SetLineColor(color_m750t500)
    histograms[12].SetLineColor(color_m1000t100)
    histograms[13].SetLineColor(color_m1000t250)
    histograms[14].SetLineColor(color_m1000t500)


    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        histograms[i].Sumw2()
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
            histograms[i].SetMinimum(hmin_global*10e-2)
        histograms[i].SetLineWidth(2)
        #histograms[i].SetMarkerSize(0.4)
        histograms[i].GetXaxis().SetTitle(parameters_label[p])
        histograms[i].GetYaxis().SetTitle("Entries")
        histograms[i].GetXaxis().SetNdivisions(6)
        histograms[i].Draw("hist same")
        legend.AddEntry(histograms[i], legends[i], "L")

    # draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/truth_signal_el_%s.pdf" % parameters[p])


#=============================================
# signal dilep plots
#=============================================

parameters = ['m','pt','dr' ]
parameters_label = [
                    'm_{DV} [GeV]',
                    'p_{T}^{DV} [GeV]',
                    '#DeltaR'
                   ]


for p in range(len(parameters)):

    # define legends
    legends = [
                'MC Z\'#rightarrow%s, m_{Z\'} = 100 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 100 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 100 GeV, c#tau = 500 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 250 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 250 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 250 GeV, c#tau = 500 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 500 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 500 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 500 GeV, c#tau = 500 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 750 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 750 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 750 GeV, c#tau = 500 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 1000 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 1000 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 1000 GeV, c#tau = 500 mm' % ch,
                ]

    # initialize legend
    legend = TLegend(0.35,0.72,0.82,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_signal_dilep_%s" %parameters[p], "", 600, 600)

    # set log
    log = False

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    if (ch == 'e#mu'):
        histograms.append(f_emu_m100t100.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m100t250.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m100t500.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m250t100.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m250t250.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m250t500.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m500t100.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m500t250.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m500t500.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m750t100.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m750t250.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m750t500.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m1000t100.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m1000t250.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m1000t500.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))

    if (ch == 'ee'):
        histograms.append(f_ee_m100t100.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m100t250.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m100t500.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m250t100.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m250t250.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m250t500.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m500t100.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m500t250.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m500t500.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m750t100.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m750t250.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m750t500.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m1000t100.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m1000t250.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m1000t500.Get('Truth/signal_dilep/%s/m_dilep_%s' % (parameters[p], parameters[p]) ))


    # set histogram color 
    histograms[0].SetLineColor(color_m100t100)
    histograms[1].SetLineColor(color_m100t250)
    histograms[2].SetLineColor(color_m100t500)
    histograms[3].SetLineColor(color_m250t100)
    histograms[4].SetLineColor(color_m250t250)
    histograms[5].SetLineColor(color_m250t500)
    histograms[6].SetLineColor(color_m500t100)
    histograms[7].SetLineColor(color_m500t250)
    histograms[8].SetLineColor(color_m500t500)
    histograms[9].SetLineColor(color_m750t100)
    histograms[10].SetLineColor(color_m750t250)
    histograms[11].SetLineColor(color_m750t500)
    histograms[12].SetLineColor(color_m1000t100)
    histograms[13].SetLineColor(color_m1000t250)
    histograms[14].SetLineColor(color_m1000t500)


    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        histograms[i].Sumw2()
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
            histograms[i].SetMinimum(hmin_global*10e-2)
        histograms[i].SetLineWidth(2)
        #histograms[i].SetMarkerSize(0.4)
        histograms[i].GetXaxis().SetTitle(parameters_label[p])
        histograms[i].GetYaxis().SetTitle("Entries")
        histograms[i].GetXaxis().SetNdivisions(6)
        histograms[i].Draw("hist same")
        legend.AddEntry(histograms[i], legends[i], "L")


    # draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/truth_signal_dielp_%s.pdf" % parameters[p])


#=============================================
# signal muon plots
#=============================================

parameters = ['m','pt','eta','phi','R','z','pdgId' ]
parameters_label = [
                    'm_{Z\'} [GeV]',
                    'p_{T}^{Z\'} [GeV]',
                    '#eta_{Z\'}',
                    '#phi_{Z\'}',
                    'R_{Z\'}',
                    'z_{Z\'}',
                    'pdgId_{Z\'}',
                   ]


for p in range(len(parameters)):

    # define legends
    legends = [
                'MC Z\'#rightarrow%s, m_{Z\'} = 100 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 100 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 100 GeV, c#tau = 500 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 250 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 250 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 250 GeV, c#tau = 500 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 500 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 500 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 500 GeV, c#tau = 500 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 750 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 750 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 750 GeV, c#tau = 500 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 1000 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 1000 GeV, c#tau = 250 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 1000 GeV, c#tau = 500 mm' % ch,
                ]

    # initialize legend
    legend = TLegend(0.35,0.72,0.82,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_signal_zp_%s" %parameters[p], "", 600, 600)

    # set log
    log = False

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    if (ch == 'e#mu'):
        histograms.append(f_emu_m100t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m100t250.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m100t500.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m250t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m250t250.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m250t500.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m500t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m500t250.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m500t500.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m750t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m750t250.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m750t500.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m1000t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m1000t250.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m1000t500.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))

    if (ch == 'ee'):
        histograms.append(f_ee_m100t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m100t250.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m100t500.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m250t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m250t250.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m250t500.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m500t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m500t250.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m500t500.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m750t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m750t250.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m750t500.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m1000t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m1000t250.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m1000t500.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))


    # set histogram color 
    histograms[0].SetLineColor(color_m100t100)
    histograms[1].SetLineColor(color_m100t250)
    histograms[2].SetLineColor(color_m100t500)
    histograms[3].SetLineColor(color_m250t100)
    histograms[4].SetLineColor(color_m250t250)
    histograms[5].SetLineColor(color_m250t500)
    histograms[6].SetLineColor(color_m500t100)
    histograms[7].SetLineColor(color_m500t250)
    histograms[8].SetLineColor(color_m500t500)
    histograms[9].SetLineColor(color_m750t100)
    histograms[10].SetLineColor(color_m750t250)
    histograms[11].SetLineColor(color_m750t500)
    histograms[12].SetLineColor(color_m1000t100)
    histograms[13].SetLineColor(color_m1000t250)
    histograms[14].SetLineColor(color_m1000t500)


    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        histograms[i].Sumw2()
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
            histograms[i].SetMinimum(hmin_global*10e-2)
        histograms[i].SetLineWidth(2)
        #histograms[i].SetMarkerSize(0.4)
        histograms[i].GetXaxis().SetTitle(parameters_label[p])
        histograms[i].GetYaxis().SetTitle("Entries")
        histograms[i].GetXaxis().SetNdivisions(6)
        histograms[i].Draw("hist same")
        legend.AddEntry(histograms[i], legends[i], "L")

    # draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/truth_signal_zp_%s.pdf" % parameters[p])



##=============================================
## Truth Z' R
##=============================================
#
#parameters = ['R', 'R_low']
#parameters_label = [
#                    'R_{DV} [mm]',
#                    'R_{DV} [mm]',
#                   ]
#
#
#for p in range(len(parameters)):
#
#    # define legends
#    legends = [
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 100 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 500 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 100 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 250 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 500 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 100 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 250 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 500 mm',
#                ]
#
#    # initialize legend
#    legend = TLegend(0.35,0.75,0.82,0.93)
# 
#    # initialize canvas
#    c= TCanvas("canvas_truth_muon_%s" %parameters[p], "", 600, 600)
#
#    # set log
#    log = False
#
#    if (log == True):
#        c.SetLogy()
#
#    # read histograms
#    histograms = []
#
#    histograms.append(f_m250t100.Get('Truth/signal_zp/R/m_zp_%s' % parameters[p]))
#    histograms.append(f_m250t250.Get('Truth/signal_zp/R/m_zp_%s' % parameters[p]))
#    histograms.append(f_m250t500.Get('Truth/signal_zp/R/m_zp_%s' % parameters[p]))
#    histograms.append(f_m500t100.Get('Truth/signal_zp/R/m_zp_%s' % parameters[p]))
#    histograms.append(f_m500t250.Get('Truth/signal_zp/R/m_zp_%s' % parameters[p]))
#    histograms.append(f_m500t500.Get('Truth/signal_zp/R/m_zp_%s' % parameters[p]))
#    histograms.append(f_m1000t100.Get('Truth/signal_zp/R/m_zp_%s' % parameters[p]))
#    histograms.append(f_m1000t250.Get('Truth/signal_zp/R/m_zp_%s' % parameters[p]))
#    histograms.append(f_m1000t500.Get('Truth/signal_zp/R/m_zp_%s' % parameters[p]))
#
#    # set histogram color 
#    histograms[0].SetLineColor(color_m250t100)
#    histograms[1].SetLineColor(color_m250t250)
#    histograms[2].SetLineColor(color_m250t500)
#    histograms[3].SetLineColor(color_m500t100)
#    histograms[4].SetLineColor(color_m500t250)
#    histograms[5].SetLineColor(color_m500t500)
#    histograms[6].SetLineColor(color_m1000t100)
#    histograms[7].SetLineColor(color_m1000t250)
#    histograms[8].SetLineColor(color_m1000t500)
#
#    histograms[0].SetMarkerColor(color_m250t100)
#    histograms[1].SetMarkerColor(color_m250t250)
#    histograms[2].SetMarkerColor(color_m250t500)
#    histograms[3].SetMarkerColor(color_m500t100)
#    histograms[4].SetMarkerColor(color_m500t250)
#    histograms[5].SetMarkerColor(color_m500t500)
#    histograms[6].SetMarkerColor(color_m1000t100)
#    histograms[7].SetMarkerColor(color_m1000t250)
#    histograms[8].SetMarkerColor(color_m1000t500)
#
#    # set fill color
#
#    # finding global maximum
#    hmax_global = 0
#    hmin_global = 10e10
#
#    for i in range(len(histograms)):
#        histograms[i].Sumw2()
#        # scale
#        if (histograms[i].Integral() > 0):
#            histograms[i].Scale( 1. / histograms[i].Integral())
#
#        hmax = getMaximum(histograms[i])
#        if (hmax > hmax_global):
#            hmax_global = hmax
#        hmin = getMinimumNonzero(histograms[i])
#        if (hmin < hmin_global):
#            hmin_global = hmin
# 
#    for i in range(len(histograms)):
#        if (log ==False):
#            histograms[i].SetMaximum(hmax_global*1.5)
#            histograms[i].SetMinimum(0)
#        if (log ==True):
#            histograms[i].SetMaximum(hmax_global*10e1)
#            histograms[i].SetMinimum(hmin_global*10e-2)
#        histograms[i].SetLineWidth(2)
#        histograms[i].SetMarkerSize(0.4)
#        histograms[i].GetXaxis().SetTitle(parameters_label[p])
#        histograms[i].GetYaxis().SetTitle("Entries")
#        histograms[i].GetXaxis().SetNdivisions(6)
#        histograms[i].Draw("same e")
#        legend.AddEntry(histograms[i], legends[i], "L")
#
#    ## draw legend
#    legend.SetBorderSize(0)
#    legend.SetTextSize(0.025)
#    legend.Draw("SAME")
#
#    c.Print("output/truth_signal_zp_%s.pdf" % parameters[p])
#
##=============================================
## Truth fraction of Z' within 2 mm
##=============================================
#
#parameters = ['dv_cut']
#parameters_label = [
#                    '',
#                   ]
#
#
#for p in range(len(parameters)):
#
#    # define legends
#    legends = [
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 100 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 500 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 100 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 250 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 500 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 100 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 250 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 500 mm',
#                ]
#
#    # initialize legend
#    legend = TLegend(0.35,0.75,0.82,0.93)
# 
#    # initialize canvas
#    c= TCanvas("canvas_truth_zp_%s" %parameters[p], "", 600, 600)
#
#    # set log
#    log = False
#
#    if (log == True):
#        c.SetLogy()
#
#    # read histograms
#    histograms = []
#
#    histograms.append(f_m250t100.Get('Truth/signal_zp/R/m_fraction_%s' % parameters[p]))
#    histograms.append(f_m250t250.Get('Truth/signal_zp/R/m_fraction_%s' % parameters[p]))
#    histograms.append(f_m250t500.Get('Truth/signal_zp/R/m_fraction_%s' % parameters[p]))
#    histograms.append(f_m500t100.Get('Truth/signal_zp/R/m_fraction_%s' % parameters[p]))
#    histograms.append(f_m500t250.Get('Truth/signal_zp/R/m_fraction_%s' % parameters[p]))
#    histograms.append(f_m500t500.Get('Truth/signal_zp/R/m_fraction_%s' % parameters[p]))
#    histograms.append(f_m1000t100.Get('Truth/signal_zp/R/m_fraction_%s' % parameters[p]))
#    histograms.append(f_m1000t250.Get('Truth/signal_zp/R/m_fraction_%s' % parameters[p]))
#    histograms.append(f_m1000t500.Get('Truth/signal_zp/R/m_fraction_%s' % parameters[p]))
#
#    # set histogram color 
#    histograms[0].SetLineColor(color_m250t100)
#    histograms[1].SetLineColor(color_m250t250)
#    histograms[2].SetLineColor(color_m250t500)
#    histograms[3].SetLineColor(color_m500t100)
#    histograms[4].SetLineColor(color_m500t250)
#    histograms[5].SetLineColor(color_m500t500)
#    histograms[6].SetLineColor(color_m1000t100)
#    histograms[7].SetLineColor(color_m1000t250)
#    histograms[8].SetLineColor(color_m1000t500)
#
#    histograms[0].SetMarkerColor(color_m250t100)
#    histograms[1].SetMarkerColor(color_m250t250)
#    histograms[2].SetMarkerColor(color_m250t500)
#    histograms[3].SetMarkerColor(color_m500t100)
#    histograms[4].SetMarkerColor(color_m500t250)
#    histograms[5].SetMarkerColor(color_m500t500)
#    histograms[6].SetMarkerColor(color_m1000t100)
#    histograms[7].SetMarkerColor(color_m1000t250)
#    histograms[8].SetMarkerColor(color_m1000t500)
#
#    histograms[0].SetMarkerStyle(20)
#    histograms[1].SetMarkerStyle(22)
#    histograms[2].SetMarkerStyle(23)
#    histograms[3].SetMarkerStyle(20)
#    histograms[4].SetMarkerStyle(22)
#    histograms[5].SetMarkerStyle(23)
#    histograms[6].SetMarkerStyle(20)
#    histograms[7].SetMarkerStyle(22)
#    histograms[8].SetMarkerStyle(23)
#
#    # set fill color
#
#    # finding global maximum
#    hmax_global = 0
#    hmin_global = 10e10
#
#    for i in range(len(histograms)):
#        histograms[i].Sumw2()
#        ## scale
#        #if (histograms[i].Integral() > 0):
#        #    histograms[i].Scale( 1. / histograms[i].Integral())
#
#        hmax = getMaximum(histograms[i])
#        if (hmax > hmax_global):
#            hmax_global = hmax
#        hmin = getMinimumNonzero(histograms[i])
#        if (hmin < hmin_global):
#            hmin_global = hmin
# 
#    for i in range(len(histograms)):
#        if (log ==False):
#            histograms[i].SetMaximum(hmax_global*1.5)
#            histograms[i].SetMinimum(0)
#        if (log ==True):
#            histograms[i].SetMaximum(hmax_global*10e1)
#            histograms[i].SetMinimum(hmin_global*10e-2)
#        histograms[i].SetLineWidth(2)
#        #histograms[i].SetMarkerSize(0.4)
#        histograms[i].GetXaxis().SetTitle(parameters_label[p])
#        histograms[i].GetYaxis().SetTitle("fraction")
#        histograms[i].GetXaxis().SetNdivisions(6)
#        histograms[i].Draw("same e")
#        legend.AddEntry(histograms[i], legends[i], "P")
#
#        print legends[i]
#        histograms[i].Print("all")
#
#
#    ## draw legend
#    legend.SetBorderSize(0)
#    legend.SetTextSize(0.025)
#    legend.Draw("SAME")
#
#    c.Print("output/truth_signal_%s.pdf" % parameters[p])
#
#
#
##=============================================
## Truth Z' eta, l, z
##=============================================
#
#parameters = ['eta', 'l', 'z']
#parameters_label = [
#                    '#eta',
#                    'l_{DV} [mm]',
#                    'z_{DV} [mm]',
#                   ]
#
#
#for p in range(len(parameters)):
#
#    # define legends
#    legends = [
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 100 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 500 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 100 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 250 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 500 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 100 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 250 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 500 mm',
#                ]
#
#    # initialize legend
#    legend = TLegend(0.35,0.75,0.82,0.93)
# 
#    # initialize canvas
#    c= TCanvas("canvas_truth_zp_%s" % parameters[p], "", 600, 600)
#
#    # set log
#    log = False
#
#    if (log == True):
#        c.SetLogy()
#
#    # read histograms
#    histograms = []
#    histograms.append(f_m250t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p],parameters[p])))
#    histograms.append(f_m250t250.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p],parameters[p])))
#    histograms.append(f_m250t500.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p],parameters[p])))
#    histograms.append(f_m500t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p],parameters[p])))
#    histograms.append(f_m500t250.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p],parameters[p])))
#    histograms.append(f_m500t500.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p],parameters[p])))
#    histograms.append(f_m1000t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p],parameters[p])))
#    histograms.append(f_m1000t250.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p],parameters[p])))
#    histograms.append(f_m1000t500.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p],parameters[p])))
#
#    # set histogram color 
#    histograms[0].SetLineColor(color_m250t100)
#    histograms[1].SetLineColor(color_m250t250)
#    histograms[2].SetLineColor(color_m250t500)
#    histograms[3].SetLineColor(color_m500t100)
#    histograms[4].SetLineColor(color_m500t250)
#    histograms[5].SetLineColor(color_m500t500)
#    histograms[6].SetLineColor(color_m1000t100)
#    histograms[7].SetLineColor(color_m1000t250)
#    histograms[8].SetLineColor(color_m1000t500)
#
#    histograms[0].SetMarkerColor(color_m250t100)
#    histograms[1].SetMarkerColor(color_m250t250)
#    histograms[2].SetMarkerColor(color_m250t500)
#    histograms[3].SetMarkerColor(color_m500t100)
#    histograms[4].SetMarkerColor(color_m500t250)
#    histograms[5].SetMarkerColor(color_m500t500)
#    histograms[6].SetMarkerColor(color_m1000t100)
#    histograms[7].SetMarkerColor(color_m1000t250)
#    histograms[8].SetMarkerColor(color_m1000t500)
#
#
#    # set fill color
#
#    # finding global maximum
#    hmax_global = 0
#    hmin_global = 10e10
#
#    for i in range(len(histograms)):
#        histograms[i].Sumw2()
#        # scale
#        if (histograms[i].Integral() > 0):
#            histograms[i].Scale( 1. / histograms[i].Integral())
#
#        hmax = getMaximum(histograms[i])
#        if (hmax > hmax_global):
#            hmax_global = hmax
#        hmin = getMinimumNonzero(histograms[i])
#        if (hmin < hmin_global):
#            hmin_global = hmin
# 
#    for i in range(len(histograms)):
#        if (log ==False):
#            histograms[i].SetMaximum(hmax_global*1.5)
#            histograms[i].SetMinimum(0)
#        if (log ==True):
#            histograms[i].SetMaximum(hmax_global*10e1)
#            histograms[i].SetMinimum(hmin_global*10e-2)
#        histograms[i].SetLineWidth(2)
#        histograms[i].SetMarkerSize(0.4)
#        histograms[i].GetXaxis().SetTitle(parameters_label[p])
#        histograms[i].GetYaxis().SetTitle("Entries")
#        histograms[i].GetXaxis().SetNdivisions(6)
#        histograms[i].Draw("same e")
#        legend.AddEntry(histograms[i], legends[i], "L")
#
#    ## draw legend
#    legend.SetBorderSize(0)
#    legend.SetTextSize(0.025)
#    legend.Draw("SAME")
#
#    c.Print("output/truth_signal_zp_%s.pdf" % parameters[p])
#
##=============================================
## Truth Z' eta acceptance
##=============================================
#
#parameters = ['muon', 'zp']
#parameters_label = [
#                    'R_{DV} [mm]',
#                   ]
#
#
#for p in range(len(parameters)):
#
#    # define legends
#    legends = [
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 100 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 500 mm',
#                ]
#
#    # initialize legend
#    legend = TLegend(0.35,0.80,0.82,0.93)
# 
#    # initialize canvas
#    c= TCanvas("canvas_truth_zp_%s" % parameters[p], "", 600, 600)
#
#    # set log
#    log = False
#
#    if (log == True):
#        c.SetLogy()
#
#    # read histograms
#    histograms = []
#    histograms.append(f_m250t100.Get('Truth/signal_%s/eta/m_truth_%s_acceptance' % (parameters[p], parameters[p])))
#    histograms.append(f_m250t500.Get('Truth/signal_%s/eta/m_truth_%s_acceptance' % (parameters[p], parameters[p])))
#
#    # set histogram color 
#    histograms[0].SetLineColor(kBlue-4)
#    histograms[1].SetLineColor(kRed-7)
#
#    histograms[0].SetMarkerColor(kBlue-4)
#    histograms[1].SetMarkerColor(kRed-7)
#
#    #histograms[0].SetLineStyle(1)
#    #histograms[1].SetLineStyle(2)
#
#
#    # set fill color
#
#    # finding global maximum
#    hmax_global = 0
#    hmin_global = 10e10
#
#    for i in range(len(histograms)):
#        histograms[i].Sumw2()
#        # scale
#        #if (histograms[i].Integral() > 0):
#        #    histograms[i].Scale( 1. / histograms[i].Integral())
#
#        hmax = getMaximum(histograms[i])
#        if (hmax > hmax_global):
#            hmax_global = hmax
#        hmin = getMinimumNonzero(histograms[i])
#        if (hmin < hmin_global):
#            hmin_global = hmin
# 
#    for i in range(len(histograms)):
#        if (log ==False):
#            histograms[i].SetMaximum(hmax_global*1.5)
#            histograms[i].SetMinimum(0)
#        if (log ==True):
#            histograms[i].SetMaximum(hmax_global*10e1)
#            histograms[i].SetMinimum(hmin_global*10e-2)
#        histograms[i].SetLineWidth(2)
#        histograms[i].SetMarkerSize(0.4)
#        histograms[i].GetXaxis().SetTitle("R [mm]")
#        histograms[i].GetYaxis().SetTitle("Entries")
#        histograms[i].GetXaxis().SetNdivisions(6)
#        histograms[i].Draw("same e")
#        legend.AddEntry(histograms[i], legends[i], "L")
#
#    ## draw legend
#    legend.SetBorderSize(0)
#    legend.SetTextSize(0.025)
#    legend.Draw("SAME")
#
#    c.Print("output/truth_signal_%s_acceptance.pdf" % parameters[p])
#
#
#
##=============================================
## Truth Z' time shift
##=============================================
#
#parameters = ['barrel', 'endcap']
#parameters_label = [
#                    '#Deltat [ns]',
#                    '#Deltat [ns]',
#                   ]
#
#
#for p in range(len(parameters)):
#
#    # define legends
#    legends = [
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 500 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 500 mm',
#                ]
#
#    # initialize legend
#    legend = TLegend(0.35,0.80,0.82,0.93)
# 
#    # initialize canvas
#    c= TCanvas("canvas_truth_zp_t_%s" % parameters[p], "", 600, 600)
#
#    # set log
#    log = True
#
#    if (log == True):
#        c.SetLogy()
#
#    # read histograms
#    histograms = []
#    #histograms.append(f_m250t500.Get('Truth/signal_zp/t/m_zp_t_%s' % parameters[p] ))
#    #histograms.append(f_m1000t500.Get('Truth/signal_zp/t/m_zp_t_%s' % parameters[p] ))
#    histograms.append(f_m500t100.Get('Truth/signal_zp/t/m_zp_t_%s' % parameters[p] ))
#    histograms.append(f_m500t500.Get('Truth/signal_zp/t/m_zp_t_%s' % parameters[p] ))
#
#    # set histogram color 
#    histograms[0].SetLineColor(kBlack)
#    histograms[1].SetLineColor(kBlack)
#
#    histograms[0].SetLineStyle(1)
#    histograms[1].SetLineStyle(2)
#
#    #histograms[0].SetMarkerColor(kBlack)
#    #histograms[1].SetMarkerColor(kBlack)
#
#
#    # set fill color
#
#    # finding global maximum
#    hmax_global = 0
#    hmin_global = 10e10
#
#    for i in range(len(histograms)):
#        histograms[i].Sumw2()
#        # scale
#        if (histograms[i].Integral() > 0):
#            histograms[i].Scale( 1. / histograms[i].Integral())
#
#        hmax = getMaximum(histograms[i])
#        if (hmax > hmax_global):
#            hmax_global = hmax
#        hmin = getMinimumNonzero(histograms[i])
#        if (hmin < hmin_global):
#            hmin_global = hmin
# 
#    for i in range(len(histograms)):
#        if (log ==False):
#            histograms[i].SetMaximum(hmax_global*1.5)
#            histograms[i].SetMinimum(0)
#        if (log ==True):
#            histograms[i].SetMaximum(hmax_global*10e1)
#            histograms[i].SetMinimum(hmin_global*10e-2)
#        histograms[i].SetLineWidth(2)
#        histograms[i].SetMarkerSize(0.4)
#        histograms[i].GetXaxis().SetTitle(parameters_label[p])
#        histograms[i].GetYaxis().SetTitle("Entries")
#        histograms[i].GetXaxis().SetNdivisions(6)
#        histograms[i].Draw("same hist")
#        legend.AddEntry(histograms[i], legends[i], "L")
#
#        print str(p) + "," + parameters[p] + ": intgral = " + str(histograms[i].Integral(0,10))
#
#    ## draw legend
#    legend.SetBorderSize(0)
#    legend.SetTextSize(0.025)
#    legend.Draw("SAME")
#
#    c.Print("output/truth_signal_zp_t_%s.pdf" % parameters[p])


