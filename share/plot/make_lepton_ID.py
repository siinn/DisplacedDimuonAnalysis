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
# mumu
f_mumu_muTight = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output_leptonID/output.mumu.muTight.root", 'READ') 
f_mumu_muMedium = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output_leptonID/output.mumu.muMedium.root", 'READ') 
f_mumu_muLoose = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output_leptonID/output.mumu.muLoose.root", 'READ') 

f_mumu_muCombined = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output_leptonID/output.mumu.muCombined.root", 'READ') 

# ee
f_ee_eTight = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output_leptonID/output.ee.eTight.root", 'READ') 
f_ee_eMedium = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output_leptonID/output.ee.eMedium.root", 'READ') 
f_ee_eLoose = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output_leptonID/output.ee.eLoose.root", 'READ') 
f_ee_eLooseNoD0 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output_leptonID/output.ee.eLooseNoD0.root", 'READ') 

# emu
f_emu_muMedium_eLoose = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output_leptonID/output.emu.muMedium_eLoose.root", 'READ') 
f_emu_muMedium_eLooseNoD0 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output_leptonID/output.emu.muMedium_eLooseNoD0.root", 'READ') 
f_emu_muLoose_eLoose = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output_leptonID/output.emu.muLoose_eLoose.root", 'READ') 
f_emu_muLoose_eLooseNoD0 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output_leptonID/output.emu.muLoose_eLooseNoD0.root", 'READ') 

#=============================================
# define colors
#=============================================

color_tight = kRed-3
color_medium = kAzure-5
color_loose = kOrange+1
color_looseNoD0 = kViolet-4

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
# mumu
#=============================================

parameters = ['mumu', 'ee', 'emu']
for p in range(len(parameters)):

    # define legends
    legends = [
                'MC Z\'#rightarrow#mu#mu, tight muon ID',
                'MC Z\'#rightarrow#mu#mu, medium muon ID',
                'MC Z\'#rightarrow#mu#mu, loose muon ID',
                'MC Z\'#rightarrow#mu#mu, combined muon (SUSY)',
                ]

    # initialize legend
    #legend = TLegend(0.27,0.73,0.88,0.93)
    legend = TLegend(0.30,0.78,0.70,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_dv_mumu_cutflow", "", 600, 600)

    # set log
    log = False

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_mumu_muTight.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))
    histograms.append(f_mumu_muMedium.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))
    histograms.append(f_mumu_muLoose.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))
    histograms.append(f_mumu_muCombined.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))

    # set histogram color 
    histograms[0].SetLineColor(color_tight)
    histograms[1].SetLineColor(color_medium)
    histograms[2].SetLineColor(color_loose)
    histograms[3].SetLineColor(color_looseNoD0)

    # set marker type
    #histograms[0].SetMarkerColor(color_tight)
    #histograms[1].SetMarkerColor(color_medium)
    #histograms[2].SetMarkerColor(color_loose)

    #histograms[0].SetMarkerStyle(22)
    #histograms[1].SetMarkerStyle(20)
    #histograms[2].SetMarkerStyle(23)

    # set fill color
    histograms[0].SetFillColor(color_tight)
    histograms[1].SetFillColor(color_medium)
    histograms[2].SetFillColor(color_loose)
    histograms[3].SetFillColor(color_looseNoD0)

    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        # scale
        #if ((histograms[i].Integral() > 0) and (i>2)):
        #    histograms[i].Scale( 87. / histograms[i].Integral())

        hmax = getMaximum(histograms[i])
        if (hmax > hmax_global):
            hmax_global = hmax
        hmin = getMinimumNonzero(histograms[i])
        if (hmin < hmin_global):
            hmin_global = hmin

 
    for i in range(len(histograms)):
        if (log ==False):
            histograms[i].SetMaximum(hmax_global*1.3)
            histograms[i].SetMinimum(hmin_global*0.5)
        if (log ==True):
            histograms[i].SetMaximum(hmax_global*10)
            histograms[i].SetMinimum(hmin_global*10e-2)
        histograms[i].GetXaxis().SetLabelSize(0.040)
        histograms[i].GetYaxis().SetNdivisions(6)
        histograms[i].GetYaxis().SetTitle("N_{DV}")
        #histograms[i].SetMarkerSize(1)
        #histograms[i].Draw("P TEXT0 SAME")
        legend.AddEntry(histograms[i], legends[i], "F")
        #print legends[i]
        #histograms[i].Print("all")

    histograms[3].Draw("HIST TEXT0 SAME")
    histograms[2].Draw("HIST TEXT0 SAME")
    histograms[1].Draw("HIST TEXT0 SAME")
    histograms[0].Draw("HIST TEXT0 SAME")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    #c.SetCanvasSize(800,600)
    #c.SetLeftMargin(200)
    c.SetRightMargin(100)
    c.Print("output/dv_cutflow_mumu_muID_%s.pdf" % parameters[p])


