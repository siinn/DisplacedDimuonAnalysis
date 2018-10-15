///------------------------------------------------------------
// Author : Siinn Che, siinn.che@cern.ch
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
#include "TEfficiency.h"




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
m_phmatch("DDL::PhotonMatch/PhotonMatch"),
m_accMass("mass"),
m_vxwght("DDL::VxWeights/VxWeights")
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
    declareProperty("PhotonMatch", m_phmatch);
    declareProperty("VxWeights", m_vxwght);

    declareProperty("usePRW", m_usePRW = false );
}


DVEfficiency::~DVEfficiency() {}


StatusCode DVEfficiency::initialize() {

    // initialize tools
    ATH_CHECK(m_dvutils.retrieve());
    ATH_CHECK(m_leptool.retrieve());
    ATH_CHECK(m_costool.retrieve());
    ATH_CHECK(m_evtc.retrieve());
    ATH_CHECK(m_dvc.retrieve());
    ATH_CHECK(m_grlTool.retrieve());
    ATH_CHECK(m_tdt.retrieve());
    ATH_CHECK(m_cos.retrieve());
    ATH_CHECK(m_or.retrieve());
    ATH_CHECK(m_prw.retrieve());
    ATH_CHECK(m_phmatch.retrieve());



    ATH_MSG_INFO ("Initializing " << name() << "...");

    //--------------------------------------------------------
    // Event cut
    //--------------------------------------------------------

    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    // cutflow
    m_dv_eff_cutflow = new TH1D("dv cut flow", "dv cut flow", 23,0,23);

    m_dv_mass = new TH1F( "m_dv_mass", "Invariant mass of all signal vertex", 50, 0, 1500 ); // GeV

    // efficiency plots
    // custom binning
    Double_t m_eta_bins_d[] = {0,0.5,1.0,1.5,2.5,4.0};
    Double_t m_pt_bins_d[] = {0,100,200,300,400,500,600,700,800,900,1000};
    Double_t m_R_bins_d[] = {0,100,150,200,250,300};
    
    m_dv_eff_eta = new TH1F( "m_dv_eff_eta", "DV reconstruction efficiency vs eta ", 20, -2.5, 2.5);
    m_dv_eff_phi = new TH1F( "m_dv_eff_phi", "DV reconstruction efficiency vs phi ", 20, -M_PI, M_PI);
    m_dv_eff_R = new TH1F( "m_dv_eff_R", "DV reconstruction efficiency vs R ", 10, 0,300); // mm
    m_dv_eff_z = new TH1F( "m_dv_eff_z", "DV reconstruction efficiency vs z ", 20, -300, 300); // mm
    m_dv_eff_mass = new TH1F( "m_dv_eff_mass", "DV reconstruction efficiency vs mass ", 150, 0, 1500); // GeV
    m_dv_eff_d0 = new TH1F( "m_dv_eff_d0", "DV reconstruction efficiency vs d0 ", 30, 0, 400); // mm
    m_dv_eff_DeltaR = new TH1F( "m_dv_eff_DeltaR", "DV reconstruction efficiency vs DeltaR ", 10, 0, 2*M_PI); // mm

    m_dv_eff_eta_den = new TH1F( "m_dv_eff_eta_den", "DV reconstruction efficiency vs eta (den)", 20, -2.5, 2.5);
    m_dv_eff_phi_den = new TH1F( "m_dv_eff_phi_den", "DV reconstruction efficiency vs phi (den)", 20, -M_PI, M_PI);
    m_dv_eff_R_den = new TH1F( "m_dv_eff_R_den", "DV reconstruction efficiency vs R (den)", 10, 0,300); // mm
    m_dv_eff_z_den = new TH1F( "m_dv_eff_z_den", "DV reconstruction efficiency vs z (den)", 20, -300, 300); // mm
    m_dv_eff_mass_den = new TH1F( "m_dv_eff_mass_den", "DV reconstruction efficiency vs mass (den)", 150, 0, 1500); // GeV
    m_dv_eff_d0_den = new TH1F( "m_dv_eff_d0_den", "DV reconstruction efficiency vs d0 (den)", 30, 0, 400); // mm
    m_dv_eff_DeltaR_den = new TH1F( "m_dv_eff_DeltaR_den", "DV reconstruction efficiency vs DeltaR (den)", 10, 0, 2*M_PI); // mm

    m_dv_eff_eta_num = new TH1F( "m_dv_eff_eta_num", "DV reconstruction efficiency vs eta (num)", 20, -2.5, 2.5);
    m_dv_eff_phi_num = new TH1F( "m_dv_eff_phi_num", "DV reconstruction efficiency vs phi (num)", 20, -M_PI, M_PI);
    m_dv_eff_R_num = new TH1F( "m_dv_eff_R_num", "DV reconstruction efficiency vs R (num)", 10, 0,300); // mm
    m_dv_eff_z_num = new TH1F( "m_dv_eff_z_num", "DV reconstruction efficiency vs z (num)", 20, -300, 300); // mm
    m_dv_eff_mass_num = new TH1F( "m_dv_eff_mass_num", "DV reconstruction efficiency vs mass (num)", 150, 0, 1500); // GeV
    m_dv_eff_d0_num = new TH1F( "m_dv_eff_d0_num", "DV reconstruction efficiency vs d0 (num)", 30, 0, 400); // mm
    m_dv_eff_DeltaR_num = new TH1F( "m_dv_eff_DeltaR_num", "DV reconstruction efficiency vs DeltaR (num)", 10, 0, 2*M_PI); // mm

    // efficiency as a function of Z' parameters
    m_dv_eff_zp_eta = new TH1F("m_dv_eff_zp_eta", " DV reconstruction efficiency vs Z' eta", 5, m_eta_bins_d);
    m_dv_eff_zp_beta = new TH1F("m_dv_eff_zp_beta", " DV reconstruction efficiency vs Z' beta", 25, 0, 1.0);
    m_dv_eff_zp_pt = new TH1F("m_dv_eff_zp_pt", " DV reconstruction efficiency vs Z' pt", 10, m_pt_bins_d); // GeV

    m_dv_eff_zp_eta_den = new TH1F("m_dv_eff_zp_eta_den", "(den) DV reconstruction efficiency vs Z' eta", 5, m_eta_bins_d);
    m_dv_eff_zp_beta_den = new TH1F("m_dv_eff_zp_beta_den", "(den) DV reconstruction efficiency vs Z' beta", 25, 0, 1.0);
    m_dv_eff_zp_pt_den = new TH1F("m_dv_eff_zp_pt_den", "(den) DV reconstruction efficiency vs Z' pt", 10, m_pt_bins_d); // GeV

    m_dv_eff_zp_eta_num = new TH1F("m_dv_eff_zp_eta_num", "(num) DV reconstruction efficiency vs Z' eta", 5, m_eta_bins_d);
    m_dv_eff_zp_beta_num = new TH1F("m_dv_eff_zp_beta_num", "(num) DV reconstruction efficiency vs Z' beta", 25, 0, 1.0);
    m_dv_eff_zp_pt_num = new TH1F("m_dv_eff_zp_pt_num", "(num) DV reconstruction efficiency vs Z' pt", 10, m_pt_bins_d); // GeV

    // efficiency as a function of mu
    m_dv_eff_mu = new TH1F("m_dv_eff_mu", "DV reconstruction efficiency vs #mu ", 50, 0, 50);
    m_dv_eff_mu_den = new TH1F("m_dv_eff_mu_den", "DV reconstruction efficiency vs #mu (den)", 50, 0, 50);
    m_dv_eff_mu_num = new TH1F("m_dv_eff_mu_num", "DV reconstruction efficiency vs #mu (num)", 50, 0, 50);

    // trigger effieicny test
    m_dv_eff_trig = new TH1F("m_dv_eff_trig", "Trigger efficiency", 4, 0, 5);
    m_dv_eff_trig_den = new TH1F("m_dv_eff_trig_den", "Trigger efficiency (den)", 4, 0, 5);
    m_dv_eff_trig_num = new TH1F("m_dv_eff_trig_num", "Trigger efficiency (num)", 4, 0, 5);


    // efficiency map (Z' pt and eta)
    // custom binning
    Float_t m_eta_bins[] = {0,0.5,1.0,1.5,2.5,4.0};
    //Float_t m_pt_bins[] = {0,150,300,500,1000};
    Float_t m_pt_bins[] = {0,100,200,300,400,500,600,700,800,900,1000};

    m_dv_eff_map_mu_eta_den = new TH2F("m_dv_eff_map_mu_eta_den", "DV reconstruction efficiency map, mu vs eta (den)", 50, 0., 50., 10, 0, 3.0); // GeV
    m_dv_eff_map_mu_eta_num = new TH2F("m_dv_eff_map_mu_eta_num", "DV reconstruction efficiency map, mu vs eta (num)", 50, 0., 50., 10, 0, 3.0); // GeV
    m_dv_eff_map_mu_eta = new TH2F("m_dv_eff_map_mu_eta", "DV reconstruction efficiency map, mu vs eta", 50, 0., 50., 10, 0, 3.0); // GeV

    m_dv_eff_map_pt_eta_den = new TH2F("m_dv_eff_map_pt_eta_den", "DV reconstruction efficiency map, pt vs eta (den)", 10, m_pt_bins, 5, m_eta_bins); // GeV
    m_dv_eff_map_pt_eta_num = new TH2F("m_dv_eff_map_pt_eta_num", "DV reconstruction efficiency map, pt vs eta (num)", 10, m_pt_bins, 5, m_eta_bins); // GeV
    m_dv_eff_map_pt_eta = new TH2F("m_dv_eff_map_pt_eta", "DV reconstruction efficiency map, pt vs eta", 10, m_pt_bins, 5,m_eta_bins); // GeV

    // pile up systematic variation, up 
    m_dv_eff_map_up_pt_eta_den = new TH2F("m_dv_eff_map_up_pt_eta_den", "DV reconstruction efficiency map_up, pt vs eta (den)", 10, m_pt_bins, 5, m_eta_bins); // GeV
    m_dv_eff_map_up_pt_eta_num = new TH2F("m_dv_eff_map_up_pt_eta_num", "DV reconstruction efficiency map_up, pt vs eta (num)", 10, m_pt_bins, 5, m_eta_bins); // GeV
    m_dv_eff_map_up_pt_eta = new TH2F("m_dv_eff_map_up_pt_eta", "DV reconstruction efficiency map_up, pt vs eta", 10, m_pt_bins, 5, m_eta_bins); // GeV

    // pile up systematic variation, down 
    m_dv_eff_map_down_pt_eta_den = new TH2F("m_dv_eff_map_down_pt_eta_den", "DV reconstruction efficiency map_down, pt vs eta (den)", 10, m_pt_bins, 5, m_eta_bins); // GeV
    m_dv_eff_map_down_pt_eta_num = new TH2F("m_dv_eff_map_down_pt_eta_num", "DV reconstruction efficiency map_down, pt vs eta (num)", 10, m_pt_bins, 5, m_eta_bins); // GeV
    m_dv_eff_map_down_pt_eta = new TH2F("m_dv_eff_map_down_pt_eta", "DV reconstruction efficiency map_down, pt vs eta", 10, m_pt_bins, 5, m_eta_bins); // GeV


    // output
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv_eff_cutflow;", m_dv_eff_cutflow) );
    CHECK( histSvc->regHist("/DV/truth/dv_mass", m_dv_mass) );

    // efficiency plots
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_eta", m_dv_eff_eta));
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_phi", m_dv_eff_phi));
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_mass",m_dv_eff_mass));
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_R",m_dv_eff_R)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_z",m_dv_eff_z)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_d0",m_dv_eff_d0)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_DeltaR",m_dv_eff_DeltaR)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_zp_eta",m_dv_eff_zp_eta)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_zp_beta",m_dv_eff_zp_beta)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_zp_pt",m_dv_eff_zp_pt)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv_eff_mu",m_dv_eff_mu)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv_eff_trig", m_dv_eff_trig) );

    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_eta_den", m_dv_eff_eta_den));
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_phi_den", m_dv_eff_phi_den));
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_mass_den",m_dv_eff_mass_den));
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_R_den",m_dv_eff_R_den)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_z_den",m_dv_eff_z_den)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_d0_den",m_dv_eff_d0_den)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_DeltaR_den",m_dv_eff_DeltaR_den)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_zp_eta_den",m_dv_eff_zp_eta_den)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_zp_beta_den",m_dv_eff_zp_beta_den)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_zp_pt_den",m_dv_eff_zp_pt_den)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv_eff_mu_den",m_dv_eff_mu_den)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv_eff_trig_den", m_dv_eff_trig_den) );

    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_eta_num", m_dv_eff_eta_num));
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_phi_num", m_dv_eff_phi_num));
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_mass_num",m_dv_eff_mass_num));
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_R_num",m_dv_eff_R_num)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_z_num",m_dv_eff_z_num)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_d0_num",m_dv_eff_d0_num)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv/dv_eff_DeltaR_num",m_dv_eff_DeltaR_num)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_zp_eta_num",m_dv_eff_zp_eta_num)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_zp_beta_num",m_dv_eff_zp_beta_num)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_zp_pt_num",m_dv_eff_zp_pt_num)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv_eff_mu_num",m_dv_eff_mu_num)) ;
    CHECK( histSvc->regHist("/DV/truth/efficiency/dv_eff_trig_num", m_dv_eff_trig_num) );

    // efficiency map
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_mu_eta", m_dv_eff_map_mu_eta) );
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_mu_eta_den", m_dv_eff_map_mu_eta_den) ); // for error calculation
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_mu_eta_num", m_dv_eff_map_mu_eta_num) );

    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_pt_eta", m_dv_eff_map_pt_eta) );
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_pt_eta_den", m_dv_eff_map_pt_eta_den) );
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_pt_eta_num", m_dv_eff_map_pt_eta_num) );

    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_up_pt_eta", m_dv_eff_map_up_pt_eta) );
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_up_pt_eta_den", m_dv_eff_map_up_pt_eta_den) );
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_up_pt_eta_num", m_dv_eff_map_up_pt_eta_num) );

    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_down_pt_eta", m_dv_eff_map_down_pt_eta) );
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_down_pt_eta_den", m_dv_eff_map_down_pt_eta_den) );
    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_down_pt_eta_num", m_dv_eff_map_down_pt_eta_num) );



    return StatusCode::SUCCESS;
}

