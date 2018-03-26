#ifndef DISPLACEDDIMUONANALYSIS_EFFICIENCYSTUDY_H
#define DISPLACEDDIMUONANALYSIS_EFFICIENCYSTUDY_H 1

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

class EfficiencyStudy: public ::AthAnalysisAlgorithm { 
    public: 
        EfficiencyStudy( const std::string& name, ISvcLocator* pSvcLocator );
        virtual ~EfficiencyStudy(); 
        
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

        // DV mass accessor
        SG::AuxElement::ConstAccessor<float> m_accMass;
        SG::AuxElement::Accessor<std::shared_ptr<xAOD::ElectronContainer>> m_accEl;
        SG::AuxElement::Accessor<std::shared_ptr<xAOD::MuonContainer>> m_accMu;

        // use PRW?
        bool m_usePRW = false;

        // number of events processed
        int n_events = 0;

        // pileup weight
        float p_weight = 1;

        // cut flow
        TH1D* m_dv_reco_cutflow; //!

        // efficiency for different eta
        TEfficiency* m_dv_eff_pt_eta1_mass1; //! 
        TEfficiency* m_dv_eff_pt_eta2_mass1; //! 
        TEfficiency* m_dv_eff_pt_eta3_mass1; //! 
        TEfficiency* m_dv_eff_pt_eta1_mass2; //! 
        TEfficiency* m_dv_eff_pt_eta2_mass2; //! 
        TEfficiency* m_dv_eff_pt_eta3_mass2; //! 
        TEfficiency* m_dv_eff_pt_eta1_mass3; //! 
        TEfficiency* m_dv_eff_pt_eta2_mass3; //! 
        TEfficiency* m_dv_eff_pt_eta3_mass3; //! 
        TEfficiency* m_dv_eff_pt_eta1_mass4; //! 
        TEfficiency* m_dv_eff_pt_eta2_mass4; //! 
        TEfficiency* m_dv_eff_pt_eta3_mass4; //! 
        TEfficiency* m_dv_eff_pt_eta1_mass5; //! 
        TEfficiency* m_dv_eff_pt_eta2_mass5; //! 
        TEfficiency* m_dv_eff_pt_eta3_mass5; //! 

        // efficiency in DeltaR
        TEfficiency* m_dv_eff_deltaR; //! 
        TH2D* m_truth_yields_map_pt_deltaR; //! 
        TH2D* m_reco_yields_map_pt_deltaR; //! 

        // truth vertex yields at mass peak
        TH2D* m_truth_yields_map_pt_eta_1; //! 
        TH2D* m_truth_yields_map_pt_eta_2; //! 
        TH2D* m_truth_yields_map_pt_eta_3; //! 
        TH2D* m_truth_yields_map_pt_eta_4; //! 
        TH2D* m_truth_yields_map_pt_eta_5; //! 

        // reco vertex yields at mass peak
        TH2D* m_reco_yields_map_pt_eta_1; //! 
        TH2D* m_reco_yields_map_pt_eta_2; //! 
        TH2D* m_reco_yields_map_pt_eta_3; //! 
        TH2D* m_reco_yields_map_pt_eta_4; //! 
        TH2D* m_reco_yields_map_pt_eta_5; //! 

        // truth distribution of long-lived particle
        TH1F* m_truth_m; //!
        TH1F* m_truth_DeltaR; //!

        // weight distribution to investigate discrepancy in specific bins
        TH1F* m_weight1; //!
        TH1F* m_weight2; //!
        TH1F* m_weight3; //!

        // \DeltaR distribution to investigate discrepancy in specific bins
        TH1F* m_DeltaR1; //!
        TH1F* m_DeltaR2; //!
        TH1F* m_DeltaR3; //!
        
        // lepton pt distribution to investigate discrepancy in specific bins
        TH1F* m_lepton_eta1; //!
        TH1F* m_lepton_eta2; //!
        TH1F* m_lepton_eta3; //!

    
}; 

#endif //> !DISPLACEDDIMUONANALYSIS_EFFICIENCYSTUDY_H
