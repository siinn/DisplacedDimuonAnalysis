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

# zp acceptance
ct100_acc_zp = [(0.6955,250),(0.6493,500),(0.6453,1000)]
ct250_acc_zp = [(0.7171,250),(0.6642,500),(0.6531,1000)]
ct500_acc_zp = [(0.7287,250),(0.6917,500),(0.6587,1000)]

# muon acceptance
ct100_acc_muon = [(0.9538,250),(0.9486,500),(0.9683,1000)]
ct250_acc_muon = [(0.9651,250),(0.9535,500),(0.9720,1000)]
ct500_acc_muon = [(0.9695,250),(0.9628,500),(0.9744,1000)]

#=============================================
# Z' acciciency
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
    c= TCanvas("canvas_zp_acceptance", "", 600, 600)

    # define histograms
    m_ct100 = TProfile("ct100","",1500,0,1500)
    m_ct250 = TProfile("ct250","",1500,0,1500)
    m_ct500 = TProfile("ct500","",1500,0,1500)


    m_ct100.SetMarkerColor(color_t100)
    m_ct250.SetMarkerColor(color_t250)
    m_ct500.SetMarkerColor(color_t500)

    m_ct100.SetMarkerStyle(22)
    m_ct250.SetMarkerStyle(20)
    m_ct500.SetMarkerStyle(23)

    m_ct100.SetLineColor(color_t100)
    m_ct250.SetLineColor(color_t250)
    m_ct500.SetLineColor(color_t500)

    #m_ct100.SetFillColorAlpha(color_t100, 0.35)
    #m_ct250.SetFillColorAlpha(color_t250, 0.35)
    #m_ct500.SetFillColorAlpha(color_t500, 0.35)

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


    for pair in ct100_acc_zp:
        for count in range(pair[0]*total):
            histograms[0].Fill(pair[1],1)
        for miss in range(total - pair[0]*total):
            histograms[0].Fill(pair[1],0)

    for pair in ct250_acc_zp:
        for count in range(pair[0]*total):
            histograms[1].Fill(pair[1],1)
        for miss in range(total - pair[0]*total):
            histograms[1].Fill(pair[1],0)

    for pair in ct500_acc_zp:
        for count in range(pair[0]*total):
            histograms[2].Fill(pair[1],1)
        for miss in range(total - pair[0]*total):
            histograms[2].Fill(pair[1],0)
    #=================================

    # to get underlying graph
    histograms[0].Paint("same")

    for i in range(len(histograms)):
        histograms[i].SetMaximum(0.8)
        histograms[i].SetMinimum(0.6)
        histograms[i].Sumw2()
        #histograms[i].SetConfidenceLevel(0.95)
        #histograms[i].SetStatisticOption("Tacciciency::kFNormal")
        #histograms[i].SetStatisticOption(ROOT.kFNormal)
        #histograms[i].SetTitle(";m_{#mu#mu} [GeV];acciciency");
        histograms[i].GetXaxis().SetNdivisions(8)
        histograms[i].GetXaxis().SetTitle("m_{#mu#mu} [GeV]")
        histograms[i].GetYaxis().SetTitle("Acceptance")
        histograms[i].Draw("TEXT0 SAME")
        legend.AddEntry(histograms[i], legends[i], "P")


    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")


    c.Print("output/custom_zp_acciciency.pdf")


#=============================================
# muon acciciency
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
    c= TCanvas("canvas_muon_acceptance", "", 600, 600)

    # define histograms
    m_ct100 = TProfile("ct100","",1500,0,1500)
    m_ct250 = TProfile("ct250","",1500,0,1500)
    m_ct500 = TProfile("ct500","",1500,0,1500)


    m_ct100.SetMarkerColor(color_t100)
    m_ct250.SetMarkerColor(color_t250)
    m_ct500.SetMarkerColor(color_t500)

    m_ct100.SetMarkerStyle(22)
    m_ct250.SetMarkerStyle(20)
    m_ct500.SetMarkerStyle(23)

    m_ct100.SetLineColor(color_t100)
    m_ct250.SetLineColor(color_t250)
    m_ct500.SetLineColor(color_t500)

    #m_ct100.SetFillColorAlpha(color_t100, 0.35)
    #m_ct250.SetFillColorAlpha(color_t250, 0.35)
    #m_ct500.SetFillColorAlpha(color_t500, 0.35)

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


    for pair in ct100_acc_muon:
        for count in range(pair[0]*total):
            histograms[0].Fill(pair[1],1)
        for miss in range(total - pair[0]*total):
            histograms[0].Fill(pair[1],0)

    for pair in ct250_acc_muon:
        for count in range(pair[0]*total):
            histograms[1].Fill(pair[1],1)
        for miss in range(total - pair[0]*total):
            histograms[1].Fill(pair[1],0)

    for pair in ct500_acc_muon:
        for count in range(pair[0]*total):
            histograms[2].Fill(pair[1],1)
        for miss in range(total - pair[0]*total):
            histograms[2].Fill(pair[1],0)
    #=================================

    # to get underlying graph
    histograms[0].Paint("same")

    for i in range(len(histograms)):
        histograms[i].SetMaximum(1.2)
        histograms[i].SetMinimum(0.8)
        histograms[i].Sumw2()
        #histograms[i].SetConfidenceLevel(0.95)
        #histograms[i].SetStatisticOption("Tacciciency::kFNormal")
        #histograms[i].SetStatisticOption(ROOT.kFNormal)
        #histograms[i].SetTitle(";m_{#mu#mu} [GeV];acciciency");
        histograms[i].GetXaxis().SetNdivisions(8)
        histograms[i].GetXaxis().SetTitle("m_{#mu#mu} [GeV]")
        histograms[i].GetYaxis().SetTitle("Acceptance")
        histograms[i].Draw("TEXT0 SAME")
        legend.AddEntry(histograms[i], legends[i], "P")


    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")


    c.Print("output/custom_muon_acciciency.pdf")


