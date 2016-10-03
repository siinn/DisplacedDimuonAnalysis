// DisplacedDimuonAnalysis includes
#include "MuonEfficiency.h"

// plotting
#include "GaudiKernel/ITHistSvc.h"
#include "TProfile.h"

// xAOD
#include "xAODTruth/TruthVertex.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/MuonAuxContainer.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODTruth/TruthParticleContainer.h"

#include <vector>


using std::pair;
using std::make_pair;

MuonEfficiency::MuonEfficiency( const std::string& name, ISvcLocator* pSvcLocator ) :
AthAnalysisAlgorithm( name, pSvcLocator ),
m_dvutils("DVUtils")
{

    // initialize tools
    declareProperty("DVUtils", m_dvutils);

}


MuonEfficiency::~MuonEfficiency() {}


StatusCode MuonEfficiency::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");
    
    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());
   
    // signal tracking efficiency
    m_eff_eta_01 = new TProfile("m_eff_eta_01", "Muon tracking efficiency vs eta, dr < 0.01", 50,-3.0,3.0);
    m_eff_pt_01 = new TProfile("m_eff_pt_01", "Muon tracking efficiency vs pt, dr < 0.01", 50,0,300);

    // output 
    CHECK( histSvc->regHist("/DV/Muons/Efficiency/eff_eta_01", m_eff_eta_01) );
    CHECK( histSvc->regHist("/DV/Muons/Efficiency/eff_pt_01", m_eff_pt_01) );

    int signal_truth = 0;
    int signal_reconstructed = 0;
    
    return StatusCode::SUCCESS;
}

StatusCode MuonEfficiency::finalize() {

    ATH_MSG_INFO ("Finalizing " << name() << "...");

    // counting number of signal muons
    ATH_MSG_INFO( "Number of signal truth = " << signal_truth);
    ATH_MSG_INFO( "Number of signal reconstructed = " << signal_reconstructed);


  return StatusCode::SUCCESS;
}

StatusCode MuonEfficiency::execute() {  
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    // get truth container
    const xAOD::TruthParticleContainer* truthParticles = nullptr;
    CHECK( evtStore()->retrieve( truthParticles, "MuonTruthParticles" ) );

    const xAOD::MuonContainer* muc = nullptr;
    CHECK( evtStore()->retrieve( muc, "Muons" ) );
   
    // main muon truth loop for efficiency 
    if (truthParticles) {
        for(auto mu_truth: *truthParticles) {
    
            // truth selection 
            //if (!TrackSelection(mu_truth) continue;    // turn off truth selection for now

            // select signal truth
            if (!isSignal(mu_truth)) continue;
            else {signal_truth++;}
            
            bool reconstructed_01 = false;

            if (m_dvutils->IsReconstructedAsMuon(*mu_truth)) {
                signal_reconstructed++;
                reconstructed_01 = true;
            } // end of IsReconstructedAsMuon

            // fill efficiency
            m_eff_eta_01->Fill(mu_truth->eta(), reconstructed_01);
            m_eff_pt_01->Fill(mu_truth->pt() / 1000., reconstructed_01);

        } // end of truth muon loop
    } // end of truth muon
    
    return StatusCode::SUCCESS;
}

StatusCode MuonEfficiency::beginInputFile() { 

  return StatusCode::SUCCESS;
}

bool MuonEfficiency::isSignal (const xAOD::TruthParticle* p) {
    if ( (p->status() ==1) and (p->absPdgId() == 13) and (p->barcode() < 200000) ){
        const xAOD::TruthParticle *parent = p;
        do {
            parent = parent->parent();
            if (parent->absPdgId() == 32) {
                return true;
            }
        } while (parent->parent() != NULL );
    } // end of muon
    return false;
} // end of isSignal

bool MuonEfficiency::TrackSelection (const xAOD::TruthParticle* tp) {
    float maxEta = 2.5;
    float minPt = 1000;
    if ( (tp->pt()>1e-7 ? (fabs(tp->eta()) < maxEta) : false) &&  \
         (tp->pt() > minPt) ) return true;
    else return false;
} // end of TrackSelection

