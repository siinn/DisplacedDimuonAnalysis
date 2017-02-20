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

//#include "xAODBase/IParticleHelpers.h"
//#include "xAODCore/ShallowCopy.h"

#include "TProfile.h"
#include "TProfile2D.h"
#include "cmath"
#include "vector"

//#include "xAODEventInfo/EventInfo.h"

//uncomment the line below to use the HistSvc for outputting trees and histograms
#include "GaudiKernel/ITHistSvc.h"



DVEfficiency::DVEfficiency( const std::string& name, ISvcLocator* pSvcLocator ) : 
AthAnalysisAlgorithm( name, pSvcLocator ),
m_dvutils("DVUtils"),
m_dvc("DV::DVCuts/DiLepBaseCuts"),
m_accMass("mass")
{
    declareProperty("DVUtils", m_dvutils);
    declareProperty("DiLepBaseCuts", m_dvc);
}


DVEfficiency::~DVEfficiency() {}


StatusCode DVEfficiency::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");
    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    m_dv_cutflow = new TH1D( "m_dv_cutflow", "Signal truth dv cutflow", 5,0,5);
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
    CHECK( histSvc->regHist("/DV/TruthVertex/dv_cutflow", m_dv_cutflow) );
    CHECK( histSvc->regHist("/DV/TruthVertex/dv_mass", m_dv_mass) );

    // efficiency plots
    CHECK( histSvc->regHist("/DV/TruthVertex/efficiency/dv/dv_eff_eta", m_dv_eff_eta) );
    CHECK( histSvc->regHist("/DV/TruthVertex/efficiency/dv/dv_eff_phi", m_dv_eff_phi) );
    CHECK( histSvc->regHist("/DV/TruthVertex/efficiency/dv/dv_eff_mass", m_dv_eff_mass) );
    CHECK( histSvc->regHist("/DV/TruthVertex/efficiency/dv/dv_eff_R", m_dv_eff_R) );
    CHECK( histSvc->regHist("/DV/TruthVertex/efficiency/dv/dv_eff_d0", m_dv_eff_d0) );

    CHECK( histSvc->regHist("/DV/TruthVertex/efficiency/zp_parameters/dv_eff_zp_eta", m_dv_eff_zp_eta) );
    CHECK( histSvc->regHist("/DV/TruthVertex/efficiency/zp_parameters/dv_eff_zp_pt", m_dv_eff_zp_pt) );

    return StatusCode::SUCCESS;
}

StatusCode DVEfficiency::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");
    
    return StatusCode::SUCCESS;
}

StatusCode DVEfficiency::execute() {  
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    // retrieve truth vertex container
    const xAOD::TruthVertexContainer* tru_vc = nullptr;
    CHECK( evtStore()->retrieve( tru_vc, "TruthVertices"));

    // vector to store truth vertex and reconstruction flag
    std::vector< std::pair<xAOD::TruthVertex*,bool> > tvv;

    // loop over the truth vertex container
    for (auto tru_v: *tru_vc){

        //-----------------------------------------------------------------
        // select signal dv
        //-----------------------------------------------------------------
        if (!m_dvutils->isSignalVertex(tru_v)) continue;
        m_dv_cutflow->Fill("Signal", 1);

        // fill truth vertex vector, initially 0 means not reconstructed
        tvv.emplace_back(tru_v,false);

        //-----------------------------------------------------------------
        // acceptance cut (both muons eta < 2.4)
        //-----------------------------------------------------------------
        if (!(m_dvutils->PassAcceptance(tru_v))) continue;
        m_dv_cutflow->Fill("#eta_{#mu} < 2.4", 1);

        //-----------------------------------------------------------------
        // check if two muons are reconstructed
        //-----------------------------------------------------------------
        if (!(m_dvutils->MuonsReconstructed(tru_v))) continue;
        m_dv_cutflow->Fill("2 muons reconst.", 1);

        //-----------------------------------------------------------------
        // match truth dv to reco dv
        //-----------------------------------------------------------------
        bool dv_matched = m_dvutils->IsReconstructed(tru_v);

        // fill vector of truth pair to flag if reconstructed
        tvv.back().second = dv_matched;

        // require reco maching
        if (!dv_matched) continue;
        m_dv_cutflow->Fill("DV reconst.", 1);

        //-----------------------------------------------------------------
        // minimum displacement cut
        //-----------------------------------------------------------------

        // minimum distance from pv (0 for MC)
        float minDist = 3.0; // mm
        if (! (tru_v->perp() > minDist)) continue;
        m_dv_cutflow->Fill("r_{DV} > 3 mm", 1);

        //-----------------------------------------------------------------
        // minimum truth DV mass cut
        //-----------------------------------------------------------------

        // minimum DV mass cut
        //float minMass = 10.0; // GeV
        //if ( m_dvutils->TruthMass(tru_v) / 1000. < minMass) continue;
        //m_dv_cutflow->Fill("MinDVMass", 1);


    } // end of truth vertex loop


    //-----------------------------------------------------------------
    // end of truth cut flow                                          -
    // below is for dv reconstruction efficiency                      -
    //-----------------------------------------------------------------

    // loop over the truth vertex container
    for (auto tru_vv: tvv){

        float DVMass = m_dvutils->TruthMass(tru_vv.first) / 1000.;
        bool dv_matched = tru_vv.second;

        // fill truth signal vertex mass
        m_dv_mass->Fill( DVMass );
     
        // fill efficiency plots
        m_dv_eff_eta->Fill( tru_vv.first->eta(), dv_matched);
        m_dv_eff_phi->Fill( tru_vv.first->phi(), dv_matched);
        m_dv_eff_mass->Fill( DVMass, dv_matched);
        m_dv_eff_R->Fill( tru_vv.first->perp() , dv_matched);
        m_dv_eff_d0->Fill( m_dvutils->GetMaxd0(tru_vv.first), dv_matched );

        // efficiency as a function of Z'
        float zp_eta = tru_vv.first->incomingParticle(0)->eta();
        float zp_pt = tru_vv.first->incomingParticle(0)->pt();

        m_dv_eff_zp_eta->Fill(zp_eta, dv_matched);
        m_dv_eff_zp_pt->Fill(zp_pt / 1000., dv_matched);

    } // end of efficiency loop


    return StatusCode::SUCCESS;
}

