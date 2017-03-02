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
    m_truth_pt = new TH1F("m_truth_pt", "signal truth pt", 50, 0, 1000); // GeV
    m_truth_pt_low = new TH1F("m_truth_pt_low", "signal truth pt", 50, 0, 50); // GeV

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
    m_zp_eta = new TH1F("m_zp_eta", "Z' eta distribution", 50, -3.0, 3.0);
    m_zp_l = new TH1F("m_zp_l", "Z' l distribution", 50, 0, 500);
    m_zp_z = new TH1F("m_zp_z", "Z' z distribution", 100, -500, 500);
    m_zp_R = new TH1F("m_zp_R", "Z' R distribution", 50, 0, 300.0); 
    m_zp_R_low = new TH1F("m_zp_R_low", "Z' R distribution low", 50, 0, 50.0); 
    m_zp_t_barrel = new TH1F("m_zp_t_barrel", "Z' lifetime in lab frame (barrel)", 50, 0, 25);
    m_zp_t_endcap = new TH1F("m_zp_t_endcap", "Z' lifetime in lab frame (endcap)", 50, 0, 25);
    m_fraction_dv_cut = new TProfile("m_fraction_dv_cut","fraction of Z' that decays within R < 2 mm", 1,0,1);

    m_zp_eta_vs_prodVtxR = new TH2F("m_zp_eta_vs_prodVtxR", "signal Z' eta vs prodVtx R", 50, 0, 300, 50, -3.0, 3.0);
    m_truth_zp_acceptance = new TProfile("m_truth_zp_acceptance", "ratio of zp passing eta cut", 50, 0, 300); // zp passing eta over all zp

    m_zp_eta_slice1 = new TH1F("m_zp_eta_slice1", "Z' eta distribution, slice 1", 50, -3.0, 3.0); // eta distribution with 0 < r < 10 mm
    m_zp_eta_slice2 = new TH1F("m_zp_eta_slice2", "Z' eta distribution, slice 2", 50, -3.0, 3.0);  // eta distribution with 10 < r < 50 mm
    m_zp_eta_slice3 = new TH1F("m_zp_eta_slice3", "Z' eta distribution, slice 3", 50, -3.0, 3.0);  // eta distribution with r > 50 mm
    m_zp_eta_slice4 = new TH1F("m_zp_eta_slice4", "Z' eta distribution, slice 4", 50, -3.0, 3.0);  // eta distribution with 40 < r < 60 mm

    m_zp_eta_slice2_pt_slice = new TH1F("m_zp_eta_slice2_pt_slice", "Z' eta distribution, eta slice 2, pt slice", 50, -3.0, 3.0);  // eta distribution with 10 < r < 50 mm, 60 < pT < 80 GeV
    m_zp_eta_slice3_pt_slice = new TH1F("m_zp_eta_slice3_pt_slice", "Z' eta distribution, eta slice 3, pt slice", 50, -3.0, 3.0);  // eta distribution with 10 < r < 50 mm, 180 < pT < 240 GeV

    m_zp_eta_low_pt = new TH1F("m_zp_eta_low_pt", "Z' eta distribution, low pt", 50, -3.0, 3.0);  // eta distribution with pT < 20 GeV
    m_zp_eta_high_pt = new TH1F("m_zp_eta_high_pt", "Z' eta distribution, high pt", 50, -3.0, 3.0);  // eta distribution with 80 < pT < 120 GeV

    m_zp_R_low_pt = new TH1F("m_zp_R_low_pt", "Z' R distribution, low pt", 50, 0, 300.0);  // R distribution with pT < 20 GeV
    m_zp_R_high_pt = new TH1F("m_zp_R_high_pt", "Z' R distribution, high pt", 50, 0, 300.0);  // R distribution with 80 < pT < 120 GeV

    m_zp_pt_slice1 = new TH1F("m_zp_pt_slice1", "Z' pt distribution, slice1", 50, 0, 1000); // pt distribution with 0 < r < 10 mm
    m_zp_pt_slice2 = new TH1F("m_zp_pt_slice2", "Z' pt distribution, slice2", 50, 0, 1000); // pt distribution with 10 < r < 50 mm
    m_zp_pt_slice3 = new TH1F("m_zp_pt_slice3", "Z' pt distribution, slice3", 50, 0, 1000); // pt distribution with r > 50 mm
    m_zp_pt_slice4 = new TH1F("m_zp_pt_slice4", "Z' pt distribution, slice4", 50, 0, 1000); // pt distribution with 40 < r < 60 mm

    // output
    CHECK( histSvc->regHist("/DV/Truth/signal_muon/pt/truth_pt", m_truth_pt) );
    CHECK( histSvc->regHist("/DV/Truth/signal_muon/pt/truth_pt_low", m_truth_pt_low) );
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
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/R/zp_R", m_zp_R) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/R/zp_R_low", m_zp_R_low) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/eta/zp_eta", m_zp_eta) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/l/zp_l", m_zp_l) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/t/zp_t_barrel", m_zp_t_barrel) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/t/zp_t_endcap", m_zp_t_endcap) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/z/zp_z", m_zp_z) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/R/fraction_dv_within_2mm", m_fraction_dv_cut) );


    CHECK( histSvc->regHist("/DV/Truth/signal_zp/eta/zp_eta_vs_prodVtxR", m_zp_eta_vs_prodVtxR) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/eta/truth_zp_acceptance", m_truth_zp_acceptance) );

    CHECK( histSvc->regHist("/DV/Truth/signal_zp/eta/zp_eta_slice1", m_zp_eta_slice1) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/eta/zp_eta_slice2", m_zp_eta_slice2) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/eta/zp_eta_slice3", m_zp_eta_slice3) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/eta/zp_eta_slice4", m_zp_eta_slice4) );

    CHECK( histSvc->regHist("/DV/Truth/signal_zp/eta/zp_eta_slice2_pt_slice", m_zp_eta_slice2_pt_slice) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/eta/zp_eta_slice3_pt_slice", m_zp_eta_slice3_pt_slice) );

    CHECK( histSvc->regHist("/DV/Truth/signal_zp/eta/zp_eta_low_pt", m_zp_eta_low_pt) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/eta/zp_eta_high_pt", m_zp_eta_high_pt) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/R/zp_R_low_pt", m_zp_R_low_pt) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/R/zp_R_high_pt", m_zp_R_high_pt) );

    CHECK( histSvc->regHist("/DV/Truth/signal_zp/pt/zp_pt_slice1", m_zp_pt_slice1) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/pt/zp_pt_slice2", m_zp_pt_slice2) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/pt/zp_pt_slice3", m_zp_pt_slice3) );
    CHECK( histSvc->regHist("/DV/Truth/signal_zp/pt/zp_pt_slice4", m_zp_pt_slice4) );
    
    return StatusCode::SUCCESS;
}

