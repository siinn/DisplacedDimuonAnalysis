#ifndef DISPLACEDDIMUONANALYSIS_LEPTONCOUNT_H
#define DISPLACEDDIMUONANALYSIS_LEPTONCOUNT_H 1

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
#include "DDLBase/IOverlapRemoval.h"
#include "DDLBase/IDiLepCosmics.h"
#include "DDLBase/IPhotonMatch.h"

// DVUtil
#include "DisplacedDimuonAnalysis/DVUtils.h"
#include "DisplacedDimuonAnalysis/LeptonSelectionTools.h"
#include "DisplacedDimuonAnalysis/FilterMatchingTools.h"
#include "DisplacedDimuonAnalysis/CosmicTools.h"

// GRL
#include "AsgAnalysisInterfaces/IGoodRunsListSelectionTool.h"

// Trigger decision tool
#include "TrigDecisionTool/TrigDecisionTool.h"
#include "TriggerMatchingTool/IMatchingTool.h"

// xAOD
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/Electron.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTracking/TrackParticle.h" 
#include "xAODTracking/Vertex.h"
#include "ElectronPhotonSelectorTools/IAsgElectronLikelihoodTool.h"
#include "MuonSelectorTools/IMuonSelectionTool.h"
#include "MuonMomentumCorrections/IMuonCalibrationAndSmearingTool.h"

class LeptonCount: public ::AthAnalysisAlgorithm { 
    public: 

        LeptonCount( const std::string& name, ISvcLocator* pSvcLocator );
        virtual ~LeptonCount(); 
        
        virtual StatusCode  initialize();
        virtual StatusCode  execute();
        virtual StatusCode  finalize();

        virtual bool GoodTrack(const xAOD::TrackParticle& tr);

        virtual StatusCode beginInputFile();

    
    private:

        // setting tools
        ToolHandle<IGoodRunsListSelectionTool> m_grlTool; //!
        ToolHandle<Trig::ITrigDecisionTool> m_tdt; //!
        ToolHandle<IDVUtils> m_dvutils; //!
        ToolHandle<ILeptonSelectionTools> m_leptool; //!
        ToolHandle<IFilterMatchingTools> m_fmtool; //!
        ToolHandle<ICosmicTools> m_costool; //!
        ToolHandle<Trig::IMatchingTool> m_tmt; //!
        ToolHandle<DDL::IOverlapRemoval> m_or;
        ToolHandle<DDL::IEventCuts> m_evtc; //!
        ToolHandle<DDL::IDiLepDVCuts> m_dilepdvc;
        ToolHandle<DDL::IDVCuts> m_dvc;
        ToolHandle<DDL::IDiLepCosmics> m_cos;
        ToolHandle<DDL::IPhotonMatch> m_phmatch;
        ToolHandle<IAsgElectronLikelihoodTool> m_elt;
        ToolHandle<CP::IMuonSelectionTool> m_mst;
        ToolHandle<CP::IMuonCalibrationAndSmearingTool> m_mct;

        // DV mass accessor
        SG::AuxElement::ConstAccessor<float> m_accMass;
        SG::AuxElement::Accessor<std::shared_ptr<xAOD::ElectronContainer>> m_accEl;
        SG::AuxElement::Accessor<std::shared_ptr<xAOD::MuonContainer>> m_accMu;

        // event cut flow
        TH1D* m_event_cutflow; //!

        // lepton cut flow
        TH1D* m_muon_cutflow; //!
        TH1D* m_electron_cutflow; //!
        TH1D* m_non_lepton_cutflow; //!

        // pile-up distribution
        TH1F* m_pileup; //!

    
}; 

#endif //> !DISPLACEDDIMUONANALYSIS_LEPTONCOUNT_H
