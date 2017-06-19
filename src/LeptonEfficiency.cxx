// DisplacedDimuonAnalysis includes
#include "LeptonEfficiency.h"

// plotting
#include "GaudiKernel/ITHistSvc.h"
#include "TProfile.h"

// xAOD
#include "xAODTruth/TruthVertex.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/MuonAuxContainer.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthVertexContainer.h"

#include <vector>


using std::pair;
using std::make_pair;

LeptonEfficiency::LeptonEfficiency( const std::string& name, ISvcLocator* pSvcLocator ) :
AthAnalysisAlgorithm( name, pSvcLocator ),
m_tdt("Trig::TrigDecisionTool/TrigDecisionTool"),
m_dvutils("DVUtils")
{

    // initialize tools
    declareProperty("DVUtils", m_dvutils);
    declareProperty("TrigDecisionTool", m_tdt);

}


LeptonEfficiency::~LeptonEfficiency() {}


StatusCode LeptonEfficiency::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");
    
    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());
   
    // signal tracking efficiency
    m_eff_mu_eta = new TProfile("m_eff_mu_eta", "Muon tracking efficiency vs Z' eta", 50,-3.0,3.0);
    m_eff_mu_pt = new TProfile("m_eff_mu_pt", "Muon tracking efficiency vs pt", 50,0,800);
    m_eff_mu_R = new TProfile("m_eff_mu_R", "Muon tracking efficiency vs R", 50,0,400);
    m_eff_mu_d0 = new TProfile("m_eff_mu_d0", "Muon tracking efficiency vs d0", 50,0,400);

    m_eff_el_eta = new TProfile("m_eff_el_eta", "Electron tracking efficiency vs Z' eta", 50,-3.0,3.0);
    m_eff_el_pt = new TProfile("m_eff_el_pt", "Electron tracking efficiency vs pt", 50,0,800);
    m_eff_el_R = new TProfile("m_eff_el_R", "Electron tracking efficiency vs R", 50,0,400);
    m_eff_el_d0 = new TProfile("m_eff_el_d0", "Electron tracking efficiency vs d0", 50,0,400);

    // 2D efficiency
    m_eff_mu_pt_vs_prodVtxR_num = new TH2F("m_eff_mu_pt_vs_prodVtxR_num", "signal mu eff pt vs prodVtx R, num", 50, 0, 300, 50, 0, 1000); // GeV
    m_eff_mu_pt_vs_prodVtxR_den = new TH2F("m_eff_mu_pt_vs_prodVtxR_den", "signal mu eff pt vs prodVtx R, den", 50, 0, 300, 50, 0, 1000); // GeV
    m_eff_mu_eta_vs_prodVtxR_num = new TH2F("m_eff_mu_eta_vs_prodVtxR_num", "signal mu eff eta vs prodVtx R, num", 50, 0, 300, 50, -3.0, 3.0); // GeV
    m_eff_mu_eta_vs_prodVtxR_den = new TH2F("m_eff_mu_eta_vs_prodVtxR_den", "signal mu eff eta vs prodVtx R, den", 50, 0, 300, 50, -3.0, 3.0); // GeV
    m_eff_mu_pt_vs_d0_num = new TH2F("m_eff_mu_pt_vs_d0_num", "signal muon eff pt vs d0, num", 50, 0, 300, 50, 0, 1000); // GeV
    m_eff_mu_pt_vs_d0_den = new TH2F("m_eff_mu_pt_vs_d0_den", "signal muon eff pt vs d0, den", 50, 0, 300, 50, 0, 1000); // GeV
    m_eff_mu_eta_vs_d0_num = new TH2F("m_eff_mu_eta_vs_d0_num", "signal muon eff eta vs d0, num", 50, 0, 300, 50, -3.0, 3.0); // GeV
    m_eff_mu_eta_vs_d0_den = new TH2F("m_eff_mu_eta_vs_d0_den", "signal muon eff eta vs d0, den", 50, 0, 300, 50, -3.0, 3.0); // GeV

    m_eff_el_pt_vs_d0_num = new TH2F("m_eff_el_pt_vs_d0_num", "signal electron eff pt vs d0, num", 50, 0, 300, 50, 0, 1000); // GeV
    m_eff_el_pt_vs_d0_den = new TH2F("m_eff_el_pt_vs_d0_den", "signal electron eff pt vs d0, den", 50, 0, 300, 50, 0, 1000); // GeV
    m_eff_el_eta_vs_d0_num = new TH2F("m_eff_el_eta_vs_d0_num", "signal electron eff eta vs d0, num", 50, 0, 300, 50, -3.0, 3.0); // GeV
    m_eff_el_eta_vs_d0_den = new TH2F("m_eff_el_eta_vs_d0_den", "signal electron eff eta vs d0, den", 50, 0, 300, 50, -3.0, 3.0); // GeV
    m_eff_el_pt_vs_prodVtxR_num = new TH2F("m_eff_el_pt_vs_prodVtxR_num", "signal el eff pt vs prodVtx R, num", 50, 0, 300, 50, 0, 1000); // GeV
    m_eff_el_pt_vs_prodVtxR_den = new TH2F("m_eff_el_pt_vs_prodVtxR_den", "signal el eff pt vs prodVtx R, den", 50, 0, 300, 50, 0, 1000); // GeV

    m_eff_el_eta_vs_prodVtxR_num = new TH2F("m_eff_el_eta_vs_prodVtxR_num", "signal el eff eta vs prodVtx R, num", 50, 0, 300, 50, -3.0, 3.0); // GeV
    m_eff_el_eta_vs_prodVtxR_den = new TH2F("m_eff_el_eta_vs_prodVtxR_den", "signal el eff eta vs prodVtx R, den", 50, 0, 300, 50, -3.0, 3.0); // GeV


    // output 
    CHECK( histSvc->regHist("/DV/muons/efficiency/eff_mu_eta", m_eff_mu_eta) );
    CHECK( histSvc->regHist("/DV/muons/efficiency/eff_mu_pt", m_eff_mu_pt) );
    CHECK( histSvc->regHist("/DV/muons/efficiency/eff_mu_R", m_eff_mu_R) );
    CHECK( histSvc->regHist("/DV/muons/efficiency/eff_mu_d0", m_eff_mu_d0) );

    CHECK( histSvc->regHist("/DV/electrons/efficiency/eff_el_eta", m_eff_el_eta) );
    CHECK( histSvc->regHist("/DV/electrons/efficiency/eff_el_pt", m_eff_el_pt) );
    CHECK( histSvc->regHist("/DV/electrons/efficiency/eff_el_R", m_eff_el_R) );
    CHECK( histSvc->regHist("/DV/electrons/efficiency/eff_el_d0", m_eff_el_d0) );

    // 2D efficiency
    CHECK( histSvc->regHist("/DV/muons/efficiency/eff_mu_pt_vs_prodVtxR", m_eff_mu_pt_vs_prodVtxR_num) );
    CHECK( histSvc->regHist("/DV/muons/efficiency/eff_mu_eta_vs_prodVtxR", m_eff_mu_eta_vs_prodVtxR_num) );
    CHECK( histSvc->regHist("/DV/muons/efficiency/eff_mu_pt_vs_d0", m_eff_mu_pt_vs_d0_num) );
    CHECK( histSvc->regHist("/DV/muons/efficiency/eff_mu_eta_vs_d0", m_eff_mu_eta_vs_d0_num) );

    CHECK( histSvc->regHist("/DV/electrons/efficiency/eff_el_pt_vs_prodVtxR", m_eff_el_pt_vs_prodVtxR_num) );
    CHECK( histSvc->regHist("/DV/electrons/efficiency/eff_el_eta_vs_prodVtxR", m_eff_el_eta_vs_prodVtxR_num) );
    CHECK( histSvc->regHist("/DV/electrons/efficiency/eff_el_pt_vs_d0", m_eff_el_pt_vs_d0_num) );
    CHECK( histSvc->regHist("/DV/electrons/efficiency/eff_el_eta_vs_d0", m_eff_el_eta_vs_d0_num) );

    int signal_mu_truth = 0;
    int signal_el_truth = 0;
    int signal_zp_truth = 0;
    int m_event_count = 0;
    bool signal_mu_reconstructed = false;
    bool signal_el_reconstructed = false;
    
    return StatusCode::SUCCESS;
}

