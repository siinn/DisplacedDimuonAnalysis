// DisplacedDimuonAnalysis includes
#include "CosmicBackground.h"

#include "TH1F.h"
#include "AthenaKernel/errorcheck.h"
#include "GaudiKernel/ITHistSvc.h"
#include "TH1F.h"

// xAOD
#include "xAODEventInfo/EventInfo.h"
#include "xAODBase/IParticleHelpers.h"
#include "xAODCore/ShallowCopy.h"
#include "xAODCore/tools/SafeDeepCopy.h"
#include "xAODTracking/TrackParticleContainer.h"
#include "xAODTracking/TrackParticleAuxContainer.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthVertex.h"
#include "xAODTruth/TruthVertexContainer.h"
#include "xAODEgamma/ElectronxAODHelpers.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/MuonAuxContainer.h"

// tools
#include "PathResolver/PathResolver.h"

// for M_PI
#include "cmath"

// debug
#include <typeinfo>


CosmicBackground::CosmicBackground( const std::string& name, ISvcLocator* pSvcLocator ):
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
m_trig("DDL::TrigMatch/TrigMatch"),
m_or("DDL::OverlapRemoval/OverlapRemoval"),
m_accMu("DDL_Muons"),
m_accEl("DDL_Electrons"),
m_acc_p4("DDL_p4"),
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
    declareProperty("TrigMatch", m_trig);
    declareProperty("DiLepCosmics", m_cos);
    declareProperty("OverlapRemoval", m_or);
}


CosmicBackground::~CosmicBackground() {}


StatusCode CosmicBackground::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");

    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    // event cut flow
    m_event_cutflow = new TH1D( "m_event_cutflow", "Event cutflow", 4,0,4);
    m_mumu_DeltaR = new TH1F("mumu_DeltaR","DeltaR of muon pair", 100, 0., 5.);
    m_mumu_DeltaR_low = new TH1F("mumu_DeltaR_low","DeltaR (low)  of muon pair", 60, 0.,1);
    m_mumu_Rcos = new TH1F("mumu_Rcos","Rcos of muon pair", 50, 0., 5.);
    m_mumu_Rcos_low = new TH1F("mumu_Rcos_low","Rcos (low)  of muon pair", 1000, 0., 0.1);
    m_mumu_DeltaR_Rcos = new TH2F("mumu_DeltaR_Rcos","#DeltaR vs R_{CR}", 100,0,5,100,0,5);
    m_mumu_NcosmicPair_DeltaR = new TH2F("mumu_NcosmicPair_DeltaR","#DeltaR vs cosmicPair muon",1000,0,5, 3,0,3);
    m_mumu_NcosmicVertex_DeltaR = new TH2F("mumu_NcosmicVertex_DeltaR","#DeltaR vs cosmicVertex muon",1000,0,5, 3,0,3);

    // register output
    CHECK( histSvc->regHist("/DV/cosmicBkg/event_cutflow", m_event_cutflow) );
    CHECK( histSvc->regHist("/DV/cosmicBkg/mumuPair/DeltaR", m_mumu_DeltaR) );
    CHECK( histSvc->regHist("/DV/cosmicBkg/mumuPair/DeltaR_low", m_mumu_DeltaR_low) );
    CHECK( histSvc->regHist("/DV/cosmicBkg/mumuPair/Rcos", m_mumu_Rcos) );
    CHECK( histSvc->regHist("/DV/cosmicBkg/mumuPair/Rcos_low", m_mumu_Rcos_low) );
    CHECK( histSvc->regHist("/DV/cosmicBkg/mumuPair/DeltaR_Rcos", m_mumu_DeltaR_Rcos) );
    CHECK( histSvc->regHist("/DV/cosmicBkg/mumu/m_mumu_NcosmicPair_DeltaR", m_mumu_NcosmicPair_DeltaR) );
    CHECK( histSvc->regHist("/DV/cosmicBkg/mumu/m_mumu_NcosmicVertex_DeltaR", m_mumu_NcosmicVertex_DeltaR) );

    // flag to check MC
    bool isMC;

    return StatusCode::SUCCESS;
}

StatusCode CosmicBackground::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");
    return StatusCode::SUCCESS;
}

