#!/bin/sh
pathena analysis_jobOption.py \
--mergeOutput \
--destSE BNL-OSG2_SCRATCHDISK \
--nEventsPerFile 5000 \
--forceStaged \
--inDS data16_13TeV.00*.physics_Main.merge.DAOD_SUSY15.*_r8669_p3185 \
--outDS user.sche.data16_13TeV.DAOD_SUSY15.r8669.p2988.Main.LowMass.relaxedcut.r10