StatusCode LeptonEfficiency::finalize() {

    ATH_MSG_INFO ("Finalizing " << name() << "...");

    if (signal_mu_truth > 0.){
        // divide 2D histograms to get efficiency plot
        m_eff_mu_pt_vs_prodVtxR_num->Divide(m_eff_mu_pt_vs_prodVtxR_den);
        m_eff_mu_eta_vs_prodVtxR_num->Divide(m_eff_mu_eta_vs_prodVtxR_den);
        m_eff_mu_pt_vs_d0_num->Divide(m_eff_mu_pt_vs_d0_den);
        m_eff_mu_eta_vs_d0_num->Divide(m_eff_mu_eta_vs_d0_den);
    }
    if (signal_el_truth > 0.){
        m_eff_el_pt_vs_prodVtxR_num->Divide(m_eff_el_pt_vs_prodVtxR_den);
        m_eff_el_eta_vs_prodVtxR_num->Divide(m_eff_el_eta_vs_prodVtxR_den);
        m_eff_el_pt_vs_d0_num->Divide(m_eff_el_pt_vs_d0_den);
        m_eff_el_eta_vs_d0_num->Divide(m_eff_el_eta_vs_d0_den);
    }

    // counting number of signal muons
    ATH_MSG_INFO( "Number of event processed = " << m_event_count);
    ATH_MSG_INFO( "Number of signal zp truth = " << signal_zp_truth);
    ATH_MSG_INFO( "Number of signal muon truth = " << signal_mu_truth);
    ATH_MSG_INFO( "Number of signal electron truth = " << signal_el_truth);
    ATH_MSG_INFO( "Number of signal mu reconstructed = " << signal_mu_reconstructed);
    ATH_MSG_INFO( "Number of signal el reconstructed = " << signal_el_reconstructed);


  return StatusCode::SUCCESS;
}

