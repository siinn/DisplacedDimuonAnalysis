// DisplacedDimuonAnalysis includes
#include "DisplacedDimuonAnalysis/DVUtils.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthParticle.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/MuonAuxContainer.h"
#include "xAODTracking/TrackParticle.h"
#include "xAODTracking/TrackParticleContainer.h"
#include "xAODTruth/TruthVertex.h"
#include "xAODTruth/TruthVertexContainer.h"
#include "xAODCore/ShallowCopy.h"
#include "xAODBase/IParticleHelpers.h"
#include "cmath"

DVUtils::DVUtils( const std::string& type,const std::string& name, const IInterface* parent) :
AthAlgTool( type, name, parent ),
m_accTr("recoTrackLink"),
m_accMu("DV_Muons"),
m_dilepdvc("DDL::DiLepDVCuts/DiLepDVCuts"),
m_mc("DDL::MuonCuts/DiLepMuonCuts"),
m_tmt("Trig::MatchingTool/MyMatchingTool")
{
    declareInterface<IDVUtils>(this);
    declareProperty("MuonCut", m_mc);
    declareProperty("DiLepDVCuts", m_dilepdvc);
    declareProperty("TriggerMatchingTool", m_tmt);

}


DVUtils::~DVUtils() {}


StatusCode DVUtils::initialize() {
    return StatusCode::SUCCESS;
}

StatusCode DVUtils::finalize() {
    return StatusCode::SUCCESS;
}


//-------------------------------------------------------------
// DV tools
//-------------------------------------------------------------

// calculate R of dv
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

// calculate z of dv
float DVUtils::getz(const xAOD::Vertex& dv, const xAOD::Vertex& pv) {
    // This function calculate z distance from hard-scattering
    // primary vertex to dv

    // access position of vertices
    auto dv_pos = dv.position();
    auto pv_pos = pv.position();

    // distance in 3d vector
    auto dist = pv_pos - dv_pos;

    // return distance in transverse plane
    return dist.z();

}

// calculate r of dv
float DVUtils::getr(const xAOD::Vertex& dv, const xAOD::Vertex& pv) {
    // This function calculate r distance from hard-scattering
    // primary vertex to dv

    // access position of vertices
    auto dv_pos = dv.position();
    auto pv_pos = pv.position();

    // distance in 3d vector
    auto dist = pv_pos - dv_pos;
    float dv_r = sqrt( dist.perp()*dist.perp() + dist.z()*dist.z() );

    // return distance in transverse plane
    return dv_r;

}

// apply muon matching and decorate dv with muon
//void DVUtils::ApplyMuonMatching(xAOD::Vertex& dv, xAOD::MuonContainer& muc) {
//
//  
//    ATH_MSG_INFO("DEBUG: 1"); 
//    // create containers
//    auto dv_muc = std::make_shared<xAOD::MuonContainer>(SG::VIEW_ELEMENTS);
//    m_accMu(dv) = dv_muc;
//    ATH_MSG_INFO("DEBUG: 2"); 
//
//    // muon matching via pointer comparison
//    for(auto trl: dv.trackParticleLinks()) {
//        ATH_MSG_INFO("DEBUG: 3"); 
//
//        //bool recoTrackLink = (*trl)->auxdata<ElementLink<xAOD::TrackParticleContainer> >("recoTrackLink");
//        //if (!recoTrackLink) continue;
//
//        const xAOD::TrackParticle* tr = *(m_accTr(**trl));
//        ATH_MSG_INFO("DEBUG: 3.1, tr = *(m_accTr(**trl)) = " << tr); 
//
//        ATH_MSG_INFO("DEBUG: 4"); 
//        for(auto mu: muc) {
//            ATH_MSG_INFO("DEBUG: 5"); 
//
//            // retrieve ID track from muon
//            auto mu_idtr = m_mc->GetTrack(*mu);
//            ATH_MSG_INFO("DEBUG: 5.1, getTrack(*mu) = mm_idtr = " << mu_idtr); 
//            ATH_MSG_INFO("DEBUG: 6.1, is mu_idtr == tr? "); 
//
//            if(mu_idtr == nullptr) continue;
//            if(mu_idtr == tr) {
//                dv_muc->push_back(mu);
//                ATH_MSG_INFO("DEBUG: 6.2, YES"); 
//            }
//            ATH_MSG_INFO("DEBUG: 6.3, NO"); 
//
//
//        } // end of muon loop
//    } // end of dv trackparticle link
//} // end of ApplyMuonMatching


