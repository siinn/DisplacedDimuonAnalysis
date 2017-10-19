#!/usr/bin/python
import os, sys
from ROOT import *
from math import log10, exp

# loading ATLAS style
gROOT.LoadMacro("AtlasStyle.C")
gROOT.LoadMacro("AtlasUtils.C")
SetAtlasStyle()

#=============================================
# fit functions
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

# sum of gaussian and quadratic background function
def fit_combined(x,par):
    return fit_background(x,par) + fit_gaus(x,par)

# signal gaussian - background
def fit_diff(x,par):
    return abs(fit_gaus(x,par) - fit_background(x,par))

# exponential
def fit_expo(x,par):
    #return par[0]*exp((x[0]-par[1]) / par[2],2));
    return exp(par[0] + par[1]*x[0]);


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

f_m_data = TFile.Open("/Users/sche/Documents/plots/analysis/root/main_analysis/main.r13.root", 'READ')

#=============================================
# define colors
#=============================================
color_1 = kRed-3
color_2 = kOrange+1
#color_3 = kAzure+6
#color_4 = kAzure+2
#color_3 = kOrange-4
#color_4 = kOrange+8
color_3 = kWhite
color_4 = kRed
#color_3 = kOrange-4
#color_4 = kOrange+1
color_5 = kGray+3
color_6 = kAzure


#=============================================
# DeltaR vs R_cr
#=============================================

if True:
 
    # initialize canvas
    c= TCanvas("canvas_deltaR_Rcr", "", 800, 600)

    legend = TLegend(0.50,0.85,0.85,0.93)
    legends = [
                '',
                ]

    # set log
    log = False

    if (log == True):
        c.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_m_data.Get('cosmicBkg/mumu/dv_mumu_DeltaR_Rcos'))

    # set text option
    gStyle.SetPaintTextFormat(".2f");
    gStyle.SetPalette(kBird)

    #histograms[0].SetAxisRange(0,1,"y");
    #histograms[0].SetAxisRange(0,1,"x");
    histograms[0].GetXaxis().SetTitle("#DeltaR")
    histograms[0].GetXaxis().SetNdivisions(6)
    histograms[0].GetYaxis().SetTitle("R_{CR}")
    histograms[0].SetMarkerSize(1.2)
    histograms[0].Sumw2()
    #histograms[0].Draw("colz text0 E")
    histograms[0].Draw("colz")

    legend.AddEntry(histograms[0], legends[0], "")
    legend.SetBorderSize(0)
    legend.SetTextSize(0.025)
    #legend.Draw("SAME")

    #c.SetCanvasSize(650,600)
    c.SetRightMargin(400)
    c.Print("output/cosmicBkg_DeltaR_Rcr.pdf")

#=============================================
# Rcr of mumu vertex and mumu pair
#=============================================

