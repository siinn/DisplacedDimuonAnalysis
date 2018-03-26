#include "LeptonCount.h"

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
#include "xAODTracking/TrackParticleContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthVertex.h"
#include "xAODTruth/TruthVertexContainer.h"
#include "AthAnalysisBaseComps/AthAnalysisHelper.h"
#include "xAODEgamma/ElectronxAODHelpers.h" 

// tools
#include "PathResolver/PathResolver.h"

// for M_PI
#include "cmath"

// debug
#include <typeinfo>

LeptonCount::LeptonCount( const std::string& name, ISvcLocator* pSvcLocator ) :
AthAnalysisAlgorithm( name, pSvcLocator ),
m_dilepdvc("DDL::DiLepDVCuts/DiLepDVCuts"),
m_evtc("DDL::EventCuts/DiLepEventCuts"),
m_dvc("DDL::DVCuts/DiLepBaseCuts"),
m_dvutils("DVUtils"),
m_leptool("LeptonSelectionTools"),
m_fmtool("FilterMatchingTools"),
m_costool("CosmicTools"),
m_grlTool("GoodRunsListSelectionTool/GRLTool"),
m_tdt("Trig::TrigDecisionTool/TrigDecisionTool"),
m_tmt("Trig::MatchingTool/TrigMatchingTool"),
m_or("DDL::OverlapRemoval/OverlapRemoval"),
m_accMu("DDL_Muons"),
m_accEl("DDL_Electrons"),
m_cos("DDL::DiLepCosmics/DiLepCosmics"),
m_phmatch("DDL::PhotonMatch/PhotonMatch"),
m_elt("AsgElectronLikelihoodTool/ElectronLikelihoodTool"),
m_mct("CP::MuonCalibrationAndSmearingTool/MuonCorrectionTool"),
m_mst("CP::MuonSelectionTool/MuonSelectionTool"),
m_accMass("mass")
{
    // initialize tools
    declareProperty("DVUtils", m_dvutils);
    declareProperty("LeptonSelectionTool", m_leptool);
    declareProperty("FilterMatchingTool", m_fmtool);
    declareProperty("CosmicTool", m_costool);
    declareProperty("GRLTool",  m_grlTool, "The private GoodRunsListSelectionTool" );
    declareProperty("TrigDecisionTool", m_tdt);
    declareProperty("TrigMatchingTool", m_tmt);
    declareProperty("OverlapRemoval", m_or);
    declareProperty("DiLepCosmics", m_cos);
    declareProperty("DiLepEventCuts", m_evtc);
    declareProperty("DiLepBaseCuts", m_dvc);
    declareProperty("DiLepDVCuts", m_dilepdvc);
    declareProperty("PhotonMatch", m_phmatch);
    declareProperty("ElectronLikelihoodTool", m_elt);
    declareProperty("MuonCorrectionTool", m_mct);
    declareProperty("MuonSelectionTool", m_mst);


}


LeptonCount::~LeptonCount() {}


StatusCode LeptonCount::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");

    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    // cut flow
    m_event_cutflow = new TH1D( "m_event_cutflow", "Event cutflow", 5,0,5);
    m_muon_cutflow = new TH1D( "m_muon_cutflow", "muon cutflow", 10,0,10);
    m_electron_cutflow = new TH1D( "m_electron_cutflow", "electron cutflow", 10,0,10);
    m_non_lepton_cutflow = new TH1D( "m_non_lepton_cutflow", "non_lepton cutflow", 10,0,10);

    CHECK( histSvc->regHist("/DV/lepton_count/event_cutflow", m_event_cutflow) );
    CHECK( histSvc->regHist("/DV/lepton_count/muon_cutflow", m_muon_cutflow) );
    CHECK( histSvc->regHist("/DV/lepton_count/electron_cutflow", m_electron_cutflow) );
    CHECK( histSvc->regHist("/DV/lepton_count/non_lepton_cutflow", m_non_lepton_cutflow) );

    // pile-up distribution
    m_pileup = new TH1F("m_pileup", "m_pileup", 100, 0, 100); 
    CHECK( histSvc->regHist("/DV/lepton_count/pileup", m_pileup) );


    return StatusCode::SUCCESS;
}

StatusCode LeptonCount::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");
    return StatusCode::SUCCESS;
}