//bool DVUtils::CheckDVMuon(const xAOD::Vertex& dv) {
//
//    if(!m_accMu.isAvailable(dv)) {
//        ATH_MSG_INFO("DEBUG: accuMu is not available"); 
//        return false;
//    }
//    if(m_accMu(dv) == nullptr) {
//        ATH_MSG_INFO("DEBUG: accuMu is empty"); 
//        return false;
//    }
//    ATH_MSG_INFO("DEBUG: Muon is available, dv = "); 
//    ATH_MSG_INFO(&dv); 
//    return true;
//}



// match dv to signal truth
const xAOD::TruthVertex* DVUtils::IsSignalDV(const DataVector<xAOD::Muon> dv_muc ) {

    // retrieve truth vertex container
    const xAOD::TruthVertexContainer* tru_vc = nullptr;
    evtStore()->retrieve( tru_vc, "TruthVertices");

    // loop over the truth vertex container
    for (auto tru_v: *tru_vc){

        // select signal dv
        if ( !(isSignalVertex(tru_v)) ) continue;

        // access signal muons
        const xAOD::TruthParticle* truth_child0 = FindFinalState(tru_v->outgoingParticle(0));
        const xAOD::TruthParticle* truth_child1 = FindFinalState(tru_v->outgoingParticle(1));

        // set flag
        bool muon_match0 = false;
        bool muon_match1 = false;

        for(auto mu: dv_muc){
            const xAOD::TruthParticle* matched_truth_muon=0;
            // access link to matched truth muon
            if(mu->isAvailable<ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink")) {
                ElementLink<xAOD::TruthParticleContainer> link = mu->auxdata<ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink");
                // check if the link is valid
                if(link.isValid()) {
                    matched_truth_muon = *link;
                    // return true if at least one truthlink of reco muon matches with given truth
                    if (matched_truth_muon->barcode() == truth_child0->barcode()) muon_match0 = true;
                    if (matched_truth_muon->barcode() == truth_child1->barcode()) muon_match1 = true;
                } // end of link.isValid
            } // end of if mu->isAvailable
        } // end of muon container

        // if we have two muons matched, this truth dv is matched to given dv
        if (muon_match0 and muon_match1) return tru_v;

    } // end of tru_vc

    // return null if there is no match (reco dv is not matched to truth)
    return nullptr;
}

// trigger matching. check if one muon of DV is matched to trigger
bool DVUtils::TriggerMatching(const DataVector<xAOD::Muon> dv_muc) {

    bool pass = false;

    for(auto mu: dv_muc){
        ATH_MSG_DEBUG("Trigger matched = " << m_tmt->match(*mu,"HLT_mu60_0eta105_msonly"));
        if (m_tmt->match(*mu,"HLT_mu60_0eta105_msonly")) pass = true;
    }
    return pass;
}

//-------------------------------------------------------------
// Truth DV tools
//-------------------------------------------------------------

// check if this truth DV is reconstructed
bool DVUtils::IsReconstructed(const xAOD::TruthVertex* tru_v) {

    // retrieve secondary vertices
    const xAOD::VertexContainer* dvc = nullptr;
    CHECK( evtStore()->retrieve( dvc, "VrtSecInclusive_SecondaryVertices" ));

    // make a copy of vertex containers
    auto dvc_copy = xAOD::shallowCopyContainer(*dvc);

    // retrieve lepton container
    const xAOD::MuonContainer* muc = nullptr;
    CHECK( evtStore()->retrieve( muc, "Muons" ));

    const xAOD::ElectronContainer* elc = nullptr;
    CHECK( evtStore()->retrieve( elc, "Electrons" ));

    // make copies of muon
    auto muc_copy = xAOD::shallowCopyContainer(*muc);
    xAOD::setOriginalObjectLink(*muc, *muc_copy.first);

    auto elc_copy = xAOD::shallowCopyContainer(*elc);
    xAOD::setOriginalObjectLink(*elc, *elc_copy.first);

    // perform lepton matching
    for(auto dv: *dvc_copy.first) {
        //ApplyMuonMatching(*dv, *muc_copy.first);
        m_dilepdvc->ApplyLeptonMatching(*dv, *elc_copy.first, *muc_copy.first);
    }

    // access signal muons
    const xAOD::TruthParticle* truth_child0 = FindFinalState(tru_v->outgoingParticle(0));
    const xAOD::TruthParticle* truth_child1 = FindFinalState(tru_v->outgoingParticle(1));

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
            const xAOD::TruthParticle* matched_truth_muon=0;
            // access link to matched truth muon
            if(mu->isAvailable<ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink")) {
                ElementLink<xAOD::TruthParticleContainer> link = mu->auxdata<ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink");
                // check if the link is valid
                if(link.isValid()) {
                    matched_truth_muon = *link;
                    // return true if at least one truthlink of reco muon matches with given truth
                    if (matched_truth_muon->barcode() == truth_child0->barcode()) muon_match0 = true;
                    if (matched_truth_muon->barcode() == truth_child1->barcode()) muon_match1 = true;
                } // end of link.isValid
            } // end of if mu->isAvailable
        } // end of muon container

        // if we have two muons matched, this truth dv is reconstructed
        if (muon_match0 and muon_match1) return true;

    } // end of loop over dvc
    return false;
} // end of IsReconstructed




