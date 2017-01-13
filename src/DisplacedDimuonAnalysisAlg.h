#ifndef DISPLACEDDIMUONANALYSIS_DISPLACEDDIMUONANALYSISALG_H
#define DISPLACEDDIMUONANALYSIS_DISPLACEDDIMUONANALYSISALG_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"

// STL
#include <map>
#include <string>
#include <vector>

// EDM
#include "AsgTools/ToolHandle.h"

// DV
#include "DVAnalyses/AlgBase.h"
#include "DDLBase/IDVCuts.h"
#include "DDLBase/IEventCuts.h"
//#include "DVCuts/IDiLepCosmics.h"
#include "DDLBase/IDiLepDVCuts.h"
//#include "DVTools/IOverlapRemoval.h"
//#include "DVTools/IPhotonMatch.h"

// DVUtil
#include "DisplacedDimuonAnalysis/DVUtils.h"

// GRL
//#include "GoodRunsLists/GoodRunsListSelectionTool.h"
#include "GoodRunsLists/IGoodRunsListSelectionTool.h"

// Trigger decision tool
#include "TrigDecisionTool/TrigDecisionTool.h"


class DisplacedDimuonAnalysisAlg: public ::AthAnalysisAlgorithm { 
    public: 

        DisplacedDimuonAnalysisAlg( const std::string& name, ISvcLocator* pSvcLocator );
        virtual ~DisplacedDimuonAnalysisAlg(); 
        
        virtual StatusCode  initialize();
        virtual StatusCode  execute();
        virtual StatusCode  finalize();
        
        virtual StatusCode beginInputFile();

        virtual void  plot_muon_kinematics(const DataVector<xAOD::Muon> dv_muc);
    
    private:

        // debug
        //StoreGateSvc* m_StoreGate;

        // tool for muon matching to dv
        ToolHandle<DDL::IDiLepDVCuts> m_dilepdvc; //!
        ToolHandle<DDL::IEventCuts> m_evtc; //!
        ToolHandle<IDVUtils>  m_dvutils; //!
        ToolHandle<DDL::IDVCuts> m_dvc; //!
        ToolHandle<IGoodRunsListSelectionTool> m_grlTool;
        ToolHandle<Trig::TrigDecisionTool> m_tdt;

        // DV mass accessor
        SG::AuxElement::ConstAccessor<float> m_accMass;

        // output
        TH1F* m_dv_M; //!
        TH1D* m_dv_cutflow; //!
        TH1D* m_event_cutflow; //!

        // dimuon vertices
        TH1F* m_dv_dimuon_M; //!
        TH1F* m_dv_dimuon_R; //!
        TH1F* m_dv_dimuon_z; //!
        TH1F* m_dv_dimuon_r; //!

        // ABCD method
        TH2F* m_dv_dimuon_R_M; //!

        // check vertex fit
        TH1F* m_chi2_ndof; //!

        // muon kinematics
        TH1F* m_signal_muon_pt; //!
        TH1F* m_signal_muon_pt_low; //!
        TH1F* m_signal_muon_eta; //!
        TH1F* m_signal_muon_phi; //!
        TH1F* m_signal_muon_DeltaR; //!
        TH1F* m_signal_muon_Delta_pT; //!








        // only for MC

        // dimuon vertices
        TH1F* m_dv_dimuon_M_matched; //!
        TH1F* m_dv_dimuon_R_matched; //!

        // ABCD method
        TH2F* m_dv_dimuon_R_M_matched; //!

          
    
}; 

#endif //> !DISPLACEDDIMUONANALYSIS_DISPLACEDDIMUONANALYSISALG_H
