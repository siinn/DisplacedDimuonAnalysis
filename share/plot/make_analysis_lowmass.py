#!/usr/bin/python
import os, sys
from ROOT import *
from math import log10, exp

# loading ATLAS style
gROOT.LoadMacro("AtlasStyle.C")
gROOT.LoadMacro("AtlasUtils.C")
SetAtlasStyle()

# set text option
gStyle.SetPaintTextFormat(".0f");

#=============================================
# Breit-Wigner function
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookHowToFit
def fit_bw(x,par):
    arg1 = 14.0/22.0 # 2/PI
    arg2 = par[1]*par[1]*par[2]*par[2] #Gamma=par[1]  M=par[2]
    arg3 = ((x[0]*x[0]) - (par[2]*par[2]))*((x[0]*x[0]) - (par[2]*par[2]))
    arg4 = x[0]*x[0]*x[0]*x[0]*((par[1]*par[1])/(par[2]*par[2]))
    return par[0]*arg1*arg2/(arg3 + arg4)

def fit_gaus(x,par):
    return par[0]*exp(-0.5*pow((x[0]-par[1]) / par[2],2));

# Linear background function
def fit_background(x,par):
    return par[3] + par[4]*x[0];

# only for drawing
def fit_background_draw(x,par):
    return par[0] + par[1]*x[0];

# sum of gaussian and exponential background function
def fit_combined(x,par):
    return fit_expo(x,par) + fit_gaus(x,par)

# signal gaussian - background
def fit_diff(x,par):
    return abs(fit_gaus(x,par) - fit_background(x,par))

# exponential
def fit_expo(x,par):
    return exp(par[3] + par[4]*x[0])

# exponential
def fit_expo_standalone(x,par):
    return exp(par[0] + par[1]*x[0])
#=============================================

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
f_data = TFile.Open("/Users/sche/Documents/plots/analysis/root/low_mass/lowmass.r14.root", 'READ') 


#=============================================
# define colors
#=============================================

color_data      =   kBlack
color_mumu      =   kRed-3
color_ee        =   kOrange+1
color_emu       =   kAzure-5

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
# Event cutflow
#=============================================

#if False:
if True:

    # define legends
    legends = [
                '2016 Data, 33.1 fb^{-1}',
                ]

    # initialize legend
    legend = TLegend(0.65,0.78,0.90,0.93)
 
    # initialize canvas
    c= TCanvas("canvas_event_cutflow", "", 800, 600)

    # set log
    log = True

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_data.Get('m_event_cutflow'))

    # set histogram color 
    histograms[0].SetLineColor(color_data)
    #histograms[1].SetLineColor(color_mumu)
    #histograms[2].SetLineColor(color_ee)
    #histograms[3].SetLineColor(color_emu)

    # set fill color
    #histograms[1].SetFillColor(color_mumu)
    #histograms[2].SetFillColor(color_ee)
    #histograms[3].SetFillColor(color_emu)

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
            histograms[i].SetMaximum(hmax_global*1e2)
            histograms[i].SetMinimum(hmin_global*1e-2)
        if (log == False):
            histograms[i].SetMaximum(hmax_global*1.5)
            histograms[i].SetMinimum(0)
        histograms[i].GetYaxis().SetTitle("Events")
        legend.AddEntry(histograms[i], legends[i], "L")

    # custom drawing order
    histograms[0].Draw("HIST TEXT0 SAME")
    #histograms[2].Draw("HIST TEXT0 SAME")
    #histograms[1].Draw("HIST TEXT0 SAME")
    #histograms[3].Draw("HIST TEXT0 SAME")

    # draw ATLAS label
    ATLAS_LABEL(0.20,0.87)
    myText(0.33,0.87,1,"Internal")
    #myText(0.33,0.87,1,"Internal simulation")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/data_event_cutflow_lowmass.pdf")

##=============================================
## Low mass distribution (only for mumu)
##=============================================

