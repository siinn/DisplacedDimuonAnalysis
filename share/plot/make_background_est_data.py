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

f_m_data = TFile.Open("root/FBE/FBE.data.r23.root", 'READ') 
f_m_susy_ttbar = TFile.Open("root/FBE/FBE.ttbar.massveto10.r37.root", 'READ') 

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

#scale_factor = 0.9
scale_factor = 1.1

##=============================================
## cutflow (control region, validation region)
##=============================================
#
#parameters = ['idid','mut','et','mumu','ee','emu']
#y_axis = ['trk-trk', '#mu-track', 'e-track', '#mu-#mu', 'e-e', 'e-#mu']
#opposite = ['trk^{+}-trk^{-}', '#mu^{#pm}-track^{#mp}', 'e^{#pm}-track^{#mp}', '#mu^{+}-#mu^{-}', 'e^{#pm}-e^{#mp}','e^{#pm}-#mu^{#mp}']
# 
#for n, p in enumerate(parameters):
#    
#        # define legends
#        legends = [
#                    #'Trk-Trk vertex from reconstruction',
#                    #'Trk-Trk vertex from random crossing (scaled)',
#                    #'Observed (trk-trk vertex from reconstruction)',
#                    '2016 data',
#                    'Track flipping',
#                    ]
#    
#        # initialize legend
#        #legend = TLegend(0.27,0.73,0.88,0.93)
#        legend = TLegend(0.65,0.80,0.93,0.88)
#     
#        # initialize canvas
#        c= TCanvas("canvas_%s_cf" %p , "", 800, 600)
#    
#        # Upper plot will be in pad1
#        pad1 = TPad("pad1", "pad1", 0, 0, 1, 1.0)
#        pad1.SetBottomMargin(0.20) 
#        pad1.SetLeftMargin(0.15) 
#        pad1.Draw()             # Draw the upper pad: pad1
#        pad1.cd()               # pad1 becomes the current pad
#    
#        # set log
#        log = True
#    
#        if (log == True):
#            c.SetLogy()
#            pad1.SetLogy()
#    
#        # read histograms
#        histograms = []
#    
#        histograms.append(f_m_data.Get('main_analysis/dv_%s/m_dv_%s_cf' %(p,p)))
#        histograms.append(f_m_data.Get('FlipBkgEst/dv_%s/m_%s_cf_flip' %(p,p)))
#    
#        # set histogram color 
#        histograms[0].SetLineColor(kBlack)
#        histograms[1].SetLineColor(color_4)
#    
#        # set histogram color 
#        #histograms[0].SetFillColor(kBlack)
#        #histograms[1].SetFillColor(color_3)
#    
#        # set marker color
#        histograms[0].SetMarkerColor(kBlack)
#        histograms[1].SetMarkerColor(color_4)
#    
#        # set marker style
#        histograms[0].SetMarkerStyle(20)
#        histograms[1].SetMarkerStyle(21)
#    
#        # set marker style
#        histograms[0].SetMarkerSize(1.5)
#        histograms[1].SetMarkerSize(1.5)
#    
#        # finding global maximum
#        hmax_global = 0
#        hmin_global = 10e10
#    
#        for i in range(len(histograms)):
#            # scale
#            #if ((histograms[i].Integral() > 0) and (i==1)):
#                #histograms[i].Scale( 0.5*histograms[i].Integral())
#                #histograms[i].Scale(scale_factor)
#    
#            hmax = getMaximum(histograms[i])
#            if (hmax > hmax_global):
#                hmax_global = hmax
#            hmin = getMinimumNonzero(histograms[i])
#            if (hmin < hmin_global):
#                hmin_global = hmin
#    
#     
#        for i in range(len(histograms)):
#            if (log == True):
#                histograms[i].SetMaximum(hmax_global*1e1)
#                histograms[i].SetMinimum(hmin_global*1e-1)
#            if (log == False):
#                histograms[i].SetMaximum(hmax_global*1.5)
#                histograms[i].SetMinimum(0)
#            #histograms[i].GetXaxis().SetLabelSize(0.040)
#            #histograms[i].GetXaxis().SetBinLabel(1,"#mu#mu,ee,e#mu")
#            #histograms[i].GetXaxis().SetBinLabel(5,"#mu^{+}#mu^{-},e^{+}e^{-},e^{+,-}#mu^{-,+}")
#            histograms[i].Sumw2
#            #histograms[i].SetMinimum(5e5)
#            #histograms[i].SetMaximum(1e8)
#            #histograms[i].GetYaxis().SetTitle("Vertex")
#            histograms[i].GetYaxis().SetTitle("%s vertex" % y_axis[n])
#            histograms[i].GetYaxis().SetTitleOffset(1.5)
#            #histograms[i].GetXaxis().SetLabelOffset(1)
#            #histograms[i].GetXaxis().SetNdivisions(8)
#            #histograms[i].SetAxisRange(1,8,"x");
#            #histograms[i].GetXaxis().SetBinLabel(1,"%s" % y_axis[n])
#            #histograms[i].GetXaxis().SetBinLabel(2,"#chi^{2}_{DV} / DOF < 5")
#            #histograms[i].GetXaxis().SetBinLabel(3,"Disp. > 2 mm")
#            #histograms[i].GetXaxis().SetBinLabel(4,"%s" % opposite[n])
#            #histograms[i].GetXaxis().SetBinLabel(5,"DisabledModule")
#            #histograms[i].GetXaxis().SetBinLabel(6,"MaterialVeto")
#            #histograms[i].GetXaxis().SetBinLabel(7,"m > 3 GeV")
#            #histograms[i].GetXaxis().SetBinLabel(8,"R_{CR} > 0.04")
#            #histograms[i].GetXaxis().SetBinLabel(9,"#DeltaR > 0.5")
#            #if (p == 'mumu' or p == 'ee' or p == 'emu'):
#            #    histograms[i].SetAxisRange(1,9,"x");
#            #    histograms[i].GetXaxis().SetBinLabel(1,"%s" % y_axis[n])
#            #    histograms[i].GetXaxis().SetBinLabel(2,"Trig. Matching")
#            #    histograms[i].GetXaxis().SetBinLabel(3,"#chi^{2}_{DV} / DOF < 5")
#            #    histograms[i].GetXaxis().SetBinLabel(4,"Disp. > 2 mm")
#            #    histograms[i].GetXaxis().SetBinLabel(5,"%s" % opposite[n])
#            #    histograms[i].GetXaxis().SetBinLabel(6,"DisabledModule")
#            #    histograms[i].GetXaxis().SetBinLabel(7,"MaterialVeto")
#            #    histograms[i].GetXaxis().SetBinLabel(8,"m > 3 GeV")
#            #    histograms[i].GetXaxis().SetBinLabel(9,"R_{CR} > 0.04")
#            #    histograms[i].GetXaxis().SetBinLabel(10,"#DeltaR > 0.5")
#            #histograms[i].GetXaxis().SetTitle("")
#            #histograms[i].Draw("PE SAME")
#            #legend.AddEntry(histograms[i], legends[i], "L")
#    
#        # custom legend
#        #legend.AddEntry(histograms[0], legends[0], "L")
#        legend.AddEntry(histograms[0], legends[0], "L")
#        legend.AddEntry(histograms[1], legends[1], "L")
#    
#        # custom drawing order
#        histograms[0].Draw("hist SAME text0")
#        histograms[1].Draw("hist SAME text")
#    
#        ## draw legend
#        legend.SetBorderSize(0)
#        legend.SetTextSize(0.030)
#        legend.Draw("SAME")
#    
#        # draw ATLAS label
#        ATLAS_LABEL(0.20,0.87)
#        #myText(0.32,0.87,1,"Preliminary")
#        myText(0.33,0.87,1,"Internal")
#        myText(0.20,0.82,1,"2016 data, 32.7 fb^{-1}")
#    
#    
#        c.Print("output/m_FBE_data_cutflow_%s.pdf" %p)
#
#
##=======================================================
## reconstructed and random crossing DV (control region)
##=======================================================
#
##if False:
#parameters = ['M', 'R', 'z', 'l', 'chi2_ndof']
##x_axis = ['Invariant mass [GeV]', 'r [mm]', 'z [mm]', '#it{l} [mm]', '#chi^{2} / DOF']
#x_axis = ['Invariant mass [GeV]', 'r [mm]', 'z [mm]', '', '#chi^{2} / DOF']
#
#for n, p in enumerate(parameters):
#
#    # define legends
#    legends = [
#                '2016 Data',
#                'Track flipping',
#                ]
#
#    # initialize legend
#    #legend = TLegend(0.27,0.73,0.88,0.93)
#    legend = TLegend(0.70,0.73,0.93,0.88)
#
#    #ATLAS_LABEL(0.20,0.87)
#    #myText(0.32,0.87,1,"Internal")
# 
#    # initialize canvas
#    c= TCanvas("canvas_idid_%s" % p, "", 800, 600)
#
#    # Upper plot will be in pad1
#    pad1 = TPad("pad1", "pad1", 0, 0.30, 1, 1.0)
#    pad1.SetBottomMargin(0.02) 
#    pad1.SetLeftMargin(0.1) 
#    pad1.Draw()             # Draw the upper pad: pad1
#    pad1.cd()               # pad1 becomes the current pad
#
#    # set log
#    log = False
#
#    if (log == True):
#        c.SetLogy()
#        pad1.SetLogy()
#
#    # read histograms
#    histograms = []
#
#    histograms.append(f_m_data.Get('dv_idid/dv_idid_%s' %p))
#    histograms.append(f_m_data.Get('FlipBkgEst/dv_idid/m_idid_flip_%s' %p))
#
#    # set histogram color 
#    histograms[0].SetLineColor(kBlack)
#    histograms[1].SetLineColor(color_4)
#
#    # set histogram color 
#    #histograms[0].SetFillColor(kBlack)
#    #histograms[1].SetFillColor(color_4)
#
#    # set marker color
#    histograms[0].SetMarkerColor(kBlack)
#    histograms[1].SetMarkerColor(color_4)
#
#    # set marker style
#    histograms[0].SetMarkerStyle(20)
#    histograms[1].SetMarkerStyle(21)
#
#    # set marker style
#    histograms[0].SetMarkerSize(0.8)
#    histograms[1].SetMarkerSize(0)
#
#    # finding global maximum
#    hmax_global = 0
#    hmin_global = 10e10
#
#
#    for i in range(len(histograms)):
#        # scale
#        #if ((histograms[i].Integral() > 0) and (i==1)):
#            #histograms[i].Scale( 0.5*histograms[i].Integral())
#            #histograms[i].Scale(scale_factor)
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
#            histograms[i].SetMaximum(hmax_global*1e1)
#            histograms[i].SetMinimum(hmin_global*1e-1)
#        if (log == False):
#            histograms[i].SetMaximum(hmax_global*1.5)
#            histograms[i].SetMinimum(0)
#        #histograms[i].GetXaxis().SetLabelSize(0.040)
#        #histograms[i].GetXaxis().SetBinLabel(1,"#mu#mu,ee,e#mu")
#        #histograms[i].GetXaxis().SetBinLabel(5,"#mu^{+}#mu^{-},e^{+}e^{-},e^{+,-}#mu^{-,+}")
#        histograms[i].Sumw2
#        if p == 'M':
#            histograms[i].SetAxisRange(0,30,"x");
#        histograms[i].GetYaxis().SetTitle("Vertex")
#        histograms[i].GetYaxis().SetTitleOffset(1)
#        histograms[i].GetXaxis().SetLabelOffset(1)
#        histograms[i].GetXaxis().SetNdivisions(8)
#        histograms[i].GetXaxis().SetTitle("%s" % x_axis[n])
#        #histograms[i].Draw("PE SAME")
#        #legend.AddEntry(histograms[i], legends[i], "L")
#
#    # custom legend
#    legend.AddEntry(histograms[0], legends[0], "PL")
#    legend.AddEntry(histograms[1], legends[1], "PL")
#
#    # custom drawing order
#    histograms[1].DrawCopy("hist");
#    histograms[1].SetFillColor(color_4);
#    histograms[1].SetFillStyle(3245);
#    #histograms[1].SetFillStyle(1001);
#    histograms[1].Draw("e2same")
#    histograms[0].Draw("PE SAME")
#
#    ## draw legend
#    legend.SetBorderSize(0)
#    legend.SetTextSize(0.035)
#    legend.Draw("SAME")
#
#    # draw detector line
#    if (p =='R'):
#        layer0 = TLine(33.25,0,33.25,7e4)
#        layer0.SetLineStyle(8)
#        layer0.SetLineColor(kRed)
#        layer0.Draw("same")
#        layer1 = TLine(50.5,0,50.5,7e4)
#        layer1.SetLineStyle(8)
#        layer1.SetLineColor(kRed)
#        layer1.Draw("same")
#        layer2 = TLine(88.5,0,88.5,7e4)
#        layer2.SetLineStyle(8)
#        layer2.SetLineColor(kRed)
#        layer2.Draw("same")
#        layer3 = TLine(122.5,0,122.5,7e4)
#        layer3.SetLineStyle(8)
#        layer3.SetLineColor(kRed)
#        layer3.Draw("same")
#        layer4 = TLine(299,0,299,7e4)
#        layer4.SetLineStyle(8)
#        layer4.SetLineColor(kRed)
#        layer4.Draw("same")
#        pixtube = TLine(229,0,229,7e4)
#        pixtube.SetLineStyle(3)
#        pixtube.SetLineColor(kGreen)
#        pixtube.Draw("same")
#        env1 = TLine(45.5,0,45.5,7e4)
#        env1.SetLineStyle(3)
#        env1.SetLineColor(kBlue)
#        env1.Draw("same")
#        env2 = TLine(242,0,242,7e4)
#        env2.SetLineStyle(3)
#        env2.SetLineColor(kBlue)
#        env2.Draw("same")
#        env3 = TLine(255,0,255,7e4)
#        env3.SetLineStyle(3)
#        env3.SetLineColor(kBlue)
#        env3.Draw("same")
#
#    # draw ATLAS label
#    ATLAS_LABEL(0.14,0.87)
#    #myText(0.32,0.87,1,"Preliminary")
#    myText(0.23,0.87,1,"Internal")
#    myText(0.14,0.80,1,"2016 data, 32.7 fb^{-1}")
#
#
#    #=================================
#    # ratio plot
#    #=================================
#    # lower plot will be in pad
#    c.cd()          # Go back to the main canvas before defining pad2
#    pad2 = TPad("pad2", "pad2", 0, 0.04, 1, 0.29)
#    pad2.SetTopMargin(0.04)
#    pad2.SetBottomMargin(0.35)
#    pad2.SetLeftMargin(0.1) 
#    pad2.Draw()
#    pad2.cd()       # pad2 becomes the current pad
#
#    # ratio plot
#    h3 = histograms[0].Clone("h3")
#
#    # Ratio plot (h3) settings
#    h3.SetTitle("") # Remove the ratio title
#
#    # Y axis ratio plot settings
#    h3.GetYaxis().SetTitle("Data / Track flip.")
#    h3.GetYaxis().SetNdivisions(505)
#    h3.GetYaxis().SetTitleFont(43)
#    h3.GetYaxis().SetTitleSize(18)
#    #h3.GetYaxis().SetTitleOffset(1.55)
#    h3.GetYaxis().SetLabelFont(43) # Absolute font size in pixel (precision 3)
#    h3.GetYaxis().SetLabelSize(20)
#
#    # X axis ratio plot settings
#    h3.GetXaxis().SetTitleFont(43)
#    h3.GetXaxis().SetTitleSize(20)
#    h3.GetXaxis().SetTitleOffset(4.5)
#    h3.GetXaxis().SetLabelOffset(0.05)
#    h3.GetXaxis().SetLabelFont(43) # Absolute font size in pixel (precision 3)
#    h3.GetXaxis().SetLabelSize(20)
#
#    # set ratio plot properties
#    h3.SetLineColor(kBlack)
#    #h3.SetFillColor(30)
#    #h3.SetFillStyle(3004)
#    #h3.SetMinimum(0.5)  
#    #h3.SetMaximum(1.5)  
#    h3.SetMinimum(0.5)
#    h3.SetMaximum(1.5)
#    h3.Sumw2()
#    h3.SetStats(0)      # No statistics on lower plot
#    h3.Divide(histograms[1])
#    h3.SetMarkerStyle(20)
#    h3.SetMarkerSize(1)
#    h3.SetMarkerColor(kBlack)
#    h3.SetLineColor(kBlack)
#    #h3.SetFillColor(color_5)
#    #h3.SetFillStyle(3004)
#    h3.GetXaxis().SetTickLength(0.08)
#    #h3.Draw("e2")           # Draw the ratio plot
#    h3.Draw("same ep")       # Draw the ratio plot
#
#    # draw horizontal line at y = 0
#    y0 = 1.
#    x_min = h3.GetXaxis().GetXmin()
#    x_max = h3.GetXaxis().GetXmax()
#    if p == 'M':
#        x_max = 31
#    l = TLine(x_min ,y0 , x_max ,y0)
#    l.SetLineColor(kRed)
#    l.SetLineStyle(2)
#    l.Draw()
#
#    # draw TMathText for curly 'l'
#    c.cd()
#    if (p == 'l'):
#        mt = TMathText()
#        mt.SetTextSize(0.04);
#        mt.DrawMathText(0.88, 0.05, "\\ell  [mm]")
#        c.Print("output/m_FBE_data_%s.eps" %p)
#
#    else:
#        c.Print("output/m_FBE_data_%s.pdf" %p)


#=============================================
# count number of selected tracks
#=============================================


if(True):

    lep = ['id','elc','mu']
    histograms = []
    
    for n, lepton in enumerate(lep):

        histograms.append(f_m_data.Get('FlipBkgEst/m_n_%s_sel' %lepton))
        #print("selected %s tracks = "%lepton )
        #print(histograms[n].Integral(1,3,"width"))
        #print("selected %s tracks = " + str(histograms[n].Integral(0,10e10)) %lepton )

        Sum = 0

        for i in range(histograms[n].GetXaxis().GetNbins()):
            if(histograms[n].GetBinContent(i+1) != 0.0):
                print(lepton + ": " + str(histograms[n].GetBinContent(i+1)) + ", nTrack = " + str(i))
                Sum += histograms[n].GetBinContent(i+1) * i
        print(lepton + " total tracks = " + str(Sum))
