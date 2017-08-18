// DisplacedDimuonAnalysis includes
#include "TrackingSystematics.h"

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
#include "xAODTruth/TruthVertex.h"
#include "xAODTruth/TruthVertexContainer.h"

// tools
#include "PathResolver/PathResolver.h"

// for M_PI
#include "cmath"

// debug
#include <typeinfo>



TrackingSystematics::TrackingSystematics( const std::string& name, ISvcLocator* pSvcLocator ) : 
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
m_tmt("Trig::MatchingTool/TrigMatchingTool"),
m_or("DDL::OverlapRemoval/OverlapRemoval"),
m_accMu("DDL_Muons"),
m_accEl("DDL_Electrons"),
//m_matchTool("InDetVertexTruthMatchTool"),
m_accMass("mass"),
m_acc_pt("pT")
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
    declareProperty("TrigMatchingTool", m_tmt);
    declareProperty("DiLepCosmics", m_cos);
    declareProperty("OverlapRemoval", m_or);
}


TrackingSystematics::~TrackingSystematics() {}


StatusCode TrackingSystematics::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");

    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    // event cut flow
    m_event_cutflow = new TH1D( "m_event_cutflow", "Event cutflow", 4,0,4);
    CHECK( histSvc->regHist("/DV/tracking_syst/event_cutflow", m_event_cutflow) );

    Float_t m_dv_idid_M_bins[] = {0.,50.,100.,150.,200.,250.,300.};

    // Ks candidate
    m_dv_idid_cf = new TH1D( "m_dv_idid_cf", "Reco dv idid cutflow", 10,0,10);
    m_dv_idid_M = new TH1F("dv_idid_M","K_{S} candidate mass in MeV", 100,0,1000 );
    m_dv_idid_R = new TH1F("dv_idid_R","K_{S} candidate R", 300,0,300);
    m_dv_idid_z = new TH1F("dv_idid_z","K_{S} candidate z", 100,-1000,1000);
    m_dv_idid_l = new TH1F("dv_idid_l","K_{S} candidate decay length", 100,0,1000);
    m_dv_idid_pt = new TH1F("dv_idid_pt","K_{S} candidate pt", 100,0,100);
    m_dv_idid_mu = new TH1F("dv_idid_mu","K_{S} candidate vs inst. mu", 50,0,50);
    m_dv_idid_DeltaR = new TH1F("dv_idid_DeltaR","K_{S} candidate #DeltaR", 100,0,5.);
    m_dv_idid_R_M = new TH2F("dv_idid_R_M","K_{S} position R vs M", 50,0,300,100,0,1000);

    //m_dv_idid_M_0 = new TH1F("dv_idid_M_0","K_{S} candidate mass in MeV (R_0)", 100,0,1000 );
    m_dv_idid_M_1 = new TH1F("dv_idid_M_1","K_{S} candidate mass in MeV (R_1)", 100,0,1000 );
    m_dv_idid_M_2 = new TH1F("dv_idid_M_2","K_{S} candidate mass in MeV (R_2)", 100,0,1000 );
    m_dv_idid_M_3 = new TH1F("dv_idid_M_3","K_{S} candidate mass in MeV (R_3)", 100,0,1000 );
    m_dv_idid_M_4 = new TH1F("dv_idid_M_4","K_{S} candidate mass in MeV (R_4)", 100,0,1000 );
    m_dv_idid_M_5 = new TH1F("dv_idid_M_5","K_{S} candidate mass in MeV (R_5)", 100,0,1000 );
    m_dv_idid_M_6 = new TH1F("dv_idid_M_6","K_{S} candidate mass in MeV (R_6)", 100,0,1000 );


    m_dv_idid_ratio_R = new TH1F("dv_idid_ratio_R","K_{S} ratio plot (R_{i} / R_{0})",6,0,300);

    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_cf", m_dv_idid_cf) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_M", m_dv_idid_M) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_R", m_dv_idid_R) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_z", m_dv_idid_z) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_l", m_dv_idid_l) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_pt", m_dv_idid_pt) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_mu", m_dv_idid_mu) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_DeltaR", m_dv_idid_DeltaR) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_R_M", m_dv_idid_R_M) );

    //CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/m/dv_idid_M_0", m_dv_idid_M_0) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/m/dv_idid_M_1", m_dv_idid_M_1) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/m/dv_idid_M_2", m_dv_idid_M_2) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/m/dv_idid_M_3", m_dv_idid_M_3) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/m/dv_idid_M_4", m_dv_idid_M_4) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/m/dv_idid_M_5", m_dv_idid_M_5) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/m/dv_idid_M_6", m_dv_idid_M_6) );

    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_ratio_R", m_dv_idid_ratio_R) );

    // truth-matched Ks and Z' comparison
    m_Ks_r = new TH1F("dv_Ks_r","K_{S} candidate r", 60,0,300);
    m_Ks_z = new TH1F("dv_Ks_z","K_{S} candidate z", 50,-1000,1000);
    m_Ks_pt = new TH1F("dv_Ks_pt","K_{S} candidate pt", 50,0,500); // GeV
    m_Ks_eta = new TH1F("dv_Ks_eta","K_{S} candidate eta", 60,-3,3); // GeV
    m_Ks_DeltaR = new TH1F("dv_Ks_DeltaR","K_{S} candidate #DeltaR", 100,0,5.);

    CHECK( histSvc->regHist("/DV/tracking_syst/truth-matched_Ks/Ks_r", m_Ks_r) );
    CHECK( histSvc->regHist("/DV/tracking_syst/truth-matched_Ks/Ks_z", m_Ks_z) );
    CHECK( histSvc->regHist("/DV/tracking_syst/truth-matched_Ks/Ks_pt", m_Ks_pt) );
    CHECK( histSvc->regHist("/DV/tracking_syst/truth-matched_Ks/Ks_eta", m_Ks_eta) );
    CHECK( histSvc->regHist("/DV/tracking_syst/truth-matched_Ks/Ks_DeltaR", m_Ks_DeltaR) );

    m_zp_r = new TH1F("dv_zp_r","K_{S} candidate r", 60,0,300);
    m_zp_z = new TH1F("dv_zp_z","K_{S} candidate z", 50,-1000,1000);
    m_zp_pt = new TH1F("dv_zp_pt","K_{S} candidate pt", 50,0,500); // GeV
    m_zp_eta = new TH1F("dv_zp_eta","K_{S} candidate eta", 60,-3,3); // GeV
    m_zp_DeltaR = new TH1F("dv_zp_DeltaR","K_{S} candidate #DeltaR", 100,0,5.);

    CHECK( histSvc->regHist("/DV/tracking_syst/truth-matched_zp/zp_r", m_zp_r) );
    CHECK( histSvc->regHist("/DV/tracking_syst/truth-matched_zp/zp_z", m_zp_z) );
    CHECK( histSvc->regHist("/DV/tracking_syst/truth-matched_zp/zp_pt", m_zp_pt) );
    CHECK( histSvc->regHist("/DV/tracking_syst/truth-matched_zp/zp_eta", m_zp_eta) );
    CHECK( histSvc->regHist("/DV/tracking_syst/truth-matched_zp/zp_DeltaR", m_zp_DeltaR) );

    // Ks candidate from primary vertex
    //m_ks_pv_cf = new TH1D( "m_ks_pv_cf", "Reco dv idid cutflow", 12,0,12);
    //m_ks_pv_M = new TH1F("m_ks_pv_M","K_{S} candidate mass in GeV", 100,0,1000 );
    //m_ks_pv_R = new TH1F("m_ks_pv_R","K_{S} candidate R", 50,0,2);
    //m_ks_pv_l = new TH1F("m_ks_pv_l","K_{S} candidate decay length", 100,0,200);

    // Ks candidate from primary vertex
    //CHECK( histSvc->regHist("/DV/tracking_syst/ks_pv/ks_pv_cf", m_ks_pv_cf) );
    //CHECK( histSvc->regHist("/DV/tracking_syst/ks_pv/ks_pv_M", m_ks_pv_M) );
    //CHECK( histSvc->regHist("/DV/tracking_syst/ks_pv/ks_pv_R", m_ks_pv_R) );
    //CHECK( histSvc->regHist("/DV/tracking_syst/ks_pv/ks_pv_l", m_ks_pv_l) );


    return StatusCode::SUCCESS;
}

