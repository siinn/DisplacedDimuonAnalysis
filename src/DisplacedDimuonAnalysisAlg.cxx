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
#include "xAODTracking/VertexContainer.h"
#include "xAODTruth/TruthParticleContainer.h"

// tools
#include "PathResolver/PathResolver.h"

// DV
#include "DVCuts/EventCuts.h"


// debug
//#include "StoreGate/StoreGate.h"
//#include "StoreGate/StoreGateSvc.h"



DisplacedDimuonAnalysisAlg::DisplacedDimuonAnalysisAlg( const std::string& name, ISvcLocator* pSvcLocator ) :
AthAnalysisAlgorithm( name, pSvcLocator ),
m_dilepdvc("DV::DiLepDVCuts/DiLepDVCuts"),
m_evtc("DV::EventCuts/DiLepEventCuts"),
m_dvutils("DVUtils"),
m_dvc("DV::DVCuts/DiLepBaseCuts"),
m_accMass("mass")
{
    // initialize tools
    declareProperty("DiLepDVCuts", m_dilepdvc);
    declareProperty("DVUtils", m_dvutils);
    declareProperty("DiLepEventCuts", m_evtc);
    declareProperty("DiLepBaseCuts", m_dvc);

}


DisplacedDimuonAnalysisAlg::~DisplacedDimuonAnalysisAlg() {}


StatusCode DisplacedDimuonAnalysisAlg::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");

    //--------------------------------------------------------
    // Event cut
    //--------------------------------------------------------

    auto evtc = dynamic_cast<DV::EventCuts*>(&*m_evtc);

    // set trigger
    evtc->SetTriggers({"HLT_mu60_0eta105_msonly"});


    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    // define histograms

    m_dv_cutflow = new TH1D( "m_dv_cutflow", "Reco dv cutflow", 6,0,6);
    m_event_cutflow = new TH1D( "m_event_cutflow", "Event cutflow", 8,0,8);

    m_dv_M = new TH1F("dv_M","All DV mass in GeV",200,0.,2000.);
    m_dv_dimuon_M = new TH1F("dv_dimuon_M","Dimuon DV mass in GeV",200,0.,2000.);
    m_dv_dimuon_R = new TH1F("dv_dimuon_R","R of dimuon dv [mm]",50,0.,300.);
    m_dv_dimuon_z = new TH1F("dv_dimuon_z","z of dimuon dv [mm]",100,0.,1000.);
    m_dv_dimuon_r = new TH1F("dv_dimuon_r","r of dimuon dv [mm]",100,0.,1000.);
    m_dv_dimuon_R_M = new TH2F("dv_dimuon_R_M","Dimuon DV position R vs M", 50,0,300,200,0,2000);

    // only for MC
    m_dv_dimuon_M_matched = new TH1F("dv_dimuon_M_matched","matched dimuon DV mass in GeV",200,0.,2000.);
    m_dv_dimuon_R_matched = new TH1F("dv_dimuon_R_matched","R of matched dimuon dv [mm]",50,0.,300.);
    m_dv_dimuon_R_M_matched = new TH2F("dv_dimuon_R_M_matched","matched dimuon DV position R vs M", 50,0,300,200,0,2000);
    m_chi2_ndof = new TH1F("chi2_ndof", "chi^2 / ndof", 50, 0, 50);

    // registor for output
    CHECK( histSvc->regHist("/DV/SecondaryVertex/dv_cutflow", m_dv_cutflow) );
    CHECK( histSvc->regHist("/DV/event_cutflow", m_event_cutflow) );

    CHECK( histSvc->regHist("/DV/SecondaryVertex/Reconstructed/reco_dv_M", m_dv_M) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/Reconstructed/reco_dv_dimuon_M", m_dv_dimuon_M) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/Reconstructed/reco_dv_dimuon_R", m_dv_dimuon_R) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/Reconstructed/reco_dv_dimuon_z", m_dv_dimuon_z) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/Reconstructed/reco_dv_dimuon_r", m_dv_dimuon_r) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/Reconstructed/reco_dv_R_M", m_dv_dimuon_R_M) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/chi2_ndof", m_chi2_ndof) );

    // only for MC
    CHECK( histSvc->regHist("/DV/SecondaryVertex/Reconstructed/truth-matched/reco_dv_dimuon_M", m_dv_dimuon_M_matched) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/Reconstructed/truth-matched/reco_dv_dimuon_R", m_dv_dimuon_R_matched) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/Reconstructed/truth-matched/reco_dv_R_M", m_dv_dimuon_R_M_matched) );

  return StatusCode::SUCCESS;
}

