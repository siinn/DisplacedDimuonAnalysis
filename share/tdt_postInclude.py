## postInlcude for Reco_trf.py
## Apply event-filters to select events passing specified trigger

from AthenaCommon.AlgSequence import AlgSequence, AthSequencer
job = AlgSequence()

## Sequencer configuration ##
seq = AthSequencer("AthFilterSeq")

from TrigAnalysisExamples.Tdt_DV import TriggerTree
TriggerTree_DV = TriggerTree( "TriggerDecisionTool_DV",
                                trigger_list=[] )
seq += TriggerTree_DV

#svcMgr.MessageSvc.OutputLevel = Lvl.ERROR
svcMgr.MessageSvc.OutputLevel = Lvl.INFO
