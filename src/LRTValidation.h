#ifndef DISPLACEDDIMUONANALYSIS_LRTVALIDATION_H
#define DISPLACEDDIMUONANALYSIS_LRTVALIDATION_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthParticle.h"
#include "xAODTracking/TrackParticleContainer.h"
#include "xAODTracking/TrackParticle.h"
#include "TProfile.h"

#include "DisplacedDimuonAnalysis/DVUtils.h"


class LRTValidation: public ::AthAnalysisAlgorithm { 
    public: 
        LRTValidation( const std::string& name, ISvcLocator* pSvcLocator );
        virtual ~LRTValidation(); 
        
        float getMatchingProbability ( const xAOD::TrackParticle* p );
        const xAOD::TruthParticle * getTruthPtr(const xAOD::TrackParticle * trackParticle);
        
        virtual StatusCode  initialize();
        virtual StatusCode  execute();
        virtual StatusCode  finalize();
        
    private: 
        ToolHandle<IDVUtils>  m_dvutils; //!

        float signal_truth; //!
        float signal_reconstructed; //!

        float n_truth_matched_by_hit; //!
        float n_truth_matched_by_dR_loose; //!
        float n_truth_matched_by_dR_tight; //!
        float n_truth_matched_by_dR_tight_ptcut; //!
        float n_reco_matched_by_hit; //!
        float n_reco_matched_by_dR_loose; //!
        float n_reco_matched_by_dR_tight; //!
        float n_reco_matched_by_dR_tight_ptcut; //!

        // efficiency plots
        TProfile* m_eff_eta_prob; //!
        TProfile* m_eff_phi_prob; //!
        TProfile* m_eff_pt_prob; //!
        TProfile* m_eff_R_prob; //!
    
        TProfile* m_eff_eta_dR_tight; //!
        TProfile* m_eff_phi_dR_tight; //!
        TProfile* m_eff_pt_dR_tight; //!
        TProfile* m_eff_R_dR_tight; //!

        TProfile* m_eff_eta_dR_loose; //!
        TProfile* m_eff_phi_dR_loose; //!
        TProfile* m_eff_pt_dR_loose; //!
        TProfile* m_eff_R_dR_loose; //!

        // fake rate plots
        TProfile* m_fakerate_eta_prob; //!
        TProfile* m_fakerate_phi_prob; //!
        TProfile* m_fakerate_pt_prob; //!
        TProfile* m_fakerate_d0_prob; //!

        TProfile* m_fakerate_nolink_eta_prob; //!
        TProfile* m_fakerate_nolink_phi_prob; //!
        TProfile* m_fakerate_nolink_pt_prob; //!
        TProfile* m_fakerate_nolink_d0_prob; //!
    
        TProfile* m_fakerate_eta_dR_tight; //!
        TProfile* m_fakerate_phi_dR_tight; //!
        TProfile* m_fakerate_pt_dR_tight; //!
        TProfile* m_fakerate_d0_dR_tight; //!

        TProfile* m_fakerate_eta_dR_loose; //!
        TProfile* m_fakerate_phi_dR_loose; //!
        TProfile* m_fakerate_pt_dR_loose; //!
        TProfile* m_fakerate_d0_dR_loose; //!

        // matching quality
        TH1F* m_dpt_prob; //!
        TH1F* m_dpt_dR_tight; //!
        TH1F* m_dpt_dR_loose; //!

        TH1F* m_dpt_norm_prob; //!
        TH1F* m_dpt_norm_dR_tight; //!
        TH1F* m_dpt_norm_dR_loose; //!
        
        TH1F* m_dR; //!
        TH1F* m_dR_hitMatched; //!
        TH1F* m_dR_hitFake; //!

        // 2D plots
        TH2F* m_dpt_vs_dR; //!
        TH2F* m_dpt_vs_dR_hitMatched; //!
        TH2F* m_dpt_vs_dR_hitFake; //!

        // plots to see if phi, eta, and theta are consistent between reco and truth
        TH2F* m_dphi_vs_truth_prodVtxR; //!
        TH2F* m_deta_vs_truth_prodVtxR; //!
        TH2F* m_dtheta_vs_truth_prodVtxR; //!

        TH1F* m_dphi_prompt; //!
        TH1F* m_dphi_displaced; //!
        TH1F* m_dphi_far_displaced; //!

        TH1F* m_dphi_wrt_bp_prompt; //!
        TH1F* m_dphi_wrt_bp_displaced; //!
        TH1F* m_dphi_wrt_bp_far_displaced; //!

        TH1F* m_deta_prompt; //!
        TH1F* m_deta_displaced; //!
        TH1F* m_deta_far_displaced; //!

        //TH1F* m_prob_dist; //!
        //TH1F* m_dR_dist; //!

        TH1F* m_average_match; //!











}; 

#endif //> !DISPLACEDDIMUONANALYSIS_LRTVALIDATION_H
