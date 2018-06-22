#!/usr/bin/env python
# --------------------------------------------------------------
# Example jobOption.py file to feed athena.py using algorithms
# and tools/cuts of the DV_xAODAnalysis package
# --------------------------------------------------------------

# read athena pool xAOD files
import AthenaPoolCnvSvc.ReadAthenaPool
from AthenaCommon.GlobalFlags import globalflags
import glob   

theApp.EvtMax = -1
#theApp.EvtMax = 2000

#--------------------------------------------
# DAOD_SUSY15 data
#--------------------------------------------
# all 2016 data
#svcMgr.EventSelector.InputCollections = glob.glob( "/n/atlas05/userdata/sche/data/DAOD_SUSY15/*/*.root.1" )

# trigger filtered
#svcMgr.EventSelector.InputCollections += glob.glob("/n/atlas05/userdata/sche/data/DAOD_SUSY15_FILTERED/user.sche.data16_13TeV.physics_Main.DAOD_SUSY15.r8669_p2950.Lep_Filtered.r10_EXT0/*.root")


#--------------------------------------------
# Signal MC samples
#--------------------------------------------

# SUSY15, official
#svcMgr.EventSelector.InputCollections = glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zprimemumu/user.sche.mc15_13TeV.301911.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m250t100.recon.DAOD_RPVLL.e4125_s2698_r8788.r12_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zprimemumu/user.sche.mc15_13TeV.301912.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m250t250.recon.DAOD_RPVLL.e4125_s2698_r8788.r12_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zprimemumu/user.sche.mc15_13TeV.301913.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m250t500.recon.DAOD_RPVLL.e4125_s2698_r8788.r14_EXT0/*.root')
svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zprimemumu/user.sche.mc15_13TeV.301914.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m500t100.recon.DAOD_RPVLL.e4125_s2698_r8788.r12_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zprimemumu/user.sche.mc15_13TeV.301915.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m500t250.recon.DAOD_RPVLL.e4125_s2698_r8788.r11_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zprimemumu/user.sche.mc15_13TeV.301916.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m500t500.recon.DAOD_RPVLL.e4125_s2698_r8788.r12_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zprimemumu/user.sche.mc15_13TeV.301917.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m1000t100.recon.DAOD_RPVLL.e4125_s2698_r8788.r13_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zprimemumu/user.sche.mc15_13TeV.301918.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m1000t250.recon.DAOD_RPVLL.e4125_s2698_r8788.r14_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zprimemumu/user.sche.mc15_13TeV.301919.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m1000t500.recon.DAOD_RPVLL.e4125_s2698_r8788.r13_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zprimemumu/user.sche.mc15_13TeV.308264.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m100t100.recon.DAOD_RPVLL.e5818_s2726_r8788.r2_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zprimemumu/user.sche.mc15_13TeV.308265.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m100t250.recon.DAOD_RPVLL.e5818_s2726_r8788.r2_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zprimemumu/user.sche.mc15_13TeV.308266.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m100t500.recon.DAOD_RPVLL.e5818_s2726_r8788.r2_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zprimemumu/user.sche.mc15_13TeV.308285.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m750t100.recon.DAOD_RPVLL.e5818_s2726_r8788.r2_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zprimemumu/user.sche.mc15_13TeV.308286.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m750t250.recon.DAOD_RPVLL.e5818_s2726_r8788.r2_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zprimemumu/user.sche.mc15_13TeV.308287.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m750t500.recon.DAOD_RPVLL.e5818_s2726_r8788.r2_EXT0/*.root')

# SUSY15, no skim, mumu
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zmumu_noskim/user.sche.mc15_13TeV.308264.Pythia8EvtGen_LLzprimemumu_m100t100.recon.DAOD_RPVLL.e5818_s2726_r8788.noskim.r3_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zmumu_noskim/user.sche.mc15_13TeV.308265.Pythia8EvtGen_LLzprimemumu_m100t250.recon.DAOD_RPVLL.e5818_s2726_r8788.noskim.r3_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zmumu_noskim/user.sche.mc15_13TeV.308266.Pythia8EvtGen_LLzprimemumu_m100t500.recon.DAOD_RPVLL.e5818_s2726_r8788.noskim.r3_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zmumu_noskim/user.sche.mc15_13TeV.301911.Pythia8EvtGen_LLzprimemumu_m250t100.recon.DAOD_RPVLL.e4125_s2698_r8788.noskim.r3_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zmumu_noskim/user.sche.mc15_13TeV.301912.Pythia8EvtGen_LLzprimemumu_m250t250.recon.DAOD_RPVLL.e4125_s2698_r8788.noskim.r3_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zmumu_noskim/user.sche.mc15_13TeV.301913.Pythia8EvtGen_LLzprimemumu_m250t500.recon.DAOD_RPVLL.e4125_s2698_r8788.noskim.r3_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zmumu_noskim/user.sche.mc15_13TeV.301914.Pythia8EvtGen_LLzprimemumu_m500t100.recon.DAOD_RPVLL.e4125_s2698_r8788.noskim.r3_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zmumu_noskim/user.sche.mc15_13TeV.301915.Pythia8EvtGen_LLzprimemumu_m500t250.recon.DAOD_RPVLL.e4125_s2698_r8788.noskim.r3_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zmumu_noskim/user.sche.mc15_13TeV.301916.Pythia8EvtGen_LLzprimemumu_m500t500.recon.DAOD_RPVLL.e4125_s2698_r8788.noskim.r3_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zmumu_noskim/user.sche.mc15_13TeV.308285.Pythia8EvtGen_LLzprimemumu_m750t100.recon.DAOD_RPVLL.e5818_s2726_r8788.noskim.r3_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zmumu_noskim/user.sche.mc15_13TeV.308286.Pythia8EvtGen_LLzprimemumu_m750t250.recon.DAOD_RPVLL.e5818_s2726_r8788.noskim.r3_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zmumu_noskim/user.sche.mc15_13TeV.308287.Pythia8EvtGen_LLzprimemumu_m750t500.recon.DAOD_RPVLL.e5818_s2726_r8788.noskim.r3_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zmumu_noskim/user.sche.mc15_13TeV.301917.Pythia8EvtGen_LLzprimemumu_m1000t100.recon.DAOD_RPVLL.e4125_s2698_r8788.noskim.r3_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zmumu_noskim/user.sche.mc15_13TeV.301918.Pythia8EvtGen_LLzprimemumu_m1000t250.recon.DAOD_RPVLL.e4125_s2698_r8788.noskim.r3_EXT0/*.root')
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zmumu_noskim/user.sche.mc15_13TeV.308266.Pythia8EvtGen_LLzprimemumu_m100t500.recon.DAOD_RPVLL.e5818_s2726_r8788.noskim.r3_EXT0/*.root')

#--------------------------------------------
# Signal MC samples (no skim) m = 250 GeV, ctau= 250 mm
#--------------------------------------------

# mumu test
#svcMgr.EventSelector.InputCollections = glob.glob('/n/atlas05/userdata/sche/20.7.8.7.SUSY15/output/mumu_noskim/DAOD_SUSY15.mumu.*.root')

# ee test
#svcMgr.EventSelector.InputCollections += glob.glob('/n/atlas05/userdata/sche/20.7.8.7.SUSY15/output/ee_noskim/DAOD_SUSY15.ee.*.root')

# emu test
#svcMgr.EventSelector.InputCollections += glob.glob('/n/atlas05/userdata/sche/20.7.8.7.SUSY15/output/emu_noskim/DAOD_SUSY15.emu.*.root')


#--------------------------------------------
# Background samples
#--------------------------------------------

# diboson
#svcMgr.EventSelector.InputCollections = glob.glob("/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/background/user.sche.mc15_13TeV.361064.Sherpa_CT10_lllvSFMinus.recon.DAOD_SUSY15.e3836_s2608_s2183_r8788.r5_EXT0/*.root")
#svcMgr.EventSelector.InputCollections = glob.glob("/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/background/user.sche.mc15_13TeV.361066.Sherpa_CT10_lllvSFPlus.recon.DAOD_SUSY15.e3836_s2608_s2183_r8788.r1_EXT0/*.root")
#svcMgr.EventSelector.InputCollections = glob.glob("/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/background/user.sche.mc15_13TeV.361068.Sherpa_CT10_llvv.recon.DAOD_SUSY15.e3836_s2608_s2183_r8788.r1_EXT0/*.root")
#svcMgr.EventSelector.InputCollections = glob.glob("/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/background/user.sche.mc15_13TeV.361063.Sherpa_CT10_llll.recon.DAOD_SUSY15.e3836_s2608_s2183_r8788.r1_EXT0/*.root")

# ttbar
#svcMgr.EventSelector.InputCollections += glob.glob("/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/background/user.sche.mc15_13TeV.410252.Sherpa_ttbar_dilepton_MEPS_NLO.recon.DAOD_SUSY15.e5450_s2726_r8788_p2949.lepfilter.r4_EXT0/user.sche.11532947.EXT0._00000*.DAOD_SUSY15.pool.root")
#svcMgr.EventSelector.InputCollections += glob.glob("/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/background/user.sche.mc15_13TeV.410252.Sherpa_ttbar_dilepton_MEPS_NLO.recon.DAOD_SUSY15.e5450_s2726_r8788_p2949.lepfilter.r4_EXT0/user.sche.11532947.EXT0._00001*.DAOD_SUSY15.pool.root")
#svcMgr.EventSelector.InputCollections += glob.glob("/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/background/user.sche.mc15_13TeV.410252.Sherpa_ttbar_dilepton_MEPS_NLO.recon.DAOD_SUSY15.e5450_s2726_r8788_p2949.lepfilter.r4_EXT0/user.sche.11532947.EXT0._00002*.DAOD_SUSY15.pool.root")
#svcMgr.EventSelector.InputCollections += glob.glob("/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/background/user.sche.mc15_13TeV.410252.Sherpa_ttbar_dilepton_MEPS_NLO.recon.DAOD_SUSY15.e5450_s2726_r8788_p2949.lepfilter.r4_EXT0/user.sche.11532947.EXT0._00003*.DAOD_SUSY15.pool.root")

# JW
#svcMgr.EventSelector.InputCollections = glob.glob("/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/background/mc15_13TeV.361023.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ3W.merge.DAOD_SUSY15.e3668_s2576_s2132_r8788_p2877/*.root.*")

# handler for the main sequence
algseq = CfgMgr.AthSequencer("AthAlgSeq")


#--------------------------------------------
# Algorithms to run
#--------------------------------------------
from DisplacedDimuonAnalysis.DisplacedDimuonAnalysisConf import DisplacedDimuonAnalysisAlg

# Data and MC
#algseq += CfgMgr.DisplacedDimuonAnalysisAlg()
#algseq += CfgMgr.TrackingSystematics()
#algseq += CfgMgr.CosmicBackground()
#algseq += CfgMgr.MuonPlots()
algseq += CfgMgr.SUSYSelection()

# MC sample only
#algseq += CfgMgr.DVEfficiency()
#algseq += CfgMgr.TruthPlots()
#algseq += CfgMgr.LeptonEfficiency()
#algseq += CfgMgr.VertexRes()

# LRT validation
#algseq += CfgMgr.LRTValidation()

# use pileup reweighting?
usePRW = True

#---------------------------------------------------------------
# including the tools /cuts needed by the algorithm, and configuring them
#---------------------------------------------------------------
svcMgr.MessageSvc.OutputLevel = INFO
#svcMgr.MessageSvc.OutputLevel = DEBUG
svcMgr.MessageSvc.defaultLimit = 99990

# histogram output service
if not hasattr(svcMgr, 'THistSvc'): svcMgr += CfgMgr.THistSvc()
svcMgr.THistSvc.Output += ["DV DATAFILE='output.root' OPT='RECREATE'"]
svcMgr.THistSvc.OutputLevel = INFO
#svcMgr.THistSvc.OutputLevel = DEBUG

# DVCuts tool
ToolSvc += CfgMgr.DDL__DVCuts("DiLepBaseCuts")
ToolSvc.DiLepBaseCuts.distMin  = 4. # mm
ToolSvc.DiLepBaseCuts.DisabledModuleMapFile = "DisabledModuleMap_Run2_v2.root"
ToolSvc.DiLepBaseCuts.MaterialMapFile = "materialMap3D_Run2_v2.1.1.root"

# EventCuts
ToolSvc += CfgMgr.DDL__EventCuts("DiLepEventCuts")
ToolSvc.DiLepEventCuts.TriggerNames = [ "HLT_mu60_0eta105_msonly",
                                        "HLT_g140_loose",
                                        "HLT_2g50_loose",
                                        "HLT_2g60_loose_L12EM15VH"
                                        ]

# GoodRunsListSelectorTool
vecStringGRL = 'data16_13TeV.periodAllYear_DetStatus-v83-pro20-15_DQDefects-00-02-04_PHYS_StandardGRL_All_Good_25ns_DAOD_RPVLL_r8669.xml'
ToolSvc += CfgMgr.GoodRunsListSelectionTool("GRLTool")
ToolSvc.GRLTool.GoodRunsListVec=[vecStringGRL]
ToolSvc.GRLTool.OutputLevel = INFO

# Muon selection tool
ToolSvc += CfgMgr.CP__MuonSelectionTool("MuonSelectionTool")
ToolSvc.MuonSelectionTool.MaxEta = 2.7
# 0 = tight, 1 = medium, 2 = loose, 3 = very loose
ToolSvc.MuonSelectionTool.MuQuality = 2
#ToolSvc.MuonSelectionTool.MuQuality = 3
ToolSvc.MuonSelectionTool.OutputLevel = INFO

# Muon correction tool
ToolSvc += CfgMgr.CP__MuonCalibrationAndSmearingTool("MuonCorrectionTool")
ToolSvc.MuonCorrectionTool.Year = "Data16"
ToolSvc.MuonCorrectionTool.Release = "Recs2016_15_07"
#ToolSvc.MuonCorrectionTool.SagittaCorr = True
ToolSvc.MuonCorrectionTool.OutputLevel = INFO

# Electron Likelihood tool
confDir = "ElectronPhotonSelectorTools/offline/mc15_20160512/"
ToolSvc += CfgMgr.AsgElectronLikelihoodTool("ElectronLikelihoodTool")
ToolSvc.ElectronLikelihoodTool.ConfigFile= confDir+"ElectronLikelihoodLooseOfflineConfig2016_Smooth_NoD0.conf"
#ToolSvc.ElectronLikelihoodTool.WorkingPoint= "LooseLHElectron"
#ToolSvc.ElectronLikelihoodTool.WorkingPoint= "MediumLHElectron"
#ToolSvc.ElectronLikelihoodTool.WorkingPoint= "TightLHElectron"
ToolSvc.ElectronLikelihoodTool.OutputLevel = INFO

# Trigger decision tool
## set up trigger decision tool
from TrigDecisionTool.TrigDecisionToolConf import Trig__TrigDecisionTool
ToolSvc += Trig__TrigDecisionTool( "TrigDecisionTool" )
ToolSvc.TrigDecisionTool.OutputLevel = INFO

if (usePRW):
    # pile-up reweighting tool
    ToolSvc += CfgMgr.CP__PileupReweightingTool("PileupReweightingTool",
                                                #ConfigFiles = ["prw.mumu.*.root"],
                                                ConfigFiles = ["prw.mumu_noskim.m500t100.root"],
                                                #ConfigFiles = ["prw.mumu.root","prw.ee.root","prw.emu.root"],
                                                #ConfigFiles = [""],
                                                LumiCalcFiles = ["ilumicalc_histograms_None_297730-311481_OflLumi-13TeV-009_DAOD_RPVLL_r8669.root"],
                                                TrigDecisionTool = "Trig::TrigDecisionTool/TrigDecisionTool",
                                                DataScaleFactor = 1. / 1.09,
                                                DataScaleFactorUP = 1.,
                                                DataScaleFactorDOWN = 1. / 1.18)
    