StatusCode LeptonEfficiency::execute() {  
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    // trigger filter for testing only
    ////bool trig_passed = false;
    //bool trig_passed = false;

    //if (m_tdt->isPassed("HLT_mu60_0eta105_msonly")) trig_passed = true;
    //if (m_tdt->isPassed("HLT_g140_loose")) trig_passed = true;
    //if (m_tdt->isPassed("HLT_2g50_loose")) trig_passed = true;
    //if (m_tdt->isPassed("HLT_2g60_loose_L12EM15VH")) trig_passed = true;

    //// trigger check
    //if(!trig_passed) return StatusCode::SUCCESS;

    // event count
    m_event_count++;

    // get truth container
    const xAOD::TruthParticleContainer* m_mu_truthparticles = nullptr;
    CHECK( evtStore()->retrieve( m_mu_truthparticles, "MuonTruthParticles" ) );

    const xAOD::TruthParticleContainer* m_el_truthparticles = nullptr;
    //CHECK( evtStore()->retrieve( m_el_truthparticles, "TruthElectrons" ) );
    CHECK( evtStore()->retrieve( m_el_truthparticles, "TruthParticles" ) );

    const xAOD::TruthVertexContainer* tru_vc = nullptr;
    CHECK( evtStore()->retrieve( tru_vc, "TruthVertices"));

    // debug only ------------------------------------------
    // loop over the truth vertex container
    for (auto tru_v: *tru_vc){
        if (!m_dvutils->isSignalVertex(tru_v)) continue;

        // count signal Z'
        signal_zp_truth++;

        // signal zp
        const xAOD::TruthParticle* signal_zp = tru_v->incomingParticle(0);

        // looping over outgoing particles
        for (unsigned int i=0; i < tru_v->nOutgoingParticles(); i++){
            const xAOD::TruthParticle* signal_lep = m_dvutils->FindFinalState(tru_v->outgoingParticle(i));
            ATH_MSG_DEBUG("Found final state particle from Z', pdgId = " << signal_lep->absPdgId() );

            // signal electron
            if (signal_lep->absPdgId() == 11){

                //count signal electron
                signal_el_truth++;

                // set reconstructed flag
                bool reconstructed = false;

                // check if electron is reconstructed
                if (m_dvutils->IsReconstructedAsElectron(signal_lep)) {
                    signal_el_reconstructed++;
                    reconstructed = true;
                }

                // fill efficiency
                m_eff_el_eta->Fill(signal_lep->eta(), reconstructed);
                m_eff_el_pt->Fill(signal_lep->pt() / 1000., reconstructed);
                m_eff_el_R->Fill(tru_v->perp(), reconstructed );

                // fill 2D efficiency
                m_eff_el_pt_vs_prodVtxR_den->Fill(tru_v->perp(), signal_lep->pt() / 1000.);
                m_eff_el_eta_vs_prodVtxR_den->Fill(tru_v->perp(), signal_lep->eta() );

                if (reconstructed) {
                    m_eff_el_pt_vs_prodVtxR_num->Fill(tru_v->perp(), signal_lep->pt() / 1000.);
                    m_eff_el_eta_vs_prodVtxR_num->Fill(tru_v->perp(), signal_lep->eta() );
                }

                // efficiency vs d0
                if (signal_lep->isAvailable<float>("d0") ) {

                    float d0 = fabs(signal_lep->auxdata< float >("d0"));
                    m_eff_el_d0->Fill( d0, reconstructed );

                    // fill 2D efficiency
                    m_eff_el_pt_vs_d0_den->Fill(d0, signal_lep->pt() / 1000.);
                    m_eff_el_eta_vs_d0_den->Fill(d0, signal_lep->eta() );
                    if (reconstructed) {
                        m_eff_el_pt_vs_d0_num->Fill(d0, signal_lep->pt() / 1000.);
                        m_eff_el_eta_vs_d0_num->Fill(d0, signal_lep->eta() );
                    }
                }
            }

            // signal muon
            if (signal_lep->absPdgId() == 13){

                // count signal muon
                signal_mu_truth++;

                // set reconstructed flag
                bool reconstructed = false;

                if (m_dvutils->IsReconstructedAsMuon(signal_lep)) {
                    signal_mu_reconstructed++;
                    reconstructed = true;
                } // end of IsReconstructedAsMuon

                // fill efficiency
                m_eff_mu_eta->Fill(signal_lep->eta(), reconstructed);
                m_eff_mu_pt->Fill(signal_lep->pt() / 1000., reconstructed);
                m_eff_mu_R->Fill(tru_v->perp(), reconstructed );

                // fill 2D efficiency
                m_eff_mu_pt_vs_prodVtxR_den->Fill(tru_v->perp(), signal_lep->pt() / 1000.);
                m_eff_mu_eta_vs_prodVtxR_den->Fill(tru_v->perp(), signal_lep->eta() );

                if (reconstructed) {
                    m_eff_mu_pt_vs_prodVtxR_num->Fill(tru_v->perp(), signal_lep->pt() / 1000.);
                    m_eff_mu_eta_vs_prodVtxR_num->Fill(tru_v->perp(), signal_lep->eta() );
                }

                // efficiency vs d0
                if (signal_lep->isAvailable<float>("d0") ) {

                    float d0 = fabs(signal_lep->auxdata< float >("d0"));
                    m_eff_mu_d0->Fill( d0, reconstructed );

                    // fill 2D efficiency
                    m_eff_mu_pt_vs_d0_den->Fill(d0, signal_lep->pt() / 1000.);
                    m_eff_mu_eta_vs_d0_den->Fill(d0, signal_lep->eta() );
                    if (reconstructed) {
                        m_eff_mu_pt_vs_d0_num->Fill(d0, signal_lep->pt() / 1000.);
                        m_eff_mu_eta_vs_d0_num->Fill(d0, signal_lep->eta() );
                    }
                }
            }
        }
    }
    
    return StatusCode::SUCCESS;
}

StatusCode LeptonEfficiency::beginInputFile() { 

  return StatusCode::SUCCESS;
}
