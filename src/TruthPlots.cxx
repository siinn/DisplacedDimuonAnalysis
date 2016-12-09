// DisplacedDimuonAnalysis includes
#include "TruthPlots.h"

#include "GaudiKernel/ITHistSvc.h"
#include "TH1F.h"
#include "TProfile.h"

// xAOD Truth
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthVertex.h"
#include "xAODTruth/TruthVertexContainer.h"

// xAOD Muon
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/MuonAuxContainer.h"

// xAOD Vertex
#include "xAODTracking/VertexContainer.h"


TruthPlots::TruthPlots( const std::string& name, ISvcLocator* pSvcLocator ) :
AthAnalysisAlgorithm( name, pSvcLocator ),
m_dvutils("DVUtils")
{
    // initialize tools
    declareProperty("DVUtils", m_dvutils);
}


TruthPlots::~TruthPlots() {}


StatusCode TruthPlots::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");
    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    // register histograms

    // signal muon
    m_truth_pt_slice1 = new TH1F("m_truth_pt_slice1", "signal truth pt_slice1", 50, 0, 1000); // GeV
    m_truth_pt_slice2 = new TH1F("m_truth_pt_slice2", "signal truth pt_slice2", 50, 0, 1000); // GeV
    m_truth_pt_slice3 = new TH1F("m_truth_pt_slice3", "signal truth pt_slice3", 50, 0, 1000); // GeV
    m_truth_pt_slice4 = new TH1F("m_truth_pt_slice4", "signal truth pt_slice4", 50, 0, 1000); // GeV

    m_truth_eta_slice1 = new TH1F("m_truth_eta_slice1", "signal truth eta_slice1", 50, -3.0, 3.0); // GeV
    m_truth_eta_slice2 = new TH1F("m_truth_eta_slice2", "signal truth eta_slice2", 50, -3.0, 3.0); // GeV
    m_truth_eta_slice3 = new TH1F("m_truth_eta_slice3", "signal truth eta_slice3", 50, -3.0, 3.0); // GeV
    m_truth_eta_slice4 = new TH1F("m_truth_eta_slice4", "signal truth eta_slice4", 50, -3.0, 3.0); // GeV

    m_truth_pt_vs_prodVtxR = new TH2F("m_truth_pt_vs_prodVtxR", "signal truth pt vs prodVtx R", 50, 0, 300, 50, 0, 1000); // GeV
    m_truth_eta_vs_prodVtxR = new TH2F("m_truth_eta_vs_prodVtxR", "signal truth eta vs prodVtx R", 50, 0, 300, 50, -3.0, 3.0); // GeV
    m_truth_muon_acceptance = new TProfile("m_truth_muon_acceptance", "ratio of muons passing eta cut", 50, 0, 300); // muon passing eta over all muon

    // signal Z'
    m_zp_eta_vs_prodVtxR = new TH2F("m_zp_eta", "signal Z' eta vs prodVtx R", 50, 0, 300, 50, -3.0, 3.0);
    m_truth_zp_acceptance = new TProfile("m_truth_zp_acceptance", "ratio of zp passing eta cut", 50, 0, 300); // zp passing eta over all zp

    m_zp_eta_slice1 = new TH1F("m_zp_eta_slice1", "Z' eta distribution, slice 1", 50, -3.0, 3.0); // eta distribution with 0 < r < 10 mm
    m_zp_eta_slice2 = new TH1F("m_zp_eta_slice2", "Z' eta distribution, slice 2", 50, -3.0, 3.0);  // eta distribution with 10 < r < 50 mm
    m_zp_eta_slice3 = new TH1F("m_zp_eta_slice3", "Z' eta distribution, slice 3", 50, -3.0, 3.0);  // eta distribution with r > 50 mm
    m_zp_eta_slice4 = new TH1F("m_zp_eta_slice4", "Z' eta distribution, slice 3", 50, -3.0, 3.0);  // eta distribution with 40 < r < 60 mm

    m_zp_pt_slice1 = new TH1F("m_zp_pt_slice1", "Z' pt distribution, slice1", 50, 0, 1000); // pt distribution with 0 < r < 10 mm
    m_zp_pt_slice2 = new TH1F("m_zp_pt_slice2", "Z' pt distribution, slice2", 50, 0, 1000); // pt distribution with 10 < r < 50 mm
    m_zp_pt_slice3 = new TH1F("m_zp_pt_slice3", "Z' pt distribution, slice3", 50, 0, 1000); // pt distribution with r > 50 mm
    m_zp_pt_slice4 = new TH1F("m_zp_pt_slice4", "Z' pt distribution, slice4", 50, 0, 1000); // pt distribution with 40 < r < 60 mm

    // output
    CHECK( histSvc->regHist("/DV/Truth/signal_muon/pt/truth_pt_slice1", m_truth_pt_slice1) );
    CHECK( histSvc->regHist("/DV/Truth/signal_muon/pt/truth_pt_slice2", m_truth_pt_slice2) );
    CHECK( histSvc->regHist("/DV/Truth/signal_muon/pt/truth_pt_slice3", m_truth_pt_slice3) );
    CHECK( histSvc->regHist("/DV/Truth/signal_muon/pt/truth_pt_slice4", m_truth_pt_slice4) );

    CHECK( histSvc->regHist("/DV/Truth/signal_muon/eta/truth_eta_slice1", m_truth_eta_slice1) );
    CHECK( histSvc->regHist("/DV/Truth/signal_muon/eta/truth_eta_slice2", m_truth_eta_slice2) );
    CHECK( histSvc->regHist("/DV/Truth/signal_muon/eta/truth_eta_slice3", m_truth_eta_slice3) );
    CHECK( histSvc->regHist("/DV/Truth/signal_muon/eta/truth_eta_slice4", m_truth_eta_slice4) );

    CHECK( histSvc->regHist("/DV/Truth/signal_muon/eta/truth_muon_acceptance", m_truth_muon_acceptance) );

    CHECK( histSvc->regHist("/DV/Truth/signal_muon/pt/m_truth_pt_vs_prodVtxR", m_truth_pt_vs_prodVtxR) );
    CHECK( histSvc->regHist("/DV/Truth/signal_muon/eta/m_truth_eta_vs_prodVtxR", m_truth_eta_vs_prodVtxR) );

    // signal Z'
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/eta/zp_eta_vs_prodVtxR", m_zp_eta_vs_prodVtxR) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/eta/truth_zp_acceptance", m_truth_zp_acceptance) );

    CHECK( histSvc->regHist("/DV/Truth/signal_zp/eta/zp_eta_slice1", m_zp_eta_slice1) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/eta/zp_eta_slice2", m_zp_eta_slice2) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/eta/zp_eta_slice3", m_zp_eta_slice3) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/eta/zp_eta_slice4", m_zp_eta_slice4) );

    CHECK( histSvc->regHist("/DV/Truth/signal_zp/pt/zp_pt_slice1", m_zp_pt_slice1) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/pt/zp_pt_slice2", m_zp_pt_slice2) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/pt/zp_pt_slice3", m_zp_pt_slice3) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/pt/zp_pt_slice4", m_zp_pt_slice4) );
    
    return StatusCode::SUCCESS;
}

