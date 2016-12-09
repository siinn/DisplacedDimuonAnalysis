
#include "LRTValidation.h"

//#include "xAODEventInfo/EventInfo.h"

//uncomment the line below to use the HistSvc for outputting trees and histograms
#include "GaudiKernel/ITHistSvc.h"
#include "TH1F.h"
#include "TProfile.h"
#include "xAODTruth/TruthVertex.h"
#include <vector>
#include "cmath"
using std::pair;
using std::make_pair;



LRTValidation::LRTValidation( const std::string& name, ISvcLocator* pSvcLocator ) : 
m_dvutils("DVUtils"),
AthAnalysisAlgorithm( name, pSvcLocator )
{

    declareProperty("DVUtils", m_dvutils);

}


LRTValidation::~LRTValidation() {}


StatusCode LRTValidation::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");
    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    // efficiency
    m_eff_eta_prob = new TProfile("eff_eta_prob","Tracking efficiency vs eta, using prob. method",50,-3.0,3.0);
    m_eff_phi_prob = new TProfile("eff_phi_prob","Tracking efficiency vs phi, using prob. method",50,-M_PI,M_PI);
    m_eff_pt_prob = new TProfile("eff_pt_prob","Tracking efficiency vs pt, using prob. method",50,0,400.);
    m_eff_R_prob = new TProfile("eff_R_prob","Tracking efficiency vs R, using prob. method",50,0,400.);

    m_eff_eta_dR_tight = new TProfile("eff_eta_dR_tight","Tracking efficiency vs eta, using dR. method",50,-3.0,3.0);
    m_eff_phi_dR_tight = new TProfile("eff_phi_dR_tight","Tracking efficiency vs phi, using dR. method",50,-M_PI,M_PI);
    m_eff_pt_dR_tight = new TProfile("eff_pt_dR_tight","Tracking efficiency vs pt, using dR. method",50,0,400.);
    m_eff_R_dR_tight = new TProfile("eff_R_dR_tight","Tracking efficiency vs R, using dR. method",50,0,400.);

    m_eff_eta_dR_loose = new TProfile("eff_eta_dR_loose","Tracking efficiency vs eta, using dR. method",50,-3.0,3.0);
    m_eff_phi_dR_loose = new TProfile("eff_phi_dR_loose","Tracking efficiency vs phi, using dR. method",50,-M_PI,M_PI);
    m_eff_pt_dR_loose = new TProfile("eff_pt_dR_loose","Tracking efficiency vs pt, using dR. method",50,0,400.);
    m_eff_R_dR_loose = new TProfile("eff_R_dR_loose","Tracking efficiency vs R, using dR. method",50,0,400.);

    // fake rate

    m_fakerate_eta_prob = new TProfile("fakerate_eta_prob","fake rate vs eta, using prob. method",50,-3.0,3.0);
    m_fakerate_phi_prob = new TProfile("fakerate_phi_prob","fake rate vs phi, using prob. method",50,-M_PI,M_PI);
    m_fakerate_pt_prob = new TProfile("fakerate_pt_prob","fake rate vs pt, using prob. method",50,0,400.);
    m_fakerate_d0_prob = new TProfile("fakerate_d0_prob","fake rate vs d0, using prob. method",50,-300,300.);

    m_fakerate_nolink_eta_prob = new TProfile("fakerate_nolink_eta_prob","fake rate vs eta, using prob, nolink. method",50,-3.0,3.0);
    m_fakerate_nolink_phi_prob = new TProfile("fakerate_nolink_phi_prob","fake rate vs phi, using prob, nolink. method",50,-M_PI,M_PI);
    m_fakerate_nolink_pt_prob = new TProfile("fakerate_nolink_pt_prob","fake rate vs pt, using prob, nolink. method",50,0,400.);
    m_fakerate_nolink_d0_prob = new TProfile("fakerate_nolink_d0_prob","fake rate vs d0, using prob, nolink. method",50,-300,300.);

    m_fakerate_eta_dR_tight = new TProfile("fakerate_eta_dR_tight","fake rate vs eta, using dR. method",50,-3.0,3.0);
    m_fakerate_phi_dR_tight = new TProfile("fakerate_phi_dR_tight","fake rate vs phi, using dR. method",50,-M_PI,M_PI);
    m_fakerate_pt_dR_tight = new TProfile("fakerate_pt_dR_tight","fake rate vs pt, using dR. method",50,0,400.);
    m_fakerate_d0_dR_tight = new TProfile("fakerate_d0_dR_tight","fake rate vs d0, using dR. method",50,-300,300.);

    m_fakerate_eta_dR_loose = new TProfile("fakerate_eta_dR_loose","fake rate vs eta, using dR. method",50,-3.0,3.0);
    m_fakerate_phi_dR_loose = new TProfile("fakerate_phi_dR_loose","fake rate vs phi, using dR. method",50,-M_PI,M_PI);
    m_fakerate_pt_dR_loose = new TProfile("fakerate_pt_dR_loose","fake rate vs pt, using dR. method",50,0,400.);
    m_fakerate_d0_dR_loose = new TProfile("fakerate_d0_dR_loose","fake rate vs d0, using dR. method",50,-300,300.);

    // matching quality
    m_dpt_prob = new TH1F("dpt_prb", "reco pt - truth pt, using prob. method", 100, -5, 5);
    m_dpt_dR_tight = new TH1F("dpt_dR_tight", "reco pt - truth pt, using dR_tight. method", 100, -5, 5);
    m_dpt_dR_loose = new TH1F("dpt_dR_loose", "reco pt - truth pt, using dR_loose. method", 100, -5, 5);

    m_dpt_norm_prob = new TH1F("dpt_norm_prb", "reco pt - truth pt, using prob. method", 100, -5, 5);
    m_dpt_norm_dR_tight = new TH1F("dpt_norm_dR_tight", "reco pt - truth pt, using dR_tight. method", 100, -5, 5);
    m_dpt_norm_dR_loose = new TH1F("dpt_norm_dR_loose", "reco pt - truth pt, using dR_loose. method", 100, -5, 5);

    // 2D dR plots

    m_dpt_vs_dR = new TH2F("dpt_vs_dR", "dpt vs dR", 50, 0.005, 0.505, 100, -5, 5);
    m_dpt_vs_dR_hitMatched = new TH2F("dpt_vs_dR_hitMatched", "dpt vs dR, only tracks matched using hit", 50, 0.005, 0.505, 100, -5, 5);
    m_dpt_vs_dR_hitFake = new TH2F("dpt_vs_dR_hitFake", "dpt vs dR, only tracks Fake using hit", 50, 0.005, 0.505, 100, -5, 5);

    // 2D plots to check phi and eta between reco and truth
    m_dphi_vs_truth_prodVtxR = new TH2F("m_dphi_vs_truth_prodVtxR", "dphi vs prod Vtx R", 300, 0.005, 300.005, 50, -1, 1);
    m_deta_vs_truth_prodVtxR = new TH2F("m_deta_vs_truth_prodVtxR", "deta vs prod Vtx R", 300, 0.005, 300.005, 50, -1, 1);
    m_dtheta_vs_truth_prodVtxR = new TH2F("m_dtheta_vs_truth_prodVtxR", "dtheta vs prod Vtx R", 300, 0.005, 300.005, 50, -1, 1);

    m_dphi_prompt = new TH1F("m_dphi_prompt", "dphi distribution for prompt decay", 100, -0.4, 0.4);
    m_dphi_displaced = new TH1F("m_dphi_displaced", "dphi distribution for displaced decay", 100, -0.4, 0.4);
    m_dphi_far_displaced = new TH1F("m_dphi_far_displaced", "dphi distribution for far_displaced decay", 100, -0.4, 0.4);

    m_dphi_wrt_bp_prompt = new TH1F("m_dphi_wrt_bp_prompt", "dphi distribution for prompt decay wrt beam pipe", 100, -0.4, 0.4);
    m_dphi_wrt_bp_displaced = new TH1F("m_dphi_wrt_bp_displaced", "dphi distribution for displaced decay wrt beam pipe", 100, -0.4, 0.4);
    m_dphi_wrt_bp_far_displaced = new TH1F("m_dphi_wrt_bp_far_displaced", "dphi distribution for far_displaced decay wrt beam pipe", 100, -0.4, 0.4);

    m_deta_prompt = new TH1F("m_deta_prompt", "deta distribution for prompt decay", 100, -0.4, 0.4);
    m_deta_displaced = new TH1F("m_deta_displaced", "deta distribution for displaced decay", 100, -0.4, 0.4);
    m_deta_far_displaced = new TH1F("m_deta_far_displaced", "deta distribution for far_displaced decay", 100, -0.4, 0.4);

    // 1D dR plots
    m_dR = new TH1F("dpt_dR", "dR", 50, 0.005, 0.505);
    m_dR_hitMatched = new TH1F("dpt_hitMatched", "dR, only tracks matched using hit", 50, 0.005, 0.505);
    m_dR_hitFake = new TH1F("dpt_hitFake", "dR, only tracks not matched using hit", 50, 0.005, 0.505);

    m_average_match = new TH1F( "average_match", "averaged matched reco track per reconstructed truth", 3,0,3);

    //m_prob_dist = new TH1F("prob_dist", "prob distribution per signal truth", 10, 0.05, 1.05);
    //m_dR_dist = new TH1F("dR_dist", "dR distribution per signal truth", 10, 0.05, 1.05);


    // efficiency

    CHECK( histSvc->regHist("/DV/LRTValidation/Efficiency/Prob/eff_eta_prob", m_eff_eta_prob) );
    CHECK( histSvc->regHist("/DV/LRTValidation/Efficiency/Prob/eff_phi_prob", m_eff_phi_prob) );
    CHECK( histSvc->regHist("/DV/LRTValidation/Efficiency/Prob/eff_pt_prob", m_eff_pt_prob) );
    CHECK( histSvc->regHist("/DV/LRTValidation/Efficiency/Prob/eff_R_prob", m_eff_R_prob) );

    CHECK( histSvc->regHist("/DV/LRTValidation/Efficiency/dR/eff_eta_dR_tight", m_eff_eta_dR_tight) );
    CHECK( histSvc->regHist("/DV/LRTValidation/Efficiency/dR/eff_phi_dR_tight", m_eff_phi_dR_tight) );
    CHECK( histSvc->regHist("/DV/LRTValidation/Efficiency/dR/eff_pt_dR_tight", m_eff_pt_dR_tight) );
    CHECK( histSvc->regHist("/DV/LRTValidation/Efficiency/dR/eff_R_dR_tight", m_eff_R_dR_tight) );

    CHECK( histSvc->regHist("/DV/LRTValidation/Efficiency/dR/eff_eta_dR_loose", m_eff_eta_dR_loose) );
    CHECK( histSvc->regHist("/DV/LRTValidation/Efficiency/dR/eff_phi_dR_loose", m_eff_phi_dR_loose) );
    CHECK( histSvc->regHist("/DV/LRTValidation/Efficiency/dR/eff_pt_dR_loose", m_eff_pt_dR_loose) );
    CHECK( histSvc->regHist("/DV/LRTValidation/Efficiency/dR/eff_R_dR_loose", m_eff_R_dR_loose) );

    // fake rate

    CHECK( histSvc->regHist("/DV/LRTValidation/FakeRate/Prob/fakerate_eta_prob", m_fakerate_eta_prob) );
    CHECK( histSvc->regHist("/DV/LRTValidation/FakeRate/Prob/fakerate_phi_prob", m_fakerate_phi_prob) );
    CHECK( histSvc->regHist("/DV/LRTValidation/FakeRate/Prob/fakerate_pt_prob", m_fakerate_pt_prob) );
    CHECK( histSvc->regHist("/DV/LRTValidation/FakeRate/Prob/fakerate_d0_prob", m_fakerate_d0_prob) );

    CHECK( histSvc->regHist("/DV/LRTValidation/FakeRate/Prob/fakerate_nolink_eta_prob", m_fakerate_nolink_eta_prob) );
    CHECK( histSvc->regHist("/DV/LRTValidation/FakeRate/Prob/fakerate_nolink_phi_prob", m_fakerate_nolink_phi_prob) );
    CHECK( histSvc->regHist("/DV/LRTValidation/FakeRate/Prob/fakerate_nolink_pt_prob", m_fakerate_nolink_pt_prob) );
    CHECK( histSvc->regHist("/DV/LRTValidation/FakeRate/Prob/fakerate_nolink_d0_prob", m_fakerate_nolink_d0_prob) );

    CHECK( histSvc->regHist("/DV/LRTValidation/FakeRate/dR/fakerate_eta_dR_tight", m_fakerate_eta_dR_tight) );
    CHECK( histSvc->regHist("/DV/LRTValidation/FakeRate/dR/fakerate_phi_dR_tight", m_fakerate_phi_dR_tight) );
    CHECK( histSvc->regHist("/DV/LRTValidation/FakeRate/dR/fakerate_pt_dR_tight", m_fakerate_pt_dR_tight) );
    CHECK( histSvc->regHist("/DV/LRTValidation/FakeRate/dR/fakerate_d0_dR_tight", m_fakerate_d0_dR_tight) );
    
    CHECK( histSvc->regHist("/DV/LRTValidation/FakeRate/dR/fakerate_eta_dR_loose", m_fakerate_eta_dR_loose) );
    CHECK( histSvc->regHist("/DV/LRTValidation/FakeRate/dR/fakerate_phi_dR_loose", m_fakerate_phi_dR_loose) );
    CHECK( histSvc->regHist("/DV/LRTValidation/FakeRate/dR/fakerate_pt_dR_loose", m_fakerate_pt_dR_loose) );
    CHECK( histSvc->regHist("/DV/LRTValidation/FakeRate/dR/fakerate_d0_dR_loose", m_fakerate_d0_dR_loose) );

    // matching quality
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/Prob/reco_pt_truth_pt_prob", m_dpt_prob) );
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dR/reco_pt_truth_pt_dr_tight", m_dpt_dR_tight) );
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dR/reco_pt_truth_pt_dr_loose", m_dpt_dR_loose) );

    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/Prob/reco_pt_truth_pt_prob_norm", m_dpt_norm_prob) );
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dR/reco_pt_truth_pt_dr_tight_norm", m_dpt_norm_dR_tight) );
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dR/reco_pt_truth_pt_dr_loose_norm", m_dpt_norm_dR_loose) );

    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dR/dpt_vs_dR", m_dpt_vs_dR) );
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dR/dpt_vs_dR_hitMatched", m_dpt_vs_dR_hitMatched) );
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dR/dpt_vs_dR_hitFake", m_dpt_vs_dR_hitFake) );

    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dR/dR", m_dR) );
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dR/dR_hitMatched", m_dR_hitMatched) );
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dR/dR_hitFake", m_dR_hitFake) );

    // investigating phi, eta between reco and truth

    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dPhi/dphi_vs_truth_prodVtxR", m_dphi_vs_truth_prodVtxR) );

    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dPhi/dphi_prompt", m_dphi_prompt) );
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dPhi/dphi_displaced", m_dphi_displaced) );
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dPhi/dphi_far_displaced", m_dphi_far_displaced) );
    
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dPhi/dphi_wrt_bp_prompt", m_dphi_wrt_bp_prompt) );
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dPhi/dphi_wrt_bp_displaced", m_dphi_wrt_bp_displaced) );
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dPhi/dphi_wrt_bp_far_displaced", m_dphi_wrt_bp_far_displaced) );

    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dEta/deta_vs_truth_prodVtxR", m_deta_vs_truth_prodVtxR) );
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dEta/deta_prompt", m_deta_prompt) );
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dEta/deta_displaced", m_deta_displaced) );
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dEta/deta_far_displaced", m_deta_far_displaced) );

    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dTheta/dtheta_vs_truth_prodVtxR", m_dtheta_vs_truth_prodVtxR) );


    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/average_match", m_average_match) );
    
    //CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/Prob/prob_dist", m_prob_dist) );
    //CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dR/dR_dist", m_dR_dist) );


    float signal_truth = 0;
    float signal_reconstructed = 0;

    float n_truth_matched_by_hit = 0;
    float n_truth_matched_by_dR_loose = 0;
    float n_truth_matched_by_dR_tight = 0;
    float n_truth_matched_by_dR_tight_ptcut = 0;
    float n_reco_matched_by_hit = 0;
    float n_reco_matched_by_dR_loose = 0;
    float n_reco_matched_by_dR_tight = 0;
    float n_reco_matched_by_dR_tight_ptcut = 0;

    
    return StatusCode::SUCCESS;
}

