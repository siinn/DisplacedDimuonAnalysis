// DisplacedDimuonAnalysis includes
#include "FlipBkgEst.h"

#include "AthenaKernel/errorcheck.h"
#include "GaudiKernel/ITHistSvc.h"
#include "TH1F.h"

// xAOD
#include "xAODEventInfo/EventInfo.h"
#include "xAODBase/IParticleHelpers.h"
#include "xAODCore/ShallowCopy.h"
#include "xAODCore/tools/SafeDeepCopy.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODTracking/TrackParticleContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthVertex.h"
#include "xAODTruth/TruthVertexContainer.h"
#include "xAODEgamma/ElectronxAODHelpers.h"

// tools
#include "PathResolver/PathResolver.h"

// for M_PI
#include "cmath"

// debug
#include <typeinfo>

FlipBkgEst::FlipBkgEst( const std::string& name, ISvcLocator* pSvcLocator ) :
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
m_accMu("DDL_Muons"),
m_accEl("DDL_Electrons"),
m_vertexer("DDL::DispVertexer/DispVertexer"),
m_acc_p4("DDL_p4"),
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
    declareProperty("Vertexer", m_vertexer);
}


FlipBkgEst::~FlipBkgEst() {}


StatusCode FlipBkgEst::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");
    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    m_n_mu = new TH1D( "m_n_mu", "Number of muons in event", 20,0,20);
    m_n_elc = new TH1D( "m_n_elc", "Number of electrons in event", 20,0,20);
    m_n_mu_sel = new TH1D( "m_n_mu_sel", "Number of selected muons in event", 20,0,20);
    m_n_elc_sel = new TH1D( "m_n_elc_sel", "Number of selected electrons in event", 20,0,20);

    m_mumu_cf_input = new TH1D( "m_mumu_cf_input", "Input mumu cutflow", 8,0,8);
    m_mumu_cf_noflip = new TH1D( "m_mumu_cf_noflip", "mumu cutflow (no flip)", 8,0,8);
    m_mumu_cf_flip = new TH1D( "m_mumu_cf_flip", "mumu cutflow (flip)", 8,0,8);

    m_emu_cf_input = new TH1D( "m_emu_cf_input", "Input emu cutflow", 8,0,8);
    m_emu_cf_noflip = new TH1D( "m_emu_cf_noflip", "emu cutflow (no flip)", 8,0,8);
    m_emu_cf_flip = new TH1D( "m_emu_cf_flip", "emu cutflow (flip)", 8,0,8);

    m_ee_cf_input = new TH1D( "m_ee_cf_input", "Input ee cutflow", 8,0,8);
    m_ee_cf_noflip = new TH1D( "m_ee_cf_noflip", "ee cutflow (no flip)", 8,0,8);
    m_ee_cf_flip = new TH1D( "m_ee_cf_flip", "ee cutflow (flip)", 8,0,8);

    m_mumu_noflip_R = new TH1F( "m_mumu_noflip_R", "vertex R (no flip)", 50,0,150);
    m_mumu_noflip_z = new TH1F( "m_mumu_noflip_z", "vertex z (no flip)", 50,-500,500);
    m_mumu_flip_R = new TH1F( "m_mumu_flip_R", "vertex R (no flip)", 50,0,50);
    m_mumu_flip_z = new TH1F( "m_mumu_flip_z", "vertex z (no flip)", 50,-500,500);

    m_ee_noflip_R = new TH1F( "m_ee_noflip_R", "vertex R (no flip)", 50,0,150);
    m_ee_noflip_z = new TH1F( "m_ee_noflip_z", "vertex z (no flip)", 50,-500,500);
    m_ee_flip_R = new TH1F( "m_ee_flip_R", "vertex R (no flip)", 50,0,50);
    m_ee_flip_z = new TH1F( "m_ee_flip_z", "vertex z (no flip)", 50,-500,500);

    m_emu_noflip_R = new TH1F( "m_emu_noflip_R", "vertex R (no flip)", 50,0,150);
    m_emu_noflip_z = new TH1F( "m_emu_noflip_z", "vertex z (no flip)", 50,-500,500);
    m_emu_flip_R = new TH1F( "m_emu_flip_R", "vertex R (no flip)", 50,0,50);
    m_emu_flip_z = new TH1F( "m_emu_flip_z", "vertex z (no flip)", 50,-500,500);

    CHECK( histSvc->regHist("/DV/FlipBkgEst/n_mu", m_n_mu) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/n_elc", m_n_elc) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/n_mu_sel", m_n_mu_sel) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/n_elc_sel", m_n_elc_sel) );

    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_mumu/mumu_cf_input", m_mumu_cf_input) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_mumu/mumu_cf_noflip", m_mumu_cf_noflip) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_mumu/mumu_cf_flip", m_mumu_cf_flip) );

    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_emu/emu_cf_input", m_emu_cf_input) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_emu/emu_cf_noflip", m_emu_cf_noflip) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_emu/emu_cf_flip", m_emu_cf_flip) );

    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_ee/ee_cf_input", m_ee_cf_input) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_ee/ee_cf_noflip", m_ee_cf_noflip) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_ee/ee_cf_flip", m_ee_cf_flip) );

    // vertex distribution
    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_mumu/mumu_noflip_R", m_mumu_noflip_R) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_mumu/mumu_noflip_z", m_mumu_noflip_z) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_mumu/mumu_flip_R", m_mumu_flip_R) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_mumu/mumu_flip_z", m_mumu_flip_z) );

    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_ee/ee_noflip_R", m_ee_noflip_R) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_ee/ee_noflip_z", m_ee_noflip_z) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_ee/ee_flip_R", m_ee_flip_R) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_ee/ee_flip_z", m_ee_flip_z) );

    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_emu/emu_noflip_R", m_emu_noflip_R) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_emu/emu_noflip_z", m_emu_noflip_z) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_emu/emu_flip_R", m_emu_flip_R) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/dv_emu/emu_flip_z", m_emu_flip_z) );

    return StatusCode::SUCCESS;
}

