#!/usr/bin/python
import os, sys
from ROOT import *
from math import log10

# loading ATLAS style
gROOT.LoadMacro("AtlasStyle.C")
gROOT.LoadMacro("AtlasUtils.C")
SetAtlasStyle()

#gStyle.SetPaintTextFormat(".2f");

#=============================================
# Convenient little functions
#=============================================

def getMaximum_F(h):
    hmax = 0
    for i in range(h.GetPaintedGraph().GetXaxis().GetNbins()):
        hmax = max(hmax, h.GetBinContent(i + 1))
        pass
    return hmax

def getMinimumNonzero_F(h):
    hmin = float('Inf')
    for i in range(h.GetPaintedGraph().GetXaxis().GetNbins()):
        c = h.GetBinContent(i + 1)
        if c > 0:
            hmin = min(hmin, c)
            pass
        pass
    return hmin 

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
f_combined = TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/truth.combined.root", 'READ') 

# all mumu samples
f_mp00 = TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/all/mp00.root", 'READ') 
f_mp01 = TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/all/mp01.root", 'READ') 
f_mp02 = TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/all/mp02.root", 'READ') 
f_mp03 = TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/all/mp03.root", 'READ') 
f_mp04 = TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/all/mp04.root", 'READ') 
f_mp05 = TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/all/mp05.root", 'READ') 
f_mp06 = TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/all/mp06.root", 'READ') 
f_mp07 = TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/all/mp07.root", 'READ') 
f_mp08 = TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/all/mp08.root", 'READ') 
f_mp09 = TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/all/mp09.root", 'READ') 
f_mp10 = TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/all/mp10.root", 'READ') 
f_mp11 = TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/all/mp11.root", 'READ') 
f_mp12 = TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/all/mp12.root", 'READ') 
f_mp13 = TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/all/mp13.root", 'READ') 
f_mp14 = TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/all/mp14.root", 'READ') 

# first half of the samples to find efficiency
m_first_half ={0: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/first_half/mp00.root", 'READ') ,
1: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/first_half/mp01.root", 'READ'),
2: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/first_half/mp02.root", 'READ'),
3: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/first_half/mp03.root", 'READ'),
4: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/first_half/mp04.root", 'READ'),
5: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/first_half/mp05.root", 'READ'),
6: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/first_half/mp06.root", 'READ'),
7: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/first_half/mp07.root", 'READ'),
8: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/first_half/mp08.root", 'READ'),
9: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/first_half/mp09.root", 'READ'),
10: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/first_half/mp10.root", 'READ'),
11: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/first_half/mp11.root", 'READ'),
12: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/first_half/mp12.root", 'READ'),
13: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/first_half/mp13.root", 'READ'),
14: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/first_half/mp14.root", 'READ')
}

# second half of the samples to apply weights
m_second_half ={0: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/second_half/mp00.root", 'READ') ,
1: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/second_half/mp01.root", 'READ'),
2: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/second_half/mp02.root", 'READ'),
3: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/second_half/mp03.root", 'READ'),
4: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/second_half/mp04.root", 'READ'),
5: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/second_half/mp05.root", 'READ'),
6: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/second_half/mp06.root", 'READ'),
7: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/second_half/mp07.root", 'READ'),
8: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/second_half/mp08.root", 'READ'),
9: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/second_half/mp09.root", 'READ'),
10: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/second_half/mp10.root", 'READ'),
11: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/second_half/mp11.root", 'READ'),
12: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/second_half/mp12.root", 'READ'),
13: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/second_half/mp13.root", 'READ'),
14: TFile.Open("/Users/sche/Documents/plots/analysis/root/efficiency/second_half/mp14.root", 'READ')
}
#=============================================
# define colors
#=============================================
color_mumu = kRed-3
color_ee = kOrange+1
color_emu= kAzure-5

#=============================================
# define legends
#=============================================
leg = [
"m = 100 GeV, c#tau = 100 mm",
"m = 100 GeV, c#tau = 250 mm",
"m = 100 GeV, c#tau = 500 mm",
"m = 250 GeV, c#tau = 100 mm",
"m = 250 GeV, c#tau = 250 mm",
"m = 250 GeV, c#tau = 500 mm",
"m = 500 GeV, c#tau = 100 mm",
"m = 500 GeV, c#tau = 250 mm",
"m = 500 GeV, c#tau = 500 mm",
"m = 750 GeV, c#tau = 100 mm",
"m = 750 GeV, c#tau = 250 mm",
"m = 750 GeV, c#tau = 500 mm",
"m = 1000 GeV, c#tau = 100 mm",
"m = 1000 GeV, c#tau = 250 mm",
"m = 1000 GeV, c#tau = 500 mm",
]

