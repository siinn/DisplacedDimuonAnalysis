// DisplacedDimuonAnalysis includes
#include "DisplacedDimuonAnalysisAlg.h"

#include "AthenaKernel/errorcheck.h"
#include "GaudiKernel/ITHistSvc.h"
#include "TH1F.h"

// xAOD
#include "xAODEventInfo/EventInfo.h"
#include "xAODBase/IParticleHelpers.h"
#include "xAODCore/ShallowCopy.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODTruth/TruthParticleContainer.h"

// tools
#include "PathResolver/PathResolver.h"

// DV
#include "DVCuts/EventCuts.h"

// for M_PI
#include "cmath"


// debug
//#include "StoreGate/StoreGate.h"
//#include "StoreGate/StoreGateSvc.h"



DisplacedDimuonAnalysisAlg::DisplacedDimuonAnalysisAlg( const std::string& name, ISvcLocator* pSvcLocator ) :
AthAnalysisAlgorithm( name, pSvcLocator ),
m_dilepdvc("DDL::DiLepDVCuts/DiLepDVCuts"),
m_evtc("DDL::EventCuts/DiLepEventCuts"),
m_dvc("DDL::DVCuts/DiLepBaseCuts"),
m_cos("DDL::DiLepCosmics/DiLepCosmics"),
m_dvutils("DVUtils"),
m_grlTool("GoodRunsListSelectionTool/GRLTool"),
m_tdt("Trig::TrigDecisionTool/TrigDecisionTool"),
m_accMass("mass")
{
    // initialize tools
    declareProperty("DiLepDVCuts", m_dilepdvc);
    declareProperty("DVUtils", m_dvutils);
    declareProperty("DiLepEventCuts", m_evtc);
    declareProperty("DiLepBaseCuts", m_dvc);
    declareProperty("GRLTool",  m_grlTool, "The private GoodRunsListSelectionTool" );
    declareProperty("TrigDecisionTool", m_tdt);
    declareProperty("DiLepCosmics", m_cos);

}


DisplacedDimuonAnalysisAlg::~DisplacedDimuonAnalysisAlg() {}


