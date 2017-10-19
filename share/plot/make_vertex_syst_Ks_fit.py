#!/usr/bin/python
import os, sys
from ROOT import *
from math import log10, exp

# loading ATLAS style
gROOT.LoadMacro("AtlasStyle.C")
gROOT.LoadMacro("AtlasUtils.C")
SetAtlasStyle()

# set text option
#gStyle.SetPaintTextFormat(".0f");

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

# only for drawing
def fit_background_draw(x,par):
    return par[0] + par[1]*x[0];

# sum of gaussian and quadratic background function
def fit_combined(x,par):
    return fit_background(x,par) + fit_gaus(x,par)

# signal gaussian - background
def fit_diff(x,par):
    return abs(fit_gaus(x,par) - fit_background(x,par))

#=============================================
# input
#=============================================

f_m_data = TFile.Open("/Users/sche/Documents/plots/analysis/root/main_analysis/main.r13.root", 'READ') 
#f_m_data = TFile.Open("/Users/sche/Documents/plots/analysis/root/vertexing_syst/syst.data.noLepTrig.root", 'READ') 
f_m_MC = TFile.Open("/Users/sche/Documents/plots/analysis/root/vertexing_syst/syst.lepfil.MC.root", 'READ') 



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

#=============================================
# Ks mass distribution in bins of r
#=============================================

# list to hold Ks count after background subtraction
Ks_count_data = []
Ks_count_MC = []

sample = ['Data', 'Combined MC']
filename = ['Data', 'Combined_MC']

#sample = ['Data']
#filename = ['Data']

