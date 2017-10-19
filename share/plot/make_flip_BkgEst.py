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

f_m_mumu = TFile.Open("/n/atlas05/userdata/sche/AtlasDerivation.20.7.9.3.FlipBkgEst/run1/output/output.mumu.root", 'READ') 
f_m_ee = TFile.Open("/n/atlas05/userdata/sche/AtlasDerivation.20.7.9.3.FlipBkgEst/run1/output/output.ee.root", 'READ') 
f_m_emu = TFile.Open("/n/atlas05/userdata/sche/AtlasDerivation.20.7.9.3.FlipBkgEst/run1/output/output.emu.root", 'READ') 
f_m_ttbar = TFile.Open("/n/atlas05/userdata/sche/AtlasDerivation.20.7.9.3.FlipBkgEst/run1/output/output.ttbar.root", 'READ') 

#=============================================
# define colors
#=============================================
color_mumu = kRed-3
color_ee = kOrange+1
color_emu= kAzure-5
color_ttbar= kGreen+2


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
    c= TCanvas("canvas_dv_cutflow", "", 600, 600)

    # set log
    log = False

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_m_mumu.Get('dv_mumu/m_dv_mumu_cf'))
    histograms.append(f_m_ee.Get('dv_ee/m_dv_ee_cf'))
    histograms.append(f_m_emu.Get('dv_emu/m_dv_emu_cf'))

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
        histograms[i].GetXaxis().SetBinLabel(1,"#mu#mu,ee,e#mu")
        histograms[i].GetXaxis().SetBinLabel(5,"#mu^{+}#mu^{-},e^{+}e^{-},e^{+,-}#mu^{-,+}")
        histograms[i].SetAxisRange(0,11,"x");
        histograms[i].GetYaxis().SetTitle("N_{DV}")
        #histograms[i].Draw("HIST TEXT0 SAME")
        legend.AddEntry(histograms[i], legends[i], "F")

    # custom drawing order
    histograms[0].Draw("HIST TEXT0 SAME")
    histograms[1].Draw("HIST TEXT0 SAME")
    histograms[2].Draw("HIST TEXT0 SAME")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    #c.SetCanvasSize(800,600)
    #c.SetLeftMargin(200)
    c.SetRightMargin(100)
    c.Print("output/dv_cutflow.pdf")



#=============================================
# vertex cutflow
#=============================================

channel = ['mumu', 'ee', 'emu', 'idid']
channel_latex = ['#mu#mu', 'ee', 'e#mu', 'trk-trk']