StatusCode DisplacedDimuonAnalysisAlg::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");

    //--------------------------------------------------------
    // Event cut
    //--------------------------------------------------------

    // set trigger
    m_evtc->SetTriggers({"HLT_mu60_0eta105_msonly"});


    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    // custom binning
    Float_t m_dv_dimuon_M_bins[] = {0,100,400,700,1000,2000};

    // define histograms

    m_event_cutflow = new TH1D( "m_event_cutflow", "Event cutflow", 4,0,4);
    m_dv_cutflow = new TH1D( "m_dv_cutflow", "Reco dv cutflow", 9,0,9);

    m_dv_M = new TH1F("dv_M","All DV mass in GeV",200,0.,2000.);
    m_dv_dimuon_M = new TH1F("dv_dimuon_M","Dimuon DV mass in GeV",5, m_dv_dimuon_M_bins );
    m_dv_dimuon_R = new TH1F("dv_dimuon_R","R of dimuon dv [mm]",50,0.,300.);
    m_dv_dimuon_R_low = new TH1F("dv_dimuon_R_low","R of dimuon dv [mm], low",100,0.,10.);
    m_dv_dimuon_z = new TH1F("dv_dimuon_z","z of dimuon dv [mm]",100,-1000.,1000.);
    m_dv_dimuon_l = new TH1F("dv_dimuon_l","l of dimuon dv [mm]",100,0.,1000.);
    m_dv_dimuon_R_M = new TH2F("dv_dimuon_R_M","Dimuon DV position R vs M", 50,0,300,200,0,2000);

    // muon kinematics distribution
    m_signal_muon_pt = new TH1F("signal_muon_pt","Signal muon pT",50,0.,1000.);
    m_signal_muon_pt_low = new TH1F("signal_muon_pt_low","Signal muon low pT",50,0.,100.);
    m_signal_muon_eta = new TH1F("signal_muon_eta","Signal muon eta",50,-3.0,3.0);
    m_signal_muon_phi = new TH1F("signal_muon_phi","Signal muon phi",50,-M_PI,M_PI);
    m_signal_muon_d0 = new TH1F("signal_muon_d0","Signal muon d0",50,-300,300);
    m_signal_muon_z0 = new TH1F("signal_muon_z0","Signal muon z0",50,-1000,1000);

    m_signal_muon_pt_min = new TH1F("signal_muon_pt_min","Signal muon pt_min",50,0.,1000.);
    m_signal_muon_pt_min_low = new TH1F("signal_muon_pt_min_low","Signal muon low pt_min",50,0.,100.);
    m_signal_muon_pt_max = new TH1F("signal_muon_pt_max","Signal muon pt_max",50,0.,1000.);
    m_signal_muon_pt_max_low = new TH1F("signal_muon_pt_max_low","Signal muon low pt_max",50,0.,100.);

    // cosmic veto
    m_signal_muon_DeltaR = new TH1F("signal_muon_DeltaR","Signal muon Delta R",100, 0., 5.);
    m_signal_muon_DeltaR_low = new TH1F("signal_muon_DeltaR_low","Signal muon Delta R low",100, 0., 1);
    m_signal_muon_Rcos = new TH1F("signal_muon_Rcos","Signal muon Rcos",100, 0., 5.);
    m_signal_muon_Rcos_low = new TH1F("signal_muon_Rcos_low","Signal muon Rcos low",100, 0., 0.5);
    m_signal_muon_phi_eta = new TH2F("signal_muon_phi-eta","Signal muon phi-eta",50,-M_PI,M_PI,50,-3.0,3.0);

    // only for MC
    m_dv_dimuon_M_matched = new TH1F("dv_dimuon_M_matched","matched dimuon DV mass in GeV",200,0.,2000.);
    m_dv_dimuon_R_matched = new TH1F("dv_dimuon_R_matched","R of matched dimuon dv [mm]",50,0.,300.);
    m_dv_dimuon_R_M_matched = new TH2F("dv_dimuon_R_M_matched","matched dimuon DV position R vs M", 50,0,300,200,0,2000);
    m_chi2_ndof = new TH1F("chi2_ndof", "chi^2 / ndof", 100, 0, 10);

    // registor for output
    CHECK( histSvc->regHist("/DV/SecondaryVertex/dv_cutflow", m_dv_cutflow) );
    CHECK( histSvc->regHist("/DV/event_cutflow", m_event_cutflow) );

    CHECK( histSvc->regHist("/DV/SecondaryVertex/reconstructed/reco_dv_M", m_dv_M) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/reconstructed/reco_dv_dimuon_M", m_dv_dimuon_M) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/reconstructed/reco_dv_dimuon_R", m_dv_dimuon_R) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/reconstructed/reco_dv_dimuon_R_low", m_dv_dimuon_R_low) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/reconstructed/reco_dv_dimuon_z", m_dv_dimuon_z) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/reconstructed/reco_dv_dimuon_l", m_dv_dimuon_l) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/reconstructed/reco_dv_R_M", m_dv_dimuon_R_M) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/chi2_ndof", m_chi2_ndof) );

    // muon kinematics distribution
    CHECK( histSvc->regHist("/DV/SecondaryVertex/muon/reconstructed/signal_muon_pt", m_signal_muon_pt) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/muon/reconstructed/signal_muon_pt_low", m_signal_muon_pt_low) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/muon/reconstructed/signal_muon_pt_min", m_signal_muon_pt_min) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/muon/reconstructed/signal_muon_pt_min_low", m_signal_muon_pt_min_low) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/muon/reconstructed/signal_muon_pt_max", m_signal_muon_pt_max) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/muon/reconstructed/signal_muon_pt_max_low", m_signal_muon_pt_max_low) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/muon/reconstructed/signal_muon_eta", m_signal_muon_eta) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/muon/reconstructed/signal_muon_phi", m_signal_muon_phi) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/muon/reconstructed/signal_muon_d0", m_signal_muon_d0) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/muon/reconstructed/signal_muon_z0", m_signal_muon_z0) );

    // cosmic veto
    CHECK( histSvc->regHist("/DV/SecondaryVertex/muon/reconstructed/cosmicVeto/signal_muon_DeltaR", m_signal_muon_DeltaR) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/muon/reconstructed/cosmicVeto/signal_muon_DeltaR_low", m_signal_muon_DeltaR_low) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/muon/reconstructed/cosmicVeto/signal_muon_Rcos", m_signal_muon_Rcos) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/muon/reconstructed/cosmicVeto/signal_muon_Rcos_low", m_signal_muon_Rcos_low) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/muon/reconstructed/cosmicVeto/signal_muon_phi_eta", m_signal_muon_phi_eta) );

    // only for MC
    CHECK( histSvc->regHist("/DV/SecondaryVertex/reconstructed/reco_truth-matched/reco_dv_dimuon_M", m_dv_dimuon_M_matched) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/reconstructed/reco_truth-matched/reco_dv_dimuon_R", m_dv_dimuon_R_matched) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/reconstructed/reco_truth-matched/reco_dv_R_M", m_dv_dimuon_R_M_matched) );

  return StatusCode::SUCCESS;
}