// check if muons of signal DV are reconstructed
bool DVUtils::MuonsReconstructed(const xAOD::TruthVertex* tru_v) {

    // access signal muons
    const xAOD::TruthParticle* truth_child0 = FindFinalState(tru_v->outgoingParticle(0));
    const xAOD::TruthParticle* truth_child1 = FindFinalState(tru_v->outgoingParticle(1));

    if (IsReconstructedAsMuon(truth_child0) and IsReconstructedAsMuon(truth_child0)) return true;
    return false;
}


// check if both muons of truth dv passes eta cut
bool DVUtils::PassAcceptance(const xAOD::TruthVertex* tru_v) {

    bool pass = true;

    for (unsigned int i = 0; i < tru_v->nOutgoingParticles(); i++){
        const xAOD::TruthParticle* truth_child = FindFinalState(tru_v->outgoingParticle(i));
        if (truth_child->eta() > 2.4) pass = false;
    }

    return pass;
}

// find maximum d0 among outgoing particles
float DVUtils::GetMaxd0(const xAOD::TruthVertex* tru_v) {

    float Maxd0 = 0;

    // loop over outgoing particles at the truth vertex
    for (unsigned int i = 0; i < tru_v->nOutgoingParticles(); i++){

        const xAOD::TruthParticle* truth_child = FindFinalState(tru_v->outgoingParticle(i));
        if (truth_child->isAvailable<float>("d0")) {
            float d0 = fabs(truth_child->auxdata<float>("d0"));
            Maxd0 = std::max( d0, Maxd0);
        }
    }
    return Maxd0;
}

// return final state of given truth particle
const xAOD::TruthParticle* DVUtils::FindFinalState(const xAOD::TruthParticle* tp) {

    ATH_MSG_DEBUG("--- New truth particle, barcode = " << tp->barcode() );
    const xAOD::TruthParticle *ftp = tp;

    int n_counter = 0;

    do {
        // return final state particle
        if (ftp->status() == 1) {
            ATH_MSG_DEBUG("--- Found Final state particle, status = " << ftp->status() << ", barcode = " << ftp->barcode() );
            return ftp;
        }
        ATH_MSG_DEBUG("Found intermediate truth particle, status = " << ftp->status() << ", barcode = " << ftp->barcode() );
        // follow decay chain
        if (ftp->child(0) != nullptr) {
            ftp = ftp->child(0);
            ATH_MSG_DEBUG("find next child, barcode = " << ftp->barcode() );
        }
        // bremsstrahlung
        if (ftp->nChildren() > 1) {
            ATH_MSG_DEBUG("More than 1 outgoing particle. nChildren = " << ftp->nChildren() );
            for (unsigned int i = 0; i < ftp->nChildren(); i++ ) {
                ATH_MSG_DEBUG("children " << i << ": status = " << ftp->child(i)->status() << ", pdgid = " << ftp->child(i)->pdgId() );
                if (ftp->child(i)->absPdgId() == 13) {
                    ftp = ftp->child(i);
                }
            } 
        } // end of nChildren > 1
        n_counter++;

    //} while ( (ftp->status() != 1) and (ftp != nullptr) );
    } while ( (ftp != nullptr) and (n_counter < 10) );

    ATH_MSG_DEBUG( "ERROR : Found intermediate particle with invalid children, status = " << ftp->status() << ", nChildren = " << ftp->nChildren() << ", is it null?" << ftp );

    return ftp;

} // end of FindFinalState

