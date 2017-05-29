#!/usr/bin/env python
# --------------------------------------------------------------
# Example jobOption.py file to feed athena.py using algorithms
# and tools/cuts of the DV_xAODAnalysis package
# --------------------------------------------------------------

# read athena pool xAOD files
import AthenaPoolCnvSvc.ReadAthenaPool
from AthenaCommon.GlobalFlags import globalflags
import glob   

#theApp.EvtMax = -1
theApp.EvtMax = 200

#--------------------------------------------
# DAOD_SUSY15 data
#--------------------------------------------
# all 2016 data
#svcMgr.EventSelector.InputCollections = glob.glob( "/n/atlas05/userdata/sche/data/DAOD_SUSY15/*/*.root.1" )

# trigger filtered
#svcMgr.EventSelector.InputCollections += glob.glob("/n/atlas05/userdata/sche/data/DAOD_SUSY15_FILTERED/user.sche.data16_13TeV.physics_Main.DAOD_RPVLL.r8669.trigfilter.r1_EXT0/*.root")


#--------------------------------------------
# Signal MC samples
#--------------------------------------------

# ee test
#svcMgr.EventSelector.InputCollections = glob.glob('/n/atlas05/userdata/sche/20.7.8.7.SUSY15/output/DAOD_SUSY15.DAOD_SUSY15.ee.pool.root')
# emu test
#svcMgr.EventSelector.InputCollections = glob.glob('/n/atlas05/userdata/sche/20.7.8.7.SUSY15/output/DAOD_SUSY15.DAOD_SUSY15.emu.pool.root')

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


#--------------------------------------------
# Background samples
#--------------------------------------------

# diboson
#svcMgr.EventSelector.InputCollections = glob.glob("/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/background/user.sche.mc15_13TeV.361064.Sherpa_CT10_lllvSFMinus.recon.DAOD_SUSY15.e3836_s2608_s2183_r8788.r5_EXT0/*.root")
#svcMgr.EventSelector.InputCollections = glob.glob("/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/background/user.sche.mc15_13TeV.361066.Sherpa_CT10_lllvSFPlus.recon.DAOD_SUSY15.e3836_s2608_s2183_r8788.r1_EXT0/*.root")
#svcMgr.EventSelector.InputCollections = glob.glob("/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/background/user.sche.mc15_13TeV.361068.Sherpa_CT10_llvv.recon.DAOD_SUSY15.e3836_s2608_s2183_r8788.r1_EXT0/*.root")
#svcMgr.EventSelector.InputCollections = glob.glob("/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/background/user.sche.mc15_13TeV.361063.Sherpa_CT10_llll.recon.DAOD_SUSY15.e3836_s2608_s2183_r8788.r1_EXT0/*.root")

# ttbar
#svcMgr.EventSelector.InputCollections = glob.glob("/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/background/user.sche.mc15_13TeV.410252.Sherpa_221_NNPDF30NNLO_ttbar_dilepton_MEPS_NLO.recon.DAOD_SUSY15.e5450_s2726_r8788.r1_EXT0/*.root")


#--------------------------------------------
# LRT validatoin
# r7, secondary pile-up truth, no vertexing
#--------------------------------------------
#svcMgr.EventSelector.InputCollections = glob.glob( '/n/atlas05/userdata/sche/MC15/xAOD/zprimemumu/fulltruth/user.sche.LRT.r7.mc15_13TeV.301913.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m250t500.recon.ESD.e4821_s2698_r8028_AOD/*.root')

# secondary pile-up truth, no vertexing, decorated
#svcMgr.EventSelector.InputCollections = glob.glob( '/n/atlas05/userdata/sche/MC15/xAOD/zprimemumu/EXOT21/fulltruth/DAOD_EXOT21.fulltruth.output.root')
# --------------------------------------------------------------

# handler for the main sequence
algseq = CfgMgr.AthSequencer("AthAlgSeq")


#--------------------------------------------
# Algorithms to run
#--------------------------------------------
from DisplacedDimuonAnalysis.DisplacedDimuonAnalysisConf import DisplacedDimuonAnalysisAlg

# Data and MC
algseq += CfgMgr.DisplacedDimuonAnalysisAlg()
#algseq += CfgMgr.FlipBkgEst()
#algseq += CfgMgr.SUSYDV()
#algseq += CfgMgr.MuonPlots()

# MC sample only
#algseq += CfgMgr.DVEfficiency()
#algseq += CfgMgr.MuonEfficiency()
#algseq += CfgMgr.TruthPlots()
#algseq += CfgMgr.VertexRes()

# LRT validation
#algseq += CfgMgr.LRTValidation()

#---------------------------------------------------------------
# including the tools /cuts needed by the algorithm, and configuring them
#---------------------------------------------------------------
svcMgr.MessageSvc.OutputLevel = INFO
#svcMgr.MessageSvc.OutputLevel = DEBUG
svcMgr.MessageSvc.defaultLimit = 9999