StatusCode DisplacedDimuonAnalysisAlg::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");
    return StatusCode::SUCCESS;
}

StatusCode DisplacedDimuonAnalysisAlg::execute() {
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    // retrieve event info
    const xAOD::EventInfo* evtInfo = nullptr;
    CHECK( evtStore()->retrieve( evtInfo, "EventInfo" ) );

    // flag to check if data or MC
    bool isMC = evtInfo->eventType(xAOD::EventInfo::IS_SIMULATION);

    m_event_cutflow->Fill("AllEvents", 1);

    // GRL
    if (!isMC and !m_grlTool->passRunLB(*evtInfo)) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("GRL (Data)", 1);

    // event cleaning
    if(!m_evtc->PassEventCleaning(*evtInfo)) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("EvtCleaning (Data)", 1);

    // trigger check
    if(!m_evtc->PassTrigger()) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("Trig", 1);

    // retrieve lepton container
    const xAOD::MuonContainer* muc = nullptr;
    CHECK( evtStore()->retrieve( muc, "Muons" ));

    const xAOD::ElectronContainer* elc = nullptr;
    CHECK( evtStore()->retrieve( elc, "Electrons" ));

    // make copies of leptons
    auto muc_copy = xAOD::shallowCopyContainer(*muc);
    xAOD::setOriginalObjectLink(*muc, *muc_copy.first);

    auto elc_copy = xAOD::shallowCopyContainer(*elc);
    xAOD::setOriginalObjectLink(*elc, *elc_copy.first);

    // retrieve primary vertices
    const xAOD::VertexContainer* pvc = nullptr;
    CHECK( evtStore()->retrieve( pvc, "PrimaryVertices" ));

    // get primary vertex
    auto pv = m_evtc->GetPV(*pvc);

    // retrieve secondary vertices
    const xAOD::VertexContainer* dvc = nullptr;
    CHECK( evtStore()->retrieve( dvc, "VrtSecInclusive_SecondaryVertices" ));

    // make a copy of vertex containers
    auto dvc_copy = xAOD::shallowCopyContainer(*dvc);

    // perform lepton matching
    for(auto dv: *dvc_copy.first) {
        //m_dvutils->ApplyMuonMatching(*dv, *muc_copy.first);
        m_dilepdvc->ApplyLeptonMatching(*dv, *elc_copy.first, *muc_copy.first);
    }

    // cut flow
    for(auto dv: *dvc_copy.first) {
        ATH_MSG_DEBUG("DEBUG: DV loop, dv = " << dv);

        // access invariant mass
        float dv_mass = std::fabs(m_accMass(*dv)) / 1000.; // in MeV

        // collect muons from this dv
        auto dv_muc = m_dilepdvc->GetMu(*dv);

        // fill all dv
        m_dv_M->Fill(dv_mass);               // all dv

        //----------------------------------------
        // counting all dv's
        //----------------------------------------
        m_dv_cutflow->Fill("SV", 1);

        //----------------------------------------
        // require dv to have 2 muons
        //----------------------------------------
        if (dv_muc->size() != 2) continue;
        m_dv_cutflow->Fill("#mu#mu", 1);

        //----------------------------------------
        // Trigger matching
        //----------------------------------------
        if (!m_dvutils->TriggerMatching(*dv_muc)) continue;
        m_dv_cutflow->Fill("Trig. Matching", 1);

        //----------------------------------------
        // combined muon
        //----------------------------------------
        if(!m_dvutils->IsCombinedMuon(*dv_muc)) continue;
        m_dv_cutflow->Fill("CombinedMuon", 1);

        //----------------------------------------
        // vertex fit quality
        //----------------------------------------
        m_chi2_ndof->Fill (dv->chiSquared() / dv->numberDoF() );
        //if(!m_dvc->PassChisqCut(*dv)) continue;
        //m_dv_cutflow->Fill("#chi^{2}_{DV} / DOF_{DV} < 5", 1);

        //----------------------------------------
        // minimum distance from pv (from 0 for MC)
        //----------------------------------------
        if(!m_dvc->PassDistCut(*dv, *pvc)) continue;
        m_dv_cutflow->Fill("r_{DV} > 3 mm", 1);

        //----------------------------------------
        // charge requirements
        //----------------------------------------
        if(!m_dilepdvc->PassChargeRequirement(*dv)) continue;
        m_dv_cutflow->Fill("#mu^{+}#mu^{-}", 1);

        //----------------------------------------
        // cosmic veto
        //----------------------------------------
        if(!PassCosmicVeto(*dv_muc)) continue;
        m_dv_cutflow->Fill("CosmicVeto", 1);

        //----------------------------------------
        // investigating pT cut
        //----------------------------------------
        if(m_dvutils->getMinPT(*dv_muc) < 50.) continue;
        m_dv_cutflow->Fill("p_{T}^{#mu} > 50 GeV", 1);

        if(m_dvutils->getMinPT(*dv_muc) < 60.) continue;
        m_dv_cutflow->Fill("p_{T}^{#mu} > 60 GeV", 1);

    
        //----------------------------------------
        // end of cut flow. Now plotting
        //----------------------------------------
        ATH_MSG_INFO("Found DV with mass = " << dv_mass << ", runNumber = "
        << evtInfo->runNumber() << ", eventNumber = "
        << evtInfo->eventNumber() << ", Lumiblock = " << evtInfo->lumiBlock() );

        // plot dv distributions
        plot_dv(*dv, *pv);

        // plot muon kinematics
        plot_muon_kinematics(*dv_muc);

        //// only for MC: find truth dv matched to this dv
        //if (isMC) {
        //    // find truth dv matched to this dv
        //    const xAOD::TruthVertex* tru_v = m_dvutils->IsSignalDV(*dv_muc);

        //    if (tru_v == nullptr) continue;
        //    m_dv_cutflow->Fill("(MC)Truth-matched", 1);

        //    // fill matched dimuon vertex
        //    m_dv_dimuon_M_matched->Fill(dv_mass);                          // dimuon mass
        //    m_dv_dimuon_R_matched->Fill(dv_R);                                // R in [mm]
        //    m_dv_dimuon_R_M_matched->Fill(dv_R, dv_mass);
        //} // end of isMC

    } // end of dv loop
    
    return StatusCode::SUCCESS;
}