StatusCode TrackingSystematics::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");

    return StatusCode::SUCCESS;
}

StatusCode TrackingSystematics::execute() {  
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    // retrieve event info
    const xAOD::EventInfo* evtInfo = nullptr;
    CHECK( evtStore()->retrieve( evtInfo, "EventInfo" ) );

    int mu = evtInfo->actualInteractionsPerCrossing();

    // flag to check if data or MC
    bool isMC = evtInfo->eventType(xAOD::EventInfo::IS_SIMULATION);

    //bool trig_passed = true;
    bool trig_passed = false;

    m_event_cutflow->Fill("AllEvents", 1);

    // GRL
    if (!isMC and !m_grlTool->passRunLB(*evtInfo)) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("GRL (Data)", 1);

    // event cleaning
    if(!m_evtc->PassEventCleaning(*evtInfo)) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("EvtCleaning (Data)", 1);

    if (m_tdt->isPassed("HLT_mu60_0eta105_msonly")) trig_passed = true;
    if (m_tdt->isPassed("HLT_g140_loose")) trig_passed = true;
    if (m_tdt->isPassed("HLT_2g50_loose")) trig_passed = true;
    if (m_tdt->isPassed("HLT_2g60_loose_L12EM15VH")) trig_passed = true;

    // trigger check
    if(!trig_passed) return StatusCode::SUCCESS;
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


    //// Ks candidate selection cuts
    //float track_pt_min = 400;
    //float track_eta_max = 2.5;
    //int n_TRT_min = 10;
    //int n_Si_min = 3;
    //float track_chi2_ndof_max = 5;
    //float d0_max = 2;
    //float z0_max = 2;
    //float cos_min = 0.995;
    //float l_min = 10.;
    //float chi2_ndof_max = 5.;

    //for (const xAOD::Vertex *vtx : (*pvc)) {

    //    // skip if this is primary vertex
    //    if (vtx->vertexType() == xAOD::VxType::PriVtx) continue;
    //    if (vtx->vertexType() == xAOD::VxType::NoVtx) continue;

    //    // require 2 tracks
    //    if (!(vtx->nTrackParticles() == 2)) continue;

    //    // LorentzVector to calculate invariant mass of two track
    //    TLorentzVector ks_cand;
    //    ATH_MSG_INFO("===========================");
    //    ATH_MSG_INFO("Ks mass before sum = " << ks_cand.M());

    //    bool pass_track = true;

    //    // loop over tracks at vertex
    //    const std::vector<ElementLink<xAOD::TrackParticleContainer>> tpLinks =  vtx->trackParticleLinks();
    //    for (const ElementLink<xAOD::TrackParticleContainer>& trk : vtx->trackParticleLinks()) {

    //        // check if we have a valid link
    //        if(!(trk)) continue;

    //        ATH_MSG_INFO("track pt = " << (*trk)->pt() << ", M = " << (*trk)->m() << ", E = " << (*trk)->e() );
   
    //        // sum p4 
    //        ks_cand += (*trk)->p4();

    //    //    // hit quality
    //    //    std::uint8_t nPix = 0;
    //    //    std::uint8_t nSCT = 0;
    //    //    std::uint8_t nTRT = 0;

    //    //    if(!(*trk)->summaryValue(nPix, xAOD::numberOfPixelHits)) {
    //    //        pass_track = false;
    //    //        continue;
    //    //    }
    //    //    if(!(*trk)->summaryValue(nSCT, xAOD::numberOfSCTHits)) {
    //    //        pass_track = false;
    //    //        continue;
    //    //    }
    //    //    if(!(*trk)->summaryValue(nTRT, xAOD::numberOfTRTHits)) {
    //    //        pass_track = false;
    //    //        continue;
    //    //    }

    //    //    auto numberDoF = (*trk)->numberDoF();
    //    //    if(numberDoF == 0) return false;

    //    //    if ((*trk)->pt() < track_pt_min) pass_track = false;
    //    //    if ((*trk)->eta() > track_eta_max) pass_track = false;
    //    //    if (nTRT <= n_TRT_min) pass_track = false;
    //    //    if ((nPix + nSCT) <= n_Si_min) pass_track = false;
    //    //    if(((*trk)->chiSquared() / numberDoF) > track_chi2_ndof_max) return false;
    //    }

    //    ATH_MSG_INFO("Ks mass after sum = " << ks_cand.M());

    //    // continue if all tracks don't pass selection
    //    if (pass_track == false) continue;

    //    // get 3D position vector
    //    const TVector3 pv_pos(pv->x(), pv->y(), pv->z());
    //    const TVector3 v_pos(vtx->x(), vtx->y(), vtx->z());

    //    //  Ks candidate 3D vector with respect to PV
    //    const TVector3 v_pos_wrt_pv(v_pos - pv_pos);

    //    // vector from PV to Ks
    //    TLorentzVector KsFlightDirection(0.,0.,0.,0.);
    //    KsFlightDirection.SetVect(v_pos_wrt_pv - pv_pos);

    //    double chi2 = vtx->chiSquared();
    //    double dof = vtx->numberDoF();
    //    double chi2ovdof = chi2/dof;
    //    float inv_mass = ks_cand.M();
    //    float ks_R = std::sqrt(v_pos_wrt_pv.x()*v_pos_wrt_pv.x() + v_pos_wrt_pv.y()*v_pos_wrt_pv.y());
    //    float ks_l = std::sqrt( v_pos_wrt_pv.x()*v_pos_wrt_pv.x() + 
    //          v_pos_wrt_pv.y()*v_pos_wrt_pv.y() + v_pos_wrt_pv.z()*v_pos_wrt_pv.z());

    //    // cut flow
    //    m_ks_pv_cf->Fill("Track pass", 1);

    //    //if (ks_l < l_min) continue;
    //    //m_ks_pv_cf->Fill("Decay length > 10 mm", 1);

    //    //if (chi2ovdof > chi2_ndof_max) continue;
    //    //m_ks_pv_cf->Fill("#Chi^2 / DOF < 5", 1);

    //    //if(!(std::abs(std::cos(ks_cand.Angle(KsFlightDirection.Vect()))) > cos_min)) continue;
    //    //m_ks_pv_cf->Fill("cos(#theta_{V}) > 0.995", 1);

    //    ATH_MSG_INFO("Ks inv_mass = " << inv_mass);

    //    m_ks_pv_M->Fill(inv_mass);
    //    m_ks_pv_R->Fill(ks_R);
    //    m_ks_pv_l->Fill(ks_l);

    //}


    //=======================================================
    // Ks cutflow
    //=======================================================
    for(auto dv: *dvc_copy.first) {

        // perform lepton matching
        m_dilepdvc->ApplyLeptonMatching(*dv, *elc_copy.first, *muc_copy.first);

        // remove overlapping muon
        m_dilepdvc->ApplyOverlapRemoval(*dv);

        // remove bad electrons
        m_leptool->BadClusterRemoval(*dv);

        // kinematic cut
        m_leptool->ElectronKinematicCut(*dv);

        // Electron identification
        m_leptool->ElectronID(*dv);

        // muon selection tool
        m_leptool->MuonSelection(*dv);

        // select only vertex with tracks
        if(dv->trackParticleLinks().size() != 2) continue;

        // find decay channel of dv
        std::string channel = m_dvutils->DecayChannel(*dv);

        // access invariant mass, pt, eta
        float dv_mass = std::fabs(m_accMass(*dv)); // in MeV
        float dv_pt = std::fabs(m_acc_pt(*dv)) / 1000.; // in GeV

        // access tracks from vertex
        auto tpLinks = dv->trackParticleLinks();
        xAOD::TrackParticle tp1 = **(tpLinks.at(0));
        xAOD::TrackParticle tp2 = **(tpLinks.at(1));

        // define lorentz vector to calculate delta R
        TLorentzVector tlv_tp0;
        TLorentzVector tlv_tp1;

        // set TLorentzVector of decay particles
        tlv_tp0 = tp1.p4();
        tlv_tp1 = tp2.p4();

        float deltaR = tlv_tp0.DeltaR(tlv_tp1);

        // fill dimuon vertex
        float dv_R = m_dvutils->getR( *dv, *pv ); // R in [mm]
        float dv_z = m_dvutils->getz( *dv, *pv ); // z in [mm]
        float dv_l = m_dvutils->getr( *dv, *pv ); // r in [mm]
        float dv_eta = m_dvutils->getEta( *dv, *pv ); // r in [mm]

        // collect leptons from this dv
        auto dv_muc = m_accMu(*dv);
        auto dv_elc = m_accEl(*dv);

        //if (channel == "trktrk") {
        if (true) {

            float track_pt_min = 400;   // MeV
            float mass_min = 350;   // MeV
            float mass_max = 650;   // MeV

            // idid pair
            m_dv_idid_cf->Fill("Trk-Trk", 1);

            // vertex fit quality
            if(!m_dvc->PassChisqCut(*dv)) continue;
            m_dv_idid_cf->Fill("#chi^{2}_{DV} / DOF < 5", 1);

            // minimum distance from pv (from 0 for MC)
            if(!m_dvc->PassDistCut(*dv, *pvc)) continue;
            m_dv_idid_cf->Fill("Disp. > 2 mm", 1);

            // charge requirements
            if(!m_dvc->PassChargeRequirement(*dv)) continue;
            m_dv_idid_cf->Fill("trk^{+}-trk^{-}", 1);

            // disabled module
            if(!m_dvc->PassDisabledModuleVeto(*dv)) continue;
            m_dv_idid_cf->Fill("DisabledModule", 1);

            // material veto (only e)
            if(!m_dvc->PassMaterialVeto(*dv)) continue;
            m_dv_idid_cf->Fill("MaterialVeto (Only e)", 1);

            // cosmic veto (R_CR)
            if(!PassCosmicVeto_R_CR(tp1, tp2)) continue;
            m_dv_idid_cf->Fill("R_{CR} > 0.04", 1);

            // low mass veto
            if(dv_mass < mass_min) continue;
            m_dv_idid_cf->Fill("m > 350 MeV", 1);

            // low mass veto
            if(dv_mass > mass_max) continue;
            m_dv_idid_cf->Fill("m < 650 MeV", 1);

            // Ks candidate selection
            if(!PassKsSelection(*dv, *pv)) continue;
            m_dv_idid_cf->Fill("K_{s} candidate", 1);

            //// cosmic veto (deltaR)
            //if(!PassCosmicVeto_DeltaR(tp1, tp2)) continue;
            //m_dv_idid_cf->Fill("#DeltaR > 0.5", 1);

            //-----------------------------------
            // fill histogram
            //-----------------------------------

            m_dv_idid_M->Fill(dv_mass);
            m_dv_idid_R->Fill(dv_R);
            m_dv_idid_z->Fill(dv_z);
            m_dv_idid_l->Fill(dv_l);
            m_dv_idid_pt->Fill(dv_pt);
            m_dv_idid_mu->Fill(mu);
            m_dv_idid_DeltaR->Fill(deltaR);
            m_dv_idid_R_M->Fill(dv_R, dv_mass);

            // fill mass plot
            //if (dv_R < 10) m_dv_idid_M_0->Fill(dv_mass);
            if (dv_R < 50)   m_dv_idid_M_1->Fill(dv_mass);
            if ((dv_R >= 50) and (dv_R < 100))  m_dv_idid_M_2->Fill(dv_mass);
            if ((dv_R >= 100) and (dv_R < 150)) m_dv_idid_M_3->Fill(dv_mass);
            if ((dv_R >= 150) and (dv_R < 200)) m_dv_idid_M_4->Fill(dv_mass);
            if ((dv_R >= 200) and (dv_R < 250)) m_dv_idid_M_5->Fill(dv_mass);
            if ((dv_R >= 250) and (dv_R < 300)) m_dv_idid_M_6->Fill(dv_mass);

            // fill ratio plot
            m_dv_idid_ratio_R->Fill(dv_R); // to be divided by the first bin after

        }

        // make histograms from truth-matched Ks
        if (isMC){

            // find closest truth vertex
            const xAOD::TruthVertex *tru_matched = nullptr;
            tru_matched = m_dvutils->getClosestTruthVertex(dv);

            // check if the matched truth vertex is from Ks
            if (tru_matched){
                const xAOD::TruthParticle* parent = tru_matched->incomingParticle(0);
                if (!(parent->absPdgId() ==310)) continue;

                m_Ks_r->Fill(dv_R);
                m_Ks_z->Fill(dv_z);
                m_Ks_pt->Fill(dv_pt);
                m_Ks_eta->Fill(dv_eta); 
                m_Ks_DeltaR->Fill(deltaR); 

            }
        }
    } // end of Ks loop


    //=======================================================
    // Z' DV cutflow
    // Here, we make pt, eta, delta R, r of truth-matched Z'
    //=======================================================
    for(auto dv: *dvc_copy.first) {

        // minimum delta R
        float deltaR_min = 0.5;
        
        // mass cut
        float mass_min = 3.;

        // select only vertex with tracks
        if(dv->trackParticleLinks().size() != 2) continue;

        // perform lepton matching
        m_dilepdvc->ApplyLeptonMatching(*dv, *elc_copy.first, *muc_copy.first);

        // remove overlapping muon
        m_dilepdvc->ApplyOverlapRemoval(*dv);

        // remove bad electrons
        m_leptool->BadClusterRemoval(*dv);

        // kinematic cut
        m_leptool->ElectronKinematicCut(*dv);

        // Electron identification
        m_leptool->ElectronID(*dv);

        // muon selection tool
        m_leptool->MuonSelection(*dv);

        // find decay channel of dv
        std::string channel = m_dvutils->DecayChannel(*dv);

        // retrieve mass, pt and eta of DV
        float dv_mass = std::fabs(m_accMass(*dv)); // in MeV
        float dv_pt = std::fabs(m_acc_pt(*dv)) / 1000.; // in GeV

        // access tracks from vertex
        auto tpLinks = dv->trackParticleLinks();

        xAOD::TrackParticle tp1 = **(tpLinks.at(0));
        xAOD::TrackParticle tp2 = **(tpLinks.at(1));

        // define lorentz vector to calculate delta R
        TLorentzVector tlv_tp0;
        TLorentzVector tlv_tp1;

        // set TLorentzVector of decay particles
        tlv_tp0 = tp1.p4();
        tlv_tp1 = tp2.p4();

        float deltaR = tlv_tp0.DeltaR(tlv_tp1);

        // fill dimuon vertex
        float dv_R = m_dvutils->getR( *dv, *pv ); // R in [mm]
        float dv_z = m_dvutils->getz( *dv, *pv ); // z in [mm]
        float dv_l = m_dvutils->getr( *dv, *pv ); // r in [mm]
        float dv_eta = m_dvutils->getEta( *dv, *pv ); // r in [mm]

        // collect leptons from this dv
        auto dv_muc = m_accMu(*dv);
        auto dv_elc = m_accEl(*dv);


        if ((channel == "mumu") or (channel == "emu") or (channel == "ee")) {

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

            // material veto
            if(!m_dvc->PassMaterialVeto(*dv)) continue;

            // low mass veto
            if(dv_mass < mass_min) continue;

            // cosmic veto
            if(!PassCosmicVeto_R_CR(tp1, tp2)) continue;

            // delta R 
            if(!PassCosmicVeto_DeltaR(tp1, tp2)) continue;

            // make histograms from truth-matched Ks
            if(isMC){
                // find closest truth vertex
                const xAOD::TruthVertex *tru_matched = nullptr;
                tru_matched = m_dvutils->getClosestTruthVertex(dv);

                // check if the matched truth vertex is from Z'
                if (tru_matched){
                    const xAOD::TruthParticle* parent = tru_matched->incomingParticle(0);
                    if (!(parent->absPdgId() ==32)) continue;

                    m_zp_r->Fill(dv_R);
                    m_zp_z->Fill(dv_z);
                    m_zp_pt->Fill(dv_pt);
                    m_zp_eta->Fill(dv_eta); 
                    m_zp_DeltaR->Fill(deltaR); 

                }

            }
        }
    } // end of Z' DV loop

    return StatusCode::SUCCESS;
}

