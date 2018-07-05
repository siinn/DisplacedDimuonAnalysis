# DisplacedDimuonAnalysis

Analysis package for displaced dilepton analysis. The package contains the following algorithms and the corresponding jobOptions.

- Main analysis code for dilepton search
- Generator-level analysis
- Signal efficiency of MC samples
- Background estimation for random-crossing using Track-flipping method
- Systematic uncertainty in track and vertex reconstruction

In addition, the analysis package also performs various tasks, including pile-up reweighting.

## Quick Instruction
You need T3 clusters or machine with ATLAS software in order to run this package. Here is a quick start guide for running the algorithm on data.

~~~~
setupATLAS
asetup AthAnalysisBase,2.4.42, here // or use the latest AnalysisBase
athena analysis_jobOption.py
~~~~

In the jobOption, you can change the local input files. Also, uncomment algorithms that you want to run from below.
~~~~
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
~~~~

Please contact me for more information.
