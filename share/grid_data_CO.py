#!/bin/sh
pathena analysis_jobOption.py \
--mergeOutput \
--destSE BNL-OSG2_SCRATCHDISK \
--nFilesPerJob 5 \
--forceStaged \
--inDS user.cohm.data16_13TeV.DAOD_SUSY15.r8669_p2988 \
--outDS user.sche.data16_13TeV.DAOD_SUSY15.r8669.p2988.CosmicBkg.r11
