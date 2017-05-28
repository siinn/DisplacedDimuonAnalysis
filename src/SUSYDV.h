#ifndef DISPLACEDDIMUONANALYSIS_SUSYDV_H
#define DISPLACEDDIMUONANALYSIS_SUSYDV_H 1

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

// GRL
#include "GoodRunsLists/IGoodRunsListSelectionTool.h"

// Trigger decision tool
#include "TrigDecisionTool/TrigDecisionTool.h"


class SUSYDV: public ::AthAnalysisAlgorithm { 
    public: 
        SUSYDV( const std::string& name, ISvcLocator* pSvcLocator );
        virtual ~SUSYDV(); 
        
        virtual StatusCode  initialize();
        virtual StatusCode  execute();
        virtual StatusCode  finalize();
        
        virtual StatusCode beginInputFile();

        virtual bool PassCosmicVeto(const DataVector<xAOD::Muon> dv_muc);
        virtual void plot_muon_kinematics(const DataVector<xAOD::Muon> dv_muc);
        virtual void plot_dv(const xAOD::Vertex& dv, const xAOD::Vertex& pv);
        virtual void plot_muon60_kinematics(const DataVector<xAOD::Muon> dv_muc);
        virtual void plot_dv60(const xAOD::Vertex& dv, const xAOD::Vertex& pv);
    
    private: 

        // debug
        //StoreGateSvc* m_StoreGate;

        // tool for muon matching to dv
        ToolHandle<DDL::IEventCuts> m_evtc; //!
        ToolHandle<DDL::IDiLepDVCuts> m_dilep;
        ToolHandle<DDL::IDVCuts> m_vxc;
        ToolHandle<DDL::IDiLepCosmics> m_cos; //!
        ToolHandle<IGoodRunsListSelectionTool> m_grlTool; //!
        ToolHandle<Trig::TrigDecisionTool> m_tdt; //!
        ToolHandle<IDVUtils> m_dvutils; //!

        // Handle to the tool
        ToolHandle<DDL::IOverlapRemoval> m_or;

        // DV mass accessor
        SG::AuxElement::ConstAccessor<float> m_accMass;

        // output
        TH1F* m_dv_M; //!
        TH1D* m_dv_cutflow; //!
        TH1D* m_event_cutflow; //!

        // dimuon vertices
        TH1F* m_dv_dimuon_M; //!
        TH1F* m_dv_dimuon_R; //!
        TH1F* m_dv_dimuon_R_low; //!
        TH1F* m_dv_dimuon_z; //!
        TH1F* m_dv_dimuon_l; //!

        // ABCD method
        TH2F* m_dv_dimuon_R_M; //!

        // check vertex fit
        TH1F* m_chi2_ndof; //!
        TH1F* m_chi2_ndof_nocosmic; //!

        // muon kinematics
        TH1F* m_signal_muon_pt; //!
        TH1F* m_signal_muon_pt_low; //!
        TH1F* m_signal_muon_eta; //!
        TH1F* m_signal_muon_phi; //!
        TH1F* m_signal_muon_d0; //!
        TH1F* m_signal_muon_z0; //!

        TH1F* m_signal_muon_pt_min; //!
        TH1F* m_signal_muon_pt_min_low; //!
        TH1F* m_signal_muon_pt_max; //!
        TH1F* m_signal_muon_pt_max_low; //!

        // cosmic veto
        TH1F* m_signal_muon_DeltaR; //!
        TH1F* m_signal_muon_DeltaR_low; //!
        TH1F* m_signal_muon_Rcos; //!
        TH1F* m_signal_muon_Rcos_low; //!

        // only for MC
        // dimuon vertices
        TH1F* m_dv_dimuon_M_matched; //!
        TH1F* m_dv_dimuon_R_matched; //!

        // ABCD method
        TH2F* m_dv_dimuon_R_M_matched; //!

        // pT > 60 GeV cut plots
        TH1F* m_dv_dimuon60_M; //!
        TH1F* m_dv_dimuon60_R; //!
        TH1F* m_dv_dimuon60_R_low; //!
        TH1F* m_dv_dimuon60_z; //!
        TH1F* m_dv_dimuon60_l; //!
        TH1F* m_signal_muon60_pt; //!
        TH1F* m_signal_muon60_pt_low; //!
        TH1F* m_signal_muon60_eta; //!
        TH1F* m_signal_muon60_phi; //!
        TH1F* m_signal_muon60_d0; //!
        TH1F* m_signal_muon60_z0; //!
        TH1F* m_signal_muon60_pt_min; //!
        TH1F* m_signal_muon60_pt_min_low; //!
        TH1F* m_signal_muon60_pt_max; //!
        TH1F* m_signal_muon60_pt_max_low; //!
        TH1F* m_signal_muon60_DeltaR; //!
        TH1F* m_signal_muon60_DeltaR_low; //!
        TH1F* m_signal_muon60_Rcos; //!
        TH1F* m_signal_muon60_Rcos_low; //!

}; 

#endif //> !DISPLACEDDIMUONANALYSIS_SUSYDV_H