StatusCode TruthPlots::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");

    // normalize distributions
    m_truth_pt_slice1->Scale( 1/ m_truth_pt_slice1->Integral() );
    m_truth_pt_slice2->Scale( 1/ m_truth_pt_slice2->Integral() );
    m_truth_pt_slice3->Scale( 1/ m_truth_pt_slice3->Integral() );
    m_truth_pt_slice4->Scale( 1/ m_truth_pt_slice4->Integral() );

    m_truth_eta_slice1->Scale( 1/ m_truth_eta_slice1->Integral() );
    m_truth_eta_slice2->Scale( 1/ m_truth_eta_slice2->Integral() );
    m_truth_eta_slice3->Scale( 1/ m_truth_eta_slice3->Integral() );
    m_truth_eta_slice4->Scale( 1/ m_truth_eta_slice4->Integral() );

    m_zp_eta_slice1->Scale( 1/m_zp_eta_slice1->Integral() );
    m_zp_eta_slice2->Scale( 1/m_zp_eta_slice2->Integral() );
    m_zp_eta_slice3->Scale( 1/m_zp_eta_slice3->Integral() );
    m_zp_eta_slice4->Scale( 1/m_zp_eta_slice4->Integral() );

    m_zp_pt_slice1->Scale( 1/m_zp_pt_slice1->Integral() );
    m_zp_pt_slice2->Scale( 1/m_zp_pt_slice2->Integral() );
    m_zp_pt_slice3->Scale( 1/m_zp_pt_slice3->Integral() );
    m_zp_pt_slice4->Scale( 1/m_zp_pt_slice4->Integral() );

    
    return StatusCode::SUCCESS;
}

