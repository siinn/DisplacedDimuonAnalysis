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

// tools
#include "PathResolver/PathResolver.h"

// DV
#include "DVCuts/EventCuts.h"

// plots 
#include "TProfile.h"
#include "TProfile2D.h"
#include "cmath"
#include "vector"




DVEfficiency::DVEfficiency( const std::string& name, ISvcLocator* pSvcLocator ) : 
AthAnalysisAlgorithm( name, pSvcLocator ),
m_dilepdvc("DDL::DiLepDVCuts/DiLepDVCuts"),
m_evtc("DDL::EventCuts/DiLepEventCuts"),
m_dvc("DDL::DVCuts/DiLepBaseCuts"),
m_cos("DDL::DiLepCosmics/DiLepCosmics"),
m_dvutils("DVUtils"),
m_grlTool("GoodRunsListSelectionTool/GRLTool"),
m_tdt("Trig::TrigDecisionTool/TrigDecisionTool"),
m_or("DDL::OverlapRemoval/OverlapRemoval"),
//m_decorate_truv("reconstructed"),
m_accMass("mass")
{
    // initialize tools
    declareProperty("DiLepDVCuts", m_dilepdvc);
    declareProperty("DVUtils", m_dvutils);
    declareProperty("DiLepEventCuts", m_evtc);
    declareProperty("DiLepBaseCuts", m_dvc);
    declareProperty("GRLTool",  m_grlTool, "The private GoodRunsListSelectionTool" );
    declareProperty("TrigDecisionTool", m_tdt);
    declareProperty("DiLepCosmics", m_cos);
    declareProperty("OverlapRemoval", m_or);
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
    m_dv_eff_eta = new TProfile( "m_dv_eff_eta", "DV reconstruction efficiency vs eta", 50, -4.0, 4.0);
    m_dv_eff_phi = new TProfile( "m_dv_eff_phi", "DV reconstruction efficiency vs phi", 50, -M_PI, M_PI);
    m_dv_eff_mass = new TProfile( "m_dv_eff_mass", "DV reconstruction efficiency vs mass", 150, 0, 1500); // GeV
    m_dv_eff_R = new TProfile( "m_dv_eff_R", "DV reconstruction efficiency vs R", 100, 0, 400); // mm
    m_dv_eff_d0 = new TProfile( "m_dv_eff_d0", "DV reconstruction efficiency vs d0", 100, 0, 400); // mm

    // efficiency as a function of Z' parameters
    m_dv_eff_zp_eta = new TProfile("m_dv_eff_zp_eta", "DV reconstruction efficiency vs Z' eta", 50, -3.0, 3.0);
    m_dv_eff_zp_pt = new TProfile("m_dv_eff_zp_pt", "DV reconstruction efficiency vs Z' pt", 50, 0, 1000); // GeV
 

    // output
    CHECK( histSvc->regHist("/DV/TruthVertex/dv_mass", m_dv_mass) );

    // efficiency plots
    CHECK( histSvc->regHist("/DV/TruthVertex/efficiency/dv/dv_eff_eta", m_dv_eff_eta) );
    CHECK( histSvc->regHist("/DV/TruthVertex/efficiency/dv/dv_eff_phi", m_dv_eff_phi) );
    CHECK( histSvc->regHist("/DV/TruthVertex/efficiency/dv/dv_eff_mass", m_dv_eff_mass) );
    CHECK( histSvc->regHist("/DV/TruthVertex/efficiency/dv/dv_eff_R", m_dv_eff_R) );
    CHECK( histSvc->regHist("/DV/TruthVertex/efficiency/dv/dv_eff_d0", m_dv_eff_d0) );

    CHECK( histSvc->regHist("/DV/TruthVertex/efficiency/zp/dv_eff_zp_eta", m_dv_eff_zp_eta) );
    CHECK( histSvc->regHist("/DV/TruthVertex/efficiency/zp/dv_eff_zp_pt", m_dv_eff_zp_pt) );

    return StatusCode::SUCCESS;
}

StatusCode DVEfficiency::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");
    
    return StatusCode::SUCCESS;
}