StatusCode CosmicBackground::execute() { 
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    // set cosmic veto threshold
    float Rcos_min = 0.04;
    float Rcos_min_pair = 0.004;
    float deltaR_min = 0.5;

    // retrieve event info
    const xAOD::EventInfo* evtInfo = nullptr;
    CHECK( evtStore()->retrieve( evtInfo, "EventInfo" ) );

    // flag to check if data or MC
    isMC = evtInfo->eventType(xAOD::EventInfo::IS_SIMULATION);

    // flag to check trigger
    bool trig_passed = false;

    m_event_cutflow->Fill("AllEvents", 1);

    // GRL
    if (!isMC and !m_grlTool->passRunLB(*evtInfo)) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("GRL (Data)", 1);

    // event cleaning
    if(!m_evtc->PassEventCleaning(*evtInfo)) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("EvtCleaning (Data)", 1);

    // trigger check
    if (m_tdt->isPassed("HLT_mu60_0eta105_msonly")) trig_passed = true;
    if (m_tdt->isPassed("HLT_g140_loose")) trig_passed = true;
    if (m_tdt->isPassed("HLT_2g50_loose")) trig_passed = true;
    if (m_tdt->isPassed("HLT_2g60_loose_L12EM15VH")) trig_passed = true;

    //if(!trig_passed) return StatusCode::SUCCESS;
    m_event_cutflow->Fill("Trig", 1);

    // retrieve muon and electron container
    const xAOD::MuonContainer* muc = nullptr;
    CHECK( evtStore()->retrieve( muc, "Muons" ) );

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

    // containers to hold two leading muons
    auto mu_sel     = new xAOD::TrackParticleContainer();
    auto mu_sel_aux = new xAOD::TrackParticleAuxContainer();
    mu_sel->setStore(mu_sel_aux);


    // vector to hold two leading muons
    //std::vector<const xAOD::IParticle*> leading_muons;
   
    // plotting muon distribution
    for (auto mu: *muc_copy.first){

        // overlap removal
        if(m_or->IsOverlap(*mu)) continue;

        // apply muon selection
        if(!m_leptool->MuonSelection(*mu)) continue;

        // access muon ID track
        auto mu_tr = mu->trackParticle(xAOD::Muon::InnerDetectorTrackParticle);
        if(mu_tr == nullptr) continue;

        // vertex track selection
        //if(!VertexTrackSelection(*mu_tr)) continue;

        // copy ID track
        xAOD::TrackParticle* tr_ptr = new xAOD::TrackParticle();
        mu_sel->push_back(tr_ptr);
        xAOD::safeDeepCopy(*mu_tr, *tr_ptr);

        // decorate ID track with muon p4
        m_acc_p4(*tr_ptr) = mu->p4();

    }

    //===============================================================================
    // loop over all possible combination of muons passing vertex track selection cut
    //===============================================================================
    // this is first round to mark cosmic muons
    if(mu_sel->size() > 1) {
        // perform vertexing
        for(auto mu1_itr = mu_sel->begin(); mu1_itr != mu_sel->end(); mu1_itr++)
        {
            for(auto mu2_itr = mu1_itr+1; mu2_itr != mu_sel->end(); mu2_itr++)
            {
                float Rcos = GetRcos(**mu1_itr, **mu2_itr);

                // require opposite charges
                if((*mu1_itr)->charge() * (*mu2_itr)->charge() > 0) continue;

                // check if this pair is from a cosmic muon
                if(Rcos > Rcos_min_pair) continue;
                m_mumu_Rcos->Fill(Rcos);
                m_mumu_Rcos_low->Fill(Rcos);

                // mark two muons as cosmic muon
                (*mu1_itr)->auxdecor<int>("cosmic") = 1;
                (*mu2_itr)->auxdecor<int>("cosmic") = 1;

                // add number of cosmic muon pair
                m_n_cosmicPair_muon++;

            }
        }
    }

    // second round to plot DeltaR between two cosmic muons
    if(mu_sel->size() > 1) {
        // perform vertexing
        for(auto mu1_itr = mu_sel->begin(); mu1_itr != mu_sel->end(); mu1_itr++)
        {
            for(auto mu2_itr = mu1_itr+1; mu2_itr != mu_sel->end(); mu2_itr++)
            {
                // require opposite charges
                if((*mu1_itr)->charge() * (*mu2_itr)->charge() > 0) continue;

                // check if muons are cosmic
                static SG::AuxElement::ConstAccessor<int> acc_cosmic("cosmic");
                bool cosmic1 = acc_cosmic.isAvailable(**mu1_itr);
                bool cosmic2 = acc_cosmic.isAvailable(**mu2_itr);

                float DeltaR = GetDeltaR(**mu1_itr, **mu2_itr);
                float Rcos = GetRcos(**mu1_itr, **mu2_itr);

                // two muons are both cosmic and not from the same cosmic muon
                if((cosmic1) and (cosmic2) and (Rcos > Rcos_min_pair)) {
                    m_mumu_DeltaR->Fill(DeltaR);
                    m_mumu_DeltaR_low->Fill(DeltaR);
                }
            }
        }
    }

    //===============================================
    // count number of cosmic vertex in this event
    //===============================================
    for(auto dv: *dvc_copy.first) {

        // mass cut
        float mass_min = 10.;

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

        // remove overlapping muon
        m_dilepdvc->ApplyOverlapRemoval(*dv);

        // remove bad electrons
        //m_leptool->BadClusterRemoval(*dv);

        // kinematic cut
        m_leptool->ElectronKinematicCut(*dv);

        // Electron identification
       //m_leptool->ElectronID(*dv);

        // muon selection tool
        m_leptool->MuonSelection(*dv);

        // select only vertex with tracks
        if(dv->trackParticleLinks().size() != 2) continue;

        // find decay channel of dv
        std::string channel = m_dvutils->DecayChannel(*dv);

        if (channel == "mumu") {

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

            // material veto (only e)
            // no material veto for muon

            // low mass veto
            if(dv_mass < mass_min) continue;

            //========================================================
            // select cosmic muon pair
            // notice that cut is inverted to select cosmic muon vertex
            //========================================================
            if(GetRcos(tp1,tp2) > Rcos_min) continue;

            // add number of cosmic muon pair
            m_n_cosmicVertex_muon++;


        } // end of mumu
    }
    //------------------------------
    // dv cut flow
    //------------------------------
    for(auto dv: *dvc_copy.first) {

        // mass cut
        float mass_min = 10.;

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

        // remove overlapping muon
        m_dilepdvc->ApplyOverlapRemoval(*dv);

        // remove bad electrons
        //m_leptool->BadClusterRemoval(*dv);

        // kinematic cut
        m_leptool->ElectronKinematicCut(*dv);

        // Electron identification
        //m_leptool->ElectronID(*dv);

        // muon selection tool
        m_leptool->MuonSelection(*dv);

        // select only vertex with tracks
        if(dv->trackParticleLinks().size() != 2) continue;

        // find decay channel of dv
        std::string channel = m_dvutils->DecayChannel(*dv);

        if (channel == "mumu") {

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

            // material veto (only e)
            // no material veto for muon

            // low mass veto
            if(dv_mass < mass_min) continue;

            // cosmic veto
            if(GetRcos(tp1,tp2) < Rcos_min) continue;

            // fill number of cosmic muon vs deltaR
            float DeltaR = GetDeltaR(tp1,tp2);
            m_mumu_NcosmicPair_DeltaR->Fill(DeltaR,m_n_cosmicPair_muon);
            m_mumu_NcosmicVertex_DeltaR->Fill(DeltaR,m_n_cosmicVertex_muon);

        } // end of mumu
    }


    // having 2 leading muons, now fill histograms
    //if(leading_muons.size() == 2) {

    //    // define TLorentzVector of decay particles
    //    TLorentzVector tlv_tp0;
    //    TLorentzVector tlv_tp1;
    //    tlv_tp0 = leading_muons.at(0)->p4();
    //    tlv_tp1 = leading_muons.at(1)->p4();

    //    float deltaPhiMinusPi = std::fabs(std::fabs(tlv_tp0.DeltaPhi(tlv_tp1)) - std::acos(-1.));
    //    float sumEta = tlv_tp0.Eta() + tlv_tp1.Eta();
    //    float Rcos = std::sqrt(sumEta * sumEta + deltaPhiMinusPi * deltaPhiMinusPi);
    //    float deltaR = tlv_tp0.DeltaR(tlv_tp1);

    //    float mu1_pt = leading_muons.at(0)->pt();
    //    float mu2_pt = leading_muons.at(1)->pt();

    //    ATH_MSG_DEBUG("Rcos = " << Rcos << ", delta R = " << deltaR);

    //    m_mumu_Rcos->Fill(Rcos);
    //    m_mumu_Rcos_low->Fill(Rcos);
    //    m_mumu_DeltaR->Fill(deltaR); 
    //    m_mumu_DeltaR_low->Fill(deltaR); 
    //    m_mumu_DeltaR_Rcos->Fill(deltaR,Rcos); 
    //}

    return StatusCode::SUCCESS;
}


