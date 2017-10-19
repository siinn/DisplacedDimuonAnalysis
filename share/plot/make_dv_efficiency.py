#!/usr/bin/python
import os, sys
from ROOT import *
from math import log10

# loading ATLAS style
gROOT.LoadMacro("AtlasStyle.C")
gROOT.LoadMacro("AtlasUtils.C")
SetAtlasStyle()

# set text option
gStyle.SetPaintTextFormat(".2f");

#=============================================
#   Statistic options
#
#   kFCP (=0)(default): using the Clopper-Pearson interval (recommended by PDG) sets kIsBayesian = false see also ClopperPearson
#   kFNormal (=1) : using the normal approximation sets kIsBayesian = false see also Normal
#   kFWilson (=2) : using the Wilson interval sets kIsBayesian = false see also Wilson
#   kFAC (=3) : using the Agresti-Coull interval sets kIsBayesian = false see also AgrestiCoull
#   kFFC (=4) : using the Feldman-Cousins frequentist method sets kIsBayesian = false see also FeldmanCousins
#   kBJeffrey (=5) : using the Jeffrey interval sets kIsBayesian = true, fBeta_alpha = 0.5 and fBeta_beta = 0.5 see also Bayesian
#   kBUniform (=6) : using a uniform prior sets kIsBayesian = true, fBeta_alpha = 1 and fBeta_beta = 1 see also Bayesian
#   kBBayesian (=7) : using a custom prior defined by fBeta_alpha and fBeta_beta sets kIsBayesian = true see also Bayesian
#   kMidP (=8) : using the Lancaster Mid-P method sets kIsBayesian = false
#=============================================



#=============================================
# Convenient little functions
#=============================================

def getMaximum(h):
    hmax = 0
    for i in range(h.GetPaintedGraph().GetXaxis().GetNbins()):
        hmax = max(hmax, h.GetBinContent(i + 1))
        pass
    return hmax

def getMinimumNonzero(h):
    hmin = float('Inf')
    for i in range(h.GetPaintedGraph().GetXaxis().GetNbins()):
        c = h.GetBinContent(i + 1)
        if c > 0:
            hmin = min(hmin, c)
            pass
        pass
    return hmin 

#=============================================
# input
#=============================================
f_m_mumu = TFile.Open("/Users/sche/Documents/plots/analysis/root/trigger_eff/trig.mumu.root", 'READ') 
f_m_ee = TFile.Open("/Users/sche/Documents/plots/analysis/root/trigger_eff/trig.ee.root", 'READ') 
f_m_emu = TFile.Open("/Users/sche/Documents/plots/analysis/root/trigger_eff/trig.emu.root", 'READ') 

#=============================================
# define colors
#=============================================
color_mumu = kRed-3
color_ee = kOrange+1
color_emu= kAzure-5

color_m250t100  =   kAzure-5
color_m250t250  =   kSpring-6
color_m250t500  =   kAzure-4
color_m500t100  =   kGreen+2
color_m500t250  =   kGreen-5
color_m500t500  =   kGreen-10
color_m1000t100 =   kRed+1
color_m1000t250 =   kRed-7
color_m1000t500 =   kRed-7


#=============================================
# Efficiency as a function of Z' variable
#=============================================