//-------------------------------------------------------------
// Muon tools
//-------------------------------------------------------------


// check if truth particle is reconstructed as muon
bool DVUtils::IsReconstructedAsMuon(const xAOD::TruthParticle* tp) {

    // retrieve muon container
    const xAOD::MuonContainer* muc = nullptr;
    CHECK( evtStore()->retrieve( muc, "Muons" ) );

    // loop over muon container, looking for matched reco muon
    for(auto mu: *muc) {
        const xAOD::TruthParticle* matched_truth_muon=0;
        // access link to matched truth muon
        if(mu->isAvailable<ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink")) {
            ElementLink<xAOD::TruthParticleContainer> link = mu->auxdata<ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink");
            // check if the link is valid
            if(link.isValid()) {
                matched_truth_muon = *link;
                // return true if at least one truthlink of reco muon matches with given truth
                if (matched_truth_muon->barcode() == tp->barcode()) return true;
            } // end of link.isValid
        } // end of if mu->isAvailable
    } // end of loop over muon container

    return false;

}



//-------------------------------------------------------------
// ID track tools
//-------------------------------------------------------------

// check if truth particle is reconstructed as ID track
bool DVUtils::IsReconstructedAsIDTrack(const xAOD::TruthParticle& tp) {

    // retrieve muon container
    const xAOD::TrackParticleContainer* idc = nullptr;
    CHECK( evtStore()->retrieve( idc, "InDetTrackParticles" ) );

    // loop over muon container, looking for matched reco muon
    for(auto mu: *idc) {
        // use MS track which is not affected by badly matched ID tracks
        double dr = tp.p4().DeltaR(mu->p4());
        if(dr < 0.20) {
            return true;
        }
    } // end of loop over muon container

    return false;

}


//-------------------------------------------------------------
// truth tools
//-------------------------------------------------------------

// check if truth particle is signal muon from (Z')
bool DVUtils::isSignal (const xAOD::TruthParticle* p) {
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



// check if truth vertex is signal vertex
bool DVUtils::isSignalVertex (const xAOD::TruthVertex* v) {

    if (!(v->nIncomingParticles() == 1)) return false;
    if (!(v->nOutgoingParticles() == 2)) return false;

    // access parent
    const xAOD::TruthParticle* parent = v->incomingParticle(0);
    if ((parent->absPdgId() ==32)) return true;
    else return false;

} // end of isSignal


// calculate invariant mass of truth vertex
float DVUtils::TruthMass (const xAOD::TruthVertex* v) {

    // Lorentz vector to calculate invariant mass of truth vertex
    TLorentzVector outgoing_tlv;

    for (unsigned int i = 0; i < v->nOutgoingParticles(); i++) {

        const xAOD::TruthParticle* p = v->outgoingParticle(i);
        outgoing_tlv += TLorentzVector( p->px(), p->py(), p->pz(), p->e());

    } // end of outgoingn particle loop

    return outgoing_tlv.M(); // return invariant mass in MeV

} // end of TruthMass



//-------------------------------------------------------------
// validation tool
//-------------------------------------------------------------

// check if track particle is reconstructed using LRT
bool DVUtils::isLargeD0Track (const xAOD::TrackParticle* tp) {
    const std::bitset<xAOD::NumberOfTrackRecoInfo> patternReco = tp->patternRecoInfo();
    if (patternReco.test(49)) { return true; }
    return false;
} // end of isLargeD0Track


// track selection for validation
bool DVUtils::TrackSelection (const xAOD::TrackParticle* tp) {
    float maxEta = 2.5;
    float minPt = 1000;
    if ( (tp->pt()>1e-7 ? (fabs(tp->eta()) < maxEta) : false) &&  \
         (tp->pt() > minPt) ) return true;
    else return false;
} // end of TrackSelection

bool DVUtils::TrackSelection (const xAOD::TruthParticle* tp) {
    float maxEta = 2.5;
    float minPt = 1000;
    if (!(tp->status() == 1)) return false;
    if (!(tp->isCharged())) return false;
    if ( (tp->pt()>1e-7 ? (fabs(tp->eta()) < maxEta) : false) &&  \
         (tp->pt() > minPt) ) return true;
    else return false;
} // end of TrackSelection








