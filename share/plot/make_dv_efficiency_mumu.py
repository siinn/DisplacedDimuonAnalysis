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
f_m250t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output/output.301911.root", 'READ') 
f_m250t250 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output/output.301912.root", 'READ') 
f_m250t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output/output.301913.root", 'READ') 
f_m500t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output/output.301914.root", 'READ') 
f_m500t250 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output/output.301915.root", 'READ') 
f_m500t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output/output.301916.root", 'READ') 
f_m1000t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output/output.301917.root", 'READ') 
f_m1000t250 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output/output.301918.root", 'READ') 
f_m1000t500 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/output/output.301919.root", 'READ') 

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
# DV cutflow
#=============================================

parameters = ['mumu', 'ee', 'emu']
for p in range(len(parameters)):

    # define legends
    legends = [
                'MC Z\'#rightarrow%s, m_{Z\'} = 250 GeV, c#tau = 100 mm' % parameters[p],
                #'MC Z\'#rightarrow%s, m_{Z\'} = 250 GeV, c#tau = 250 mm' % parameters[p],
                'MC Z\'#rightarrow%s, m_{Z\'} = 250 GeV, c#tau = 500 mm' % parameters[p],
                #'MC Z\'#rightarrow%s, m_{Z\'} = 500 GeV, c#tau = 100 mm' % parameters[p] ,
                #'MC Z\'#rightarrow%s, m_{Z\'} = 500 GeV, c#tau = 250 mm' % parameters[p] ,
                #'MC Z\'#rightarrow%s, m_{Z\'} = 500 GeV, c#tau = 500 mm' % parameters[p],
                'MC Z\'#rightarrow%s, m_{Z\'} = 1000 GeV, c#tau = 100 mm' % parameters[p],
                #'MC Z\'#rightarrow%s, m_{Z\'} = 1000 GeV, c#tau = 250 mm' % parameters[p],
                'MC Z\'#rightarrow%s, m_{Z\'} = 1000 GeV, c#tau = 500 mm' % parameters[p],
                ]

    # initialize legend
    #legend = TLegend(0.27,0.73,0.88,0.93)
    legend = TLegend(0.30,0.78,0.70,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_dv_cutflow", "", 600, 600)

    # set log
    log = False

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_m250t100.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))
    #histograms.append(f_m250t250.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))
    histograms.append(f_m250t500.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))
    #histograms.append(f_m500t100.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))
    #histograms.append(f_m500t250.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))
    #histograms.append(f_m500t500.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))
    histograms.append(f_m1000t100.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))
    #histograms.append(f_m1000t250.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))
    histograms.append(f_m1000t500.Get('dv_%s/m_dv_%s_cf' % (parameters[p], parameters[p])))

    # set histogram color 
    histograms[0].SetLineColor(color_m250t100)
    #histograms[1].SetLineColor(color_m250t250)
    histograms[1].SetLineColor(color_m250t500)
    #histograms[3].SetLineColor(color_m500t100)
    #histograms[4].SetLineColor(color_m500t250)
    #histograms[5].SetLineColor(color_m500t500)
    histograms[2].SetLineColor(color_m1000t100)
    #histograms[7].SetLineColor(color_m1000t250)
    histograms[3].SetLineColor(color_m1000t500)

    # set fill color
    histograms[0].SetFillColor(color_m250t100)
    #histograms[1].SetFillColor(color_m250t250)
    histograms[1].SetFillColor(color_m250t500)
    #histograms[3].SetFillColor(color_m500t100)
    #histograms[4].SetFillColor(color_m500t250)
    #histograms[5].SetFillColor(color_m500t500)
    histograms[2].SetFillColor(color_m1000t100)
    #histograms[7].SetFillColor(color_m1000t250)
    histograms[3].SetFillColor(color_m1000t500)

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
            histograms[i].SetMinimum(0)
        if (log ==True):
            histograms[i].SetMaximum(10e3)
            histograms[i].SetMinimum(1)
        histograms[i].GetXaxis().SetLabelSize(0.040)
        histograms[i].GetYaxis().SetNdivisions(6)
        histograms[i].GetYaxis().SetTitle("N_{DV}")
        #histograms[i].Draw("HIST TEXT0 SAME")
        legend.AddEntry(histograms[i], legends[i], "F")

        print legends[i]
        histograms[i].Print("all")

    histograms[2].Draw("HIST TEXT0 SAME")
    histograms[3].Draw("HIST TEXT0 SAME")
    histograms[0].Draw("HIST TEXT0 SAME")
    histograms[1].Draw("HIST TEXT0 SAME")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    #c.SetCanvasSize(800,600)
    #c.SetLeftMargin(200)
    c.SetRightMargin(100)
    c.Print("output/dv_truth_cutflow_%s.pdf" % parameters[p])


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
#    legend = TLegend(0.35,0.75,0.80,0.93)
# 
#    # initialize canvas
#    c= TCanvas("dv_dimuon_%s" % parameters[p], "", 600, 600)
#
#    # set log
#    log = True
#
#    if (log == True):
#        c.SetLogy()
#
#    # read histograms
#    histograms = []
#
#    histograms.append(f_m250t100.Get('SecondaryVertex/dv_dimuon_%s' % parameters[p]))
#    histograms.append(f_m250t250.Get('SecondaryVertex/dv_dimuon_%s' % parameters[p]))
#    histograms.append(f_m250t500.Get('SecondaryVertex/dv_dimuon_%s' % parameters[p]))
#    histograms.append(f_m500t100.Get('SecondaryVertex/dv_dimuon_%s' % parameters[p]))
#    histograms.append(f_m500t250.Get('SecondaryVertex/dv_dimuon_%s' % parameters[p]))
#    histograms.append(f_m500t500.Get('SecondaryVertex/dv_dimuon_%s' % parameters[p]))
#    histograms.append(f_m1000t100.Get('SecondaryVertex/dv_dimuon_%s' % parameters[p]))
#    histograms.append(f_m1000t250.Get('SecondaryVertex/dv_dimuon_%s' % parameters[p]))
#    histograms.append(f_m1000t500.Get('SecondaryVertex/dv_dimuon_%s' % parameters[p]))
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
#    # finding global maximum
#    hmax_global = 0
#    hmin_global = 10e10
#
#    for i in range(len(histograms)):
#        # scale
#        #if ((histograms[i].Integral() > 0) and (i>2)):
#        #    histograms[i].Scale( 87. / histograms[i].Integral())
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
#            histograms[i].SetMaximum(hmax_global*1.5)
#            histograms[i].SetMinimum(0)
#        if (log ==True):
#            histograms[i].SetMaximum(10e3)
#            histograms[i].SetMinimum(1)
#        histograms[i].GetXaxis().SetTitle(parameters_label[p])
#        histograms[i].GetYaxis().SetTitle("Entries")
#        histograms[i].GetXaxis().SetNdivisions(8)
#        histograms[i].Draw("HIST SAME")
#        legend.AddEntry(histograms[i], legends[i], "L")
#        histograms[i].SetLineWidth(3)
#
#    ## draw legend
#    legend.SetBorderSize(0)
#    legend.SetTextSize(0.025)
#    legend.Draw("SAME")
#
#    c.Print("output/dv_dimuon_%s.pdf" % file_name[p])

