#!/bin/sh
pathena analysis_jobOption.py \
--mergeOutput \
--destSE BNL-OSG2_SCRATCHDISK \
--inDS \
"mc15_13TeV.361066.Sherpa_CT10_lllvSFPlus.merge.DAOD_SUSY15.e3836_s2608_s2183_r8788_p2877, \
mc15_13TeV.361064.Sherpa_CT10_lllvSFMinus.merge.DAOD_SUSY15.e3836_s2608_s2183_r8788_p2877, \
mc15_13TeV.361068.Sherpa_CT10_llvv.merge.DAOD_SUSY15.e3836_s2608_s2183_r8788_p2877, \
mc15_13TeV.361063.Sherpa_CT10_llll.merge.DAOD_SUSY15.e3836_s2608_s2183_r8788_p2877" \
--outDS user.sche.mc15_13TeV.36106x.syst.combinedLep.MC.NewCuts.unskimmedSUSY15.r9
