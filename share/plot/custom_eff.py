#!/usr/bin/python
import os, sys
from ROOT import *
from math import log10

# loading ATLAS style
gROOT.LoadMacro("AtlasStyle.C")
gROOT.LoadMacro("AtlasUtils.C")
SetAtlasStyle()

gStyle.SetPaintTextFormat(".2f");

# set text option
#gStyle.SetPaintTextFormat(".0f");

#=============================================
# define colors
#=============================================

color_t100  =   kBlue-4
color_t250  =   kSpring-6
color_t500  =   kRed-7

#=============================================
# define colors
#=============================================

# total number of samples
total = 20000

# efficiency, read from make_dv_efficiency.py
ct100_eff = [(1926,250),(2202,500),(2465,1000)]
ct250_eff = [(1801,250),(2183,500),(2704,1000)]
ct500_eff = [(1353,250),(1727,500),(2235,1000)]

# fraciton of DV passing R > 2 mm, read from make_truth_plots.py
ct100_rate = [((1-0.278633)*total,250),((1-0.347632)*total,500),((1-0.435388)*total,1000)]
ct250_rate = [((1-0.157249)*total,250),((1-0.208804)*total,500),((1-0.259995)*total,1000)]
ct500_rate = [((1-0.097561)*total,250),((1-0.134034)*total,500),((1-0.175341)*total,1000)]

#=============================================
# efficiency
#=============================================

if True:

    # define legends
    legends = [
                'MC Z\'#rightarrow#mu#mu, c#tau = 100 mm',
                'MC Z\'#rightarrow#mu#mu, c#tau = 250 mm',
                'MC Z\'#rightarrow#mu#mu, c#tau = 500 mm',
                ]

    # initialize legend
    legend = TLegend(0.23,0.80,0.70,0.93)

    # initialize canvas
    c= TCanvas("canvas_dv_cutflow", "", 600, 600)

    # define histograms
    m_ct100 = TEfficiency("ct100","",1500,0,1500)
    m_ct250 = TEfficiency("ct250","",1500,0,1500)
    m_ct500 = TEfficiency("ct500","",1500,0,1500)


    m_ct100.SetMarkerColor(color_t100)
    m_ct250.SetMarkerColor(color_t250)
    m_ct500.SetMarkerColor(color_t500)

    m_ct100.SetMarkerStyle(22)
    m_ct250.SetMarkerStyle(20)
    m_ct500.SetMarkerStyle(23)

    m_ct100.SetLineColor(color_t100)
    m_ct250.SetLineColor(color_t250)
    m_ct500.SetLineColor(color_t500)

    m_ct100.SetFillColorAlpha(color_t100, 0.35)
    m_ct250.SetFillColorAlpha(color_t250, 0.35)
    m_ct500.SetFillColorAlpha(color_t500, 0.35)

    #m_ct100.SetFillStyle(3003)
    #m_ct250.SetFillStyle(3004)
    #m_ct500.SetFillStyle(3005)

    histograms = []

    histograms.append(m_ct100)
    histograms.append(m_ct250)
    histograms.append(m_ct500)

    #=================================
    # fill
    #=================================


    for pair in ct100_eff:
        for count in range(pair[0]):
            histograms[0].Fill(1,pair[1])
        for miss in range(total - pair[0]):
            histograms[0].Fill(0,pair[1])

    for pair in ct250_eff:
        for count in range(pair[0]):
            histograms[1].Fill(1,pair[1])
        for miss in range(total - pair[0]):
            histograms[1].Fill(0,pair[1])

    for pair in ct500_eff:
        for count in range(pair[0]):
            histograms[2].Fill(1,pair[1])
        for miss in range(total - pair[0]):
            histograms[2].Fill(0,pair[1])
    #=================================

    # to get underlying graph
    histograms[0].Paint("same")

    for i in range(len(histograms)):
        #histograms[i].SetMaximum(0.2)
        #histograms[i].SetMinimum(0)
        #histograms[i].Sumw2()
        histograms[i].SetConfidenceLevel(0.95)
        #histograms[i].SetStatisticOption("TEfficiency::kFNormal")
        histograms[i].SetStatisticOption(ROOT.kFNormal)
        histograms[i].SetTitle(";m_{#mu#mu} [GeV];Efficiency");
        #histograms[i].GetXaxis().SetNdivisions(8)
        #histograms[i].GetXaxis().SetTitle("m_{#mu#mu} [GeV]")
        #histograms[i].GetYaxis().SetTitle("Efficiency")
        #histograms[i].Draw("TEXT0 SAME")
        legend.AddEntry(histograms[i], legends[i], "FP")

    #create a function for fitting and do the fit
    #fit0 = TF1("f0","pol1",0,1);
    #fit0.SetParameters(0.1,0.0001);
    #fit0.SetLineWidth(3);
    #histograms[0].Fit(fit0)

    #fit1 = TF1("f1","pol1",0,10);
    #fit1.SetParameters(0.1,0.0001);
    #histograms[1].Fit(fit1)

    #fit2 = TF1("f2","pol1",0,10);
    #fit2.SetParameters(0.08,0.0001);
    #histograms[2].Fit(fit2)

    #fit0.Draw("same")

    # draw
    histograms[0].Draw("same a4 p")
    histograms[1].Draw("same 4")
    histograms[2].Draw("same 4")

    # update pad
    gPad.Update()
    graph = histograms[0].GetPaintedGraph()
    graph.SetMinimum(0)
    graph.SetMaximum(0.4)
    graph.GetXaxis().SetNdivisions(8)
    gPad.Update()




    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")


    c.Print("output/custom_efficiency.pdf")