StatusCode DisplacedDimuonAnalysisAlg::beginInputFile() { 

  return StatusCode::SUCCESS;
}

bool DisplacedDimuonAnalysisAlg::PassCosmicVeto(const DataVector<xAOD::Muon> dv_muc) {
    ATH_MSG_DEBUG ("Plotting cosmic veto" << name() << "...");

    // fill phi vs eta
    for(auto mu: dv_muc){
        m_signal_muon_phi_eta->Fill(mu->phi(),mu->eta());
    }

    // fill Rcos, deltaR
    bool PassCosmicVeto = true;
    float Rcos_min = 0.04;
    //float DeltaR_min = 0.04;
    float deltaPhiMinusPi = m_dvutils->getDeltaPhiMinusPi(dv_muc);
    float sumEta = m_dvutils->getSumEta(dv_muc);
    float Rcos = std::sqrt(sumEta * sumEta + deltaPhiMinusPi * deltaPhiMinusPi);
    float deltaR = m_dvutils->getDeltaR(dv_muc);

    m_signal_muon_DeltaR->Fill(deltaR); // angular difference between two muons
    m_signal_muon_DeltaR_low->Fill(deltaR); // angular difference between two muons
    m_signal_muon_Rcos->Fill(Rcos);
    m_signal_muon_Rcos_low->Fill(Rcos);

    // set flag
    //if ((Rcos < Rcos_min) or (deltaR < DeltaR_min)) PassCosmicVeto = false;
    if (Rcos < Rcos_min) PassCosmicVeto = false;

    return PassCosmicVeto;
}