if True:

    # define legends
    legends = [ '2016 Data, 33.1 fb^{-1}',
                ]

    # initialize legend
    legend = TLegend(0.65,0.78,0.90,0.93)
 
    # initialize canvas
    c= TCanvas("dv_mumu_low_mass" , "", 800, 600)

    # Upper plot will be in pad1
    #pad1 = TPad("pad1", "pad1", 0, 0.30, 1, 1.0)
    #pad1.SetBottomMargin(0.02) 
    #pad1.SetLeftMargin(0.1) 
    #pad1.Draw()             # Draw the upper pad: pad1
    #pad1.cd()               # pad1 becomes the current pad

    # set log
    log = False

    if (log == True):
        #pad1.SetLogy()
        c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_data.Get('main_analysis/dv_mumu/dv_mumu_M_low'))

    # set histogram color 
    histograms[0].SetLineColor(color_data)

    # set fill color
    #histograms[0].SetFillColor(color_data)

    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10

    for i in range(len(histograms)):
        # scale
        #if ((histograms[i].Integral() > 0) and (i>1)):
        #histograms[i].Rebin(6)
        #histograms[i].GetXaxis().SetRangeUser(0,15)

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
            histograms[i].SetMaximum(10e5)
            histograms[i].SetMinimum(1e-1)
        histograms[i].GetXaxis().SetTitle("m_{#mu#mu} [GeV]")
        histograms[i].GetYaxis().SetTitle("Vertices")
        histograms[i].GetXaxis().SetNdivisions(8)
        histograms[i].SetLineWidth(2)
        #histograms[i].Draw("hist same F text0")
        #legend.AddEntry(histograms[i], legends[i], "F")
        histograms[i].Draw("hist same")
        legend.AddEntry(histograms[i], legends[i], "L")

    #--------------------------------
    # fit signal and background
    #--------------------------------
    # define fit functions
    fit_list = [] # combined fit function
    fits = [] # to draw combined fit function
    
    signal = [] # to draw signal fit function
    background = [] # to draw background fit function
    Diff = [] # to draw signal - background


    fit_list.append(TF1("combined_fit", fit_combined, 0.5, 1000, 5))

    fit_list[0].SetParameters(1.9,3.091,0.30,1,-1)
    # below parameters: chi2 = 7.60, est = 0.13
    #fit_list[0].SetParLimits(0,3.0,5.0); # gaus fit, const
    #fit_list[0].SetParLimits(1,3.0,3.1); # gaus fit, mean
    #fit_list[0].SetParLimits(2,0.1,0.20); # gaus fit, sigma
    #fit_list[0].SetParLimits(3,3.2,4.0); # expo fit, const
    #fit_list[0].SetParLimits(4,-1.2,-0.4); # expo fit, slope

    # fit for 0.5 GeV bin
    # below parameters: chi2 = 3.94, est = 0.0.00066
    fit_list[0].SetParLimits(0,2.0,3.0); # gaus fit, const
    #fit_list[0].SetParLimits(1,3.0,3.1); # gaus fit, mean
    fit_list[0].SetParLimits(2,0.1,0.25); # gaus fit, sigma
    fit_list[0].SetParLimits(3,2.0,4.0); # expo fit, const
    fit_list[0].SetParLimits(4,-1.2,-0.4); # expo fit, slope

    histograms[0].Fit("combined_fit","0MR")

    # take fitted function
    fits.append(histograms[0].GetFunction("combined_fit"))

    ## draw combined fit
    fits[0].SetLineColor(kRed)
    fits[0].SetLineWidth(2)
    fits[0].SetLineStyle(2)
    fits[0].Draw("same")

    print("Low mass background est (10,20) = " + str(fits[0].Integral(10,20)))
    print("Low mass background est (20,30)= " + str(fits[0].Integral(20,30)))
    print("Low mass background est (>10) = " + str(fits[0].Integral(10,1000)))
    print("Low mass background est (>30) = " + str(fits[0].Integral(30,1000)))
    print("chi2 = " + str(fits[0].GetChisquare()))


    #===========================================
    # fit mumu vertex distribution
    #===========================================
    ## define fit functions
    #fit_list = [] # combined fit function
    #fits = [] # to draw combined fit function

    ## create exponential fit for J/psi 
    #fit_list.append(TF1("fit_expo", fit_expo_standalone, 0, 10, 2))
    ##fit_list[0].SetParameters(2,-1) # gaus fit
    #fit_list[0].SetParLimits(0,3.0,5.0); # gaus fit
    #fit_list[0].SetParLimits(1,-1,-0.5); # gaus fit

    ## fit histogram
    #histograms[0].Fit("fit_expo","0MR")
    #
    ## take fitted function
    #fits.append(histograms[0].GetFunction("fit_expo"))

    ## draw combined fit
    #fits[0].SetLineColor(kRed)
    #fits[0].SetLineWidth(2)
    #fits[0].SetLineStyle(2)
    #fits[0].Draw("same")

    #print("Low mass exponential background est = " + str(fits[0].Integral(5,100)))
    #print("chi2 = " + str(fits[0].GetChisquare()))

    ##===========================================

    # draw ATLAS label
    ATLAS_LABEL(0.20,0.87)
    myText(0.33,0.87,1,"Internal")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    legend.Draw("SAME")

    c.Print("output/data_dv_mumu_M_low.pdf")