StatusCode LeptonCount::execute() {

    ATH_MSG_DEBUG ("Executing " << name() << "...");

    // retrieve event info
    const xAOD::EventInfo* evtInfo = nullptr;
    CHECK( evtStore()->retrieve( evtInfo, "EventInfo" ) );

    // flag to check if data or MC
    bool isMC = evtInfo->eventType(xAOD::EventInfo::IS_SIMULATION);

    // pile-up
    int pileup = evtInfo->actualInteractionsPerCrossing();
    m_pileup->Fill(pileup);

    // retrieve lepton container
    const xAOD::MuonContainer* muc = nullptr;
    CHECK( evtStore()->retrieve( muc, "Muons" ));

    const xAOD::ElectronContainer* elc = nullptr;
    CHECK( evtStore()->retrieve( elc, "Electrons" ));

    const xAOD::PhotonContainer* phc = nullptr;
    CHECK(evtStore()->retrieve(phc, "Photons"));

    const xAOD::TrackParticleContainer* idc = nullptr;
    CHECK( evtStore()->retrieve( idc, "InDetTrackParticles" ));

    // retrieve primary vertices
    const xAOD::VertexContainer* pvc = nullptr;
    CHECK( evtStore()->retrieve( pvc, "PrimaryVertices" ));

    //bool trig_passed = true;
    bool trig_passed = false;

    // make copies of leptons
    auto muc_copy = xAOD::shallowCopyContainer(*muc);
    xAOD::setOriginalObjectLink(*muc, *muc_copy.first);

    auto elc_copy = xAOD::shallowCopyContainer(*elc);
    xAOD::setOriginalObjectLink(*elc, *elc_copy.first);

    auto idc_copy = xAOD::shallowCopyContainer(*idc);
    xAOD::setOriginalObjectLink(*idc, *idc_copy.first);

    // apply overlap removal
    m_or->FindOverlap(*elc_copy.first, *muc_copy.first);

    // perform matching of photons to electrons
    //m_phmatch->MatchPhotons(*phc, *elc_copy.first);

    //---------------------------------------
    // Event cut flow
    //---------------------------------------

    if(isMC){
        m_event_cutflow->Fill("AllEvents",1);
    }
    else {
        // all events already passed RPVLL filter
        m_event_cutflow->Fill("RPVLLFilter", 1);
    }

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
    if(!trig_passed) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("Trig", 1);

    // cosmic veto
    if(!m_cos->PassCosmicEventVeto(*elc, *muc)) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("CosmicVeto", 1);

    // get primary vertex
    auto pv = m_evtc->GetPV(*pvc);
    if (pv == nullptr) return StatusCode::SUCCESS;

    // get pv position
    auto pv_pos = m_evtc->GetPV(*pvc)->position();

    // PV position < 200 mm
    float pv_z_max = 200.;

    // apply primary vertex position cut
    if (pv) {

        // z_pv cut
        if(std::abs(pv_pos.z()) > pv_z_max) return StatusCode::SUCCESS;
        
    }
    else return StatusCode::SUCCESS;
    m_event_cutflow->Fill("z_{PV} < 200 mm", 1);


    //-----------------------------------
    // lepton cutflow
    //-----------------------------------

    for(auto mu: *muc_copy.first) {

        // all muon
        m_muon_cutflow->Fill("#mu",1);

        // overlap removal
        if(m_or->IsOverlap(*mu)) continue;
        m_muon_cutflow->Fill("OverlapRemoval",1);
    
        //--------------------------------
        // apply muon selection
        //--------------------------------
        // set minimum pt
        float mu_pt_min = 10.;
        float mu_eta_max = 2.5;

        // retrieve muon ID track 
        auto mu_tr = (*mu).trackParticle(xAOD::Muon::InnerDetectorTrackParticle);

        // valid combined muon
        if(!(mu_tr != nullptr && (*mu).muonType() == xAOD::Muon::Combined)) continue;
        m_muon_cutflow->Fill("CombinedMuon",1);

        // retrieve muon pt and eta
        float mu_pt = mu_tr->pt() / 1000.;
        float mu_eta = std::fabs((*mu).eta());

        // muon correction
        if(!(m_mct->applyCorrection(*mu))) continue;
        m_muon_cutflow->Fill("MuonCorrection",1);

        // muon ID
        if(!(m_mst->accept(*mu))) continue;
        m_muon_cutflow->Fill("MuonID",1);

        // muon pt
        if(mu_pt < mu_pt_min) continue;
        m_muon_cutflow->Fill("p_{T} > 10 GeV",1);

        // muon eta
        if(mu_eta > mu_eta_max) continue;
        m_muon_cutflow->Fill("#eta < 2.5",1);

        // VrtSecInclusive requirement
        if(!GoodTrack(*mu_tr)) continue;
        m_muon_cutflow->Fill("VrtSecInclusive",1);

        // mark ID track as muon
        mu_tr->auxdecor<int>("muon") = 1;

    }


    for(auto el: *elc_copy.first) {

        // all electron
        m_electron_cutflow->Fill("e",1);

        // overlap removal
        if(m_or->IsOverlap(*el)) continue;
        m_electron_cutflow->Fill("OverlapRemoval",1);

        // remove bad electrons
        if(!m_leptool->BadClusterRemoval(*el)) continue;
        m_electron_cutflow->Fill("BadCluster",1);

        // kinematic cut
        if(!m_leptool->ElectronKinematicCut(*el)) continue;
        m_electron_cutflow->Fill("p_{T} > 10 GeV, #eta < 2.47",1);

        // Electron identification
        if(!m_leptool->ElectronID(*el)) continue;
        m_electron_cutflow->Fill("ElectronID",1);

        // access electron ID track
        auto el_tr = xAOD::EgammaHelpers::getOriginalTrackParticle(el);
        if(el_tr == nullptr) continue;
        m_electron_cutflow->Fill("OriginalTrack",1);

        // VrtSecInclusive requirement
        if(!GoodTrack(*el_tr)) continue;
        m_electron_cutflow->Fill("VrtSecInclusive",1);

        // mark ID track as electron
        el_tr->auxdecor<int>("electron") = 1;

    }

    for(auto id_tr: *idc_copy.first) {
        
        // all id track
        m_non_lepton_cutflow->Fill("x",1);

        // lepton flag
        bool isLepton = false;

        // get original track
        auto orig_tr = dynamic_cast<const xAOD::TrackParticle*>(xAOD::getOriginalObject(*id_tr));
        if (orig_tr == nullptr) orig_tr = id_tr;
        m_non_lepton_cutflow->Fill("OriginalTrack",1);

        // check if this ID track is a lepton
        if((orig_tr)->auxdecor<int>("electron") or (orig_tr)->auxdecor<int>("muon")) {
            isLepton = true;
        }
        if (isLepton) continue;
        m_non_lepton_cutflow->Fill("NonLeptonic",1);

        // track quality requirement
        if(!GoodTrack(*id_tr)) continue;
        m_non_lepton_cutflow->Fill("VrtSecInclusive",1);

        // track kinematic cut
        if(!m_fmtool->PassTrackKinematic(*id_tr)) continue;
        m_non_lepton_cutflow->Fill("p_{T} > 10 GeV, #eta < 2.5",1);

        // perigee requirement
        //const Trk::Perigee* perigee = &(id_tr->perigeeParameters());

        //if(!perigee) continue; 
        //m_non_lepton_cutflow->Fill("HasPerigee",1);

    }


    return StatusCode::SUCCESS;
}

