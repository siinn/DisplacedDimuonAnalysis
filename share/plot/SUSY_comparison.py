#!/usr/bin/python
import os, sys
from ROOT import *
from math import log10

# loading ATLAS style
gROOT.LoadMacro("AtlasStyle.C")
gROOT.LoadMacro("AtlasUtils.C")
gROOT.LoadMacro("AtlasLabels.C")
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
f_exot_m500t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/SUSYcomparison/output.EXOT.m500t100.root", 'READ') 
f_exot_m500t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/SUSYcomparison/output.EXOT.m500t500.root", 'READ') 
f_susy_m500t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/SUSYcomparison/output.SUSY.m500t100.root", 'READ') 
f_susy_m500t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/SUSYcomparison/output.SUSY.m500t500.root", 'READ') 

#=============================================
# define colors
#=============================================

color_exot_m500t100 =   kRed-4
color_exot_m500t500 =   kRed-9

color_susy_m500t100 =   kAzure-7
color_susy_m500t500 =   kAzure-9

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
# Event cutflow (EXOT)
#=============================================

#if False:
if True:

    # define legends
    legends = [
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 100 mm (EXOT)',
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 500 mm (EXOT)',
                ]

    # initialize legend
    legend = TLegend(0.40,0.83,0.70,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_exot_event_cutflow", "", 800, 600)
    #c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_exot_m500t100.Get('m_event_cutflow'))
    histograms.append(f_exot_m500t500.Get('m_event_cutflow'))

    # set histogram color 
    histograms[0].SetLineColor(kBlack)
    histograms[1].SetLineColor(kBlack)

    # set line style
    histograms[1].SetLineStyle(2)

    # set fill color
    #histograms[0].SetFillColor(color_data_pt50) 
    #histograms[1].SetFillColor(color_m250t100)
    #histograms[2].SetFillColor(color_m1000t100)
 
    for i in range(len(histograms)):
        #histograms[i].SetMaximum(10e5)
        #histograms[i].SetMinimum(10e0)
        histograms[i].SetMaximum(3e4)
        histograms[i].SetMinimum(1e3)
        histograms[i].GetYaxis().SetTitle("Events")
        #histograms[i].Draw("HIST TEXT0 SAME")
        legend.AddEntry(histograms[i], legends[i], "L")

    # custom drawing order
    histograms[0].Draw("HIST TEXT0 SAME")
    histograms[1].Draw("HIST TEXT0 SAME")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/SUSYcomp_EXOT_event_cutflow.pdf")

#=============================================
# Event cutflow (SUSY)
#=============================================

#if False:
if True:

    # define legends
    legends = [
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 100 mm (SUSY)',
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 500 mm (SUSY)',
                ]

    # initialize legend
    legend = TLegend(0.40,0.83,0.70,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_event_cutflow", "", 800, 600)
    #c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_susy_m500t100.Get('m_event_cutflow'))
    histograms.append(f_susy_m500t500.Get('m_event_cutflow'))

    # set histogram color 
    histograms[0].SetLineColor(kBlack)
    histograms[1].SetLineColor(kBlack)

    # set line style
    histograms[1].SetLineStyle(2)

    # set fill color
    #histograms[0].SetFillColor(color_data_pt50) 
    #histograms[1].SetFillColor(color_m250t100)
    #histograms[2].SetFillColor(color_m1000t100)
 
    for i in range(len(histograms)):
        #histograms[i].SetMaximum(10e5)
        #histograms[i].SetMinimum(10e0)
        histograms[i].SetMaximum(3e4)
        histograms[i].SetMinimum(1e3)
        histograms[i].GetYaxis().SetTitle("Events")
        #histograms[i].Draw("HIST TEXT0 SAME")
        legend.AddEntry(histograms[i], legends[i], "L")

    # custom drawing order
    histograms[0].Draw("HIST TEXT0 SAME")
    histograms[1].Draw("HIST TEXT0 SAME")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/SUSYcomp_SUSY_event_cutflow.pdf")
#=============================================
# DV cutflow (EXOT)
#=============================================

#if False:
if True:

    # define legends
    legends = [
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 100 mm (EXOT)',
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 500 mm (EXOT)',
                ]

    # initialize legend
    #legend = TLegend(0.27,0.73,0.88,0.93)
    legend = TLegend(0.40,0.83,0.70,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_dv_cutflow_exot", "", 800, 600)
    #c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_exot_m500t100.Get('SecondaryVertex/m_dv_cutflow'))
    histograms.append(f_exot_m500t500.Get('SecondaryVertex/m_dv_cutflow'))

    # set histogram color 
    #histograms[0].SetLineColor(color_exot_m500t100)
    #histograms[1].SetLineColor(color_exot_m500t500)
    histograms[0].SetLineColor(kBlack)
    histograms[1].SetLineColor(kBlack)

    # set line style
    histograms[1].SetLineStyle(2)

    # set fill color
    #histograms[0].SetFillColor(color_data)
    #histograms[0].SetFillStyle(3004)

    #histograms[1].SetFillColor(color_m250t100)
    #histograms[2].SetFillColor(color_m1000t100)
 
    for i in range(len(histograms)):
        #histograms[i].SetMaximum(10e4)
        #histograms[i].SetMinimum(10e1)
        histograms[i].SetMaximum(5e3)
        histograms[i].SetMinimum(5e2)
        histograms[i].GetXaxis().SetLabelSize(0.040)
        histograms[i].GetYaxis().SetTitle("N_{DV}")
        histograms[i].SetLineWidth(2)
        #histograms[i].Draw("HIST TEXT0 SAME")
        legend.AddEntry(histograms[i], legends[i], "L")

    # custom drawing order
    histograms[0].Draw("HIST TEXT0 SAME")
    histograms[1].Draw("HIST TEXT0 SAME")

    # ATLAS label
    #ATLASLabel(0.2,0.2,"Internal");

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    #c.SetCanvasSize(800,600)
    #c.SetLeftMargin(200)
    c.SetRightMargin(100)
    c.Print("output/SUSYcomp_EXOT_dv_cutflow.pdf")

#=============================================
# DV cutflow (SUSY)
#=============================================

#if False:
if True:

    # define legends
    legends = [
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 100 mm (SUSY)',
                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 500 GeV, c#tau = 500 mm (SUSY)',
                ]

    # initialize legend
    #legend = TLegend(0.27,0.73,0.88,0.93)
    legend = TLegend(0.40,0.83,0.70,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_dv_cutflow_susy", "", 800, 600)
    #c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_susy_m500t100.Get('SecondaryVertex/m_dv_cutflow'))
    histograms.append(f_susy_m500t500.Get('SecondaryVertex/m_dv_cutflow'))

    # set histogram color 
    #histograms[0].SetLineColor(color_exot_m500t100)
    #histograms[1].SetLineColor(color_exot_m500t500)
    histograms[0].SetLineColor(kBlack)
    histograms[1].SetLineColor(kBlack)

    # set line style
    histograms[1].SetLineStyle(2)

    # set fill color
    #histograms[0].SetFillColor(color_data)
    #histograms[0].SetFillStyle(3004)

    #histograms[1].SetFillColor(color_m250t100)
    #histograms[2].SetFillColor(color_m1000t100)
 
    for i in range(len(histograms)):
        #histograms[i].SetMaximum(10e4)
        #histograms[i].SetMinimum(10e1)
        histograms[i].SetMaximum(5e3)
        histograms[i].SetMinimum(5e2)
        histograms[i].GetXaxis().SetLabelSize(0.040)
        histograms[i].GetYaxis().SetTitle("N_{DV}")
        histograms[i].SetLineWidth(2)
        #histograms[i].Draw("HIST TEXT0 SAME")
        legend.AddEntry(histograms[i], legends[i], "L")

    # custom drawing order
    histograms[0].Draw("HIST TEXT0 SAME")
    histograms[1].Draw("HIST TEXT0 SAME")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    #c.SetCanvasSize(800,600)
    #c.SetLeftMargin(200)
    c.SetRightMargin(100)
    c.Print("output/SUSYcomp_SUSY_dv_cutflow.pdf")


##=============================================
## DV cutflow (background)
##=============================================
#
##if False:
#if True:
#
#    # define legends
#    legends = [
#                'MC W^{-}Z, scaled to 33.1 fb^{-1}',
#                'MC W^{+}Z, scaled to 33.1 fb^{-1}',
#                'MC WW, scaled to 33.1 fb^{-1}',
#                'MC ZZ, scaled to 33.1 fb^{-1}',
#                'MC t#bar{t}, scaled to 33.1 fb^{-1}',
#                ]
#
#    # initialize legend
#    legend = TLegend(0.45,0.70,0.80,0.93)
# 
#    # initialize canvas
#    c= TCanvas("canvas_dv_cutflow_background", "", 600, 600)
#    c.SetLogy()
#
#    # read histograms
#    histograms = []
#
#    histograms.append(f_lllvm.Get('SecondaryVertex/m_dv_cutflow'))
#    histograms.append(f_lllvp.Get('SecondaryVertex/m_dv_cutflow'))
#    histograms.append(f_llvv.Get('SecondaryVertex/m_dv_cutflow'))
#    histograms.append(f_llll.Get('SecondaryVertex/m_dv_cutflow'))
#    histograms.append(f_ttbar.Get('SecondaryVertex/m_dv_cutflow'))
#
#    # set histogram color 
#    histograms[0].SetLineColor(color_lllvm)
#    histograms[1].SetLineColor(color_lllvp)
#    histograms[2].SetLineColor(color_llvv)
#    histograms[3].SetLineColor(color_llll)
#    histograms[4].SetLineColor(color_ttbar)
#
#    # set fill color
#    histograms[0].SetFillColor(color_lllvm)
#    histograms[1].SetFillColor(color_lllvp)
#    histograms[2].SetFillColor(color_llvv)
#    histograms[3].SetFillColor(color_llll)
#    histograms[4].SetFillColor(color_ttbar)
#
#    # scale
#    histograms[0].Scale(2.78)
#    histograms[1].Scale(3.06)
#    histograms[2].Scale(3.28)
#    histograms[3].Scale(3.25)
#    histograms[4].Scale(4.17)
# 
#    for i in range(len(histograms)):
#        histograms[i].SetMaximum(10e6)
#        histograms[i].SetMinimum(1)
#        histograms[i].GetXaxis().SetLabelSize(0.040)
#        histograms[i].GetYaxis().SetTitle("N_{DV}")
#        #histograms[i].Draw("HIST TEXT0 SAME")
#        legend.AddEntry(histograms[i], legends[i], "F")
#
#    # custom drawing order
#    histograms[4].Draw("HIST TEXT0 SAME")
#    histograms[2].Draw("HIST TEXT0 SAME")
#    histograms[3].Draw("HIST TEXT0 SAME")
#    histograms[1].Draw("HIST TEXT0 SAME")
#    histograms[0].Draw("HIST TEXT0 SAME")
#
#    ## draw legend
#    legend.SetBorderSize(0)
#    legend.SetTextSize(0.025)
#    legend.Draw("SAME")
#
#    #c.SetCanvasSize(800,600)
#    #c.SetLeftMargin(200)
#    c.SetRightMargin(100)
#    c.Print("output/dv_cutflow_background.pdf")
#
#
##=============================================
## DV distribution
##=============================================
#
#parameters = ['M', 'R', 'R_low', 'z', 'l']
#file_name = ['M', 'tangential_R', 'tangential_R_low', 'z', 'radial_l']
#parameters_label = ['m_{DV} [GeV]',
#                    'r_{DV} [mm]',
#                    'r_{DV} [mm]',
#                    'z_{DV} [mm]',
#                    'l_{DV} [mm]']
#
#for p in range(len(parameters)):
#        
#
#    # define legends
#    legends = [ '2016 Data, 33.1 fb^{-1}',
#                '2016 Data, 33.1 fb^{-1}, no cut on #DeltaR > 0.5',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 100 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 100 mm',
#                ]
#
#    # initialize legend
#    legend = TLegend(0.35,0.75,0.80,0.93)
# 
#    # initialize canvas
#    c= TCanvas("dv_dimuon_%s" % parameters[p], "", 600, 600)
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
#    histograms.append(f_data.Get('SecondaryVertex/dv_dimuon_%s' % parameters[p] ))
#    histograms.append(f_data.Get('SecondaryVertex/pt60/dv_dimuon60_%s' % parameters[p] ))
#    histograms.append(f_m250t100.Get('SecondaryVertex/dv_dimuon_%s' % parameters[p] ))
#    histograms.append(f_m1000t100.Get('SecondaryVertex/dv_dimuon_%s' % parameters[p] ))
#
#    # set histogram color 
#    histograms[0].SetLineColor(color_data_pt0)
#    histograms[1].SetLineColor(color_data_pt60)
#    histograms[2].SetLineColor(color_m250t100) 
#    histograms[3].SetLineColor(color_m1000t100) 
#
#    # set fill color
#    histograms[0].SetFillColor(color_data_pt0)
#    histograms[1].SetFillColor(color_data_pt60)
#    #histograms[3].SetFillColor(color_m250t100) 
#    #histograms[4].SetFillColor(color_m250t500) 
#    #histograms[5].SetFillColor(color_m1000t100) 
#    #histograms[6].SetFillColor(color_m1000t500) 
#
#    # finding global maximum
#    hmax_global = 0
#    hmin_global = 10e10
#
#    for i in range(len(histograms)):
#        # scale
#        if ((histograms[i].Integral() > 0) and (i>1)):
#            histograms[i].Scale( 78. / histograms[i].Integral())
#
#        hmax = getMaximum(histograms[i])
#        if (hmax > hmax_global):
#            hmax_global = hmax
#        hmin = getMinimumNonzero(histograms[i])
#        if (hmin < hmin_global):
#            hmin_global = hmin
#
#
#    for i in range(len(histograms)):
#        if (log ==False):
#            histograms[i].SetMaximum(hmax_global*2.0)
#            histograms[i].SetMinimum(0)
#        if (log ==True):
#            histograms[i].SetMaximum(10e5)
#            histograms[i].SetMinimum(1e-1)
#        histograms[i].GetXaxis().SetTitle(parameters_label[p])
#        histograms[i].GetYaxis().SetTitle("Entries")
#        histograms[i].GetXaxis().SetNdivisions(8)
#        histograms[i].SetLineWidth(3)
#        #histograms[i].Draw("hist same F text0")
#        #legend.AddEntry(histograms[i], legends[i], "F")
#        if (i<2):
#            legend.AddEntry(histograms[i], legends[i], "F")
#        if (i>1):
#            legend.AddEntry(histograms[i], legends[i], "L")
#
#    # custom drawing
#    #histograms[1].Draw("hist same text0")
#    #histograms[0].Draw("hist same text0")
#    #histograms[2].Draw("hist same text0")
#    #histograms[3].Draw("hist same text0")
#    histograms[1].Draw("hist same ")
#    histograms[0].Draw("hist same ")
#    histograms[2].Draw("hist same ")
#    histograms[3].Draw("hist same ")
#
#    ## draw legend
#    legend.SetBorderSize(0)
#    legend.SetTextSize(0.025)
#    legend.Draw("SAME")
#
#    c.Print("output/dv_dimuon_%s.pdf" % file_name[p])
#
#
##=============================================
## DV chi2 / nDOF
##=============================================
#
#
#if True:
#        
#
#    # define legends
#    legends = ['2016 Data, 33.1 fb^{-1}, no cut on #DeltaR',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 100 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 500 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 100 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 500 mm',
#                ]
#
#    # initialize legend
#    legend = TLegend(0.30,0.72,0.90,0.92)
# 
#    # initialize canvas
#    c= TCanvas("dv_chi2_ndof_%s" % parameters[p], "", 600, 600)
#
#    # set log
#    log = False
#
#    if (log == True):
#        c.SetLogy()
#
#
#    # read histograms
#    histograms = []
#
#    #histograms.append(f_data.Get('SecondaryVertex/chi2_ndof_nocosmic'))
#    histograms.append(f_data.Get('SecondaryVertex/chi2_ndof'))
#    histograms.append(f_m250t100.Get('SecondaryVertex/chi2_ndof'))
#    histograms.append(f_m250t500.Get('SecondaryVertex/chi2_ndof'))
#    histograms.append(f_m1000t100.Get('SecondaryVertex/chi2_ndof'))
#    histograms.append(f_m1000t500.Get('SecondaryVertex/chi2_ndof'))
#
#    # set histogram color 
#    histograms[0].SetLineColor(color_data)
#    histograms[1].SetLineColor(color_m250t100)
#    histograms[2].SetLineColor(color_m250t500)
#    histograms[3].SetLineColor(color_m1000t100)
#    histograms[4].SetLineColor(color_m1000t500)
#
#    # set fill color
#    histograms[0].SetFillColor(color_data)
#    histograms[0].SetFillStyle(3004)
#    histograms[1].SetFillColorAlpha(color_m250t100 ,0.3)
#    histograms[2].SetFillColorAlpha(color_m250t500 ,0.3)
#    histograms[3].SetFillColorAlpha(color_m1000t100,0.3)
#    histograms[4].SetFillColorAlpha(color_m1000t500,0.3)
#
#    hmax_global = 0
#
#    for i in range(len(histograms)):
#        # scale
#        if ((histograms[i].Integral() > 0) and (i>0)):
#            histograms[i].Scale( 87. / histograms[i].Integral())
#
#        hmax = getMaximum(histograms[i])
#        if (hmax > hmax_global):
#            hmax_global = hmax
#        hmin = getMinimumNonzero(histograms[i])
#        if (hmin < hmin_global):
#            hmin_global = hmin
#
# 
#    for i in range(len(histograms)):
#        if (log == True):
#            histograms[i].SetMaximum(10e3)
#            histograms[i].SetMinimum(1)
#        if (log == False):
#            histograms[i].SetMaximum(hmax_global*1.1)
#            histograms[i].SetMinimum(0)
#        histograms[i].GetXaxis().SetTitle("#chi^{2}_{DV} / DOF")
#        histograms[i].GetYaxis().SetTitle("Entries")
#        histograms[i].GetXaxis().SetNdivisions(8)
#        #histograms[i].Draw("same hist")
#        legend.AddEntry(histograms[i], legends[i], "F")
#
#    # custom drawing order
#    histograms[4].Draw("HIST SAME")
#    histograms[3].Draw("HIST SAME")
#    histograms[2].Draw("HIST SAME")
#    histograms[1].Draw("HIST SAME")
#    histograms[0].Draw("HIST SAME")
#
#
#    ## draw legend
#    legend.SetBorderSize(0)
#    legend.SetTextSize(0.025)
#    legend.Draw("SAME")
#
#    c.Print("output/dv_chi2_nDOF.pdf")
#
#
##=============================================
## muon distribution
##=============================================
#
#parameters = ['pt', 'pt_low','pt_min','pt_min_low','pt_max','pt_max_low', 'eta', 'phi', 'd0', 'z0']
#parameters_label = [
#                    'p_{T} [GeV]',
#                    'p_{T} [GeV]',
#                    'p_{T} [GeV]',
#                    'p_{T} [GeV]',
#                    'p_{T} [GeV]',
#                    'p_{T} [GeV]',
#                    '#eta',
#                    '#phi',
#                    'd0 [mm]',
#                    'z0 [mm]'
#                   ]
#
#for p in range(len(parameters)):
#
#    # define legends
#    legends = [ '2016 Data, 33.1 fb^{-1}',
#                '2016 Data, 33.1 fb^{-1}, p_{T}^{#mu} > 60 GeV',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 100 mm, scaled',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 100 mm, scaled',
#                ]
#
#    # initialize legend
#    legend = TLegend(0.30,0.75,0.75,0.93)
# 
#    # initialize canvas
#    c= TCanvas("signal_muon_%s" % parameters[p], "", 600, 600)
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
#    histograms.append(f_data.Get('SecondaryVertex/muon/signal_muon_%s' % parameters[p] ))
#    histograms.append(f_data.Get('SecondaryVertex/muon/pt60/signal_muon60_%s' % parameters[p] ))
#    histograms.append(f_m250t100.Get('SecondaryVertex/muon/signal_muon_%s' % parameters[p] ))
#    histograms.append(f_m1000t100.Get('SecondaryVertex/muon/signal_muon_%s' % parameters[p] ))
#
#    # set histogram color 
#    histograms[0].SetLineColor(color_data_pt0)
#    histograms[1].SetLineColor(color_data_pt60)
#    histograms[2].SetLineColor(color_m250t100) 
#    histograms[3].SetLineColor(color_m1000t100) 
#
#    # set fill color
#    histograms[0].SetFillColor(color_data_pt0)
#    histograms[1].SetFillColor(color_data_pt60)
#    #histograms[3].SetFillColor(color_m250t100) 
#    #histograms[4].SetFillColor(color_m250t500) 
#    #histograms[5].SetFillColor(color_m1000t100) 
#    #histograms[6].SetFillColor(color_m1000t500) 
#
#    # finding global maximum
#    hmax_global = 0
#    hmin_global = 10e10
#
#    for i in range(len(histograms)):
#        # scale
#        if ((histograms[i].Integral() > 0) and (i>1)):
#            histograms[i].Scale( 87. / histograms[i].Integral())
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
#            histograms[i].SetMinimum(hmin_global*10e-1)
#        histograms[i].SetLineWidth(3)
#        histograms[i].GetXaxis().SetTitle(parameters_label[p])
#        histograms[i].GetYaxis().SetTitle("Entries")
#        histograms[i].GetXaxis().SetNdivisions(6)
#        if (i<2):
#            legend.AddEntry(histograms[i], legends[i], "F")
#        if (i>1):
#            legend.AddEntry(histograms[i], legends[i], "L")
#
#    # custom drawing
#    histograms[0].Draw("hist same")
#    histograms[1].Draw("hist same")
#    histograms[2].Draw("hist same")
#    histograms[3].Draw("hist same")
#
#    ## draw legend
#    legend.SetBorderSize(0)
#    legend.SetTextSize(0.025)
#    legend.Draw("SAME")
#
#    c.Print("output/signal_muon_%s.pdf" % parameters[p])
#
#
#
##=============================================
## cosmic veto
##=============================================
#
#parameters = ['DeltaR', 'DeltaR_low', 'Rcos', 'Rcos_low']
#parameters_label = [
#                    '#DeltaR',
#                    '#DeltaR',
#                    'R_cr',
#                    'R_cr',
#                   ]
#
#
#for p in range(len(parameters)):
#
#    # define legends
#    legends = [ '2016 Data, 33.1 fb^{-1}',
#                #'2016 Data, 33.1 fb^{-1}, p_{T}^{#mu} > 60 GeV',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 100 mm',
#                'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 1000 GeV, c#tau = 100 mm',
#                ]
#
#    # initialize legend
#    legend = TLegend(0.35,0.80,0.82,0.93)
# 
#    # initialize canvas
#    c= TCanvas("cosmic_muon_%s" % parameters[p], "", 600, 600)
#
#    # set log
#    log = False
#    if 'Delta' in parameters[p]:
#        log = False
#
#    if (log == True):
#        c.SetLogy()
#
#    # read histograms
#    histograms = []
#
#    histograms.append(f_data.Get('SecondaryVertex/cosmicVeto/signal_muon_%s' % parameters[p] ))
#    #histograms.append(f_data.Get('SecondaryVertex/cosmicVeto/pt60/signal_muon60_%s' % parameters[p] ))
#    histograms.append(f_m250t100.Get('SecondaryVertex/cosmicVeto/signal_muon_%s' % parameters[p] ))
#    histograms.append(f_m1000t100.Get('SecondaryVertex/cosmicVeto/signal_muon_%s' % parameters[p] ))
#
#    # set histogram color 
#    histograms[0].SetLineColor(color_data_pt0)
#    #histograms[1].SetLineColor(color_data_pt60)
#    histograms[1].SetLineColor(color_m250t100) 
#    histograms[2].SetLineColor(color_m1000t100) 
#
#    # set fill color
#    histograms[0].SetFillColor(color_data_pt0)
#    #histograms[1].SetFillColor(color_data_pt60)
#
#    # finding global maximum
#    hmax_global = 0
#    hmin_global = 10e10
#
#    for i in range(len(histograms)):
#        # scale
#        #if ((histograms[i].Integral() > 0) and (i>0)):
#        #    histograms[i].Scale( 87. / histograms[i].Integral())
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
#        histograms[i].GetXaxis().SetTitle(parameters_label[p])
#        histograms[i].GetYaxis().SetTitle("Entries")
#        histograms[i].GetXaxis().SetNdivisions(6)
#        if (i<1):
#            legend.AddEntry(histograms[i], legends[i], "F")
#        if (i>0):
#            legend.AddEntry(histograms[i], legends[i], "L")
#
#    # custom drawing
#    histograms[1].Draw("hist same")
#    histograms[0].Draw("hist same")
#    #histograms[1].Draw("hist same")
#    histograms[2].Draw("hist same")
#
#    ## draw legend
#    legend.SetBorderSize(0)
#    legend.SetTextSize(0.025)
#    legend.Draw("SAME")
#
#    c.Print("output/cosmic_veto_muon_%s.pdf" % parameters[p])
#
#
#
#