StatusCode DisplacedDimuonAnalysisAlg::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");
    return StatusCode::SUCCESS;
}

StatusCode DisplacedDimuonAnalysisAlg::execute() {
    ATH_MSG_DEBUG ("Executing " << name() << "...");
    setFilterPassed(true);

    // retrieve event info
    const xAOD::EventInfo* evtInfo = nullptr;
    CHECK( evtStore()->retrieve( evtInfo, "EventInfo" ) );

    // flag to check if data or MC
    bool isMC = evtInfo->eventType(xAOD::EventInfo::IS_SIMULATION);

    m_event_cutflow->Fill("AllEvents", 1);

    // GRL
    if(!m_evtc->PassGRL(*evtInfo)) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("PassedGRL", 1);

    // trigger check
    if(!m_evtc->PassTrigger()) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("PasseedTrig", 1);

    // event cleaning
    if(!m_evtc->PassEventCleaning(*evtInfo)) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("PassedEvtCleaning", 1);

    // retrieve lepton container
    const xAOD::MuonContainer* muc = nullptr;
    CHECK( evtStore()->retrieve( muc, "Muons" ));

    // make copies of leptons
    auto muc_copy = xAOD::shallowCopyContainer(*muc);
    xAOD::setOriginalObjectLink(*muc, *muc_copy.first);

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
        m_dvutils->ApplyMuonMatching(*dv, *muc_copy.first);
    }

    // final plotting
    for(auto dv: *dvc_copy.first) {
        // access invariant mass
        float dv_mass = std::fabs(m_accMass(*dv)) / 1000.; // in MeV

        // collect muons from this dv
        auto dv_muc = m_dilepdvc->GetMu(*dv);

        // fill all dv
        m_dv_M->Fill(dv_mass);               // all dv

        //----------------------------------------
        // counting all dv's
        //----------------------------------------
        m_dv_cutflow->Fill("All DV", 1);

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
        // vertex fit quality
        //----------------------------------------
        m_chi2_ndof->Fill (dv->chiSquared() / dv->numberDoF() );
        if(!m_dvc->PassChisqCut(*dv)) continue;
        m_dv_cutflow->Fill("(#chi^{2} / DOF)_{DV} < 5", 1);

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
        // un-used cutflow
        //----------------------------------------
        // material veto
        //if(!m_dvc->PassMaterialVeto(*dv)) continue;
        //m_dv_cutflow->Fill("MaterialVeto", 1);

        // perform blind on data (mask if R > 3 mm and M > 100 GeV )
        //if (!isMC and m_dvc->PassDistCut(*dv, *pvc) and m_dvc->PassMassCut(*dv) ) continue;
        //if (m_dvc->PassDistCut(*dv, *pvc) and m_dvc->PassMassCut(*dv) ) continue;
        //m_dv_cutflow->Fill("PassedBlind", 1);

        // minimum DV mass
        //if(!m_dvc->PassMassCut(*dv)) continue;
        //m_dv_cutflow->Fill("PassMinDVMass", 1);

        // fill dimuon vertex
        float dv_R = m_dvutils->getR( *dv, *pv );                 // R in [mm]
        float dv_z = m_dvutils->getz( *dv, *pv );                 // z in [mm]
        float dv_r = m_dvutils->getr( *dv, *pv );                 // r in [mm]

        m_dv_dimuon_M->Fill(dv_mass);                             // dimuon mass
        m_dv_dimuon_R->Fill(dv_R);                                
        m_dv_dimuon_z->Fill(dv_z);                                
        m_dv_dimuon_r->Fill(dv_r);                                
        m_dv_dimuon_R_M->Fill(dv_R, dv_mass);


        // This is block reserved for reco dv matching
        if (isMC) {
            // find truth dv matched to this dv
            const xAOD::TruthVertex* tru_v = m_dvutils->IsSignalDV(*dv_muc);

            if (tru_v == nullptr) continue;
            m_dv_cutflow->Fill("(MC)Truth-matched", 1);

            // fill matched dimuon vertex
            m_dv_dimuon_M_matched->Fill(dv_mass);                          // dimuon mass
            m_dv_dimuon_R_matched->Fill(dv_R);                                // R in [mm]
            m_dv_dimuon_R_M_matched->Fill(dv_R, dv_mass);

        } // end of isMC
    } // end of dv loop
    
    return StatusCode::SUCCESS;
}

StatusCode DisplacedDimuonAnalysisAlg::beginInputFile() { 

  return StatusCode::SUCCESS;
}