StatusCode LRTValidation::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");
    
    ATH_MSG_INFO( "Number of signal truth = " << signal_truth);
    ATH_MSG_INFO( "Number of signal reconstructed = " << signal_reconstructed);

    // normalized reco pt - truth pt plot
    m_dpt_norm_prob->Scale( 1/m_dpt_norm_prob->Integral() );
    m_dpt_norm_dR_tight->Scale( 1/m_dpt_norm_dR_tight->Integral() );
    m_dpt_norm_dR_loose->Scale( 1/m_dpt_norm_dR_loose->Integral() );

    // correction for division
    m_dpt_norm_prob->Sumw2();
    m_dpt_norm_dR_tight->Sumw2();
    m_dpt_norm_dR_loose->Sumw2();

    // output average number of reco tracks / matched truth
    m_average_match->Fill( "Hit", n_reco_matched_by_hit / n_truth_matched_by_hit );
    m_average_match->Fill( "dR_Loose", n_reco_matched_by_dR_loose / n_truth_matched_by_dR_loose );
    m_average_match->Fill( "dR_Tight", n_reco_matched_by_dR_tight / n_truth_matched_by_dR_tight );
    m_average_match->Fill( "dR_Tight_pTcut", n_reco_matched_by_dR_tight_ptcut / n_truth_matched_by_dR_tight_ptcut );

    // normalize dPhi, dEta plots
    m_dphi_prompt->Scale( 1/m_dphi_prompt->Integral() );
    m_dphi_displaced->Scale( 1/m_dphi_displaced->Integral() );
    m_dphi_far_displaced->Scale( 1/m_dphi_far_displaced->Integral() );

    m_dphi_wrt_bp_prompt->Scale( 1/m_dphi_wrt_bp_prompt->Integral() );
    m_dphi_wrt_bp_displaced->Scale( 1/m_dphi_wrt_bp_displaced->Integral() );
    m_dphi_wrt_bp_far_displaced->Scale( 1/m_dphi_wrt_bp_far_displaced->Integral() );

    m_deta_prompt->Scale( 1/m_deta_prompt->Integral() );
    m_deta_displaced->Scale( 1/m_deta_displaced->Integral() );
    m_deta_far_displaced->Scale( 1/m_deta_far_displaced->Integral() );
   
    return StatusCode::SUCCESS;
}