for c in range(len(channel)):

    parameters = ['noflip', 'flip']
    
    #if False:
    for p in range(len(parameters)):
    
        # define legends
        legends = [
                    'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                    'MC Z\'#rightarrowee, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                    'MC Z\'#rightarrowe#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                    'MC t#bar{t}, scaled to 33.1 fb^{-1}'
                    ]
    
        # initialize legend
        #legend = TLegend(0.27,0.73,0.88,0.93)
        legend = TLegend(0.30,0.80,0.70,0.93)
     
        # initialize canvas
        cvs = TCanvas("canvas_%s_%s_cf" % (channel[c], parameters[p]), "", 600, 600)
    
        # set log
        log = True
    
        if (log == True):
            cvs.SetLogy()
    
        # read histograms
        histograms = []
    
        histograms.append(f_m_mumu.Get('FlipBkgEst/dv_%s/m_%s_cf_%s' %(channel[c], channel[c], parameters[p])))
        histograms.append(f_m_ee.Get('FlipBkgEst/dv_%s/m_%s_cf_%s' %(channel[c], channel[c], parameters[p])))
        histograms.append(f_m_emu.Get('FlipBkgEst/dv_%s/m_%s_cf_%s' %(channel[c], channel[c], parameters[p])))
        histograms.append(f_m_ttbar.Get('FlipBkgEst/dv_%s/m_%s_cf_%s' %(channel[c], channel[c], parameters[p])))
    
        # set histogram color 
        histograms[0].SetLineColor(color_mumu)
        histograms[1].SetLineColor(color_ee)
        histograms[2].SetLineColor(color_emu)
        histograms[3].SetLineColor(color_ttbar)
    
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
                histograms[i].SetMaximum(hmax_global*1e2)
                histograms[i].SetMinimum(hmin_global*1e-1)
                #histograms[i].SetMinimum(10e-2)
            if (log == False):
                histograms[i].SetMaximum(hmax_global*1.5)
                histograms[i].SetMinimum(0)
            histograms[i].GetXaxis().SetLabelSize(0.040)
            histograms[i].GetXaxis().SetTitle("r_{%s} [mm]" % channel_latex[c])
            histograms[i].GetYaxis().SetTitle("N_{%s}" % channel_latex[c])
            histograms[i].SetAxisRange(0,8,"x");
            #histograms[i].Draw("HIST TEXT0 SAME")
            histograms[i].GetXaxis().SetBinLabel(1,"%s" % channel_latex[c])
            histograms[i].GetXaxis().SetBinLabel(2,"#chi^{2}_{DV} / DOF < 5")
            histograms[i].GetXaxis().SetBinLabel(3,"Disp. > 2 mm")
            histograms[i].GetXaxis().SetBinLabel(4,"+/-")
            histograms[i].GetXaxis().SetBinLabel(5,"DisabledModule")
            histograms[i].GetXaxis().SetBinLabel(6,"MaterialVeto(e only)")
            histograms[i].GetXaxis().SetBinLabel(7,"R_{CR} > 0.04")
            histograms[i].GetXaxis().SetBinLabel(8,"#DeltaR > 0.5")
            histograms[i].GetXaxis().SetBinLabel(9,"Truth-matched")
            legend.AddEntry(histograms[i], legends[i], "L")
    
        # custom drawing order
        histograms[0].Draw("HIST SAME text0")
        histograms[1].Draw("HIST SAME text0")
        histograms[2].Draw("HIST SAME text0")
        histograms[3].Draw("HIST SAME text0")
    
        ## draw legend
        legend.SetBorderSize(0)
        legend.SetTextSize(0.025)
        legend.Draw("SAME")
    
        #c.SetCanvasSize(800,600)
        #c.SetLeftMargin(200)
        cvs.SetRightMargin(100)
        cvs.Print("output/flipBkg_cutflow_%s_%s.pdf" % (parameters[p], channel[c]))



#=============================================
# vertex from no filp 
#=============================================

channel = ['mumu', 'ee', 'emu', 'idid']
channel_latex = ['#mu#mu', 'ee', 'e#mu', 'trk-trk']

for c in range(len(channel)):

    parameters = ['R', 'z']
    
    #if False:
    for p in range(len(parameters)):
    
        # define legends
        legends = [
                    'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                    'MC Z\'#rightarrowee, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                    'MC Z\'#rightarrowe#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                    'MC t#bar{t}, scaled to 33.1 fb^{-1}'
                    ]
    
        # initialize legend
        #legend = TLegend(0.27,0.73,0.88,0.93)
        legend = TLegend(0.30,0.80,0.70,0.93)
     
        # initialize canvas
        cvs = TCanvas("canvas_%s_noflip" % channel[c], "", 600, 600)
    
        # set log
        log = False
    
        if (log == True):
            cvs.SetLogy()
    
        # read histograms
        histograms = []
    
        histograms.append(f_m_mumu.Get('FlipBkgEst/dv_%s/m_%s_noflip_%s' %(channel[c], channel[c], parameters[p])))
        histograms.append(f_m_ee.Get('FlipBkgEst/dv_%s/m_%s_noflip_%s' %(channel[c], channel[c], parameters[p])))
        histograms.append(f_m_emu.Get('FlipBkgEst/dv_%s/m_%s_noflip_%s' %(channel[c], channel[c], parameters[p])))
        histograms.append(f_m_ttbar.Get('FlipBkgEst/dv_%s/m_%s_noflip_%s' %(channel[c], channel[c], parameters[p])))
    
        # set histogram color 
        histograms[0].SetLineColor(color_mumu)
        histograms[1].SetLineColor(color_ee)
        histograms[2].SetLineColor(color_emu)
        histograms[3].SetLineColor(color_ttbar)
    
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
                #histograms[i].SetMinimum(hmin_global*10e-2)
                histograms[i].SetMinimum(10e-2)
            if (log == False):
                histograms[i].SetMaximum(hmax_global*1.5)
                histograms[i].SetMinimum(0)
            histograms[i].GetXaxis().SetLabelSize(0.040)
            #histograms[i].GetXaxis().SetBinLabel(1,"#mu#mu,ee,e#mu")
            #histograms[i].GetXaxis().SetBinLabel(5,"#mu^{+}#mu^{-},e^{+}e^{-},e^{+,-}#mu^{-,+}")
            histograms[i].GetXaxis().SetTitle("%s_{%s} [mm]" % (parameters[p], channel_latex[c]))
            histograms[i].GetYaxis().SetTitle("N_{%s}" % channel_latex[c])
            #histograms[i].Draw("HIST TEXT0 SAME")
            legend.AddEntry(histograms[i], legends[i], "L")
    
        # custom drawing order
        histograms[0].Draw("HIST SAME")
        histograms[1].Draw("HIST SAME")
        histograms[2].Draw("HIST SAME")
        histograms[3].Draw("HIST SAME")
    
        ## draw legend
        legend.SetBorderSize(0)
        legend.SetTextSize(0.025)
        legend.Draw("SAME")
    
        #c.SetCanvasSize(800,600)
        #c.SetLeftMargin(200)
        cvs.SetRightMargin(100)
        cvs.Print("output/flipBkg_noflip_%s_%s.pdf" % (parameters[p], channel[c]))