StatusCode DVEfficiency::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");

    // store sum of squares of weights
    m_dv_eff_eta_den->Sumw2();
    m_dv_eff_phi_den->Sumw2();
    m_dv_eff_mass_den->Sumw2();
    m_dv_eff_R_den->Sumw2();
    m_dv_eff_z_den->Sumw2();
    m_dv_eff_d0_den->Sumw2();
    m_dv_eff_zp_eta_den->Sumw2();
    m_dv_eff_zp_beta_den->Sumw2();
    m_dv_eff_zp_pt_den->Sumw2();
    m_dv_eff_mu_den->Sumw2();
    m_dv_eff_trig_den->Sumw2();
    
    m_dv_eff_eta_num->Sumw2();
    m_dv_eff_phi_num->Sumw2();
    m_dv_eff_mass_num->Sumw2();
    m_dv_eff_R_num->Sumw2();
    m_dv_eff_z_num->Sumw2();
    m_dv_eff_d0_num->Sumw2();
    m_dv_eff_zp_eta_num->Sumw2();
    m_dv_eff_zp_beta_num->Sumw2();
    m_dv_eff_zp_pt_num->Sumw2();
    m_dv_eff_mu_num->Sumw2();
    m_dv_eff_trig_num->Sumw2();
    
    // store sum of squares of weights
    m_dv_eff_map_pt_eta_num->Sumw2();
    m_dv_eff_map_pt_eta_den->Sumw2();
    m_dv_eff_map_mu_eta_num->Sumw2();
    m_dv_eff_map_mu_eta_den->Sumw2();
    
    // systematic variation
    m_dv_eff_map_up_pt_eta_num->Sumw2();
    m_dv_eff_map_up_pt_eta_den->Sumw2();
    m_dv_eff_map_down_pt_eta_num->Sumw2();
    m_dv_eff_map_down_pt_eta_den->Sumw2();

    // calculate efficiency
    m_dv_eff_eta->Divide(m_dv_eff_eta_num, m_dv_eff_eta_den,1,1, "b");
    m_dv_eff_phi->Divide(m_dv_eff_phi_num, m_dv_eff_phi_den,1,1, "b");
    m_dv_eff_mass->Divide(m_dv_eff_mass_num, m_dv_eff_mass_den,1,1, "b");
    m_dv_eff_R->Divide(m_dv_eff_R_num, m_dv_eff_R_den,1,1, "b");
    m_dv_eff_z->Divide(m_dv_eff_z_num, m_dv_eff_z_den,1,1, "b");
    m_dv_eff_d0->Divide(m_dv_eff_d0_num, m_dv_eff_d0_den,1,1, "b");
    m_dv_eff_zp_eta->Divide(m_dv_eff_zp_eta_num, m_dv_eff_zp_eta_den,1,1, "b");
    m_dv_eff_zp_beta->Divide(m_dv_eff_zp_beta_num, m_dv_eff_zp_beta_den,1,1, "b");
    m_dv_eff_zp_pt->Divide(m_dv_eff_zp_pt_num, m_dv_eff_zp_pt_den,1,1, "b");
    m_dv_eff_mu->Divide(m_dv_eff_mu_num, m_dv_eff_mu_den,1,1, "b");
    m_dv_eff_trig->Divide(m_dv_eff_trig_num, m_dv_eff_trig_den,1,1, "b");
    
    m_dv_eff_map_pt_eta->Divide(m_dv_eff_map_pt_eta_num, m_dv_eff_map_pt_eta_den,1,1, "b");
    m_dv_eff_map_mu_eta->Divide(m_dv_eff_map_mu_eta_num, m_dv_eff_map_mu_eta_den,1,1, "b");

    // systematic variation
    m_dv_eff_map_up_pt_eta->Divide(m_dv_eff_map_up_pt_eta_num, m_dv_eff_map_up_pt_eta_den,1,1, "b");
    m_dv_eff_map_down_pt_eta->Divide(m_dv_eff_map_down_pt_eta_num, m_dv_eff_map_down_pt_eta_den,1,1, "b");

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
    int mu = 0;
    if (m_usePRW) {

        // systematic variation of pileup weight
        CP::SystematicSet s;

        // get pileup weight
        s.insert( CP::SystematicVariation("PRW_DATASF",0) );
        m_prw->applySystematicVariation(s);
        p_weight = m_prw->getCombinedWeight(*evtInfo);

        // scale up data mu
        s.clear();
        s.insert( CP::SystematicVariation("PRW_DATASF",1) );
        m_prw->applySystematicVariation(s);
        p_weight_up = m_prw->getCombinedWeight( *evtInfo );

        // scale down data mu
        s.clear();
        s.insert(CP::SystematicVariation("PRW_DATASF",-1) );
        m_prw->applySystematicVariation(s);
        p_weight_down = m_prw->getCombinedWeight( *evtInfo );

        ATH_MSG_DEBUG("p up = " << p_weight_up << ", p down = " << p_weight_down << ", p = " << p_weight);

        // pile-up
        mu = evtInfo->actualInteractionsPerCrossing();
    }

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

    // perform matching of photons to electrons
    m_phmatch->MatchPhotons(*phc, *elc_copy.first);

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
    m_dv_eff_cutflow->Fill("All events",0);
    m_dv_eff_cutflow->Fill("GRL (Data)",0);
    m_dv_eff_cutflow->Fill("EventCleaning (Data)",0);
    m_dv_eff_cutflow->Fill("TrigFilter",0);
    m_dv_eff_cutflow->Fill("z_{PV} < 200 mm",0);

    // event selection flag
    bool event_passed = true;

    // all events
    m_dv_eff_cutflow->Fill("All events",1);

    // GRL
    if (!isMC and !m_grlTool->passRunLB(*evtInfo)) event_passed = false;
    if (event_passed) m_dv_eff_cutflow->Fill("GRL (Data)",p_weight);

    // event cleaning
    if (!m_evtc->PassEventCleaning(*evtInfo)) event_passed = false;
    if (event_passed) m_dv_eff_cutflow->Fill("EventCleaning (Data)",p_weight);

    // trigger filter
    bool trig_passed = false;
   
    // fill denominator 
    m_dv_eff_trig_den->Fill("HLT_mu60_0eta105_msonly",p_weight);
    m_dv_eff_trig_den->Fill("HLT_g140_loose",p_weight);
    m_dv_eff_trig_den->Fill("HLT_2g50_loose",p_weight);
    m_dv_eff_trig_den->Fill("Combined",p_weight);

    // place holder
    m_dv_eff_trig_num->Fill("HLT_mu60_0eta105_msonly",0);
    m_dv_eff_trig_num->Fill("HLT_g140_loose",0);
    m_dv_eff_trig_num->Fill("HLT_2g50_loose",0);
    m_dv_eff_trig_num->Fill("Combined",0);

    //---------------------------------------------------------
    // check trigger and fill histogram for trigger efficiency
    //---------------------------------------------------------
    if (m_tdt->isPassed("HLT_mu60_0eta105_msonly")) {
        m_dv_eff_trig_num->Fill("HLT_mu60_0eta105_msonly",p_weight);
        trig_passed = true;
        }
    if (m_tdt->isPassed("HLT_g140_loose")) {
        m_dv_eff_trig_num->Fill("HLT_g140_loose",p_weight);
        trig_passed = true;
        }
    if (m_tdt->isPassed("HLT_2g50_loose")) {
        m_dv_eff_trig_num->Fill("HLT_2g50_loose",p_weight);
        trig_passed = true;
        }

    if(trig_passed) {
        m_dv_eff_trig_num->Fill("Combined",p_weight);
    }
    else {
        event_passed = false;
    }
    //---------------------------------------------------------

    // trigger filter
    if (event_passed) m_dv_eff_cutflow->Fill("TrigFilter",p_weight);

    // cosmic veto
    if(!m_cos->PassCosmicEventVeto(*elc, *muc)) event_passed = false;
    
    if (event_passed) m_dv_eff_cutflow->Fill("CosmicVeto",p_weight);
    else m_dv_eff_cutflow->Fill("CosmicVeto", 0);

    // PV position < 200 mm
    float pv_z_max = 200.;

    // apply primary vertex position cut
    if (pv) {

        // get primary vertex position
        auto pv_pos = pv->position();

        // z_pv cut
        if(std::abs(pv_pos.z()) > pv_z_max) event_passed = false;
    }
    else event_passed = false;

    if (event_passed) m_dv_eff_cutflow->Fill("z_{PV} < 200 mm",p_weight);


    //=================================================
    // vertex cut flow
    //=================================================
    // set to true if vertex pass all selection
    bool dv_matched = false;

    // cut flow
    for(auto dv: *dvc_copy.first) {

        // flag to mark if this DV passed cuts
        bool vertex_passed = true;

        // vertex found
        m_dv_eff_cutflow->Fill("Vertex",p_weight);

        // access tracks from vertex
        auto tpLinks = dv->trackParticleLinks();
        xAOD::TrackParticle tp1 = **(tpLinks.at(0));
        xAOD::TrackParticle tp2 = **(tpLinks.at(1));

        // access invariant mass
        float dv_mass = std::fabs(m_accMass(*dv)) / 1000.; // in GeV

        // mass and position cut
        float mass_min = 12.;
        float dv_R_max = 300;
        float dv_z_max = 300;

        // perform lepton matching
        m_dilepdvc->ApplyLeptonMatching(*dv, *elc_copy.first, *muc_copy.first);

        // remove overlapping muon
        m_dilepdvc->ApplyOverlapRemoval(*dv);

        // trigger matching
        m_dilepdvc->DoTriggerMatching(*dv);

        // perform filter matching
        m_dilepdvc->DoFilterMatching(*dv);

        // muon selection tool
        m_leptool->MuonSelection(*dv);

        // remove bad electrons
        m_leptool->BadClusterRemoval(*dv);

        // kinematic cut
        m_leptool->ElectronKinematicCut(*dv);

        // Electron identification
        m_leptool->ElectronID(*dv);

        // get position of DV
        float dv_R = m_dvutils->getR( *dv, *pv );                 // R in [mm]
        float dv_z = std::abs(m_dvutils->getz( *dv, *pv ));       // z in [mm]

        // posotion w.r.t beam spot
        float dv_R_wrt_beam = (*dv).position().perp();    // R in [mm]
        float dv_z_wrt_beam = (*dv).position().z();       // z in [mm]

        // select only vertex with tracks
        if(dv->trackParticleLinks().size() != 2) vertex_passed = false;

        // find decay channel of dv
        std::string channel = m_dvutils->DecayChannel(*dv);

        // only select mumu, ee, or emu
        if (!((channel == "mumu") or (channel == "emu") or (channel == "ee"))) vertex_passed = false;
        if(vertex_passed) m_dv_eff_cutflow->Fill("LeptonID",p_weight);

        // Trigger matching
        //if(!m_dvutils->TrigMatching(*dv)) vertex_passed = false;
        if(!m_dilepdvc->PassTriggerMatching(*dv)) vertex_passed = false;
        if(vertex_passed) m_dv_eff_cutflow->Fill("Trig.Matching",p_weight);

        // vertex fit quality
        if(!m_dvc->PassChisqCut(*dv)) vertex_passed = false;
        if(vertex_passed) m_dv_eff_cutflow->Fill("#chi^{2} / DOF",p_weight);

        // minimum distance from pv (from 0 for MC)
        if(!m_dvc->PassDistCut(*dv, *pvc)) vertex_passed = false;
        if(vertex_passed) m_dv_eff_cutflow->Fill("Disp. > 2 mm",p_weight);

        // charge requirements
        if(!m_dvc->PassChargeRequirement(*dv)) vertex_passed = false;
        if(vertex_passed) m_dv_eff_cutflow->Fill("Oppo.Charge",p_weight);

        // disabled module
        if(!m_dvc->PassDisabledModuleVeto(*dv)) vertex_passed = false;
        if(vertex_passed) m_dv_eff_cutflow->Fill("DisabledModule",p_weight);

        if ((channel == "emu") or (channel == "ee")) {
            if(!m_dvc->PassMaterialVeto(*dv)) vertex_passed = false;
        }
        if(vertex_passed) m_dv_eff_cutflow->Fill("MaterialVeto",p_weight);

        // low mass veto
        if(dv_mass < mass_min) vertex_passed = false;
        if(vertex_passed) m_dv_eff_cutflow->Fill("LowMass",p_weight);

        // cosmic veto (R_CR)
        //if(!PassCosmicVeto_R_CR(tp1, tp2)) vertex_passed = false;
        //if(vertex_passed) m_dv_eff_cutflow->Fill("R_{CR} > 0.01",p_weight);

        // RPVLL filter matching
        if(!m_dilepdvc->PassFilterMatching(*dv)) vertex_passed = false;
        if(vertex_passed) m_dv_eff_cutflow->Fill("FilterMatching",p_weight);

        // DV R <  300 mm
        if(dv_R_wrt_beam > dv_R_max) vertex_passed = false;
        if(vertex_passed) m_dv_eff_cutflow->Fill("R_{DV} > 300 mm",p_weight);

        // DV z <  300 mm
        if(std::abs(dv_z_wrt_beam) > dv_z_max) vertex_passed = false;
        if(vertex_passed) m_dv_eff_cutflow->Fill("z_{DV} > 300 mm",p_weight);

        // find closest truth vertex by position
        if(vertex_passed) {
            const xAOD::TruthVertex* matched_truth = m_dvutils->getClosestTruthVertex(dv, true);
            if(matched_truth) {

                // fill truth matched
                m_dv_eff_cutflow->Fill("Truth matched",p_weight);

                // mark this event as reconstructed
                matched_truth->auxdecor<bool>("reconstructed") = 1;

                // collect leptons from this dv
                auto dv_muc = m_accMu(*dv);
                auto dv_elc = m_accEl(*dv);

                // get scale factors
                auto trigsf = m_vxwght->GetTrigSF(*dv_elc, *dv_muc);

                // decorate truth vertex with trigger scale factor
                matched_truth->auxdecor<bool>("trigger_sc") = trigsf.at(DDL::Syst::Nom);

                // fill truth matched
                m_dv_eff_cutflow->Fill("Trigger SF",p_weight*trigsf.at(DDL::Syst::Nom));
            }
        }

    } // end of dv loop
    
    // if event didn't pass event selection, mark as not reconstructed
    //if (!event_passed) dv_matched = false;

    //-----------------------------------------------------------------
    // End of cut flow.
    // We use dv_matched flag to create efficiency plots and maps below.
    //-----------------------------------------------------------------

    // retrieve truth vertex container
    const xAOD::TruthVertexContainer* tru_vc = nullptr;
    evtStore()->retrieve( tru_vc, "TruthVertices");

    // loop over the truth vertex container
    for (auto tru_v: *tru_vc){

        // only selecting signal truth
        if (!m_dvutils->isSignalVertex(tru_v)) continue;

        // check if this truth dv is reconstructed
        bool dv_matched = (tru_v->auxdecor<bool>("reconstructed") && event_passed);

        // retrieve trigger scale factor
        float trigger_sc = 1.0;
        if(dv_matched) float trigger_sc = tru_v->auxdecor<bool>("trigger_sc");

        // fill truth signal vertex mass
        float DVMass = m_dvutils->TruthMass(tru_v) / 1000.;
        m_dv_mass->Fill(DVMass);

        // get opening angle
        float dv_dr = m_dvutils->Truth_dr(tru_v);

        // Z' parameters
        float zp_eta = std::abs(tru_v->incomingParticle(0)->eta());
        float zp_pt = tru_v->incomingParticle(0)->pt() / 1000.; // GeV
        float zp_m = tru_v->incomingParticle(0)->m();
        float zp_e = tru_v->incomingParticle(0)->e();
        float zp_beta = sqrt(1 - (zp_m/zp_e)*(zp_m/zp_e));
     
        // fill denominator with generator weight
        m_dv_eff_eta_den->Fill(tru_v->eta(), 1);
        m_dv_eff_phi_den->Fill(tru_v->phi(), 1);
        m_dv_eff_mass_den->Fill(DVMass, 1);
        m_dv_eff_R_den->Fill(tru_v->perp(), 1);
        m_dv_eff_z_den->Fill(tru_v->z(), 1);
        m_dv_eff_d0_den->Fill(m_dvutils->GetMaxd0(tru_v), 1);
        m_dv_eff_DeltaR_den->Fill(dv_dr, 1);

        m_dv_eff_zp_eta_den->Fill(zp_eta, 1);
        m_dv_eff_zp_beta_den->Fill(zp_beta, 1);
        m_dv_eff_zp_pt_den->Fill(zp_pt, 1);
        m_dv_eff_mu_den->Fill(mu, 1);

        // efficiency map
        m_dv_eff_map_pt_eta_den->Fill(zp_pt, zp_eta, 1);
        m_dv_eff_map_mu_eta_den->Fill(mu, zp_eta, 1);

        // systematic variation
        m_dv_eff_map_up_pt_eta_den->Fill(zp_pt, zp_eta, 1);
        m_dv_eff_map_down_pt_eta_den->Fill(zp_pt, zp_eta, 1);

        // if matched, use weights to fill numerator
        if (dv_matched) {
            m_dv_eff_eta_num->Fill(tru_v->eta(), p_weight*trigger_sc);
            m_dv_eff_phi_num->Fill(tru_v->phi(), p_weight*trigger_sc);
            m_dv_eff_mass_num->Fill(DVMass, p_weight*trigger_sc);
            m_dv_eff_R_num->Fill(tru_v->perp(), p_weight*trigger_sc);
            m_dv_eff_z_num->Fill(tru_v->z(), p_weight*trigger_sc);
            m_dv_eff_d0_num->Fill(m_dvutils->GetMaxd0(tru_v), p_weight*trigger_sc);
            m_dv_eff_DeltaR_num->Fill(dv_dr, p_weight*trigger_sc);

            m_dv_eff_zp_eta_num->Fill(zp_eta, p_weight*trigger_sc);
            m_dv_eff_zp_beta_num->Fill(zp_beta, p_weight*trigger_sc);
            m_dv_eff_zp_pt_num->Fill(zp_pt, p_weight*trigger_sc);
            m_dv_eff_mu_num->Fill(mu, p_weight*trigger_sc);

            // efficiency map
            m_dv_eff_map_pt_eta_num->Fill(zp_pt, zp_eta, p_weight*trigger_sc); 
            m_dv_eff_map_mu_eta_num->Fill(mu, zp_eta, p_weight*trigger_sc); 

            // systematic variation
            m_dv_eff_map_up_pt_eta_num->Fill(zp_pt, zp_eta, p_weight_up); 
            m_dv_eff_map_down_pt_eta_num->Fill(zp_pt, zp_eta, p_weight_down); 
        }


    } // end of efficiency loop


    return StatusCode::SUCCESS;
}


bool DVEfficiency::PassCosmicVeto_R_CR(xAOD::TrackParticle& tr0, xAOD::TrackParticle& tr1){

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

