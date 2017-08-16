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
    m_mumu_DeltaR_low = new TH1F("mumu_DeltaR_low","DeltaR (low)  of muon pair", 50, 0.,1);
    m_mumu_Rcos = new TH1F("mumu_Rcos","Rcos of muon pair", 50, 0., 5.);
    m_mumu_Rcos_low = new TH1F("mumu_Rcos_low","Rcos (low)  of muon pair", 25, 0., 0.1);
    m_mumu_DeltaR_Rcos = new TH2F("mumu_DeltaR_Rcos","#DeltaR vs R_{CR}", 500,0,5,100,0,5);

    // register output
    CHECK( histSvc->regHist("/DV/cosmic_bkg/event_cutflow", m_event_cutflow) );
    CHECK( histSvc->regHist("/DV/cosmic_bkg/mumu_DeltaR", m_mumu_DeltaR) );
    CHECK( histSvc->regHist("/DV/cosmic_bkg/mumu_DeltaR_low", m_mumu_DeltaR_low) );
    CHECK( histSvc->regHist("/DV/cosmic_bkg/mumu_Rcos", m_mumu_Rcos) );
    CHECK( histSvc->regHist("/DV/cosmic_bkg/mumu_Rcos_low", m_mumu_Rcos_low) );
    CHECK( histSvc->regHist("/DV/cosmic_bkg/mumu_DeltaR_Rcos", m_mumu_DeltaR_Rcos) );

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

    if(!trig_passed) return StatusCode::SUCCESS;
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

    // containers to hold two leading muons
    auto mu_sel     = new xAOD::TrackParticleContainer();
    auto mu_sel_aux = new xAOD::TrackParticleAuxContainer();
    mu_sel->setStore(mu_sel_aux);


    // vector to hold two leading muons
    std::vector<const xAOD::IParticle*> leading_muons;
   
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
        if(!VertexTrackSelection(*mu_tr)) continue;


        // adding muon to leading muon vector
        if(leading_muons.size() < 2) {
            ATH_MSG_DEBUG("Adding muon with pt = " << mu_tr->pt());
            leading_muons.push_back(mu_tr);
        }

        // if leading muon is full, replace the lowest pt muon
        else if(leading_muons.size() == 2) {

            // retrieve muon pt
            float mu_current_pt = mu_tr->pt();
            float mu1_pt = leading_muons.at(0)->pt();
            float mu2_pt = leading_muons.at(1)->pt();
            ATH_MSG_DEBUG("(before)muon 1 pt = " << mu1_pt << ", muon 2 pt = " << mu2_pt);
            ATH_MSG_DEBUG("current muon pt = " << mu_current_pt);

            // replace lowest pt muon
            if((mu1_pt > mu2_pt) and (mu_current_pt > mu2_pt)) {
                leading_muons.at(1) = mu_tr;
            }
            else if((mu1_pt < mu2_pt) and (mu_current_pt > mu1_pt)) {
                leading_muons.at(0) = mu_tr;
            }

        }
    }

    // having 2 leading muons, now fill histograms
    if(leading_muons.size() == 2) {

        // define TLorentzVector of decay particles
        TLorentzVector tlv_tp0;
        TLorentzVector tlv_tp1;
        tlv_tp0 = leading_muons.at(0)->p4();
        tlv_tp1 = leading_muons.at(1)->p4();

        float deltaPhiMinusPi = std::fabs(std::fabs(tlv_tp0.DeltaPhi(tlv_tp1)) - std::acos(-1.));
        float sumEta = tlv_tp0.Eta() + tlv_tp1.Eta();
        float Rcos = std::sqrt(sumEta * sumEta + deltaPhiMinusPi * deltaPhiMinusPi);
        float deltaR = tlv_tp0.DeltaR(tlv_tp1);

        float mu1_pt = leading_muons.at(0)->pt();
        float mu2_pt = leading_muons.at(1)->pt();

        ATH_MSG_DEBUG("Rcos = " << Rcos << ", delta R = " << deltaR);

        m_mumu_Rcos->Fill(Rcos);
        m_mumu_Rcos_low->Fill(Rcos);
        m_mumu_DeltaR->Fill(deltaR); 
        m_mumu_DeltaR_low->Fill(deltaR); 
        m_mumu_DeltaR_Rcos->Fill(deltaR,Rcos); 
    }

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
