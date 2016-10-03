
#include "GaudiKernel/DeclareFactoryEntries.h"

#include "../DisplacedDimuonAnalysisAlg.h"
#include "../DisplacedDimuonAnalysis/DVUtils.h"

DECLARE_ALGORITHM_FACTORY( DisplacedDimuonAnalysisAlg )
DECLARE_TOOL_FACTORY( DVUtils )


#include "../MuonEfficiency.h"
DECLARE_ALGORITHM_FACTORY( MuonEfficiency )


#include "../DVEfficiency.h"
DECLARE_ALGORITHM_FACTORY( DVEfficiency )

DECLARE_FACTORY_ENTRIES( DisplacedDimuonAnalysis ) 
{
  DECLARE_ALGORITHM( DVEfficiency );
  DECLARE_ALGORITHM( MuonEfficiency );
  DECLARE_ALGORITHM( DisplacedDimuonAnalysisAlg )
  DECLARE_TOOL( DVUtils )
}
