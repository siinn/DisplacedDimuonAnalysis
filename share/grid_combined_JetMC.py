#!/bin/sh
pathena analysis_jobOption.py \
--mergeOutput \
--destSE BNL-OSG2_SCRATCHDISK \
--inDS mc15_13TeV.36102*.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ*W.merge.DAOD_SUSY15.*_r8788_p2877 \
--outDS user.sche.mc15_13TeV.36102x.syst.combinedJet.NewCuts.MC.NoTrgFil.r10
