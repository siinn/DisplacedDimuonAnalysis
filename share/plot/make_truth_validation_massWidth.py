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

f_mumu_m100t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.mumu.m100t100.root", 'READ')
f_mumu_m250t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.mumu.m250t100.root", 'READ')
f_mumu_m500t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.mumu.m500t100.root", 'READ')
f_mumu_m750t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.mumu.m750t100.root", 'READ')
f_mumu_m1000t100 = TFile.Open("/n/atlas05/userdata/sche/2.4.21.DV_Analysis/run/validation/output.mumu.m1000t100.root", 'READ')

#=============================================
# define decay channel
#=============================================

ch = 'e#mu'
#ch = 'ee'
#ch = 'mumu'


#=============================================
# define colors
#=============================================
color_m100t100  =   kGreen-5
color_m250t100  =   kAzure-5
color_m500t100  =   kBlue+1
color_m750t100  =   kRed-7
color_m1000t100 =   kRed+1

color_m100t250  =   kGreen+2
color_m100t500  =   kGreen+3
color_m250t250  =   kCyan+2
color_m250t500  =   kAzure-5
color_m500t250  =   kBlue+2
color_m500t500  =   kBlue+3
color_m750t250  =   kMagenta+2
color_m750t500  =   kRed-7
color_m1000t250 =   kRed+2
color_m1000t500 =   kRed+3

#=============================================
# Breit-Wigner function
#=============================================
#https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookHowToFit
def bw(x,par):
    arg1 = 14.0/22.0 # 2/PI
    arg2 = par[1]*par[1]*par[2]*par[2] #Gamma=par[1]  M=par[2]
    arg3 = ((x[0]*x[0]) - (par[2]*par[2]))*((x[0]*x[0]) - (par[2]*par[2]))
    arg4 = x[0]*x[0]*x[0]*x[0]*((par[1]*par[1])/(par[2]*par[2]))
    return par[0]*arg1*arg2/(arg3 + arg4)



#=============================================
# signal Z' plots
#=============================================

parameters = ['m']
parameters_label = [
                    'm_{Z\'} [GeV]',
                   ]


for p in range(len(parameters)):

    # define legends
    legends = [
                'MC Z\'#rightarrow%s, m_{Z\'} = 100 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 250 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 500 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 750 GeV, c#tau = 100 mm' % ch,
                'MC Z\'#rightarrow%s, m_{Z\'} = 1000 GeV, c#tau = 100 mm' % ch,
                ]

    # initialize legend
    legend = TLegend(0.35,0.78,0.82,0.93)
 
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
        histograms.append(f_emu_m250t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m500t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m750t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_emu_m1000t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))

    if (ch == 'ee'):
        histograms.append(f_ee_m100t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m250t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m500t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m750t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_ee_m1000t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
    if (ch == 'mumu'):
        histograms.append(f_mumu_m100t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_mumu_m250t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_mumu_m500t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_mumu_m750t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))
        histograms.append(f_mumu_m1000t100.Get('Truth/signal_zp/%s/m_zp_%s' % (parameters[p], parameters[p]) ))


    # set histogram color 
    histograms[0].SetLineColor(color_m100t100)
    histograms[1].SetLineColor(color_m250t100)
    histograms[2].SetLineColor(color_m500t100)
    histograms[3].SetLineColor(color_m750t100)
    histograms[4].SetLineColor(color_m1000t100)


    # define bw functions
    bws = []
    fits = []

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

        #--------------------------------
        # Breit-Wigner fit
        #--------------------------------
        if p == 0:
            division = histograms[i].GetNbinsX()
            m_min = histograms[i].GetBinLowEdge(1)
            m_max = histograms[i].GetBinLowEdge(division+1)
 
            bws.append(TF1("bw%s" %i, bw, m_min, m_max, 3))
            bws[i].SetParName(0,"const")
            bws[i].SetParName(1,"sigma")
            bws[i].SetParName(2,"mean")
            bws[i].SetParameter(0,1.0)
            bws[i].SetParameter(1,5)
            if (i < 2):
                bws[i].SetParameter(2,150.0)
            else:
                bws[i].SetParameter(2,750.0)
            histograms[i].Fit("bw%s" %i ,"0MRL")
            fits.append(histograms[i].GetFunction("bw%s" %i))
            fits[i].SetLineColor(kBlack);
            fits[i].SetLineWidth(1);
            #fits[i].SetLineStyle(2);
            fits[i].Draw("same");


    # draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/truth_signal_zp_massWidth%s.pdf" % parameters[p])