StatusCode DVEfficiency::execute() {  
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    // retrieve event info
    const xAOD::EventInfo* evtInfo = nullptr;
    CHECK( evtStore()->retrieve( evtInfo, "EventInfo" ) );

    // flag to check if data or MC
    bool isMC = evtInfo->eventType(xAOD::EventInfo::IS_SIMULATION);

    // GRL
    if (!isMC and !m_grlTool->passRunLB(*evtInfo)) return StatusCode::SUCCESS;

    // event cleaning
    if(!m_evtc->PassEventCleaning(*evtInfo)) return StatusCode::SUCCESS;

    // trigger check
    if(!m_evtc->PassTrigger()) return StatusCode::SUCCESS;

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

    // cut flow
    for(auto dv: *dvc_copy.first) {

        // access invariant mass
        float dv_mass = std::fabs(m_accMass(*dv)) / 1000.; // in MeV

        // collect muons from this dv
        auto dv_muc = m_dilepdvc->GetMu(*dv);

        // remove overlapping muon
        m_dilepdvc->ApplyOverlapRemoval(*dv);

        //----------------------------------------
        // require dv to have 2 muons
        //----------------------------------------
        if (dv_muc->size() != 2) continue;

        //----------------------------------------
        // Trigger matching
        //----------------------------------------
        if (!m_dvutils->TriggerMatching(*dv_muc)) continue;

        //----------------------------------------
        // combined muon
        //----------------------------------------
        if(!m_dvutils->IsCombinedMuon(*dv_muc)) continue;

        //----------------------------------------
        // vertex fit quality
        //----------------------------------------
        if(!m_dvc->PassChisqCut(*dv)) continue;

        //----------------------------------------
        // minimum distance from pv (from 0 for MC)
        //----------------------------------------
        //if(!m_dvc->PassDistCut(*dv, *pvc)) continue;

        //----------------------------------------
        // charge requirements
        //----------------------------------------
        if(!m_dvc->PassChargeRequirement(*dv)) continue;

        //----------------------------------------
        // disabled module
        //----------------------------------------
        if(!m_dvc->PassDisabledModuleVeto(*dv)) continue;

        //----------------------------------------
        // DESD filter
        //----------------------------------------
        m_dilepdvc->DoTriggerMatching(*dv);
        if(!m_dilepdvc->PassDESDMatching(*dv)) continue;

        //----------------------------------------
        // cosmic veto
        //----------------------------------------
        if(!PassCosmicVeto(*dv_muc)) continue;

        //----------------------------------------
        // delta R 
        //----------------------------------------
        float deltaR_min = 0.5;
        if(m_dvutils->getDeltaR(*dv_muc) < deltaR_min) continue;

        if (isMC) {
            // find truth dv matched to this dv
            const xAOD::TruthVertex* tru_v = m_dvutils->IsSignalDV(*dv_muc);

            if (tru_v == nullptr) continue;

            // flag this truth vertex as reconstructed
            tru_v->auxdecor<int>("reconstructed") = 1;

        } // end of isMC
    } // end of dv loop


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

        // create accessor for reconstruction flag
        static SG::AuxElement::ConstAccessor<int> acc_tru_v("reconstructed");
        bool dv_matched = acc_tru_v.isAvailable(*tru_v);

        // fill truth signal vertex mass
        float DVMass = m_dvutils->TruthMass(tru_v) / 1000.;
        m_dv_mass->Fill(DVMass);
     
        // fill efficiency plots
        m_dv_eff_eta->Fill( tru_v->eta(), dv_matched);
        m_dv_eff_phi->Fill( tru_v->phi(), dv_matched);
        m_dv_eff_mass->Fill( DVMass, dv_matched);
        m_dv_eff_R->Fill( tru_v->perp() , dv_matched);
        m_dv_eff_d0->Fill( m_dvutils->GetMaxd0(tru_v), dv_matched );

        // efficiency as a function of Z'
        float zp_eta = tru_v->incomingParticle(0)->eta();
        float zp_pt = tru_v->incomingParticle(0)->pt();

        m_dv_eff_zp_eta->Fill(zp_eta, dv_matched);
        m_dv_eff_zp_pt->Fill(zp_pt / 1000., dv_matched);

    } // end of efficiency loop


    return StatusCode::SUCCESS;
}

bool DVEfficiency::PassCosmicVeto(const DataVector<xAOD::Muon> dv_muc) {

    bool PassCosmicVeto = true;

    float Rcos_min = 0.04;
    float deltaPhiMinusPi = m_dvutils->getDeltaPhiMinusPi(dv_muc);
    float sumEta = m_dvutils->getSumEta(dv_muc);

    float Rcos = std::sqrt(sumEta * sumEta + deltaPhiMinusPi * deltaPhiMinusPi);

    if (Rcos < Rcos_min) PassCosmicVeto = false;

    return PassCosmicVeto;
}

