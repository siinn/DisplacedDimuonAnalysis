#!/bin/sh
pathena analysis_jobOption.py \
--mergeOutput \
--nFilesPerJob 10 \
--destSE BNL-OSG2_SCRATCHDISK \
--inDS mc15_13TeV.361068.Sherpa_CT10_llvv.merge.DAOD_SUSY15.e3836_s2608_s2183_r8788_p2877 \
--outDS user.sche.mc15_13TeV.llvv.Ks.NewCuts.prw.r1
