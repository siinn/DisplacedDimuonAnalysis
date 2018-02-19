#include "LowMass.h"

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

LowMass::LowMass( const std::string& name, ISvcLocator* pSvcLocator ) :
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
m_phmatch("DDL::PhotonMatch/PhotonMatch"),
m_fmtool("FilterMatchingTools"),
//m_matchTool("InDetVertexTruthMatchTool"),
m_accMass("mass")
{
    // initialize tools
    declareProperty("DVUtils", m_dvutils);
    declareProperty("LeptonSelectionTool", m_leptool);
    declareProperty("CosmicTool", m_costool);
    declareProperty("GRLTool",  m_grlTool, "The private GoodRunsListSelectionTool" );
    declareProperty("TrigDecisionTool", m_tdt);
    declareProperty("TrigMatchingTool", m_tmt);
    declareProperty("DiLepCosmics", m_cos);
    declareProperty("OverlapRemoval", m_or);
    declareProperty("DiLepEventCuts", m_evtc);
    declareProperty("DiLepBaseCuts", m_dvc);
    declareProperty("DiLepDVCuts", m_dilepdvc);
    declareProperty("PhotonMatch", m_phmatch);
    declareProperty("FilterMatchingTool", m_fmtool);
    //declareProperty("InDetVertexTruthMatchTool", m_matchTool);


}


LowMass::~LowMass() {}


