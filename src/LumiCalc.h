#ifndef DISPLACEDDIMUONANALYSIS_LUMICALC_H
#define DISPLACEDDIMUONANALYSIS_LUMICALC_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"



class LumiCalc: public ::AthAnalysisAlgorithm { 

    public: 

       LumiCalc( const std::string& name, ISvcLocator* pSvcLocator );
       virtual ~LumiCalc(); 
    
       virtual StatusCode  initialize();
       virtual StatusCode  execute();
       virtual StatusCode  finalize();
       
       virtual StatusCode beginInputFile();
    
    private: 

}; 

#endif //> !DISPLACEDDIMUONANALYSIS_LUMICALC_H
