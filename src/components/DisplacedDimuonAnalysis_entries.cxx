
#include "GaudiKernel/DeclareFactoryEntries.h"

#include "../DisplacedDimuonAnalysisAlg.h"
#include "../DisplacedDimuonAnalysis/DVUtils.h"
#include "../DisplacedDimuonAnalysis/LeptonSelectionTools.h"
#include "../DisplacedDimuonAnalysis/CosmicTools.h"

DECLARE_ALGORITHM_FACTORY( DisplacedDimuonAnalysisAlg )
DECLARE_TOOL_FACTORY( DVUtils )
DECLARE_TOOL_FACTORY( LeptonSelectionTools )
DECLARE_TOOL_FACTORY( CosmicTools )


#include "../MuonEfficiency.h"
DECLARE_ALGORITHM_FACTORY( MuonEfficiency )


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


#include "../SUSYDV.h"
DECLARE_ALGORITHM_FACTORY( SUSYDV )


#include "../FlipBkgEst.h"
DECLARE_ALGORITHM_FACTORY( FlipBkgEst )

DECLARE_FACTORY_ENTRIES( DisplacedDimuonAnalysis ) 
{
  DECLARE_ALGORITHM( FlipBkgEst );
  DECLARE_ALGORITHM( SUSYDV );
  DECLARE_ALGORITHM( MuonPlots );
  DECLARE_ALGORITHM( LumiCalc );
  DECLARE_ALGORITHM( TruthPlots );
  DECLARE_ALGORITHM( VertexRes );
  DECLARE_ALGORITHM( LRTValidation );
  DECLARE_ALGORITHM( DVEfficiency );
  DECLARE_ALGORITHM( MuonEfficiency );
  DECLARE_ALGORITHM( DisplacedDimuonAnalysisAlg )
  DECLARE_TOOL( DVUtils )
}
