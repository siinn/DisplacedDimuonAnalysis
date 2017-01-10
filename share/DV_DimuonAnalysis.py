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
theApp.EvtMax = 10000

#--------------------------------------------
# DAOD_SUSY15 data
#--------------------------------------------
#svcMgr.EventSelector.InputCollections = glob.glob( "/n/atlas05/userdata/sche/data/DAOD_SUSY15/*/*.root.1" )
#svcMgr.EventSelector.InputCollections = glob.glob( "/n/atlas05/userdata/sche/data/DAOD_SUSY15/data16_13TeV.00298773.physics_Main.merge.DAOD_SUSY15.f698_m1453_r8669_p2875/*.root.1" )

# subset, ~ 1 fb-1
svcMgr.EventSelector.InputCollections = glob.glob("/n/atlas05/userdata/sche/data/DAOD_SUSY15/data16_13TeV.00302393.physics_Main.merge.DAOD_SUSY15.f711_m1616_r8669_p2875/*.root.1")
svcMgr.EventSelector.InputCollections += glob.glob("/n/atlas05/userdata/sche/data/DAOD_SUSY15/data16_13TeV.00301973.physics_Main.merge.DAOD_SUSY15.f709_m1616_r8669_p2875/*.root.1")

#--------------------------------------------
# Signal MC samples
#--------------------------------------------

# SUSY15, official
#svcMgr.EventSelector.InputCollections = glob.glob( '/n/atlas05/userdata/sche/MC15/SUSY15/zprimemumu/mc15_13TeV.301913.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m250t500.merge.DAOD_SUSY15.e4125_s2698_r8788_p2877_tid10371380_00/*.root.1')

#--------------------------------------------

# r8, t=100, retracking from HITS
#svcMgr.EventSelector.InputCollections = glob.glob( '/n/atlas05/userdata/sche/MC15/xAOD/zprimemumu/LRT.VRT/r8/user.sche.mc15_13TeV.301911.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m250t100.simul.HITS.e4821_s2698.xAOD.r8_EXT0/*.root')

# r8, t=500, retracking from HITS
#svcMgr.EventSelector.InputCollections = glob.glob( '/n/atlas05/userdata/sche/MC15/xAOD/zprimemumu/LRT.VRT/r8/user.sche.mc15_13TeV.301913.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m250t500.simul.HITS.e4821_s2698.xAOD.r8_EXT0/*.root')

# r9, t=100, retracking from HITS, N = 20,000
#svcMgr.EventSelector.InputCollections = glob.glob( '/n/atlas05/userdata/sche/MC15/xAOD/zprimemumu/LRT.VRT/r9/user.sche.mc15_13TeV.301917.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m1000t100.simul.HITS.e4125_s2698.xAOD.r9_EXT0/*.root')

# r9, t=500, retracking from HITS, N = 20,000
#svcMgr.EventSelector.InputCollections = glob.glob( '/n/atlas05/userdata/sche/MC15/xAOD/zprimemumu/LRT.VRT/r9/user.sche.mc15_13TeV.301919.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m1000t500.simul.HITS.e4125_s2698.xAOD.r9_EXT0/*.root')


# EXOT21, decorated, r9, t=100, retracking from HITS, N = 20,000
#svcMgr.EventSelector.InputCollections = glob.glob( "/n/atlas05/userdata/sche/MC15/xAOD/zprimemumu/EXOT21/r9/user.sche.mc15_13TeV.301917.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m1000t100.simul.HITS.e4125_s2698.xAOD.r9_EXT0.root" )

# EXOT21, decorated, r9, t=500, retracking from HITS, N = 20,000
#svcMgr.EventSelector.InputCollections = glob.glob( "/n/atlas05/userdata/sche/MC15/xAOD/zprimemumu/EXOT21/r9/user.sche.mc15_13TeV.301919.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m1000t500.simul.HITS.e4125_s2698.xAOD.r9_EXT0.root" )


#--------------------------------------------
# Background samples
#--------------------------------------------

# ttbar -> dilepton
#svcMgr.EventSelector.InputCollections = ["/n/atlas05/userdata/sche/MC15/xAOD/background/mc15_13TeV.410252.Sherpa_221_NNPDF30NNLO_ttbar_dilepton_MEPS_NLO.merge.AOD.e5450_s2726_r7772_r7676/AOD.09776749._000351.pool.root.1"]

# diboson
#svcMgr.EventSelector.InputCollections = ["/n/atlas05/userdata/sche/MC15/xAOD/background/mc15_13TeV.361063.Sherpa_CT10_llll.merge.AOD.e3836_s2608_s2183_r7725_r7676/AOD.07915626._000073.pool.root.1"]
#svcMgr.EventSelector.InputCollections = ["/n/atlas05/userdata/sche/MC15/xAOD/background/mc15_13TeV.361064.Sherpa_CT10_lllvSFMinus.merge.AOD.e3836_s2608_s2183_r7725_r7676/AOD.07915663._000005.pool.root.1"]
#svcMgr.EventSelector.InputCollections = ["/n/atlas05/userdata/sche/MC15/xAOD/background/mc15_13TeV.361066.Sherpa_CT10_lllvSFPlus.merge.AOD.e3836_s2608_s2183_r7725_r7676/AOD.07915700._000028.pool.root.1"]
#svcMgr.EventSelector.InputCollections = ["/n/atlas05/userdata/sche/MC15/xAOD/background/mc15_13TeV.361068.Sherpa_CT10_llvv.merge.AOD.e3836_s2608_s2183_r7725_r7676/AOD.07915759._000544.pool.root.1"]

