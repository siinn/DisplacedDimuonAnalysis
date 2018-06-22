#!/bin/sh
pathena analysis_jobOption.py \
--mergeOutput \
--nFilesPerJob 10 \
--destSE BNL-OSG2_SCRATCHDISK \
--inDS mc15_13TeV.361025.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ5W.merge.DAOD_SUSY15.e3668_s2576_s2132_r8788_p2877 \
--outDS user.sche.mc15_13TeV.361025.jetjet_JZ5W.Ks.NewCuts.prw.r6