StatusCode TruthPlots::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");

    // normalize distributions
    //m_truth_pt_slice1->Scale( 1/ m_truth_pt_slice1->Integral() );
    //m_truth_pt_slice2->Scale( 1/ m_truth_pt_slice2->Integral() );
    //m_truth_pt_slice3->Scale( 1/ m_truth_pt_slice3->Integral() );
    //m_truth_pt_slice4->Scale( 1/ m_truth_pt_slice4->Integral() );

    //m_truth_eta_slice1->Scale( 1/ m_truth_eta_slice1->Integral() );
    //m_truth_eta_slice2->Scale( 1/ m_truth_eta_slice2->Integral() );
    //m_truth_eta_slice3->Scale( 1/ m_truth_eta_slice3->Integral() );
    //m_truth_eta_slice4->Scale( 1/ m_truth_eta_slice4->Integral() );

    //m_zp_eta_slice1->Scale( 1/m_zp_eta_slice1->Integral() );
    //m_zp_eta_slice2->Scale( 1/m_zp_eta_slice2->Integral() );
    //m_zp_eta_slice3->Scale( 1/m_zp_eta_slice3->Integral() );
    //m_zp_eta_slice4->Scale( 1/m_zp_eta_slice4->Integral() );

    //m_zp_pt_slice1->Scale( 1/m_zp_pt_slice1->Integral() );
    //m_zp_pt_slice2->Scale( 1/m_zp_pt_slice2->Integral() );
    //m_zp_pt_slice3->Scale( 1/m_zp_pt_slice3->Integral() );
    //m_zp_pt_slice4->Scale( 1/m_zp_pt_slice4->Integral() );

    
    return StatusCode::SUCCESS;
}

