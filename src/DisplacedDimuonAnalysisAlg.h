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
#include "DDLBase/IEventCuts.h"
#include "DDLBase/IDVCuts.h"
#include "DDLBase/IDiLepDVCuts.h"
#include "DDLBase/IOverlapRemoval.h"
#include "DDLBase/IDiLepCosmics.h"
#include "DDLBase/IPhotonMatch.h"
#include "DDLBase/IVxWeights.h"

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

// pile-up reweighting
#include "AsgAnalysisInterfaces/IPileupReweightingTool.h"

class DisplacedDimuonAnalysisAlg: public ::AthAnalysisAlgorithm { 
    public: 

        DisplacedDimuonAnalysisAlg( const std::string& name, ISvcLocator* pSvcLocator );
        virtual ~DisplacedDimuonAnalysisAlg(); 
        
        virtual StatusCode  initialize();
        virtual StatusCode  execute();
        virtual StatusCode  finalize();
        
        virtual StatusCode beginInputFile();

        virtual bool PassCosmicVeto(const DataVector<xAOD::Muon> dv_muc, const DataVector<xAOD::Electron> dv_elc, std::string channel);
        virtual void plot_signal_tp(const DataVector<xAOD::Muon> dv_muc, const DataVector<xAOD::Electron> dv_elc, std::string channel);
        virtual void plot_dv(const xAOD::Vertex& dv, const xAOD::Vertex& pv, std::string channel);

        virtual bool PassCosmicVeto_R_CR(xAOD::TrackParticle& tr1, xAOD::TrackParticle& tr2);

        // cosmic background plots
        virtual void FillCosmicBkg(xAOD::TrackParticle& tr1, xAOD::TrackParticle& tr2, std::string channel);
    
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
        ToolHandle<CP::IPileupReweightingTool> m_prw; //!
        ToolHandle<DDL::IVxWeights> m_vxwght; //!

        // DV mass accessor
        SG::AuxElement::ConstAccessor<float> m_accMass;
        SG::AuxElement::Accessor<std::shared_ptr<xAOD::ElectronContainer>> m_accEl;
        SG::AuxElement::Accessor<std::shared_ptr<xAOD::MuonContainer>> m_accMu;
        SG::AuxElement::Accessor<ElementLink<xAOD::TrackParticleContainer>> m_accTr;

        // output
        TH1D* m_event_cutflow; //!

        // pile-up distribution
        TH1F* m_pileup; //!

        // use PRW?
        bool m_usePRW;

        // pileup weight
        float p_weight = 1;

        //----------------------------------
        // mumu plots
        //----------------------------------
            TH1D* m_dv_mumu_cf; //!

            // dimuon vertices
            TH1F* m_dv_mumu_M; //!
            TH1F* m_dv_mumu_M_low; //!
            TH1F* m_dv_mumu_R; //!
            TH1F* m_dv_mumu_R_low; //!
            TH1F* m_dv_mumu_z; //!
            TH1F* m_dv_mumu_l; //!
            TH1F* m_dv_mumu_eta; //!
            TH2F* m_dv_mumu_R_M; //! ABCD
            // check vertex fit
            TH1F* m_dv_mumu_chi2_ndof; //!

            // muon kinematics
            TH1F* m_dv_mumu_mu_pt; //!
            TH1F* m_dv_mumu_mu_pt_low; //!
            TH1F* m_dv_mumu_mu_eta; //!
            TH1F* m_dv_mumu_mu_phi; //!
            TH1F* m_dv_mumu_mu_d0; //!
            TH1F* m_dv_mumu_mu_z0; //!

            // cosmic veto
            TH1F* m_dv_mumu_DeltaR; //!
            TH1F* m_dv_mumu_DeltaR_low; //!
            TH1F* m_dv_mumu_Rcos; //!
            TH1F* m_dv_mumu_Rcos_low; //!
            TH2F* m_dv_mumu_DeltaR_Rcos; //!

            // MC matching
            TH1F* m_dv_mumu_M_matched; //!
            TH1F* m_dv_mumu_R_matched; //!
            TH2F* m_dv_mumu_R_M_matched; //!
            TH1F* m_dv_mumu_eta_matched; //!

        //----------------------------------
        // ee plots
        //----------------------------------

            TH1D* m_dv_ee_cf; //!

