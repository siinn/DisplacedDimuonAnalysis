#!/bin/sh
pathena grid_jobOption.py --mergeOutput --destSE BNL-OSG2_USERDISK --inDS "data16_13TeV.*.physics_Main.merge.DAOD_SUSY15.*r8669_p2875*" --outDS user.sche.data16_13TeV.physics_Main.merge.DAOD_SUSY15.m1616_r8669_p2875.r18

pathena analysis_jobOption.py --mergeOutput --destSE BNL-OSG2_USERDISK --inDS user.sche.data16_13TeV.physics_Main.DAOD_RPVLL.r8669.trigfilter.r1_EXT0/ --outDS user.sche.data16_13TeV.physics_Main.merge.DAOD_SUSY15.r8669.r18.trig.filtered

