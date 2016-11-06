// DisplacedDimuonAnalysis includes
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
    m_dpt_prob = new TH1F("dpt_prb", "reco pt - truth pt, using prob. method", 50, -30, 30);
    m_dpt_dR_tight = new TH1F("dpt_dR_tight", "reco pt - truth pt, using dR_tight. method", 50, -30, 30);
    m_dpt_dR_loose = new TH1F("dpt_dR_loose", "reco pt - truth pt, using dR_loose. method", 50, -30, 30);

    m_dpt_norm_prob = new TH1F("dpt_norm_prb", "reco pt - truth pt, using prob. method", 50, -30, 30);
    m_dpt_norm_dR_tight = new TH1F("dpt_norm_dR_tight", "reco pt - truth pt, using dR_tight. method", 50, -30, 30);
    m_dpt_norm_dR_loose = new TH1F("dpt_norm_dR_loose", "reco pt - truth pt, using dR_loose. method", 50, -30, 30);

    m_prob_dist = new TH1F("prob_dist", "prob distribution per signal truth", 10, 0.05, 1.05);
    m_dR_dist = new TH1F("dR_dist", "dR distribution per signal truth", 10, 0.05, 1.05);


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
    
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/Prob/prob_dist", m_prob_dist) );
    CHECK( histSvc->regHist("/DV/LRTValidation/MatchingQuality/dR/dR_dist", m_dR_dist) );


    float signal_truth = 0;
    float signal_reconstructed = 0;
    
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

    // divide histogram by total number of truth
    // to obtain average number of reco tracks per truth
    m_prob_dist->Scale( 1/signal_truth);
    m_dR_dist->Scale( 1/signal_truth);

    // correction for division
    m_prob_dist->Sumw2();
    m_dR_dist->Sumw2();
    
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
                m_prob_dist->Fill(prob);

                // fill if we have matching reco track
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

            // fill dR. distribution per truth
            m_dR_dist->Fill(dr);

            // set reconstruction flag
            if(dr < mindR_tight) reconstructed_dR_tight = true;
            if(dr < mindR_loose) reconstructed_dR_loose = true;

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
        if (!m_dvutils->isLargeD0Track(idtrk)) continue;

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

        // access matching probability
	    float prob = getMatchingProbability(idtrk);
        if (prob < minProbEffLow) {
            if (hasTruthLink) reconstructed_prob = false;
            else reconstructed_prob_nolink = false;
        }

        // calculate difference in pT for matching reco tracks
        if (prob > minProbEffLow and hasTruthLink) {
            float dpt = (idtrk->pt() - associatedTruth->pt() ) / 1000.; // in GeV
            
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

            // calculate dr between truth particle and indet track particle
            double dr = tru->p4().DeltaR(idtrk->p4());
            float dpt = (idtrk->pt() - tru->pt() ) / 1000.; // in GeV

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