parameters = ['eta', 'beta', 'pt']
parameter_labels = ['#eta', '#beta', 'p_{T} [GeV]']
for p in range(len(parameters)):

    # define legends
    legends = [
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrowee, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrowe#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                ]

    # initialize legend
    #legend = TLegend(0.27,0.73,0.88,0.93)
    legend = TLegend(0.40,0.82,0.70,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_dv_eff", "", 800, 600)

    # Upper plot will be in pad1
    pad1 = TPad("pad1", "pad1", 0, 0, 1, 1.0)
    pad1.SetBottomMargin(0.20) 
    pad1.SetLeftMargin(0.15) 
    pad1.SetRightMargin(0.15) 
    pad1.Draw()             # Draw the upper pad: pad1
    pad1.cd()               # pad1 becomes the current pad

    # set log
    log = False

    if (log == True):
        pad1.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_m_mumu.Get('truth/efficiency/zp/m_dv_eff_zp_%s' % parameters[p]))
    histograms.append(f_m_ee.Get('truth/efficiency/zp/m_dv_eff_zp_%s' % parameters[p]))
    histograms.append(f_m_emu.Get('truth/efficiency/zp/m_dv_eff_zp_%s' % parameters[p]))

    # set histogram color 
    histograms[0].SetLineColor(color_mumu)
    histograms[1].SetLineColor(color_ee)
    histograms[2].SetLineColor(color_emu)

    histograms[0].SetMarkerColor(color_mumu)
    histograms[1].SetMarkerColor(color_ee)
    histograms[2].SetMarkerColor(color_emu)

    histograms[0].SetMarkerSize(1)
    histograms[1].SetMarkerSize(1)
    histograms[2].SetMarkerSize(1)

    for i in range(len(histograms)):
        histograms[i].SetLineWidth(2)
        histograms[i].SetTitle(";%s;Efficiency" % parameter_labels[p])
        histograms[i].SetStatisticOption(5)
        legend.AddEntry(histograms[i], legends[i], "L")


    # draw axis
    histograms[2].Draw("A")

    # change axis
    histograms[2].Paint("")
    histograms[2].GetPaintedGraph().SetMaximum(0.8);

    # draw efficiency plots
    histograms[2].Draw("same ")
    histograms[1].Draw("same ")
    histograms[0].Draw("same ")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/dv_eff_%s.pdf" % parameters[p])


#=============================================
# Trig efficiency
#=============================================

parameters = ['mu', 'trig']
parameter_labels = ['#mu', '']
for n, param in enumerate(parameters):

    # define legends
    legends = [
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrowee, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrowe#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                ]

    # initialize legend
    #legend = TLegend(0.27,0.73,0.88,0.93)
    legend = TLegend(0.40,0.80,0.70,0.91)
 
    # initialize canvas
    c= TCanvas("canvas_dv_eff_trig", "", 800, 600)

    # Upper plot will be in pad1
    pad1 = TPad("pad1", "pad1", 0, 0, 1, 1.0)
    pad1.SetBottomMargin(0.20) 
    pad1.SetLeftMargin(0.15) 
    pad1.SetRightMargin(0.15) 
    pad1.Draw()             # Draw the upper pad: pad1
    pad1.cd()               # pad1 becomes the current pad

    # set log
    log = False

    if (log == True):
        pad1.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_m_mumu.Get('truth/efficiency/m_dv_eff_%s' % param))
    histograms.append(f_m_ee.Get('truth/efficiency/m_dv_eff_%s' % param))
    histograms.append(f_m_emu.Get('truth/efficiency/m_dv_eff_%s' % param))

    # set histogram color 
    histograms[0].SetLineColor(color_mumu)
    histograms[1].SetLineColor(color_ee)
    histograms[2].SetLineColor(color_emu)

    histograms[0].SetMarkerColor(color_mumu)
    histograms[1].SetMarkerColor(color_ee)
    histograms[2].SetMarkerColor(color_emu)

    histograms[0].SetMarkerSize(1)
    histograms[1].SetMarkerSize(1)
    histograms[2].SetMarkerSize(1)

    for i in range(len(histograms)):
        histograms[i].SetLineWidth(2)
        histograms[i].SetTitle(";%s;Efficiency" % parameter_labels[n])
        if (param == "mu"):
            histograms[i].SetStatisticOption(5)
        if (param == "trig"):
            histograms[i].Scale(1./20000.)
            histograms[i].SetMaximum(0.7)
            histograms[i].SetMinimum(0)
        legend.AddEntry(histograms[i], legends[i], "L")


    if (param == "mu"):
        # draw axis
        histograms[2].Draw("A")

        # change axis
        histograms[2].Paint("")
        histograms[2].GetPaintedGraph().SetMaximum(0.8);

        # draw efficiency plots
        histograms[2].Draw("same ")
        histograms[1].Draw("same ")
        histograms[0].Draw("same ")

    else:
        histograms[2].Draw("hist same text0")
        histograms[1].Draw("hist same text0")
        histograms[0].Draw("hist same text0")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/dv_eff_%s.pdf" % param)