##=============================================
## fraction of dv within 2 mm
##=============================================
#
#if True:
#
#    # define legends
#    legends = [
#                'MC Z\'#rightarrow#mu#mu, c#tau = 100 mm',
#                'MC Z\'#rightarrow#mu#mu, c#tau = 250 mm',
#                'MC Z\'#rightarrow#mu#mu, c#tau = 500 mm',
#                ]
#
#    # initialize legend
#    legend = TLegend(0.50,0.80,0.70,0.93)
#
#    # initialize canvas
#    c= TCanvas("canvas_dv_fraction", "", 600, 600)
#
#    # define histograms
#    m_ct100 = TProfile("ct100_fraction","",1500,0,1500)
#    m_ct250 = TProfile("ct250_fraction","",1500,0,1500)
#    m_ct500 = TProfile("ct500_fraction","",1500,0,1500)
#
#
#    m_ct100.SetMarkerColor(color_t100)
#    m_ct250.SetMarkerColor(color_t250)
#    m_ct500.SetMarkerColor(color_t500)
#
#    m_ct100.SetMarkerStyle(22)
#    m_ct250.SetMarkerStyle(20)
#    m_ct500.SetMarkerStyle(23)
#
#    histograms = []
#
#    histograms.append(m_ct100)
#    histograms.append(m_ct250)
#    histograms.append(m_ct500)
#
#    #=================================
#    # fill
#    #=================================
#
#    for pair in ct100_rate:
#        for count in range(pair[0]):
#            histograms[0].Fill(pair[1],1)
#        for miss in range(total - pair[0]):
#            histograms[0].Fill(pair[1],0)
#
#    for pair in ct250_rate:
#        for count in range(pair[0]):
#            histograms[1].Fill(pair[1],1)
#        for miss in range(total - pair[0]):
#            histograms[1].Fill(pair[1],0)
#
#    for pair in ct500_rate:
#        for count in range(pair[0]):
#            histograms[2].Fill(pair[1],1)
#        for miss in range(total - pair[0]):
#            histograms[2].Fill(pair[1],0)
#
#
#    for i in range(len(histograms)):
#        histograms[i].SetMaximum(1.5)
#        histograms[i].SetMinimum(0)
#        histograms[i].Sumw2()
#        histograms[i].GetXaxis().SetNdivisions(8)
#        histograms[i].GetXaxis().SetTitle("m_{#mu#mu} [GeV]")
#        histograms[i].GetYaxis().SetTitle("fraction")
#        histograms[i].Draw("TEXT0 SAME")
#        #histograms[i].Draw("SAME")
#        legend.AddEntry(histograms[i], legends[i], "P")
#
#    ## draw legend
#    legend.SetBorderSize(0)
#    legend.SetTextSize(0.025)
#    legend.Draw("SAME")
#
#    c.Print("output/custom_fraction.pdf")
#
#
##=============================================
## efficiency before apply DV cut (pseudo eff.)
##=============================================
#
#if True:
#
#    # define legends
#    legends = [
#                'MC Z\'#rightarrow#mu#mu, c#tau = 100 mm',
#                'MC Z\'#rightarrow#mu#mu, c#tau = 250 mm',
#                'MC Z\'#rightarrow#mu#mu, c#tau = 500 mm',
#                ]
#
#    # initialize legend
#    legend = TLegend(0.50,0.80,0.70,0.93)
#
#    # initialize canvas
#    c= TCanvas("canvas_dv_ep", "", 600, 600)
#
#    # define histograms
#    m_ct100 = TProfile("ct100_fraction","",1500,0,1500)
#    m_ct250 = TProfile("ct250_fraction","",1500,0,1500)
#    m_ct500 = TProfile("ct500_fraction","",1500,0,1500)
#
#
#    m_ct100.SetMarkerColor(color_t100)
#    m_ct250.SetMarkerColor(color_t250)
#    m_ct500.SetMarkerColor(color_t500)
#
#    m_ct100.SetMarkerStyle(22)
#    m_ct250.SetMarkerStyle(20)
#    m_ct500.SetMarkerStyle(23)
#
#    histograms = []
#
#    histograms.append(m_ct100)
#    histograms.append(m_ct250)
#    histograms.append(m_ct500)
#
#    #=================================
#    # fill
#    #=================================
#
#    ct100_eff_p = []
#    ct250_eff_p = []
#    ct500_eff_p = []
#
#    for i in range(len(ct100_eff)):
#        ct100_eff_p.append(( ct100_eff[i][0]/ ct100_rate[i][0] , ct100_eff[i][1] ))
#    for i in range(len(ct250_eff)):
#        ct250_eff_p.append(( ct250_eff[i][0]/ ct250_rate[i][0] , ct250_eff[i][1] ))
#    for i in range(len(ct500_eff)):
#        ct500_eff_p.append(( ct500_eff[i][0]/ ct500_rate[i][0] , ct500_eff[i][1] ))
#
#    print ct100_eff_p
#    print ct250_eff_p
#    print ct500_eff_p
#
#    for pair in ct100_eff_p:
#        for count in range(pair[0]*total):
#            histograms[0].Fill(pair[1],1)
#        for miss in range(total - pair[0]):
#            histograms[0].Fill(pair[1],0)
#
#    for pair in ct250_eff_p:
#        for count in range(pair[0]*total):
#            histograms[1].Fill(pair[1],1)
#        for miss in range(total - pair[0]):
#            histograms[1].Fill(pair[1],0)
#
#    for pair in ct500_eff_p:
#        for count in range(pair[0]*total):
#            histograms[2].Fill(pair[1],1)
#        for miss in range(total - pair[0]):
#            histograms[2].Fill(pair[1],0)
#
#
#    for i in range(len(histograms)):
#        histograms[i].SetMaximum(0.3)
#        histograms[i].SetMinimum(0)
#        histograms[i].Sumw2()
#        histograms[i].GetXaxis().SetNdivisions(8)
#        histograms[i].GetXaxis().SetTitle("m_{#mu#mu} [GeV]")
#        histograms[i].GetYaxis().SetTitle("efficiency / DV cut passing rate")
#        histograms[i].Draw("TEXT0 SAME")
#        #histograms[i].Draw("SAME")
#        legend.AddEntry(histograms[i], legends[i], "P")
#
#    ## draw legend
#    legend.SetBorderSize(0)
#    legend.SetTextSize(0.025)
#    legend.Draw("SAME")
#
#    c.Print("output/custom_eff_div_rate.pdf")
#
#