for m, sample in enumerate(sample):
    #parameters = ['1','2','3','4','5','6']
    parameters = ['1','2','3','4','5','6']
    R_bin = [
            'r (mm) < 50',
            '50 #leq r (mm) < 100',
            '100 #leq r (mm) < 150',
            '150 #leq r (mm) < 200',
            '200 #leq r (mm) < 250',
            '250 #leq r (mm) < 300',
            ]
    #x_axis = ['Invariant mass [GeV]', 'r [mm]', 'z [mm]', '', 'p_{T} [GeV]', '<#mu>']
    
    for n, p in enumerate(parameters):
        # define legends
        legends = [
                    '%s' % sample
                    ]
    
        # initialize legend
        legend = TLegend(0.56,0.75,0.86,0.86)
    
        #ATLAS_LABEL(0.20,0.87)
        #myText(0.32,0.87,1,"Internal")
     
        # initialize canvas
        c= TCanvas("canvas_idid_%s_%s" % (sample,p), "", 800, 600)
    
        # Upper plot will be in pad1
        pad1 = TPad("pad1", "pad1", 0, 0, 1, 1)
        pad1.SetBottomMargin(0.15)
        pad1.SetLeftMargin(0.15)
        pad1.SetRightMargin(0.10)
        pad1.Draw()             # Draw the upper pad: pad1
        pad1.cd()               # pad1 becomes the current pad
    
        # set log
        log = True
        #log = False
    
        if (log == True):
            pad1.SetLogy()
    
        # read histograms
        histograms = []
   
        if (sample == 'Data'):
            histograms.append(f_m_data.Get('tracking_syst/dv_idid/m/dv_idid_M_%s' %p))
        if (sample == 'Combined MC'):
            histograms.append(f_m_MC.Get('tracking_syst/dv_idid/m/dv_idid_M_%s' %p))
    
        # set histogram color 
        histograms[0].SetLineColor(kBlack)
        #histograms[1].SetLineColor(color_4)
    
        # set histogram color 
        #histograms[0].SetFillColor(kBlack)
        #histograms[1].SetFillColor(color_4)
    
        # set marker color
        histograms[0].SetMarkerColor(kBlack)
        #histograms[1].SetMarkerColor(color_4)
    
        # set marker style
        histograms[0].SetMarkerStyle(20)
        #histograms[1].SetMarkerStyle(21)
    
        # set marker style
        histograms[0].SetMarkerSize(0.5)
        #histograms[1].SetMarkerSize(0.5)
    
        # finding global maximum
        hmax_global = 0
        hmin_global = 10e10
    
        # normalize data to MC
        #histograms[0].Scale(histograms[1].Integral()/histograms[0].Integral())
    
        for i in range(len(histograms)):
            # scale
            #if ((histograms[i].Integral() > 0) and (i==1)):
                #histograms[i].Scale( 0.5*histograms[i].Integral())
                #histograms[i].Scale(scale_factor)
    
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
            histograms[i].Sumw2
            histograms[i].GetYaxis().SetTitle("Vertex")
            #histograms[i].GetYaxis().SetTitleOffset(1)
            #histograms[i].GetXaxis().SetLabelOffset(1)
            histograms[i].GetXaxis().SetNdivisions(8)
            histograms[i].GetXaxis().SetTitle("Invariant mass [MeV]")
            #histograms[i].Draw("PE SAME")
            #legend.AddEntry(histograms[i], legends[i], "L")
            histograms[i].Draw("hist e same")

        #--------------------------------
        # fit signal and background
        #--------------------------------
        # define fit functions
        fit_list = [] # combined fit function
        fits = [] # to draw combined fit function

        signal = [] # to draw signal fit function
        background = [] # to draw background fit function
        Diff = [] # to draw signal - background
    
        for i in range(len(histograms)):
            # get histogram info
            division = histograms[i].GetNbinsX()
            m_min = histograms[i].GetBinLowEdge(1)
            m_max = histograms[i].GetBinLowEdge(division+1)
        
            # create combined fit function
            #fit_list.append(TF1("combined_fit_%s_%s" %(sample, i), fit_combined, m_min, m_max, 5))
            fit_list.append(TF1("combined_fit_%s_%s" %(sample, i), fit_combined, 350, 650, 5))
            fit_list[i].SetParName(0,"const")
            fit_list[i].SetParName(1,"mean")
            fit_list[i].SetParName(2,"sigma")
            fit_list[i].SetParName(3,"y0")
            fit_list[i].SetParName(4,"m")

            # set parameter limits
            fit_list[i].SetParameters(0.5,500,50,0,0.1) # wider

            if (sample=='Data'):
                #fit_list[i].SetParLimits(0,450,100000); # gaus const
                fit_list[i].SetParLimits(1,490,510); # gaus mean
                fit_list[i].SetParLimits(2,10,1000); # gaus sigma
                #fit_list[i].SetParLimits(3,100,1000); # linear const
                fit_list[i].SetParLimits(4,-10,10); # linear slope
                if(n==0):
                    #fit_list[i].SetParLimits(0,65000,1000000); # gaus const
                    fit_list[i].SetParLimits(0,65000,100000); # gaus const
                    fit_list[i].SetParLimits(2,5,8); # gaus sigma
                if(n==1):
                    fit_list[i].SetParLimits(0,65000,1000000); # gaus const

            if (sample=='Combined MC'):
                #fit_list[i].SetParLimits(0,50,1000); # gaus const
                fit_list[i].SetParLimits(1,490,510); # gaus mean
                fit_list[i].SetParLimits(2,5,100); # gaus sigma
                fit_list[i].SetParLimits(3,0,1000); # linear const
                fit_list[i].SetParLimits(4,-10,10); # linear slope

            # fit histogram
            if((n==0) and (sample=='Data')):
                histograms[i].Fit("combined_fit_%s_%s" %(sample,i) ,"0MR")
            else:
                histograms[i].Fit("combined_fit_%s_%s" %(sample,i) ,"0MRQ")

            # take fitted function
            fits.append(histograms[i].GetFunction("combined_fit_%s_%s" %(sample,i)))

            # access combined fit parameter
            fit_parameters = []
            for j in range(5):
                fit_parameters.append(fits[i].GetParameter(j))

            # draw background fit
            background.append(TF1("background_%s_%s" %(sample, i), fit_background_draw, 350, 650, 2))
            background[i].SetParameters(fit_parameters[3],fit_parameters[4])
            background[i].SetLineColor(kBlue)
            background[i].SetLineStyle(2)
            background[i].Draw("same")
                
            # draw signal fit	
            signal.append(TF1("signal_%s_%s" %(sample, i), fit_gaus, 350, 650, 3))
            signal[i].SetParameters(fit_parameters[0],fit_parameters[1],fit_parameters[2])
            signal[i].SetLineColor(kRed)
            #signal[i].Draw("same")

            # draw abs(signal - background)
            Diff.append(TF1("Diff_%s_%s" %(sample, i), fit_diff, 350, 650, 5))
            Diff[i].SetParameters(fit_parameters[0],fit_parameters[1],fit_parameters[2],fit_parameters[3],fit_parameters[4])
            Diff[i].SetLineColor(kOrange)
            #Diff[i].Draw("same")

            int_min = Diff[i].GetMinimumX(400,500)
            int_max = Diff[i].GetMinimumX(500,600)
            #print "signal Integral = " + str(signal[i].Integral(int_min,int_max))
            #print "background Integral = " + str(background[i].Integral(int_min,int_max))
            #print "signal - background Integral = " + str(Diff[i].Integral(int_min,int_max))

            # append Ks count so that we can create ratio plot
            if (sample == 'Data'):
                Ks_count_data.append(Diff[i].Integral(int_min,int_max))
            if (sample == 'Combined MC'):
                Ks_count_MC.append(Diff[i].Integral(int_min,int_max))

            # draw combined fit
            fits[i].SetLineColor(kRed)
            fits[i].SetLineWidth(2)
            fits[i].Draw("same")
    
        # custom legend
        legend.AddEntry(histograms[0], legends[0], "L")
        legend.AddEntry(fits[i], "Signal + background fit", "L")
        legend.AddEntry(background[i], "Background fit", "L")
    
        ## draw legend
        legend.SetBorderSize(0)
        legend.SetTextSize(0.030)
        legend.Draw("SAME")
    
        # draw ATLAS label
        ATLAS_LABEL(0.18,0.87)
        if (sample == 'Data'):
            myText(0.31,0.87,1,"Internal")
        if (sample == 'Combined MC'):
            myText(0.31,0.87,1,"Internal simulation")
        #myText(0.32,0.87,1,"Preliminary")
        myText(0.18,0.82,1,"%s" %R_bin[n])
    
        c.Print("output/m_syst_Ks_%s_%s.pdf" %(filename[m], p))


