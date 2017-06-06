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
#include "DDLBase/IEventCuts.h"
#include "DDLBase/IDVCuts.h"
#include "DDLBase/IDiLepDVCuts.h"
#include "DDLBase/IDiLepCosmics.h"
#include "DDLBase/IOverlapRemoval.h"

// DVUtil
#include "DisplacedDimuonAnalysis/DVUtils.h"
#include "DisplacedDimuonAnalysis/LeptonSelectionTools.h"
#include "DisplacedDimuonAnalysis/CosmicTools.h"

// GRL
#include "GoodRunsLists/IGoodRunsListSelectionTool.h"

// Trigger decision tool
#include "TrigDecisionTool/TrigDecisionTool.h"

// TEfficiency
#include "TEfficiency.h"

class DVEfficiency: public ::AthAnalysisAlgorithm { 
    public: 
        DVEfficiency( const std::string& name, ISvcLocator* pSvcLocator );
        virtual ~DVEfficiency(); 
        
        virtual StatusCode  initialize();
        virtual StatusCode  execute();
        virtual StatusCode  finalize();
        
        virtual bool PassCosmicVeto(const DataVector<xAOD::Muon> dv_muc, const DataVector<xAOD::Electron> dv_elc, std::string channel);
        
        
        
    private: 
        // tool for muon matching to dv
        ToolHandle<DDL::IEventCuts> m_evtc; //!
        ToolHandle<DDL::IDiLepDVCuts> m_dilepdvc;
        ToolHandle<DDL::IDVCuts> m_dvc;
        ToolHandle<DDL::IDiLepCosmics> m_cos; //!
        ToolHandle<IGoodRunsListSelectionTool> m_grlTool; //!
        ToolHandle<Trig::TrigDecisionTool> m_tdt; //!
        ToolHandle<IDVUtils> m_dvutils; //!
        ToolHandle<ILeptonSelectionTools> m_leptool; //!
        ToolHandle<ICosmicTools> m_costool; //!
        ToolHandle<DDL::IOverlapRemoval> m_or;
        
        // DV mass accessor
        SG::AuxElement::ConstAccessor<float> m_accMass;
        SG::AuxElement::Accessor<std::shared_ptr<xAOD::ElectronContainer>> m_accEl;
        SG::AuxElement::Accessor<std::shared_ptr<xAOD::MuonContainer>> m_accMu;

        // efficiency as a function of track parameters
        TEfficiency* m_dv_eff_eta; //! 
        TEfficiency* m_dv_eff_phi; //! 
        TEfficiency* m_dv_eff_mass; //! 
        TEfficiency* m_dv_eff_R; //! 
        TEfficiency* m_dv_eff_d0; //! 
        
        // efficiency as a function of Z' parameters
        TEfficiency* m_dv_eff_zp_eta; //! 
        TEfficiency* m_dv_eff_zp_beta; //! 
        TEfficiency* m_dv_eff_zp_pt; //! 

        // efficiency map
        TH2F* m_dv_eff_map_pt_eta_num; //! 
        TH2F* m_dv_eff_map_pt_eta_den; //! 
        TH2F* m_dv_eff_map_pt_eta; //! 
        //TEfficiency* m_dv_eff_map_pt_eta; //! 
        
        // invariant mass of all signal truth vertex
        TH1F* m_dv_mass; //!

        // difference between truth and reco DV
        TH1F* m_dv_R_err_tight; //!
        TH1F* m_dv_z_err_tight; //!
        TH1F* m_dv_m_err_tight; //!
    
        TH1F* m_dv_R_err_loose; //!
        TH1F* m_dv_z_err_loose; //!
        TH1F* m_dv_m_err_loose; //!
    
}; 

#endif //> !DISPLACEDDIMUONANALYSIS_DVEFFICIENCY_H