StatusCode LowMass::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");

    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    // event cut flow
    m_event_cutflow = new TH1D( "m_event_cutflow", "Event cutflow", 5,0,5);
    CHECK( histSvc->regHist("/DV/low_mass/event_cutflow", m_event_cutflow) );

    // pile-up distribution
    m_pileup = new TH1F("m_pileup", "m_pileup", 100, 0, 100); 
    CHECK( histSvc->regHist("/DV/low_mass/pileup", m_pileup) );

    //--------------------------------------------------------
    // mumu
    //--------------------------------------------------------

    //Float_t m_dv_mumu_M_bins[] = {0,10,40,70,100,400,700,1000,2000};
    m_dv_mumu_cf = new TH1D( "lowmass_mumu_cf", "Reco dv mumu cutflow", 11,0,11);
    m_dv_mumu_M = new TH1F("lowmass_mumu_M","Dimuon DV mass in GeV", 40,0,10);
    m_dv_mumu_R = new TH1F("lowmass_mumu_R","R of dimuon dv [mm]",50,0.,300.);
    m_dv_mumu_R_low = new TH1F("lowmass_mumu_R_low","R of dimuon dv [mm], low",50,0.,50.);
    m_dv_mumu_z = new TH1F("lowmass_mumu_z","z of dimuon dv [mm]",100,-1000.,1000.);
    m_dv_mumu_l = new TH1F("lowmass_mumu_l","l of dimuon dv [mm]",100,0.,1000.);
    m_dv_mumu_eta = new TH1F("lowmass_mumu_eta","eta of dimuon dv ",30,-3,3);
    m_dv_mumu_R_M = new TH2F("lowmass_mumu_R_M","Dimuon DV position R vs M", 50,0,300,200,0,2000);

    // muon kinematics distribution
    m_dv_mumu_mu_pt = new TH1F("lowmass_mumu_mu_pt","Signal muon pT",50,0.,1000.);
    m_dv_mumu_mu_pt_low = new TH1F("lowmass_mumu_mu_pt_low","Signal muon low pT",50,0.,100.);
    m_dv_mumu_mu_eta = new TH1F("lowmass_mumu_mu_eta","Signal muon eta",50,-3.0,3.0);
    m_dv_mumu_mu_phi = new TH1F("lowmass_mumu_mu_phi","Signal muon phi",50,-M_PI,M_PI);
    m_dv_mumu_mu_d0 = new TH1F("lowmass_mumu_mu_d0","Signal muon d0",50,-300,300);
    m_dv_mumu_mu_z0 = new TH1F("lowmass_mumu_mu_z0","Signal muon z0",50,-1000,1000);

    // cosmic veto
    m_dv_mumu_DeltaR = new TH1F("lowmass_mumu_DeltaR","Signal muon Delta R",100, 0., 5.);
    m_dv_mumu_DeltaR_low = new TH1F("lowmass_mumu_DeltaR_low","Signal muon Delta R low",60, 0., 1);
    m_dv_mumu_Rcos = new TH1F("lowmass_mumu_Rcos","Signal muon Rcos",50, 0., 5.);
    m_dv_mumu_Rcos_low = new TH1F("lowmass_mumu_Rcos_low","Signal muon Rcos low",1000, 0., 0.1);
    m_dv_mumu_DeltaR_Rcos = new TH2F("lowmass_mumu_DeltaR_Rcos","#DeltaR vs R_{CR}", 100,0,5,100,0,5);

    // only for MC
    m_dv_mumu_M_matched = new TH1F("lowmass_mumu_M_matched","matched dimuon DV mass in GeV",200,0.,2000.);
    m_dv_mumu_R_matched = new TH1F("lowmass_mumu_R_matched","R of matched dimuon dv [mm]",50,0.,300.);
    m_dv_mumu_R_M_matched = new TH2F("lowmass_mumu_R_M_matched","matched dimuon DV position R vs M", 50,0,300,200,0,2000);
    m_dv_mumu_chi2_ndof = new TH1F("lowmass_mumu_chi2_ndof", "chi^2 / ndof (mumu)", 100, 0, 10);
    m_dv_mumu_eta_matched = new TH1F("lowmass_mumu_eta_matched","eta of dimuon dv",40,-4.,4.);

    // registor for output
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/dv_mumu_cf", m_dv_mumu_cf) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/dv_mumu_M", m_dv_mumu_M) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/dv_mumu_R", m_dv_mumu_R) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/dv_mumu_R_low", m_dv_mumu_R_low) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/dv_mumu_z", m_dv_mumu_z) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/dv_mumu_l", m_dv_mumu_l) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/dv_mumu_eta", m_dv_mumu_eta) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/dv_mumu_R_M", m_dv_mumu_R_M) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/dv_mumu_chi2_ndof", m_dv_mumu_chi2_ndof) );

    // muon kinematics distribution
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/outgoing/dv_mumu_mu_pt", m_dv_mumu_mu_pt) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/outgoing/dv_mumu_mu_pt_low", m_dv_mumu_mu_pt_low) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/outgoing/dv_mumu_mu_eta", m_dv_mumu_mu_eta) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/outgoing/dv_mumu_mu_phi", m_dv_mumu_mu_phi) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/outgoing/dv_mumu_mu_d0", m_dv_mumu_mu_d0) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/outgoing/dv_mumu_mu_z0", m_dv_mumu_mu_z0) );

    // cosmic veto
    CHECK( histSvc->regHist("/DV/low_mass/cosmicBkg/mumu/dv_mumu_DeltaR", m_dv_mumu_DeltaR) );
    CHECK( histSvc->regHist("/DV/low_mass/cosmicBkg/mumu/dv_mumu_DeltaR_low", m_dv_mumu_DeltaR_low) );
    CHECK( histSvc->regHist("/DV/low_mass/cosmicBkg/mumu/dv_mumu_Rcos", m_dv_mumu_Rcos) );
    CHECK( histSvc->regHist("/DV/low_mass/cosmicBkg/mumu/dv_mumu_Rcos_low", m_dv_mumu_Rcos_low) );
    CHECK( histSvc->regHist("/DV/low_mass/cosmicBkg/mumu/dv_mumu_DeltaR_Rcos", m_dv_mumu_DeltaR_Rcos) );

    // only for MC
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/truth-matched/reco_dv_mumu_M", m_dv_mumu_M_matched) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/truth-matched/reco_dv_mumu_R", m_dv_mumu_R_matched) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/truth-matched/reco_dv_mumu_R_M", m_dv_mumu_R_M_matched) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_mumu/truth-matched/truth_dv_zp_eta", m_dv_mumu_eta_matched) );

    //--------------------------------------------------------
    // ee
    //--------------------------------------------------------

    //Float_t m_dv_ee_M_bins[] = {0,10,40,70,100,400,700,1000,2000};
    m_dv_ee_cf = new TH1D( "m_dv_ee_cf", "Reco dv ee cutflow", 11,0,11);
    m_dv_ee_M = new TH1F("lowmass_ee_M","ee DV mass in GeV",40,0,10);
    m_dv_ee_R = new TH1F("lowmass_ee_R","R of ee dv [mm]",50,0.,300.);
    m_dv_ee_R_low = new TH1F("lowmass_ee_R_low","R of ee dv [mm], low",50,0.,50.);
    m_dv_ee_z = new TH1F("lowmass_ee_z","z of ee dv [mm]",100,-1000.,1000.);
    m_dv_ee_l = new TH1F("lowmass_ee_l","l of ee dv [mm]",100,0.,1000.);
    m_dv_ee_eta = new TH1F("lowmass_ee_eta","eta of ee dv ",30,-3,3);
    m_dv_ee_R_M = new TH2F("lowmass_ee_R_M","ee DV position R vs M", 50,0,300,200,0,2000);

    // muon kinematics distribution
    m_dv_ee_e_pt = new TH1F("lowmass_ee_e_pt","Signal electron pT",50,0.,1000.);
    m_dv_ee_e_pt_low = new TH1F("lowmass_ee_e_pt_low","Signal electron low pT",50,0.,100.);
    m_dv_ee_e_eta = new TH1F("lowmass_ee_e_eta","Signal electron eta",50,-3.0,3.0);
    m_dv_ee_e_phi = new TH1F("lowmass_ee_e_phi","Signal electron phi",50,-M_PI,M_PI);
    m_dv_ee_e_d0 = new TH1F("lowmass_ee_e_d0","Signal electron d0",50,-300,300);
    m_dv_ee_e_z0 = new TH1F("lowmass_ee_e_z0","Signal electron z0",50,-1000,1000);

    // cosmic veto
    m_dv_ee_DeltaR = new TH1F("lowmass_ee_DeltaR","Signal ee Delta R",100, 0., 5.);
    m_dv_ee_DeltaR_low = new TH1F("lowmass_ee_DeltaR_low","Signal ee Delta R low",100, 0., 1);
    m_dv_ee_Rcos = new TH1F("lowmass_ee_Rcos","Signal ee Rcos",50, 0., 5.);
    m_dv_ee_Rcos_low = new TH1F("lowmass_ee_Rcos_low","Signal ee Rcos low",25, 0., 0.1);

    // only for MC
    m_dv_ee_M_matched = new TH1F("lowmass_ee_M_matched","matched ee DV mass in GeV",200,0.,2000.);
    m_dv_ee_R_matched = new TH1F("lowmass_ee_R_matched","R of matched ee dv [mm]",50,0.,300.);
    m_dv_ee_R_M_matched = new TH2F("lowmass_ee_R_M_matched","matched ee DV position R vs M", 50,0,300,200,0,2000);
    m_dv_ee_eta_matched = new TH1F("lowmass_ee_eta_matched","eta of ee dv",100,-4.,4.);
    m_dv_ee_chi2_ndof = new TH1F("lowmass_ee_chi2_ndof", "chi^2 / ndof (ee)", 100, 0, 10);

    // registor for output
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/dv_ee_cf", m_dv_ee_cf) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/dv_ee_M", m_dv_ee_M) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/dv_ee_R", m_dv_ee_R) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/dv_ee_R_low", m_dv_ee_R_low) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/dv_ee_z", m_dv_ee_z) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/dv_ee_l", m_dv_ee_l) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/dv_ee_eta", m_dv_ee_eta) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/dv_ee_R_M", m_dv_ee_R_M) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/dv_ee_chi2_ndof", m_dv_ee_chi2_ndof) );

    // electron kinematics distribution
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/outgoing/dv_ee_e_pt", m_dv_ee_e_pt) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/outgoing/dv_ee_e_pt_low", m_dv_ee_e_pt_low) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/outgoing/dv_ee_e_eta", m_dv_ee_e_eta) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/outgoing/dv_ee_e_phi", m_dv_ee_e_phi) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/outgoing/dv_ee_e_d0", m_dv_ee_e_d0) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/outgoing/dv_ee_e_z0", m_dv_ee_e_z0) );

    // cosmic veto
    CHECK( histSvc->regHist("/DV/low_mass/cosmicBkg/ee/dv_ee_DeltaR", m_dv_ee_DeltaR) );
    CHECK( histSvc->regHist("/DV/low_mass/cosmicBkg/ee/dv_ee_DeltaR_low", m_dv_ee_DeltaR_low) );
    CHECK( histSvc->regHist("/DV/low_mass/cosmicBkg/ee/dv_ee_Rcos", m_dv_ee_Rcos) );
    CHECK( histSvc->regHist("/DV/low_mass/cosmicBkg/ee/dv_ee_Rcos_low", m_dv_ee_Rcos_low) );

    // only for MC
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/truth-matched/reco_dv_ee_M", m_dv_ee_M_matched) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/truth-matched/reco_dv_ee_R", m_dv_ee_R_matched) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/truth-matched/reco_dv_ee_R_M", m_dv_ee_R_M_matched) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_ee/truth-matched/truth_dv_zp_eta", m_dv_ee_eta_matched) );

    //--------------------------------------------------------
    // emu
    //--------------------------------------------------------

    //Float_t m_dv_emu_M_bins[] = {0,10,40,70,100,400,700,1000,2000};
    m_dv_emu_cf = new TH1D( "m_dv_emu_cf", "Reco dv emu cutflow", 11,0,11);
    m_dv_emu_M = new TH1F("lowmass_emu_M","emu DV mass in GeV",40,0,10);
    m_dv_emu_R = new TH1F("lowmass_emu_R","R of emu dv [mm]",50,0.,300.);
    m_dv_emu_R_low = new TH1F("lowmass_emu_R_low","R of emu dv [mm], low",50,0.,50.);
    m_dv_emu_z = new TH1F("lowmass_emu_z","z of emu dv [mm]",100,-1000.,1000.);
    m_dv_emu_l = new TH1F("lowmass_emu_l","l of emu dv [mm]",100,0.,1000.);
    m_dv_emu_eta = new TH1F("lowmass_emu_eta","eta of emu dv ",30,-3,3);
    m_dv_emu_R_M = new TH2F("lowmass_emu_R_M","emu DV position R vs M", 50,0,300,200,0,2000);

    // muon kinematics distribution
    m_dv_emu_e_pt = new TH1F("lowmass_emu_e_pt","Signal electron pT",50,0.,1000.);
    m_dv_emu_e_pt_low = new TH1F("lowmass_emu_e_pt_low","Signal electron low pT",50,0.,100.);
    m_dv_emu_e_eta = new TH1F("lowmass_emu_e_eta","Signal electron eta",50,-3.0,3.0);
    m_dv_emu_e_phi = new TH1F("lowmass_emu_e_phi","Signal electron phi",50,-M_PI,M_PI);
    m_dv_emu_e_d0 = new TH1F("lowmass_emu_e_d0","Signal electron d0",50,-300,300);
    m_dv_emu_e_z0 = new TH1F("lowmass_emu_e_z0","Signal electron z0",50,-1000,1000);

    m_dv_emu_mu_pt = new TH1F("lowmass_emu_mu_pt","Signal electron pT",50,0.,1000.);
    m_dv_emu_mu_pt_low = new TH1F("lowmass_emu_mu_pt_low","Signal electron low pT",50,0.,100.);
    m_dv_emu_mu_eta = new TH1F("lowmass_emu_mu_eta","Signal electron eta",50,-3.0,3.0);
    m_dv_emu_mu_phi = new TH1F("lowmass_emu_mu_phi","Signal electron phi",50,-M_PI,M_PI);
    m_dv_emu_mu_d0 = new TH1F("lowmass_emu_mu_d0","Signal electron d0",50,-300,300);
    m_dv_emu_mu_z0 = new TH1F("lowmass_emu_mu_z0","Signal electron z0",50,-1000,1000);

    // cosmic veto
    m_dv_emu_DeltaR = new TH1F("lowmass_emu_DeltaR","Signal emu Delta R",100, 0., 5.);
    m_dv_emu_DeltaR_low = new TH1F("lowmass_emu_DeltaR_low","Signal emu Delta R low",100, 0., 1);
    m_dv_emu_Rcos = new TH1F("lowmass_emu_Rcos","Signal emu Rcos",50, 0., 5.);
    m_dv_emu_Rcos_low = new TH1F("lowmass_emu_Rcos_low","Signal emu Rcos low",25, 0., 0.1);

    // only for MC
    m_dv_emu_M_matched = new TH1F("lowmass_emu_M_matched","matched emu DV mass in GeV",200,0.,2000.);
    m_dv_emu_R_matched = new TH1F("lowmass_emu_R_matched","R of matched emu dv [mm]",50,0.,300.);
    m_dv_emu_R_M_matched = new TH2F("lowmass_emu_R_M_matched","matched emu DV position R vs M", 50,0,300,200,0,2000);
    m_dv_emu_eta_matched = new TH1F("lowmass_emu_eta_matched","eta of emu dv",100,-4.,4.);
    m_dv_emu_chi2_ndof = new TH1F("lowmass_emu_chi2_ndof", "chi^2 / ndof (emu)", 100, 0, 10);

    // registor for output
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/dv_emu_cf", m_dv_emu_cf) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/dv_emu_M", m_dv_emu_M) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/dv_emu_R", m_dv_emu_R) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/dv_emu_R_low", m_dv_emu_R_low) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/dv_emu_z", m_dv_emu_z) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/dv_emu_l", m_dv_emu_l) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/dv_emu_eta", m_dv_emu_eta) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/dv_emu_R_M", m_dv_emu_R_M) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/dv_emu_chi2_ndof", m_dv_emu_chi2_ndof) );

    // electron kinematics distribution
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/outgoing/dv_emu_e_pt", m_dv_emu_e_pt) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/outgoing/dv_emu_e_pt_low", m_dv_emu_e_pt_low) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/outgoing/dv_emu_e_eta", m_dv_emu_e_eta) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/outgoing/dv_emu_e_phi", m_dv_emu_e_phi) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/outgoing/dv_emu_e_d0", m_dv_emu_e_d0) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/outgoing/dv_emu_e_z0", m_dv_emu_e_z0) );

    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/outgoing/dv_emu_mu_pt", m_dv_emu_mu_pt) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/outgoing/dv_emu_mu_pt_low", m_dv_emu_mu_pt_low) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/outgoing/dv_emu_mu_eta", m_dv_emu_mu_eta) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/outgoing/dv_emu_mu_phi", m_dv_emu_mu_phi) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/outgoing/dv_emu_mu_d0", m_dv_emu_mu_d0) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/outgoing/dv_emu_mu_z0", m_dv_emu_mu_z0) );

    // cosmic veto
    CHECK( histSvc->regHist("/DV/low_mass/cosmicBkg/emu/dv_emu_DeltaR", m_dv_emu_DeltaR) );
    CHECK( histSvc->regHist("/DV/low_mass/cosmicBkg/emu/dv_emu_DeltaR_low", m_dv_emu_DeltaR_low) );
    CHECK( histSvc->regHist("/DV/low_mass/cosmicBkg/emu/dv_emu_Rcos", m_dv_emu_Rcos) );
    CHECK( histSvc->regHist("/DV/low_mass/cosmicBkg/emu/dv_emu_Rcos_low", m_dv_emu_Rcos_low) );

    // only for MC
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/truth-matched/reco_dv_emu_M", m_dv_emu_M_matched) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/truth-matched/reco_dv_emu_R", m_dv_emu_R_matched) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/truth-matched/reco_dv_emu_R_M", m_dv_emu_R_M_matched) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_emu/truth-matched/truth_dv_zp_eta", m_dv_emu_eta_matched) );


    //--------------------------------------------------------
    // trk-trk vertex plots
    //--------------------------------------------------------
    m_dv_idid_cf = new TH1D( "m_dv_idid_cf", "Reco dv idid cutflow", 8,0,8);
    m_dv_idid_M = new TH1F("lowmass_idid_M","lowmass mass in GeV", 40, 0, 10. );
    m_dv_idid_M_high = new TH1F("lowmass_idid_M_high","lowmass mass in GeV", 8000, 0, 2000. );
    m_dv_idid_R = new TH1F("lowmass_idid_R","R [mm]", 60, 0, 300. );
    m_dv_idid_z = new TH1F("lowmass_idid_z","z [mm]", 20, -1000., 1000.);
    m_dv_idid_l = new TH1F("lowmass_idid_l","l [mm]", 20, 0., 1000.);
    m_dv_idid_eta = new TH1F("lowmass_idid_eta","eta of idid dv ",30,-3,3);
    m_dv_idid_chi2_ndof = new TH1F("lowmass_idid_chi2_ndof","chi^2 / ndof", 20, 0.,5.);

    // cosmic veto
    m_dv_idid_DeltaR = new TH1F("lowmass_idid_DeltaR","Signal muon Delta R",100, 0., 5.);
    m_dv_idid_DeltaR_low = new TH1F("lowmass_idid_DeltaR_low","Signal muon Delta R low",20, 0., 1);
    m_dv_idid_Rcos = new TH1F("lowmass_idid_Rcos","Signal muon Rcos",25, 0., 5.);
    m_dv_idid_Rcos_low = new TH1F("lowmass_idid_Rcos_low","Signal muon Rcos low",1000, 0., 0.1);
    m_dv_idid_DeltaR_Rcos = new TH2F("lowmass_idid_DeltaR_Rcos","#DeltaR vs R_{CR}", 100,0,5,100,0,5);

    CHECK( histSvc->regHist("/DV/low_mass/dv_idid/dv_idid_cf", m_dv_idid_cf) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_idid/dv_idid_M", m_dv_idid_M) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_idid/dv_idid_M_high", m_dv_idid_M_high) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_idid/dv_idid_R", m_dv_idid_R) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_idid/dv_idid_z", m_dv_idid_z) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_idid/dv_idid_l", m_dv_idid_l) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_idid/dv_idid_eta", m_dv_idid_eta) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_idid/dv_idid_chi2_ndof", m_dv_idid_chi2_ndof) );

    // cosmic veto
    CHECK( histSvc->regHist("/DV/low_mass/cosmicBkg/idid/dv_idid_DeltaR", m_dv_idid_DeltaR) );
    CHECK( histSvc->regHist("/DV/low_mass/cosmicBkg/idid/dv_idid_DeltaR_low", m_dv_idid_DeltaR_low) );
    CHECK( histSvc->regHist("/DV/low_mass/cosmicBkg/idid/dv_idid_Rcos", m_dv_idid_Rcos) );
    CHECK( histSvc->regHist("/DV/low_mass/cosmicBkg/idid/dv_idid_Rcos_low", m_dv_idid_Rcos_low) );
    CHECK( histSvc->regHist("/DV/low_mass/cosmicBkg/idid/dv_idid_DeltaR_Rcos", m_dv_idid_DeltaR_Rcos) );

    //--------------------------------------------------------
    // mu + track, e + track
    //--------------------------------------------------------
    m_dv_mut_cf = new TH1D( "lowmass_dv_mut_cf", "Reco dv mut cutflow", 8,0,8);
    m_dv_mut_M = new TH1F("lowmass_mu_M","lowmass mass in GeV", 40, 0, 10. );
    m_dv_mut_M_high = new TH1F("lowmass_mu_M_high","lowmass mass in GeV", 8000, 0, 2000. );

    m_dv_et_cf = new TH1D( "lowmass_dv_et_cf", "Reco dv et cutflow", 8,0,8);
    m_dv_et_M = new TH1F("lowmass_et_M","lowmass mass in GeV", 40, 0, 10. );
    m_dv_et_M_high = new TH1F("lowmass_et_M_high","lowmass mass in GeV", 8000, 0, 2000. );

    CHECK( histSvc->regHist("/DV/low_mass/dv_mut/dv_mut_cf", m_dv_mut_cf) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_mut/dv_mut_M", m_dv_mut_M) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_mut/dv_mut_M_high", m_dv_mut_M_high) );

    CHECK( histSvc->regHist("/DV/low_mass/dv_et/dv_et_cf", m_dv_et_cf) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_et/dv_et_M", m_dv_et_M) );
    CHECK( histSvc->regHist("/DV/low_mass/dv_et/dv_et_M_high", m_dv_et_M_high) );

    return StatusCode::SUCCESS;
}