void DisplacedDimuonAnalysisAlg::plot_muon_kinematics(const DataVector<xAOD::Muon> dv_muc) {
    ATH_MSG_DEBUG ("Plotting muon kinematics" << name() << "...");

    // plot muon pT
    for(auto mu: dv_muc){

        m_signal_muon_pt->Fill(mu->pt() / 1000.); // GeV
        m_signal_muon_pt_low->Fill(mu->pt() / 1000.); // GeV
        m_signal_muon_eta->Fill(mu->eta());
        m_signal_muon_phi->Fill(mu->phi());
        m_signal_muon_d0->Fill(mu->primaryTrackParticle()->d0());
        m_signal_muon_z0->Fill(mu->primaryTrackParticle()->z0());
    }

    // plot the lowest pT of two muons
    float max_muon_pt = m_dvutils->getMaxPT(dv_muc);
    float min_muon_pt = m_dvutils->getMinPT(dv_muc);
    m_signal_muon_pt_max->Fill(max_muon_pt);
    m_signal_muon_pt_max_low->Fill(max_muon_pt);
    m_signal_muon_pt_min->Fill(min_muon_pt);
    m_signal_muon_pt_min_low->Fill(min_muon_pt);

    return;
}

void DisplacedDimuonAnalysisAlg::plot_dv(const xAOD::Vertex& dv, const xAOD::Vertex& pv) {
    ATH_MSG_DEBUG ("Plotting dv distribution" << name() << "...");

    // access invariant mass
    float dv_mass = std::fabs(m_accMass(dv)) / 1000.; // in MeV

    // fill dimuon vertex
    float dv_R = m_dvutils->getR( dv, pv );                 // R in [mm]
    float dv_z = m_dvutils->getz( dv, pv );                 // z in [mm]
    float dv_l = m_dvutils->getr( dv, pv );                 // r in [mm]

    m_dv_dimuon_M->Fill(dv_mass);                             // dimuon mass
    m_dv_dimuon_R->Fill(dv_R);                                
    m_dv_dimuon_R_low->Fill(dv_R);                                
    m_dv_dimuon_z->Fill(dv_z);                                
    m_dv_dimuon_l->Fill(dv_l);                                
    m_dv_dimuon_R_M->Fill(dv_R, dv_mass);


    return;
}