StatusCode TrackingSystematics::beginInputFile() { 
    return StatusCode::SUCCESS;
}

bool TrackingSystematics::PassCosmicVeto_R_CR(xAOD::TrackParticle& tr0, xAOD::TrackParticle& tr1){

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

bool TrackingSystematics::PassCosmicVeto_DeltaR(xAOD::TrackParticle& tr0, xAOD::TrackParticle& tr1){

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

bool TrackingSystematics::PassKsSelection(const xAOD::Vertex& dv, const xAOD::Vertex& pv){

    bool PassKsCandidate = true;

    float d0_max = 100.;
    float z0_max = 100.;
    float pt_min = 450.;
    float delta_z0_max = 2.0;
    float cos_min = 0.;
    float l_min = 15.;

    // LorentzVector of Ks candidate
    TLorentzVector ks_cand;

    // access two tracks
    auto tpLinks = dv.trackParticleLinks();
    xAOD::TrackParticle tp1 = **(tpLinks.at(0));
    xAOD::TrackParticle tp2 = **(tpLinks.at(1));
    
    // get 3D position vector
    const TVector3 pv_pos(pv.x(), pv.y(), pv.z());
    const TVector3 dv_pos(dv.x(), dv.y(), dv.z());
    
    // vector from PV to Ks
    TLorentzVector KsFlightDirection(0.,0.,0.,0.);
    KsFlightDirection.SetVect(dv_pos - pv_pos);

    // |z0_1 - z0_2|
    float delta_z0 = tp1.auxdataConst<float>("z0_wrtSV") - tp2.auxdataConst<float>("z0_wrtSV");

    // decay length
    float dv_l = std::sqrt(dv.x()*dv.x() + dv.y()*dv.y() + dv.z()*dv.z());

    // d0, z0, pt track requirements
    for(const auto & trk: dv.trackParticleLinks())
    {
        if ((*trk)->auxdataConst<float>("d0_wrtSV") > d0_max) PassKsCandidate = false;
        if ((*trk)->auxdataConst<float>("z0_wrtSV") > z0_max) PassKsCandidate = false;
        if ((*trk)->auxdataConst<float>("pt_wrtSV") < pt_min) PassKsCandidate = false;

        // sum p4 
        ks_cand += (*trk)->p4();
    }
    // difference in z0 with respect to DV
    if(delta_z0 > delta_z0_max) PassKsCandidate = false;

    // require Ks candidates to be aligned with DV - PV
    if( std::abs(std::cos(ks_cand.Angle(KsFlightDirection.Vect()))) < cos_min ) PassKsCandidate = false;

    // require minimum decay length
    if(dv_l < l_min) PassKsCandidate = false;


    return PassKsCandidate;

}