StatusCode LowMass::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");
    ATH_MSG_INFO("n_event_all = " << n_event_all);
    ATH_MSG_INFO("n_vrtsec_all = " << n_vrtsec_all);
    ATH_MSG_INFO("n_dvc_copy = " << n_dvc_copy);
    ATH_MSG_INFO("n_dv_all = " << n_dv_all);
    ATH_MSG_INFO("n_dv_passed_cut = " << n_dv_passed_cut);
    return StatusCode::SUCCESS;
}

StatusCode LowMass::execute() {
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    ATH_MSG_DEBUG("n_event_all = " << n_event_all
                 << ", n_vrtsec_all = " << n_vrtsec_all
                 << ", n_dvc_copy = " << n_dvc_copy
                 << ", n_dv_all = " << n_dv_all);

    // retrieve event info
    const xAOD::EventInfo* evtInfo = nullptr;
    CHECK( evtStore()->retrieve( evtInfo, "EventInfo" ) );

    // pile-up
    int pileup = evtInfo->actualInteractionsPerCrossing();
    m_pileup->Fill(pileup);

    // flag to check if data or MC
    bool isMC = evtInfo->eventType(xAOD::EventInfo::IS_SIMULATION);

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

    // make copies of leptons
    auto muc_copy = xAOD::shallowCopyContainer(*muc);
    xAOD::setOriginalObjectLink(*muc, *muc_copy.first);

    auto elc_copy = xAOD::shallowCopyContainer(*elc);
    xAOD::setOriginalObjectLink(*elc, *elc_copy.first);

    // apply overlap removal
    m_or->FindOverlap(*elc_copy.first, *muc_copy.first);

    // perform matching of photons to electrons
    m_phmatch->MatchPhotons(*phc, *elc_copy.first);

    // retrieve secondary vertices
    const xAOD::VertexContainer* dvc = nullptr;
    CHECK( evtStore()->retrieve( dvc, "VrtSecInclusive_SecondaryVertices" ));
    n_vrtsec_all = n_vrtsec_all + dvc->size();

    // make a copy of vertex containers
    auto dvc_copy = xAOD::shallowCopyContainer(*dvc);

    // count all event before looop
    n_event_all++;
    n_dvc_copy = n_dvc_copy + (*dvc_copy.first).size();


    //---------------------------------------
    // Event cut flow
    //---------------------------------------

    bool trig_passed = false;

    if(isMC){
        m_event_cutflow->Fill("AllEvents",1);
        //m_event_cutflow->SetBinContent(1, 20000);
    }

    // all events already passed RPVLL filter
    m_event_cutflow->Fill("RPVLLFilter", 1);

    // GRL
    if (!isMC and !m_grlTool->passRunLB(*evtInfo)) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("GRL (Data)", 1);

    // event cleaning
    if(!m_evtc->PassEventCleaning(*evtInfo)) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("EvtCleaning (Data)", 1);

    if (m_tdt->isPassed("HLT_mu60_0eta105_msonly")) trig_passed = true;
    if (m_tdt->isPassed("HLT_g140_loose")) trig_passed = true;
    if (m_tdt->isPassed("HLT_2g50_loose")) trig_passed = true;

    // trigger check
    // no trigger requirement for more statistics
    //if(!trig_passed) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("Trig", 1);

    // cosmic veto
    if(!m_cos->PassCosmicEventVeto(*elc, *muc)) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("CosmicVeto", 1);

    // get primary vertex
    auto pv = m_evtc->GetPV(*pvc);

    // PV position < 200 mm
    float pv_z_max = 200.;

    // apply primary vertex position cut
    if (pv) {

        // get primary vertex position
        auto pv_pos = pv->position();

        // z_pv cut
        if(pv_pos.z() > pv_z_max) return StatusCode::SUCCESS;
    }
    else return StatusCode::SUCCESS;
    m_event_cutflow->Fill("z_{PV} < 200 mm", 1);



    //------------------------------
    // dv cut flow
    //------------------------------
    for(auto dv: *dvc_copy.first) {

        // mass cut
        float mass_min = 10.;
        //float track_mass_min = 6.;
        //float deltaR_min = 0.04;
        float dv_R_max = 300;
        float dv_z_max = 300;

        // counting all dv
        n_dv_all++;

        // perform lepton matching
        m_dilepdvc->ApplyLeptonMatching(*dv, *elc_copy.first, *muc_copy.first);

        // access invariant mass
        float dv_mass = std::fabs(m_accMass(*dv)) / 1000.; // in MeV

        // collect leptons from this dv
        auto dv_muc = m_accMu(*dv);
        auto dv_elc = m_accEl(*dv);

        // access tracks from vertex
        auto tpLinks = dv->trackParticleLinks();

        xAOD::TrackParticle tp1 = **(tpLinks.at(0));
        xAOD::TrackParticle tp2 = **(tpLinks.at(1));

        // remove bad electrons
        m_leptool->BadClusterRemoval(*dv);

        // kinematic cut
        m_leptool->ElectronKinematicCut(*dv);

        // Electron identification
        m_leptool->ElectronID(*dv);

        // muon selection tool
        m_leptool->MuonSelection(*dv);

        // remove overlapping muon
        m_dilepdvc->ApplyOverlapRemoval(*dv);

        // perform filter matching
        m_dilepdvc->DoFilterMatching(*dv);

        // select only vertex with tracks
        if(dv->trackParticleLinks().size() != 2) continue;

        // find track invariant mass
        float track_mass = m_dvutils->TrackMass(tp1, tp2) / 1000.; // in GeV

        // find delta R between tracks
        float deltaR = m_dvutils->getDeltaR(tp1, tp2);

        // get position of DV
        float dv_R = m_dvutils->getR( *dv, *pv );                 // R in [mm]
        float dv_z = std::abs(m_dvutils->getz( *dv, *pv ));       // z in [mm]

        // find decay channel of dv
        std::string channel = m_dvutils->DecayChannel(*dv);


        if (channel == "mumu") {

            // mumu pair
            m_dv_mumu_cf->Fill("#mu#mu", 1);

            // Trigger matching
            //if(!m_dvutils->TrigMatching(*dv)) continue;
            //m_dv_mumu_cf->Fill("Trig. Matching", 1);

            // vertex fit quality
            m_dv_mumu_chi2_ndof->Fill (dv->chiSquared() / dv->numberDoF() );
            if(!m_dvc->PassChisqCut(*dv)) continue;
            m_dv_mumu_cf->Fill("#chi^{2}_{DV} / DOF < 5", 1);

            // minimum distance from pv (from 0 for MC)
            if(!m_dvc->PassDistCut(*dv, *pvc)) continue;
            m_dv_mumu_cf->Fill("Disp. > 2 mm", 1);

            // charge requirements
            if(!m_dvc->PassChargeRequirement(*dv)) continue;
            m_dv_mumu_cf->Fill("#mu^{+}#mu^{-}", 1);

            // disabled module
            if(!m_dvc->PassDisabledModuleVeto(*dv)) continue;
            m_dv_mumu_cf->Fill("DisabledModule", 1);

            // material veto (excluding mu)
            //m_dv_mumu_cf->Fill("MaterialVeto (excluding mu)", 1);

            // low mass veto
            if(dv_mass > mass_min) continue;
            m_dv_mumu_cf->Fill("LowMassVeto", 1);

            // fill cosmic veto background
            FillCosmicBkg(tp1, tp2, channel);

            // cosmic veto
            //if(!PassCosmicVeto(*dv_muc, *dv_elc, channel)) continue;
            //m_dv_mumu_cf->Fill("R_{CR} > 0.04", 1);

            // RPVLL filter matching
            //if(!m_dilepdvc->PassFilterMatching(*dv)) continue;
            //m_dv_mumu_cf->Fill("FilterMatching", 1);

            // DV R <  300 mm
            if(dv_R > dv_R_max) continue;
            m_dv_mumu_cf->Fill("R_{DV} > 300 mm", 1);

            // DV z <  300 mm
            if(dv_z > dv_z_max) continue;
            m_dv_mumu_cf->Fill("z_{DV} > 300 mm", 1);

            // track kinematic cut
            if(!m_fmtool->PassTrackKinematic(tp1, tp2)) continue;
            m_dv_mumu_cf->Fill("Track kinematic", 1);

            // end of cut flow. Now plotting
            ATH_MSG_INFO("Found signal mumu with mass = " << dv_mass << ", runNumber = "
            << evtInfo->runNumber() << ", eventNumber = "
            << evtInfo->eventNumber() << ", Lumiblock = " << evtInfo->lumiBlock() );

            // plot dv distributions
            plot_dv(*dv, *pv, channel);

            // plot muon kinematics
            plot_signal_tp(*dv_muc, *dv_elc, channel);

            // truth match
            if (isMC){
                // create truth vertex for matching
                const xAOD::TruthVertex *tru_matched = nullptr;

                tru_matched = m_dvutils->getClosestTruthVertex(dv);
                if(tru_matched) m_dv_mumu_cf->Fill("Truth matched", 1);
            }
        } // end of mumu

        if (channel == "ee") {

            // ee pair
            m_dv_ee_cf->Fill("ee", 1);

            // Trigger matching
            //if(!m_dvutils->TrigMatching(*dv)) continue;
            //m_dv_ee_cf->Fill("Trig. Matching", 1);

            // vertex fit quality
            m_dv_ee_chi2_ndof->Fill (dv->chiSquared() / dv->numberDoF() );
            if(!m_dvc->PassChisqCut(*dv)) continue;
            m_dv_ee_cf->Fill("#chi^{2}_{DV} / DOF < 5", 1);

            // minimum distance from pv (from 0 for MC)
            if(!m_dvc->PassDistCut(*dv, *pvc)) continue;
            m_dv_ee_cf->Fill("Disp. > 2 mm", 1);

            // charge requirements
            if(!m_dvc->PassChargeRequirement(*dv)) continue;
            m_dv_ee_cf->Fill("e^{+}e^{-}", 1);

            // disabled module
            if(!m_dvc->PassDisabledModuleVeto(*dv)) continue;
            m_dv_ee_cf->Fill("DisabledModule", 1);

            // material veto
            //if(!m_dvc->PassMaterialVeto(*dv)) continue;
            //m_dv_ee_cf->Fill("MaterialVeto", 1);

            // low mass veto
            if(dv_mass > mass_min) continue;
            m_dv_ee_cf->Fill("LowMassVeto", 1);
            //m_dv_ee_cf->Fill("m < 10 GeV", 1);

            // fill cosmic veto background
            FillCosmicBkg(tp1, tp2, channel);

            // cosmic veto
            //if(!PassCosmicVeto(*dv_muc, *dv_elc, channel)) continue;
            //m_dv_ee_cf->Fill("R_{CR} > 0.04", 1);

            // RPVLL filter matching
            //if(!m_dilepdvc->PassFilterMatching(*dv)) continue;
            //m_dv_ee_cf->Fill("FilterMatching", 1);

            // DV R <  300 mm
            if(dv_R > dv_R_max) continue;
            m_dv_ee_cf->Fill("R_{DV} > 300 mm", 1);

            // DV z <  300 mm
            if(dv_z > dv_z_max) continue;
            m_dv_ee_cf->Fill("z_{DV} > 300 mm", 1);

            // track kinematic cut
            if(!m_fmtool->PassTrackKinematic(tp1, tp2)) continue;
            m_dv_ee_cf->Fill("Track kinematic", 1);

            // plot dv distributions
            plot_dv(*dv, *pv, channel);

            // plot muon kinematics
            plot_signal_tp(*dv_muc, *dv_elc, channel);

            // truth match
            if (isMC){
                // create truth vertex for matching
                const xAOD::TruthVertex *tru_matched = nullptr;

                tru_matched = m_dvutils->getClosestTruthVertex(dv);
                if(tru_matched) m_dv_ee_cf->Fill("Truth matched", 1);
            }

        } // end of ee

        if (channel == "emu") {

            // emu pair
            m_dv_emu_cf->Fill("e#mu", 1);

            // Trigger matching
            //if(!m_dvutils->TrigMatching(*dv)) continue;
            //m_dv_emu_cf->Fill("Trig. Matching", 1);

            // vertex fit quality
            m_dv_emu_chi2_ndof->Fill (dv->chiSquared() / dv->numberDoF() );
            if(!m_dvc->PassChisqCut(*dv)) continue;
            m_dv_emu_cf->Fill("#chi^{2}_{DV} / DOF < 5", 1);

            // minimum distance from pv (from 0 for MC)
            if(!m_dvc->PassDistCut(*dv, *pvc)) continue;
            m_dv_emu_cf->Fill("Disp. > 2 mm", 1);

            // charge requirements
            if(!m_dvc->PassChargeRequirement(*dv)) continue;
            m_dv_emu_cf->Fill("e^{+}#mu^{-}, e^{-}#mu^{+}", 1);

            // disabled module
            if(!m_dvc->PassDisabledModuleVeto(*dv)) continue;
            m_dv_emu_cf->Fill("DisabledModule", 1);

            // material veto
            //if(!m_dvc->PassMaterialVeto(*dv)) continue;
            //m_dv_emu_cf->Fill("MaterialVeto", 1);

            // low mass veto
            if(dv_mass > mass_min) continue;
            m_dv_emu_cf->Fill("LowMassVeto", 1);
            //m_dv_emu_cf->Fill("m < 10 GeV", 1);

            // fill cosmic veto background
            FillCosmicBkg(tp1, tp2, channel);

            // cosmic veto
            //if(!PassCosmicVeto(*dv_muc, *dv_elc, channel)) continue;
            //m_dv_emu_cf->Fill("R_{CR} > 0.04", 1);

            // RPVLL filter matching
            //if(!m_dilepdvc->PassFilterMatching(*dv)) continue;
            //m_dv_emu_cf->Fill("FilterMatching", 1);

            // DV R <  300 mm
            if(dv_R > dv_R_max) continue;
            m_dv_emu_cf->Fill("R_{DV} > 300 mm", 1);

            // DV z <  300 mm
            if(dv_z > dv_z_max) continue;
            m_dv_emu_cf->Fill("z_{DV} > 300 mm", 1);

            // track kinematic cut
            if(!m_fmtool->PassTrackKinematic(tp1, tp2)) continue;
            m_dv_emu_cf->Fill("Track kinematic", 1);

            // plot dv distributions
            plot_dv(*dv, *pv, channel);

            // plot muon kinematics
            plot_signal_tp(*dv_muc, *dv_elc, channel);

            // truth match
            if (isMC){
                // create truth vertex for matching
                const xAOD::TruthVertex *tru_matched = nullptr;

                tru_matched = m_dvutils->getClosestTruthVertex(dv);
                if(tru_matched) m_dv_emu_cf->Fill("Truth matched", 1);
            }

        } // end of emu

        if (channel == "mut") {

            // mut pair
            m_dv_mut_cf->Fill("mu-trk", 1);

            // vertex fit quality
            if(!m_dvc->PassChisqCut(*dv)) continue;
            m_dv_mut_cf->Fill("#chi^{2}_{DV} / DOF < 5", 1);

            // minimum distance from pv (from 0 for MC)
            if(!m_dvc->PassDistCut(*dv, *pvc)) continue;
            m_dv_mut_cf->Fill("Disp. > 2 mm", 1);

            // charge requirements
            if(!m_dvc->PassChargeRequirement(*dv)) continue;
            m_dv_mut_cf->Fill("#mu^{+,-}-trk^{-,+}", 1);

            // disabled module
            if(!m_dvc->PassDisabledModuleVeto(*dv)) continue;
            m_dv_mut_cf->Fill("DisabledModule", 1);

            // material veto (excluding mu)
            //if(!m_dvc->PassMaterialVeto(*dv)) continue;
            //m_dv_mut_cf->Fill("MaterialVeto (excluding mu)", 1);

            // low mass veto
            if(dv_mass > mass_min) {
                m_dv_mut_M_high->Fill(dv_mass);                    // dimuon mass
                continue;
            }
            m_dv_mut_cf->Fill("LowMassVeto", 1);

            // RPVLL filter matching
            //if(!m_dilepdvc->PassFilterMatching(*dv)) continue;
            //m_dv_mut_cf->Fill("FilterMatching", 1);

            // fill cosmic veto background
            FillCosmicBkg(tp1, tp2, channel);
            
            // cosmic veto (R_CR)
            if(!PassCosmicVeto_R_CR(tp1, tp2)) continue;
            m_dv_mut_cf->Fill("R_{CR} > 0.04", 1);

            // DV R <  300 mm
            if(dv_R > dv_R_max) continue;
            m_dv_mut_cf->Fill("R_{DV} > 300 mm", 1);

            // DV z <  300 mm
            if(dv_z > dv_z_max) continue;
            m_dv_mut_cf->Fill("z_{DV} > 300 mm", 1);

            // track kinematic cut
            if(!m_fmtool->PassTrackKinematic(tp1, tp2)) continue;
            m_dv_mut_cf->Fill("Track kinematic", 1);

            // plot vertex distribution
            plot_dv(*dv, *pv, channel);

            // truth match
            if (isMC){
                // create truth vertex for matching
                const xAOD::TruthVertex *tru_matched = nullptr;

                tru_matched = m_dvutils->getClosestTruthVertex(dv);
                if(tru_matched) m_dv_mut_cf->Fill("Truth matched", 1);
            }

        }

        if (channel == "et") {

            // et pair
            m_dv_et_cf->Fill("mu-trk", 1);

            // vertex fit quality
            if(!m_dvc->PassChisqCut(*dv)) continue;
            m_dv_et_cf->Fill("#chi^{2}_{DV} / DOF < 5", 1);

            // minimum distance from pv (from 0 for MC)
            if(!m_dvc->PassDistCut(*dv, *pvc)) continue;
            m_dv_et_cf->Fill("Disp. > 2 mm", 1);

            // charge requirements
            if(!m_dvc->PassChargeRequirement(*dv)) continue;
            m_dv_et_cf->Fill("#mu^{+,-}-trk^{-,+}", 1);

            // disabled module
            if(!m_dvc->PassDisabledModuleVeto(*dv)) continue;
            m_dv_et_cf->Fill("DisabledModule", 1);

            // material veto (excluding mu)
            //if(!m_dvc->PassMaterialVeto(*dv)) continue;
            //m_dv_et_cf->Fill("MaterialVeto (excluding mu)", 1);

            // low mass veto
            if(dv_mass > mass_min) {
                m_dv_et_M_high->Fill(dv_mass);                    // dimuon mass
                continue;
            }
            m_dv_et_cf->Fill("LowMassVeto", 1);

            // RPVLL filter matching
            //if(!m_dilepdvc->PassFilterMatching(*dv)) continue;
            //m_dv_et_cf->Fill("FilterMatching", 1);

            // fill cosmic veto background
            FillCosmicBkg(tp1, tp2, channel);
            
            // cosmic veto (R_CR)
            //if(!PassCosmicVeto_R_CR(tp1, tp2)) continue;
            //m_dv_et_cf->Fill("R_{CR} > 0.04", 1);

            // DV R <  300 mm
            if(dv_R > dv_R_max) continue;
            m_dv_et_cf->Fill("R_{DV} > 300 mm", 1);

            // DV z <  300 mm
            if(dv_z > dv_z_max) continue;
            m_dv_et_cf->Fill("z_{DV} > 300 mm", 1);

            // track kinematic cut
            if(!m_fmtool->PassTrackKinematic(tp1, tp2)) continue;
            m_dv_et_cf->Fill("Track kinematic", 1);

            // plot vertex distribution
            plot_dv(*dv, *pv, channel);


            // truth match
            if (isMC){
                // create truth vertex for matching
                const xAOD::TruthVertex *tru_matched = nullptr;

                tru_matched = m_dvutils->getClosestTruthVertex(dv);
                if(tru_matched) m_dv_et_cf->Fill("Truth matched", 1);
            }

        }

        if (channel == "idid") {

            // idid pair
            m_dv_idid_cf->Fill("xx", 1);

            // vertex fit quality
            if(!m_dvc->PassChisqCut(*dv)) continue;
            m_dv_idid_cf->Fill("#chi^{2}_{DV} / DOF < 5", 1);

            // minimum distance from pv (from 0 for MC)
            if(!m_dvc->PassDistCut(*dv, *pvc)) continue;
            m_dv_idid_cf->Fill("Disp. > 2 mm", 1);

            // charge requirements
            if(!m_dvc->PassChargeRequirement(*dv)) continue;
            m_dv_idid_cf->Fill("x^{+}x^{-}", 1);

            // disabled module
            if(!m_dvc->PassDisabledModuleVeto(*dv)) continue;
            m_dv_idid_cf->Fill("DisabledModule", 1);

            // material veto (excluding mu)
            //if(!m_dvc->PassMaterialVeto(*dv)) continue;
            //m_dv_idid_cf->Fill("MaterialVeto (excluding mu)", 1);

            // low mass veto
            if(dv_mass > mass_min) {
                m_dv_idid_M_high->Fill(dv_mass);                    // dimuon mass
                continue;
            }
            m_dv_idid_cf->Fill("LowMassVeto", 1);

            // fill cosmic veto background
            FillCosmicBkg(tp1, tp2, channel);
            
            // cosmic veto (R_CR)
            //if(!PassCosmicVeto_R_CR(tp1, tp2)) continue;
            //m_dv_idid_cf->Fill("R_{CR} > 0.04", 1);

            // RPVLL filter matching
            //if(!m_dilepdvc->PassFilterMatching(*dv)) continue;
            //m_dv_idid_cf->Fill("FilterMatching", 1);

            // DV R <  300 mm
            if(dv_R > dv_R_max) continue;
            m_dv_idid_cf->Fill("R_{DV} > 300 mm", 1);

            // DV z <  300 mm
            if(dv_z > dv_z_max) continue;
            m_dv_idid_cf->Fill("z_{DV} > 300 mm", 1);

            // track kinematic cut
            if(!m_fmtool->PassTrackKinematic(tp1, tp2)) continue;
            m_dv_idid_cf->Fill("Track kinematic", 1);


            //==========================================
            // plot xx distributions
            //==========================================
            plot_dv(*dv, *pv, channel);
            m_dv_idid_chi2_ndof->Fill (dv->chiSquared() / dv->numberDoF() );

            n_dv_passed_cut++;

            // truth match
            if (isMC){
                // create truth vertex for matching
                const xAOD::TruthVertex *tru_matched = nullptr;

                tru_matched = m_dvutils->getClosestTruthVertex(dv);
                if(tru_matched) m_dv_idid_cf->Fill("Truth matched", 1);
            }

        }

    } // end of dv loop







    return StatusCode::SUCCESS;
}

