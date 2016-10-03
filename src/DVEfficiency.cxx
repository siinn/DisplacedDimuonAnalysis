//------------------------------------------------------------
// Author : Siinn Che, siinn.che@cern.ch
//
// Calculate efficiency to reconstruct signal DV by comparing
// the truth muon and reco muon from DV with dR method
//------------------------------------------------------------

// DisplacedDimuonAnalysis includes
#include "DVEfficiency.h"

// truth vertex
#include "xAODTruth/TruthVertex.h"
#include "xAODTruth/TruthVertexContainer.h"

// vertex
#include "xAODTracking/Vertex.h"
#include "xAODTracking/VertexContainer.h"

// truth particle
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthParticle.h"

// track particle
#include "xAODTracking/TrackParticleContainer.h"
#include "xAODTracking/TrackParticle.h"

#include "xAODBase/IParticleHelpers.h"
#include "xAODCore/ShallowCopy.h"

#include "TProfile.h"
#include "cmath"

//#include "xAODEventInfo/EventInfo.h"

//uncomment the line below to use the HistSvc for outputting trees and histograms
#include "GaudiKernel/ITHistSvc.h"



DVEfficiency::DVEfficiency( const std::string& name, ISvcLocator* pSvcLocator ) : 
AthAnalysisAlgorithm( name, pSvcLocator ),
m_dvutils("DVUtils"),
m_dilepdvc("DV::DiLepDVCuts/DiLepDVCuts")
{
    declareProperty("DVUtils", m_dvutils);
    declareProperty("DiLepDVCuts", m_dilepdvc);
}


DVEfficiency::~DVEfficiency() {}


StatusCode DVEfficiency::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");
    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    m_dv_cutflow = new TH1D( "m_dv_cutflow", "Signal truth dv cutflow", 10,0,10);
    m_dv_eff_eta = new TProfile( "m_dv_eff_eta", "DV reconstruction efficiency vs eta", 50, -3.0, 3.0);
    m_dv_eff_phi = new TProfile( "m_dv_eff_phi", "DV reconstruction efficiency vs phi", 50, -M_PI, M_PI);
    m_dv_eff_mass = new TProfile( "m_dv_eff_mass", "DV reconstruction efficiency vs mass", 50, 0, 400); // GeV
    m_dv_eff_R = new TProfile( "m_dv_eff_R", "DV reconstruction efficiency vs R", 50, 0, 400); // mm

    CHECK( histSvc->regHist("/DV/TruthVertex/SignalCutFlow/dv_cutflow", m_dv_cutflow) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/Efficiency/dv_eff_eta", m_dv_eff_eta) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/Efficiency/dv_eff_phi", m_dv_eff_phi) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/Efficiency/dv_eff_mass", m_dv_eff_mass) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/Efficiency/dv_eff_R", m_dv_eff_R) );
   

    int n_truth_siganl_dv = 0;
    int n_matched_signal_dv = 0;
    // number of truth dv that passes acceptance cut
    // i.e. both muons must have eta < 2.4
    int n_accepted = 0;

    return StatusCode::SUCCESS;
}

StatusCode DVEfficiency::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");
    ATH_MSG_INFO ("Number of signal DV = " << n_truth_siganl_dv );
    ATH_MSG_INFO ("Number of signal DV accepted within detector volume = " << n_accepted );
    ATH_MSG_INFO ("Number of matched signal DV = " << n_matched_signal_dv );
    
    return StatusCode::SUCCESS;
}

