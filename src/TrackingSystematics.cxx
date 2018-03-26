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
m_phmatch("DDL::PhotonMatch/PhotonMatch"),
m_accMass("mass"),
m_prw("CP::PileupReweightingTool/PileupReweightingTool"),
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
    declareProperty("PhotonMatch", m_phmatch);
    declareProperty("PileupReweightingTool", m_prw);

    declareProperty("usePRW", m_usePRW = false );
    declareProperty("useSV", m_useSV = true );
    declareProperty("usePV", m_usePV = true );
}


TrackingSystematics::~TrackingSystematics() {}


StatusCode TrackingSystematics::initialize() {

    // initialize tools
    ATH_CHECK(m_dilepdvc.retrieve());
    ATH_CHECK(m_dvutils.retrieve());
    ATH_CHECK(m_leptool.retrieve());
    ATH_CHECK(m_cos.retrieve());
    ATH_CHECK(m_evtc.retrieve());
    ATH_CHECK(m_dvc.retrieve());
    ATH_CHECK(m_grlTool.retrieve());
    ATH_CHECK(m_tdt.retrieve());
    ATH_CHECK(m_tmt.retrieve());
    ATH_CHECK(m_costool.retrieve());
    ATH_CHECK(m_or.retrieve());
    ATH_CHECK(m_phmatch.retrieve());
    ATH_CHECK(m_prw.retrieve());


    ATH_MSG_INFO ("Initializing " << name() << "...");

    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    // event cut flow
    m_event_cutflow = new TH1D( "m_event_cutflow", "Event cutflow", 6,0,6);
    CHECK( histSvc->regHist("/DV/tracking_syst/event_cutflow", m_event_cutflow) );

    // pileup distribution
    m_pileup = new TH1F("m_pileup", "m_pileup", 100, 0, 100); 
    m_pileup_reweighted = new TH1F("m_pileup_reweighted", "m_pileup_reweighted", 100, 0, 100); 
    
    CHECK( histSvc->regHist("/DV/tracking_syst/pileup", m_pileup) );
    CHECK( histSvc->regHist("/DV/tracking_syst/pileup_reweighted", m_pileup_reweighted) );

    // Ks candidate
    m_dv_idid_cf = new TH1D( "m_dv_idid_cf", "Reco dv idid cutflow", 10,0,10);
    m_dv_idid_M = new TH1F("dv_idid_M","K_{S} candidate mass in MeV", 100, 350, 650 );
    m_dv_idid_R = new TH1F("dv_idid_R","K_{S} candidate R", 300,0,300);
    m_dv_idid_z = new TH1F("dv_idid_z","K_{S} candidate z", 100,-1000,1000);
    m_dv_idid_l = new TH1F("dv_idid_l","K_{S} candidate decay length", 100,0,1000);
    m_dv_idid_pt = new TH1F("dv_idid_pt","K_{S} candidate pt", 100,0,10);
    m_dv_idid_eta = new TH1F("dv_idid_eta","K_{S} candidate eta", 50, 0, 3.0);
    m_dv_idid_mu = new TH1F("dv_idid_mu","K_{S} candidate vs inst. mu", 50,0,50);
    m_dv_idid_DeltaR = new TH1F("dv_idid_DeltaR","K_{S} candidate #DeltaR", 100,0,5.);
    m_dv_idid_track_pt = new TH1F("dv_idid_track_pt","K_{S} candidate track_pt", 500,0,50);
    m_dv_idid_R_M = new TH2F("dv_idid_R_M","K_{S} position R vs M", 50,0,300,100,0,1000);
    
    // Ks using only LRT tracks
    m_dv_LRT_M = new TH1F("dv_LRT_M","K_{S} candidate mass in MeV", 100, 350, 650 );
    m_dv_LRT_M_R1 = new TH1F("dv_LRT_M_R1","K_{S} candidate mass in MeV (R_1)", 100, 350, 650 );
    m_dv_LRT_M_R2 = new TH1F("dv_LRT_M_R2","K_{S} candidate mass in MeV (R_2)", 100, 350, 650 );
    m_dv_LRT_M_R3 = new TH1F("dv_LRT_M_R3","K_{S} candidate mass in MeV (R_3)", 100, 350, 650 );
    m_dv_LRT_M_R4 = new TH1F("dv_LRT_M_R4","K_{S} candidate mass in MeV (R_4)", 100, 350, 650 );
    m_dv_LRT_M_R5 = new TH1F("dv_LRT_M_R5","K_{S} candidate mass in MeV (R_5)", 100, 350, 650 );

    m_dv_LRT_M_pt1 = new TH1F("dv_LRT_M_pt1","K_{S} candidate mass in MeV (pt_1)", 100, 350, 650 );
    m_dv_LRT_M_pt2 = new TH1F("dv_LRT_M_pt2","K_{S} candidate mass in MeV (pt_2)", 100, 350, 650 );
    m_dv_LRT_M_pt3 = new TH1F("dv_LRT_M_pt3","K_{S} candidate mass in MeV (pt_3)", 100, 350, 650 );
    m_dv_LRT_M_pt4 = new TH1F("dv_LRT_M_pt4","K_{S} candidate mass in MeV (pt_4)", 100, 350, 650 );

    m_dv_LRT_M_eta1 = new TH1F("dv_LRT_M_eta1","K_{S} candidate mass in MeV (eta_1)", 100, 350, 650 );
    m_dv_LRT_M_eta2 = new TH1F("dv_LRT_M_eta2","K_{S} candidate mass in MeV (eta_2)", 100, 350, 650 );
    m_dv_LRT_M_eta3 = new TH1F("dv_LRT_M_eta3","K_{S} candidate mass in MeV (eta_3)", 100, 350, 650 );
    m_dv_LRT_M_eta4 = new TH1F("dv_LRT_M_eta4","K_{S} candidate mass in MeV (eta_4)", 100, 350, 650 );

    // Ks using only ST tracks
    m_dv_ST_M = new TH1F("dv_ST_M","K_{S} candidate mass in MeV (ST+ST)", 100, 350, 650 );
    m_dv_ST_M_R1 = new TH1F("dv_ST_M_R1","K_{S} candidate mass in MeV (R_1),(ST+ST)", 100, 350, 650 );
    m_dv_ST_M_R2 = new TH1F("dv_ST_M_R2","K_{S} candidate mass in MeV (R_2),(ST+ST)", 100, 350, 650 );
    m_dv_ST_M_R3 = new TH1F("dv_ST_M_R3","K_{S} candidate mass in MeV (R_3),(ST+ST)", 100, 350, 650 );
    m_dv_ST_M_R4 = new TH1F("dv_ST_M_R4","K_{S} candidate mass in MeV (R_4),(ST+ST)", 100, 350, 650 );
    m_dv_ST_M_R5 = new TH1F("dv_ST_M_R5","K_{S} candidate mass in MeV (R_5),(ST+ST)", 100, 350, 650 );

    m_dv_ST_M_pt1 = new TH1F("dv_ST_M_pt1","K_{S} candidate mass in MeV (pt_1)", 100, 350, 650 );
    m_dv_ST_M_pt2 = new TH1F("dv_ST_M_pt2","K_{S} candidate mass in MeV (pt_2)", 100, 350, 650 );
    m_dv_ST_M_pt3 = new TH1F("dv_ST_M_pt3","K_{S} candidate mass in MeV (pt_3)", 100, 350, 650 );
    m_dv_ST_M_pt4 = new TH1F("dv_ST_M_pt4","K_{S} candidate mass in MeV (pt_4)", 100, 350, 650 );

    m_dv_ST_M_eta1 = new TH1F("dv_ST_M_eta1","K_{S} candidate mass in MeV (eta_1)", 100, 350, 650 );
    m_dv_ST_M_eta2 = new TH1F("dv_ST_M_eta2","K_{S} candidate mass in MeV (eta_2)", 100, 350, 650 );
    m_dv_ST_M_eta3 = new TH1F("dv_ST_M_eta3","K_{S} candidate mass in MeV (eta_3)", 100, 350, 650 );
    m_dv_ST_M_eta4 = new TH1F("dv_ST_M_eta4","K_{S} candidate mass in MeV (eta_4)", 100, 350, 650 );

    // Ks using ST and LRT tracks
    m_dv_STLRT_M = new TH1F("dv_STLRT_M","K_{S} candidate mass in MeV (LRT +ST)", 100, 350, 650 );
    m_dv_STLRT_M_R1 = new TH1F("dv_STLRT_M_R1","K_{S} candidate mass in MeV (R_1),(ST+LRT)", 100, 350, 650 );
    m_dv_STLRT_M_R2 = new TH1F("dv_STLRT_M_R2","K_{S} candidate mass in MeV (R_2),(ST+LRT)", 100, 350, 650 );
    m_dv_STLRT_M_R3 = new TH1F("dv_STLRT_M_R3","K_{S} candidate mass in MeV (R_3),(ST+LRT)", 100, 350, 650 );
    m_dv_STLRT_M_R4 = new TH1F("dv_STLRT_M_R4","K_{S} candidate mass in MeV (R_4),(ST+LRT)", 100, 350, 650 );
    m_dv_STLRT_M_R5 = new TH1F("dv_STLRT_M_R5","K_{S} candidate mass in MeV (R_5),(ST+LRT)", 100, 350, 650 );

    m_dv_STLRT_M_pt1 = new TH1F("dv_STLRT_M_pt1","K_{S} candidate mass in MeV (pt_1)", 100, 350, 650 );
    m_dv_STLRT_M_pt2 = new TH1F("dv_STLRT_M_pt2","K_{S} candidate mass in MeV (pt_2)", 100, 350, 650 );
    m_dv_STLRT_M_pt3 = new TH1F("dv_STLRT_M_pt3","K_{S} candidate mass in MeV (pt_3)", 100, 350, 650 );
    m_dv_STLRT_M_pt4 = new TH1F("dv_STLRT_M_pt4","K_{S} candidate mass in MeV (pt_4)", 100, 350, 650 );

    m_dv_STLRT_M_eta1 = new TH1F("dv_STLRT_M_eta1","K_{S} candidate mass in MeV (eta_1)", 100, 350, 650 );
    m_dv_STLRT_M_eta2 = new TH1F("dv_STLRT_M_eta2","K_{S} candidate mass in MeV (eta_2)", 100, 350, 650 );
    m_dv_STLRT_M_eta3 = new TH1F("dv_STLRT_M_eta3","K_{S} candidate mass in MeV (eta_3)", 100, 350, 650 );
    m_dv_STLRT_M_eta4 = new TH1F("dv_STLRT_M_eta4","K_{S} candidate mass in MeV (eta_4)", 100, 350, 650 );


    // output histograms
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_cf", m_dv_idid_cf) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_M", m_dv_idid_M) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_R", m_dv_idid_R) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_z", m_dv_idid_z) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_l", m_dv_idid_l) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_pt", m_dv_idid_pt) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_eta", m_dv_idid_eta) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_mu", m_dv_idid_mu) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_DeltaR", m_dv_idid_DeltaR) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_track_pt", m_dv_idid_track_pt) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_idid/dv_idid_R_M", m_dv_idid_R_M) );

    // Ks using only LRT tracks
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_LRT/dv_LRT_M", m_dv_LRT_M) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_LRT/m/dv_LRT_M_R1", m_dv_LRT_M_R1) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_LRT/m/dv_LRT_M_R2", m_dv_LRT_M_R2) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_LRT/m/dv_LRT_M_R3", m_dv_LRT_M_R3) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_LRT/m/dv_LRT_M_R4", m_dv_LRT_M_R4) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_LRT/m/dv_LRT_M_R5", m_dv_LRT_M_R5) );

    CHECK( histSvc->regHist("/DV/tracking_syst/dv_LRT/m/dv_LRT_M_pt1", m_dv_LRT_M_pt1) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_LRT/m/dv_LRT_M_pt2", m_dv_LRT_M_pt2) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_LRT/m/dv_LRT_M_pt3", m_dv_LRT_M_pt3) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_LRT/m/dv_LRT_M_pt4", m_dv_LRT_M_pt4) );

    CHECK( histSvc->regHist("/DV/tracking_syst/dv_LRT/m/dv_LRT_M_eta1", m_dv_LRT_M_eta1) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_LRT/m/dv_LRT_M_eta2", m_dv_LRT_M_eta2) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_LRT/m/dv_LRT_M_eta3", m_dv_LRT_M_eta3) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_LRT/m/dv_LRT_M_eta4", m_dv_LRT_M_eta4) );

    // Ks using only ST tracks
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_ST/dv_ST_M", m_dv_ST_M) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_ST/m/dv_ST_M_R1", m_dv_ST_M_R1) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_ST/m/dv_ST_M_R2", m_dv_ST_M_R2) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_ST/m/dv_ST_M_R3", m_dv_ST_M_R3) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_ST/m/dv_ST_M_R4", m_dv_ST_M_R4) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_ST/m/dv_ST_M_R5", m_dv_ST_M_R5) );

    CHECK( histSvc->regHist("/DV/tracking_syst/dv_ST/m/dv_ST_M_pt1", m_dv_ST_M_pt1) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_ST/m/dv_ST_M_pt2", m_dv_ST_M_pt2) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_ST/m/dv_ST_M_pt3", m_dv_ST_M_pt3) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_ST/m/dv_ST_M_pt4", m_dv_ST_M_pt4) );

    CHECK( histSvc->regHist("/DV/tracking_syst/dv_ST/m/dv_ST_M_eta1", m_dv_ST_M_eta1) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_ST/m/dv_ST_M_eta2", m_dv_ST_M_eta2) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_ST/m/dv_ST_M_eta3", m_dv_ST_M_eta3) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_ST/m/dv_ST_M_eta4", m_dv_ST_M_eta4) );

    // Ks using ST and LRT tracks
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_STLRT/dv_STLRT_M", m_dv_STLRT_M) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_STLRT/m/dv_STLRT_M_R1", m_dv_STLRT_M_R1) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_STLRT/m/dv_STLRT_M_R2", m_dv_STLRT_M_R2) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_STLRT/m/dv_STLRT_M_R3", m_dv_STLRT_M_R3) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_STLRT/m/dv_STLRT_M_R4", m_dv_STLRT_M_R4) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_STLRT/m/dv_STLRT_M_R5", m_dv_STLRT_M_R5) );

    CHECK( histSvc->regHist("/DV/tracking_syst/dv_STLRT/m/dv_STLRT_M_pt1", m_dv_STLRT_M_pt1) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_STLRT/m/dv_STLRT_M_pt2", m_dv_STLRT_M_pt2) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_STLRT/m/dv_STLRT_M_pt3", m_dv_STLRT_M_pt3) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_STLRT/m/dv_STLRT_M_pt4", m_dv_STLRT_M_pt4) );

    CHECK( histSvc->regHist("/DV/tracking_syst/dv_STLRT/m/dv_STLRT_M_eta1", m_dv_STLRT_M_eta1) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_STLRT/m/dv_STLRT_M_eta2", m_dv_STLRT_M_eta2) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_STLRT/m/dv_STLRT_M_eta3", m_dv_STLRT_M_eta3) );
    CHECK( histSvc->regHist("/DV/tracking_syst/dv_STLRT/m/dv_STLRT_M_eta4", m_dv_STLRT_M_eta4) );



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

    return StatusCode::SUCCESS;
}