StatusCode FlipBkgEst::finalize() {
    return StatusCode::SUCCESS;
}

StatusCode FlipBkgEst::execute() {  
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    // retrieve event info
    const xAOD::EventInfo* evtInfo = nullptr;
    CHECK( evtStore()->retrieve( evtInfo, "EventInfo" ) );

    // flag to check if data or MC
    bool isMC = evtInfo->eventType(xAOD::EventInfo::IS_SIMULATION);

    // flag to check trigger
    bool trig_passed = false;

    // GRL
    if (!isMC and !m_grlTool->passRunLB(*evtInfo)) return StatusCode::SUCCESS;

    // event cleaning
    if(!m_evtc->PassEventCleaning(*evtInfo)) return StatusCode::SUCCESS;

    // trigger check
    if (m_tdt->isPassed("HLT_mu60_0eta105_msonly")) trig_passed = true;
    if (m_tdt->isPassed("HLT_g140_loose")) trig_passed = true;
    if (m_tdt->isPassed("HLT_2g50_loose")) trig_passed = true;
    if (m_tdt->isPassed("HLT_2g60_loose_L12EM15VH")) trig_passed = true;

    if(!trig_passed) return StatusCode::SUCCESS;

    // trigger check
    //if(!m_evtc->PassTrigger()) return StatusCode::SUCCESS;

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

    // apply overlap removal
    m_or->FindOverlap(*elc_copy.first, *muc_copy.first);

    // retrieve primary vertices
    const xAOD::VertexContainer* pvc = nullptr;
    CHECK( evtStore()->retrieve( pvc, "PrimaryVertices" ));

    // get primary vertex
    auto pv = m_evtc->GetPV(*pvc);
    auto pv_pos = m_evtc->GetPV(*pvc)->position();

    // retrieve secondary vertices
    const xAOD::VertexContainer* dvc = nullptr;
    CHECK( evtStore()->retrieve( dvc, "VrtSecInclusive_SecondaryVertices" ));

    // make a copy of vertex containers
    auto dvc_copy = xAOD::shallowCopyContainer(*dvc);

    // perform lepton matching
    for(auto dv: *dvc_copy.first) {
        m_dilepdvc->ApplyLeptonMatching(*dv, *elc_copy.first, *muc_copy.first);
    }

    // create container for good tracks
    auto el_sel     = new xAOD::TrackParticleContainer();
    auto el_sel_aux = new xAOD::TrackParticleAuxContainer();
    el_sel->setStore(el_sel_aux);

    auto mu_sel     = new xAOD::TrackParticleContainer();
    auto mu_sel_aux = new xAOD::TrackParticleAuxContainer();
    mu_sel->setStore(mu_sel_aux);

    m_n_mu->Fill(muc->size());
    m_n_elc->Fill(elc->size());

    for(auto mu: *muc_copy.first) {

        // apply muon selection
        if(!m_leptool->MuonSelection(*mu)) continue;

        // access muon ID track
        auto mu_tr = mu->trackParticle(xAOD::Muon::InnerDetectorTrackParticle);
        if(mu_tr == nullptr) continue;
        if(!m_vertexer->GoodTrack(*mu_tr)) continue;

        // copy ID track
        xAOD::TrackParticle* tr_ptr = new xAOD::TrackParticle();
        mu_sel->push_back(tr_ptr);
        xAOD::safeDeepCopy(*mu_tr, *tr_ptr);

        // decorate ID track with muon p4
        m_acc_p4(*tr_ptr) = mu->p4();
    }

    for(auto el: *elc_copy.first)
    {
        // remove bad electrons
        if(!m_leptool->BadClusterRemoval(*el)) continue;

        // kinematic cut
        if(!m_leptool->ElectronKinematicCut(*el)) continue;

        // Electron identification
        if(!m_leptool->ElectronID(*el)) continue;

        // access electron ID track
        auto el_tr = xAOD::EgammaHelpers::getOriginalTrackParticle(el);
        if(el_tr == nullptr) continue;
        if(!m_vertexer->GoodTrack(*el_tr)) continue;

        // copy ID track
        xAOD::TrackParticle* tr_ptr = new xAOD::TrackParticle();
        el_sel->push_back(tr_ptr);
        xAOD::safeDeepCopy(*el_tr, *tr_ptr);

        // decorate ID track with muon p4
        m_acc_p4(*tr_ptr) = el->p4();
    }

    // Fill selected leptons
    ATH_MSG_INFO("total electron size = " << elc_copy.first->size() << ", total muon size = " << muc_copy.first->size());
    ATH_MSG_INFO("selected electron size = " << el_sel->size() << ", selected muon size = " << mu_sel->size());
    m_n_mu_sel->Fill(mu_sel->size());
    m_n_elc_sel->Fill(el_sel->size());

    if(el_sel->size() + mu_sel->size() > 1) {
        // create containers for displaced vertices
        auto dv     = new xAOD::VertexContainer();
        auto dv_aux = new xAOD::VertexAuxContainer();
        dv->setStore(dv_aux);

        m_vertexer->SetVtxContainer(*dv);

        // perform vertexing
        for(auto el1_itr = el_sel->begin(); el1_itr != el_sel->end(); el1_itr++)
        {
            for(auto el2_itr = el1_itr+1; el2_itr != el_sel->end(); el2_itr++)
            {
                auto dp = (**el1_itr).definingParameters();
                ATH_MSG_INFO("**el1_itr (before invert) DP:" 
                            << ", d0 = " << dp[0]
                            << ", z0 = " << dp[1]
                            << ", phi0 = "  << dp[2]
                            << ", theta = " << dp[3]);

                // vertex fit of two original tracks
                PerformFit(**el1_itr, **el2_itr, pv_pos, "ee");

                // parameters to invert
                float d0_inv = -dp[0];
                float z0_inv = -dp[1];
                float phi0_inv = -dp[2];
                float theta_inv = M_PI - dp[3];

                // set defining parameters to flip track
                (**el1_itr).setDefiningParameters(d0_inv, z0_inv, phi0_inv, theta_inv, dp[4]);
                auto dp_inverted = (**el1_itr).definingParameters();

                ATH_MSG_INFO("**el1_itr (after invert) DP:" 
                            << ", d0 = " << dp_inverted[0]
                            << ", z0 = " << dp_inverted[1]
                            << ", phi0 = "  << dp_inverted[2] 
                            << ", theta = " << dp_inverted[3] );

                // vertex fit of two original tracks
                PerformFit_flip(**el1_itr, **el2_itr, pv_pos, "ee");

                // flip the track back
                (**el1_itr).setDefiningParameters(dp[0], dp[1], dp[2], dp[3], dp[4]);
                auto dp_back = (**el1_itr).definingParameters();
                ATH_MSG_INFO("**el1_itr (back to original) DP:" 
                            << ", d0 = " << dp_back[0]
                            << ", z0 = " << dp_back[1]
                            << ", phi0 = "  << dp_back[2] 
                            << ", theta = " << dp_back[3] );

            }

            for(auto mu_itr = mu_sel->begin(); mu_itr != mu_sel->end(); mu_itr++)
            {
                auto dp = (**el1_itr).definingParameters();
                ATH_MSG_INFO("**el1_itr (before invert) DP:" 
                            << ", d0 = " << dp[0]
                            << ", z0 = " << dp[1]
                            << ", phi0 = "  << dp[2] 
                            << ", theta = " << dp[3]);

                // vertex fit of two original tracks
                //ATH_MSG_INFO("Perform fit with two tracks = " << **el1_itr << ", " << **el2_itr);
                PerformFit(**el1_itr, **mu_itr, pv_pos, "emu");

                // parameters to invert
                float d0_inv = -dp[0];
                float z0_inv = -dp[1];
                float phi0_inv = -dp[2];
                float theta_inv = M_PI - dp[3];

                // set defining parameters to flip track
                (**el1_itr).setDefiningParameters(d0_inv, z0_inv, phi0_inv, theta_inv, dp[4]);
                auto dp_inverted = (**el1_itr).definingParameters();

                ATH_MSG_INFO("**el1_itr (after invert) DP:" 
                            << ", d0 = " << dp_inverted[0]
                            << ", z0 = " << dp_inverted[1]
                            << ", phi0 = "  << dp_inverted[2] 
                            << ", theta = " << dp_inverted[3] );

                // vertex fit of two original tracks
                PerformFit_flip(**el1_itr, **mu_itr, pv_pos, "emu");

                // flip the track back
                (**el1_itr).setDefiningParameters(dp[0], dp[1], dp[2], dp[3], dp[4]);
                auto dp_back = (**el1_itr).definingParameters();
                ATH_MSG_INFO("**el1_itr (back to original) DP:" 
                            << ", d0 = " << dp_back[0]
                            << ", z0 = " << dp_back[1]
                            << ", phi0 = "  << dp_back[2] 
                            << ", theta = " << dp_back[3] );

            }
        }
        for(auto mu1_itr = mu_sel->begin(); mu1_itr != mu_sel->end(); mu1_itr++)
        {
            for(auto mu2_itr = mu1_itr+1; mu2_itr != mu_sel->end(); mu2_itr++)
            {
                auto dp = (**mu1_itr).definingParameters();
                ATH_MSG_INFO("**mu1_itr (before invert) DP:" 
                            << ", d0 = " << dp[0]
                            << ", z0 = " << dp[1]
                            << ", phi0 = "  << dp[2] 
                            << ", theta = " << dp[3]);

                // vertex fit of two original tracks
                PerformFit(**mu1_itr, **mu2_itr, pv_pos, "mumu");

                // parameters to invert
                float d0_inv = -dp[0];
                float z0_inv = -dp[1];
                float phi0_inv = -dp[2];
                float theta_inv = M_PI - dp[3];

                // set defining parameters to flip track
                (**mu1_itr).setDefiningParameters(d0_inv, z0_inv, phi0_inv, theta_inv, dp[4]);
                auto dp_inverted = (**mu1_itr).definingParameters();

                ATH_MSG_INFO("**mu1_itr (after invert) DP:" 
                            << ", d0 = " << dp_inverted[0]
                            << ", z0 = " << dp_inverted[1]
                            << ", phi0 = "  << dp_inverted[2] 
                            << ", theta = " << dp_inverted[3] );

                // vertex fit of two original tracks
                PerformFit_flip(**mu1_itr, **mu2_itr, pv_pos, "mumu");

                // flip the track back
                (**mu1_itr).setDefiningParameters(dp[0], dp[1], dp[2], dp[3], dp[4]);
                auto dp_back = (**mu1_itr).definingParameters();
                ATH_MSG_INFO("**mu1_itr (back to original) DP:" 
                            << ", d0 = " << dp_back[0]
                            << ", z0 = " << dp_back[1]
                            << ", phi0 = "  << dp_back[2] 
                            << ", theta = " << dp_back[3] );

            }
        }

        delete dv;
        delete dv_aux;
    }

    delete elc_copy.first;
    delete elc_copy.second;
    delete muc_copy.first;
    delete muc_copy.second;
    delete el_sel;
    delete el_sel_aux;
    delete mu_sel;
    delete mu_sel_aux;

    return StatusCode::SUCCESS;
}

