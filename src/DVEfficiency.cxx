//------------------------------------------------------------
// Author : Siinn Che, siinn.che@cern.ch
//
// Calculate efficiency to reconstruct signal DV by comparing
// the truth muon and reco muon from DV with dR method
//------------------------------------------------------------

// DisplacedDimuonAnalysis includes
#include "DVEfficiency.h"
#include "GaudiKernel/ITHistSvc.h"
#include "AthenaKernel/errorcheck.h"

// xAOD
#include "xAODEventInfo/EventInfo.h"
#include "xAODTruth/TruthVertex.h"
#include "xAODTruth/TruthVertexContainer.h"
#include "xAODTracking/Vertex.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthParticle.h"
#include "xAODTracking/TrackParticleContainer.h"
#include "xAODTracking/TrackParticle.h"
#include "xAODCore/ShallowCopy.h"
#include "xAODBase/IParticleHelpers.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/PhotonContainer.h"

// tools
#include "PathResolver/PathResolver.h"

// plots 
#include "TProfile.h"
#include "TProfile2D.h"
#include "TH2D.h"
#include "cmath"
#include "vector"




DVEfficiency::DVEfficiency( const std::string& name, ISvcLocator* pSvcLocator ) : 
AthAnalysisAlgorithm( name, pSvcLocator ),
m_dilepdvc("DDL::DiLepDVCuts/DiLepDVCuts"),
m_evtc("DDL::EventCuts/DiLepEventCuts"),
m_dvc("DDL::DVCuts/DiLepBaseCuts"),
m_cos("DDL::DiLepCosmics/DiLepCosmics"),
m_dvutils("DVUtils"),
m_leptool("LeptonSelectionTools"),
m_costool("CosmicTools"),
m_grlTool("GoodRunsListSelectionTool/GRLTool"),
m_tdt("Trig::TrigDecisionTool/TrigDecisionTool"),
m_or("DDL::OverlapRemoval/OverlapRemoval"),
m_prw("CP::PileupReweightingTool/PileupReweightingTool"),
m_accMu("DDL_Muons"),
m_accEl("DDL_Electrons"),
m_accMass("mass")
{
    // initialize tools
    declareProperty("DiLepDVCuts", m_dilepdvc);
    declareProperty("DVUtils", m_dvutils);
    declareProperty("LeptonSelectionTool", m_leptool);
    declareProperty("CosmicTool", m_costool);
    declareProperty("DiLepEventCuts", m_evtc);
    declareProperty("DiLepBaseCuts", m_dvc);
    declareProperty("GRLTool",  m_grlTool, "The private GoodRunsListSelectionTool" );
    declareProperty("TrigDecisionTool", m_tdt);
    declareProperty("DiLepCosmics", m_cos);
    declareProperty("OverlapRemoval", m_or);
    declareProperty("PileupReweightingTool", m_prw);
}


DVEfficiency::~DVEfficiency() {}


