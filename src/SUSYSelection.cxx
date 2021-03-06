//------------------------------------------------------------
// Author : Siinn Che, siinn.che@cern.ch
//
// Calculate efficiency to reconstruct signal DV by comparing
// the truth muon and reco muon from DV with dR method
//------------------------------------------------------------

// DisplacedDimuonAnalysis includes
#include "SUSYSelection.h"
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




SUSYSelection::SUSYSelection( const std::string& name, ISvcLocator* pSvcLocator ) : 
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


SUSYSelection::~SUSYSelection() {}


StatusCode SUSYSelection::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");

    //--------------------------------------------------------
    // Event cut
    //--------------------------------------------------------

    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    // cutflow
    m_susy_dv_eff_cutflow = new TH1D("dv cut flow", "dv cut flow", 18,0,18);

    m_susy_dv_mass = new TH1F( "m_susy_dv_mass", "Invariant mass of all signal vertex", 50, 0, 1500 ); // GeV

    // efficiency plots
    m_susy_dv_eff_eta = new TEfficiency( "m_susy_dv_eff_eta", "DV reconstruction efficiency vs eta", 20, -3.0, 3.0);
    m_susy_dv_eff_phi = new TEfficiency( "m_susy_dv_eff_phi", "DV reconstruction efficiency vs phi", 20, -M_PI, M_PI);
    m_susy_dv_eff_mass = new TEfficiency( "m_susy_dv_eff_mass", "DV reconstruction efficiency vs mass", 150, 0, 1500); // GeV
    m_susy_dv_eff_R = new TEfficiency( "m_susy_dv_eff_R", "DV reconstruction efficiency vs R", 20, 0, 400); // mm
    m_susy_dv_eff_z = new TEfficiency( "m_susy_dv_eff_z", "DV reconstruction efficiency vs z", 20, -1000, 1000); // mm
    m_susy_dv_eff_d0 = new TEfficiency( "m_susy_dv_eff_d0", "DV reconstruction efficiency vs d0", 30, 0, 400); // mm

    // efficiency as a function of Z' parameters
    m_susy_dv_eff_zp_eta = new TEfficiency("m_susy_dv_eff_zp_eta", "DV reconstruction efficiency vs Z' eta", 30, -3.0, 3.0);
    m_susy_dv_eff_zp_beta = new TEfficiency("m_susy_dv_eff_zp_beta", "DV reconstruction efficiency vs Z' beta", 25, 0, 1.0);
    m_susy_dv_eff_zp_pt = new TEfficiency("m_susy_dv_eff_zp_pt", "DV reconstruction efficiency vs Z' pt", 25, 0, 500); // GeV

    // efficiency as a function of mu
    m_susy_dv_eff_mu = new TEfficiency("m_susy_dv_eff_mu", "DV reconstruction efficiency vs #mu", 50, 0, 50);

    // trigger effieicny test
    m_susy_dv_eff_trig = new TH1D("m_susy_dv_eff_trig", "Trigger efficiency", 4, 0, 4);

    // efficiency map (Z' pt and eta)
    m_susy_dv_eff_map_pt_eta_den = new TH2F("m_susy_dv_eff_map_pt_eta_den", "DV reconstruction efficiency map, pt vs eta (den)", 10, 0., 1000., 6, -3.0, 3.0); // GeV
    m_susy_dv_eff_map_pt_eta_num = new TH2F("m_susy_dv_eff_map_pt_eta_num", "DV reconstruction efficiency map, pt vs eta (num)", 10, 0., 1000., 6, -3.0, 3.0); // GeV
    m_susy_dv_eff_map_pt_eta = new TH2F("m_susy_dv_eff_map_pt_eta", "DV reconstruction efficiency map, pt vs eta", 10, 0., 1000., 6, -3.0, 3.0); // GeV

    // efficiency map (Z' eta and pileup)
    m_susy_dv_eff_map_mu_eta_den = new TH2F("m_susy_dv_eff_map_mu_eta_den", "DV reconstruction efficiency map, mu vs eta (den)", 50, 0., 50., 30, -3.0, 3.0); // GeV
    m_susy_dv_eff_map_mu_eta_num = new TH2F("m_susy_dv_eff_map_mu_eta_num", "DV reconstruction efficiency map, mu vs eta (num)", 50, 0., 50., 30, -3.0, 3.0); // GeV
    m_susy_dv_eff_map_mu_eta = new TH2F("m_susy_dv_eff_map_mu_eta", "DV reconstruction efficiency map, mu vs eta", 50, 0., 50., 30, -3.0, 3.0); // GeV


    // output
    CHECK( histSvc->regHist("/DV/SUSYselection/efficiency/dv_eff_cutflow;", m_susy_dv_eff_cutflow));
    CHECK( histSvc->regHist( "/DV/SUSYselection/dv_mass", m_susy_dv_mass) );

    // efficiency plots
    CHECK( histSvc->regGraph("/DV/SUSYselection/efficiency/dv/dv_eff_eta", reinterpret_cast<TGraph*>(m_susy_dv_eff_eta)));
    CHECK( histSvc->regGraph("/DV/SUSYselection/efficiency/dv/dv_eff_phi", reinterpret_cast<TGraph*>(m_susy_dv_eff_phi)));
    CHECK( histSvc->regGraph("/DV/SUSYselection/efficiency/dv/dv_eff_mass",reinterpret_cast<TGraph*>(m_susy_dv_eff_mass)));
    CHECK( histSvc->regGraph("/DV/SUSYselection/efficiency/dv/dv_eff_R",reinterpret_cast<TGraph*>(m_susy_dv_eff_R)) );
    CHECK( histSvc->regGraph("/DV/SUSYselection/efficiency/dv/dv_eff_z",reinterpret_cast<TGraph*>(m_susy_dv_eff_z)) );
    CHECK( histSvc->regGraph("/DV/SUSYselection/efficiency/dv/dv_eff_d0",reinterpret_cast<TGraph*>(m_susy_dv_eff_d0)) );

    CHECK( histSvc->regGraph("/DV/SUSYselection/efficiency/zp/dv_eff_zp_eta",reinterpret_cast<TGraph*>(m_susy_dv_eff_zp_eta)) );
    CHECK( histSvc->regGraph("/DV/SUSYselection/efficiency/zp/dv_eff_zp_beta",reinterpret_cast<TGraph*>(m_susy_dv_eff_zp_beta)) );
    CHECK( histSvc->regGraph("/DV/SUSYselection/efficiency/zp/dv_eff_zp_pt",reinterpret_cast<TGraph*>(m_susy_dv_eff_zp_pt)) );

    CHECK( histSvc->regGraph("/DV/SUSYselection/efficiency/dv_eff_mu",reinterpret_cast<TGraph*>(m_susy_dv_eff_mu)) );
    CHECK( histSvc->regHist( "/DV/SUSYselection/efficiency/dv_eff_trig", m_susy_dv_eff_trig) );

    // efficiency map
    CHECK( histSvc->regHist( "/DV/SUSYselection/efficiency/zp/dv_eff_map_pt_eta", m_susy_dv_eff_map_pt_eta) );
    CHECK( histSvc->regHist( "/DV/SUSYselection/efficiency/zp/dv_eff_map_pt_eta_den", m_susy_dv_eff_map_pt_eta_den) );
    CHECK( histSvc->regHist( "/DV/SUSYselection/efficiency/zp/dv_eff_map_pt_eta_num", m_susy_dv_eff_map_pt_eta_num) );
    CHECK( histSvc->regHist( "/DV/SUSYselection/efficiency/zp/dv_eff_map_mu_eta", m_susy_dv_eff_map_mu_eta) );
    CHECK( histSvc->regHist( "/DV/SUSYselection/efficiency/zp/dv_eff_map_mu_eta_den", m_susy_dv_eff_map_mu_eta_den) ); // for error calculation
    CHECK( histSvc->regHist( "/DV/SUSYselection/efficiency/zp/dv_eff_map_mu_eta_num", m_susy_dv_eff_map_mu_eta_num) );

    return StatusCode::SUCCESS;
}

