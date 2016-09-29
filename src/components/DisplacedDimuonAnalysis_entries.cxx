
#include "GaudiKernel/DeclareFactoryEntries.h"

#include "../DisplacedDimuonAnalysisAlg.h"
#include "../DisplacedDimuonAnalysis/DVUtils.h"

DECLARE_ALGORITHM_FACTORY( DisplacedDimuonAnalysisAlg )
DECLARE_TOOL_FACTORY( DVUtils )


#include "../MuonEfficiency.h"
DECLARE_ALGORITHM_FACTORY( MuonEfficiency )

DECLARE_FACTORY_ENTRIES( DisplacedDimuonAnalysis ) 
{
  DECLARE_ALGORITHM( MuonEfficiency );
  DECLARE_ALGORITHM( DisplacedDimuonAnalysisAlg )
  DECLARE_TOOL( DVUtils )
}
