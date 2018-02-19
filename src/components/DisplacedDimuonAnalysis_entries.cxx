
#include "GaudiKernel/DeclareFactoryEntries.h"

#include "../DisplacedDimuonAnalysisAlg.h"
#include "../SUSYSelection.h"
#include "../LowMass.h"
#include "../DisplacedDimuonAnalysis/DVUtils.h"
#include "../DisplacedDimuonAnalysis/LeptonSelectionTools.h"
#include "../DisplacedDimuonAnalysis/CosmicTools.h"
#include "../DisplacedDimuonAnalysis/FilterMatchingTools.h"

DECLARE_ALGORITHM_FACTORY( DisplacedDimuonAnalysisAlg )
DECLARE_ALGORITHM_FACTORY( SUSYSelection )
DECLARE_ALGORITHM_FACTORY( LowMass )
DECLARE_TOOL_FACTORY( DVUtils )
DECLARE_TOOL_FACTORY( LeptonSelectionTools )
DECLARE_TOOL_FACTORY( CosmicTools )
DECLARE_TOOL_FACTORY( FilterMatchingTools )


#include "../LeptonEfficiency.h"
DECLARE_ALGORITHM_FACTORY( LeptonEfficiency )


#include "../DVEfficiency.h"
DECLARE_ALGORITHM_FACTORY( DVEfficiency )


#include "../LRTValidation.h"
DECLARE_ALGORITHM_FACTORY( LRTValidation )


#include "../VertexRes.h"
DECLARE_ALGORITHM_FACTORY( VertexRes )


#include "../TruthPlots.h"
DECLARE_ALGORITHM_FACTORY( TruthPlots )

#include "../MuonPlots.h"
DECLARE_ALGORITHM_FACTORY( MuonPlots )

#include "../TrackingSystematics.h"
DECLARE_ALGORITHM_FACTORY( TrackingSystematics )

#include "../CosmicBackground.h"
DECLARE_ALGORITHM_FACTORY( CosmicBackground )


DECLARE_FACTORY_ENTRIES( DisplacedDimuonAnalysis ) 
{
  DECLARE_ALGORITHM( MuonPlots );
  DECLARE_ALGORITHM( TruthPlots );
  DECLARE_ALGORITHM( VertexRes );
  DECLARE_ALGORITHM( LRTValidation );
  DECLARE_ALGORITHM( DVEfficiency );
  DECLARE_ALGORITHM( LeptonEfficiency );
  DECLARE_ALGORITHM( DisplacedDimuonAnalysisAlg )
  DECLARE_ALGORITHM( SUSYSelection )
  DECLARE_ALGORITHM( LowMass )
  DECLARE_ALGORITHM( TrackingSystematics )
  DECLARE_ALGORITHM( CosmicBackground )
  DECLARE_TOOL( DVUtils )
  DECLARE_TOOL( LeptonSelectionTools )
  DECLARE_TOOL( CosmicTools )
  DECLARE_TOOL( FilterMatchingTools );
}