StatusCode SUSYSelection::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");

    m_susy_dv_eff_map_pt_eta->Divide(m_susy_dv_eff_map_pt_eta_num, m_susy_dv_eff_map_pt_eta_den,1,1, "b");
    m_susy_dv_eff_map_mu_eta->Divide(m_susy_dv_eff_map_mu_eta_num, m_susy_dv_eff_map_mu_eta_den,1,1, "b");

    // divide trigger plot by the number of events processed
    //m_susy_dv_eff_trig->Scale(1/n_events);

    ATH_MSG_INFO("number of events processed = " << n_events);
    
    return StatusCode::SUCCESS;
}

StatusCode SUSYSelection::execute() {  
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

    //=================================================
    // event cut flow
    //=================================================
   
    // place holder
    m_susy_dv_eff_cutflow->Fill("RPVLL",0);
    m_susy_dv_eff_cutflow->Fill("GRL",0);
    m_susy_dv_eff_cutflow->Fill("EventCleaning",0);
    m_susy_dv_eff_cutflow->Fill("TrigFilter",0);
    m_susy_dv_eff_cutflow->Fill("PV",0);
    m_susy_dv_eff_cutflow->Fill("CosmicVeto",0);

    // event selection flag
    bool event_passed = true;

    // all events
    m_susy_dv_eff_cutflow->Fill("RPVLL",1);

    // GRL
    if (!isMC and !m_grlTool->passRunLB(*evtInfo)) event_passed = false;
    if (event_passed) m_susy_dv_eff_cutflow->Fill("GRL",1);

    // event cleaning
    if(!m_evtc->PassEventCleaning(*evtInfo)) event_passed = false;
    if (event_passed) m_susy_dv_eff_cutflow->Fill("EventCleaning",1);

    // RPVLL filter
    //if(!m_dvutils->PassRPVLLFilter(*elc, *phc, *muc)) dv_matched = false;

    m_susy_dv_eff_trig->Fill("HLT_mu60_0eta105_msonly",0);
    m_susy_dv_eff_trig->Fill("HLT_g140_loose",0);
    m_susy_dv_eff_trig->Fill("HLT_2g50_loose",0);

    bool trig_passed = false;

    // check trigger and fill histogram for trigger efficiency
    if (m_tdt->isPassed("HLT_mu60_0eta105_msonly")) {
        m_susy_dv_eff_trig->Fill("HLT_mu60_0eta105_msonly",1);
        trig_passed = true;
        }
    if (m_tdt->isPassed("HLT_g140_loose")) {
        m_susy_dv_eff_trig->Fill("HLT_g140_loose",1);
        trig_passed = true;
        }
    if (m_tdt->isPassed("HLT_2g50_loose")) {
        m_susy_dv_eff_trig->Fill("HLT_2g50_loose",1);
        trig_passed = true;
        }

    // trigger 
    if(!trig_passed) {
        event_passed = false;
    }
    else {
        m_susy_dv_eff_trig->Fill("Combined",1);
    }

    if (event_passed) m_susy_dv_eff_cutflow->Fill("TrigFilter",1);

    // check for PV in allowed volume
    if(!m_evtc->PassPVCuts(*pvc)) event_passed = false;
    if(event_passed) m_susy_dv_eff_cutflow->Fill("PV",1);
   

    // cosmic veto
    if(!m_cos->PassCosmicEventVeto(*elc, *muc)) event_passed = false;
    if(event_passed) m_susy_dv_eff_cutflow->Fill("CosmicVeto",1);


    //=================================================
    // vertex cut flow
    //=================================================
    // set to true if vertex pass all selection
    bool dv_matched = false;

    // cut flow
    for(auto dv: *dvc_copy.first) {

        m_susy_dv_eff_cutflow->Fill("Vertex",1);

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

        // find decay channel of dv
        std::string channel = m_dvutils->DecayChannel(*dv);

        // only select mumu, ee, or emu
        if (!((channel == "mumu") or (channel == "emu") or (channel == "ee"))) continue;

        // vertex type
        DDL::DiLepType dv_type = DDL::DiLepType::None;

        if(channel == "mumu") dv_type = DDL::DiLepType::mm;
        if(channel == "ee") dv_type = DDL::DiLepType::ee;
        if(channel == "emu") dv_type = DDL::DiLepType::em;
        
        m_dilepdvc->ApplyKinematics(*dv);
        if(dv->trackParticleLinks().size() != 2) continue;
        
        // lepton identification
        m_dilepdvc->ApplyTightness(*dv);
        if(dv->trackParticleLinks().size() != 2) continue;
        
        // overlap removal of electrons and muons
        m_dilepdvc->ApplyOverlapRemoval(*dv);
        if(!m_dilepdvc->PassNLeptons(*dv, dv_type)) continue;
        m_susy_dv_eff_cutflow->Fill("LeptonID",1);
      
        // trigger matching  
        m_dilepdvc->DoTriggerMatching(*dv);
        if(!m_dilepdvc->PassTriggerMatching(*dv)) continue;
        m_susy_dv_eff_cutflow->Fill("Trig.Matching",1);
        
        if(!m_dvc->PassChisqCut(*dv)) continue;
        m_susy_dv_eff_cutflow->Fill("#chi^{2} / DOF",1);

        if(!m_dvc->PassDistCut(*dv, *pvc)) continue;
        m_susy_dv_eff_cutflow->Fill("MinDisplacment",1);
        
        if(!m_dvc->PassChargeRequirement(*dv)) continue;
        m_susy_dv_eff_cutflow->Fill("Oppo.Charge",1);

        if(!m_dvc->PassDisabledModuleVeto(*dv)) continue;
        m_susy_dv_eff_cutflow->Fill("DisabledModule",1);
        
        if(!m_dvc->PassMaterialVeto(*dv)) continue;
        m_susy_dv_eff_cutflow->Fill("MaterialVeto",1);
        
        if(!m_dvc->PassMassCut(*dv)) continue;
        m_susy_dv_eff_cutflow->Fill("LowMass",1);
        
        if(!m_dvc->PassLowMassVeto(*dv)) continue;
        m_susy_dv_eff_cutflow->Fill("InvMass",1);

        if(!m_dvc->PassFiducialCuts(*dv)) continue;
        m_susy_dv_eff_cutflow->Fill("Fiducial",1);

        m_dilepdvc->DoFilterMatching(*dv);
        if(!m_dilepdvc->PassFilterMatching(*dv)) continue;
        m_susy_dv_eff_cutflow->Fill("FilterMatching",1);

        // mark this event as reconstructed
        dv_matched = true;


        //} // end of isMC
    } // end of dv loop

    // if event didn't pass event selection, mark as not reconstructed
    if (!event_passed) dv_matched = false;

    //-----------------------------------------------------------------
    // end of cut flow                                                
    // Below is for efficiency filling
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
        m_susy_dv_mass->Fill(DVMass);
     
        // fill efficiency plots
        m_susy_dv_eff_eta->Fill(dv_matched, tru_v->eta());
        m_susy_dv_eff_phi->Fill(dv_matched, tru_v->phi());
        m_susy_dv_eff_mass->Fill(dv_matched, DVMass);
        m_susy_dv_eff_R->Fill(dv_matched, tru_v->perp());
        m_susy_dv_eff_z->Fill(dv_matched,tru_v->z());
        m_susy_dv_eff_d0->Fill(dv_matched,m_dvutils->GetMaxd0(tru_v));

        // efficiency as a function of Z'
        float zp_eta = tru_v->incomingParticle(0)->eta();
        float zp_pt = tru_v->incomingParticle(0)->pt();
        float zp_m = tru_v->incomingParticle(0)->m();
        float zp_e = tru_v->incomingParticle(0)->e();
        float zp_beta = sqrt(1 - (zp_m/zp_e)*(zp_m/zp_e));

        m_susy_dv_eff_zp_eta->Fill(dv_matched, zp_eta);
        m_susy_dv_eff_zp_beta->Fill(dv_matched, zp_beta);
        m_susy_dv_eff_zp_pt->Fill(dv_matched, zp_pt / 1000.);
        m_susy_dv_eff_mu->Fill(dv_matched, mu);

        // fill efficiency map
        m_susy_dv_eff_map_pt_eta_den->Fill(zp_pt / 1000., zp_eta);
        m_susy_dv_eff_map_mu_eta_den->Fill(mu, zp_eta);

        if (dv_matched) {
            m_susy_dv_eff_map_pt_eta_num->Fill(zp_pt / 1000., zp_eta); 
            m_susy_dv_eff_map_mu_eta_num->Fill(mu, zp_eta); 
        }


    } // end of efficiency loop


    return StatusCode::SUCCESS;
}


bool SUSYSelection::PassCosmicVeto_R_CR(xAOD::TrackParticle& tr0, xAOD::TrackParticle& tr1){

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

