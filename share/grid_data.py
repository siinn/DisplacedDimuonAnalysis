#!/bin/sh
pathena analysis_jobOption.py \
--mergeOutput \
--destSE BNL-OSG2_SCRATCHDISK \
--nFilesPerJob 20 \
--forceStaged \
--inDS user.sche.data16_13TeV.physics_Main.DAOD_SUSY15.r8669_p2950.Lep_Filtered.r10_EXT0/ \
--outDS user.sche.data16_13TeV.DAOD_SUSY15.r8669.p2988.Main.LowMass.relaxedcut.r10