if (True):
    
    # define legends
    legends = [
                '#mu#mu vertex',
                '#mu#mu pair (scaled)',
                ]
    
    # initialize legend
    legend = TLegend(0.65,0.75,0.93,0.88)
    
    # initialize canvas
    c= TCanvas("canvas_Rcr_cf", "", 800, 600)
    
    # Upper plot will be in pad1
    pad1 = TPad("pad1", "pad1", 0, 0, 1, 1.0)
    pad1.SetBottomMargin(0.20) 
    pad1.SetLeftMargin(0.15) 
    pad1.Draw()             # Draw the upper pad: pad1
    pad1.cd()               # pad1 becomes the current pad
    
    # set log
    log = False
    #log = True
    
    if (log == True):
        c.SetLogy()
        pad1.SetLogy()
    
    # read histograms
    histograms = []
    
    histograms.append(f_m_data.Get('cosmicBkg/mumu/dv_mumu_Rcos_low'))
    histograms.append(f_m_data.Get('cosmicBkg/mumuPair/mumu_Rcos_low'))
    
    # set histogram color 
    histograms[0].SetLineColor(color_4)
    histograms[1].SetLineColor(kBlack)
    
    # set histogram color 
    #histograms[0].SetFillColor(kBlack)
    #histograms[1].SetFillColor(color_3)
    
    # set marker color
    histograms[0].SetMarkerColor(color_4)
    histograms[1].SetMarkerColor(kBlack)
    
    # set marker style
    histograms[0].SetMarkerStyle(22)
    histograms[1].SetMarkerStyle(23)
    
    # set marker style
    histograms[0].SetMarkerSize(1.5)
    histograms[1].SetMarkerSize(1.5)
    
    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10
   
    # scale mumu pair to mumu vertex
    histograms[1].Scale(histograms[0].Integral() / histograms[1].Integral())

    for i in range(len(histograms)):
        # scale
        #if ((histograms[i].Integral() > 0) and (i==1)):
            #histograms[i].Scale( 0.5*histograms[i].Integral())
            #histograms[i].Scale(scale_factor)

        # rebin before finding minimum, maximum
        histograms[i].Rebin(9)
    
        hmax = getMaximum(histograms[i])
        if (hmax > hmax_global):
            hmax_global = hmax
        hmin = getMinimumNonzero(histograms[i])
        if (hmin < hmin_global):
            hmin_global = hmin
    
    
    for i in range(len(histograms)):
        if (log == True):
            histograms[i].SetMaximum(hmax_global*1e1)
            histograms[i].SetMinimum(hmin_global*1e-1)
        if (log == False):
            histograms[i].SetMaximum(hmax_global*1.5)
            histograms[i].SetMinimum(0)
        #histograms[i].GetXaxis().SetLabelSize(0.040)
        #histograms[i].GetXaxis().SetBinLabel(1,"#mu#mu,ee,e#mu")
        #histograms[i].GetXaxis().SetBinLabel(5,"#mu^{+}#mu^{-},e^{+}e^{-},e^{+,-}#mu^{-,+}")
        histograms[i].Sumw2
        #histograms[i].SetMinimum(5e5)
        #histograms[i].SetMaximum(1e8)
        histograms[i].GetYaxis().SetTitle("Entries")
        histograms[i].GetYaxis().SetTitleOffset(1.5)
        histograms[i].GetXaxis().SetTitle("R_{CR}")
        #histograms[i].GetXaxis().SetLabelOffset(1)
        histograms[i].GetXaxis().SetNdivisions(10)
        histograms[i].SetAxisRange(0,0.04,"x");
        #histograms[i].GetXaxis().SetTitle("")
        #histograms[i].Draw("PE SAME")
        #legend.AddEntry(histograms[i], legends[i], "L")

    
    # custom legend
    legend.AddEntry(histograms[0], legends[0], "L")
    legend.AddEntry(histograms[1], legends[1], "L")
    
    # custom drawing order
    histograms[0].Draw("hist SAME")
    histograms[1].Draw("hist SAME")
    
    #===========================================
    # fit mumu vertex distribution
    #===========================================
    # define fit functions
    fit_list = [] # combined fit function
    fits = [] # to draw combined fit function
    
    # get histogram info
    division = histograms[0].GetNbinsX()
    m_min = histograms[0].GetBinLowEdge(1)
    m_max = histograms[0].GetBinLowEdge(division+1)
    
    # create combined fit function
    #fit_list.append(TF1("fit_gaus", fit_gaus, 0, 1, 3))
    fit_list.append(TF1("fit_gaus", fit_expo, 0, 1, 2))
    #fit_list[0].SetParameters(1.0,1.0)
    fit_list[0].SetParLimits(0,3.0,6.0); # const
    fit_list[0].SetParLimits(1,-1000,-600); # slope
    
    # fit histogram
    histograms[0].Fit("fit_gaus","0MR")
    
    # take fitted function
    fits.append(histograms[0].GetFunction("fit_gaus"))
    
    # draw combined fit
    fits[0].SetLineColor(kBlue)
    fits[0].SetLineWidth(2)
    fits[0].SetLineStyle(2)
    fits[0].Draw("same")

    print("background est = " + str(fits[0].Integral(0.04,5)))
    print("chi2 = " + str(fits[0].GetChisquare()))
    #===========================================
    
    ## draw legend
    legend.AddEntry(fits[0], "#mu#mu vertex fit", "L")
    legend.SetBorderSize(0)
    legend.SetTextSize(0.030)
    legend.Draw("SAME")
    
    # draw ATLAS label
    ATLAS_LABEL(0.20,0.87)
    #myText(0.32,0.87,1,"Preliminary")
    myText(0.33,0.87,1,"Internal")
    myText(0.20,0.82,1,"2016 data, 33.1 fb^{-1}")
    
    c.Print("output/cosmicBkg_Rcr.pdf")