#=============================================
# ee
#=============================================

parameters = ['mumu', 'ee', 'emu']
for p in range(len(parameters)):

    # define legends
    legends = [
                'MC Z\'#rightarrowee, tight electron ID',
                'MC Z\'#rightarrowee, medium electron ID',
                'MC Z\'#rightarrowee, loose electron ID',
                'MC Z\'#rightarrowee, loose electron ID with no d_{0} cut',
                ]

    # initialize legend
    #legend = TLegend(0.27,0.73,0.88,0.93)
    legend = TLegend(0.30,0.78,0.70,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_dv_ee_cutflow", "", 600, 600)

    # set log
    log = False

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_ee_eTight.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))
    histograms.append(f_ee_eMedium.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))
    histograms.append(f_ee_eLoose.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))
    histograms.append(f_ee_eLooseNoD0.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))

    # set histogram color 
    histograms[0].SetLineColor(color_tight)
    histograms[1].SetLineColor(color_medium)
    histograms[2].SetLineColor(color_loose)
    histograms[3].SetLineColor(color_looseNoD0)

    # set marker type
    #histograms[0].SetMarkerColor(color_tight)
    #histograms[1].SetMarkerColor(color_medium)
    #histograms[2].SetMarkerColor(color_loose)

    #histograms[0].SetMarkerStyle(22)
    #histograms[1].SetMarkerStyle(20)
    #histograms[2].SetMarkerStyle(23)

    # set fill color
    histograms[0].SetFillColor(color_tight)
    histograms[1].SetFillColor(color_medium)
    histograms[2].SetFillColor(color_loose)
    histograms[3].SetFillColor(color_looseNoD0)

    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        # scale
        #if ((histograms[i].Integral() > 0) and (i>2)):
        #    histograms[i].Scale( 87. / histograms[i].Integral())

        hmax = getMaximum(histograms[i])
        if (hmax > hmax_global):
            hmax_global = hmax
        hmin = getMinimumNonzero(histograms[i])
        if (hmin < hmin_global):
            hmin_global = hmin

 
    for i in range(len(histograms)):
        if (log ==False):
            histograms[i].SetMaximum(hmax_global*1.4)
            histograms[i].SetMinimum(hmin_global*0.5)
        if (log ==True):
            histograms[i].SetMaximum(hmax_global*10)
            histograms[i].SetMinimum(hmin_global*10e-2)
        histograms[i].GetXaxis().SetLabelSize(0.040)
        histograms[i].GetYaxis().SetNdivisions(6)
        histograms[i].GetYaxis().SetTitle("N_{DV}")
        #histograms[i].SetMarkerSize(1)
        #histograms[i].Draw("P TEXT0 SAME")
        legend.AddEntry(histograms[i], legends[i], "F")
        #print legends[i]
        #histograms[i].Print("all")

    histograms[3].Draw("HIST TEXT0 SAME")
    histograms[2].Draw("HIST TEXT0 SAME")
    histograms[1].Draw("HIST TEXT0 SAME")
    histograms[0].Draw("HIST TEXT0 SAME")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    #c.SetCanvasSize(800,600)
    #c.SetLeftMargin(200)
    c.SetRightMargin(100)
    c.Print("output/dv_cutflow_ee_eID_%s.pdf" % parameters[p])