StatusCode FlipBkgEst::beginInputFile() { 
    return StatusCode::SUCCESS;
}


bool FlipBkgEst::PassCosmicVeto_R_CR(xAOD::TrackParticle& tr0, xAOD::TrackParticle& tr1){

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

    if (Rcos < Rcos_min) PassCosmicVeto = false;

    return PassCosmicVeto;
}

bool FlipBkgEst::PassCosmicVeto_DeltaR(xAOD::TrackParticle& tr0, xAOD::TrackParticle& tr1){

    bool PassCosmicVeto = true;
    float deltaR_min = 0.5;

    TLorentzVector tlv_tp0;
    TLorentzVector tlv_tp1;

    // define TLorentzVector of decay particles
    tlv_tp0 = tr0.p4();
    tlv_tp1 = tr1.p4();

    float deltaR = tlv_tp0.DeltaR(tlv_tp1);

    if (deltaR < deltaR_min) PassCosmicVeto = false;

    return PassCosmicVeto;
}


void FlipBkgEst::PerformFit(xAOD::TrackParticle& tr1, xAOD::TrackParticle& tr2, const Amg::Vector3D& pv,std::string channel)
{

    // perform vertex fit
    ATH_MSG_INFO("Performing vertex fit, channel = " << channel);
    auto fit = m_vertexer->FitVertex(tr1, tr2, pv);
    if(fit) {

        // retrieve primary vertices
        const xAOD::VertexContainer* pvc = nullptr;
        evtStore()->retrieve( pvc, "PrimaryVertices" );

        float vtx_x = fit->position().x();
        float vtx_y = fit->position().y();
        float vtx_z = fit->position().z();

        float vtx_perp = std::sqrt(vtx_x*vtx_x + vtx_y*vtx_y);

        ATH_MSG_INFO("PerfomrFit: Found vertex with perp = " << vtx_perp);

        ATH_MSG_INFO("Found vertex with channel " << channel);
        if(channel == "mumu") {
        
            m_mumu_cf_noflip->Fill("#mu#mu",1);

            // vertex fit quality
            if(!m_dvc->PassChisqCut(*fit)) return;
            m_mumu_cf_noflip->Fill("#chi^{2}_{DV} / DOF < 5", 1);

            // minimum distance from pv (from 0 for MC)
            if(!m_dvc->PassDistCut(*fit, *pvc)) return;
            m_mumu_cf_noflip->Fill("Disp. > 2 mm", 1);

            // charge requirements
            if(!m_dvc->PassChargeRequirement(*fit)) return;
            m_mumu_cf_noflip->Fill("#mu^{+}#mu^{-}", 1);

            // disabled module
            if(!m_dvc->PassDisabledModuleVeto(*fit)) return;
            m_mumu_cf_noflip->Fill("DisabledModule", 1);

            // material veto
            m_mumu_cf_noflip->Fill("MaterialVeto (Only e)", 1);

            // cosmic veto (R_CR)
            if(!PassCosmicVeto_R_CR(tr1, tr2)) return;
            m_mumu_cf_noflip->Fill("R_{CR} > 0.04", 1);

            // cosmic veto (deltaR)
            if(!PassCosmicVeto_DeltaR(tr1, tr2)) return;
            m_mumu_cf_noflip->Fill("#DeltaR > 0.5", 1);

            // vertex distribution fill
            m_mumu_noflip_R->Fill(vtx_perp);
            m_mumu_noflip_z->Fill(vtx_z);
    
        }
        if(channel == "emu") {
            m_emu_cf_noflip->Fill("e#mu",1);

            // vertex fit quality
            if(!m_dvc->PassChisqCut(*fit)) return;
            m_emu_cf_noflip->Fill("#chi^{2}_{DV} / DOF < 5", 1);

            // minimum distance from pv (from 0 for MC)
            if(!m_dvc->PassDistCut(*fit, *pvc)) return;
            m_emu_cf_noflip->Fill("Disp. > 2 mm", 1);

            // charge requirements
            if(!m_dvc->PassChargeRequirement(*fit)) return;
            m_emu_cf_noflip->Fill("e^{+/-}#mu^{-/+}", 1);

            // disabled module
            if(!m_dvc->PassDisabledModuleVeto(*fit)) return;
            m_emu_cf_noflip->Fill("DisabledModule", 1);

            // material veto (only e)
            m_emu_cf_noflip->Fill("MaterialVeto (Only e)", 1);

            // cosmic veto (R_CR)
            if(!PassCosmicVeto_R_CR(tr1, tr2)) return;
            m_emu_cf_noflip->Fill("R_{CR} > 0.04", 1);

            // cosmic veto (deltaR)
            if(!PassCosmicVeto_DeltaR(tr1, tr2)) return;
            m_emu_cf_noflip->Fill("#DeltaR > 0.5", 1);

            // vertex distribution fill
            m_emu_noflip_R->Fill(vtx_perp);
            m_emu_noflip_z->Fill(vtx_z);
        }
        if(channel == "ee") {
            m_ee_cf_noflip->Fill("ee",1);

            // vertex fit quality
            if(!m_dvc->PassChisqCut(*fit)) return;
            m_ee_cf_noflip->Fill("#chi^{2}_{DV} / DOF < 5", 1);

            // minimum distance from pv (from 0 for MC)
            if(!m_dvc->PassDistCut(*fit, *pvc)) return;
            m_ee_cf_noflip->Fill("Disp. > 2 mm", 1);

            // charge requirements
            if(!m_dvc->PassChargeRequirement(*fit)) return;
            m_ee_cf_noflip->Fill("e^{+}e^{-}", 1);

            // disabled module
            if(!m_dvc->PassDisabledModuleVeto(*fit)) return;
            m_ee_cf_noflip->Fill("DisabledModule", 1);

            // material veto (only e)
            m_ee_cf_noflip->Fill("MaterialVeto (Only e)", 1);

            // cosmic veto (R_CR)
            if(!PassCosmicVeto_R_CR(tr1, tr2)) return;
            m_ee_cf_noflip->Fill("R_{CR} > 0.04", 1);

            // cosmic veto (deltaR)
            if(!PassCosmicVeto_DeltaR(tr1, tr2)) return;
            m_ee_cf_noflip->Fill("#DeltaR > 0.5", 1);

            // vertex distribution fill
            m_ee_noflip_R->Fill(vtx_perp);
            m_ee_noflip_z->Fill(vtx_z);
        }
    }
}