StatusCode TruthPlots::execute() {  
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    const xAOD::TruthVertexContainer* tru_vc = nullptr;
    CHECK( evtStore()->retrieve( tru_vc, "TruthVertices"));

    // loop over the truth vertex container
    for (auto tru_v: *tru_vc){
        if (!m_dvutils->isSignalVertex(tru_v)) continue;

        // get lowset pt muon
        float min_muon_pt = m_dvutils->GetMinPT(tru_v); // GeV

        // fill muon plots
        for (unsigned int i=0; i < tru_v->nOutgoingParticles(); i++){
            const xAOD::TruthParticle* signal_mu = m_dvutils->FindFinalState(tru_v->outgoingParticle(i));

            // fill signal truth plots
            m_truth_pt_vs_prodVtxR->Fill( tru_v->perp(), signal_mu->pt() / 1000.);
            m_truth_eta_vs_prodVtxR->Fill( tru_v->perp(), signal_mu->eta() );

            // signal muon with lower pT
            m_truth_pt->Fill( min_muon_pt / 1000.);
            m_truth_pt_low->Fill( min_muon_pt / 1000. );

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
        float zp_l = sqrt( tru_v->perp()*tru_v->perp() + tru_v->z()*tru_v->z() );
        float zp_t = ( tru_v->incomingParticle(0)->e() / tru_v->incomingParticle(0)->m() ) * 1.6; // lifetime of 500 mm at lab frame

        // basic plots
        m_zp_R->Fill(tru_v->perp());
        m_zp_R_low->Fill(tru_v->perp());
        m_zp_eta->Fill(zp_eta);
        m_zp_l->Fill(zp_l);
        m_zp_z->Fill(tru_v->z());

        // Delta t (time shift) due to long lifetime
        if (std::abs(zp_eta) < 1) m_zp_t_barrel->Fill(zp_t);
        if (std::abs(zp_eta) > 1 and std::abs(zp_eta) < 2.5) m_zp_t_endcap->Fill(zp_t);

        bool NotPass_R = false;
        if (tru_v->perp() < 2) NotPass_R = true;
        m_fraction_dv_cut->Fill("fraction", NotPass_R);

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

            // also pT slice
            if ( (zp_pt > 60000) and (zp_pt < 80000) ){ // MeV
                m_zp_eta_slice2_pt_slice->Fill(zp_eta);
            }
        }
        if (tru_v->perp() > 50.) {
            m_zp_eta_slice3->Fill(zp_eta);
            m_zp_pt_slice3->Fill(zp_pt / 1000.); // GeV

            // also pT slice
            if ( (zp_pt > 180000) and (zp_pt < 240000) ){ // MeV
                m_zp_eta_slice3_pt_slice->Fill(zp_eta);
            }
        }
        if ( (tru_v->perp() > 40.) and (tru_v->perp() < 60.) ) {
            m_zp_eta_slice4->Fill(zp_eta);
            m_zp_pt_slice4->Fill(zp_pt / 1000.); // GeV
        }

        // Z' distribution separated by pT
        if ( zp_pt < 20000 ) { // MeV
            m_zp_eta_low_pt->Fill(zp_eta);
            m_zp_R_low_pt->Fill(tru_v->perp());
        }
        if ( (zp_pt > 80000) and (zp_pt < 120000)) { // MeV
            m_zp_eta_high_pt->Fill(zp_eta);
            m_zp_R_high_pt->Fill(tru_v->perp());
        }




    } // end of truth vertex loop

    return StatusCode::SUCCESS;
}

