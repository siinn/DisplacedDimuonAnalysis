#ifndef DISPLACEDDIMUONANALYSIS_DVEFFICIENCY_H
#define DISPLACEDDIMUONANALYSIS_DVEFFICIENCY_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"
#include "DVCuts/IDiLepDVCuts.h"

// DVUtil
#include "DisplacedDimuonAnalysis/DVUtils.h"



class DVEfficiency: public ::AthAnalysisAlgorithm { 
 public: 
  DVEfficiency( const std::string& name, ISvcLocator* pSvcLocator );
  virtual ~DVEfficiency(); 

  virtual StatusCode  initialize();
  virtual StatusCode  execute();
  virtual StatusCode  finalize();

  int n_truth_siganl_dv;
  int n_matched_signal_dv;
  

 private: 
        ToolHandle<DV::IDiLepDVCuts> m_dilepdvc; //!
        ToolHandle<IDVUtils>  m_dvutils; //!

        TH1D* m_dv_cutflow; //!

        TProfile* m_dv_eff_eta; //! 
        TProfile* m_dv_eff_phi; //! 
        TProfile* m_dv_eff_mass; //! 
        TProfile* m_dv_eff_R; //! 

        int n_accepted; //!

}; 

#endif //> !DISPLACEDDIMUONANALYSIS_DVEFFICIENCY_H
