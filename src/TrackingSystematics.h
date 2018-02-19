#ifndef DISPLACEDDIMUONANALYSIS_TRACKINGSYSTEMATICS_H
#define DISPLACEDDIMUONANALYSIS_TRACKINGSYSTEMATICS_H 1

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
#include "TriggerMatchingTool/IMatchingTool.h"


class TrackingSystematics: public ::AthAnalysisAlgorithm { 
    public: 
        TrackingSystematics( const std::string& name, ISvcLocator* pSvcLocator );
        virtual ~TrackingSystematics(); 
        virtual StatusCode  initialize();     //once, before any input is loaded
        virtual StatusCode  beginInputFile(); //start of each input file, only metadata loaded
        virtual StatusCode  execute();        //per event
        virtual StatusCode  finalize();       //once, after all events processed

        virtual bool PassCosmicVeto_R_CR(xAOD::TrackParticle& tr1, xAOD::TrackParticle& tr2);
        virtual bool PassKsSelection(const xAOD::Vertex& dv, const xAOD::Vertex& pv);

    
    private: 

        // setting tools
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
        ToolHandle<Trig::IMatchingTool> m_tmt; //!
        ToolHandle<DDL::IPhotonMatch> m_phmatch;

        // DV mass accessor
        SG::AuxElement::ConstAccessor<float> m_accMass;
        SG::AuxElement::ConstAccessor<float> m_acc_pt;
        SG::AuxElement::Accessor<std::shared_ptr<xAOD::ElectronContainer>> m_accEl;
        SG::AuxElement::Accessor<std::shared_ptr<xAOD::MuonContainer>> m_accMu;

        // output
        TH1D* m_event_cutflow; //!

        // Ks candidate from VrtSecInclusive
        TH1D* m_dv_idid_cf; //!
        TH1F* m_dv_idid_M; //!
        TH1F* m_dv_idid_R; //!
        TH1F* m_dv_idid_z; //!
        TH1F* m_dv_idid_l; //!
        TH1F* m_dv_idid_pt; //!
        TH1F* m_dv_idid_mu; //!
        TH1F* m_dv_idid_DeltaR; //!
        TH2F* m_dv_idid_R_M; //!

        // mass plot in bins of R
        TH1F* m_dv_idid_M_1; //!
        TH1F* m_dv_idid_M_2; //!
        TH1F* m_dv_idid_M_3; //!
        TH1F* m_dv_idid_M_4; //!
        TH1F* m_dv_idid_M_5; //!
        TH1F* m_dv_idid_M_6; //!
        TH1F* m_dv_idid_M_7; //!
        TH1F* m_dv_idid_M_8; //!

        // ratio plot (Ri / R0)
        TH1F* m_dv_idid_ratio_R; //!


        // truth-matched Ks, Z' comparison
        TH1F* m_Ks_pt; //!
        TH1F* m_Ks_eta; //!
        TH1F* m_Ks_r; //!
        TH1F* m_Ks_z; //!
        TH1F* m_Ks_DeltaR; //!

        TH1F* m_zp_pt; //!
        TH1F* m_zp_eta; //!
        TH1F* m_zp_r; //!
        TH1F* m_zp_z; //!
        TH1F* m_zp_DeltaR; //!



}; 

#endif //> !DISPLACEDDIMUONANALYSIS_TRACKINGSYSTEMATICS_H
