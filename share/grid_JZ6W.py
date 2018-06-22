#!/bin/sh
pathena analysis_jobOption.py \
--mergeOutput \
--nFilesPerJob 10 \
--destSE BNL-OSG2_SCRATCHDISK \
--inDS mc15_13TeV.361026.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ6W.merge.DAOD_SUSY15.e3569_s2608_s2183_r8788_p2877 \
--outDS user.sche.mc15_13TeV.jetjet_JZ6W.Ks.NewCuts.r6
