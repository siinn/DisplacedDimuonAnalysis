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

        float signal_truth;
        float signal_reconstructed;

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

        TH1F* m_prob_dist; //!
        TH1F* m_dR_dist; //!











}; 

#endif //> !DISPLACEDDIMUONANALYSIS_LRTVALIDATION_H
