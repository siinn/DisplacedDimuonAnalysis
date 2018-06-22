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
#include "DDLBase/IPhotonMatch.h"
#include "DDLBase/IVxWeights.h"

// DVUtil
#include "DisplacedDimuonAnalysis/DVUtils.h"
#include "DisplacedDimuonAnalysis/LeptonSelectionTools.h"
#include "DisplacedDimuonAnalysis/CosmicTools.h"

// GRL
//#include "GoodRunsLists/IGoodRunsListSelectionTool.h"
#include "AsgAnalysisInterfaces/IGoodRunsListSelectionTool.h"

// Trigger decision tool
#include "TrigDecisionTool/TrigDecisionTool.h"

// TEfficiency
#include "TEfficiency.h"

// pile-up reweighting
#include "AsgAnalysisInterfaces/IPileupReweightingTool.h"

class DVEfficiency: public ::AthAnalysisAlgorithm { 
    public: 
        DVEfficiency( const std::string& name, ISvcLocator* pSvcLocator );
        virtual ~DVEfficiency(); 
        
        virtual StatusCode  initialize();
        virtual StatusCode  execute();
        virtual StatusCode  finalize();
        
        virtual bool PassCosmicVeto_R_CR(xAOD::TrackParticle& tr1, xAOD::TrackParticle& tr2);
        
        
        
    private: 

        // setting tools
        ToolHandle<DDL::IEventCuts> m_evtc; //!
        ToolHandle<DDL::IDiLepDVCuts> m_dilepdvc;
        ToolHandle<DDL::IDVCuts> m_dvc;
        ToolHandle<DDL::IDiLepCosmics> m_cos; //!
        ToolHandle<IGoodRunsListSelectionTool> m_grlTool; //!
        ToolHandle<Trig::ITrigDecisionTool> m_tdt; //!
        ToolHandle<IDVUtils> m_dvutils; //!
        ToolHandle<ILeptonSelectionTools> m_leptool; //!
        ToolHandle<ICosmicTools> m_costool; //!
        ToolHandle<DDL::IOverlapRemoval> m_or; //!
        ToolHandle<CP::IPileupReweightingTool> m_prw; //!
        ToolHandle<DDL::IPhotonMatch> m_phmatch;
        ToolHandle<DDL::IVxWeights> m_vxwght; //!

        // DV mass accessor
        SG::AuxElement::ConstAccessor<float> m_accMass;
        SG::AuxElement::Accessor<std::shared_ptr<xAOD::ElectronContainer>> m_accEl;
        SG::AuxElement::Accessor<std::shared_ptr<xAOD::MuonContainer>> m_accMu;

        // use PRW?
        bool m_usePRW;

        // number of events processed
        int n_events = 0;

        // pileup weight
        float p_weight = 1;
        float p_weight_up = 1;
        float p_weight_down = 1;

        // cut flow
        TH1D* m_dv_eff_cutflow; //!

        // efficiency as a function of track parameters
        TH1F* m_dv_eff_eta; //! 
        TH1F* m_dv_eff_phi; //! 
        TH1F* m_dv_eff_mass; //! 
        TH1F* m_dv_eff_R; //! 
        TH1F* m_dv_eff_z; //! 
        TH1F* m_dv_eff_d0; //! 
        TH1F* m_dv_eff_DeltaR; //! 
        
        TH1F* m_dv_eff_eta_den; //! 
        TH1F* m_dv_eff_phi_den; //! 
        TH1F* m_dv_eff_mass_den; //! 
        TH1F* m_dv_eff_R_den; //! 
        TH1F* m_dv_eff_z_den; //! 
        TH1F* m_dv_eff_d0_den; //! 
        TH1F* m_dv_eff_DeltaR_den; //! 

        TH1F* m_dv_eff_eta_num; //! 
        TH1F* m_dv_eff_phi_num; //! 
        TH1F* m_dv_eff_mass_num; //! 
        TH1F* m_dv_eff_R_num; //! 
        TH1F* m_dv_eff_z_num; //! 
        TH1F* m_dv_eff_d0_num; //! 
        TH1F* m_dv_eff_DeltaR_num; //! 
        
        // efficiency as a function of Z' parameters
        TH1F* m_dv_eff_zp_eta; //! 
        TH1F* m_dv_eff_zp_beta; //! 
        TH1F* m_dv_eff_zp_pt; //! 

        TH1F* m_dv_eff_zp_eta_den; //! 
        TH1F* m_dv_eff_zp_beta_den; //! 
        TH1F* m_dv_eff_zp_pt_den; //! 
        
        TH1F* m_dv_eff_zp_eta_num; //! 
        TH1F* m_dv_eff_zp_beta_num; //! 
        TH1F* m_dv_eff_zp_pt_num; //! 

        // efficiency as a function of pile-up
        TH1F* m_dv_eff_mu; //! 
        TH1F* m_dv_eff_mu_den; //! 
        TH1F* m_dv_eff_mu_num; //! 

        // trigger efficiency
        TH1F* m_dv_eff_trig; //! 
        TH1F* m_dv_eff_trig_den; //! 
        TH1F* m_dv_eff_trig_num; //! 

        // efficiency map (eta vs mu) for reweighting
        TH2F* m_dv_eff_map_mu_eta_num; //! 
        TH2F* m_dv_eff_map_mu_eta_den; //! 
        TH2F* m_dv_eff_map_mu_eta; //! 

        // efficiency map
        TH2F* m_dv_eff_map_pt_eta_num; //! 
        TH2F* m_dv_eff_map_pt_eta_den; //! 
        TH2F* m_dv_eff_map_pt_eta; //! 

        // efficiency map (pile up systematic variation, Up)
        TH2F* m_dv_eff_map_up_pt_eta_num; //! 
        TH2F* m_dv_eff_map_up_pt_eta_den; //! 
        TH2F* m_dv_eff_map_up_pt_eta; //! 

        // efficiency map (pile up systematic variation, Down)
        TH2F* m_dv_eff_map_down_pt_eta_num; //! 
        TH2F* m_dv_eff_map_down_pt_eta_den; //! 
        TH2F* m_dv_eff_map_down_pt_eta; //! 

        // invariant mass of all signal truth vertex
        TH1F* m_dv_mass; //!
    
}; 

#endif //> !DISPLACEDDIMUONANALYSIS_DVEFFICIENCY_H