# histogram output service
if not hasattr(svcMgr, 'THistSvc'): svcMgr += CfgMgr.THistSvc()
svcMgr.THistSvc.Output += ["DV DATAFILE='output.root' OPT='RECREATE'"]

# DVCuts tool
ToolSvc += CfgMgr.DDL__DVCuts("DiLepBaseCuts")
ToolSvc.DiLepBaseCuts.distMin  = 2. # mm
ToolSvc.DiLepBaseCuts.DisabledModuleMapFile = "DisabledModuleMap_Run2_v2.root"
ToolSvc.DiLepBaseCuts.MaterialMapFile = "materialMap3D_Run2_v2.1.1.root"

# EventCuts
ToolSvc += CfgMgr.DDL__EventCuts("DiLepEventCuts")
ToolSvc.DiLepEventCuts.TriggerNames = [ "HLT_mu60_0eta105_msonly",
                                        "HLT_g140_loose",
                                        "HLT_2g50_loose"
                                        ]

# GoodRunsListSelectorTool
vecStringGRL = '/n/atlas05/userdata/sche/2.4.21.DV_Analysis/DV_xAODAnalysis/DVAnalyses/data/data16_13TeV.periodAllYear_DetStatus-v83-pro20-15_DQDefects-00-02-04_PHYS_StandardGRL_All_Good_25ns_DAOD_RPVLL_r8669.xml'
ToolSvc += CfgMgr.GoodRunsListSelectionTool("GRLTool")
ToolSvc.GRLTool.GoodRunsListVec=[vecStringGRL]
ToolSvc.GRLTool.OutputLevel = INFO

# Muon selection tool
ToolSvc += CfgMgr.CP__MuonSelectionTool("MuonSelectionTool")
ToolSvc.MuonSelectionTool.MaxEta = 2.7
ToolSvc.MuonSelectionTool.MuQuality = 2
ToolSvc.MuonSelectionTool.OutputLevel = INFO

# Muon correction tool
ToolSvc += CfgMgr.CP__MuonCalibrationAndSmearingTool("MuonCorrectionTool")
ToolSvc.MuonCorrectionTool.Year = "Data16"
ToolSvc.MuonCorrectionTool.Release = "Recs2016_15_07"
ToolSvc.MuonCorrectionTool.SagittaCorr = True
ToolSvc.MuonCorrectionTool.OutputLevel = INFO

# Electron Likelihood tool
confDir = "ElectronPhotonSelectorTools/offline/mc15_20160512/"
ToolSvc += CfgMgr.AsgElectronLikelihoodTool("ElectronLikelihoodTool")
#ToolSvc.ElectronLikelihoodTool.ConfigFile= confDir+"ElectronLikelihoodLooseOfflineConfig2016_Smooth_NoD0.conf"
ToolSvc.ElectronLikelihoodTool.WorkingPoint= "MediumLHElectron"
#ToolSvc.ElectronLikelihoodTool.WorkingPoint= "TightLHElectron"
ToolSvc.ElectronLikelihoodTool.OutputLevel = INFO


#---------------------------------------------------------------
# obsolte 
#---------------------------------------------------------------

# from GoodRunsLists.GoodRunsListsConf import *
# GoodRunsTool = GoodRunsListSelectorTool("GRLTool")
# GoodRunsTool.GoodRunsListVec  = [vecStringGRL ] ## specify your grl here
# GoodRunsTool.EventSelectorMode = True
# GoodRunsTool.PassThrough = False
# GoodRunsTool.OutputLevel = DEBUG
# #GoodRunsTool.RejectBlackRunsInEventSelector = True  ## False by default

# Trigger matching tool
#ToolSvc += CfgMgr.Trig__MatchingTool("TriggerMatchingTool",OutputLevel=DEBUG)
#ToolSvc += CfgMgr.Trig__MatchingTool("TriggerMatchingTool")

# TriggerDecisionTool
#from TrigDecision.TrigDecisionConf import TrigDec__TrigDecisionTool
#tdt = TrigDec__TrigDecisionTool("TrigDecisionTool")
#tdt.OutputLevel = DEBUG
#ToolSvc += tdt

# Luminosity calculatoin tool
#ToolSvc += CfgMgr.LumiBlockMetaDataTool("LumiBlockMetaDataTool")
#svcMgr.MetaDataSvc.MetaDataTools += [ ToolSvc.LumiBlockMetaDataTool ]

# Overlap removal tool
#from AssociationUtils.config import recommended_tools
#orTool = recommended_tools(masterName="OverlapRemovalTool",
#                           doElectrons=False,
#                           doMuons=True,
#                           doJets=False,
#                           doTaus=False,
#                           doPhotons=False)

