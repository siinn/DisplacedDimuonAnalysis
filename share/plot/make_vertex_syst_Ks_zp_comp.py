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

# sum of BW and quadratic background function
def fit_combined(x,par):
    #return fit_background(x,par) + fit_bw(x,par);
    return fit_background(x,par) + fit_gaus(x,par);


#=============================================
# input
#=============================================

f_m_MC = TFile.Open("/Users/sche/Documents/plots/analysis/root/Ks_zp_comparison/syst.MC.root", 'READ') 


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
# Ks and Z' distribution
#=============================================

parameters = ['r', 'z', 'pt', 'eta', 'DeltaR']
x_axis = ['r [mm]', 'z [mm]', 'p_{T} [GeV]', '#eta', '#Delta R']
y_axis = ['K_{S}', 'Z\'']

for n, p in enumerate(parameters):

    # define legends
    legends = [
                'K_{S}',
                'Z\', m = 250 GeV, c#tau = 250'
                ]

    # initialize legend
    #legend = TLegend(0.27,0.73,0.88,0.93)
    legend = TLegend(0.57,0.78,0.80,0.91)

    #ATLAS_LABEL(0.20,0.87)
    #myText(0.32,0.87,1,"Internal")
 
    # initialize canvas
    c= TCanvas("canvas_ks_zp_%s" % p, "", 800, 600)

    # Upper plot will be in pad1
    pad1 = TPad("pad1", "pad1", 0, 0, 1.0, 1.0)
    pad1.SetBottomMargin(0.15) 
    pad1.SetLeftMargin(0.15) 
    pad1.Draw()             # Draw the upper pad: pad1
    pad1.cd()               # pad1 becomes the current pad

    # set log
    log = False

    # set log for R plot
    #if (p == 'r'):
    #    log = True

    if (log == True):
        pad1.SetLogy()

    # read histograms
    histograms = []

    histograms.append(f_m_MC.Get('tracking_syst/truth-matched_Ks/dv_Ks_%s' %p))
    histograms.append(f_m_MC.Get('tracking_syst/truth-matched_zp/dv_zp_%s' %p))

    # set histogram color 
    histograms[0].SetLineColor(kBlack)
    histograms[1].SetLineColor(color_4)

    # set histogram color 
    #histograms[0].SetFillColor(kBlack)
    #histograms[1].SetFillColor(color_4)

    # set marker color
    histograms[0].SetMarkerColor(kBlack)
    histograms[1].SetMarkerColor(color_4)

    # set marker style
    histograms[0].SetMarkerStyle(22)
    histograms[1].SetMarkerStyle(23)

    # set marker style
    histograms[0].SetMarkerSize(1)
    histograms[1].SetMarkerSize(1)

    # finding global maximum
    hmax_global = 0
    hmin_global = 10e10


    # normalize data to MC
    #histograms[1].Scale(histograms[0].Integral()/histograms[1].Integral())

    for i in range(len(histograms)):
        # scale
        #if (histograms[i].Integral() > 0):
        #    histograms[i].Scale( 1/histograms[i].Integral())
        #    #histograms[i].Scale(scale_factor)
        histograms[i].Rebin(2)

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
        #if p == 'M':
        #histograms[i].SetAxisRange(0,150,"x");
        #histograms[i].SetAxisRange(0,300,"y");
        histograms[i].GetYaxis().SetTitle("N")
        #histograms[i].GetYaxis().SetTitleOffset(1)
        #histograms[i].GetXaxis().SetLabelOffset(1)
        histograms[i].GetXaxis().SetNdivisions(8)
        histograms[i].GetXaxis().SetTitle("%s" % x_axis[n])
        #histograms[i].Draw("PE SAME")
        #legend.AddEntry(histograms[i], legends[i], "L")

    # custom legend
    legend.AddEntry(histograms[0], legends[0], "L")
    legend.AddEntry(histograms[1], legends[1], "L")

    # custom drawing order
    #histograms[1].DrawCopy("hist");
    #histograms[1].SetFillColor(color_4);
    #histograms[1].SetFillStyle(3245);
    #histograms[1].SetFillStyle(1001);
    #histograms[1].Draw("e2same")
    #histograms[0].Draw("PE SAME")
    histograms[1].Draw("hist e same")
    histograms[0].Draw("hist e same")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.035)
    legend.Draw("SAME")

    # draw detector line
    top_line = 0.25
    if (p =='r'):
        layer0 = TLine(33.25,0,33.25,top_line)
        layer0.SetLineStyle(8)
        layer0.SetLineColor(kRed)
        layer0.Draw("same")
        layer1 = TLine(50.5,0,50.5,top_line)
        layer1.SetLineStyle(8)
        layer1.SetLineColor(kRed)
        layer1.Draw("same")
        layer2 = TLine(88.5,0,88.5,top_line)
        layer2.SetLineStyle(8)
        layer2.SetLineColor(kRed)
        layer2.Draw("same")
        layer3 = TLine(122.5,0,122.5,top_line)
        layer3.SetLineStyle(8)
        layer3.SetLineColor(kRed)
        layer3.Draw("same")
        layer4 = TLine(299,0,299,top_line)
        layer4.SetLineStyle(8)
        layer4.SetLineColor(kRed)
        layer4.Draw("same")
        pixtube = TLine(229,0,229,top_line)
        pixtube.SetLineStyle(3)
        pixtube.SetLineColor(kGreen)
        pixtube.Draw("same")
        env1 = TLine(45.5,0,45.5,top_line)
        env1.SetLineStyle(3)
        env1.SetLineColor(kBlue)
        env1.Draw("same")
        env2 = TLine(242,0,242,top_line)
        env2.SetLineStyle(3)
        env2.SetLineColor(kBlue)
        env2.Draw("same")
        env3 = TLine(255,0,255,top_line)
        env3.SetLineStyle(3)
        env3.SetLineColor(kBlue)
        env3.Draw("same")

    # draw ATLAS label
    ATLAS_LABEL(0.18,0.87)
    myText(0.31,0.87,1,"Internal")
    #myText(0.32,0.87,1,"Preliminary")
    #myText(0.23,0.87,1,"Internal simulation")
    #myText(0.14,0.80,1,"t#bar{t} MC sample, 0.7M events")


    ##=================================
    ## ratio plot
    ##=================================
    ## lower plot will be in pad
    #c.cd()          # Go back to the main canvas before defining pad2
    #pad2 = TPad("pad2", "pad2", 0, 0.04, 1, 0.29)
    #pad2.SetTopMargin(0.04)
    #pad2.SetBottomMargin(0.35)
    #pad2.SetLeftMargin(0.1) 
    #pad2.Draw()
    #pad2.cd()       # pad2 becomes the current pad

    ## ratio plot
    #h3 = histograms[0].Clone("h3")

    ## Ratio plot (h3) settings
    #h3.SetTitle("") # Remove the ratio title

    ## Y axis ratio plot settings
    #h3.GetYaxis().SetTitle("Data / MC")
    #h3.GetYaxis().SetNdivisions(505)
    #h3.GetYaxis().SetTitleFont(43)
    #h3.GetYaxis().SetTitleSize(18)
    ##h3.GetYaxis().SetTitleOffset(1.55)
    #h3.GetYaxis().SetLabelFont(43) # Absolute font size in pixel (precision 3)
    #h3.GetYaxis().SetLabelSize(20)

    ## X axis ratio plot settings
    #h3.GetXaxis().SetTitleFont(43)
    #h3.GetXaxis().SetTitleSize(20)
    #h3.GetXaxis().SetTitleOffset(4.5)
    #h3.GetXaxis().SetLabelOffset(0.05)
    #h3.GetXaxis().SetLabelFont(43) # Absolute font size in pixel (precision 3)
    #h3.GetXaxis().SetLabelSize(20)

    ## set ratio plot properties
    #h3.SetLineColor(kBlack)
    ##h3.SetFillColor(30)
    ##h3.SetFillStyle(3004)
    ##h3.SetMinimum(0.5)  
    ##h3.SetMaximum(1.5)  
    #h3.SetMinimum(0.)
    #h3.SetMaximum(2.)
    #h3.Sumw2()
    #h3.SetStats(0)      # No statistics on lower plot
    #h3.Divide(histograms[1])
    #h3.SetMarkerStyle(20)
    #h3.SetMarkerSize(0.5)
    #h3.SetMarkerColor(kBlack)
    #h3.SetLineColor(kBlack)
    ##h3.SetFillColor(color_5)
    ##h3.SetFillStyle(3004)
    #h3.GetXaxis().SetTickLength(0.08)
    ##h3.Draw("e2")           # Draw the ratio plot
    #h3.Draw("same ep")       # Draw the ratio plot

    ## draw horizontal line at y = 0
    #y0 = 1.
    #x_min = h3.GetXaxis().GetXmin()
    #x_max = h3.GetXaxis().GetXmax()
    #l = TLine(x_min ,y0 , x_max ,y0)
    #l.SetLineColor(kRed)
    #l.SetLineStyle(2)
    #l.Draw()

    c.Print("output/m_syst_zp_Ks_%s.pdf" %p)