StatusCode DVEfficiency::execute() {  
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    // retrieve truth vertex container
    const xAOD::TruthVertexContainer* tru_vc = nullptr;
    CHECK( evtStore()->retrieve( tru_vc, "TruthVertices"));

    // retrieve secondary vertices
    const xAOD::VertexContainer* dvc = nullptr;
    CHECK( evtStore()->retrieve( dvc, "VrtSecInclusive_SecondaryVertices" ));

    // make a copy of vertex containers
    auto dvc_copy = xAOD::shallowCopyContainer(*dvc);

    // retrieve lepton container
    const xAOD::MuonContainer* muc = nullptr;
    CHECK( evtStore()->retrieve( muc, "Muons" ));

    // make copies of leptons
    auto muc_copy = xAOD::shallowCopyContainer(*muc);
    xAOD::setOriginalObjectLink(*muc, *muc_copy.first);

    // perform lepton matching
    for(auto dv: *dvc_copy.first)
    {
        m_dilepdvc->ApplyLeptonMatching(*dv, *muc_copy.first);

    }

    // loop over the truth vertex container
    for (auto vertex_truth: *tru_vc){

        // access parent
        const xAOD::TruthParticle* parent = vertex_truth->incomingParticle(0);

        //-----------------------------------------------------------------
        // select signal truth, Z'
        //-----------------------------------------------------------------
        if (!(vertex_truth->nIncomingParticles() == 1)) continue;
        if (!(vertex_truth->nOutgoingParticles() == 2)) continue;
        if (!(parent->absPdgId() ==32)) continue;

        // counting signal dv
        n_truth_siganl_dv++;
        m_dv_cutflow->Fill("Signal", 1);

        // access signal muons
        const xAOD::TruthParticle* truth_child0 = vertex_truth->outgoingParticle(0);
        const xAOD::TruthParticle* truth_child1 = vertex_truth->outgoingParticle(1);

        //-----------------------------------------------------------------
        // acceptance cut
        //-----------------------------------------------------------------
        if ( !((truth_child0->eta()) < 2.4) or !((truth_child1->eta() < 2.4)) ) continue;

        // fill cut flow histogram
        m_dv_cutflow->Fill("Accepted", 1);
        n_accepted++;

        //-----------------------------------------------------------------
        // minimum displacement cut
        //-----------------------------------------------------------------
        if (! (vertex_truth->perp() > 3.)) continue;
        m_dv_cutflow->Fill("MinDist", 1);

        //-----------------------------------------------------------------
        // check if two muons are reconstructed
        //-----------------------------------------------------------------
        if (!(m_dvutils->IsReconstructedAsMuon(*truth_child0) and m_dvutils->IsReconstructedAsMuon(*truth_child0))) continue;
        m_dv_cutflow->Fill("DiMuonReconst.", 1);

        // calculate invariant mass of truth vertex
        TLorentzVector outgoing_tlv;

        // sum four momentum of outgoing particles
        outgoing_tlv = TLorentzVector( truth_child0->px(), truth_child0->py(), truth_child0->pz(), truth_child0->e());
        outgoing_tlv += TLorentzVector( truth_child1->px(), truth_child1->py(), truth_child1->pz(), truth_child1->e());

        bool dv_matched = false;

        // loop over dv's
        for (auto dv: *dvc_copy.first) {

            // set flag
            bool muon_match0 = false;
            bool muon_match1 = false;

            // collect muons from this dv
            auto dv_muc = m_dilepdvc->GetMu(*dv);

            // require dv to have 2 muons
            if (dv_muc->size() != 2) continue;

            // loop over reconstructed dv's to find match
            for(auto mu: *dv_muc){
                // use MS track which is not affected by badly matched ID tracks
                const xAOD::IParticle* mu_ip = nullptr;
                auto mstrk = mu->trackParticle(xAOD::Muon::MuonSpectrometerTrackParticle);
                if(mstrk == nullptr) mu_ip = mu;
                else mu_ip = mstrk;

                // find angle between muons from dv and truth dv
                double dr0 = truth_child0->p4().DeltaR(mu_ip->p4());
                double dr1 = truth_child1->p4().DeltaR(mu_ip->p4());

                // if muons are matched, set flag
                if (dr0 < 0.1) muon_match0 = true;
                if (dr1 < 0.1) muon_match1 = true;

            } // end of muon container

            // if we have two muons matched, this truth dv is reconstructed
            if (muon_match0 and muon_match1) dv_matched = true;

        } // end of loop over dvc

        // fill efficiency plots
        m_dv_eff_eta->Fill( vertex_truth->eta(), dv_matched);
        m_dv_eff_phi->Fill( vertex_truth->phi(), dv_matched);
        m_dv_eff_mass->Fill( outgoing_tlv.M() / 1000. , dv_matched);
        m_dv_eff_R->Fill( vertex_truth->perp() , dv_matched);

        //-----------------------------------------------------------------
        // require reco maching
        //-----------------------------------------------------------------
        if (!dv_matched) continue;

        m_dv_cutflow->Fill("DVReconst.", 1);

        // counting matched dv
        n_matched_signal_dv++;

        ////m_dv_R->Fill( m_dvutils->getR( *dv, *pv ) );    // R in [mm]
    } // end of truth vertex loop
    return StatusCode::SUCCESS;
}

