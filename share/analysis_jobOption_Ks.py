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
ServiceMgr.EventSelector.SkipEvents = 0

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
#svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zprimemumu/user.sche.mc15_13TeV.301914.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m500t100.recon.DAOD_RPVLL.e4125_s2698_r8788.r12_EXT0/*.root')
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
dir_noskim = "/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zmumu_noskim/"
#svcMgr.EventSelector.InputCollections += glob.glob(dir_noskim+"user.sche.mc15_13TeV.308264.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m100t100.merge.DAOD_SUSY15.e4125_s2698_r8788_p2952.r1_EXT0/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_noskim+"user.sche.mc15_13TeV.308265.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m100t250.merge.DAOD_SUSY15.e4125_s2698_r8788_p2952.r1_EXT0/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_noskim+"user.sche.mc15_13TeV.308266.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m100t500.merge.DAOD_SUSY15.e4125_s2698_r8788_p2952.r1_EXT0/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_noskim+"mc15_13TeV.301911.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m250t100.merge.DAOD_SUSY15.e4125_s2698_r8788_p2877/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_noskim+"mc15_13TeV.301912.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m250t250.merge.DAOD_SUSY15.e4125_s2698_r8788_p2877/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_noskim+"mc15_13TeV.301913.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m250t500.merge.DAOD_SUSY15.e4125_s2698_r8788_p2877/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_noskim+"mc15_13TeV.301914.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m500t100.merge.DAOD_SUSY15.e4125_s2698_r8788_p2877/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_noskim+"mc15_13TeV.301915.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m500t250.merge.DAOD_SUSY15.e4125_s2698_r8788_p2877/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_noskim+"mc15_13TeV.301916.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m500t500.merge.DAOD_SUSY15.e4125_s2698_r8788_p2877/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_noskim+"user.sche.mc15_13TeV.308285.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m750t100.merge.DAOD_SUSY15.e5818_s2726_r8788_p2952.r1_EXT0/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_noskim+"user.sche.mc15_13TeV.308286.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m750t250.merge.DAOD_SUSY15.e5818_s2726_r8788_p2952.r1_EXT0/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_noskim+"user.sche.mc15_13TeV.308287.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m750t500.merge.DAOD_SUSY15.e5818_s2726_r8788_p2952.r1_EXT0/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_noskim+"mc15_13TeV.301917.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m1000t100.merge.DAOD_SUSY15.e4125_s2698_r8788_p2877/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_noskim+"mc15_13TeV.301918.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m1000t250.merge.DAOD_SUSY15.e4125_s2698_r8788_p2877/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_noskim+"mc15_13TeV.301919.Pythia8EvtGen_A14NNPDF23LO_LLzprimemumu_m1000t500.merge.DAOD_SUSY15.e4125_s2698_r8788_p2877/*.root*")



# SUSY15, no skim, ee
dir_ee_noskim = "/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zee_noskim/"
#svcMgr.EventSelector.InputCollections += glob.glob(dir_ee_noskim+"user.sche.mc15_13TeV.309539.Pythia8EvtGen_A14NNPDF23LO_LLzprimeee_m100t100.recon.DAOD_SUSY15.e6545_s2726_r8788.r1_EXT0/*.root*")



# SUSY15, no skim, emu
dir_emu_noskim = "/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/zemu_noskim"

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
dir_bkg = "/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/background/"

# Dijet
#svcMgr.EventSelector.InputCollections += glob.glob(dir_bkg+"mc15_13TeV.361023.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ3W.merge.DAOD_SUSY15.e3668_s2576_s2132_r8788_p2877/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_bkg+"mc15_13TeV.361024.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ4W.merge.DAOD_SUSY15.e3668_s2576_s2132_r8788_p2877/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_bkg+"mc15_13TeV.361025.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ5W.merge.DAOD_SUSY15.e3668_s2576_s2132_r8788_p2877/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_bkg+"mc15_13TeV.361026.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ6W.merge.DAOD_SUSY15.e3569_s2608_s2183_r8788_p2877/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_bkg+"mc15_13TeV.361027.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ7W.merge.DAOD_SUSY15.e3668_s2608_s2183_r8788_p2877/*.root*")

