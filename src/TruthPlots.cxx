// DisplacedDimuonAnalysis includes
#include "TruthPlots.h"

#include "GaudiKernel/ITHistSvc.h"
#include "TH1F.h"
#include "TProfile.h"

// xAOD
#include "xAODEventInfo/EventInfo.h"

// xAOD Truth
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthVertex.h"
#include "xAODTruth/TruthVertexContainer.h"

// xAOD Muon
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/MuonAuxContainer.h"

// xAOD Vertex
#include "xAODTracking/VertexContainer.h"
#include "cmath"


TruthPlots::TruthPlots( const std::string& name, ISvcLocator* pSvcLocator ) :
AthAnalysisAlgorithm( name, pSvcLocator ),
m_prw("CP::PileupReweightingTool/PileupReweightingTool"),
m_dvutils("DVUtils")
{
    // initialize tools
    declareProperty("DVUtils", m_dvutils);
    declareProperty("PileupReweightingTool", m_prw);
}


TruthPlots::~TruthPlots() {}


StatusCode TruthPlots::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");
    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    // register histograms

    // pile-up distribution
    m_pileup = new TH1F("m_pileup", "m_pileup", 100, 0, 100); 
    m_pileup_reweighted = new TH1F("m_pileup_reweighted", "m_pileup_reweighted", 100, 0, 100); 

    // pile-up weights
    m_p_weight = new TH1F("m_p_weight", "m_p_weight", 100, 0, 100); 

    // signal el
    m_el_m = new TH1F("m_el_m", "signal el m", 100, 0, 1.0); // MeV
    m_el_pt = new TH1F("m_el_pt", "signal el pt", 1000, 0, 1000); // GeV
    m_el_pt_low = new TH1F("m_el_pt_low", "signal el pt", 50, 0, 50); // GeV
    m_el_eta = new TH1F("m_el_eta", "signal el eta", 30, -3.0, 3.0);
    m_el_phi = new TH1F("m_el_phi", "signal el phi", 50, -M_PI, M_PI);
    m_el_R = new TH1F("m_el_R", "signal el R", 50, 0., 300.0);
    m_el_z = new TH1F("m_el_z", "signal el z", 50, -500.0, 500.0);
    m_el_d0 = new TH1F("m_el_d0", "signal el d0", 3000, 0, 300);
    m_el_z0 = new TH1F("m_el_z0", "signal el z0", 100, -500, 500);
    m_el_pdgId = new TH1F("m_el_pdgId", "signal el pdgId", 64, -32, 32);

    // signal muon
    m_muon_m = new TH1F("m_muon_m", "signal muon m", 100, 0, 0.2); // GeV
    m_muon_pt = new TH1F("m_muon_pt", "signal muon pt", 1000, 0, 1000); // GeV
    m_muon_pt_low = new TH1F("m_muon_pt_low", "signal muon pt", 50, 0, 50); // GeV
    m_muon_eta = new TH1F("m_muon_eta", "signal muon eta", 30, -3.0, 3.0);
    m_muon_phi = new TH1F("m_muon_phi", "signal muon phi", 50, -M_PI, M_PI);
    m_muon_R = new TH1F("m_muon_R", "signal muon R", 50, 0., 300.0);
    m_muon_z = new TH1F("m_muon_z", "signal muon z", 50, -500.0, 500.0);
    m_muon_d0 = new TH1F("m_muon_d0", "signal muon d0", 3000, 0, 300);
    m_muon_z0 = new TH1F("m_muon_z0", "signal muon z0", 100, -500, 500);
    m_muon_pdgId = new TH1F("m_muon_pdgId", "signal muon pdgId", 64, -32, 32);

    m_muon_pt_vs_prodVtxR = new TH2F("m_muon_pt_vs_prodVtxR", "signal muon pt vs prodVtx R", 50, 0, 300, 50, 0, 1000); // GeV
    m_muon_eta_vs_prodVtxR = new TH2F("m_muon_eta_vs_prodVtxR", "signal muon eta vs prodVtx R", 50, 0, 300, 30, -3.0, 3.0);
    m_muon_acceptance = new TProfile("m_muon_acceptance", "ratio of muons passing eta cut", 50, 0, 300); // muon passing eta over all muon

    // signal dilep
    m_dilep_m = new TH1F("m_dilep_m", "signal dilep m", 750, 0, 1500); // GeV
    m_dilep_pt = new TH1F("m_dilep_pt", "signal dilep pt", 50, 0, 1000); // GeV
    m_dilep_dr = new TH1F("m_dilep_dr", "signal dilep dr", 50, 0, 2*M_PI);

    // signal Z'
    m_zp_pt = new TH1F("m_zp_pt", "Z' pt distribution", 50, 0, 1000);
    m_zp_eta = new TH1F("m_zp_eta", "Z' eta distribution", 30, -3.0, 3.0);
    m_zp_phi = new TH1F("m_zp_phi", "Z' phi distribution", 50, -M_PI, M_PI);
    m_zp_m = new TH1F("m_zp_m", "Z' m distribution", 750, 0, 1500); // GeV
    m_zp_pdgId = new TH1F("m_zp_pdgId", "signal zp pdgId", 35, 0, 35);
    m_zp_beta = new TH1F("m_zp_beta", "Z' beta distribution", 50, 0, 1.0);
    m_zp_ctau0 = new TH1F("m_zp_ctau0", "Z' ctau0 distribution", 50, 0, 2000);
    m_zp_l = new TH1F("m_zp_l", "Z' l distribution", 50, 0, 500);
    m_zp_z = new TH1F("m_zp_z", "Z' z distribution", 100, -500, 500);
    m_zp_R = new TH1F("m_zp_R", "Z' R distribution", 50, 0, 300.0); 
    m_zp_R_low = new TH1F("m_zp_R_low", "Z' R distribution low", 50, 0, 50.0); 
    m_zp_t_barrel = new TH1F("m_zp_t_barrel", "Z' lifetime in lab frame (barrel)", 50, 0, 25);
    m_zp_t_endcap = new TH1F("m_zp_t_endcap", "Z' lifetime in lab frame (endcap)", 50, 0, 25);

    m_fraction_dv_cut = new TProfile("m_fraction_dv_cut","fraction of Z' that decays within R < 2 mm", 1,0,1);
    m_zp_eta_vs_prodVtxR = new TH2F("m_zp_eta_vs_prodVtxR", "signal Z' eta vs prodVtx R", 50, 0, 300, 30, -3.0, 3.0);
    m_truth_zp_acceptance = new TProfile("m_truth_zp_acceptance", "ratio of zp passing eta cut", 50, 0, 300); // zp passing eta over all zp

    // output

    // pile-up
    CHECK( histSvc->regHist("/DV/truth/pileup", m_pileup) );
    CHECK( histSvc->regHist("/DV/truth/pileup_reweighted", m_pileup_reweighted) );

    // pile-up weight
    CHECK( histSvc->regHist("/DV/truth/p_weight", m_p_weight) );

    // signal el
    CHECK( histSvc->regHist("/DV/truth/signal_el/m/el_m", m_el_m) );
    CHECK( histSvc->regHist("/DV/truth/signal_el/pt/el_pt", m_el_pt) );
    CHECK( histSvc->regHist("/DV/truth/signal_el/pt/el_pt_low", m_el_pt_low) );
    CHECK( histSvc->regHist("/DV/truth/signal_el/eta/el_eta", m_el_eta) );
    CHECK( histSvc->regHist("/DV/truth/signal_el/phi/el_phi", m_el_phi) );
    CHECK( histSvc->regHist("/DV/truth/signal_el/R/el_R", m_el_R) );
    CHECK( histSvc->regHist("/DV/truth/signal_el/z/el_z", m_el_z) );
    CHECK( histSvc->regHist("/DV/truth/signal_el/d0/el_d0", m_el_d0) );
    CHECK( histSvc->regHist("/DV/truth/signal_el/z0/el_z0", m_el_z0) );
    CHECK( histSvc->regHist("/DV/truth/signal_el/pdgId/el_pdgId", m_el_pdgId) );

    // signal muon
    CHECK( histSvc->regHist("/DV/truth/signal_muon/m/muon_m", m_muon_m) );
    CHECK( histSvc->regHist("/DV/truth/signal_muon/pt/muon_pt", m_muon_pt) );
    CHECK( histSvc->regHist("/DV/truth/signal_muon/pt/muon_pt_low", m_muon_pt_low) );
    CHECK( histSvc->regHist("/DV/truth/signal_muon/eta/muon_eta", m_muon_eta) );
    CHECK( histSvc->regHist("/DV/truth/signal_muon/phi/muon_phi", m_muon_phi) );
    CHECK( histSvc->regHist("/DV/truth/signal_muon/R/muon_R", m_muon_R) );
    CHECK( histSvc->regHist("/DV/truth/signal_muon/z/muon_z", m_muon_z) );
    CHECK( histSvc->regHist("/DV/truth/signal_muon/d0/muon_d0", m_muon_d0) );
    CHECK( histSvc->regHist("/DV/truth/signal_muon/z0/muon_z0", m_muon_z0) );
    CHECK( histSvc->regHist("/DV/truth/signal_muon/pdgId/muon_pdgId", m_muon_pdgId) );

    CHECK( histSvc->regHist("/DV/truth/signal_muon/eta/muon_acceptance", m_muon_acceptance) );
    CHECK( histSvc->regHist("/DV/truth/signal_muon/pt/muon_pt_vs_prodVtxR", m_muon_pt_vs_prodVtxR) );
    CHECK( histSvc->regHist("/DV/truth/signal_muon/eta/muon_eta_vs_prodVtxR", m_muon_eta_vs_prodVtxR) );

    // signal dilep
    CHECK( histSvc->regHist("/DV/truth/signal_dilep/m/dilep_m", m_dilep_m) );
    CHECK( histSvc->regHist("/DV/truth/signal_dilep/pt/dilep_pt", m_dilep_pt) );
    CHECK( histSvc->regHist("/DV/truth/signal_dilep/dr/dilep_dr", m_dilep_dr) );

    // signal Z'
    CHECK( histSvc->regHist("/DV/truth/signal_zp/m/zp_m", m_zp_m) );
    CHECK( histSvc->regHist("/DV/truth/signal_zp/pt/zp_pt", m_zp_pt) );
    CHECK( histSvc->regHist("/DV/truth/signal_zp/eta/zp_eta", m_zp_eta) );
    CHECK( histSvc->regHist("/DV/truth/signal_zp/phi/zp_phi", m_zp_phi) );
    CHECK( histSvc->regHist("/DV/truth/signal_zp/R/zp_R", m_zp_R) );
    CHECK( histSvc->regHist("/DV/truth/signal_zp/R/zp_R_low", m_zp_R_low) );
    CHECK( histSvc->regHist("/DV/truth/signal_zp/pdgId/zp_pdgId", m_zp_pdgId) );
    CHECK( histSvc->regHist("/DV/truth/signal_zp/beta/zp_beta", m_zp_beta) );
    CHECK( histSvc->regHist("/DV/truth/signal_zp/ctau0/zp_ctau0", m_zp_ctau0) );
    CHECK( histSvc->regHist("/DV/truth/signal_zp/l/zp_l", m_zp_l) );
    CHECK( histSvc->regHist("/DV/truth/signal_zp/t/zp_t_barrel", m_zp_t_barrel) );
    CHECK( histSvc->regHist("/DV/truth/signal_zp/t/zp_t_endcap", m_zp_t_endcap) );
    CHECK( histSvc->regHist("/DV/truth/signal_zp/z/zp_z", m_zp_z) );

    CHECK( histSvc->regHist("/DV/truth/signal_zp/R/fraction_dv_within_2mm", m_fraction_dv_cut) );
    CHECK( histSvc->regHist("/DV/truth/signal_zp/eta/zp_eta_vs_prodVtxR", m_zp_eta_vs_prodVtxR) );
    CHECK( histSvc->regHist("/DV/truth/signal_zp/eta/truth_zp_acceptance", m_truth_zp_acceptance) );


    return StatusCode::SUCCESS;
}

