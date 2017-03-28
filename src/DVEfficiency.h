#ifndef DISPLACEDDIMUONANALYSIS_DVEFFICIENCY_H
#define DISPLACEDDIMUONANALYSIS_DVEFFICIENCY_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"

// STL
#include <map>
#include <string>
#include <vector>

// EDM
#include "AsgTools/ToolHandle.h"

// DV
#include "DVAnalyses/AlgBase.h"
#include "DDLBase/IEventCuts.h"
#include "DDLBase/IDVCuts.h"
#include "DDLBase/IDiLepDVCuts.h"
#include "DDLBase/IDiLepCosmics.h"
#include "DDLBase/IOverlapRemoval.h"

// DVUtil
#include "DisplacedDimuonAnalysis/DVUtils.h"

// GRL
#include "GoodRunsLists/IGoodRunsListSelectionTool.h"

// Trigger decision tool
#include "TrigDecisionTool/TrigDecisionTool.h"

class DVEfficiency: public ::AthAnalysisAlgorithm { 
    public: 
        DVEfficiency( const std::string& name, ISvcLocator* pSvcLocator );
        virtual ~DVEfficiency(); 
        
        virtual StatusCode  initialize();
        virtual StatusCode  execute();
        virtual StatusCode  finalize();
        
        virtual bool PassCosmicVeto(const DataVector<xAOD::Muon> dv_muc);
        
        
        
    private: 
        // tool for muon matching to dv
        ToolHandle<DDL::IEventCuts> m_evtc; //!
        ToolHandle<DDL::IDiLepDVCuts> m_dilepdvc;
        ToolHandle<DDL::IDVCuts> m_dvc;
        ToolHandle<DDL::IDiLepCosmics> m_cos; //!
        ToolHandle<IGoodRunsListSelectionTool> m_grlTool; //!
        ToolHandle<Trig::TrigDecisionTool> m_tdt; //!
        ToolHandle<IDVUtils> m_dvutils; //!
        
        // Handle to the tool
        ToolHandle<DDL::IOverlapRemoval> m_or;
        
        // DV mass accessor
        SG::AuxElement::ConstAccessor<float> m_accMass;
        
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