# diboson
#svcMgr.EventSelector.InputCollections += glob.glob(dir_bkg+"mc15_13TeV.361063.Sherpa_CT10_llll.merge.DAOD_SUSY15.e3836_s2608_s2183_r8788_p2877/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_bkg+"mc15_13TeV.361064.Sherpa_CT10_lllvSFMinus.merge.DAOD_SUSY15.e3836_s2608_s2183_r8788_p2877/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_bkg+"mc15_13TeV.361066.Sherpa_CT10_lllvSFPlus.merge.DAOD_SUSY15.e3836_s2608_s2183_r8788_p2877/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_bkg+"mc15_13TeV.361068.Sherpa_CT10_llvv.merge.DAOD_SUSY15.e3836_s2608_s2183_r8788_p2877/*.root*")

# ttbar
#svcMgr.EventSelector.InputCollections += glob.glob(dir_bkg+"mc15_13TeV.410252.Sherpa_221_NNPDF30NNLO_ttbar_dilepton_MEPS_NLO.merge.DAOD_SUSY15.e5450_s2726_r8788_p2877/*.root*")

# Ks particle gun
#svcMgr.EventSelector.InputCollections += glob.glob("/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/single_particle/user.sche.mc15_13TeV.422017.ParticleGunEvtGen_single_K0spipi_PtFlat4_10.DAOD_SUSY15.e4524_s2726_r7059_r6282.r1_EXT0/*.root*")



