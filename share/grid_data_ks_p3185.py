#!/bin/sh
pathena analysis_jobOption.py \
--mergeOutput \
--destSE BNL-OSG2_SCRATCHDISK \
--nEventsPerJob 20000 \
--forceStaged \
--inDS user.dkrauss.data16_13TeV.DAOD_SUSY15.r8669_p3185 \
--outDS user.sche.data16_13TeV.DAOD_SUSY15.r8669.p3185.Main.Ks.p3185.r1
