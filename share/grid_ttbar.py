#!/bin/sh
pathena analysis_jobOption.py \
--mergeOutput \
--nFilesPerJob 10 \
--destSE BNL-OSG2_SCRATCHDISK \
--inDS mc15_13TeV.410252.Sherpa_221_NNPDF30NNLO_ttbar_dilepton_MEPS_NLO.merge.DAOD_SUSY15.e5450_s2726_r8788_p2877 \
--outDS user.sche.mc15_13TeV.410252.Sherpa_ttbar_dilepton_MEPS_NLO.syst.NewCuts.prw.r5
