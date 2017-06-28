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
#include "xAODTruth/TruthVertex.h"
#include "xAODTruth/TruthVertexContainer.h"

// tools
#include "PathResolver/PathResolver.h"

// for M_PI
#include "cmath"

// debug
#include <typeinfo>

DisplacedDimuonAnalysisAlg::DisplacedDimuonAnalysisAlg( const std::string& name, ISvcLocator* pSvcLocator ) :
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
    declareProperty("TrigMatchingTool", m_tmt);
    declareProperty("DiLepCosmics", m_cos);
    declareProperty("OverlapRemoval", m_or);


}


DisplacedDimuonAnalysisAlg::~DisplacedDimuonAnalysisAlg() {}


StatusCode DisplacedDimuonAnalysisAlg::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");

    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    // event cut flow
    m_event_cutflow = new TH1D( "m_event_cutflow", "Event cutflow", 4,0,4);
    CHECK( histSvc->regHist("/DV/event_cutflow", m_event_cutflow) );

    //--------------------------------------------------------
    // mumu
    //--------------------------------------------------------

    Float_t m_dv_mumu_M_bins[] = {0,10,40,70,100,400,700,1000,2000};
    m_dv_mumu_cf = new TH1D( "m_dv_mumu_cf", "Reco dv mumu cutflow", 11,0,11);
    m_dv_mumu_M = new TH1F("dv_mumu_M","Dimuon DV mass in GeV",8, m_dv_mumu_M_bins );
    m_dv_mumu_R = new TH1F("dv_mumu_R","R of dimuon dv [mm]",50,0.,300.);
    m_dv_mumu_R_low = new TH1F("dv_mumu_R_low","R of dimuon dv [mm], low",50,0.,50.);
    m_dv_mumu_z = new TH1F("dv_mumu_z","z of dimuon dv [mm]",100,-1000.,1000.);
    m_dv_mumu_l = new TH1F("dv_mumu_l","l of dimuon dv [mm]",100,0.,1000.);
    m_dv_mumu_R_M = new TH2F("dv_mumu_R_M","Dimuon DV position R vs M", 50,0,300,200,0,2000);

    // muon kinematics distribution
    m_dv_mumu_mu_pt = new TH1F("dv_mumu_mu_pt","Signal muon pT",50,0.,1000.);
    m_dv_mumu_mu_pt_low = new TH1F("dv_mumu_mu_pt_low","Signal muon low pT",50,0.,100.);
    m_dv_mumu_mu_eta = new TH1F("dv_mumu_mu_eta","Signal muon eta",50,-3.0,3.0);
    m_dv_mumu_mu_phi = new TH1F("dv_mumu_mu_phi","Signal muon phi",50,-M_PI,M_PI);
    m_dv_mumu_mu_d0 = new TH1F("dv_mumu_mu_d0","Signal muon d0",50,-300,300);
    m_dv_mumu_mu_z0 = new TH1F("dv_mumu_mu_z0","Signal muon z0",50,-1000,1000);
    //m_dv_mumu_mu_pt_min = new TH1F("dv_mumu_mu_pt_min","Signal muon pt_min",50,0.,1000.);
    //m_dv_mumu_mu_pt_min_low = new TH1F("dv_mumu_mu_pt_min_low","Signal muon low pt_min",50,0.,100.);
    //m_dv_mumu_mu_pt_max = new TH1F("dv_mumu_mu_pt_max","Signal muon pt_max",50,0.,1000.);
    //m_dv_mumu_mu_pt_max_low = new TH1F("dv_mumu_mu_pt_max_low","Signal muon low pt_max",50,0.,100.);

    // cosmic veto
    m_dv_mumu_DeltaR = new TH1F("dv_mumu_DeltaR","Signal muon Delta R",100, 0., 5.);
    m_dv_mumu_DeltaR_low = new TH1F("dv_mumu_DeltaR_low","Signal muon Delta R low",100, 0., 1);
    m_dv_mumu_Rcos = new TH1F("dv_mumu_Rcos","Signal muon Rcos",50, 0., 5.);
    m_dv_mumu_Rcos_low = new TH1F("dv_mumu_Rcos_low","Signal muon Rcos low",25, 0., 0.1);

    // only for MC
    m_dv_mumu_M_matched = new TH1F("dv_mumu_M_matched","matched dimuon DV mass in GeV",200,0.,2000.);
    m_dv_mumu_R_matched = new TH1F("dv_mumu_R_matched","R of matched dimuon dv [mm]",50,0.,300.);
    m_dv_mumu_R_M_matched = new TH2F("dv_mumu_R_M_matched","matched dimuon DV position R vs M", 50,0,300,200,0,2000);
    m_dv_mumu_chi2_ndof = new TH1F("dv_mumu_chi2_ndof", "chi^2 / ndof (mumu)", 100, 0, 10);
    m_dv_mumu_eta_matched = new TH1F("dv_mumu_eta_matched","eta of dimuon dv",40,-4.,4.);

    // registor for output
    CHECK( histSvc->regHist("/DV/dv_mumu/dv_mumu_cf", m_dv_mumu_cf) );
    CHECK( histSvc->regHist("/DV/dv_mumu/dv_mumu_M", m_dv_mumu_M) );
    CHECK( histSvc->regHist("/DV/dv_mumu/dv_mumu_R", m_dv_mumu_R) );
    CHECK( histSvc->regHist("/DV/dv_mumu/dv_mumu_R_low", m_dv_mumu_R_low) );
    CHECK( histSvc->regHist("/DV/dv_mumu/dv_mumu_z", m_dv_mumu_z) );
    CHECK( histSvc->regHist("/DV/dv_mumu/dv_mumu_l", m_dv_mumu_l) );
    CHECK( histSvc->regHist("/DV/dv_mumu/dv_mumu_R_M", m_dv_mumu_R_M) );
    CHECK( histSvc->regHist("/DV/dv_mumu/dv_mumu_chi2_ndof", m_dv_mumu_chi2_ndof) );

    // muon kinematics distribution
    CHECK( histSvc->regHist("/DV/dv_mumu/outgoing/dv_mumu_mu_pt", m_dv_mumu_mu_pt) );
    CHECK( histSvc->regHist("/DV/dv_mumu/outgoing/dv_mumu_mu_pt_low", m_dv_mumu_mu_pt_low) );
    //CHECK( histSvc->regHist("/DV/dv_mumu/outgoing/dv_mumu_mu_pt_min", m_dv_mumu_mu_pt_min) );
    //CHECK( histSvc->regHist("/DV/dv_mumu/outgoing/dv_mumu_mu_pt_min_low", m_dv_mumu_mu_pt_min_low) );
    //CHECK( histSvc->regHist("/DV/dv_mumu/outgoing/dv_mumu_mu_pt_max", m_dv_mumu_mu_pt_max) );
    //CHECK( histSvc->regHist("/DV/dv_mumu/outgoing/dv_mumu_mu_pt_max_low", m_dv_mumu_mu_pt_max_low) );
    CHECK( histSvc->regHist("/DV/dv_mumu/outgoing/dv_mumu_mu_eta", m_dv_mumu_mu_eta) );
    CHECK( histSvc->regHist("/DV/dv_mumu/outgoing/dv_mumu_mu_phi", m_dv_mumu_mu_phi) );
    CHECK( histSvc->regHist("/DV/dv_mumu/outgoing/dv_mumu_mu_d0", m_dv_mumu_mu_d0) );
    CHECK( histSvc->regHist("/DV/dv_mumu/outgoing/dv_mumu_mu_z0", m_dv_mumu_mu_z0) );

    // cosmic veto
    CHECK( histSvc->regHist("/DV/dv_mumu/cv/dv_mumu_DeltaR", m_dv_mumu_DeltaR) );
    CHECK( histSvc->regHist("/DV/dv_mumu/cv/dv_mumu_DeltaR_low", m_dv_mumu_DeltaR_low) );
    CHECK( histSvc->regHist("/DV/dv_mumu/cv/dv_mumu_Rcos", m_dv_mumu_Rcos) );
    CHECK( histSvc->regHist("/DV/dv_mumu/cv/dv_mumu_Rcos_low", m_dv_mumu_Rcos_low) );

    // only for MC
    CHECK( histSvc->regHist("/DV/dv_mumu/truth-matched/reco_dv_mumu_M", m_dv_mumu_M_matched) );
    CHECK( histSvc->regHist("/DV/dv_mumu/truth-matched/reco_dv_mumu_R", m_dv_mumu_R_matched) );
    CHECK( histSvc->regHist("/DV/dv_mumu/truth-matched/reco_dv_mumu_R_M", m_dv_mumu_R_M_matched) );
    CHECK( histSvc->regHist("/DV/dv_mumu/truth-matched/truth_dv_zp_eta", m_dv_mumu_eta_matched) );

    //--------------------------------------------------------
    // ee
    //--------------------------------------------------------

    Float_t m_dv_ee_M_bins[] = {0,10,40,70,100,400,700,1000,2000};
    m_dv_ee_cf = new TH1D( "m_dv_ee_cf", "Reco dv ee cutflow", 11,0,11);
    m_dv_ee_M = new TH1F("dv_ee_M","ee DV mass in GeV",8, m_dv_ee_M_bins );
    m_dv_ee_R = new TH1F("dv_ee_R","R of ee dv [mm]",50,0.,300.);
    m_dv_ee_R_low = new TH1F("dv_ee_R_low","R of ee dv [mm], low",50,0.,50.);
    m_dv_ee_z = new TH1F("dv_ee_z","z of ee dv [mm]",100,-1000.,1000.);
    m_dv_ee_l = new TH1F("dv_ee_l","l of ee dv [mm]",100,0.,1000.);
    m_dv_ee_R_M = new TH2F("dv_ee_R_M","ee DV position R vs M", 50,0,300,200,0,2000);

    // muon kinematics distribution
    m_dv_ee_e_pt = new TH1F("dv_ee_e_pt","Signal electron pT",50,0.,1000.);
    m_dv_ee_e_pt_low = new TH1F("dv_ee_e_pt_low","Signal electron low pT",50,0.,100.);
    m_dv_ee_e_eta = new TH1F("dv_ee_e_eta","Signal electron eta",50,-3.0,3.0);
    m_dv_ee_e_phi = new TH1F("dv_ee_e_phi","Signal electron phi",50,-M_PI,M_PI);
    m_dv_ee_e_d0 = new TH1F("dv_ee_e_d0","Signal electron d0",50,-300,300);
    m_dv_ee_e_z0 = new TH1F("dv_ee_e_z0","Signal electron z0",50,-1000,1000);
    //m_dv_ee_e_pt_min = new TH1F("dv_ee_e_pt_min","Signal electron pt_min",50,0.,1000.);
    //m_dv_ee_e_pt_min_low = new TH1F("dv_ee_e_pt_min_low","Signal electron low pt_min",50,0.,100.);
    //m_dv_ee_e_pt_max = new TH1F("dv_ee_e_pt_max","Signal electron pt_max",50,0.,1000.);
    //m_dv_ee_e_pt_max_low = new TH1F("dv_ee_e_pt_max_low","Signal electron low pt_max",50,0.,100.);

    // cosmic veto
    m_dv_ee_DeltaR = new TH1F("dv_ee_DeltaR","Signal ee Delta R",100, 0., 5.);
    m_dv_ee_DeltaR_low = new TH1F("dv_ee_DeltaR_low","Signal ee Delta R low",100, 0., 1);
    m_dv_ee_Rcos = new TH1F("dv_ee_Rcos","Signal ee Rcos",50, 0., 5.);
    m_dv_ee_Rcos_low = new TH1F("dv_ee_Rcos_low","Signal ee Rcos low",25, 0., 0.1);

    // only for MC
    m_dv_ee_M_matched = new TH1F("dv_ee_M_matched","matched ee DV mass in GeV",200,0.,2000.);
    m_dv_ee_R_matched = new TH1F("dv_ee_R_matched","R of matched ee dv [mm]",50,0.,300.);
    m_dv_ee_R_M_matched = new TH2F("dv_ee_R_M_matched","matched ee DV position R vs M", 50,0,300,200,0,2000);
    m_dv_ee_eta_matched = new TH1F("dv_ee_eta_matched","eta of ee dv",100,-4.,4.);
    m_dv_ee_chi2_ndof = new TH1F("dv_ee_chi2_ndof", "chi^2 / ndof (ee)", 100, 0, 10);

    // registor for output
    CHECK( histSvc->regHist("/DV/dv_ee/dv_ee_cf", m_dv_ee_cf) );
    CHECK( histSvc->regHist("/DV/dv_ee/dv_ee_M", m_dv_ee_M) );
    CHECK( histSvc->regHist("/DV/dv_ee/dv_ee_R", m_dv_ee_R) );
    CHECK( histSvc->regHist("/DV/dv_ee/dv_ee_R_low", m_dv_ee_R_low) );
    CHECK( histSvc->regHist("/DV/dv_ee/dv_ee_z", m_dv_ee_z) );
    CHECK( histSvc->regHist("/DV/dv_ee/dv_ee_l", m_dv_ee_l) );
    CHECK( histSvc->regHist("/DV/dv_ee/dv_ee_R_M", m_dv_ee_R_M) );
    CHECK( histSvc->regHist("/DV/dv_ee/dv_ee_chi2_ndof", m_dv_ee_chi2_ndof) );

    // electron kinematics distribution
    CHECK( histSvc->regHist("/DV/dv_ee/outgoing/dv_ee_e_pt", m_dv_ee_e_pt) );
    CHECK( histSvc->regHist("/DV/dv_ee/outgoing/dv_ee_e_pt_low", m_dv_ee_e_pt_low) );
    //CHECK( histSvc->regHist("/DV/dv_ee/outgoing/dv_ee_e_pt_min", m_dv_ee_e_pt_min) );
    //CHECK( histSvc->regHist("/DV/dv_ee/outgoing/dv_ee_e_pt_min_low", m_dv_ee_e_pt_min_low) );
    //CHECK( histSvc->regHist("/DV/dv_ee/outgoing/dv_ee_e_pt_max", m_dv_ee_e_pt_max) );
    //CHECK( histSvc->regHist("/DV/dv_ee/outgoing/dv_ee_e_pt_max_low", m_dv_ee_e_pt_max_low) );
    CHECK( histSvc->regHist("/DV/dv_ee/outgoing/dv_ee_e_eta", m_dv_ee_e_eta) );
    CHECK( histSvc->regHist("/DV/dv_ee/outgoing/dv_ee_e_phi", m_dv_ee_e_phi) );
    CHECK( histSvc->regHist("/DV/dv_ee/outgoing/dv_ee_e_d0", m_dv_ee_e_d0) );
    CHECK( histSvc->regHist("/DV/dv_ee/outgoing/dv_ee_e_z0", m_dv_ee_e_z0) );

    // cosmic veto
    CHECK( histSvc->regHist("/DV/dv_ee/cv/dv_ee_DeltaR", m_dv_ee_DeltaR) );
    CHECK( histSvc->regHist("/DV/dv_ee/cv/dv_ee_DeltaR_low", m_dv_ee_DeltaR_low) );
    CHECK( histSvc->regHist("/DV/dv_ee/cv/dv_ee_Rcos", m_dv_ee_Rcos) );
    CHECK( histSvc->regHist("/DV/dv_ee/cv/dv_ee_Rcos_low", m_dv_ee_Rcos_low) );

    // only for MC
    CHECK( histSvc->regHist("/DV/dv_ee/truth-matched/reco_dv_ee_M", m_dv_ee_M_matched) );
    CHECK( histSvc->regHist("/DV/dv_ee/truth-matched/reco_dv_ee_R", m_dv_ee_R_matched) );
    CHECK( histSvc->regHist("/DV/dv_ee/truth-matched/reco_dv_ee_R_M", m_dv_ee_R_M_matched) );
    CHECK( histSvc->regHist("/DV/dv_ee/truth-matched/truth_dv_zp_eta", m_dv_ee_eta_matched) );

    //--------------------------------------------------------
    // emu
    //--------------------------------------------------------

    Float_t m_dv_emu_M_bins[] = {0,10,40,70,100,400,700,1000,2000};
    m_dv_emu_cf = new TH1D( "m_dv_emu_cf", "Reco dv emu cutflow", 11,0,11);
    m_dv_emu_M = new TH1F("dv_emu_M","emu DV mass in GeV",8, m_dv_emu_M_bins );
    m_dv_emu_R = new TH1F("dv_emu_R","R of emu dv [mm]",50,0.,300.);
    m_dv_emu_R_low = new TH1F("dv_emu_R_low","R of emu dv [mm], low",50,0.,50.);
    m_dv_emu_z = new TH1F("dv_emu_z","z of emu dv [mm]",100,-1000.,1000.);
    m_dv_emu_l = new TH1F("dv_emu_l","l of emu dv [mm]",100,0.,1000.);
    m_dv_emu_R_M = new TH2F("dv_emu_R_M","emu DV position R vs M", 50,0,300,200,0,2000);

    // muon kinematics distribution
    m_dv_emu_e_pt = new TH1F("dv_emu_e_pt","Signal electron pT",50,0.,1000.);
    m_dv_emu_e_pt_low = new TH1F("dv_emu_e_pt_low","Signal electron low pT",50,0.,100.);
    m_dv_emu_e_eta = new TH1F("dv_emu_e_eta","Signal electron eta",50,-3.0,3.0);
    m_dv_emu_e_phi = new TH1F("dv_emu_e_phi","Signal electron phi",50,-M_PI,M_PI);
    m_dv_emu_e_d0 = new TH1F("dv_emu_e_d0","Signal electron d0",50,-300,300);
    m_dv_emu_e_z0 = new TH1F("dv_emu_e_z0","Signal electron z0",50,-1000,1000);
    //m_dv_emu_e_pt_min = new TH1F("dv_emu_e_pt_min","Signal electron pt_min",50,0.,1000.);
    //m_dv_emu_e_pt_min_low = new TH1F("dv_emu_e_pt_min_low","Signal electron low pt_min",50,0.,100.);
    //m_dv_emu_e_pt_max = new TH1F("dv_emu_e_pt_max","Signal electron pt_max",50,0.,1000.);
    //m_dv_emu_e_pt_max_low = new TH1F("dv_emu_e_pt_max_low","Signal electron low pt_max",50,0.,100.);

    m_dv_emu_mu_pt = new TH1F("dv_emu_mu_pt","Signal electron pT",50,0.,1000.);
    m_dv_emu_mu_pt_low = new TH1F("dv_emu_mu_pt_low","Signal electron low pT",50,0.,100.);
    m_dv_emu_mu_eta = new TH1F("dv_emu_mu_eta","Signal electron eta",50,-3.0,3.0);
    m_dv_emu_mu_phi = new TH1F("dv_emu_mu_phi","Signal electron phi",50,-M_PI,M_PI);
    m_dv_emu_mu_d0 = new TH1F("dv_emu_mu_d0","Signal electron d0",50,-300,300);
    m_dv_emu_mu_z0 = new TH1F("dv_emu_mu_z0","Signal electron z0",50,-1000,1000);
    //m_dv_emu_mu_pt_min = new TH1F("dv_emu_mu_pt_min","Signal electron pt_min",50,0.,1000.);
    //m_dv_emu_mu_pt_min_low = new TH1F("dv_emu_mu_pt_min_low","Signal electron low pt_min",50,0.,100.);
    //m_dv_emu_mu_pt_max = new TH1F("dv_emu_mu_pt_max","Signal electron pt_max",50,0.,1000.);
    //m_dv_emu_mu_pt_max_low = new TH1F("dv_emu_mu_pt_max_low","Signal electron low pt_max",50,0.,100.);

    // cosmic veto
    m_dv_emu_DeltaR = new TH1F("dv_emu_DeltaR","Signal emu Delta R",100, 0., 5.);
    m_dv_emu_DeltaR_low = new TH1F("dv_emu_DeltaR_low","Signal emu Delta R low",100, 0., 1);
    m_dv_emu_Rcos = new TH1F("dv_emu_Rcos","Signal emu Rcos",50, 0., 5.);
    m_dv_emu_Rcos_low = new TH1F("dv_emu_Rcos_low","Signal emu Rcos low",25, 0., 0.1);

    // only for MC
    m_dv_emu_M_matched = new TH1F("dv_emu_M_matched","matched emu DV mass in GeV",200,0.,2000.);
    m_dv_emu_R_matched = new TH1F("dv_emu_R_matched","R of matched emu dv [mm]",50,0.,300.);
    m_dv_emu_R_M_matched = new TH2F("dv_emu_R_M_matched","matched emu DV position R vs M", 50,0,300,200,0,2000);
    m_dv_emu_eta_matched = new TH1F("dv_emu_eta_matched","eta of emu dv",100,-4.,4.);
    m_dv_emu_chi2_ndof = new TH1F("dv_emu_chi2_ndof", "chi^2 / ndof (emu)", 100, 0, 10);

    // registor for output
    CHECK( histSvc->regHist("/DV/dv_emu/dv_emu_cf", m_dv_emu_cf) );
    CHECK( histSvc->regHist("/DV/dv_emu/dv_emu_M", m_dv_emu_M) );
    CHECK( histSvc->regHist("/DV/dv_emu/dv_emu_R", m_dv_emu_R) );
    CHECK( histSvc->regHist("/DV/dv_emu/dv_emu_R_low", m_dv_emu_R_low) );
    CHECK( histSvc->regHist("/DV/dv_emu/dv_emu_z", m_dv_emu_z) );
    CHECK( histSvc->regHist("/DV/dv_emu/dv_emu_l", m_dv_emu_l) );
    CHECK( histSvc->regHist("/DV/dv_emu/dv_emu_R_M", m_dv_emu_R_M) );
    CHECK( histSvc->regHist("/DV/dv_emu/dv_emu_chi2_ndof", m_dv_emu_chi2_ndof) );

    // electron kinematics distribution
    CHECK( histSvc->regHist("/DV/dv_emu/outgoing/dv_emu_e_pt", m_dv_emu_e_pt) );
    CHECK( histSvc->regHist("/DV/dv_emu/outgoing/dv_emu_e_pt_low", m_dv_emu_e_pt_low) );
    CHECK( histSvc->regHist("/DV/dv_emu/outgoing/dv_emu_e_eta", m_dv_emu_e_eta) );
    CHECK( histSvc->regHist("/DV/dv_emu/outgoing/dv_emu_e_phi", m_dv_emu_e_phi) );
    CHECK( histSvc->regHist("/DV/dv_emu/outgoing/dv_emu_e_d0", m_dv_emu_e_d0) );
    CHECK( histSvc->regHist("/DV/dv_emu/outgoing/dv_emu_e_z0", m_dv_emu_e_z0) );

    CHECK( histSvc->regHist("/DV/dv_emu/outgoing/dv_emu_mu_pt", m_dv_emu_mu_pt) );
    CHECK( histSvc->regHist("/DV/dv_emu/outgoing/dv_emu_mu_pt_low", m_dv_emu_mu_pt_low) );
    CHECK( histSvc->regHist("/DV/dv_emu/outgoing/dv_emu_mu_eta", m_dv_emu_mu_eta) );
    CHECK( histSvc->regHist("/DV/dv_emu/outgoing/dv_emu_mu_phi", m_dv_emu_mu_phi) );
    CHECK( histSvc->regHist("/DV/dv_emu/outgoing/dv_emu_mu_d0", m_dv_emu_mu_d0) );
    CHECK( histSvc->regHist("/DV/dv_emu/outgoing/dv_emu_mu_z0", m_dv_emu_mu_z0) );

    // cosmic veto
    CHECK( histSvc->regHist("/DV/dv_emu/cv/dv_emu_DeltaR", m_dv_emu_DeltaR) );
    CHECK( histSvc->regHist("/DV/dv_emu/cv/dv_emu_DeltaR_low", m_dv_emu_DeltaR_low) );
    CHECK( histSvc->regHist("/DV/dv_emu/cv/dv_emu_Rcos", m_dv_emu_Rcos) );
    CHECK( histSvc->regHist("/DV/dv_emu/cv/dv_emu_Rcos_low", m_dv_emu_Rcos_low) );

    // only for MC
    CHECK( histSvc->regHist("/DV/dv_emu/truth-matched/reco_dv_emu_M", m_dv_emu_M_matched) );
    CHECK( histSvc->regHist("/DV/dv_emu/truth-matched/reco_dv_emu_R", m_dv_emu_R_matched) );
    CHECK( histSvc->regHist("/DV/dv_emu/truth-matched/reco_dv_emu_R_M", m_dv_emu_R_M_matched) );
    CHECK( histSvc->regHist("/DV/dv_emu/truth-matched/truth_dv_zp_eta", m_dv_emu_eta_matched) );

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
    //if(!m_evtc->PassTrigger()) return StatusCode::SUCCESS;
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

    // perform lepton matching
    for(auto dv: *dvc_copy.first) {
        m_dilepdvc->ApplyLeptonMatching(*dv, *elc_copy.first, *muc_copy.first);
    }

    //------------------------------
    // dv cut flow
    //------------------------------
    for(auto dv: *dvc_copy.first) {

        // access invariant mass
        float dv_mass = std::fabs(m_accMass(*dv)) / 1000.; // in MeV

        // collect leptons from this dv
        auto dv_muc = m_accMu(*dv);
        auto dv_elc = m_accEl(*dv);

        // remove overlapping muon
        m_dilepdvc->ApplyOverlapRemoval(*dv);

        // muon selection tool
        //m_leptool->MuonSelection(*dv);

        // old method: only require combined muon
        if(!m_dvutils->IsCombinedMuon(*dv_muc)) continue;

        // remove bad electrons
        m_leptool->BadClusterRemoval(*dv);

        // kinematic cut
        m_leptool->ElectronKinematicCut(*dv);

        // Electron identification
        m_leptool->ElectronID(*dv);

        // counting all dv's
        //m_dv_mumu_cf->Fill("DV", 1);
        //m_dv_ee_cf->Fill("DV", 1);
        //m_dv_emu_cf->Fill("DV", 1);

        // find decay channel of dv
        std::string channel = m_dvutils->DecayChannel(*dv);

        if (channel == "mumu") {

            // mumu pair
            m_dv_mumu_cf->Fill("#mu#mu", 1);

            // Trigger matching
            if(!m_dvutils->TrigMatching(*dv)) break;
            m_dv_mumu_cf->Fill("Trig. Matching", 1);

            // vertex fit quality
            m_dv_mumu_chi2_ndof->Fill (dv->chiSquared() / dv->numberDoF() );
            if(!m_dvc->PassChisqCut(*dv)) break;
            m_dv_mumu_cf->Fill("#chi^{2}_{DV} / DOF < 5", 1);

            // minimum distance from pv (from 0 for MC)
            if(!m_dvc->PassDistCut(*dv, *pvc)) break;
            m_dv_mumu_cf->Fill("Disp. > 2 mm", 1);

            // charge requirements
            if(!m_dvc->PassChargeRequirement(*dv)) break;
            m_dv_mumu_cf->Fill("#mu^{+}#mu^{-}", 1);

            // disabled module
            if(!m_dvc->PassDisabledModuleVeto(*dv)) break;
            m_dv_mumu_cf->Fill("DisabledModule", 1);

            // material veto (only e)
            m_dv_mumu_cf->Fill("MaterialVeto (Only e)", 1);

            // DESD filter
            //m_dilepdvc->DoTriggerMatching(*dv);
            //if(!m_dilepdvc->PassDESDMatching(*dv)) break;
            //m_dv_mumu_cf->Fill("PassDESDMatching", 1);

            // cosmic veto
            if(!PassCosmicVeto(*dv_muc, *dv_elc, channel)) break;
            m_dv_mumu_cf->Fill("R_{CR} > 0.04", 1);

            // delta R 
            float deltaR_min = 0.5;
            if(m_dvutils->getDeltaR(*dv_muc) < deltaR_min) break;
            m_dv_mumu_cf->Fill("#DeltaR > 0.5", 1);
    
            // end of cut flow. Now plotting
            ATH_MSG_INFO("Found mumu with mass = " << dv_mass << ", runNumber = "
            << evtInfo->runNumber() << ", eventNumber = "
            << evtInfo->eventNumber() << ", Lumiblock = " << evtInfo->lumiBlock() );

            // plot dv distributions
            plot_dv(*dv, *pv, channel);

            // plot muon kinematics
            plot_signal_tp(*dv_muc, *dv_elc, channel);

            // only for MC: find truth dv matched to this dv
            if (isMC) {
                // find truth dv matched to this dv
                const xAOD::TruthVertex* tru_v_loose = m_dvutils->IsSignalDV_loose(*dv_muc, *dv_elc, channel, *dv);
                if (tru_v_loose != nullptr) {
                    m_dv_mumu_cf->Fill("Matched (Loose)", 1);
                }

                const xAOD::TruthVertex* tru_v = m_dvutils->IsSignalDV(*dv_muc, *dv_elc, channel);
                if (tru_v != nullptr) {
                    m_dv_mumu_cf->Fill("Matched (Tight)", 1);

                    // fill matched dimuon vertex
                    float dv_R = m_dvutils->getR( *dv, *pv );                 // R in [mm]
                    m_dv_mumu_M_matched->Fill(dv_mass);                          // dimuon mass
                    m_dv_mumu_R_matched->Fill(dv_R);                                // R in [mm]
                    m_dv_mumu_R_M_matched->Fill(dv_R, dv_mass);
                    m_dv_mumu_eta_matched->Fill(tru_v->incomingParticle(0)->eta());
                }

            } // end of isMC
        } // end of mumu

        if (channel == "ee") {

            // ee pair
            m_dv_ee_cf->Fill("ee", 1);

            // Trigger matching
            //m_dilepdvc->DoTriggerMatching(*dv);
            //if(!m_dilepdvc->PassTriggerMatching(*dv)) break;
            if(!m_dvutils->TrigMatching(*dv)) break;
            m_dv_ee_cf->Fill("Trig. Matching", 1);

            // vertex fit quality
            m_dv_ee_chi2_ndof->Fill (dv->chiSquared() / dv->numberDoF() );
            if(!m_dvc->PassChisqCut(*dv)) break;
            m_dv_ee_cf->Fill("#chi^{2}_{DV} / DOF < 5", 1);

            // minimum distance from pv (from 0 for MC)
            if(!m_dvc->PassDistCut(*dv, *pvc)) break;
            m_dv_ee_cf->Fill("Disp. > 2 mm", 1);

            // charge requirements
            if(!m_dvc->PassChargeRequirement(*dv)) break;
            m_dv_ee_cf->Fill("e^{+}e^{-}", 1);

            // disabled module
            if(!m_dvc->PassDisabledModuleVeto(*dv)) break;
            m_dv_ee_cf->Fill("DisabledModule", 1);

            // material veto
            if(!m_dvc->PassMaterialVeto(*dv)) break;
            m_dv_ee_cf->Fill("MaterialVeto", 1);

            //// DESD filter
            //m_dilepdvc->DoTriggerMatching(*dv);
            //if(!m_dilepdvc->PassDESDMatching(*dv)) break;
            //m_dv_ee_cf->Fill("PassDESDMatching", 1);

            // cosmic veto
            if(!PassCosmicVeto(*dv_muc, *dv_elc, channel)) break;
            m_dv_ee_cf->Fill("R_{CR} > 0.04", 1);

            // delta R 
            float deltaR_min = 0.5;
            if(m_costool->getDeltaR(*dv_muc, *dv_elc, channel) < deltaR_min) break;
            m_dv_ee_cf->Fill("#DeltaR > 0.5", 1);
    
            // end of cut flow. Now plotting
            ATH_MSG_INFO("Found ee with mass = " << dv_mass << ", runNumber = "
            << evtInfo->runNumber() << ", eventNumber = "
            << evtInfo->eventNumber() << ", Lumiblock = " << evtInfo->lumiBlock() );

            // plot dv distributions
            plot_dv(*dv, *pv, channel);

            // plot muon kinematics
            plot_signal_tp(*dv_muc, *dv_elc, channel);

            // only for MC: find truth dv matched to this dv
            if (isMC) {
                // find truth dv matched to this dv
                const xAOD::TruthVertex* tru_v_loose = m_dvutils->IsSignalDV_loose(*dv_muc, *dv_elc, channel, *dv);
                if (tru_v_loose != nullptr) {
                    m_dv_ee_cf->Fill("Matched (Loose)", 1);
                }

                const xAOD::TruthVertex* tru_v = m_dvutils->IsSignalDV(*dv_muc, *dv_elc, channel);
                if (tru_v != nullptr) {
                    m_dv_ee_cf->Fill("Matched (Tight)", 1);
                }
            } // end of MC

        } // end of ee

        if (channel == "emu") {

            // emu pair
            m_dv_emu_cf->Fill("e#mu", 1);

            // Trigger matching
            //m_dilepdvc->DoTriggerMatching(*dv);
            //if(!m_dilepdvc->PassTriggerMatching(*dv)) break;
            if(!m_dvutils->TrigMatching(*dv)) break;
            m_dv_emu_cf->Fill("Trig. Matching", 1);

            // vertex fit quality
            m_dv_emu_chi2_ndof->Fill (dv->chiSquared() / dv->numberDoF() );
            if(!m_dvc->PassChisqCut(*dv)) break;
            m_dv_emu_cf->Fill("#chi^{2}_{DV} / DOF < 5", 1);

            // minimum distance from pv (from 0 for MC)
            if(!m_dvc->PassDistCut(*dv, *pvc)) break;
            m_dv_emu_cf->Fill("Disp. > 2 mm", 1);

            // charge requirements
            if(!m_dvc->PassChargeRequirement(*dv)) break;
            m_dv_emu_cf->Fill("e^{+}#mu^{-}, e^{-}#mu^{+}", 1);

            // disabled module
            if(!m_dvc->PassDisabledModuleVeto(*dv)) break;
            m_dv_emu_cf->Fill("DisabledModule", 1);

            // material veto
            if(!m_dvc->PassMaterialVeto(*dv)) break;
            m_dv_emu_cf->Fill("MaterialVeto", 1);

            //// DESD filter
            //m_dilepdvc->DoTriggerMatching(*dv);
            //if(!m_dilepdvc->PassDESDMatching(*dv)) break;
            //m_dv_emu_cf->Fill("PassDESDMatching", 1);

            // cosmic veto
            if(!PassCosmicVeto(*dv_muc, *dv_elc, channel)) break;
            m_dv_emu_cf->Fill("R_{CR} > 0.04", 1);

            // delta R 
            float deltaR_min = 0.5;
            if(m_costool->getDeltaR(*dv_muc, *dv_elc, channel) < deltaR_min) break;
            m_dv_emu_cf->Fill("#DeltaR > 0.5", 1);
    
            // end of cut flow. Now plotting
            ATH_MSG_INFO("Found emu with mass = " << dv_mass << ", runNumber = "
            << evtInfo->runNumber() << ", eventNumber = "
            << evtInfo->eventNumber() << ", Lumiblock = " << evtInfo->lumiBlock() );

            // plot dv distributions
            plot_dv(*dv, *pv, channel);

            // plot muon kinematics
            plot_signal_tp(*dv_muc, *dv_elc, channel);

            // only for MC: find truth dv matched to this dv
            if (isMC) {
                // find truth dv matched to this dv
                const xAOD::TruthVertex* tru_v_loose = m_dvutils->IsSignalDV_loose(*dv_muc, *dv_elc, channel, *dv);
                if (tru_v_loose != nullptr) {
                    m_dv_emu_cf->Fill("Matched (Loose)", 1);
                }

                const xAOD::TruthVertex* tru_v = m_dvutils->IsSignalDV(*dv_muc, *dv_elc, channel);
                if (tru_v != nullptr) {
                    m_dv_emu_cf->Fill("Matched (Tight)", 1);
                }
            } // end of MC

        } // end of emu

    } // end of dv loop

    return StatusCode::SUCCESS;
}

