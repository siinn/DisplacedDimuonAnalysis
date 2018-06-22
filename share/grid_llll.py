#!/bin/sh
pathena analysis_jobOption.py \
--mergeOutput \
--nFilesPerJob 10 \
--destSE BNL-OSG2_SCRATCHDISK \
--inDS mc15_13TeV.361063.Sherpa_CT10_llll.merge.DAOD_SUSY15.e3836_s2608_s2183_r8788_p2877 \
--outDS user.sche.mc15_13TeV.llll.Ks.NewCuts.prw.r1