#=============================================
# emu
#=============================================

parameters = ['mumu', 'ee', 'emu']
for p in range(len(parameters)):

    # define legends
    legends = [
                'MC Z\'#rightarrowe#mu, medium mu ID, loose elc ID' ,
                'MC Z\'#rightarrowe#mu, medium mu ID, loose elc ID with no d_{0} cut' ,
                'MC Z\'#rightarrowe#mu, loose mu ID, loose elc ID' ,
                'MC Z\'#rightarrowe#mu, loose mu ID, loose elc ID with no d_{0} cut' ,
                ]

    # initialize legend
    #legend = TLegend(0.27,0.73,0.88,0.93)
    legend = TLegend(0.23,0.78,0.65,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_dv_emu_%s_cutflow" %parameters[p], "", 600, 600)

    # set log
    log = False

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_emu_muMedium_eLoose.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))
    histograms.append(f_emu_muMedium_eLooseNoD0.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))
    histograms.append(f_emu_muLoose_eLoose.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))
    histograms.append(f_emu_muLoose_eLooseNoD0.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))

    # set histogram color 
    histograms[0].SetLineColor(color_tight)
    histograms[1].SetLineColor(color_medium)
    histograms[2].SetLineColor(color_loose)
    histograms[3].SetLineColor(color_looseNoD0)

    # set marker type
    #histograms[0].SetMarkerColor(color_tight)
    #histograms[1].SetMarkerColor(color_medium)
    #histograms[2].SetMarkerColor(color_loose)

    #histograms[0].SetMarkerStyle(22)
    #histograms[1].SetMarkerStyle(20)
    #histograms[2].SetMarkerStyle(23)

    # set fill color
    #histograms[0].SetFillColor(color_tight)
    #histograms[1].SetFillColor(color_medium)
    #histograms[2].SetFillColor(color_loose)
    #histograms[3].SetFillColor(color_looseNoD0)

    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        # scale
        #if ((histograms[i].Integral() > 0) and (i>2)):
        #    histograms[i].Scale( 87. / histograms[i].Integral())

        hmax = getMaximum(histograms[i])
        if (hmax > hmax_global):
            hmax_global = hmax
        hmin = getMinimumNonzero(histograms[i])
        if (hmin < hmin_global):
            hmin_global = hmin

 
    for i in range(len(histograms)):
        if (log ==False):
            histograms[i].SetMaximum(hmax_global*1.4)
            histograms[i].SetMinimum(hmin_global*0.5)
        if (log ==True):
            histograms[i].SetMaximum(hmax_global*10)
            histograms[i].SetMinimum(hmin_global*10e-2)
        histograms[i].GetXaxis().SetLabelSize(0.040)
        histograms[i].GetYaxis().SetNdivisions(6)
        histograms[i].GetYaxis().SetTitle("N_{DV}")
        #histograms[i].SetMarkerSize(1)
        #histograms[i].Draw("P TEXT0 SAME")
        legend.AddEntry(histograms[i], legends[i], "F")
        #print legends[i]
        #histograms[i].Print("all")

    histograms[3].Draw("HIST TEXT0 SAME")
    histograms[1].Draw("HIST TEXT0 SAME")
    histograms[2].Draw("HIST TEXT0 SAME")
    histograms[0].Draw("HIST TEXT0 SAME")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    #c.SetCanvasSize(800,600)
    #c.SetLeftMargin(200)
    c.SetRightMargin(100)
    c.Print("output/dv_cutflow_emu_lepID_%s.pdf" % parameters[p])