#=============================================
# eta distribution of all mumu samples
#=============================================

if False:

    # define legends
    legends = []
    for i in range(0,15):
        legends.append('MC Z\'#rightarrow#mu#mu, %s' %i)

    # initialize legend
    legend = TLegend(0.40,0.60,0.70,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_dv_zp_eta", "", 800, 600)

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

    histograms.append(f_mp00.Get('truth/signal_zp/eta/m_zp_eta'))
    histograms.append(f_mp01.Get('truth/signal_zp/eta/m_zp_eta'))
    histograms.append(f_mp02.Get('truth/signal_zp/eta/m_zp_eta'))
    histograms.append(f_mp03.Get('truth/signal_zp/eta/m_zp_eta'))
    histograms.append(f_mp04.Get('truth/signal_zp/eta/m_zp_eta'))
    histograms.append(f_mp05.Get('truth/signal_zp/eta/m_zp_eta'))
    histograms.append(f_mp06.Get('truth/signal_zp/eta/m_zp_eta'))
    histograms.append(f_mp07.Get('truth/signal_zp/eta/m_zp_eta'))
    histograms.append(f_mp08.Get('truth/signal_zp/eta/m_zp_eta'))
    histograms.append(f_mp09.Get('truth/signal_zp/eta/m_zp_eta'))
    histograms.append(f_mp10.Get('truth/signal_zp/eta/m_zp_eta'))
    histograms.append(f_mp11.Get('truth/signal_zp/eta/m_zp_eta'))
    histograms.append(f_mp12.Get('truth/signal_zp/eta/m_zp_eta'))
    histograms.append(f_mp13.Get('truth/signal_zp/eta/m_zp_eta'))
    histograms.append(f_mp14.Get('truth/signal_zp/eta/m_zp_eta'))

    # set histogram color 
    for i in range(0,15):
        histograms[i].SetLineColor(i)

    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        # scale
        #if ((histograms[i].Integral() > 0) and (i>1)):
        #    histograms[i].Scale( 1. / histograms[i].Integral())

        hmax = getMaximum(histograms[i])
        if (hmax > hmax_global):
            hmax_global = hmax
        hmin = getMinimumNonzero(histograms[i])
        if (hmin < hmin_global):
            hmin_global = hmin

    for i in range(len(histograms)):
        if (log == True):
            histograms[i].SetMaximum(hmax_global*10e2)
            histograms[i].SetMinimum(hmin_global*10e-2)
        if (log == False):
            histograms[i].SetMaximum(hmax_global*1.5)
            histograms[i].SetMinimum(0)
        histograms[i].SetLineWidth(1.5)
        histograms[i].SetTitle(";#eta;Event")
        #histograms[i].SetStatisticOption(5)
        legend.AddEntry(histograms[i], legends[i], "L")
        histograms[i].Draw("SAME")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/signal_zp_eta_all_mumu.pdf")

#=============================================
# calculate weights from combined MC
#=============================================

# define weight
weight = []

if True:
    # define legends
    legends = [
                'MC Z\'#rightarrow#mu#mu, combined',
                ]
    
    # initialize legend
    #legend = TLegend(0.27,0.73,0.88,0.93)
    legend = TLegend(0.40,0.82,0.70,0.93)
    
    # initialize canvas
    c= TCanvas("canvas_dv_zp_eta", "", 800, 600)
    
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
    
    histograms.append(f_combined.Get('truth/signal_zp/eta/m_zp_eta'))
    
    # set histogram color 
    histograms[0].SetLineColor(color_mumu)
    histograms[0].SetMarkerColor(color_mumu)
    histograms[0].SetMarkerSize(1)
    
    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10
    
    for i in range(len(histograms)):
        # scale
        #if ((histograms[i].Integral() > 0) and (i>1)):
        #    histograms[i].Scale( 1. / histograms[i].Integral())
    
        hmax = getMaximum(histograms[i])
        if (hmax > hmax_global):
            hmax_global = hmax
        hmin = getMinimumNonzero(histograms[i])
        if (hmin < hmin_global):
            hmin_global = hmin
    
    for i in range(len(histograms)):
        if (log == True):
            histograms[i].SetMaximum(hmax_global*10e2)
            histograms[i].SetMinimum(hmin_global*10e-2)
        if (log == False):
            histograms[i].SetMaximum(hmax_global*1.5)
            histograms[i].SetMinimum(0)
        histograms[i].SetLineWidth(1.5)
        histograms[i].SetTitle(";#eta;Event")
        #histograms[i].SetStatisticOption(5)
        legend.AddEntry(histograms[i], legends[i], "L")
        histograms[i].Draw()
    
    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")
    
    c.Print("output/signal_zp_eta.pdf")

#==================================================== 
# from the combined eta distribution, find weight by
# weight = 1 / xi * N_events / N_bins
#==================================================== 

# find event, bin count
#N_events = histograms[0].GetEntries()
N_events = histograms[0].Integral(1,histograms[0].GetNbinsX())
N_bins = histograms[0].GetNbinsX()

for n in range(histograms[0].GetNbinsX()):
    weight.append( 1 / histograms[0].GetBinContent(n+1) * N_events / N_bins)
    #weight.append( 1 / histograms[0].GetBinContent(n+1) )

print("N events = " + str(N_events))
print("N bins = " + str(N_bins))


#=============================================
# efficiency of first half
#=============================================

for n in range(len(m_first_half)):

    # define legends
    legends = []
    legends.append('MC Z\'#rightarrow#mu#mu, %s' % leg[n])

    # initialize legend
    legend = TLegend(0.20,0.80,0.40,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_dv_zp_first_half_eff_%s" %n, "", 800, 600)

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

    histograms.append(m_first_half[n].Get('truth/efficiency/zp/m_dv_eff_zp_eta'))

    # set histogram color 
    histograms[0].SetLineColor(kBlack)

    ## finding global maximum
    #hmax_global = 0
    #hmin_global = 10e10

    #for i in range(len(histograms)):
    #    # scale
    #    #if ((histograms[i].Integral() > 0) and (i>1)):
    #    #    histograms[i].Scale( 1. / histograms[i].Integral())

    #    hmax = getMaximum_F(histograms[i])
    #    if (hmax > hmax_global):
    #        hmax_global = hmax
    #    hmin = getMinimumNonzero_F(histograms[i])
    #    if (hmin < hmin_global):
    #        hmin_global = hmin

    for i in range(len(histograms)):
        #if (log == True):
        #    histograms[i].SetMaximum(hmax_global*10e2)
        #    histograms[i].SetMinimum(hmin_global*10e-2)
        #if (log == False):
        #    histograms[i].SetMaximum(hmax_global*1.5)
        #    histograms[i].SetMinimum(0)
        histograms[i].SetLineWidth(1.5)
        histograms[i].SetTitle(";#eta;Efficiency")
        histograms[i].SetStatisticOption(5)
        # draw axis
        histograms[i].Draw("A")
        # change axis
        histograms[i].Paint("")
        histograms[i].GetPaintedGraph().SetMaximum(0.8);
        histograms[i].Draw("same ")
        legend.AddEntry(histograms[i], legends[i], "L")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/signal_zp_eff_eta_first_%s.pdf" %n)



#=============================================
# eta distribution of first half
#=============================================

for n in range(len(m_first_half)):

    # define legends
    legends = []
    legends.append('MC Z\'#rightarrow#mu#mu, %s, Produced' %leg[n])
    legends.append('MC Z\'#rightarrow#mu#mu, %s, Reconstructed' %leg[n])
    legends.append('MC Z\'#rightarrow#mu#mu, %s, Prod #times eff' %leg[n])

    # initialize legend
    legend = TLegend(0.20,0.80,0.40,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_dv_zp_first_half_eta_%s" %n, "", 800, 600)

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
    eff = []

    histograms.append(m_first_half[n].Get('truth/signal_zp/eta/m_zp_eta'))
    histograms.append(m_first_half[n].Get('main_analysis/dv_mumu/dv_mumu_eta'))
    histograms.append(histograms[0].Clone()) 

    # efficiency to apply to truth
    eff.append(m_first_half[n].Get('truth/efficiency/zp/m_dv_eff_zp_eta'))

    # set histogram color 
    histograms[0].SetLineColor(kBlack)
    histograms[1].SetLineColor(kRed)
    histograms[2].SetLineColor(kBlue)

    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    # prod * efficiency
    for x in range(1,N_bins+1):
        # efficiency at ith bin
        ei = eff[0].GetEfficiency(x)

        # get value at xth bin
        xi = histograms[2].GetBinContent(x)

        # x * efficiency
        histograms[2].SetBinContent(x,xi*ei)

    print("(sample 1, non-reweigh) reconstructed Integral, %s = " %leg[n] + str(histograms[1].Integral(0,histograms[1].GetNbinsX())))
    print("(sample 1, non-reweigh) Prod * eff Integral 1, %s = " %leg[n] + str(histograms[2].Integral(1,histograms[2].GetNbinsX())))

    for i in range(len(histograms)):
        # scale
        #if ((histograms[i].Integral() > 0) and (i>1)):
        #    histograms[i].Scale( 1. / histograms[i].Integral())

        hmax = getMaximum(histograms[i])
        if (hmax > hmax_global):
            hmax_global = hmax
        hmin = getMinimumNonzero(histograms[i])
        if (hmin < hmin_global):
            hmin_global = hmin

    for i in range(len(histograms)):
        if (log == True):
            histograms[i].SetMaximum(hmax_global*10e2)
            histograms[i].SetMinimum(hmin_global*10e-2)
        if (log == False):
            histograms[i].SetMaximum(hmax_global*1.5)
            histograms[i].SetMinimum(0)
        histograms[i].SetLineWidth(1.5)
        histograms[i].SetTitle(";#eta;Event")
        #histograms[i].SetStatisticOption(5)
        legend.AddEntry(histograms[i], legends[i], "L")
        histograms[i].Draw("SAME")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/signal_zp_eta_first_%s.pdf" %n)


#=============================================
# eta distribution of second half
#=============================================

for n in range(len(m_first_half)):

    # define legends
    legends = []
    legends.append('MC Z\'#rightarrow#mu#mu, %s, Produced' %leg[n])
    legends.append('MC Z\'#rightarrow#mu#mu, %s, Reconstructed' %leg[n])
    legends.append('MC Z\'#rightarrow#mu#mu, %s, Prod #times eff' %leg[n])

    # initialize legend
    legend = TLegend(0.20,0.80,0.40,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_dv_zp_second_half_eta_%s" %n, "", 800, 600)

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
    eff = []

    histograms.append(m_second_half[n].Get('truth/signal_zp/eta/m_zp_eta'))
    histograms.append(m_second_half[n].Get('main_analysis/dv_mumu/dv_mumu_eta'))
    histograms.append(histograms[0].Clone()) 

    # efficiency to apply to truth
    eff.append(m_first_half[n].Get('truth/efficiency/zp/m_dv_eff_zp_eta'))

    # prod * eff
    for x in range(1,N_bins+1):
        # efficiency at ith bin
        ei = eff[0].GetEfficiency(x)

        # get value at xth bin
        xi = histograms[2].GetBinContent(x)

        # x * efficiency
        histograms[2].SetBinContent(x,xi*ei)

    print("(sample 2, non-reweigh) reconstructed Integral, %s = " %leg[n] + str(histograms[1].Integral(0,histograms[1].GetNbinsX())))
    print("(sample 2, non-reweigh) Prod * eff Integral 1, %s = " %leg[n] + str(histograms[2].Integral(1,histograms[2].GetNbinsX())))

    # set histogram color 
    histograms[0].SetLineColor(kBlack)
    histograms[1].SetLineColor(kRed)
    histograms[2].SetLineColor(kBlue)

    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        # scale
        #if ((histograms[i].Integral() > 0) and (i>1)):
        #    histograms[i].Scale( 1. / histograms[i].Integral())

        hmax = getMaximum(histograms[i])
        if (hmax > hmax_global):
            hmax_global = hmax
        hmin = getMinimumNonzero(histograms[i])
        if (hmin < hmin_global):
            hmin_global = hmin

    for i in range(len(histograms)):
        if (log == True):
            histograms[i].SetMaximum(hmax_global*10e2)
            histograms[i].SetMinimum(hmin_global*10e-2)
        if (log == False):
            histograms[i].SetMaximum(hmax_global*1.5)
            histograms[i].SetMinimum(0)
        histograms[i].SetLineWidth(1.5)
        histograms[i].SetTitle(";#eta;Event")
        #histograms[i].SetStatisticOption(5)
        legend.AddEntry(histograms[i], legends[i], "L")
        histograms[i].Draw("SAME")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/signal_zp_eta_second_%s.pdf" %n)



#=============================================
# re-weighted second half
#=============================================

for n in range(len(m_second_half)):

    # define legends
    legends = []
    legends.append('MC Z\'#rightarrow#mu#mu, %s, Produced' %leg[n])
    legends.append('MC Z\'#rightarrow#mu#mu, %s, Reconstructed' %leg[n])
    legends.append('MC Z\'#rightarrow#mu#mu, %s, Prod #times eff' %leg[n])

    # initialize legend
    legend = TLegend(0.20,0.80,0.40,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_dv_zp_second_half_eta_reweighted_%s" %n, "", 800, 600)

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
    eff = []

    histograms.append(m_second_half[n].Get('truth/signal_zp/eta/m_zp_eta'))
    histograms.append(m_second_half[n].Get('main_analysis/dv_mumu/dv_mumu_eta'))
    histograms.append(histograms[0].Clone()) 

    # efficiency to apply to truth
    eff.append(m_first_half[n].Get('truth/efficiency/zp/m_dv_eff_zp_eta'))

    print("Prod (before reweight) Integral, %s = " %leg[n] + str(histograms[0].Integral(0,histograms[0].GetNbinsX())))
    print("reconstructed (before reweight) Integral, %s = " %leg[n] + str(histograms[1].Integral(0,histograms[1].GetNbinsX())))

    for i in range(len(histograms)):
        # integral before reweighting
        integral_before = histograms[i].Integral(0,histograms[i].GetNbinsX())

        # find event, bin count
        N_events = histograms[i].GetEntries()
        N_bins = histograms[i].GetNbinsX()

        for x in range(1,N_bins+1):
            # retrieve xi and wi
            xi = histograms[i].GetBinContent(x)
            wi = weight[x-1]

            # reweight each bin
            histograms[i].SetBinContent(x,xi*wi)

        # integral after reweighting
        integral_after = histograms[i].Integral(0,histograms[i].GetNbinsX())

        #print("before reweighting Integral, %s = " %leg[n] + str(integral_before))
        #print("after reweighting Integral, %s = " %leg[n] + str(integral_after))

        # normalize
        #histograms[i].Scale(integral_before / integral_after)
        #print("normalized reweighting Integral, %s = " %leg[n] + str(histograms[i].Integral(0,histograms[i].GetNbinsX())))

    print("Prod (after reweight) Integral, %s = " %leg[n] + str(histograms[0].Integral(0,histograms[0].GetNbinsX())))
    print("reconstructed (after reweight) Integral, %s = " %leg[n] + str(histograms[1].Integral(0,histograms[1].GetNbinsX())))


    # copy of truth so that we can multiply by eff
    #print("(copied) Integral, %s = " %leg[n] + str(histograms[2].Integral(0,histograms[0].GetNbinsX())))

    integral = 0
    for x in range(1,N_bins+1):
        # efficiency at ith bin
        ei = eff[0].GetEfficiency(x)
        print ("Efficiency Up = " + str(eff[0].GetEfficiencyErrorUp(x)))
        print ("Efficiency Low = " + str(eff[0].GetEfficiencyErrorLow(x)))
        ei_error = (eff[0].GetEfficiencyErrorUp(x) + eff[0].GetEfficiencyErrorLow(x)) / 2
        print ("ei_error = " + str(ei_error))

        # get value at xth bin
        xi = histograms[2].GetBinContent(x)

        # x * efficiency
        histograms[2].SetBinContent(x,xi*ei)
        histograms[2].SetBinError(x,xi*ei_error)
        integral += xi*ei

        print("eff at %s = " %x + str(ei) + ", xi = " + str(xi) + ", bin = " + str(xi*ei) )

    print("reconstructed Integral, %s = " %leg[n] + str(histograms[1].Integral(0,histograms[1].GetNbinsX())))
    print("Prod * eff Integral 1, %s = " %leg[n] + str(histograms[2].Integral(1,histograms[2].GetNbinsX())))
    print("Prod * eff Integral 3, %s = " %leg[n] + str(integral))

    # set histogram color 
    histograms[0].SetLineColor(kBlack)
    histograms[1].SetLineColor(kRed)
    histograms[2].SetLineColor(kBlue)

    # set marker 
    histograms[2].SetMarkerSize(0.5)
    histograms[2].SetMarkerColor(kBlue)

    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        # scale
        #if ((histograms[i].Integral() > 0) and (i>1)):
        #    histograms[i].Scale( 1. / histograms[i].Integral())

        hmax = getMaximum(histograms[i])
        if (hmax > hmax_global):
            hmax_global = hmax
        hmin = getMinimumNonzero(histograms[i])
        if (hmin < hmin_global):
            hmin_global = hmin

    for i in range(len(histograms)):
        if (log == True):
            histograms[i].SetMaximum(hmax_global*10e2)
            histograms[i].SetMinimum(hmin_global*10e-2)
        if (log == False):
            histograms[i].SetMaximum(hmax_global*1.5)
            histograms[i].SetMinimum(0)
        histograms[i].SetLineWidth(1.5)
        histograms[i].SetTitle(";#eta;Event")
        #histograms[i].SetStatisticOption(5)
        legend.AddEntry(histograms[i], legends[i], "L")
        #histograms[i].Draw("SAME")
        #histograms[i].Draw("hist same e")

    histograms[0].Draw("hist same")
    histograms[1].Draw("hist same")
    histograms[2].Draw("hist same e")

    histograms[2].Print("all")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/signal_zp_eta_second_%s_reweighted.pdf" %n)


