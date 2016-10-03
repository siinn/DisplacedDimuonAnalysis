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

DisplacedDimuonAnalysisAlg::DisplacedDimuonAnalysisAlg( const std::string& name, ISvcLocator* pSvcLocator ) :
AthAnalysisAlgorithm( name, pSvcLocator ),
m_dilepdvc("DV::DiLepDVCuts/DiLepDVCuts"),
m_evtc("DV::EventCuts/DiLepEventCuts"),
m_dvutils("DVUtils"),
m_accMass("mass")
{
    // initialize tools
    declareProperty("DiLepDVCuts", m_dilepdvc);
    declareProperty("DVUtils", m_dvutils);
    declareProperty("DiLepEventCuts", m_evtc);

}


DisplacedDimuonAnalysisAlg::~DisplacedDimuonAnalysisAlg() {}


StatusCode DisplacedDimuonAnalysisAlg::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");
    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    // define histograms
    m_dv_M = new TH1F("dv_M","All DV mass in GeV",50,0.,500.);
    m_dv_dimuon_M = new TH1F("dv_dimuon_M","Dimuon DV mass in GeV",50,0.,500.);
    m_dv_dimuon_R = new TH1F("dv_dimuon_R","R of dimuon dv [mm]",50,0.,400.);

    // registor for output
    CHECK( histSvc->regHist("/DV/SecondaryVertex/Reconstructed/reco_dv_M", m_dv_M) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/Reconstructed/reco_dv_dimuon_M", m_dv_dimuon_M) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/Reconstructed/reco_dv_dimuon_R", m_dv_dimuon_R) );

    int n_dv = 0;
    int n_muon_dv = 0;

  return StatusCode::SUCCESS;
}

StatusCode DisplacedDimuonAnalysisAlg::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");
    ATH_MSG_INFO ("Number of DV with no muons = " << n_dv << ", number of DV with two muons = " << n_muon_dv);
    
    return StatusCode::SUCCESS;
}

StatusCode DisplacedDimuonAnalysisAlg::execute() {
    ATH_MSG_DEBUG ("Executing " << name() << "...");
    setFilterPassed(false); //optional: start with algorithm not passed
    
    // retrieve event info
    const xAOD::EventInfo* evtInfo = nullptr;
    CHECK( evtStore()->retrieve( evtInfo, "EventInfo" ) );

    // retrieve lepton container
    const xAOD::MuonContainer* muc = nullptr;
    const xAOD::ElectronContainer* elc = nullptr;
    CHECK( evtStore()->retrieve( muc, "Muons" ));
    CHECK( evtStore()->retrieve( elc, "Electrons" ));

    // make copies of leptons
    auto muc_copy = xAOD::shallowCopyContainer(*muc);
    auto elc_copy = xAOD::shallowCopyContainer(*elc);
    xAOD::setOriginalObjectLink(*muc, *muc_copy.first);
    xAOD::setOriginalObjectLink(*elc, *elc_copy.first);

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
    for(auto dv: *dvc_copy.first)
    {
        m_dilepdvc->ApplyLeptonMatching(*dv, *muc_copy.first);

    }


    // final plotting
    for(auto dv: *dvc_copy.first)
    {
        // access invariant mass
        float dv_mass = std::fabs(m_accMass(*dv)) / 1000.; // in MeV

        // collect muons from this dv
        auto dv_muc = m_dilepdvc->GetMu(*dv);

        // fill all dv
        n_dv++;
        m_dv_M->Fill(dv_mass);               // all dv

        // require dv to have 2 muons
        if (dv_muc->size() != 2) continue;

        // fill dimuon vertex
        n_muon_dv++;                                           // number of dimuon dv
        m_dv_dimuon_M->Fill(dv_mass);                          // dimuon mass
        m_dv_dimuon_R->Fill( m_dvutils->getR( *dv, *pv ) );    // R in [mm]
        

    }

    
    
    
    setFilterPassed(true); //if got here, assume that means algorithm passed
    return StatusCode::SUCCESS;
}

StatusCode DisplacedDimuonAnalysisAlg::beginInputFile() { 
    //
    //This method is called at the start of each input file, even if
    //the input file contains no events. Accumulate metadata information here
    //
    
    //example of retrieval of CutBookkeepers: (remember you will need to include the necessary header files and use statements in requirements file)
    // const xAOD::CutBookkeeperContainer* bks = 0;
    // CHECK( inputMetaStore()->retrieve(bks, "CutBookkeepers") );
    
    //example of IOVMetaData retrieval (see https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/AthAnalysisBase#How_to_access_file_metadata_in_C)
    //float beamEnergy(0); CHECK( retrieveMetadata("/TagInfo","beam_energy",beamEnergy) );
    //std::vector<float> bunchPattern; CHECK( retrieveMetadata("/Digitiation/Parameters","BeamIntensityPattern",bunchPattern) );



  return StatusCode::SUCCESS;
}