void FlipBkgEst::PerformFit_flip(xAOD::TrackParticle& tr1, xAOD::TrackParticle& tr2, const Amg::Vector3D& pv,std::string channel)
{

    // perform vertex fit
    ATH_MSG_DEBUG("Performing vertex fit, channel = " << channel);
    auto fit = m_vertexer->FitVertex(tr1, tr2, pv);
    if(fit) {

        // retrieve primary vertices
        const xAOD::VertexContainer* pvc = nullptr;
        evtStore()->retrieve( pvc, "PrimaryVertices" );

        float vtx_x = fit->position().x();
        float vtx_y = fit->position().y();
        float vtx_z = fit->position().z();

        float vtx_perp = std::sqrt(vtx_x*vtx_x + vtx_y*vtx_y);

        ATH_MSG_INFO("PerfomrFit: Found vertex with perp = " << vtx_perp);


        ATH_MSG_INFO("Found vertex with channel " << channel);
        if(channel == "mumu") {
        
            m_mumu_cf_flip->Fill("#mu#mu",1);

            // vertex fit quality
            if(!m_dvc->PassChisqCut(*fit)) return;
            m_mumu_cf_flip->Fill("#chi^{2}_{DV} / DOF < 5", 1);

            // minimum distance from pv (from 0 for MC)
            if(!m_dvc->PassDistCut(*fit, *pvc)) return;
            m_mumu_cf_flip->Fill("Disp. > 2 mm", 1);

            // charge requirements
            if(!m_dvc->PassChargeRequirement(*fit)) return;
            m_mumu_cf_flip->Fill("#mu^{+}#mu^{-}", 1);

            // disabled module
            if(!m_dvc->PassDisabledModuleVeto(*fit)) return;
            m_mumu_cf_flip->Fill("DisabledModule", 1);

            // material veto
            m_mumu_cf_flip->Fill("MaterialVeto (Only e)", 1);

            // cosmic veto (R_CR)
            if(!PassCosmicVeto_R_CR(tr1, tr2)) return;
            m_mumu_cf_flip->Fill("R_{CR} > 0.04", 1);

            // cosmic veto (deltaR)
            if(!PassCosmicVeto_DeltaR(tr1, tr2)) return;
            m_mumu_cf_flip->Fill("#DeltaR > 0.5", 1);

            // vertex distribution fill
            m_mumu_flip_R->Fill(vtx_perp);
            m_mumu_flip_z->Fill(vtx_z);
    
        }
        if(channel == "emu") {
            m_emu_cf_flip->Fill("e#mu",1);

            // vertex fit quality
            if(!m_dvc->PassChisqCut(*fit)) return;
            m_emu_cf_flip->Fill("#chi^{2}_{DV} / DOF < 5", 1);

            // minimum distance from pv (from 0 for MC)
            if(!m_dvc->PassDistCut(*fit, *pvc)) return;
            m_emu_cf_flip->Fill("Disp. > 2 mm", 1);

            // charge requirements
            if(!m_dvc->PassChargeRequirement(*fit)) return;
            m_emu_cf_flip->Fill("e^{+/-}#mu^{-/+}", 1);

            // disabled module
            if(!m_dvc->PassDisabledModuleVeto(*fit)) return;
            m_emu_cf_flip->Fill("DisabledModule", 1);

            // material veto (only e)
            m_emu_cf_flip->Fill("MaterialVeto (Only e)", 1);

            // cosmic veto (R_CR)
            if(!PassCosmicVeto_R_CR(tr1, tr2)) return;
            m_emu_cf_flip->Fill("R_{CR} > 0.04", 1);

            // cosmic veto (deltaR)
            if(!PassCosmicVeto_DeltaR(tr1, tr2)) return;
            m_emu_cf_flip->Fill("#DeltaR > 0.5", 1);

            // vertex distribution fill
            m_emu_flip_R->Fill(vtx_perp);
            m_emu_flip_z->Fill(vtx_z);
        }
        if(channel == "ee") {
            m_ee_cf_flip->Fill("ee",1);

            // vertex fit quality
            if(!m_dvc->PassChisqCut(*fit)) return;
            m_ee_cf_flip->Fill("#chi^{2}_{DV} / DOF < 5", 1);

            // minimum distance from pv (from 0 for MC)
            if(!m_dvc->PassDistCut(*fit, *pvc)) return;
            m_ee_cf_flip->Fill("Disp. > 2 mm", 1);

            // charge requirements
            if(!m_dvc->PassChargeRequirement(*fit)) return;
            m_ee_cf_flip->Fill("e^{+}e^{-}", 1);

            // disabled module
            if(!m_dvc->PassDisabledModuleVeto(*fit)) return;
            m_ee_cf_flip->Fill("DisabledModule", 1);

            // material veto (only e)
            m_ee_cf_flip->Fill("MaterialVeto (Only e)", 1);

            // cosmic veto (R_CR)
            if(!PassCosmicVeto_R_CR(tr1, tr2)) return;
            m_ee_cf_flip->Fill("R_{CR} > 0.04", 1);

            // cosmic veto (deltaR)
            if(!PassCosmicVeto_DeltaR(tr1, tr2)) return;
            m_ee_cf_flip->Fill("#DeltaR > 0.5", 1);

            // vertex distribution fill
            m_ee_flip_R->Fill(vtx_perp);
            m_ee_flip_z->Fill(vtx_z);
        }
    }
}
