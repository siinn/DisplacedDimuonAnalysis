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
//m_vertexer("DDL::DispVertexer/DispVertexer"),
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
    //declareProperty("Vertexer", m_vertexer);
}


FlipBkgEst::~FlipBkgEst() {}


StatusCode FlipBkgEst::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");
    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    m_n_mu = new TH1D( "m_n_mu", "Number of muons in event", 20,0,20);
    m_n_elc = new TH1D( "m_n_elc", "Number of electrons in event", 20,0,20);

    CHECK( histSvc->regHist("/DV/FlipBkgEst/n_mu", m_n_mu) );
    CHECK( histSvc->regHist("/DV/FlipBkgEst/n_elc", m_n_elc) );

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

    ATH_MSG_INFO("electron size = " << elc->size());
    ATH_MSG_INFO("muon size = " << muc->size());

    m_n_mu->Fill(muc->size());
    m_n_elc->Fill(elc->size());

    for (int i=0; i<muc->size(); i++){
        for (int j=0; j<i; j++) {
            //ATH_MSG_INFO("muon1 = " << muc->at(i) << ", muon2 = " << muc->at(j));
            //ATH_MSG_INFO("combinedtp1 = " << muc->at(i)->combinedTrackParticleLink() << ", combinedtp2 = " << muc->at(j)->combinedTrackParticleLink());
            ATH_MSG_INFO("idtp1 = " << muc->at(i)->inDetTrackParticleLink() << ", idtp2 = " << muc->at(j)->inDetTrackParticleLink());

            xAOD::TrackParticle* tr_ptr1 = new xAOD::TrackParticle();
            xAOD::TrackParticle* tr_ptr2 = new xAOD::TrackParticle();

            ElementLink<xAOD::TrackParticleContainer> idtpLink1 = muc->at(i)->inDetTrackParticleLink();
            ElementLink<xAOD::TrackParticleContainer> idtpLink2 = muc->at(j)->inDetTrackParticleLink();

            if(idtpLink1.isValid() and idtpLink2.isValid()){

                xAOD::safeDeepCopy(**muc->at(i)->inDetTrackParticleLink(), *tr_ptr1);
                xAOD::safeDeepCopy(**muc->at(j)->inDetTrackParticleLink(), *tr_ptr2);

                ATH_MSG_INFO("copy1 = " << tr_ptr1 << ", copy2 = " << tr_ptr2);

                //auto fit = m_vertexer->FitVertex(**tr_ptr1, **tr_ptr2, pv_pos);
                //if(!fit) continue;
                //ATH_MSG_INFO("found vertex = " << fit);
            }



        }
    }



//    for(auto mu: *muc) {
//        const xAOD::TruthParticle* matched_truth_muon=0;
//        // access link to matched truth muon
//        if(mu->isAvailable<ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink")) {
//            ElementLink<xAOD::TruthParticleContainer> link = mu->auxdata<ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink");
//            // check if the link is valid
//            if(link.isValid()) {
//                matched_truth_muon = *link;
//                // return true if at least one truthlink of reco muon matches with given truth
//                if (matched_truth_muon->barcode() == tp->barcode()) return true;
//            } // end of link.isValid
//        } // end of if mu->isAvailable
//    } // end of loop over muon container



    return StatusCode::SUCCESS;
}

StatusCode FlipBkgEst::beginInputFile() { 
    return StatusCode::SUCCESS;
}


bool FlipBkgEst::PassCosmicVeto(const DataVector<xAOD::Muon> dv_muc, const DataVector<xAOD::Electron> dv_elc, std::string channel) {
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


