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
# mumu
f_m_mumu_noskim = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output_eff/output.mumu.muLoose_eLooseNoD0.root", 'READ') 

# ee
f_m_ee_noskim = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output_eff/output.ee.muLoose_eLooseNoD0.root", 'READ') 

# emu
f_m_emu_noskim = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output_eff/output.emu.muLoose_eLooseNoD0.root", 'READ') 

#=============================================
# define colors
#=============================================

color_m250t100  =   kAzure-5
color_m250t250  =   kSpring-6
color_m250t500  =   kAzure-4
color_m500t100  =   kGreen+2
color_m500t250  =   kGreen-5
color_m500t500  =   kGreen-10
color_m1000t100 =   kRed+1
color_m1000t250 =   kRed-7
color_m1000t500 =   kRed-7

#color_m250t100  =   kBlue+2
#color_m250t250  =   kBlue-7
#color_m250t500  =   kBlue-10
#color_m500t100  =   kGreen+2
#color_m500t250  =   kGreen-5
#color_m500t500  =   kGreen-10
#color_m1000t100 =   kRed+1
#color_m1000t250 =   kRed-7
#color_m1000t500 =   kRed-10


#=============================================
# Efficiency map (emu)
#=============================================

if True:
 
    # initialize canvas
    c= TCanvas("canvas_dv_eff_map", "", 800, 600)

    legend = TLegend(0.50,0.85,0.85,0.93)
    legends = [
                #'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                #'MC Z\'#rightarrowee, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrowe#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                ]

    # set log
    log = False

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_m_emu_noskim.Get('truth/efficiency/zp/m_dv_eff_map_pt_eta'))

    # set text option
    gStyle.SetPaintTextFormat(".2f");
    gStyle.SetPalette(kBird)

    histograms[0].GetXaxis().SetTitle("p_{T}")
    histograms[0].GetXaxis().SetNdivisions(6)
    histograms[0].GetYaxis().SetTitle("#eta")
    histograms[0].SetMarkerSize(1.2)
    histograms[0].Sumw2()
    histograms[0].Draw("colz text0 E")

    legend.AddEntry(histograms[0], legends[0], "")
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    #c.SetCanvasSize(650,600)
    c.SetRightMargin(400)
    c.Print("output/dv_eff_map_emu.pdf")

#=============================================
# Efficiency map (ee)
#=============================================

if True:
 
    # initialize canvas
    c= TCanvas("canvas_dv_eff_map", "", 800, 600)

    legend = TLegend(0.50,0.85,0.85,0.93)
    legends = [
                #'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrowee, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                #'MC Z\'#rightarrowe#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                ]

    # set log
    log = False

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_m_ee_noskim.Get('truth/efficiency/zp/m_dv_eff_map_pt_eta'))

    # set text option
    gStyle.SetPaintTextFormat(".2f");
    gStyle.SetPalette(kBird)

    histograms[0].GetXaxis().SetTitle("p_{T}")
    histograms[0].GetXaxis().SetNdivisions(6)
    histograms[0].GetYaxis().SetTitle("#eta")
    histograms[0].SetMarkerSize(1.2)
    histograms[0].Sumw2()
    histograms[0].Draw("colz text0 E")

    legend.AddEntry(histograms[0], legends[0], "")
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    #c.SetCanvasSize(650,600)
    c.SetRightMargin(400)
    c.Print("output/dv_eff_map_ee.pdf")

#=============================================
# Efficiency map (mumu)
#=============================================

if True:
 
    # initialize canvas
    c= TCanvas("canvas_dv_eff_map", "", 800, 600)

    legend = TLegend(0.50,0.85,0.85,0.93)
    legends = [
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                #'MC Z\'#rightarrowee, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                #'MC Z\'#rightarrowe#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                ]

    # set log
    log = False

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_m_mumu_noskim.Get('truth/efficiency/zp/m_dv_eff_map_pt_eta'))

    # set text option
    gStyle.SetPaintTextFormat(".2f");
    gStyle.SetPalette(kBird)

    histograms[0].GetXaxis().SetTitle("p_{T}")
    histograms[0].GetXaxis().SetNdivisions(6)
    histograms[0].GetYaxis().SetTitle("#eta")
    histograms[0].SetMarkerSize(1.2)
    histograms[0].Sumw2()
    histograms[0].Draw("colz text0 E")

    legend.AddEntry(histograms[0], legends[0], "")
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    #c.SetCanvasSize(650,600)
    c.SetRightMargin(400)
    c.Print("output/dv_eff_map_mumu.pdf")