StatusCode DVEfficiency::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");

    //--------------------------------------------------------
    // Event cut
    //--------------------------------------------------------

    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    m_dv_mass = new TH1F( "m_dv_mass", "Invariant mass of all signal vertex", 50, 0, 1500 ); // GeV

    // efficiency plots
    m_dv_eff_eta = new TEfficiency( "m_dv_eff_eta", "DV reconstruction efficiency vs eta", 20, -3.0, 3.0);
    m_dv_eff_phi = new TEfficiency( "m_dv_eff_phi", "DV reconstruction efficiency vs phi", 20, -M_PI, M_PI);
    m_dv_eff_mass = new TEfficiency( "m_dv_eff_mass", "DV reconstruction efficiency vs mass", 150, 0, 1500); // GeV
    m_dv_eff_R = new TEfficiency( "m_dv_eff_R", "DV reconstruction efficiency vs R", 20, 0, 400); // mm
    m_dv_eff_z = new TEfficiency( "m_dv_eff_z", "DV reconstruction efficiency vs z", 20, -1000, 1000); // mm
    m_dv_eff_d0 = new TEfficiency( "m_dv_eff_d0", "DV reconstruction efficiency vs d0", 30, 0, 400); // mm

    // efficiency as a function of Z' parameters
    m_dv_eff_zp_eta = new TEfficiency("m_dv_eff_zp_eta", "DV reconstruction efficiency vs Z' eta", 30, -3.0, 3.0);
    m_dv_eff_zp_beta = new TEfficiency("m_dv_eff_zp_beta", "DV reconstruction efficiency vs Z' beta", 25, 0, 1.0);
    m_dv_eff_zp_pt = new TEfficiency("m_dv_eff_zp_pt", "DV reconstruction efficiency vs Z' pt", 25, 0, 500); // GeV

    // efficiency as a function of mu
    m_dv_eff_mu = new TEfficiency("m_dv_eff_mu", "DV reconstruction efficiency vs #mu", 50, 0, 50);

    // trigger effieicny test
    m_dv_eff_trig = new TH1D("m_dv_eff_trig", "Trigger efficiency", 4, 0, 4);

    // efficiency map (Z' pt and eta)
    m_dv_eff_map_pt_eta_den = new TH2F("m_dv_eff_map_pt_eta_den", "DV reconstruction efficiency map, pt vs eta (den)", 10, 0., 1000., 6, -3.0, 3.0); // GeV
    m_dv_eff_map_pt_eta_num = new TH2F("m_dv_eff_map_pt_eta_num", "DV reconstruction efficiency map, pt vs eta (num)", 10, 0., 1000., 6, -3.0, 3.0); // GeV
    m_dv_eff_map_pt_eta = new TH2F("m_dv_eff_map_pt_eta", "DV reconstruction efficiency map, pt vs eta", 10, 0., 1000., 6, -3.0, 3.0); // GeV

    // efficiency map (Z' eta and pileup)
    m_dv_eff_map_mu_eta_den = new TH2F("m_dv_eff_map_mu_eta_den", "DV reconstruction efficiency map, mu vs eta (den)", 50, 0., 50., 30, -3.0, 3.0); // GeV
    m_dv_eff_map_mu_eta_num = new TH2F("m_dv_eff_map_mu_eta_num", "DV reconstruction efficiency map, mu vs eta (num)", 50, 0., 50., 30, -3.0, 3.0); // GeV
    m_dv_eff_map_mu_eta = new TH2F("m_dv_eff_map_mu_eta", "DV reconstruction efficiency map, mu vs eta", 50, 0., 50., 30, -3.0, 3.0); // GeV


    // output
    CHECK( histSvc->regHist("/DV/truth/dv_mass", m_dv_mass) );

    // efficiency plots
    CHECK( histSvc->regGraph("/DV/truth/efficiency/dv/dv_eff_eta", reinterpret_cast<TGraph*>(m_dv_eff_eta)));
    CHECK( histSvc->regGraph("/DV/truth/efficiency/dv/dv_eff_phi", reinterpret_cast<TGraph*>(m_dv_eff_phi)));
    CHECK( histSvc->regGraph("/DV/truth/efficiency/dv/dv_eff_mass",reinterpret_cast<TGraph*>(m_dv_eff_mass)));
    CHECK( histSvc->regGraph("/DV/truth/efficiency/dv/dv_eff_R",reinterpret_cast<TGraph*>(m_dv_eff_R)) );
    CHECK( histSvc->regGraph("/DV/truth/efficiency/dv/dv_eff_z",reinterpret_cast<TGraph*>(m_dv_eff_z)) );
    CHECK( histSvc->regGraph("/DV/truth/efficiency/dv/dv_eff_d0",reinterpret_cast<TGraph*>(m_dv_eff_d0)) );

    CHECK( histSvc->regGraph("/DV/truth/efficiency/zp/dv_eff_zp_eta",reinterpret_cast<TGraph*>(m_dv_eff_zp_eta)) );
    CHECK( histSvc->regGraph("/DV/truth/efficiency/zp/dv_eff_zp_beta",reinterpret_cast<TGraph*>(m_dv_eff_zp_beta)) );
    CHECK( histSvc->regGraph("/DV/truth/efficiency/zp/dv_eff_zp_pt",reinterpret_cast<TGraph*>(m_dv_eff_zp_pt)) );

    CHECK( histSvc->regGraph("/DV/truth/efficiency/dv_eff_mu",reinterpret_cast<TGraph*>(m_dv_eff_mu)) );
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv_eff_trig", m_dv_eff_trig) );

    // efficiency map
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_pt_eta", m_dv_eff_map_pt_eta) );
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_pt_eta_den", m_dv_eff_map_pt_eta_den) );
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_pt_eta_num", m_dv_eff_map_pt_eta_num) );
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_mu_eta", m_dv_eff_map_mu_eta) );
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_mu_eta_den", m_dv_eff_map_mu_eta_den) ); // for error calculation
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_mu_eta_num", m_dv_eff_map_mu_eta_num) );

    return StatusCode::SUCCESS;
}

