#ifndef DISPLACEDDILEPTONANALYSIS_LEPTONEFFICIENCY_H
#define DISPLACEDDILEPTONANALYSIS_LEPTONEFFICIENCY_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthParticle.h"
#include "xAODTracking/TrackParticleContainer.h"
#include "xAODTracking/TrackParticle.h"

// DVUtil
#include "DisplacedDimuonAnalysis/DVUtils.h"

// Trigger decision tool
#include "TrigDecisionTool/TrigDecisionTool.h"


class LeptonEfficiency: public ::AthAnalysisAlgorithm { 
    public: 

        LeptonEfficiency( const std::string& name, ISvcLocator* pSvcLocator );
        virtual ~LeptonEfficiency(); 
        
        virtual StatusCode  initialize();
        virtual StatusCode  execute();
        virtual StatusCode  finalize();
        
        virtual StatusCode beginInputFile();
    
    private: 

        ToolHandle<IDVUtils>  m_dvutils; //!
        ToolHandle<Trig::TrigDecisionTool> m_tdt; //!

        int m_event_count;
        int signal_mu_truth;
        int signal_el_truth;
        int signal_zp_truth;
        int signal_mu_reconstructed;
        int signal_el_reconstructed;

        TProfile* m_eff_mu_eta; //!
        TProfile* m_eff_mu_pt; //!
        TProfile* m_eff_mu_R; //!
        TProfile* m_eff_mu_d0; //!

        TProfile* m_eff_el_eta; //!
        TProfile* m_eff_el_pt; //!
        TProfile* m_eff_el_R; //!
        TProfile* m_eff_el_d0; //!

        // 2D efficiency plots
        TH2F* m_eff_mu_pt_vs_prodVtxR_num; //!
        TH2F* m_eff_mu_pt_vs_prodVtxR_den; //!

        TH2F* m_eff_mu_eta_vs_prodVtxR_num; //!
        TH2F* m_eff_mu_eta_vs_prodVtxR_den; //!

        TH2F* m_eff_mu_pt_vs_d0_num; //!
        TH2F* m_eff_mu_pt_vs_d0_den; //!

        TH2F* m_eff_mu_eta_vs_d0_num; //!
        TH2F* m_eff_mu_eta_vs_d0_den; //!

        TH2F* m_eff_el_pt_vs_prodVtxR_num; //!
        TH2F* m_eff_el_pt_vs_prodVtxR_den; //!

        TH2F* m_eff_el_eta_vs_prodVtxR_num; //!
        TH2F* m_eff_el_eta_vs_prodVtxR_den; //!

        TH2F* m_eff_el_pt_vs_d0_num; //!
        TH2F* m_eff_el_pt_vs_d0_den; //!

        TH2F* m_eff_el_eta_vs_d0_num; //!
        TH2F* m_eff_el_eta_vs_d0_den; //!



}; 

#endif //> !DISPLACEDDILEPTONANALYSIS_LEPTONEFFICIENCY_H
