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

f_m_mumu = TFile.Open("/Users/sche/Documents/plots/analysis/root/main_analysis/main.mumu.root", 'READ') 
f_m_ee = TFile.Open("/Users/sche/Documents/plots/analysis/root/main_analysis/main.ee.root", 'READ') 
f_m_emu = TFile.Open("/Users/sche/Documents/plots/analysis/root/main_analysis/main.emu.root", 'READ') 

#=============================================
# define colors
#=============================================
color_mumu = kRed-3
color_ee = kOrange+1
color_emu= kAzure-5

#=============================================
# Event cutflow
#=============================================

#if False:
if True:

    # define legends
    legends = [
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrowee, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrowe#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                ]

    # initialize legend
    legend = TLegend(0.2,0.80,0.50,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_event_cutflow", "", 800, 600)

    # Upper plot will be in pad1
    pad1 = TPad("pad1", "pad1", 0, 0, 1, 1.0)
    pad1.SetBottomMargin(0.20) 
    pad1.SetLeftMargin(0.15) 
    pad1.SetRightMargin(0.15) 
    pad1.Draw()             # Draw the upper pad: pad1
    pad1.cd()               # pad1 becomes the current pad

    # set log
    log = True

    if (log == True):
        pad1.SetLogy()


    # read histograms
    histograms = []

    histograms.append(f_m_mumu.Get('m_event_cutflow'))
    histograms.append(f_m_ee.Get('m_event_cutflow'))
    histograms.append(f_m_emu.Get('m_event_cutflow'))

    # set histogram color 
    histograms[0].SetLineColor(color_mumu)
    histograms[1].SetLineColor(color_ee)
    histograms[2].SetLineColor(color_emu)

    # set fill color
    #histograms[0].SetFillColor(color_mumu) 
    #histograms[1].SetFillColor(color_ee)
    #histograms[2].SetFillColor(color_emu)

    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        # scale
        #if ((histograms[i].Integral() > 0) and (i>1)):
        #    histograms[i].Scale( 78. / histograms[i].Integral())

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
            histograms[i].SetMaximum(hmax_global*10e1)
            histograms[i].SetMinimum(hmin_global*10e-2)
        histograms[i].GetYaxis().SetTitle("Events")
        #histograms[i].Draw("HIST TEXT0 SAME")
        legend.AddEntry(histograms[i], legends[i], "L")

    # custom drawing order
    histograms[1].Draw("HIST TEXT0 SAME")
    histograms[0].Draw("HIST TEXT0 SAME")
    histograms[2].Draw("HIST TEXT0 SAME")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/event_cutflow.pdf")


#=============================================
# DV cutflow
#=============================================

#if False:
if True:

    # define legends
    legends = [
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrowee, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrowe#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                ]

    # initialize legend
    #legend = TLegend(0.27,0.73,0.88,0.93)
    legend = TLegend(0.30,0.80,0.70,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_dv_cutflow", "", 800, 600)

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

    histograms.append(f_m_mumu.Get('main_analysis/dv_mumu/m_dv_mumu_cf'))
    histograms.append(f_m_ee.Get('main_analysis/dv_ee/m_dv_ee_cf'))
    histograms.append(f_m_emu.Get('main_analysis/dv_emu/m_dv_emu_cf'))

    # set histogram color 
    histograms[0].SetLineColor(color_mumu)
    histograms[1].SetLineColor(color_ee)
    histograms[2].SetLineColor(color_emu)

    # set fill color
    #histograms[0].SetFillColor(color_mumu)
    #histograms[1].SetFillColor(color_ee)
    #histograms[2].SetFillColor(color_emu)

    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        # scale
        #if ((histograms[i].Integral() > 0) and (i>1)):
        #    histograms[i].Scale( 78. / histograms[i].Integral())

        hmax = getMaximum(histograms[i])
        if (hmax > hmax_global):
            hmax_global = hmax
        hmin = getMinimumNonzero(histograms[i])
        if (hmin < hmin_global):
            hmin_global = hmin

 
    for i in range(len(histograms)):
        if (log == True):
            histograms[i].SetMaximum(hmax_global*10e3)
            histograms[i].SetMinimum(hmin_global*10e-2)
        if (log == False):
            histograms[i].SetMaximum(hmax_global*1.5)
            histograms[i].SetMinimum(0)
        histograms[i].GetXaxis().SetLabelSize(0.040)
        #histograms[i].GetXaxis().SetBinLabel(1,"#mu#mu,ee,e#mu")
        histograms[i].SetAxisRange(0,10,"x");
        histograms[i].GetYaxis().SetTitle("Vertex")
        legend.AddEntry(histograms[i], legends[i], "L")

    # custom drawing order
    histograms[0].Draw("HIST TEXT0 SAME")
    histograms[1].Draw("HIST TEXT0 SAME")
    histograms[2].Draw("HIST TEXT0 SAME")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/dv_cutflow.pdf")



#=============================================
# DV distribution
#=============================================

parameters = ['M', 'R', 'R_low', 'z', 'l']
file_name = ['M', 'tangential_R', 'tangential_R_low', 'z', 'radial_l']
parameters_label = ['m_{DV} [GeV]',
                    'r_{DV} [mm]',
                    'r_{DV} [mm]',
                    'z_{DV} [mm]',
                    'l_{DV} [mm]']

for p in range(len(parameters)):
        

    # define legends
    legends = [
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrowee, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrowe#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                ]

    # initialize legend
    legend = TLegend(0.35,0.75,0.80,0.93)
 
    # initialize canvas
    c= TCanvas("dv_dimuon_%s" % parameters[p], "", 800, 600)

    # set log
    log = False

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_m_mumu.Get('main_analysis/dv_mumu/dv_mumu_%s' % parameters[p] ))
    histograms.append(f_m_ee.Get('main_analysis/dv_ee/dv_ee_%s' % parameters[p] ))
    histograms.append(f_m_emu.Get('main_analysis/dv_emu/dv_emu_%s' % parameters[p] ))

    # set histogram color 
    histograms[0].SetLineColor(color_mumu)
    histograms[1].SetLineColor(color_ee) 
    histograms[2].SetLineColor(color_emu) 

    # set fill color
    histograms[0].SetFillColor(color_mumu)
    histograms[1].SetFillColor(color_ee) 
    histograms[2].SetFillColor(color_emu) 

    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        # scale
        #if ((histograms[i].Integral() > 0) and (i>1)):
        #    histograms[i].Scale( 78. / histograms[i].Integral())

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
        histograms[i].GetXaxis().SetTitle(parameters_label[p])
        histograms[i].GetYaxis().SetTitle("Entries")
        histograms[i].GetXaxis().SetNdivisions(8)
        histograms[i].SetLineWidth(3)
        #histograms[i].Draw("hist same F text0")
        #legend.AddEntry(histograms[i], legends[i], "F")
        if (i<2):
            legend.AddEntry(histograms[i], legends[i], "F")
        if (i>1):
            legend.AddEntry(histograms[i], legends[i], "L")

    # custom drawing
    histograms[2].Draw("hist same ")
    histograms[1].Draw("hist same ")
    histograms[0].Draw("hist same ")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/dv_dimuon_%s.pdf" % file_name[p])


#=============================================
# DV chi2 / nDOF
#=============================================


if True:
        

    # define legends
    legends = [
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrowee, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrowe#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                ]

    # initialize legend
    legend = TLegend(0.30,0.72,0.90,0.92)
 
    # initialize canvas
    c= TCanvas("dv_chi2_ndof_%s" % parameters[p], "", 800, 600)

    # set log
    log = False

    if (log == True):
        c.SetLogy()


    # read histograms
    histograms = []

    #histograms.append(f_m_mumu.Get('dv_mumu/chi2_ndof_nocosmic'))
    histograms.append(f_m_mumu.Get('main_analysis/dv_mumu/dv_mumu_chi2_ndof'))
    histograms.append(f_m_ee.Get('main_analysis/dv_ee/dv_ee_chi2_ndof'))
    histograms.append(f_m_emu.Get('main_analysis/dv_emu/dv_emu_chi2_ndof'))

    # set histogram color 
    histograms[0].SetLineColor(color_mumu)
    histograms[1].SetLineColor(color_ee)
    histograms[2].SetLineColor(color_emu)

    # set fill color
    #histograms[0].SetFillColor(color_mumu)
    #histograms[1].SetFillColorAlpha(color_ee ,0.3)
    #histograms[3].SetFillColorAlpha(color_emu,0.3)

    hmax_global = 0

    for i in range(len(histograms)):
        # scale
        #if ((histograms[i].Integral() > 0) and (i>0)):
        #    histograms[i].Scale( 87. / histograms[i].Integral())

        hmax = getMaximum(histograms[i])
        if (hmax > hmax_global):
            hmax_global = hmax
        hmin = getMinimumNonzero(histograms[i])
        if (hmin < hmin_global):
            hmin_global = hmin

 
    for i in range(len(histograms)):
        if (log == True):
            histograms[i].SetMaximum(10e3)
            histograms[i].SetMinimum(1)
        if (log == False):
            histograms[i].SetMaximum(hmax_global*1.1)
            histograms[i].SetMinimum(0)
        histograms[i].GetXaxis().SetTitle("#chi^{2}_{DV} / DOF")
        histograms[i].GetYaxis().SetTitle("Entries")
        histograms[i].GetXaxis().SetNdivisions(8)
        #histograms[i].Draw("same hist")
        legend.AddEntry(histograms[i], legends[i], "F")

    # custom drawing order
    histograms[2].Draw("HIST SAME")
    histograms[1].Draw("HIST SAME")
    histograms[0].Draw("HIST SAME")


    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/dv_chi2_nDOF.pdf")


#=============================================
# muon distribution
#=============================================

parameters = ['pt', 'pt_low', 'eta', 'phi', 'd0', 'z0']
parameters_label = [
                    'p_{T} [GeV]',
                    'p_{T} [GeV]',
                    'p_{T} [GeV]',
                    'p_{T} [GeV]',
                    'p_{T} [GeV]',
                    'p_{T} [GeV]',
                    '#eta',
                    '#phi',
                    'd0 [mm]',
                    'z0 [mm]'
                   ]

for p in range(len(parameters)):

    # define legends
    legends = [
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrowe#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                ]

    # initialize legend
    legend = TLegend(0.30,0.75,0.75,0.93)
 
    # initialize canvas
    c= TCanvas("signal_muon_%s" % parameters[p], "", 800, 600)

    # set log
    log = False

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_m_mumu.Get('main_analysis/dv_mumu/outgoing/dv_mumu_mu_%s' % parameters[p] ))
    histograms.append(f_m_emu.Get('main_analysis/dv_emu/outgoing/dv_emu_mu_%s' % parameters[p] ))

    # set histogram color 
    histograms[0].SetLineColor(color_mumu)
    histograms[1].SetLineColor(color_emu) 

    # set fill color
    #histograms[0].SetFillColor(color_mumu)
    #histograms[3].SetFillColor(color_ee) 
    #histograms[4].SetFillColor(color_m250t500) 
    #histograms[5].SetFillColor(color_emu) 
    #histograms[6].SetFillColor(color_m1000t500) 

    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        # scale
        #if ((histograms[i].Integral() > 0) and (i>1)):
        #    histograms[i].Scale( 87. / histograms[i].Integral())

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
            histograms[i].SetMinimum(hmin_global*10e-1)
        histograms[i].SetLineWidth(3)
        histograms[i].GetXaxis().SetTitle(parameters_label[p])
        histograms[i].GetYaxis().SetTitle("Entries")
        histograms[i].GetXaxis().SetNdivisions(6)
        if (i<2):
            legend.AddEntry(histograms[i], legends[i], "F")
        if (i>1):
            legend.AddEntry(histograms[i], legends[i], "L")

    # custom drawing
    histograms[0].Draw("hist same")
    histograms[1].Draw("hist same")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/signal_muon_%s.pdf" % parameters[p])


#=============================================
# electron distribution
#=============================================

parameters = ['pt', 'pt_low','eta', 'phi', 'd0', 'z0']
parameters_label = [
                    'p_{T} [GeV]',
                    'p_{T} [GeV]',
                    'p_{T} [GeV]',
                    'p_{T} [GeV]',
                    'p_{T} [GeV]',
                    'p_{T} [GeV]',
                    '#eta',
                    '#phi',
                    'd0 [mm]',
                    'z0 [mm]'
                   ]

for p in range(len(parameters)):

    # define legends
    legends = [
                'MC Z\'#rightarrowee, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                'MC Z\'#rightarrowe#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                ]

    # initialize legend
    legend = TLegend(0.30,0.75,0.75,0.93)
 
    # initialize canvas
    c= TCanvas("signal_muon_%s" % parameters[p], "", 800, 600)

    # set log
    log = False

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_m_ee.Get('main_analysis/dv_ee/outgoing/dv_ee_e_%s' % parameters[p] ))
    histograms.append(f_m_emu.Get('main_analysis/dv_emu/outgoing/dv_emu_e_%s' % parameters[p] ))

    # set histogram color 
    histograms[0].SetLineColor(color_ee) 
    histograms[1].SetLineColor(color_emu) 

    # set fill color
    #histograms[0].SetFillColor(color_mumu)
    #histograms[3].SetFillColor(color_ee) 
    #histograms[4].SetFillColor(color_m250t500) 
    #histograms[5].SetFillColor(color_emu) 
    #histograms[6].SetFillColor(color_m1000t500) 

    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        # scale
        #if ((histograms[i].Integral() > 0) and (i>1)):
        #    histograms[i].Scale( 87. / histograms[i].Integral())

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
            histograms[i].SetMinimum(hmin_global*10e-1)
        histograms[i].SetLineWidth(3)
        histograms[i].GetXaxis().SetTitle(parameters_label[p])
        histograms[i].GetYaxis().SetTitle("Entries")
        histograms[i].GetXaxis().SetNdivisions(6)
        if (i<2):
            legend.AddEntry(histograms[i], legends[i], "F")
        if (i>1):
            legend.AddEntry(histograms[i], legends[i], "L")

    # custom drawing
    histograms[0].Draw("hist same")
    histograms[1].Draw("hist same")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/signal_electron_%s.pdf" % parameters[p])