#=============================================
# vertex from filp 
#=============================================

channel = ['mumu', 'ee', 'emu', 'idid']
channel_latex = ['#mu#mu', 'ee', 'e#mu', 'trk-trk']

for c in range(len(channel)):

    parameters = ['R', 'z']
    
    #if False:
    for p in range(len(parameters)):
    
        # define legends
        legends = [
                    'MC Z\'#rightarrow#mu#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                    'MC Z\'#rightarrowee, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                    'MC Z\'#rightarrowe#mu, m_{Z\'} = 250 GeV, c#tau = 250 mm',
                    'MC t#bar{t}, scaled to 33.1 fb^{-1}'
                    ]
    
        # initialize legend
        #legend = TLegend(0.27,0.73,0.88,0.93)
        legend = TLegend(0.30,0.80,0.70,0.93)
     
        # initialize canvas
        cvs = TCanvas("canvas_%s_flip" % channel[c], "", 600, 600)
    
        # set log
        log = False
    
        if (log == True):
            cvs.SetLogy()
    
        # read histograms
        histograms = []
    
        histograms.append(f_m_mumu.Get('FlipBkgEst/dv_%s/m_%s_flip_%s' %(channel[c], channel[c], parameters[p])))
        histograms.append(f_m_ee.Get('FlipBkgEst/dv_%s/m_%s_flip_%s' %(channel[c], channel[c], parameters[p])))
        histograms.append(f_m_emu.Get('FlipBkgEst/dv_%s/m_%s_flip_%s' %(channel[c], channel[c], parameters[p])))
        histograms.append(f_m_ttbar.Get('FlipBkgEst/dv_%s/m_%s_flip_%s' %(channel[c], channel[c], parameters[p])))
    
        # set histogram color 
        histograms[0].SetLineColor(color_mumu)
        histograms[1].SetLineColor(color_ee)
        histograms[2].SetLineColor(color_emu)
        histograms[3].SetLineColor(color_ttbar)
    
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
                #histograms[i].SetMinimum(hmin_global*10e-2)
                histograms[i].SetMinimum(10e-2)
            if (log == False):
                histograms[i].SetMaximum(hmax_global*1.5)
                histograms[i].SetMinimum(0)
            histograms[i].GetXaxis().SetLabelSize(0.040)
            #histograms[i].GetXaxis().SetBinLabel(1,"#mu#mu,ee,e#mu")
            #histograms[i].GetXaxis().SetBinLabel(5,"#mu^{+}#mu^{-},e^{+}e^{-},e^{+,-}#mu^{-,+}")
            histograms[i].GetXaxis().SetTitle("%s_{%s} [mm]" % (parameters[p], channel_latex[c]))
            histograms[i].GetYaxis().SetTitle("N_{%s}" % channel_latex[c])
            #histograms[i].Draw("HIST TEXT0 SAME")
            legend.AddEntry(histograms[i], legends[i], "L")
    
        # custom drawing order
        histograms[0].Draw("HIST SAME")
        histograms[1].Draw("HIST SAME")
        histograms[2].Draw("HIST SAME")
        histograms[3].Draw("HIST SAME")
    
        ## draw legend
        legend.SetBorderSize(0)
        legend.SetTextSize(0.025)
        legend.Draw("SAME")
    
        #c.SetCanvasSize(800,600)
        #c.SetLeftMargin(200)
        cvs.SetRightMargin(100)
        cvs.Print("output/flipBkg_flip_%s_%s.pdf" % (parameters[p],channel[c] ))