StatusCode DisplacedDimuonAnalysisAlg::beginInputFile() { 

  return StatusCode::SUCCESS;
}

bool DisplacedDimuonAnalysisAlg::PassCosmicVeto(const DataVector<xAOD::Muon> dv_muc, const DataVector<xAOD::Electron> dv_elc, std::string channel) {
    ATH_MSG_DEBUG ("Plotting cosmic veto" << name() << "...");

    bool PassCosmicVeto = true;

    float Rcos_min = 0.04;
    float deltaR_min = 0.5;
    float deltaPhiMinusPi = m_costool->getDeltaPhiMinusPi(dv_muc, dv_elc, channel);
    float sumEta = m_costool->getSumEta(dv_muc, dv_elc, channel);

    float Rcos = std::sqrt(sumEta * sumEta + deltaPhiMinusPi * deltaPhiMinusPi);
    float deltaR = m_costool->getDeltaR(dv_muc, dv_elc, channel);

    if (channel == "mumu"){
        // plot Rcos after applying deltaR < deltaR_min
        if (deltaR > deltaR_min){
            m_dv_mumu_Rcos->Fill(Rcos);
            m_dv_mumu_Rcos_low->Fill(Rcos);
        }

        // plot delta R after applying Rcos < Rcos_min
        if (Rcos > Rcos_min){
            m_dv_mumu_DeltaR->Fill(deltaR);
            m_dv_mumu_DeltaR_low->Fill(deltaR);
        }
    }

    if (channel == "ee"){
        // plot Rcos after applying deltaR < deltaR_min
        if (deltaR > deltaR_min){
            m_dv_ee_Rcos->Fill(Rcos);
            m_dv_ee_Rcos_low->Fill(Rcos);
        }

        // plot delta R after applying Rcos < Rcos_min
        if (Rcos > Rcos_min){
            m_dv_ee_DeltaR->Fill(deltaR);
            m_dv_ee_DeltaR_low->Fill(deltaR);
        }
    }

    if (channel == "emu"){
        // plot Rcos after applying deltaR < deltaR_min
        if (deltaR > deltaR_min){
            m_dv_emu_Rcos->Fill(Rcos);
            m_dv_emu_Rcos_low->Fill(Rcos);
        }

        // plot delta R after applying Rcos < Rcos_min
        if (Rcos > Rcos_min){
            m_dv_emu_DeltaR->Fill(deltaR);
            m_dv_emu_DeltaR_low->Fill(deltaR);
        }
    }

    if (Rcos < Rcos_min) PassCosmicVeto = false;

    return PassCosmicVeto;
}


