#ifndef DISPLACEDDIMUONANALYSIS_MUONEFFICIENCY_H
#define DISPLACEDDIMUONANALYSIS_MUONEFFICIENCY_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"



class MuonEfficiency: public ::AthAnalysisAlgorithm { 
 public: 
  MuonEfficiency( const std::string& name, ISvcLocator* pSvcLocator );
  virtual ~MuonEfficiency(); 

  virtual StatusCode  initialize();
  virtual StatusCode  execute();
  virtual StatusCode  finalize();
  
  virtual StatusCode beginInputFile();

 private: 

}; 

#endif //> !DISPLACEDDIMUONANALYSIS_MUONEFFICIENCY_H