StatusCode LRTValidation::execute() {  
    ATH_MSG_DEBUG ("Executing " << name() << "...");


    //----------------------------------------------------------
    // Main truth loop for efficiency
    //----------------------------------------------------------

    // get truth container
    const xAOD::TruthParticleContainer* truthParticles = 0;
    CHECK( evtStore()->retrieve( truthParticles, "TruthParticles" ) );

    // loop over TruthParticles container 
    for (auto tru: *truthParticles) {

        // truth selection 
        if (!m_dvutils->TrackSelection(tru)) continue;

        // select signal truth
        if (!m_dvutils->isSignal(tru)) continue;

        // count signal truth
        signal_truth++;

        // * Vector of pairs: <truth_matching_probability, track> if prob > minProbEffLow (0.5).
        std::vector <pair<float, const xAOD::TrackParticle*> > prospects;

	    float bestMatch = 0;
        float minProbEffLow = 0.5;
        float mindR_tight = 0.1;
        float mindR_loose = 0.2;

        // set flag for both methods
        bool reconstructed_prob = false;
        bool reconstructed_dR_tight = false;
        bool reconstructed_dR_tight_ptcut = false;
        bool reconstructed_dR_loose = false;

        // retreiving InDetTrackParticles
        const xAOD::TrackParticleContainer* recoTracks = 0;
        CHECK( evtStore()->retrieve( recoTracks, "InDetTrackParticles" ) );

        // calculate R of production vertex
        float tru_x = tru->prodVtx()->x();
        float tru_y = tru->prodVtx()->y();
        float tru_R = std::sqrt(tru_x * tru_x + tru_y * tru_y);
        
        // loop over InDetTrackParticles container 
        for (auto idtrk: *recoTracks) {

            // reco track selection
            if (!m_dvutils->TrackSelection(idtrk)) continue;

            //----------------------------------------------------------
            // Matching by matching probability
            //----------------------------------------------------------

	        // * Get associated truth particle and probability.
	        const xAOD::TruthParticle * associatedTruth = getTruthPtr(idtrk);
	        float prob = getMatchingProbability(idtrk);

	        // * If the associated truth particle matches the current particle in the truth particle loop... 
	        if (associatedTruth && associatedTruth == tru) {

                // fill prob. distribution per truth
                //m_prob_dist->Fill(prob);

                // if this reco track is matched, count it
                if (prob > minProbEffLow) n_reco_matched_by_hit++;

                // push back the reco track with the most matching prob.
	            bestMatch = std::max(prob, bestMatch);
	            if ( (prob == bestMatch) && (prob > minProbEffLow) ){
	                prospects.push_back(make_pair(prob, idtrk));
	                reconstructed_prob = true;
	            }

	        } // end of associatedTruth == this Truth

            //----------------------------------------------------------
            // Matching by dR
            //----------------------------------------------------------

            double dr = tru->p4().DeltaR(idtrk->p4());

            // calculate dpt to make cut dpt < 0.1
            float dpt = ( (idtrk->pt() - tru->pt()) / tru->pt() ) ; // this is fraction. No unit

            // fill dR. distribution per truth
            //m_dR_dist->Fill(dr);

            // set reconstruction flag
            if(dr < mindR_tight) {
                reconstructed_dR_tight = true;
                n_reco_matched_by_dR_tight++;
            }
            if( (dr < mindR_tight) and (dpt < 0.1) ){
                reconstructed_dR_tight_ptcut = true;
                n_reco_matched_by_dR_tight_ptcut++;
            }
            if(dr < mindR_loose) {
                reconstructed_dR_loose = true;
                n_reco_matched_by_dR_loose++;
            }

        } // end of InDetTrackParticles loop

        // count signal truth reconstructed by hit matching
        if (reconstructed_prob) signal_reconstructed++;

        // fill histograms for prob. method
        m_eff_eta_prob->Fill(tru->eta(), reconstructed_prob);
        m_eff_phi_prob->Fill(tru->phi(), reconstructed_prob);
        m_eff_pt_prob->Fill( tru->pt() / 1000., reconstructed_prob);
        m_eff_R_prob->Fill(tru_R, reconstructed_prob);

        // fill histograms for dR method
        m_eff_eta_dR_tight->Fill(tru->eta(), reconstructed_dR_tight);
        m_eff_phi_dR_tight->Fill(tru->phi(), reconstructed_dR_tight);
        m_eff_pt_dR_tight->Fill( tru->pt() / 1000., reconstructed_dR_tight);
        m_eff_R_dR_tight->Fill(tru_R, reconstructed_dR_tight);

        m_eff_eta_dR_loose->Fill(tru->eta(), reconstructed_dR_loose);
        m_eff_phi_dR_loose->Fill(tru->phi(), reconstructed_dR_loose);
        m_eff_pt_dR_loose->Fill( tru->pt() / 1000., reconstructed_dR_loose);
        m_eff_R_dR_loose->Fill(tru_R, reconstructed_dR_loose);

        // count truth matched with different truth-matching methods
        if (reconstructed_prob) n_truth_matched_by_hit++;
        if (reconstructed_dR_loose) n_truth_matched_by_dR_loose++;
        if (reconstructed_dR_tight) n_truth_matched_by_dR_tight++;
        if (reconstructed_dR_tight_ptcut) n_truth_matched_by_dR_tight_ptcut++;

    } // end of truthParticle loop


    //----------------------------------------------------------
    // Reco track loop for fake rate
    //----------------------------------------------------------

    // retreiving InDetTrackParticles
    const xAOD::TrackParticleContainer* recoTracks = 0;
    CHECK( evtStore()->retrieve( recoTracks, "InDetTrackParticles" ) );

    // loop over InDetTrackParticles container 
    for (auto idtrk: *recoTracks) {

        // reco track selection
        if (!m_dvutils->TrackSelection(idtrk)) continue;
        //if (!m_dvutils->isLargeD0Track(idtrk)) continue;

        // set flag for both methods
        bool reconstructed_prob = true;
        bool reconstructed_prob_nolink = true;
        bool reconstructed_dR_tight = false;
        bool reconstructed_dR_loose = false;

        float minProbEffLow = 0.5;
        float mindR_tight = 0.1;
        float mindR_loose = 0.2;

        //----------------------------------------------------------
        // Matching by matching probability
        //----------------------------------------------------------

        const xAOD::TruthParticle * associatedTruth = getTruthPtr(idtrk);
        bool hasTruthLink = !(associatedTruth == nullptr);
	    float prob = getMatchingProbability(idtrk);

        // access matching probability
        if (prob < minProbEffLow) {
            if (hasTruthLink) reconstructed_prob = false;
            else reconstructed_prob_nolink = false;
        }

        // calculate difference in pT for matching reco tracks
        if (prob > minProbEffLow and hasTruthLink) {
            float dpt = ( (idtrk->pt() - associatedTruth->pt()) / associatedTruth->pt() ); // this is fraction. No unit
            
            m_dpt_prob->Fill( dpt );
            m_dpt_norm_prob->Fill( dpt );
        }

        //----------------------------------------------------------
        // Matching by dR
        //----------------------------------------------------------

        // get truth container
        const xAOD::TruthParticleContainer* truthParticles = 0;
        CHECK( evtStore()->retrieve( truthParticles, "TruthParticles" ) );

        for (auto tru: *truthParticles){
    
            // truth selection tool
            if (!m_dvutils->TrackSelection(tru)) continue;

            // calculate deviation between truth particle and indet track particle
            double dr = tru->p4().DeltaR(idtrk->p4());
            float dpt = ( (idtrk->pt() - tru->pt()) / tru->pt() ) ; // this is fraction. No unit

            float dphi = ( (idtrk->phi() - tru->phi()) / tru->phi() ) ; // this is fraction. No unit
            float deta = ( (idtrk->eta() - tru->eta()) / tru->eta() ) ; // this is fraction. No unit

            // check if phi wrt beam pipe is available
            float dphi_wrt_bp = 0.;
            if(tru->isAvailable<float>("phi")){
                dphi_wrt_bp = ( (idtrk->phi() - tru->auxdata<float>("phi")) / tru->auxdata<float>("phi") ) ; // this is fraction. No unit
            }
            ATH_MSG_INFO("Debug here 1, phi = " << dphi_wrt_bp);

            float truth_theta = 2.*atan(exp(-tru->eta() ));
            float reco_theta = 2.*atan(exp(-idtrk->eta() ));
            float dtheta = ( (reco_theta - truth_theta) / truth_theta ) ; // this is fraction. No unit
        
            //----------------------------------------------------
            // fill 2D plots dR vs fraction of change in pT 
            m_dpt_vs_dR->Fill(dr, dpt);

            // fill dR between this reco track and all truth particles
            m_dR->Fill(dr);

            if (associatedTruth && associatedTruth == tru) {
                ATH_MSG_INFO("Debug here 2, phi = " << dphi_wrt_bp << ", prob = " << prob);
                // fill plots only for the pair of reco and truth that are matched by hit
                if (prob > minProbEffLow) {
                    m_dpt_vs_dR_hitMatched->Fill(dr, dpt);
                    m_dR_hitMatched->Fill(dr);

                    ATH_MSG_INFO("Debug here 3, phi = " << dphi_wrt_bp << ", prob = " << prob);
                    // in order to plot dPhi, dEta as a function of R, check if there is a production vtx
                    if (tru->prodVtx()){
                        m_dphi_vs_truth_prodVtxR->Fill(tru->prodVtx()->perp(),dphi);
                        m_deta_vs_truth_prodVtxR->Fill(tru->prodVtx()->perp(),deta);
                        m_dtheta_vs_truth_prodVtxR->Fill(tru->prodVtx()->perp(),dtheta);

                        // if prompt decay
                        if (tru->prodVtx()->perp() < 1.0 ) {
                            m_dphi_prompt->Fill(dphi);
                            m_deta_prompt->Fill(deta);

                            // fill dphi wrt to beampipe only if phi is available wrt beampipe
                            ATH_MSG_INFO("do I have dphi wrt bp? " << dphi_wrt_bp << ", prob = " << prob);
                            if (!(dphi_wrt_bp == 0.)) m_dphi_wrt_bp_prompt->Fill(dphi_wrt_bp);

                        } // end of prompt decay

                        // displaced decay
                        if ( (tru->prodVtx()->perp() > 1.0) and (tru->prodVtx()->perp() < 50.0)) {
                            m_dphi_displaced->Fill(dphi);
                            m_deta_displaced->Fill(deta);

                            // fill dphi wrt to beampipe only if phi is available wrt beampipe
                            if (!(dphi_wrt_bp == 0.)) m_dphi_wrt_bp_displaced->Fill(dphi_wrt_bp);

                        }
                        // far displaced
                        if (tru->prodVtx()->perp() > 50.0 ) {
                            m_dphi_far_displaced->Fill(dphi);
                            m_deta_far_displaced->Fill(deta);

                            // fill dphi wrt to beampipe only if phi is available wrt beampipe
                            if (!(dphi_wrt_bp == 0.)) m_dphi_wrt_bp_far_displaced->Fill(dphi_wrt_bp);

                        }
                    } // end of tru->prodVtx
                }
                // fill plots only for the pair of reco and truth are not matched (fake) by hit
                else {
                    m_dpt_vs_dR_hitFake->Fill(dr, dpt);
                    m_dR_hitFake->Fill(dr);
                }
            }
            //----------------------------------------------------

            if(dr < mindR_tight) {
                reconstructed_dR_tight = true;

                // fill reco pt - truth pt in GeV
                m_dpt_dR_tight->Fill( dpt );
                m_dpt_norm_dR_tight->Fill( dpt );

            } // end of mindR_tight
            if(dr < mindR_loose) {
                reconstructed_dR_loose = true;

                // fill reco pt - truth pt in GeV
                m_dpt_dR_loose->Fill( dpt );
                m_dpt_norm_dR_loose->Fill( dpt );

            } // end of mindR_loose
        }

        // fill histograms for prob. method
        m_fakerate_eta_prob->Fill(idtrk->eta(), !reconstructed_prob);
        m_fakerate_phi_prob->Fill(idtrk->phi(), !reconstructed_prob);
        m_fakerate_pt_prob->Fill( idtrk->pt() / 1000., !reconstructed_prob);
        m_fakerate_d0_prob->Fill(idtrk->d0(), !reconstructed_prob);

        m_fakerate_nolink_eta_prob->Fill(idtrk->eta(), !reconstructed_prob_nolink);
        m_fakerate_nolink_phi_prob->Fill(idtrk->phi(), !reconstructed_prob_nolink);
        m_fakerate_nolink_pt_prob->Fill( idtrk->pt() / 1000., !reconstructed_prob_nolink);
        m_fakerate_nolink_d0_prob->Fill(idtrk->d0(), !reconstructed_prob_nolink);
        

        // fill histograms for dR method
        m_fakerate_eta_dR_tight->Fill(idtrk->eta(), !reconstructed_dR_tight);
        m_fakerate_phi_dR_tight->Fill(idtrk->phi(), !reconstructed_dR_tight);
        m_fakerate_pt_dR_tight->Fill( idtrk->pt() / 1000., !reconstructed_dR_tight);
        m_fakerate_d0_dR_tight->Fill(idtrk->d0(), !reconstructed_dR_tight);

        m_fakerate_eta_dR_loose->Fill(idtrk->eta(), !reconstructed_dR_loose);
        m_fakerate_phi_dR_loose->Fill(idtrk->phi(), !reconstructed_dR_loose);
        m_fakerate_pt_dR_loose->Fill( idtrk->pt() / 1000., !reconstructed_dR_loose);
        m_fakerate_d0_dR_loose->Fill(idtrk->d0(), !reconstructed_dR_loose);

    } // end of reco track loop




    return StatusCode::SUCCESS;
}






float LRTValidation::getMatchingProbability (const xAOD::TrackParticle* trackParticle) {
    float result(std::numeric_limits<float>::quiet_NaN());
    if (trackParticle->isAvailable<float>("truthMatchProbability")){
        result = trackParticle->auxdata<float>("truthMatchProbability" );
    }
    else return 0.;
    return result;
} // end of getMatchingProbability

const xAOD::TruthParticle * LRTValidation::getTruthPtr(const xAOD::TrackParticle * trackParticle){
    typedef ElementLink<xAOD::TruthParticleContainer> ElementTruthLink_t;
    const xAOD::TruthParticle * result(nullptr);
    //0. is there any truth?
    if (trackParticle->isAvailable<ElementTruthLink_t>("truthParticleLink")) {
        //1. ..then get link
        const ElementTruthLink_t ptruthContainer= trackParticle->auxdata<ElementTruthLink_t>("truthParticleLink" );
        if (ptruthContainer.isValid()){
          result= *ptruthContainer;
        }
    }
    return result;
}


