// DisplacedDimuonAnalysis includes
#include "DisplacedDimuonAnalysis/DVUtils.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthParticle.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/MuonAuxContainer.h"

// DV

DVUtils::DVUtils( const std::string& type,const std::string& name, const IInterface* parent) :
AthAlgTool( type, name, parent )
{
    declareInterface<IDVUtils>(this);

}


DVUtils::~DVUtils() {}


StatusCode DVUtils::initialize() {
    return StatusCode::SUCCESS;
}

StatusCode DVUtils::finalize() {
    return StatusCode::SUCCESS;
}

float DVUtils::getR(const xAOD::Vertex& dv, const xAOD::Vertex& pv) {
    // This function calculate perpendicular distance from hard-scattering
    // primary vertex to dv

    // access position of vertices
    auto dv_pos = dv.position();
    auto pv_pos = pv.position();

    // distance in 3d vector
    auto dist = pv_pos - dv_pos;

    // return distance in transverse plane
    return dist.perp();

}

bool DVUtils::IsReconstructedAsMuon(const xAOD::TruthParticle& tp) {

    // retrieve muon container
    const xAOD::MuonContainer* muc = nullptr;
    CHECK( evtStore()->retrieve( muc, "Muons" ) );

    // loop over muon container, looking for matched reco muon
    for(auto mu: *muc) {
        // use MS track which is not affected by badly matched ID tracks
        const xAOD::IParticle* mu_ip = nullptr;
        auto mstrk = mu->trackParticle(xAOD::Muon::MuonSpectrometerTrackParticle);
        if(mstrk == nullptr) mu_ip = mu;
        else mu_ip = mstrk;
    
        double dr = tp.p4().DeltaR(mu_ip->p4());
        if(dr < 0.10) {
            return true;
        }
    } // end of loop over muon container

    return false;

}