StatusCode TrackingSystematics::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");

    ATH_MSG_INFO ("n_standard = " << n_standard);
    ATH_MSG_INFO ("n_lrt = " << n_lrt);


    return StatusCode::SUCCESS;
}

StatusCode TrackingSystematics::execute() {  
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    // retrieve event info
    const xAOD::EventInfo* evtInfo = nullptr;
    CHECK( evtStore()->retrieve( evtInfo, "EventInfo" ) );

    // pile-up
    int pileup = evtInfo->actualInteractionsPerCrossing();

    // flag to check if data or MC
    bool isMC = evtInfo->eventType(xAOD::EventInfo::IS_SIMULATION);

    // get combine weight from pileup reweighting tool
    int mu = 0;
    if (m_usePRW && isMC) {
        p_weight = m_prw->getCombinedWeight(*evtInfo);
        ATH_MSG_DEBUG("pileup weight = " << p_weight);

        // pile-up
        mu = evtInfo->actualInteractionsPerCrossing();
    }

    // fill pileup distribution
    m_pileup->Fill(mu,1);
    m_pileup_reweighted->Fill(mu,p_weight);


    // retrieve lepton container
    const xAOD::MuonContainer* muc = nullptr;
    CHECK( evtStore()->retrieve( muc, "Muons" ));

    const xAOD::ElectronContainer* elc = nullptr;
    CHECK( evtStore()->retrieve( elc, "Electrons" ));

    const xAOD::PhotonContainer* phc = nullptr;
    CHECK(evtStore()->retrieve(phc, "Photons"));

    // retrieve primary vertices
    const xAOD::VertexContainer* pvc = nullptr;
    CHECK( evtStore()->retrieve( pvc, "PrimaryVertices" ));

    // retrieve GSF conversion vertices
    const xAOD::VertexContainer* gvc = nullptr;
    CHECK( evtStore()->retrieve( gvc, "GSFConversionVertices" ));

    //bool trig_passed = true;
    bool trig_passed = false;

    // make copies of leptons
    auto muc_copy = xAOD::shallowCopyContainer(*muc);
    xAOD::setOriginalObjectLink(*muc, *muc_copy.first);

    auto elc_copy = xAOD::shallowCopyContainer(*elc);
    xAOD::setOriginalObjectLink(*elc, *elc_copy.first);

    // apply overlap removal
    m_or->FindOverlap(*elc_copy.first, *muc_copy.first);

    // perform matching of photons to electrons
    m_phmatch->MatchPhotons(*phc, *elc_copy.first);

    //---------------------------------------
    // Event cut flow
    //---------------------------------------

    if(isMC){
        m_event_cutflow->Fill("AllEvents",p_weight);
    }
    else{
        // all events already passed RPVLL filter
        m_event_cutflow->Fill("RPVLLFilter", p_weight);
    }

    // GRL
    if (!isMC and !m_grlTool->passRunLB(*evtInfo)) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("GRL (Data)", p_weight);

    // event cleaning
    if(!m_evtc->PassEventCleaning(*evtInfo)) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("EvtCleaning (Data)", p_weight);

    if (m_tdt->isPassed("HLT_mu60_0eta105_msonly")) trig_passed = true;
    if (m_tdt->isPassed("HLT_g140_loose")) trig_passed = true;
    if (m_tdt->isPassed("HLT_2g50_loose")) trig_passed = true;

    // trigger check
    //if(!trig_passed) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("Trig", p_weight);

    // cosmic veto
    if(!m_cos->PassCosmicEventVeto(*elc, *muc)) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("CosmicVeto", p_weight);

    // get primary vertex
    auto pv = m_evtc->GetPV(*pvc);

    // PV position cut
    float pv_z_max = 200.;

    // apply primary vertex position cut
    if (pv) {

        // get primary vertex position
        auto pv_pos = pv->position();

        // z_pv cut
        if(std::abs(pv_pos.z()) > pv_z_max) return StatusCode::SUCCESS;
    }
    if ((m_usePV) && (!pv)) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("z_{PV} < 200 mm", p_weight);



    //=======================================================
    // Ks cutflow
    //=======================================================
    if (m_useSV) {

        // retrieve secondary vertices
        const xAOD::VertexContainer* dvc = nullptr;
        CHECK( evtStore()->retrieve( dvc, "VrtSecInclusive_SecondaryVertices" ));

        // make a copy of vertex containers
        auto dvc_copy = xAOD::shallowCopyContainer(*dvc);

        for(auto dv: *dvc_copy.first) {

            // select only vertex with tracks
            if(dv->trackParticleLinks().size() != 2) continue;


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
            float dv_eta = std::abs(m_dvutils->getEta( *dv, *pv )); // r in [mm]

            // access invariant mass, pt, eta
            float dv_mass = std::abs(m_accMass(*dv)); // in MeV
            float dv_pt = std::abs(m_acc_pt(*dv)) / 1000.; // in GeV

            //// collect leptons from this dv
            ////auto dv_muc = m_accMu(*dv);
            ////auto dv_elc = m_accEl(*dv);

            if (true) {

                float track_pt_min = 400;   // MeV
                float mass_min = 350;   // MeV
                float mass_max = 650;   // MeV
                float dv_R_max = 300;   // mm
                float dv_z_max = 300;   // mm

                // idid pair
                m_dv_idid_cf->Fill("xx", p_weight);

                // vertex fit quality
                if(!m_dvc->PassChisqCut(*dv)) continue;
                m_dv_idid_cf->Fill("#chi^{2}_{DV} / DOF < 5", p_weight);

                // minimum distance from pv (from 0 for MC)
                if(!m_dvc->PassDistCut(*dv, *pvc)) continue;
                m_dv_idid_cf->Fill("Disp. > 2 mm", p_weight);

                // charge requirements
                if(!m_dvc->PassChargeRequirement(*dv)) continue;
                m_dv_idid_cf->Fill("OppoCharge", p_weight);

                // disabled module
                if(!m_dvc->PassDisabledModuleVeto(*dv)) continue;
                m_dv_idid_cf->Fill("DisabledModule", p_weight);

                // material veto (only e)
                if(!m_dvc->PassMaterialVeto(*dv)) continue;
                m_dv_idid_cf->Fill("MaterialVeto", p_weight);

                // mass window cut
                if(dv_mass < mass_min) continue;
                m_dv_idid_cf->Fill("m > 350 MeV", p_weight);

                // mass window cut
                if(dv_mass > mass_max) continue;
                m_dv_idid_cf->Fill("m < 650 MeV", p_weight);

                // DV R <  300 mm
                if(dv_R > dv_R_max) continue;
                m_dv_idid_cf->Fill("R_{DV} > 300 mm", p_weight);

                // DV z <  300 mm
                if(std::abs(dv_z) > dv_z_max) continue;
                m_dv_idid_cf->Fill("z_{DV} > 300 mm", p_weight);

                // Ks candidate selection
                if(!PassKsSelection(*dv, *pv)) continue;
                m_dv_idid_cf->Fill("K_{s} candidate", p_weight);

                //-----------------------------------
                // fill histogram
                //-----------------------------------

                // fill vertex parameters
                m_dv_idid_M->Fill(dv_mass,p_weight);
                m_dv_idid_R->Fill(dv_R,p_weight);
                m_dv_idid_z->Fill(dv_z,p_weight);
                m_dv_idid_l->Fill(dv_l,p_weight);
                m_dv_idid_pt->Fill(dv_pt,p_weight);
                m_dv_idid_eta->Fill(dv_eta,p_weight);
                m_dv_idid_mu->Fill(pileup,p_weight);
                m_dv_idid_DeltaR->Fill(deltaR,p_weight);
                m_dv_idid_R_M->Fill(dv_R, dv_mass,p_weight);

                // fill track parameter
                m_dv_idid_track_pt->Fill(tp1.pt() / 1000.,p_weight);
                m_dv_idid_track_pt->Fill(tp2.pt() / 1000.,p_weight);

                // if both tracks are reconstructed by LRT
                if ((m_dvutils->isLargeD0Track(&tp1)) && (m_dvutils->isLargeD0Track(&tp2))) { 

                    // count lrt vertex
                    n_lrt++;

                    // fill LRT vertex
                    m_dv_LRT_M->Fill(dv_mass, p_weight);

                    // fill mass plot in bins of R
                    if (dv_R < 100) m_dv_LRT_M_R1->Fill(dv_mass,p_weight);
                    else if ((dv_R >= 100) and (dv_R < 150)) m_dv_LRT_M_R2->Fill(dv_mass,p_weight);
                    else if ((dv_R >= 150) and (dv_R < 200)) m_dv_LRT_M_R3->Fill(dv_mass,p_weight);
                    else if ((dv_R >= 200) and (dv_R < 250)) m_dv_LRT_M_R4->Fill(dv_mass,p_weight);
                    else if (dv_R >= 250) m_dv_LRT_M_R5->Fill(dv_mass,p_weight);

                    // fill mass plot in bins of eta
                    if (dv_eta < 0.5) m_dv_LRT_M_eta1->Fill(dv_mass,p_weight);
                    else if ((dv_eta >= 0.5) and (dv_eta < 1.0)) m_dv_LRT_M_eta2->Fill(dv_mass,p_weight);
                    else if ((dv_eta >= 1.0) and (dv_eta < 1.5)) m_dv_LRT_M_eta3->Fill(dv_mass,p_weight);
                    else m_dv_LRT_M_eta4->Fill(dv_mass,p_weight);

                    // fill mass plot in bins of pt
                    if (dv_pt < 2.5) m_dv_LRT_M_pt1->Fill(dv_mass,p_weight);
                    else if ((dv_pt >= 2.5) and (dv_pt < 3.0)) m_dv_LRT_M_pt2->Fill(dv_mass,p_weight);
                    else if ((dv_pt >= 3.0) and (dv_pt < 4.0)) m_dv_LRT_M_pt3->Fill(dv_mass,p_weight);
                    else m_dv_LRT_M_pt4->Fill(dv_mass,p_weight);

                }

                // if both tracks are reconstructed by standard tracking
                else if (!(m_dvutils->isLargeD0Track(&tp1)) && !(m_dvutils->isLargeD0Track(&tp2))) { 

                    // count standard vertex
                    n_standard++;

                    // fill standard vertex
                    m_dv_ST_M->Fill(dv_mass, p_weight);

                    // fill mass plot in bins of R
                    if (dv_R < 100) m_dv_ST_M_R1->Fill(dv_mass,p_weight);
                    else if ((dv_R >= 100) and (dv_R < 150)) m_dv_ST_M_R2->Fill(dv_mass,p_weight);
                    else if ((dv_R >= 150) and (dv_R < 200)) m_dv_ST_M_R3->Fill(dv_mass,p_weight);
                    else if ((dv_R >= 200) and (dv_R < 250)) m_dv_ST_M_R4->Fill(dv_mass,p_weight);
                    else if (dv_R >= 250) m_dv_ST_M_R5->Fill(dv_mass,p_weight);

                    // fill mass plot in bins of eta
                    if (dv_eta < 0.5) m_dv_ST_M_eta1->Fill(dv_mass,p_weight);
                    else if ((dv_eta >= 0.5) and (dv_eta < 1.0)) m_dv_ST_M_eta2->Fill(dv_mass,p_weight);
                    else if ((dv_eta >= 1.0) and (dv_eta < 1.5)) m_dv_ST_M_eta3->Fill(dv_mass,p_weight);
                    else m_dv_ST_M_eta4->Fill(dv_mass,p_weight);

                    // fill mass plot in bins of pt
                    if (dv_pt < 2.5) m_dv_ST_M_pt1->Fill(dv_mass,p_weight);
                    else if ((dv_pt >= 2.5) and (dv_pt < 3.0)) m_dv_ST_M_pt2->Fill(dv_mass,p_weight);
                    else if ((dv_pt >= 3.0) and (dv_pt < 4.0)) m_dv_ST_M_pt3->Fill(dv_mass,p_weight);
                    else m_dv_ST_M_pt4->Fill(dv_mass,p_weight);
                }

                // if one track is reconstructed by ST and the other track is reconstructed by LRT
                else { 

                    // count vertex
                    n_lrt_standard++;

                    // fill standard vertex
                    m_dv_STLRT_M->Fill(dv_mass, p_weight);

                    // fill mass plot in bins of R
                    if (dv_R < 100) m_dv_STLRT_M_R1->Fill(dv_mass,p_weight);
                    else if ((dv_R >= 100) and (dv_R < 150)) m_dv_STLRT_M_R2->Fill(dv_mass,p_weight);
                    else if ((dv_R >= 150) and (dv_R < 200)) m_dv_STLRT_M_R3->Fill(dv_mass,p_weight);
                    else if ((dv_R >= 200) and (dv_R < 250)) m_dv_STLRT_M_R4->Fill(dv_mass,p_weight);
                    else if (dv_R >= 250) m_dv_STLRT_M_R5->Fill(dv_mass,p_weight);

                    // fill mass plot in bins of eta
                    if (dv_eta < 0.5) m_dv_STLRT_M_eta1->Fill(dv_mass,p_weight);
                    else if ((dv_eta >= 0.5) and (dv_eta < 1.0)) m_dv_STLRT_M_eta2->Fill(dv_mass,p_weight);
                    else if ((dv_eta >= 1.0) and (dv_eta < 1.5)) m_dv_STLRT_M_eta3->Fill(dv_mass,p_weight);
                    else m_dv_STLRT_M_eta4->Fill(dv_mass,p_weight);

                    // fill mass plot in bins of pt
                    if (dv_pt < 2.5) m_dv_STLRT_M_pt1->Fill(dv_mass,p_weight);
                    else if ((dv_pt >= 2.5) and (dv_pt < 3.0)) m_dv_STLRT_M_pt2->Fill(dv_mass,p_weight);
                    else if ((dv_pt >= 3.0) and (dv_pt < 4.0)) m_dv_STLRT_M_pt3->Fill(dv_mass,p_weight);
                    else m_dv_STLRT_M_pt4->Fill(dv_mass,p_weight);
                }

                
            }

            //// make histograms from truth-matched Ks
            //if (isMC){

            //    // find closest truth vertex
            //    const xAOD::TruthVertex *tru_matched = nullptr;
            //    tru_matched = m_dvutils->getClosestTruthVertex(dv);

            //    // check if the matched truth vertex is from Ks
            //    if (tru_matched){
            //        const xAOD::TruthParticle* parent = tru_matched->incomingParticle(0);
            //        if (!(parent->absPdgId() ==310)) continue;

            //        m_Ks_r->Fill(dv_R,p_weight);
            //        m_Ks_z->Fill(dv_z,p_weight);
            //        m_Ks_pt->Fill(dv_pt,p_weight);
            //        m_Ks_eta->Fill(dv_eta,p_weight); 
            //        m_Ks_DeltaR->Fill(deltaR,p_weight); 

            //    }
            //}
        } // end of Ks loop
    }

    //=======================================================
    // Z' DV cutflow
    // Here, we make pt, eta, delta R, r of truth-matched Z'
    //=======================================================
    if (m_useSV) {

        // retrieve secondary vertices
        const xAOD::VertexContainer* dvc = nullptr;
        CHECK( evtStore()->retrieve( dvc, "VrtSecInclusive_SecondaryVertices" ));

        // make a copy of vertex containers
        auto dvc_copy = xAOD::shallowCopyContainer(*dvc);
       
        for(auto dv: *dvc_copy.first) {

            // mass cut
            float mass_min = 10.;

            // vertex position cut
            float dv_R_max = 300;
            float dv_z_max = 300;

            // select only vertex with tracks
            if(dv->trackParticleLinks().size() != 2) continue;

            // perform lepton matching
            m_dilepdvc->ApplyLeptonMatching(*dv, *elc_copy.first, *muc_copy.first);

            // remove overlapping muon
            m_dilepdvc->ApplyOverlapRemoval(*dv);

            // perform filter matching
            m_dilepdvc->DoFilterMatching(*dv);

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
            float dv_mass = std::abs(m_accMass(*dv)); // in MeV
            float dv_pt = std::abs(m_acc_pt(*dv)) / 1000.; // in GeV

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

                // RPVLL filter matching
                if(!m_dilepdvc->PassFilterMatching(*dv)) continue;

                // DV R <  300 mm
                if(dv_R > dv_R_max) continue;

                // DV z <  300 mm
                if(std::abs(dv_z) > dv_z_max) continue;

                // make histograms from truth-matched Ks
                if(isMC){
                    // find closest truth vertex
                    const xAOD::TruthVertex *tru_matched = nullptr;
                    tru_matched = m_dvutils->getClosestTruthVertex(dv);

                    // check if the matched truth vertex is from Z'
                    if (tru_matched){
                        const xAOD::TruthParticle* parent = tru_matched->incomingParticle(0);
                        if (!(parent->absPdgId() ==32)) continue;

                        m_zp_r->Fill(dv_R,p_weight);
                        m_zp_z->Fill(dv_z,p_weight);
                        m_zp_pt->Fill(dv_pt,p_weight);
                        m_zp_eta->Fill(dv_eta,p_weight); 
                        m_zp_DeltaR->Fill(deltaR,p_weight); 

                    }

                }
            }
        } // end of Z' DV loop
    }




    return StatusCode::SUCCESS;
}

StatusCode TrackingSystematics::beginInputFile() { 
    return StatusCode::SUCCESS;
}

bool TrackingSystematics::PassCosmicVeto_R_CR(xAOD::TrackParticle& tr0, xAOD::TrackParticle& tr1){

    bool PassCosmicVeto = true;
    float Rcos_min = 0.01;

    // define TLorentzVector of decay particles
    TLorentzVector tlv_tp0;
    TLorentzVector tlv_tp1;
    tlv_tp0 = tr0.p4();
    tlv_tp1 = tr1.p4();

    float deltaPhiMinusPi = std::abs(std::abs(tlv_tp0.DeltaPhi(tlv_tp1)) - std::acos(-1.));
    float sumEta = tlv_tp0.Eta() + tlv_tp1.Eta();
    float Rcos = std::sqrt(sumEta * sumEta + deltaPhiMinusPi * deltaPhiMinusPi);

    ATH_MSG_DEBUG("Rcos = " << Rcos << ", tlv_tp0.eta = " << tlv_tp0.Eta() << ", tlv_tp1.eta = " << tlv_tp1.Eta());

    if (Rcos < Rcos_min) PassCosmicVeto = false;

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