StatusCode LowMass::beginInputFile() { 

  return StatusCode::SUCCESS;
}

bool LowMass::PassCosmicVeto(const DataVector<xAOD::Muon> dv_muc, const DataVector<xAOD::Electron> dv_elc, std::string channel) {
    ATH_MSG_DEBUG ("Plotting cosmic veto" << name() << "...");

    bool PassCosmicVeto = true;

    float Rcos_min = 0.04;
    float deltaR_min = 0.5;
    float deltaPhiMinusPi = m_costool->getDeltaPhiMinusPi(dv_muc, dv_elc, channel);
    float sumEta = m_costool->getSumEta(dv_muc, dv_elc, channel);

    float Rcos = std::sqrt(sumEta * sumEta + deltaPhiMinusPi * deltaPhiMinusPi);
    float deltaR = m_costool->getDeltaR(dv_muc, dv_elc, channel);

    if (Rcos < Rcos_min) PassCosmicVeto = false;

    return PassCosmicVeto;
}


void LowMass::plot_signal_tp(const DataVector<xAOD::Muon> dv_muc, const DataVector<xAOD::Electron> dv_elc, std::string channel) {

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

void LowMass::plot_dv(const xAOD::Vertex& dv, const xAOD::Vertex& pv, std::string channel) {

    // access invariant mass
    float dv_mass = std::fabs(m_accMass(dv)) / 1000.; // in MeV
    ATH_MSG_DEBUG("At fill, m = " << dv_mass);

    // fill dimuon vertex
    float dv_R = m_dvutils->getR( dv, pv );                 // R in [mm]
    float dv_z = m_dvutils->getz( dv, pv );                 // z in [mm]
    float dv_l = m_dvutils->getr( dv, pv );                 // r in [mm]
    float dv_eta = m_dvutils->getEta( dv, pv );

    if (channel == "mumu"){
        m_dv_mumu_M->Fill(dv_mass);                             // dimuon mass
        m_dv_mumu_R->Fill(dv_R);                                
        m_dv_mumu_R_low->Fill(dv_R);                                
        m_dv_mumu_z->Fill(dv_z);                                
        m_dv_mumu_l->Fill(dv_l);                                
        m_dv_mumu_eta->Fill(dv_eta);                                
        m_dv_mumu_R_M->Fill(dv_R, dv_mass);
    }

    if (channel == "ee"){
        m_dv_ee_M->Fill(dv_mass);                             // dimuon mass
        m_dv_ee_R->Fill(dv_R);                                
        m_dv_ee_R_low->Fill(dv_R);                                
        m_dv_ee_z->Fill(dv_z);                                
        m_dv_ee_l->Fill(dv_l);                                
        m_dv_ee_eta->Fill(dv_eta);                                
        m_dv_ee_R_M->Fill(dv_R, dv_mass);
    }

    if (channel == "emu"){
        m_dv_emu_M->Fill(dv_mass);                             // dimuon mass
        m_dv_emu_R->Fill(dv_R);                                
        m_dv_emu_R_low->Fill(dv_R);                                
        m_dv_emu_z->Fill(dv_z);                                
        m_dv_emu_l->Fill(dv_l);                                
        m_dv_emu_eta->Fill(dv_eta);                                
        m_dv_emu_R_M->Fill(dv_R, dv_mass);
    }

    if (channel == "idid"){
        m_dv_idid_M->Fill(dv_mass);                             // dimuon mass
        m_dv_idid_R->Fill(dv_R);                                
        m_dv_idid_z->Fill(dv_z);                                
        m_dv_idid_l->Fill(dv_l);                                
        m_dv_idid_eta->Fill(dv_eta);                                
    }

    if (channel == "et"){
        m_dv_et_M->Fill(dv_mass);                             // dimuon mass
    }

    if (channel == "mut"){
        m_dv_mut_M->Fill(dv_mass);                             // dimuon mass
    }

    return;
}


bool LowMass::PassCosmicVeto_R_CR(xAOD::TrackParticle& tr0, xAOD::TrackParticle& tr1){

    bool PassCosmicVeto = true;
    float Rcos_min = 0.01;

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

bool LowMass::PassCosmicVeto_DeltaR(xAOD::TrackParticle& tr0, xAOD::TrackParticle& tr1){

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

void LowMass::FillCosmicBkg(xAOD::TrackParticle& tr0, xAOD::TrackParticle& tr1, std::string channel){

    // define TLorentzVector of decay particles
    TLorentzVector tlv_tp0;
    TLorentzVector tlv_tp1;
    tlv_tp0 = tr0.p4();
    tlv_tp1 = tr1.p4();

    float Rcos_min = 0.04;
    float deltaR_min = 0.5;

    float deltaPhiMinusPi = std::fabs(std::fabs(tlv_tp0.DeltaPhi(tlv_tp1)) - std::acos(-1.));
    float sumEta = tlv_tp0.Eta() + tlv_tp1.Eta();
    float Rcos = std::sqrt(sumEta * sumEta + deltaPhiMinusPi * deltaPhiMinusPi);
    float deltaR = tlv_tp0.DeltaR(tlv_tp1);


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
        // plot delta R vs R_cr
        m_dv_mumu_DeltaR_Rcos->Fill(deltaR,Rcos);
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

    if (channel == "idid"){
        // plot Rcos after applying deltaR < deltaR_min
        if (deltaR > deltaR_min){
            m_dv_idid_Rcos->Fill(Rcos);
            m_dv_idid_Rcos_low->Fill(Rcos);
        }

        // plot delta R after applying Rcos < Rcos_min
        if (Rcos > Rcos_min){
            m_dv_idid_DeltaR->Fill(deltaR);
            m_dv_idid_DeltaR_low->Fill(deltaR);
        }
        // plot delta R vs R_cr
        m_dv_idid_DeltaR_Rcos->Fill(deltaR,Rcos);
    }


    return;
}