StatusCode TruthPlots::execute() {  
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    const xAOD::TruthVertexContainer* tru_vc = nullptr;
    CHECK( evtStore()->retrieve( tru_vc, "TruthVertices"));

    // loop over the truth vertex container
    for (auto tru_v: *tru_vc){
        if (!m_dvutils->isSignalVertex(tru_v)) continue;

        // fill muon plots
        for (unsigned int i=0; i < tru_v->nOutgoingParticles(); i++){
            const xAOD::TruthParticle* signal_mu = m_dvutils->FindFinalState(tru_v->outgoingParticle(i));

            // fill signal truth plots
            m_truth_pt_vs_prodVtxR->Fill( tru_v->perp(), signal_mu->pt() / 1000.);
            m_truth_eta_vs_prodVtxR->Fill( tru_v->perp(), signal_mu->eta() );

            // signal muon distribution by slice in r
            if ( tru_v->perp() < 10. ) {
                m_truth_pt_slice1->Fill( signal_mu->pt() / 1000. );
                m_truth_eta_slice1->Fill( signal_mu->eta() );
            }
            if ( (tru_v->perp() > 10.) and (tru_v->perp() < 50.) ) {
                m_truth_pt_slice2->Fill( signal_mu->pt() / 1000. );
                m_truth_eta_slice2->Fill( signal_mu->eta() );
            }
            if (tru_v->perp() > 50.) {
                m_truth_pt_slice3->Fill( signal_mu->pt() / 1000. );
                m_truth_eta_slice3->Fill( signal_mu->eta() );
            }
            if ( (tru_v->perp() > 40.) and (tru_v->perp() < 60.) ) {
                m_truth_pt_slice4->Fill( signal_mu->pt() / 1000. );
                m_truth_eta_slice4->Fill( signal_mu->eta() );
            }

            // fill muon acceptance ratio
            bool accepted =false;
            if (signal_mu->eta() < 2.4) accepted = true;
            m_truth_muon_acceptance->Fill( tru_v->perp(), accepted);

        } // end of outgoing particle loop

        // fill Z' plots
        float zp_eta = tru_v->incomingParticle(0)->eta();
        float zp_pt = tru_v->incomingParticle(0)->pt();

        ATH_MSG_INFO( "z' eta = " << zp_eta << ", dv eta = " << tru_v->eta() );

        // 2D eta vs r distribution
        m_zp_eta_vs_prodVtxR->Fill( tru_v->perp(), zp_eta);

        // find ratio between Z's that pass acceptance (eta <2.4) to all Z'
        bool zp_passed = false;
        if (zp_eta < 2.4) zp_passed = true;
        m_truth_zp_acceptance->Fill( tru_v->perp(), zp_passed);

        // Z' distribution sliced by r
        if ( tru_v->perp() < 10. ) {
            m_zp_eta_slice1->Fill(zp_eta);
            m_zp_pt_slice1->Fill(zp_pt / 1000.); // GeV
        }
        if ( (tru_v->perp() > 10.) and (tru_v->perp() < 50.) ) {
            m_zp_eta_slice2->Fill(zp_eta);
            m_zp_pt_slice2->Fill(zp_pt / 1000.); // GeV
        }
        if (tru_v->perp() > 50.) {
            m_zp_eta_slice3->Fill(zp_eta);
            m_zp_pt_slice3->Fill(zp_pt / 1000.); // GeV
        }
        if ( (tru_v->perp() > 40.) and (tru_v->perp() < 60.) ) {
            m_zp_eta_slice4->Fill(zp_eta);
            m_zp_pt_slice4->Fill(zp_pt / 1000.); // GeV
        }


    } // end of truth vertex loop

    return StatusCode::SUCCESS;
}