void DisplacedDimuonAnalysisAlg::plot_signal_tp(const DataVector<xAOD::Muon> dv_muc, const DataVector<xAOD::Electron> dv_elc, std::string channel) {

    if (channel == "mumu"){
        // plot muon distribution
        for(auto mu: dv_muc){
            m_dv_mumu_mu_pt->Fill(mu->pt() / 1000.); // GeV
            m_dv_mumu_mu_pt_low->Fill(mu->pt() / 1000.); // GeV
            m_dv_mumu_mu_eta->Fill(mu->eta());
            m_dv_mumu_mu_phi->Fill(mu->phi());
            m_dv_mumu_mu_d0->Fill(mu->primaryTrackParticle()->d0());
            m_dv_mumu_mu_z0->Fill(mu->primaryTrackParticle()->z0());
        }

        //// plot the lowest pT of two muons
        //float max_muon_pt = m_dvutils->getMaxPT(dv_muc);
        //float min_muon_pt = m_dvutils->getMinPT(dv_muc);
        //m_dv_mumu_mu_pt_max->Fill(max_muon_pt);
        //m_dv_mumu_mu_pt_max_low->Fill(max_muon_pt);
        //m_dv_mumu_mu_pt_min->Fill(min_muon_pt);
        //m_dv_mumu_mu_pt_min_low->Fill(min_muon_pt);
    }

    if (channel == "ee"){
        // plot muon distribution
        for(auto el: dv_elc){
            m_dv_ee_e_pt->Fill(el->pt() / 1000.); // GeV
            m_dv_ee_e_pt_low->Fill(el->pt() / 1000.); // GeV
            m_dv_ee_e_eta->Fill(el->eta());
            m_dv_ee_e_phi->Fill(el->phi());
            m_dv_ee_e_d0->Fill(el->trackParticle()->d0());
            m_dv_ee_e_z0->Fill(el->trackParticle()->z0());
        }
    }

    if (channel == "emu"){
        // plot muon distribution
        for(auto mu: dv_muc){
            m_dv_emu_mu_pt->Fill(mu->pt() / 1000.); // GeV
            m_dv_emu_mu_pt_low->Fill(mu->pt() / 1000.); // GeV
            m_dv_emu_mu_eta->Fill(mu->eta());
            m_dv_emu_mu_phi->Fill(mu->phi());
            m_dv_emu_mu_d0->Fill(mu->primaryTrackParticle()->d0());
            m_dv_emu_mu_z0->Fill(mu->primaryTrackParticle()->z0());
        }
        for(auto el: dv_elc){
            m_dv_emu_e_pt->Fill(el->pt() / 1000.); // GeV
            m_dv_emu_e_pt_low->Fill(el->pt() / 1000.); // GeV
            m_dv_emu_e_eta->Fill(el->eta());
            m_dv_emu_e_phi->Fill(el->phi());
            m_dv_emu_e_d0->Fill(el->trackParticle()->d0());
            m_dv_emu_e_z0->Fill(el->trackParticle()->z0());
        }
    }

    return;
}

