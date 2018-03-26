#ifndef DISPLACEDDIMUONANALYSIS_COSMICBACKGROUND_H
#define DISPLACEDDIMUONANALYSIS_COSMICBACKGROUND_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"

// STL
#include <map>
#include <string>
#include <vector>

// EDM
#include "AsgTools/ToolHandle.h"

// xAOD
#include "xAODEventInfo/EventInfo.h"
#include "xAODTracking/TrackParticle.h"
#include "xAODTracking/VertexContainer.h"

// DV
#include "DDLBase/IEventCuts.h"
#include "DDLBase/IDVCuts.h"
#include "DDLBase/IDiLepDVCuts.h"
#include "DDLBase/IDiLepCosmics.h"
#include "DDLBase/IOverlapRemoval.h"
#include "DDLBase/ITrigMatch.h"

// DVUtil
#include "DisplacedDimuonAnalysis/DVUtils.h"
#include "DisplacedDimuonAnalysis/LeptonSelectionTools.h"
#include "DisplacedDimuonAnalysis/CosmicTools.h"

// GRL
#include "GoodRunsLists/IGoodRunsListSelectionTool.h"

// Trigger decision tool
#include "TrigDecisionTool/TrigDecisionTool.h"
#include "TriggerMatchingTool/IMatchingTool.h"

class CosmicBackground: public ::AthAnalysisAlgorithm { 

    public: 

        CosmicBackground( const std::string& name, ISvcLocator* pSvcLocator );
        virtual ~CosmicBackground(); 
        
        virtual StatusCode  initialize();     
        virtual StatusCode  execute();        
        virtual StatusCode  finalize();       

        virtual bool VertexTrackSelection(const xAOD::TrackParticle& tr);
        virtual float GetRcos(xAOD::TrackParticle& tr1, xAOD::TrackParticle& tr2);
        virtual float GetDeltaR(xAOD::TrackParticle& tr1, xAOD::TrackParticle& tr2);

        bool isMC;
     
    private: 

        // define tool
        ToolHandle<Trig::IMatchingTool> m_tmt;
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
        //ToolHandle<DDL::IDispVertexer> m_vertexer; //!
        ToolHandle<DDL::ITrigMatch> m_trig; //!

        // DV mass accessor
        SG::AuxElement::ConstAccessor<float> m_accMass;
        SG::AuxElement::Accessor<std::shared_ptr<xAOD::ElectronContainer>> m_accEl;
        SG::AuxElement::Accessor<std::shared_ptr<xAOD::MuonContainer>> m_accMu;
        SG::AuxElement::Accessor<TLorentzVector> m_acc_p4;

        // define histograms

        TH1D* m_event_cutflow; //!

        // cosmic plots of two muon pair
        TH1F* m_mumu_Rcos; //!
        TH1F* m_mumu_Rcos_low; //!
        TH1F* m_mumu_DeltaR; //!
        TH1F* m_mumu_DeltaR_low; //!
        TH2F* m_mumu_DeltaR_Rcos; //!

        // 2D number of cosmic muon vs vertices in DeltaR control region
        int m_n_cosmicPair_muon = 0; //!
        int m_n_cosmicVertex_muon = 0; //!
        TH2F* m_mumu_NcosmicPair_DeltaR; //!
        TH2F* m_mumu_NcosmicVertex_DeltaR; //!


}; 

#endif //> !DISPLACEDDIMUONANALYSIS_COSMICBACKGROUND_H