StatusCode TruthPlots::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");
    return StatusCode::SUCCESS;
}

StatusCode TruthPlots::execute() {  
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    // retrieve event info
    const xAOD::EventInfo* evtInfo = nullptr;
    CHECK( evtStore()->retrieve( evtInfo, "EventInfo" ) );

    //  // get combine weight from pileup reweighting tool
    //  float p_weight = m_prw->getCombinedWeight(*evtInfo);

    //  // pile-up
    //  int pileup = evtInfo->actualInteractionsPerCrossing();
    //  m_pileup->Fill(pileup);
    //  m_pileup_reweighted->Fill(pileup,p_weight);

    //  // get pile-up weights
    //  m_p_weight->SetBinContent(pileup+1,p_weight);
    //  ATH_MSG_DEBUG("mu = " << pileup << ", weight = " << p_weight << ", bin center = " << m_p_weight->GetBinCenter(pileup+1));

    const xAOD::TruthVertexContainer* tru_vc = nullptr;
    CHECK( evtStore()->retrieve( tru_vc, "TruthVertices"));

    // loop over the truth vertex container
    for (auto tru_v: *tru_vc){
        if (!m_dvutils->isSignalVertex(tru_v)) continue;

        // get lowset pt muon
        //float min_muon_pt = m_dvutils->GetMinPT(tru_v); // GeV

        // fill lepton plots
        for (unsigned int i=0; i < tru_v->nOutgoingParticles(); i++){
            const xAOD::TruthParticle* signal_lep = m_dvutils->FindFinalState(tru_v->outgoingParticle(i));

            //------------------------------
            // signal el basic plots
            //------------------------------
            if (std::abs(signal_lep->pdgId()) == 11){
                m_el_m->Fill(signal_lep->m()); // MeV
                m_el_eta->Fill(signal_lep->eta());
                m_el_pdgId->Fill(signal_lep->pdgId());
                m_el_phi->Fill(signal_lep->phi());
                m_el_R->Fill(tru_v->perp());
                m_el_z->Fill(tru_v->z());
                m_el_d0->Fill(signal_lep->auxdata< float >("d0"));
                m_el_z0->Fill(signal_lep->auxdata< float >("z0"));
                m_el_pt->Fill( signal_lep->pt() / 1000.);
                m_el_pt_low->Fill( signal_lep->pt() / 1000. );
            }

            //------------------------------
            // signal muon basic plots
            //------------------------------
            if (std::abs(signal_lep->pdgId()) == 13){
                m_muon_m->Fill(signal_lep->m() / 1000.); // GeV
                m_muon_eta->Fill(signal_lep->eta());
                m_muon_pdgId->Fill(signal_lep->pdgId());
                m_muon_phi->Fill(signal_lep->phi());
                m_muon_R->Fill(tru_v->perp());
                m_muon_z->Fill(tru_v->z());
                m_muon_d0->Fill(signal_lep->auxdata< float >("d0"));
                m_muon_z0->Fill(signal_lep->auxdata< float >("z0"));
                m_muon_pt->Fill( signal_lep->pt() / 1000.);
                m_muon_pt_low->Fill( signal_lep->pt() / 1000. );

                // fill signal muon plots
                m_muon_pt_vs_prodVtxR->Fill( tru_v->perp(), signal_lep->pt() / 1000.);
                m_muon_eta_vs_prodVtxR->Fill( tru_v->perp(), signal_lep->eta() );

                // fill muon acceptance ratio
                bool accepted =false;
                if (signal_lep->eta() < 2.4) accepted = true;
                m_muon_acceptance->Fill( tru_v->perp(), accepted);
            }

        } // end of outgoing particle loop

        //------------------------------
        // signal muon basic plots
        //------------------------------
        float dilep_m = m_dvutils->TruthMass(tru_v);
        float dilep_pt = m_dvutils->TruthPt(tru_v);
        float dilep_dr = m_dvutils->Truth_dr(tru_v);

        m_dilep_m->Fill(dilep_m / 1000.);
        m_dilep_pt->Fill(dilep_pt / 1000.);
        m_dilep_dr->Fill(dilep_dr);

        //------------------------------
        // fill Z' plots
        //------------------------------
        float zp_eta = tru_v->incomingParticle(0)->eta();
        float zp_phi = tru_v->incomingParticle(0)->phi();
        float zp_m = tru_v->incomingParticle(0)->m();
        float zp_e = tru_v->incomingParticle(0)->e();
        float zp_pt = tru_v->incomingParticle(0)->pt();
        float zp_R = tru_v->perp();
        float zp_z = tru_v->z();
        float zp_l = sqrt(zp_R * zp_R+ zp_z * zp_z);
        float zp_t = zp_e/zp_m * 1.6; // lifetime of 500 mm at lab frame
        float zp_beta = sqrt(1 - (zp_m/zp_e)*(zp_m/zp_e));
        float zp_ctau0 = zp_l / sqrt((zp_e/zp_m)*(zp_e/zp_m) - 1);

        // basic plots
        m_zp_m->Fill(zp_m / 1000.);
        m_zp_pt->Fill(zp_pt / 1000.);
        m_zp_eta->Fill(zp_eta);
        m_zp_phi->Fill(zp_phi);
        m_zp_pdgId->Fill(tru_v->incomingParticle(0)->pdgId());
        m_zp_beta->Fill(zp_beta);
        m_zp_ctau0->Fill(zp_ctau0);

        m_zp_R->Fill(zp_R);
        m_zp_R_low->Fill(zp_R);
        m_zp_l->Fill(zp_l);
        m_zp_z->Fill(zp_z);

        // 2D eta vs r distribution
        m_zp_eta_vs_prodVtxR->Fill( zp_R, zp_eta);

        // Delta t (time shift) due to long lifetime
        if (std::abs(zp_eta) < 1) m_zp_t_barrel->Fill(zp_t);
        if (std::abs(zp_eta) > 1 and std::abs(zp_eta) < 2.5) m_zp_t_endcap->Fill(zp_t);

        // count fraction of Z' within 2 mm
        bool NotPass_R = false;
        if (zp_R < 2) NotPass_R = true;
        m_fraction_dv_cut->Fill("fraction", NotPass_R);

        // find ratio between Z's that pass acceptance (eta <2.4) to all Z'
        bool zp_passed = false;
        if (zp_eta < 2.4) zp_passed = true;
        m_truth_zp_acceptance->Fill( zp_R, zp_passed);

    } // end of truth vertex loop

    return StatusCode::SUCCESS;
}