bool CosmicBackground::VertexTrackSelection(const xAOD::TrackParticle& tr){

    bool Pass = true;

    // vertex selection cuts
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


    return Pass;
}


float CosmicBackground::GetRcos(xAOD::TrackParticle& tr0, xAOD::TrackParticle& tr1){

    // define TLorentzVector of decay particles
    TLorentzVector tlv_tp0;
    TLorentzVector tlv_tp1;
    tlv_tp0 = tr0.p4();
    tlv_tp1 = tr1.p4();

    float deltaPhiMinusPi = std::fabs(std::fabs(tlv_tp0.DeltaPhi(tlv_tp1)) - std::acos(-1.));
    float sumEta = tlv_tp0.Eta() + tlv_tp1.Eta();
    float Rcos = std::sqrt(sumEta * sumEta + deltaPhiMinusPi * deltaPhiMinusPi);

    return Rcos;
}

float CosmicBackground::GetDeltaR(xAOD::TrackParticle& tr0, xAOD::TrackParticle& tr1){

    // define TLorentzVector of decay particles
    TLorentzVector tlv_tp0;
    TLorentzVector tlv_tp1;

    // define TLorentzVector of decay particles
    tlv_tp0 = tr0.p4();
    tlv_tp1 = tr1.p4();

    float deltaR = tlv_tp0.DeltaR(tlv_tp1);

    return deltaR;
}

