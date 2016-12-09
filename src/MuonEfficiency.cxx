// DisplacedDimuonAnalysis includes
#include "MuonEfficiency.h"

// plotting
#include "GaudiKernel/ITHistSvc.h"
#include "TProfile.h"

// xAOD
#include "xAODTruth/TruthVertex.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/MuonAuxContainer.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODTruth/TruthParticleContainer.h"

#include <vector>


using std::pair;
using std::make_pair;

MuonEfficiency::MuonEfficiency( const std::string& name, ISvcLocator* pSvcLocator ) :
AthAnalysisAlgorithm( name, pSvcLocator ),
m_dvutils("DVUtils")
{

    // initialize tools
    declareProperty("DVUtils", m_dvutils);

}


MuonEfficiency::~MuonEfficiency() {}


StatusCode MuonEfficiency::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");
    
    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());
   
    // signal tracking efficiency
    m_eff_eta = new TProfile("m_eff_eta", "Muon tracking efficiency vs Z' eta", 50,-3.0,3.0);
    m_eff_pt = new TProfile("m_eff_pt", "Muon tracking efficiency vs pt", 50,0,800);
    m_eff_R = new TProfile("m_eff_R", "Muon tracking efficiency vs R", 50,0,400);
    m_eff_d0 = new TProfile("m_eff_d0", "Muon tracking efficiency vs d0", 50,0,400);

    // 2D efficiency
    m_eff_pt_vs_prodVtxR_num = new TH2F("m_eff_pt_vs_prodVtxR_num", "signal eff pt vs prodVtx R, num", 50, 0, 300, 50, 0, 1000); // GeV
    m_eff_pt_vs_prodVtxR_den = new TH2F("m_eff_pt_vs_prodVtxR_den", "signal eff pt vs prodVtx R, den", 50, 0, 300, 50, 0, 1000); // GeV

    m_eff_eta_vs_prodVtxR_num = new TH2F("m_eff_eta_vs_prodVtxR_num", "signal eff eta vs prodVtx R, num", 50, 0, 300, 50, -3.0, 3.0); // GeV
    m_eff_eta_vs_prodVtxR_den = new TH2F("m_eff_eta_vs_prodVtxR_den", "signal eff eta vs prodVtx R, den", 50, 0, 300, 50, -3.0, 3.0); // GeV

    m_eff_pt_vs_d0_num = new TH2F("m_eff_pt_vs_d0_num", "signal eff pt vs d0, num", 50, 0, 300, 50, 0, 1000); // GeV
    m_eff_pt_vs_d0_den = new TH2F("m_eff_pt_vs_d0_den", "signal eff pt vs d0, den", 50, 0, 300, 50, 0, 1000); // GeV

    m_eff_eta_vs_d0_num = new TH2F("m_eff_eta_vs_d0_num", "signal eff eta vs d0, num", 50, 0, 300, 50, -3.0, 3.0); // GeV
    m_eff_eta_vs_d0_den = new TH2F("m_eff_eta_vs_d0_den", "signal eff eta vs d0, den", 50, 0, 300, 50, -3.0, 3.0); // GeV

    // output 
    CHECK( histSvc->regHist("/DV/Muons/Efficiency/eff_eta", m_eff_eta) );
    CHECK( histSvc->regHist("/DV/Muons/Efficiency/eff_pt", m_eff_pt) );
    CHECK( histSvc->regHist("/DV/Muons/Efficiency/eff_R", m_eff_R) );
    CHECK( histSvc->regHist("/DV/Muons/Efficiency/eff_d0", m_eff_d0) );

    // 2D efficiency
    CHECK( histSvc->regHist("/DV/Muons/Efficiency/eff_pt_vs_prodVtxR", m_eff_pt_vs_prodVtxR_num) );
    CHECK( histSvc->regHist("/DV/Muons/Efficiency/eff_eta_vs_prodVtxR", m_eff_eta_vs_prodVtxR_num) );
    CHECK( histSvc->regHist("/DV/Muons/Efficiency/eff_pt_vs_d0", m_eff_pt_vs_d0_num) );
    CHECK( histSvc->regHist("/DV/Muons/Efficiency/eff_eta_vs_d0", m_eff_eta_vs_d0_num) );

    int signal_truth = 0;
    int signal_reconstructed = 0;
    
    return StatusCode::SUCCESS;
}

