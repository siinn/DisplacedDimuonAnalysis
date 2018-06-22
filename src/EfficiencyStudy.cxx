//------------------------------------------------------------
// Author : Siinn Che, siinn.che@cern.ch
//------------------------------------------------------------

// DisplacedDimuonAnalysis includes
#include "EfficiencyStudy.h"
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




EfficiencyStudy::EfficiencyStudy( const std::string& name, ISvcLocator* pSvcLocator ) : 
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
    declareProperty("PhotonMatch", m_phmatch);

    declareProperty("usePRW", m_usePRW = false );
}


EfficiencyStudy::~EfficiencyStudy() {}


StatusCode EfficiencyStudy::initialize() {

    // initialize tools
    ATH_CHECK(m_dilepdvc.retrieve());
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
    m_dv_reco_cutflow = new TH1D("dv cut flow", "dv cut flow", 19,0,19);

    // efficiency at different eta
    //Double_t m_pt_bins_double[] = {0,100,200,300,400,500,1000};
    Double_t m_pt_bins_double[] = {0,150,300,500,1000};
    m_dv_eff_pt_eta1_mass1 = new TEfficiency("m_dv_eff_pt_eta1_mass1", "DV reconstruction efficiency vs LLP pt (eta1, mass1)", 4,m_pt_bins_double); // GeV
    m_dv_eff_pt_eta2_mass1 = new TEfficiency("m_dv_eff_pt_eta2_mass1", "DV reconstruction efficiency vs LLP pt (eta2, mass1)", 4,m_pt_bins_double); // GeV
    m_dv_eff_pt_eta3_mass1 = new TEfficiency("m_dv_eff_pt_eta3_mass1", "DV reconstruction efficiency vs LLP pt (eta3, mass1)", 4,m_pt_bins_double); // GeV
    m_dv_eff_pt_eta1_mass2 = new TEfficiency("m_dv_eff_pt_eta1_mass2", "DV reconstruction efficiency vs LLP pt (eta1, mass2)", 4,m_pt_bins_double); // GeV
    m_dv_eff_pt_eta2_mass2 = new TEfficiency("m_dv_eff_pt_eta2_mass2", "DV reconstruction efficiency vs LLP pt (eta2, mass2)", 4,m_pt_bins_double); // GeV
    m_dv_eff_pt_eta3_mass2 = new TEfficiency("m_dv_eff_pt_eta3_mass2", "DV reconstruction efficiency vs LLP pt (eta3, mass2)", 4,m_pt_bins_double); // GeV
    m_dv_eff_pt_eta1_mass3 = new TEfficiency("m_dv_eff_pt_eta1_mass3", "DV reconstruction efficiency vs LLP pt (eta1, mass3)", 4,m_pt_bins_double); // GeV
    m_dv_eff_pt_eta2_mass3 = new TEfficiency("m_dv_eff_pt_eta2_mass3", "DV reconstruction efficiency vs LLP pt (eta2, mass3)", 4,m_pt_bins_double); // GeV
    m_dv_eff_pt_eta3_mass3 = new TEfficiency("m_dv_eff_pt_eta3_mass3", "DV reconstruction efficiency vs LLP pt (eta3, mass3)", 4,m_pt_bins_double); // GeV
    m_dv_eff_pt_eta1_mass4 = new TEfficiency("m_dv_eff_pt_eta1_mass4", "DV reconstruction efficiency vs LLP pt (eta1, mass4)", 4,m_pt_bins_double); // GeV
    m_dv_eff_pt_eta2_mass4 = new TEfficiency("m_dv_eff_pt_eta2_mass4", "DV reconstruction efficiency vs LLP pt (eta2, mass4)", 4,m_pt_bins_double); // GeV
    m_dv_eff_pt_eta3_mass4 = new TEfficiency("m_dv_eff_pt_eta3_mass4", "DV reconstruction efficiency vs LLP pt (eta3, mass4)", 4,m_pt_bins_double); // GeV
    m_dv_eff_pt_eta1_mass5 = new TEfficiency("m_dv_eff_pt_eta1_mass5", "DV reconstruction efficiency vs LLP pt (eta1, mass5)", 4,m_pt_bins_double); // GeV
    m_dv_eff_pt_eta2_mass5 = new TEfficiency("m_dv_eff_pt_eta2_mass5", "DV reconstruction efficiency vs LLP pt (eta2, mass5)", 4,m_pt_bins_double); // GeV
    m_dv_eff_pt_eta3_mass5 = new TEfficiency("m_dv_eff_pt_eta3_mass5", "DV reconstruction efficiency vs LLP pt (eta3, mass5)", 4,m_pt_bins_double); // GeV

    // truth vertex yields at mass peak
    Float_t m_eta_bins[] = {0,0.5,1.5,2.5,4.0};
    //Float_t m_pt_bins[] = {0,100,200,300,400,500,1000};
    Float_t m_pt_bins[] = {0,150,300,500,1000};
    m_truth_yields_map_pt_eta_1 = new TH2D("m_truth_yields_map_pt_eta_1", "truth vertex yields map pt, eta, 1",4,m_pt_bins,4,m_eta_bins); // GeV
    m_truth_yields_map_pt_eta_2 = new TH2D("m_truth_yields_map_pt_eta_2", "truth vertex yields map pt, eta, 2",4,m_pt_bins,4,m_eta_bins); // GeV
    m_truth_yields_map_pt_eta_3 = new TH2D("m_truth_yields_map_pt_eta_3", "truth vertex yields map pt, eta, 3",4,m_pt_bins,4,m_eta_bins); // GeV
    m_truth_yields_map_pt_eta_4 = new TH2D("m_truth_yields_map_pt_eta_4", "truth vertex yields map pt, eta, 4",4,m_pt_bins,4,m_eta_bins); // GeV
    m_truth_yields_map_pt_eta_5 = new TH2D("m_truth_yields_map_pt_eta_5", "truth vertex yields map pt, eta, 5",4,m_pt_bins,4,m_eta_bins); // GeV

    // reco vertex yields at mass peak
    m_reco_yields_map_pt_eta_1 = new TH2D("m_reco_yields_map_pt_eta_1", "reco vertex yields map pt, eta, 1",4,m_pt_bins,4,m_eta_bins); // GeV
    m_reco_yields_map_pt_eta_2 = new TH2D("m_reco_yields_map_pt_eta_2", "reco vertex yields map pt, eta, 2",4,m_pt_bins,4,m_eta_bins); // GeV
    m_reco_yields_map_pt_eta_3 = new TH2D("m_reco_yields_map_pt_eta_3", "reco vertex yields map pt, eta, 3",4,m_pt_bins,4,m_eta_bins); // GeV
    m_reco_yields_map_pt_eta_4 = new TH2D("m_reco_yields_map_pt_eta_4", "reco vertex yields map pt, eta, 4",4,m_pt_bins,4,m_eta_bins); // GeV
    m_reco_yields_map_pt_eta_5 = new TH2D("m_reco_yields_map_pt_eta_5", "reco vertex yields map pt, eta, 5",4,m_pt_bins,4,m_eta_bins); // GeV

    // truth mass distribution of long-lived particle
    m_truth_m = new TH1F("m_truth_m", "truth_m", 1000., 0, 1000.); // GeV
    m_truth_DeltaR = new TH1F("m_truth_DeltaR", "truth_DeltaR", 50, 0, 2*M_PI); // GeV

    // efficiency vs delta r
    Float_t m_deltaR_bins[] = {0,0.5,1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6,6.5};
    m_dv_eff_deltaR = new TEfficiency("m_dv_eff_deltaR", "DV reconstruction efficiency vs delta R", 10, 0, 2*M_PI); // GeV
    m_truth_yields_map_pt_deltaR = new TH2D("m_truth_yields_map_pt_deltaR", "truth vertex yields map pt, eta, 1", 4, m_pt_bins, 13, m_deltaR_bins); // GeV
    m_reco_yields_map_pt_deltaR = new TH2D("m_reco_yields_map_pt_deltaR", "reco vertex yields map pt, eta, 1", 4, m_pt_bins, 13, m_deltaR_bins); // GeV

    // weight distribution for specific bins
    m_weight1 = new TH1F("m_weight1", "weight1", 50., 0, 50.); // GeV
    m_weight2 = new TH1F("m_weight2", "weight2", 50., 0, 50.); // GeV
    m_weight3 = new TH1F("m_weight3", "weight3", 50., 0, 50.); // GeV

    // DeltaR distribution for specific bins
    m_DeltaR1 = new TH1F("m_DeltaR1", "DeltaR1", 50., 0, 6); // GeV
    m_DeltaR2 = new TH1F("m_DeltaR2", "DeltaR2", 50., 0, 6); // GeV
    m_DeltaR3 = new TH1F("m_DeltaR3", "DeltaR3", 50., 0, 6); // GeV

    // lepton eta for specific bins
    m_lepton_eta1 = new TH1F("m_eta1", "eta1", 20., 0, 4); // GeV
    m_lepton_eta2 = new TH1F("m_eta2", "eta2", 20., 0, 4); // GeV
    m_lepton_eta3 = new TH1F("m_eta3", "eta3", 20., 0, 4); // GeV

    // lepton eta for specific bins
    m_lepton_pt1 = new TH1F("m_pt1", "pt1", 50., 0, 1000); // GeV
    m_lepton_pt2 = new TH1F("m_pt2", "pt2", 50., 0, 1000); // GeV
    m_lepton_pt3 = new TH1F("m_pt3", "pt3", 50., 0, 1000); // GeV

    // output

    // reco vertex cutflow
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/reco/dv_reco_cutflow;", m_dv_reco_cutflow) );

    // efficiency at different eta
    CHECK( histSvc->regGraph("/DV/EfficiencyStudy/eff/dv_eff_pt_eta1_mass1",reinterpret_cast<TGraph*>(m_dv_eff_pt_eta1_mass1)) );
    CHECK( histSvc->regGraph("/DV/EfficiencyStudy/eff/dv_eff_pt_eta2_mass1",reinterpret_cast<TGraph*>(m_dv_eff_pt_eta2_mass1)) );
    CHECK( histSvc->regGraph("/DV/EfficiencyStudy/eff/dv_eff_pt_eta3_mass1",reinterpret_cast<TGraph*>(m_dv_eff_pt_eta3_mass1)) );
    CHECK( histSvc->regGraph("/DV/EfficiencyStudy/eff/dv_eff_pt_eta1_mass2",reinterpret_cast<TGraph*>(m_dv_eff_pt_eta1_mass2)) );
    CHECK( histSvc->regGraph("/DV/EfficiencyStudy/eff/dv_eff_pt_eta2_mass2",reinterpret_cast<TGraph*>(m_dv_eff_pt_eta2_mass2)) );
    CHECK( histSvc->regGraph("/DV/EfficiencyStudy/eff/dv_eff_pt_eta3_mass2",reinterpret_cast<TGraph*>(m_dv_eff_pt_eta3_mass2)) );
    CHECK( histSvc->regGraph("/DV/EfficiencyStudy/eff/dv_eff_pt_eta1_mass3",reinterpret_cast<TGraph*>(m_dv_eff_pt_eta1_mass3)) );
    CHECK( histSvc->regGraph("/DV/EfficiencyStudy/eff/dv_eff_pt_eta2_mass3",reinterpret_cast<TGraph*>(m_dv_eff_pt_eta2_mass3)) );
    CHECK( histSvc->regGraph("/DV/EfficiencyStudy/eff/dv_eff_pt_eta3_mass3",reinterpret_cast<TGraph*>(m_dv_eff_pt_eta3_mass3)) );
    CHECK( histSvc->regGraph("/DV/EfficiencyStudy/eff/dv_eff_pt_eta1_mass4",reinterpret_cast<TGraph*>(m_dv_eff_pt_eta1_mass4)) );
    CHECK( histSvc->regGraph("/DV/EfficiencyStudy/eff/dv_eff_pt_eta2_mass4",reinterpret_cast<TGraph*>(m_dv_eff_pt_eta2_mass4)) );
    CHECK( histSvc->regGraph("/DV/EfficiencyStudy/eff/dv_eff_pt_eta3_mass4",reinterpret_cast<TGraph*>(m_dv_eff_pt_eta3_mass4)) );
    CHECK( histSvc->regGraph("/DV/EfficiencyStudy/eff/dv_eff_pt_eta1_mass5",reinterpret_cast<TGraph*>(m_dv_eff_pt_eta1_mass5)) );
    CHECK( histSvc->regGraph("/DV/EfficiencyStudy/eff/dv_eff_pt_eta2_mass5",reinterpret_cast<TGraph*>(m_dv_eff_pt_eta2_mass5)) );
    CHECK( histSvc->regGraph("/DV/EfficiencyStudy/eff/dv_eff_pt_eta3_mass5",reinterpret_cast<TGraph*>(m_dv_eff_pt_eta3_mass5)) );

    // efficiency vs delta r
    CHECK( histSvc->regGraph("/DV/EfficiencyStudy/eff/m_dv_eff_deltaR",reinterpret_cast<TGraph*>(m_dv_eff_deltaR)) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/truth/truth_yields_map_pt_deltaR", m_truth_yields_map_pt_deltaR) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/reco/reco_yields_map_pt_deltaR", m_reco_yields_map_pt_deltaR) );

    // truth vertex yields at mass peak
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/truth/truth_yields_map_pt_eta_1", m_truth_yields_map_pt_eta_1) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/truth/truth_yields_map_pt_eta_2", m_truth_yields_map_pt_eta_2) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/truth/truth_yields_map_pt_eta_3", m_truth_yields_map_pt_eta_3) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/truth/truth_yields_map_pt_eta_4", m_truth_yields_map_pt_eta_4) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/truth/truth_yields_map_pt_eta_5", m_truth_yields_map_pt_eta_5) );
    
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/truth/truth_m", m_truth_m) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/truth/truth_DeltaR", m_truth_DeltaR) );

    // reco vertex yields at mass peak
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/reco/reco_yields_map_pt_eta_1", m_reco_yields_map_pt_eta_1) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/reco/reco_yields_map_pt_eta_2", m_reco_yields_map_pt_eta_2) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/reco/reco_yields_map_pt_eta_3", m_reco_yields_map_pt_eta_3) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/reco/reco_yields_map_pt_eta_4", m_reco_yields_map_pt_eta_4) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/reco/reco_yields_map_pt_eta_5", m_reco_yields_map_pt_eta_5) );

    // weight distribution
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/specific_bin/m_weight1", m_weight1) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/specific_bin/m_weight2", m_weight2) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/specific_bin/m_weight3", m_weight3) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/specific_bin/m_DeltaR1", m_DeltaR1) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/specific_bin/m_DeltaR2", m_DeltaR2) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/specific_bin/m_DeltaR3", m_DeltaR3) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/specific_bin/m_lepton_eta1", m_lepton_eta1) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/specific_bin/m_lepton_eta2", m_lepton_eta2) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/specific_bin/m_lepton_eta3", m_lepton_eta3) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/specific_bin/m_lepton_pt1", m_lepton_pt1) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/specific_bin/m_lepton_pt2", m_lepton_pt2) );
    CHECK( histSvc->regHist("/DV/EfficiencyStudy/specific_bin/m_lepton_pt3", m_lepton_pt3) );


    // Use weighted events in TEfficiency class
     m_dv_eff_pt_eta1_mass1->SetUseWeightedEvents();
     m_dv_eff_pt_eta2_mass1->SetUseWeightedEvents();
     m_dv_eff_pt_eta3_mass1->SetUseWeightedEvents();
     m_dv_eff_pt_eta1_mass2->SetUseWeightedEvents();
     m_dv_eff_pt_eta2_mass2->SetUseWeightedEvents();
     m_dv_eff_pt_eta3_mass2->SetUseWeightedEvents();
     m_dv_eff_pt_eta1_mass3->SetUseWeightedEvents();
     m_dv_eff_pt_eta2_mass3->SetUseWeightedEvents();
     m_dv_eff_pt_eta3_mass3->SetUseWeightedEvents();
     m_dv_eff_pt_eta1_mass4->SetUseWeightedEvents();
     m_dv_eff_pt_eta2_mass4->SetUseWeightedEvents();
     m_dv_eff_pt_eta3_mass4->SetUseWeightedEvents();
     m_dv_eff_pt_eta1_mass5->SetUseWeightedEvents();
     m_dv_eff_pt_eta2_mass5->SetUseWeightedEvents();
     m_dv_eff_pt_eta3_mass5->SetUseWeightedEvents();
     m_dv_eff_deltaR->SetUseWeightedEvents();




    return StatusCode::SUCCESS;
}

