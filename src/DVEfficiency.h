#ifndef DISPLACEDDIMUONANALYSIS_DVEFFICIENCY_H
#define DISPLACEDDIMUONANALYSIS_DVEFFICIENCY_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"
#include "DDLBase/IDiLepDVCuts.h"
#include "DDLBase/IDVCuts.h"

// DVUtil
#include "DisplacedDimuonAnalysis/DVUtils.h"



class DVEfficiency: public ::AthAnalysisAlgorithm { 
 public: 
  DVEfficiency( const std::string& name, ISvcLocator* pSvcLocator );
  virtual ~DVEfficiency(); 

  virtual StatusCode  initialize();
  virtual StatusCode  execute();
  virtual StatusCode  finalize();

  

 private: 
        ToolHandle<IDVUtils>  m_dvutils; //!
        ToolHandle<DDL::IDVCuts> m_dvc; //!

        // DV mass accessor
        SG::AuxElement::ConstAccessor<float> m_accMass;

        TH1D* m_dv_cutflow; //!

        TProfile* m_dv_eff_eta; //! 
        TProfile* m_dv_eff_phi; //! 
        TProfile* m_dv_eff_mass; //! 
        TProfile* m_dv_eff_R; //! 
        TProfile* m_dv_eff_d0; //! 

        // efficiency as a function of Z' parameters
        TProfile* m_dv_eff_zp_eta; //! 
        TProfile* m_dv_eff_zp_pt; //! 

        // invariant mass of all signal truth vertex
        TH1F* m_dv_mass; //!


}; 

#endif //> !DISPLACEDDIMUONANALYSIS_DVEFFICIENCY_H