#--------------------------------------------
# SUSY dilepton samples
#--------------------------------------------
# samples with ctau = 100 mm
svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/SUSY/mc15_13TeV.402804.MGPy8EG_A14N23LO_SS_RPVDV_700_500_lam12k_100.merge.DAOD_SUSY15.e4332_s2608_s2183_r8788_p3187/*.root*')
svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/SUSY/mc15_13TeV.402810.MGPy8EG_A14N23LO_SS_RPVDV_700_50_lam12k_100.merge.DAOD_SUSY15.e4634_s2726_r8788_p3187/*.root*')
svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/SUSY/mc15_13TeV.402818.MGPy8EG_A14N23LO_SS_RPVDV_1600_1300_lam12k_100.merge.DAOD_SUSY15.e5539_s2726_r8788_p3187/*.root*')
svcMgr.EventSelector.InputCollections += glob.glob( '/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/SUSY/mc15_13TeV.402823.MGPy8EG_A14N23LO_SS_RPVDV_1600_50_lam12k_100.merge.DAOD_SUSY15.e5539_s2726_r8788_p3187/*.root*')




#--------------------------------------------
# All SUSY samples
#--------------------------------------------
dir_susy = "/n/atlas05/userdata/sche/MC15/DAOD_SUSY15/SUSY/"
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402803.MGPy8EG_A14N23LO_SS_RPVDV_700_500_lam12k_10.merge.DAOD_SUSY15.e4332_s2608_s2183_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402804.MGPy8EG_A14N23LO_SS_RPVDV_700_500_lam12k_100.merge.DAOD_SUSY15.e4332_s2608_s2183_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402805.MGPy8EG_A14N23LO_SS_RPVDV_700_500_lam12k_1000.merge.DAOD_SUSY15.e4332_s2608_s2183_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402809.MGPy8EG_A14N23LO_SS_RPVDV_700_50_lam12k_10.merge.DAOD_SUSY15.e4634_s2726_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402810.MGPy8EG_A14N23LO_SS_RPVDV_700_50_lam12k_100.merge.DAOD_SUSY15.e4634_s2726_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402811.MGPy8EG_A14N23LO_SS_RPVDV_700_50_lam12k_1000.merge.DAOD_SUSY15.e4634_s2726_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402812.MGPy8EG_A14N23LO_SS_RPVDV_700_500_lam12k_30.merge.DAOD_SUSY15.e5539_s2726_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402813.MGPy8EG_A14N23LO_SS_RPVDV_700_500_lam12k_300.merge.DAOD_SUSY15.e5539_s2726_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402814.MGPy8EG_A14N23LO_SS_RPVDV_700_50_lam12k_30.merge.DAOD_SUSY15.e5539_s2726_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402815.MGPy8EG_A14N23LO_SS_RPVDV_700_50_lam12k_300.merge.DAOD_SUSY15.e5539_s2726_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402816.MGPy8EG_A14N23LO_SS_RPVDV_1600_1300_lam12k_10.merge.DAOD_SUSY15.e5539_s2726_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402817.MGPy8EG_A14N23LO_SS_RPVDV_1600_1300_lam12k_30.merge.DAOD_SUSY15.e5539_s2726_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402818.MGPy8EG_A14N23LO_SS_RPVDV_1600_1300_lam12k_100.merge.DAOD_SUSY15.e5539_s2726_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402819.MGPy8EG_A14N23LO_SS_RPVDV_1600_1300_lam12k_300.merge.DAOD_SUSY15.e5539_s2726_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402820.MGPy8EG_A14N23LO_SS_RPVDV_1600_1300_lam12k_1000.merge.DAOD_SUSY15.e5539_s2726_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402821.MGPy8EG_A14N23LO_SS_RPVDV_1600_50_lam12k_10.merge.DAOD_SUSY15.e5539_s2726_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402822.MGPy8EG_A14N23LO_SS_RPVDV_1600_50_lam12k_30.merge.DAOD_SUSY15.e5539_s2726_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402823.MGPy8EG_A14N23LO_SS_RPVDV_1600_50_lam12k_100.merge.DAOD_SUSY15.e5539_s2726_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402824.MGPy8EG_A14N23LO_SS_RPVDV_1600_50_lam12k_300.merge.DAOD_SUSY15.e5539_s2726_r8788_p3187/*.root*")
#svcMgr.EventSelector.InputCollections += glob.glob(dir_susy+"mc15_13TeV.402825.MGPy8EG_A14N23LO_SS_RPVDV_1600_50_lam12k_1000.merge.DAOD_SUSY15.e5539_s2726_r8788_p3187/*.root*")


# Local SUSY15 test
#svcMgr.EventSelector.InputCollections = glob.glob("/n/atlas05/userdata/sche/20.7.8.7.SUSY15/run_p2877_local/DAOD_SUSY15.pool.root")

# handler for the main sequence
algseq = CfgMgr.AthSequencer("AthAlgSeq")

#--------------------------------------------
# Algorithms to run
#--------------------------------------------
from DisplacedDimuonAnalysis.DisplacedDimuonAnalysisConf import DisplacedDimuonAnalysisAlg

# Data and MC
algseq += CfgMgr.DisplacedDimuonAnalysisAlg()
#algseq += CfgMgr.LeptonCount()
#algseq += CfgMgr.TrackingSystematics()
#algseq += CfgMgr.CosmicBackground()
#algseq += CfgMgr.MuonPlots()
#algseq += CfgMgr.SUSYSelection()
#algseq += CfgMgr.LowMass()

# MC sample only
#algseq += CfgMgr.DVEfficiency()
#algseq += CfgMgr.EfficiencyStudy()
#algseq += CfgMgr.TruthPlots()
#algseq += CfgMgr.LeptonEfficiency()
#algseq += CfgMgr.VertexRes()

# LRT validation
#algseq += CfgMgr.LRTValidation()

# use pileup reweighting?
#usePRW = True
usePRW = False

#---------------------------------------------------------------
# including the tools /cuts needed by the algorithm, and configuring them
#---------------------------------------------------------------
svcMgr.MessageSvc.OutputLevel = INFO
#svcMgr.MessageSvc.OutputLevel = DEBUG
svcMgr.MessageSvc.defaultLimit = 999999

# histogram output service
if not hasattr(svcMgr, 'THistSvc'): svcMgr += CfgMgr.THistSvc()
svcMgr.THistSvc.Output += ["DV DATAFILE='output.root' OPT='RECREATE'"]
svcMgr.THistSvc.OutputLevel = INFO
#svcMgr.THistSvc.OutputLevel = DEBUG

# Efficiency
ToolSvc += CfgMgr.DVEfficiency("DVEfficiency",
                                 usePRW = usePRW)

# Tracking and vertexing systematics
ToolSvc += CfgMgr.TrackingSystematics("TrackingSystematics",
                                 usePRW = usePRW,
                                 usePV = True,
                                 useSV = True)
                                 
# Efficiency study
ToolSvc += CfgMgr.EfficiencyStudy("EfficiencyStudy",
                                 usePRW = usePRW)

# Truth plots
ToolSvc += CfgMgr.TruthPlots("TruthPlots",
                                 usePRW = usePRW)

# DVCuts tool
ToolSvc += CfgMgr.DDL__DVCuts("DiLepBaseCuts")
ToolSvc.DiLepBaseCuts.distMin  = 2. # mm
ToolSvc.DiLepBaseCuts.DisabledModuleMapFile = "DisabledModuleMap_Run2_v2.root"
ToolSvc.DiLepBaseCuts.MaterialMapFile = "materialMap3D_Run2_v2.1.1.root"


# DESD Filter
ToolSvc += CfgMgr.DDL__DiLepDESD("DiLepDESD",
                                 ReqTrigMatch = True,
                                 SiPhTrigger  = ["HLT_g140_loose"],
                                 DiPhTrigger  = ["HLT_2g50_loose"],
                                 SiMuTrigger  = ["HLT_mu60_0eta105_msonly"],
                                 ElEtaMax     = 2.5,
                                 PhEtaMax     = 2.5,
                                 MuEtaMax     = 2.5,
                                 MuBaEtaMax   = 1.07,
                                 ElD0Min      = 2.,
                                 MuD0Min      = 1.5,
                                 MuChi2DoF    = 5.,
                                 SiEGPtMin    = 150000.,
                                 SiPhXPtMin   =  10000.,
                                 SiMuPtMin    =  62000.,
                                 DiEGPtMin    =  55000.)

# Cosmic tool
ToolSvc += CfgMgr.DDL__DiLepCosmics("DiLepCosmics", RcosMax = 0.01)                                        

# GoodRunsListSelectorTool
vecStringGRL = 'data16_13TeV.periodAllYear_DetStatus-v83-pro20-15_DQDefects-00-02-04_PHYS_StandardGRL_All_Good_25ns_DAOD_RPVLL_r8669.xml'
ToolSvc += CfgMgr.GoodRunsListSelectionTool("GRLTool")
ToolSvc.GRLTool.GoodRunsListVec=[vecStringGRL]
ToolSvc.GRLTool.OutputLevel = INFO

# Muon selection tool
ToolSvc += CfgMgr.CP__MuonSelectionTool("MuonSelectionTool")
ToolSvc.MuonSelectionTool.MaxEta = 2.5
# 0 = tight, 1 = medium, 2 = loose, 3 = very loose
ToolSvc.MuonSelectionTool.MuQuality = 2
#ToolSvc.MuonSelectionTool.MuQuality = 3
ToolSvc.MuonSelectionTool.TurnOffMomCorr = True
ToolSvc.MuonSelectionTool.PixCutOff = True
ToolSvc.MuonSelectionTool.OutputLevel = INFO

# Muon correction tool
ToolSvc += CfgMgr.CP__MuonCalibrationAndSmearingTool("MuonCorrectionTool")
ToolSvc.MuonCorrectionTool.Year = "Data16"
ToolSvc.MuonCorrectionTool.Release = "Recs2016_15_07"
ToolSvc.MuonSelectionTool.TurnOffMomCorr = True
ToolSvc.MuonSelectionTool.PixCutOff = True
ToolSvc.MuonCorrectionTool.OutputLevel = INFO

# Electron Likelihood tool
#confDir = "ElectronPhotonSelectorTools/offline/mc15_20160512/"
ToolSvc += CfgMgr.AsgElectronLikelihoodTool("ElectronLikelihoodTool")
#ToolSvc.ElectronLikelihoodTool.ConfigFile= confDir+"ElectronLikelihoodLooseOfflineConfig2016_Smooth_NoD0.conf"
ToolSvc.ElectronLikelihoodTool.WorkingPoint= "LooseLHNod0Electron"
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
                                                #ConfigFiles = ["prw.mumu_noskim.m250t100.root"],
                                                #ConfigFiles = ["prw.mumu_noskim.m500t500.root"],
                                                #ConfigFiles = ["prw.mumu_noskim.m750t500.root"],
                                                ConfigFiles = ["prw.ee.m100t100.root"],
                                                #ConfigFiles = ["prw.susy.700_500.root"],
                                                #ConfigFiles = ["prw.llll.root"],
                                                #ConfigFiles = [""],
                                                #ConfigFiles = [""],
                                                LumiCalcFiles = ["ilumicalc_histograms_None_297730-311481_OflLumi-13TeV-009_DAOD_RPVLL_r8669.root"],
                                                TrigDecisionTool = "Trig::TrigDecisionTool/TrigDecisionTool",
                                                DataScaleFactor = 1. / 1.09,
                                                DataScaleFactorUP = 1.,
                                                DataScaleFactorDOWN = 1. / 1.18)
    