#print Ks_count_data
#print Ks_count_MC


#=============================================
# Ks ratio plot
#=============================================

if True:

    # define legends
    legends = [
                'Data',
                'Combined MC'
                ]

    # initialize legend
    #legend = TLegend(0.27,0.73,0.88,0.93)
    legend = TLegend(0.70,0.73,0.93,0.88)

    #ATLAS_LABEL(0.20,0.87)
    #myText(0.32,0.87,1,"Internal")
 
    # initialize canvas
    c= TCanvas("canvas_ratio_%s" % p, "", 800, 600)

    # Upper plot will be in pad1
    pad1 = TPad("pad1", "pad1", 0, 0.30, 1, 1.0)
    pad1.SetBottomMargin(0.02) 
    pad1.SetLeftMargin(0.1) 
    pad1.Draw()             # Draw the upper pad: pad1
    pad1.cd()               # pad1 becomes the current pad

    # set log
    log = False

    if (log == True):
        pad1.SetLogy()

    # read histograms
    histograms = []
    Ks_in_R0 = []

    # Ks counts in bins of R
    histograms.append(TH1F("Ks_vertices_data","Ks_vertices",6,0,6))
    histograms.append(TH1F("Ks_vertices_MC","Ks_vertices",6,0,6))

    # Ks in the lowest bin
    Ks_in_R0.append(TH1F("Ks_vertices_data_R0","Ks_vertices_R0",6,0,6))
    Ks_in_R0.append(TH1F("Ks_vertices_MC_R0","Ks_vertices_R0",6,0,6))

    # fill histograms
    for i,n in enumerate(Ks_count_data):
        histograms[0].SetBinContent(i+1, n)
        Ks_in_R0[0].SetBinContent(i+1, Ks_count_data[0])
    for i,n in enumerate(Ks_count_MC):
        histograms[1].SetBinContent(i+1, n)
        Ks_in_R0[1].SetBinContent(i+1, Ks_count_MC[0])

    # correct errors before divide
    histograms[0].Sumw2()
    histograms[1].Sumw2()
    Ks_in_R0[0].Sumw2()
    Ks_in_R0[1].Sumw2()

    # divide
    histograms[0].Divide(Ks_in_R0[0])
    histograms[1].Divide(Ks_in_R0[1])

    # set x-axis
    R_bins = [  "r < 50",
                "50 #leq r < 100",
                "100 #leq r < 150",
                "150 #leq r < 200",
                "200 #leq r < 250",
                "250 #leq r < 300"]

    for i, label in enumerate(R_bins):
        histograms[0].GetXaxis().SetBinLabel(i+1,"%s" % label)
        #histograms[0].GetXaxis().LabelsOption("v")
    
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

    for i in range(len(histograms)):
        # scale
        #if ((histograms[i].Integral() > 0) and (i==1)):
            #histograms[i].Scale( 0.5*histograms[i].Integral())
            #histograms[i].Scale(scale_factor)

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
        histograms[i].Sumw2
        histograms[i].GetYaxis().SetTitle("K_{S}^{r} / K_{S}^{r < 50 mm}")
        histograms[i].GetYaxis().SetTitleOffset(1)
        histograms[i].GetXaxis().SetLabelOffset(1)
        histograms[i].GetXaxis().SetTitle("r [mm]")
        #histograms[i].GetXaxis().SetNdivisions(8)
        histograms[i].Draw("hist same")
        histograms[i].Draw("p same text0")
        #legend.AddEntry(histograms[i], legends[i], "L")


    # custom legend
    legend.AddEntry(histograms[0], legends[0], "LP")
    legend.AddEntry(histograms[1], legends[1], "LP")

    ## draw legend
    legend.SetBorderSize(0)
    legend.SetTextSize(0.035)
    legend.Draw("SAME")

    # draw ATLAS label
    ATLAS_LABEL(0.14,0.87)
    myText(0.23,0.87,1,"Internal")
    #myText(0.32,0.87,1,"Preliminary")
    #myText(0.23,0.87,1,"Internal simulation")
    #myText(0.14,0.80,1,"t#bar{t} MC sample, 0.7M events")


    #=================================
    # ratio plot
    #=================================
    # lower plot will be in pad
    c.cd()          # Go back to the main canvas before defining pad2
    pad2 = TPad("pad2", "pad2", 0, 0.04, 1, 0.29)
    pad2.SetTopMargin(0.04)
    pad2.SetBottomMargin(0.35)
    pad2.SetLeftMargin(0.1) 
    pad2.Draw()
    pad2.cd()       # pad2 becomes the current pad

    # ratio plot
    h3 = histograms[0].Clone("h3")

    # Ratio plot (h3) settings
    h3.SetTitle("") # Remove the ratio title

    # Y axis ratio plot settings
    h3.GetYaxis().SetTitle("Data / MC")
    h3.GetYaxis().SetNdivisions(505)
    h3.GetYaxis().SetTitleFont(43)
    h3.GetYaxis().SetTitleSize(18)
    #h3.GetYaxis().SetTitleOffset(1.55)
    h3.GetYaxis().SetLabelFont(43)
    h3.GetYaxis().SetLabelSize(20)

    # X axis ratio plot settings
    h3.GetXaxis().SetTitleFont(43)
    h3.GetXaxis().SetTitleSize(20)
    h3.GetXaxis().SetTitleOffset(4.5)
    h3.GetXaxis().SetLabelOffset(0.05)
    h3.GetXaxis().SetLabelFont(43)
    h3.GetXaxis().SetLabelSize(15)

    # set ratio plot properties
    h3.SetLineColor(kBlack)
    #h3.SetFillColor(30)
    #h3.SetFillStyle(3004)
    #h3.SetMinimum(0.5)  
    #h3.SetMaximum(1.5)  
    h3.SetMinimum(0.8)
    h3.SetMaximum(1.2)
    h3.Sumw2()
    h3.SetStats(0)      # No statistics on lower plot
    h3.Divide(histograms[1])
    h3.SetMarkerStyle(20)
    h3.SetMarkerSize(1)
    h3.SetMarkerColor(kBlack)
    h3.SetLineColor(kBlack)
    #h3.SetFillColor(color_5)
    #h3.SetFillStyle(3004)
    h3.GetXaxis().SetTickLength(0.08)
    h3.Draw("same ep text0")
    print(h3.Print("all"))

    # draw horizontal line at y = 0
    y0 = 1.
    x_min = h3.GetXaxis().GetXmin()
    x_max = h3.GetXaxis().GetXmax()
    l = TLine(x_min ,y0 , x_max ,y0)
    l.SetLineColor(kRed)
    l.SetLineStyle(2)
    l.Draw()

    c.Print("output/m_syst_Ks_ratio.pdf")