StatusCode LeptonCount::beginInputFile() { 

  return StatusCode::SUCCESS;
}

bool LeptonCount::GoodTrack(const xAOD::TrackParticle& tr) {

    // default track selections
    float m_trPt = 1000.;
    float m_trChi2 = 50.;
    float m_trD0Min = 2.;
    float m_trD0Max = 300.;
    float m_trZ0Min = 0.;
    float m_trZ0Max = 1500.;
    float m_trD0Err = 200000.;
    float m_trZ0Err = 200000.;
    float m_trSct = 2;
    float m_trShr = 2;
    float m_trPix = 2;
    float m_vxChi2 = 5.;

    // Implementation from: VrtSecInclusive/src/TrackSelectionAlgs.cxx (SelGoodTrkParticle)
    
    // retrieve number of hits
    std::uint8_t nPix = 0;
    if(!tr.summaryValue(nPix, xAOD::numberOfPixelHits))
    {
      ATH_MSG_WARNING("Could not retrieve numberOfPixelHits from track!");
      return false;
    }
    
    std::uint8_t nSCT = 0;
    if(!tr.summaryValue(nSCT, xAOD::numberOfSCTHits))
    {
      ATH_MSG_WARNING("Could not retrieve numberOfSCTHits from track!");
      return false;
    }
    
    std::uint8_t nTRT = 0;
    if(!tr.summaryValue(nTRT, xAOD::numberOfTRTHits))
    {
      ATH_MSG_WARNING("Could not retrieve numberOfTRTHits from track!");
      return false;
    }
    
    std::uint8_t nPixSharedHits = 0;
    if(!tr.summaryValue(nPixSharedHits, xAOD::numberOfPixelSharedHits))
    {
      ATH_MSG_WARNING("Could not retrieve numberOfPixelSharedHits from track!");
      return false;
    }
    
    std::uint8_t nSCTSharedHits = 0;
    if(!tr.summaryValue(nSCTSharedHits, xAOD::numberOfSCTSharedHits))
    {
      ATH_MSG_WARNING("Could not retrieve numberOfSCTSharedHits from track!");
      return false;
    }
    
    // pt cut
    if(tr.pt() < m_trPt) return false;
    
    // fit quality cut
    auto numberDoF = tr.numberDoF();
    if(numberDoF == 0) return false;
    if((tr.chiSquared() / numberDoF) > m_trChi2) return false;
    
    // d0 cut
    double d0 = std::fabs(tr.d0());
    if(d0 < m_trD0Min || d0 > m_trD0Max) return false;
    
    // z0 cut
    double z0 = std::fabs(tr.z0());
    if(z0 < m_trZ0Min || z0 > m_trZ0Max) return false;
    
    // d0 and z0 uncertainties
    auto CovMtx = tr.definingParametersCovMatrix();
    auto d0_sigma2 = CovMtx(0,0);
    auto z0_sigma2 = CovMtx(1,1);
    
    // impact parameter significance cuts
    if(d0_sigma2 > m_trD0Err*m_trD0Err) return false;
    if(z0_sigma2 > m_trZ0Err*m_trZ0Err) return false;
    
    // sct hits cut
    if(nSCT < m_trSct) return false;
    
    // shared hits cut
    if((nPixSharedHits + nSCTSharedHits) > m_trShr) return false;
    
    // pixel hits cut
    if((nTRT == 0) && (nPix < m_trPix)) return false;
    
    return true;
}