StatusCode EfficiencyStudy::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");
    
    ATH_MSG_INFO("number of events processed = " << n_events);
    
    return StatusCode::SUCCESS;
}

StatusCode EfficiencyStudy::execute() {  
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
    m_dv_reco_cutflow->Fill("RPVLL",0);
    m_dv_reco_cutflow->Fill("GRL",0);
    m_dv_reco_cutflow->Fill("EventCleaning",0);
    m_dv_reco_cutflow->Fill("TrigFilter",0);

    // event selection flag
    bool event_passed = true;

    // all events
    m_dv_reco_cutflow->Fill("RPVLL",1);

    // GRL
    if (!isMC and !m_grlTool->passRunLB(*evtInfo)) event_passed = false;
    if (event_passed) m_dv_reco_cutflow->Fill("GRL",1);

    // event cleaning
    if (!m_evtc->PassEventCleaning(*evtInfo)) event_passed = false;
    if (event_passed) m_dv_reco_cutflow->Fill("EventCleaning",1);

    // trigger filter
    bool trig_passed = false;
    
    // check trigger and fill histogram for trigger efficiency
    if (m_tdt->isPassed("HLT_mu60_0eta105_msonly")) trig_passed = true;
    if (m_tdt->isPassed("HLT_g140_loose")) trig_passed = true;
    if (m_tdt->isPassed("HLT_2g50_loose")) trig_passed = true;

    if(!trig_passed) event_passed = false;

    // trigger filter
    if (event_passed) m_dv_reco_cutflow->Fill("TrigFilter",1);

    // cosmic veto
    if(!m_cos->PassCosmicEventVeto(*elc, *muc)) event_passed = false;
    
    if (event_passed) m_dv_reco_cutflow->Fill("CosmicVeto", 1);
    else m_dv_reco_cutflow->Fill("CosmicVeto", 0);

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

    if (event_passed) m_dv_reco_cutflow->Fill("z_{PV} < 200 mm", 1);
    else m_dv_reco_cutflow->Fill("z_{PV} < 200 mm", 0);


    //=================================================
    // vertex cut flow
    //=================================================
    // set to true if vertex pass all selection
    //bool dv_matched = false;

    // cut flow
    for(auto dv: *dvc_copy.first) {

        // flag to mark if this DV passed cuts
        bool vertex_passed = true;

        // vertex found
        m_dv_reco_cutflow->Fill("Vertex",1);

        // access tracks from vertex
        auto tpLinks = dv->trackParticleLinks();
        xAOD::TrackParticle tp1 = **(tpLinks.at(0));
        xAOD::TrackParticle tp2 = **(tpLinks.at(1));

        // access invariant mass
        float dv_mass = std::fabs(m_accMass(*dv)) / 1000.; // in GeV

        // mass and position cut
        float mass_min = 10.;
        float dv_R_max = 300;
        float dv_z_max = 300;

        // collect leptons from this dv
        auto dv_muc = m_accMu(*dv);
        auto dv_elc = m_accEl(*dv);

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
        float dv_z_wrt_beam = std::abs((*dv).position().z());       // z in [mm]

        // select only vertex with tracks
        if(dv->trackParticleLinks().size() != 2) vertex_passed = false;

        // find decay channel of dv
        std::string channel = m_dvutils->DecayChannel(*dv);

        // only select mumu, ee, or emu
        if (!((channel == "mumu") or (channel == "emu") or (channel == "ee"))) vertex_passed = false;
        if(vertex_passed) m_dv_reco_cutflow->Fill("LeptonID",1);

        // Trigger matching
        if(!m_dvutils->TrigMatching(*dv)) vertex_passed = false;
        if(vertex_passed) m_dv_reco_cutflow->Fill("Trig.Matching",1);

        // vertex fit quality
        if(!m_dvc->PassChisqCut(*dv)) vertex_passed = false;
        if(vertex_passed) m_dv_reco_cutflow->Fill("#chi^{2} / DOF",1);

        // minimum distance from pv (from 0 for MC)
        if(!m_dvc->PassDistCut(*dv, *pvc)) vertex_passed = false;
        if(vertex_passed) m_dv_reco_cutflow->Fill("Disp. > 2 mm",1);

        // charge requirements
        if(!m_dvc->PassChargeRequirement(*dv)) vertex_passed = false;
        if(vertex_passed) m_dv_reco_cutflow->Fill("Oppo.Charge",1);

        // disabled module
        if(!m_dvc->PassDisabledModuleVeto(*dv)) vertex_passed = false;
        if(vertex_passed) m_dv_reco_cutflow->Fill("DisabledModule",1);

        if ((channel == "emu") or (channel == "ee")) {
            if(!m_dvc->PassMaterialVeto(*dv)) vertex_passed = false;
        }
        if(vertex_passed) m_dv_reco_cutflow->Fill("MaterialVeto",1);

        // low mass veto
        if(dv_mass < mass_min) vertex_passed = false;
        if(vertex_passed) m_dv_reco_cutflow->Fill("LowMass",1);

        // cosmic veto (R_CR)
        //if(!PassCosmicVeto_R_CR(tp1, tp2)) vertex_passed = false;
        //if(vertex_passed) m_dv_reco_cutflow->Fill("R_{CR} > 0.01",1);

        // RPVLL filter matching
        if(!m_dilepdvc->PassFilterMatching(*dv)) vertex_passed = false;
        if(vertex_passed) m_dv_reco_cutflow->Fill("FilterMatching", 1);

        // DV R <  300 mm
        if(dv_R_wrt_beam > dv_R_max) vertex_passed = false;
        if(vertex_passed) m_dv_reco_cutflow->Fill("R_{DV} > 300 mm", 1);

        // DV z <  300 mm
        if(dv_z_wrt_beam > dv_z_max) vertex_passed = false;
        if(vertex_passed) m_dv_reco_cutflow->Fill("z_{DV} > 300 mm", 1);

        // mark this event as reconstructed
        //dv_matched = true;
    
        // find closest truth vertex by position
        if(vertex_passed) {
            const xAOD::TruthVertex* matched_truth = m_dvutils->getClosestTruthVertex(dv, true);
            if(matched_truth) matched_truth->auxdecor<bool>("reconstructed") = 1;
        }

    } // end of dv loop
    
    // if event didn't pass event selection, mark as not reconstructed
    //if (!event_passed) dv_matched = false;

    //-----------------------------------------------------------------
    // End of cut flow.
    // We use dv_matched flag to fill truth and reco vertex yields
    //-----------------------------------------------------------------

    // retrieve truth vertex container
    const xAOD::TruthVertexContainer* tru_vc = nullptr;
    evtStore()->retrieve( tru_vc, "TruthVertices");

    // loop over the truth vertex container
    for (auto tru_v: *tru_vc){

        // check if this truth dv is reconstructed
        bool isReconstructed = (tru_v->auxdecor<bool>("reconstructed") && event_passed);

        // only selecting signal truth
        if (!m_dvutils->isSignalVertex(tru_v)) continue;

        // flag to mark if this is mumu vertex
        bool isMuMu = true;

        // store lepton pt and eta for investigating specific bins
        std::vector<float> lepton_eta = {};
        std::vector<float> lepton_pt = {};
       
        // loop over outgoing particles
        for (unsigned int i = 0; i < tru_v->nOutgoingParticles(); i++){

            // set isMuMu to false if there is an electron 
            const xAOD::TruthParticle* truth_child = tru_v->outgoingParticle(i);
            if ((truth_child->absPdgId() == 11)) isMuMu = false;

            // get eta of muon
            if ((truth_child->absPdgId() == 13)) {
                lepton_eta.push_back(std::abs(truth_child->eta()));
                lepton_pt.push_back(truth_child->pt() / 1000.); // GeV
            }
    
        }
  
        // only select mumu channel
        if (!isMuMu) continue;

        // lepton eta fiducial cut
        //float lepton_eta_max = 2.5;
        //if ((lepton_eta.at(0) > lepton_eta_max) or (lepton_eta.at(0) > lepton_eta_max)) continue;

        // get opening angle
        float dv_dr = m_dvutils->Truth_dr(tru_v);

        // only selecting vertex with DeltaR between 1 and 3.5
        float DeltaR_min = 1.0;
        float DeltaR_max = 2*M_PI;
        //if ((dv_dr < DeltaR_min) || (dv_dr > DeltaR_max)) continue;

        // fill DeltaR distribution
        m_truth_DeltaR->Fill(dv_dr, p_weight);

        // mass of dv, not counting neutral particle
        float dv_mass = m_dvutils->TruthMass(tru_v) / 1000.;
        float dv_pt = m_dvutils->TruthPt(tru_v) / 1000.;
        float dv_eta = std::abs(m_dvutils->TruthEta(tru_v));
        //float dv_eta = tru_v->incomingParticle(0)->eta();

        // retrieve long-lived particle kinematics
        //float llp_pt = tru_v->incomingParticle(0)->pt();
        //float llp_m = tru_v->incomingParticle(0)->m();
        //float llp_e = tru_v->incomingParticle(0)->e();
        //float llp_beta = sqrt(1 - (llp_m/llp_e)*(llp_m/llp_e));

        // set Z' mass and Gamma to use as benchmarks
        float mass_1 = 100;
        float mass_2 = 250;
        float mass_3 = 500;
        float mass_4 = 750;
        float mass_5 = 1000;

        float Gamma_1 = 2.8;
        float Gamma_2 = 6.9;
        float Gamma_3 = 14.7;
        float Gamma_4 = 23.0;
        float Gamma_5 = 31.0;
   
        // set eta regions
        float eta1 = 0.5;
        float eta2 = 1.5;
        float eta3 = 2.5;

        // fill mass distribution
        m_truth_m->Fill(dv_mass, p_weight);

        // efficiency vs DeltaR
        m_dv_eff_deltaR->FillWeighted(isReconstructed, p_weight, dv_dr);
        m_truth_yields_map_pt_deltaR->Fill(dv_pt, dv_dr, p_weight);
        if (isReconstructed) m_reco_yields_map_pt_deltaR->Fill(dv_pt, dv_dr, p_weight);
        
        // fill truth vertex yields and efficiency
        if( (dv_mass > mass_1 - 3*Gamma_1) && (dv_mass < mass_1 + 3*Gamma_1) ) {

            // truth yield
            m_truth_yields_map_pt_eta_1->Fill(dv_pt, dv_eta, p_weight);

            // reco yield
            if (isReconstructed) m_reco_yields_map_pt_eta_1->Fill(dv_pt, dv_eta, p_weight);

            // fill efficiency
            if (dv_eta < eta1) m_dv_eff_pt_eta1_mass1->FillWeighted(isReconstructed, p_weight, dv_pt);
            else if (dv_eta < eta2) m_dv_eff_pt_eta2_mass1->FillWeighted(isReconstructed, p_weight, dv_pt);
            else if (dv_eta < eta3) m_dv_eff_pt_eta3_mass1->FillWeighted(isReconstructed, p_weight, dv_pt);
        }

        if( (dv_mass > mass_2 - 3*Gamma_2) && (dv_mass < mass_2 + 3*Gamma_2) ) {

            // truth yield
            m_truth_yields_map_pt_eta_2->Fill(dv_pt, dv_eta, p_weight);

            // reco yield
            if (isReconstructed) m_reco_yields_map_pt_eta_2->Fill(dv_pt, dv_eta, p_weight);

            // fill efficiency
            if (dv_eta < eta1) m_dv_eff_pt_eta1_mass2->FillWeighted(isReconstructed, p_weight, dv_pt);
            else if (dv_eta < eta2) m_dv_eff_pt_eta2_mass2->FillWeighted(isReconstructed, p_weight, dv_pt);
            else if (dv_eta < eta3) m_dv_eff_pt_eta3_mass2->FillWeighted(isReconstructed, p_weight, dv_pt);
        }

        if( (dv_mass > mass_3 - 3*Gamma_3) && (dv_mass < mass_3 + 3*Gamma_3) ) {

            // truth yield
            m_truth_yields_map_pt_eta_3->Fill(dv_pt, dv_eta, p_weight);

            // reco yield
            if (isReconstructed) m_reco_yields_map_pt_eta_3->Fill(dv_pt, dv_eta, p_weight);

            // fill efficiency
            if (dv_eta < eta1) m_dv_eff_pt_eta1_mass3->FillWeighted(isReconstructed, p_weight, dv_pt);
            else if (dv_eta < eta2) m_dv_eff_pt_eta2_mass3->FillWeighted(isReconstructed, p_weight, dv_pt);
            else if (dv_eta < eta3) {

                m_dv_eff_pt_eta3_mass3->FillWeighted(isReconstructed, p_weight, dv_pt);

                if ((dv_pt > 300.) && (dv_pt < 500.)) m_weight1->Fill(p_weight);
                if ((dv_pt > 300.) && (dv_pt < 500.)) m_DeltaR1->Fill(dv_dr);
                if ((dv_pt > 300.) && (dv_pt < 500.)) m_lepton_eta1->Fill(lepton_eta.at(0));
                if ((dv_pt > 300.) && (dv_pt < 500.)) m_lepton_eta1->Fill(lepton_eta.at(1));
                if ((dv_pt > 300.) && (dv_pt < 500.)) m_lepton_pt1->Fill(lepton_pt.at(0));
                if ((dv_pt > 300.) && (dv_pt < 500.)) m_lepton_pt1->Fill(lepton_pt.at(1));
            }
        }

        if( (dv_mass > mass_4 - 3*Gamma_4) && (dv_mass < mass_4 + 3*Gamma_4) ) {

            // truth yield
            m_truth_yields_map_pt_eta_4->Fill(dv_pt, dv_eta, p_weight);

            // reco yield
            if (isReconstructed) m_reco_yields_map_pt_eta_4->Fill(dv_pt, dv_eta, p_weight);

            // fill efficiency
            if (dv_eta < eta1) m_dv_eff_pt_eta1_mass4->FillWeighted(isReconstructed, p_weight, dv_pt);
            else if (dv_eta < eta2) {
                    m_dv_eff_pt_eta2_mass4->FillWeighted(isReconstructed, p_weight, dv_pt);

                    // investigating specific bin
                    if ((dv_pt > 300.) && (dv_pt < 500.)) m_weight2->Fill(p_weight);
                    if ((dv_pt > 300.) && (dv_pt < 500.)) m_DeltaR2->Fill(dv_dr);
                    if ((dv_pt > 300.) && (dv_pt < 500.)) m_lepton_eta2->Fill(lepton_eta.at(0));
                    if ((dv_pt > 300.) && (dv_pt < 500.)) m_lepton_eta2->Fill(lepton_eta.at(1));
                    if ((dv_pt > 300.) && (dv_pt < 500.)) m_lepton_pt2->Fill(lepton_pt.at(0));
                    if ((dv_pt > 300.) && (dv_pt < 500.)) m_lepton_pt2->Fill(lepton_pt.at(1));

            }
            else if (dv_eta < eta3) m_dv_eff_pt_eta3_mass4->FillWeighted(isReconstructed, p_weight, dv_pt);
        }

        if( (dv_mass > mass_5 - 3*Gamma_5) && (dv_mass < mass_5 + 3*Gamma_5) ) {

            // truth yield
            m_truth_yields_map_pt_eta_5->Fill(dv_pt, dv_eta, p_weight);

            // reco yield
            if (isReconstructed) m_reco_yields_map_pt_eta_5->Fill(dv_pt, dv_eta, p_weight);

            // fill efficiency
            if (dv_eta < eta1) m_dv_eff_pt_eta1_mass5->FillWeighted(isReconstructed, p_weight, dv_pt);
            else if (dv_eta < eta2) m_dv_eff_pt_eta2_mass5->FillWeighted(isReconstructed, p_weight, dv_pt);
            else if (dv_eta < eta3) {
                m_dv_eff_pt_eta3_mass5->FillWeighted(isReconstructed, p_weight, dv_pt);

                // investigating specific bin
                if (dv_pt < 150.) m_weight3->Fill(p_weight);
                if (dv_pt < 150.) m_DeltaR3->Fill(dv_dr);
                if (dv_pt < 150.) m_lepton_eta3->Fill(lepton_eta.at(0));
                if (dv_pt < 150.) m_lepton_eta3->Fill(lepton_eta.at(1));
                if (dv_pt < 150.) m_lepton_pt3->Fill(lepton_pt.at(0));
                if (dv_pt < 150.) m_lepton_pt3->Fill(lepton_pt.at(1));
            }
        }


    } // end of efficiency loop


    return StatusCode::SUCCESS;
}


bool EfficiencyStudy::PassCosmicVeto_R_CR(xAOD::TrackParticle& tr0, xAOD::TrackParticle& tr1){

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

