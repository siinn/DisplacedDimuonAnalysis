// DisplacedDimuonAnalysis includes
#include "MuonPlots.h"

//#include "xAODEventInfo/EventInfo.h"

#include "TH1F.h"

// plotting
#include "GaudiKernel/ITHistSvc.h"

// xAOD
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/MuonAuxContainer.h"


MuonPlots::MuonPlots( const std::string& name, ISvcLocator* pSvcLocator ) :
AthAnalysisAlgorithm( name, pSvcLocator ),
m_tmt("Trig::MatchingTool/MyMatchingTool")
{
    declareProperty("TriggerMatchingTool", m_tmt);
}


MuonPlots::~MuonPlots() {}


StatusCode MuonPlots::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");

    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    // muon distribution
    m_mu_pt = new TH1F("m_mu_pt", "muon pt", 100,0,100); // GeV
    m_mu_pt_low = new TH1F("m_mu_pt_low", "muon pt_low", 100,0,10); // GeV
    m_mu_trig_matched_pt = new TH1F("m_mu_trig_matched_pt", "trig matched muon pt", 100,0,100); // GeV
    m_mu_trig_matched_pt_low = new TH1F("m_mu_trig_matched_pt_low", "trig matched muon pt_low", 100,0,10); // GeV
    m_mu_trig_matched_pt_msonly = new TH1F("m_mu_trig_matched_pt_msonly", "trig matched muon pt msonly", 100,0,100); // GeV
    m_mu_trig_matched_pt_low_msonly = new TH1F("m_mu_trig_matched_pt_low_msonly", "trig matched muon pt_low msonly", 100,0,10); // GeV

    CHECK( histSvc->regHist("/DV/muons/reconstructed/mu_pt", m_mu_pt) );
    CHECK( histSvc->regHist("/DV/muons/reconstructed/mu_pt_low", m_mu_pt_low) );
    CHECK( histSvc->regHist("/DV/muons/reconstructed/mu_trig_matched_pt", m_mu_trig_matched_pt) );
    CHECK( histSvc->regHist("/DV/muons/reconstructed/mu_trig_matched_pt_low", m_mu_trig_matched_pt_low) );
    CHECK( histSvc->regHist("/DV/muons/reconstructed/mu_trig_matched_pt_msonly", m_mu_trig_matched_pt_msonly) );
    CHECK( histSvc->regHist("/DV/muons/reconstructed/mu_trig_matched_pt_low_msonly", m_mu_trig_matched_pt_low_msonly) );
    
    return StatusCode::SUCCESS;
}

StatusCode MuonPlots::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");
    return StatusCode::SUCCESS;
}

StatusCode MuonPlots::execute() {  
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    const xAOD::MuonContainer* muc = nullptr;
    CHECK( evtStore()->retrieve( muc, "Muons" ) );
   
    // plotting muon distribution
    for (auto mu: *muc){

        float mu_pt = mu->pt() / 1000.; // GeV

        m_mu_pt->Fill(mu_pt);
        m_mu_pt_low->Fill(mu_pt);

        // trigger matching
        if (m_tmt->match(*mu,"HLT_mu60_0eta105_msonly")) {

            m_mu_trig_matched_pt->Fill(mu_pt);
            m_mu_trig_matched_pt_low->Fill(mu_pt);

            // plotting ms pt
            if (mu->muonSpectrometerTrackParticleLink().isValid()){
                const xAOD::TrackParticle* mu_mstp = mu->trackParticle(xAOD::Muon::MuonSpectrometerTrackParticle);
                float mu_pt_msonly = mu_mstp->pt() / 1000.;
                m_mu_trig_matched_pt_msonly->Fill(mu_pt_msonly);
                m_mu_trig_matched_pt_low_msonly->Fill(mu_pt_msonly);
            }
        }

    }
    return StatusCode::SUCCESS;
}
