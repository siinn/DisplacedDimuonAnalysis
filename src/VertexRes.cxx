// DisplacedDimuonAnalysis includes
#include "VertexRes.h"

//#include "xAODEventInfo/EventInfo.h"

#include "GaudiKernel/ITHistSvc.h"
#include "TTree.h"
#include "TH1F.h"

#include "xAODBase/IParticleHelpers.h"
#include "xAODCore/ShallowCopy.h"

// xAOD
#include "xAODTruth/TruthVertex.h"
#include "xAODTruth/TruthVertexContainer.h"


VertexRes::VertexRes( const std::string& name, ISvcLocator* pSvcLocator ) :
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


VertexRes::~VertexRes() {}


StatusCode VertexRes::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");
    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    m_dv_dimuon_res_R = new TH1F("m_dv_dimuon_res_R", "DV R resolution", 50, -1, 1);
    m_dv_dimuon_res_M = new TH1F("m_dv_dimuon_res_M", "DV M resolution", 50, -300, 300);

    CHECK( histSvc->regHist("/DV/SecondaryVertex/Reconstructed/Resolution/dv_dimuon_res_R", m_dv_dimuon_res_R) );
    CHECK( histSvc->regHist("/DV/SecondaryVertex/Reconstructed/Resolution/dv_dimuon_res_M", m_dv_dimuon_res_M) );
    
    return StatusCode::SUCCESS;
}

StatusCode VertexRes::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");
    return StatusCode::SUCCESS;
}

StatusCode VertexRes::execute() {  
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    // retrieve event info
    const xAOD::EventInfo* evtInfo = nullptr;
    CHECK( evtStore()->retrieve( evtInfo, "EventInfo" ) );

    // flag to check if data or MC
    bool isMC = evtInfo->eventType(xAOD::EventInfo::IS_SIMULATION);

    // skip the algorithm if running on data
    if (!isMC) return StatusCode::SUCCESS;

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

    // retrieve lepton container
    const xAOD::MuonContainer* muc = nullptr;
    CHECK( evtStore()->retrieve( muc, "Muons" ));

    // make copies of leptons
    auto muc_copy = xAOD::shallowCopyContainer(*muc);
    xAOD::setOriginalObjectLink(*muc, *muc_copy.first);


    // perform lepton matching
    for(auto dv: *dvc_copy.first) {
        m_dvutils->ApplyMuonMatching(*dv, *muc_copy.first);
    }

    // loop over sv to calculate deviation
    for(auto dv: *dvc_copy.first) {

        // collect muons from this dv
        auto dv_muc = m_dilepdvc->GetMu(*dv);

        // find truth dv matched to this dv
        const xAOD::TruthVertex* tru_v = m_dvutils->IsSignalDV(*dv_muc);

        if (tru_v == nullptr) continue;

        // calculate R from PV, and mass
        float dv_R = m_dvutils->getR( *dv, *pv );              // R in [mm]
        float dv_mass = std::fabs(m_accMass(*dv)) / 1000.; // in MeV

        // access invariant mass
        float tru_v_mass = m_dvutils->TruthMass(tru_v) / 1000.; // in MeV

        m_dv_dimuon_res_R->Fill ( (dv_R - tru_v->perp()) );
        m_dv_dimuon_res_M->Fill ( (dv_mass - tru_v_mass) );

        ATH_MSG_DEBUG( "Found truth-matched DV, DV_mass = " << dv_mass <<  ", R = " << dv_R );
        ATH_MSG_DEBUG( "Truth DV, truth mass = " << tru_v_mass << ", R = " << tru_v->perp() );


    } // end of dv loop




    return StatusCode::SUCCESS;
}