StatusCode MuonEfficiency::finalize() {

    ATH_MSG_INFO ("Finalizing " << name() << "...");

    // divide 2D histograms to get efficiency plot
    m_eff_pt_vs_prodVtxR_num->Divide(m_eff_pt_vs_prodVtxR_den);
    m_eff_eta_vs_prodVtxR_num->Divide(m_eff_eta_vs_prodVtxR_den);
    m_eff_pt_vs_d0_num->Divide(m_eff_pt_vs_d0_den);
    m_eff_eta_vs_d0_num->Divide(m_eff_eta_vs_d0_den);

    // counting number of signal muons
    ATH_MSG_INFO( "Number of signal truth = " << signal_truth);
    ATH_MSG_INFO( "Number of signal reconstructed = " << signal_reconstructed);


  return StatusCode::SUCCESS;
}

StatusCode MuonEfficiency::execute() {  
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    // get truth container
    const xAOD::TruthParticleContainer* truthParticles = nullptr;
    CHECK( evtStore()->retrieve( truthParticles, "MuonTruthParticles" ) );

    const xAOD::MuonContainer* muc = nullptr;
    CHECK( evtStore()->retrieve( muc, "Muons" ) );
   
    // main muon truth loop for efficiency 
    if (truthParticles) {
        for(auto mu_truth: *truthParticles) {
    
            // truth selection  + signal selection
            if (!m_dvutils->TrackSelection(mu_truth)) continue;
            if (!m_dvutils->isSignal(mu_truth)) continue;

            // count signal truth
            signal_truth++;
            
            bool reconstructed = false;

            if (m_dvutils->IsReconstructedAsMuon(mu_truth)) {
                signal_reconstructed++;
                reconstructed = true;
            } // end of IsReconstructedAsMuon

            // fill efficiency
            m_eff_eta->Fill(mu_truth->eta(), reconstructed);
            m_eff_pt->Fill(mu_truth->pt() / 1000., reconstructed);
            m_eff_R->Fill(mu_truth->prodVtx()->perp(), reconstructed );

            // fill 2D efficiency
            m_eff_pt_vs_prodVtxR_den->Fill(mu_truth->prodVtx()->perp(), mu_truth->pt() / 1000.);
            m_eff_eta_vs_prodVtxR_den->Fill(mu_truth->prodVtx()->perp(), mu_truth->eta() );

            if (reconstructed) {
                m_eff_pt_vs_prodVtxR_num->Fill(mu_truth->prodVtx()->perp(), mu_truth->pt() / 1000.);
                m_eff_eta_vs_prodVtxR_num->Fill(mu_truth->prodVtx()->perp(), mu_truth->eta() );
            }

            // efficiency vs d0
            if (mu_truth->isAvailable<float>("d0") ) {

                float d0 = fabs(mu_truth->auxdata< float >("d0"));
                m_eff_d0->Fill( d0, reconstructed );

                // fill 2D efficiency
                m_eff_pt_vs_d0_den->Fill(d0, mu_truth->pt() / 1000.);
                m_eff_eta_vs_d0_den->Fill(d0, mu_truth->eta() );
                if (reconstructed) {
                    m_eff_pt_vs_d0_num->Fill(d0, mu_truth->pt() / 1000.);
                    m_eff_eta_vs_d0_num->Fill(d0, mu_truth->eta() );
                }
            }

        } // end of truth muon loop
    } // end of truthParticles (truth muon)
    
    return StatusCode::SUCCESS;
}

StatusCode MuonEfficiency::beginInputFile() { 

  return StatusCode::SUCCESS;
}