void DisplacedDimuonAnalysisAlg::plot_dv(const xAOD::Vertex& dv, const xAOD::Vertex& pv, std::string channel) {

    // access invariant mass
    float dv_mass = std::fabs(m_accMass(dv)) / 1000.; // in MeV

    // fill dimuon vertex
    float dv_R = m_dvutils->getR( dv, pv );                 // R in [mm]
    float dv_z = m_dvutils->getz( dv, pv );                 // z in [mm]
    float dv_l = m_dvutils->getr( dv, pv );                 // r in [mm]

    if (channel == "mumu"){
        m_dv_mumu_M->Fill(dv_mass);                             // dimuon mass
        m_dv_mumu_R->Fill(dv_R);                                
        m_dv_mumu_R_low->Fill(dv_R);                                
        m_dv_mumu_z->Fill(dv_z);                                
        m_dv_mumu_l->Fill(dv_l);                                
        m_dv_mumu_R_M->Fill(dv_R, dv_mass);
    }

    if (channel == "ee"){
        m_dv_ee_M->Fill(dv_mass);                             // dimuon mass
        m_dv_ee_R->Fill(dv_R);                                
        m_dv_ee_R_low->Fill(dv_R);                                
        m_dv_ee_z->Fill(dv_z);                                
        m_dv_ee_l->Fill(dv_l);                                
        m_dv_ee_R_M->Fill(dv_R, dv_mass);
    }

    if (channel == "emu"){
        m_dv_emu_M->Fill(dv_mass);                             // dimuon mass
        m_dv_emu_R->Fill(dv_R);                                
        m_dv_emu_R_low->Fill(dv_R);                                
        m_dv_emu_z->Fill(dv_z);                                
        m_dv_emu_l->Fill(dv_l);                                
        m_dv_emu_R_M->Fill(dv_R, dv_mass);
    }

    return;
}