StatusCode DVEfficiency::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");

    m_dv_eff_map_pt_eta->Divide(m_dv_eff_map_pt_eta_num, m_dv_eff_map_pt_eta_den,1,1, "b");
    m_dv_eff_map_mu_eta->Divide(m_dv_eff_map_mu_eta_num, m_dv_eff_map_mu_eta_den,1,1, "b");

    // divide trigger plot by the number of events processed
    //m_dv_eff_trig->Scale(1/n_events);

    ATH_MSG_INFO("number of events processed = " << n_events);
    
    return StatusCode::SUCCESS;
}

StatusCode DVEfficiency::execute() {  
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    // count number of events processed
    n_events++;

    // retrieve event info
    const xAOD::EventInfo* evtInfo = nullptr;
    CHECK( evtStore()->retrieve( evtInfo, "EventInfo" ) );

    // get combine weight from pileup reweighting tool
    p_weight = m_prw->getCombinedWeight(*evtInfo);
    ATH_MSG_DEBUG("pileup weight = " << p_weight);


    // pile-up
    int mu = evtInfo->actualInteractionsPerCrossing();

    // flag to check if data or MC
    bool isMC = evtInfo->eventType(xAOD::EventInfo::IS_SIMULATION);

    // retrieve lepton container
    const xAOD::MuonContainer* muc = nullptr;
    CHECK( evtStore()->retrieve( muc, "Muons" ));

    const xAOD::ElectronContainer* elc = nullptr;
    CHECK( evtStore()->retrieve( elc, "Electrons" ));

    const xAOD::PhotonContainer* phc = nullptr;
    CHECK( evtStore()->retrieve( phc, "Photons" ));

    // make copies of leptons
    auto muc_copy = xAOD::shallowCopyContainer(*muc);
    xAOD::setOriginalObjectLink(*muc, *muc_copy.first);

    auto elc_copy = xAOD::shallowCopyContainer(*elc);
    xAOD::setOriginalObjectLink(*elc, *elc_copy.first);

    // apply overlap removal
    m_or->FindOverlap(*elc_copy.first, *muc_copy.first);

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

    // set global flag
    bool dv_matched = false;

    // cut flow
    for(auto dv: *dvc_copy.first) {

        // access tracks from vertex
        auto tpLinks = dv->trackParticleLinks();
        xAOD::TrackParticle tp1 = **(tpLinks.at(0));
        xAOD::TrackParticle tp2 = **(tpLinks.at(1));

        // access invariant mass
        float dv_mass = std::fabs(m_accMass(*dv)) / 1000.; // in GeV

        // mass cut
        float mass_min = 10.;

        // collect leptons from this dv
        auto dv_muc = m_accMu(*dv);
        auto dv_elc = m_accEl(*dv);

        // perform lepton matching
        m_dilepdvc->ApplyLeptonMatching(*dv, *elc_copy.first, *muc_copy.first);

        // remove overlapping muon
        m_dilepdvc->ApplyOverlapRemoval(*dv);

        // muon selection tool
        m_leptool->MuonSelection(*dv);

        // remove bad electrons
        m_leptool->BadClusterRemoval(*dv);

        // kinematic cut
        m_leptool->ElectronKinematicCut(*dv);

        // Electron identification
        m_leptool->ElectronID(*dv);

        // select only vertex with tracks
        if(dv->trackParticleLinks().size() != 2) continue;

        // find decay channel of dv
        std::string channel = m_dvutils->DecayChannel(*dv);

        // only select mumu, ee, or emu
        if (!((channel == "mumu") or (channel == "emu") or (channel == "ee"))) continue;

        // Trigger matching
        if(!m_dvutils->TrigMatching(*dv)) continue;

        // vertex fit quality
        if(!m_dvc->PassChisqCut(*dv)) continue;

        // minimum distance from pv (from 0 for MC)
        if(!m_dvc->PassDistCut(*dv, *pvc)) continue;

        // charge requirements
        if(!m_dvc->PassChargeRequirement(*dv)) continue;

        // disabled module
        if(!m_dvc->PassDisabledModuleVeto(*dv)) continue;

        //if ((channel == "emu") or (channel == "ee")) {
        //    if(!m_dvc->PassMaterialVeto(*dv)) continue;
        //}

        // low mass veto
        if(dv_mass < mass_min) continue;

        // cosmic veto (R_CR)
        if(!PassCosmicVeto_R_CR(tp1, tp2)) continue;

        // mark this event as reconstructed
        dv_matched = true;


        //} // end of isMC
    } // end of dv loop

    // GRL
    if (!isMC and !m_grlTool->passRunLB(*evtInfo)) dv_matched = false;

    // event cleaning
    if(!m_evtc->PassEventCleaning(*evtInfo)) dv_matched = false;

    // RPVLL filter
    //if(!m_dvutils->PassRPVLLFilter(*elc, *phc, *muc)) dv_matched = false;

    // trigger check
    bool trig_passed = false;
    
    m_dv_eff_trig->Fill("HLT_mu60_0eta105_msonly",0);
    m_dv_eff_trig->Fill("HLT_g140_loose",0);
    m_dv_eff_trig->Fill("HLT_2g50_loose",0);

    // check trigger and fill histogram for trigger efficiency
    if (m_tdt->isPassed("HLT_mu60_0eta105_msonly")) {
        m_dv_eff_trig->Fill("HLT_mu60_0eta105_msonly",1);
        trig_passed = true;
        }
    if (m_tdt->isPassed("HLT_g140_loose")) {
        m_dv_eff_trig->Fill("HLT_g140_loose",1);
        trig_passed = true;
        }
    if (m_tdt->isPassed("HLT_2g50_loose")) {
        m_dv_eff_trig->Fill("HLT_2g50_loose",1);
        trig_passed = true;
        }

    if(!trig_passed) dv_matched = false;
    else m_dv_eff_trig->Fill("Combined",1);


    //-----------------------------------------------------------------
    // end of cut flow                                                -
    // below is for dv reconstruction efficiency                      -
    //-----------------------------------------------------------------

    // retrieve truth vertex container
    const xAOD::TruthVertexContainer* tru_vc = nullptr;
    evtStore()->retrieve( tru_vc, "TruthVertices");

    // loop over the truth vertex container
    for (auto tru_v: *tru_vc){

        // only selecting signal truth
        if (!m_dvutils->isSignalVertex(tru_v)) continue;

        // fill truth signal vertex mass
        float DVMass = m_dvutils->TruthMass(tru_v) / 1000.;
        m_dv_mass->Fill(DVMass);
     
        // fill efficiency plots
        m_dv_eff_eta->Fill(dv_matched, tru_v->eta());
        m_dv_eff_phi->Fill(dv_matched, tru_v->phi());
        m_dv_eff_mass->Fill(dv_matched, DVMass);
        m_dv_eff_R->Fill(dv_matched, tru_v->perp());
        m_dv_eff_z->Fill(dv_matched,tru_v->z());
        m_dv_eff_d0->Fill(dv_matched,m_dvutils->GetMaxd0(tru_v));

        // efficiency as a function of Z'
        float zp_eta = tru_v->incomingParticle(0)->eta();
        float zp_pt = tru_v->incomingParticle(0)->pt();
        float zp_m = tru_v->incomingParticle(0)->m();
        float zp_e = tru_v->incomingParticle(0)->e();
        float zp_beta = sqrt(1 - (zp_m/zp_e)*(zp_m/zp_e));

        m_dv_eff_zp_eta->Fill(dv_matched, zp_eta);
        m_dv_eff_zp_beta->Fill(dv_matched, zp_beta);
        m_dv_eff_zp_pt->Fill(dv_matched, zp_pt / 1000.);
        m_dv_eff_mu->Fill(dv_matched, mu);

        // fill efficiency map
        m_dv_eff_map_pt_eta_den->Fill(zp_pt / 1000., zp_eta);
        m_dv_eff_map_mu_eta_den->Fill(mu, zp_eta);

        if (dv_matched) {
            m_dv_eff_map_pt_eta_num->Fill(zp_pt / 1000., zp_eta); 
            m_dv_eff_map_mu_eta_num->Fill(mu, zp_eta); 
        }


    } // end of efficiency loop


    return StatusCode::SUCCESS;
}


bool DVEfficiency::PassCosmicVeto_R_CR(xAOD::TrackParticle& tr0, xAOD::TrackParticle& tr1){

    bool PassCosmicVeto = true;
    float Rcos_min = 0.04;

    // define TLorentzVector of decay particles
    TLorentzVector tlv_tp0;
    TLorentzVector tlv_tp1;
    tlv_tp0 = tr0.p4();
    tlv_tp1 = tr1.p4();

    float deltaPhiMinusPi = std::fabs(std::fabs(tlv_tp0.DeltaPhi(tlv_tp1)) - std::acos(-1.));
    float sumEta = tlv_tp0.Eta() + tlv_tp1.Eta();
    float Rcos = std::sqrt(sumEta * sumEta + deltaPhiMinusPi * deltaPhiMinusPi);

    ATH_MSG_DEBUG("Rcos = " << Rcos << ", tlv_tp0.eta = " << tlv_tp0.Eta() << ", tlv_tp1.eta = " << tlv_tp1.Eta());

    if (Rcos < Rcos_min) PassCosmicVeto = false;

    return PassCosmicVeto;
}