# reprocessed ttbar -> dilepton
#svcMgr.EventSelector.InputCollections = ["/n/atlas05/userdata/sche/20.7.8.GetTfCommand/run1_r8788/output.root"]
#svcMgr.EventSelector.InputCollections = ["/n/atlas05/userdata/sche/20.7.8.GetTfCommand/run3/DAOD_TEST1.output.root"]
#svcMgr.EventSelector.InputCollections = ["/n/atlas05/userdata/sche/20.7.8.GetTfCommand/run4/output.root"]
#svcMgr.EventSelector.InputCollections = ["/n/atlas05/userdata/sche/20.7.8.GetTfCommand/run3/output.root"]

#--------------------------------------------
# LRT validatoin
# r7, secondary pile-up truth, no vertexing
#--------------------------------------------
#svcMgr.EventSelector.InputCollections = glob.glob( '/n/atlas05/userdata/sche/MC15/xAOD/zprimemumu/fulltruth/user.sche.LRT.r7.mc15_13TeV.301913.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m250t500.recon.ESD.e4821_s2698_r8028_AOD/*.root')

# secondary pile-up truth, no vertexing, decorated
#svcMgr.EventSelector.InputCollections = glob.glob( '/n/atlas05/userdata/sche/MC15/xAOD/zprimemumu/EXOT21/fulltruth/DAOD_EXOT21.fulltruth.output.root')



#svcMgr.EventSelector.InputCollections = [""]
# --------------------------------------------------------------

# handler for the main sequence
algseq = CfgMgr.AthSequencer("AthAlgSeq")

from DisplacedDimuonAnalysis.DisplacedDimuonAnalysisConf import DisplacedDimuonAnalysisAlg
# Data and MC
algseq += CfgMgr.DisplacedDimuonAnalysisAlg()
#algseq += CfgMgr.LumiCalc()

# MC sample only
#algseq += CfgMgr.DVEfficiency()
#algseq += CfgMgr.MuonEfficiency()
#algseq += CfgMgr.TruthPlots()
#algseq += CfgMgr.VertexRes()

# LRT validation
#algseq += CfgMgr.LRTValidation()

# histogram output service
if not hasattr(svcMgr, 'THistSvc'): svcMgr += CfgMgr.THistSvc()
svcMgr.THistSvc.Output += ["DV DATAFILE='output.root' OPT='RECREATE'"]
svcMgr.MessageSvc.OutputLevel = INFO
svcMgr.MessageSvc.defaultLimit = 999999


#---------------------------------------------------------------
# including the tools /cuts needed by the algorithm, and configuring them
#---------------------------------------------------------------
#ToolSvc += CfgMgr.DV__EventCuts("EventCuts",useGRL=True,checkTriggers=True)

# Trigger matching tool
#ToolSvc += CfgMgr.Trig__MatchingTool("TriggerMatchingTool",OutputLevel=DEBUG)
#ToolSvc += CfgMgr.Trig__MatchingTool("TriggerMatchingTool")

# DVCuts tool
ToolSvc += CfgMgr.DV__DVCuts("DiLepBaseCuts")
ToolSvc.DiLepBaseCuts.distMin  = 3. # mm
#ToolSvc.DiLepBaseCuts.DVMassMin  = 100000.0 # MeV 
#ToolSvc.DiLepBaseCuts.MaterialMapFile = "/n/atlas05/userdata/sche/2.4.18.DV_Analysis/DV_xAODAnalysis/DVAnalyses/data/materialMap3D_Run2_v4.root"


# GoodRunsListSelectorTool
vecStringGRL = '/n/atlas05/userdata/sche/2.4.21.DV_Analysis/DV_xAODAnalysis/DVAnalyses/data/data16_13TeV.periodAllYear_DetStatus-v83-pro20-15_DQDefects-00-02-04_PHYS_StandardGRL_All_Good_25ns_DAOD_RPVLL_r8669.xml'
#vecStringGRL = '/n/atlas05/userdata/sche/2.4.21.DV_Analysis/grl/subset.xml'


ToolSvc += CfgMgr.GoodRunsListSelectionTool("GRLTool")
ToolSvc.GRLTool.GoodRunsListVec=[vecStringGRL]
ToolSvc.GRLTool.OutputLevel = INFO


# from GoodRunsLists.GoodRunsListsConf import *
# GoodRunsTool = GoodRunsListSelectorTool("GRLTool")
# GoodRunsTool.GoodRunsListVec  = [vecStringGRL ] ## specify your grl here
# GoodRunsTool.EventSelectorMode = True
# GoodRunsTool.PassThrough = False
# GoodRunsTool.OutputLevel = DEBUG
# #GoodRunsTool.RejectBlackRunsInEventSelector = True  ## False by default


# Luminosity calculatoin tool
#ToolSvc += CfgMgr.LumiBlockMetaDataTool("LumiBlockMetaDataTool")
#svcMgr.MetaDataSvc.MetaDataTools += [ ToolSvc.LumiBlockMetaDataTool ]