            // dielectron vertices
            TH1F* m_dv_ee_M; //!
            TH1F* m_dv_ee_R; //!
            TH1F* m_dv_ee_R_low; //!
            TH1F* m_dv_ee_z; //!
            TH1F* m_dv_ee_l; //!
            TH1F* m_dv_ee_eta; //!
            TH2F* m_dv_ee_R_M; //! ABCD
            // check vertex fit
            TH1F* m_dv_ee_chi2_ndof; //!

            // electron kinematics
            TH1F* m_dv_ee_e_pt; //!
            TH1F* m_dv_ee_e_pt_low; //!
            TH1F* m_dv_ee_e_eta; //!
            TH1F* m_dv_ee_e_phi; //!
            TH1F* m_dv_ee_e_d0; //!
            TH1F* m_dv_ee_e_z0; //!

            // cosmic veto
            TH1F* m_dv_ee_DeltaR; //!
            TH1F* m_dv_ee_DeltaR_low; //!
            TH1F* m_dv_ee_Rcos; //!
            TH1F* m_dv_ee_Rcos_low; //!

            // MC matching
            TH1F* m_dv_ee_M_matched; //!
            TH1F* m_dv_ee_R_matched; //!
            TH2F* m_dv_ee_R_M_matched; //!
            TH1F* m_dv_ee_eta_matched; //!

        //----------------------------------
        // emu plots
        //----------------------------------

            TH1D* m_dv_emu_cf; //!

            // dielectron vertices
            TH1F* m_dv_emu_M; //!
            TH1F* m_dv_emu_R; //!
            TH1F* m_dv_emu_R_low; //!
            TH1F* m_dv_emu_z; //!
            TH1F* m_dv_emu_l; //!
            TH1F* m_dv_emu_eta; //!
            TH2F* m_dv_emu_R_M; //! ABCD
            // check vertex fit
            TH1F* m_dv_emu_chi2_ndof; //!

            // electron kinematics
            TH1F* m_dv_emu_e_pt; //!
            TH1F* m_dv_emu_e_pt_low; //!
            TH1F* m_dv_emu_e_eta; //!
            TH1F* m_dv_emu_e_phi; //!
            TH1F* m_dv_emu_e_d0; //!
            TH1F* m_dv_emu_e_z0; //!

            TH1F* m_dv_emu_mu_pt; //!
            TH1F* m_dv_emu_mu_pt_low; //!
            TH1F* m_dv_emu_mu_eta; //!
            TH1F* m_dv_emu_mu_phi; //!
            TH1F* m_dv_emu_mu_d0; //!
            TH1F* m_dv_emu_mu_z0; //!

            // cosmic veto
            TH1F* m_dv_emu_DeltaR; //!
            TH1F* m_dv_emu_DeltaR_low; //!
            TH1F* m_dv_emu_Rcos; //!
            TH1F* m_dv_emu_Rcos_low; //!

            // MC matching
            TH1F* m_dv_emu_M_matched; //!
            TH1F* m_dv_emu_R_matched; //!
            TH2F* m_dv_emu_R_M_matched; //!
            TH1F* m_dv_emu_eta_matched; //!

        //----------------------------------
        // lepton + track plots
        //----------------------------------
            TH1D* m_dv_mut_cf; //!
            TH1D* m_dv_et_cf; //!

        //----------------------------------
        // trk-trk plots
        //----------------------------------
            TH1D* m_dv_idid_cf; //!
            TH1F* m_dv_idid_M; //!
            TH1F* m_dv_idid_z; //!
            TH1F* m_dv_idid_R; //!
            TH1F* m_dv_idid_l; //!
            TH1F* m_dv_idid_eta; //!
            TH1F* m_dv_idid_chi2_ndof; //!

            // cosmic veto
            TH1F* m_dv_idid_DeltaR; //!
            TH1F* m_dv_idid_DeltaR_low; //!
            TH1F* m_dv_idid_Rcos; //!
            TH1F* m_dv_idid_Rcos_low; //!
            TH2F* m_dv_idid_DeltaR_Rcos; //!

            int n_event_all = 0;
            int n_vrtsec_all = 0;
            int n_dvc_copy = 0;
            int n_dv_all = 0;
            int n_dv_passed_cut = 0;

    
}; 

#endif //> !DISPLACEDDIMUONANALYSIS_DISPLACEDDIMUONANALYSISALG_H
