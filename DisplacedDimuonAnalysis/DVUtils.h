#ifndef DVUTILS_H
#define DVUTILS_H

#include "AthenaBaseComps/AthAlgTool.h"
// EDM
#include "AsgTools/ToolHandle.h"

// custom tool
#include "IDVUtils.h"

// trigger matching tool
#include "TriggerMatchingTool/IMatchingTool.h"

// from DV framework
#include "DDLBase/IMuonCuts.h"
#include "DDLBase/IDiLepDVCuts.h"

#include "xAODTracking/Vertex.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthParticle.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/MuonAuxContainer.h"

class DVUtils : public AthAlgTool, virtual public IDVUtils {

    public:

        DVUtils (const std::string& type, const std::string& name, const IInterface* parent);
        virtual ~DVUtils();

        virtual StatusCode  initialize();
        virtual StatusCode  finalize();

        // get position of dv wrt pv
        float getR(const xAOD::Vertex& dv, const xAOD::Vertex& pv);
        float getz(const xAOD::Vertex& dv, const xAOD::Vertex& pv);
        float getr(const xAOD::Vertex& dv, const xAOD::Vertex& pv);

        // reco dv is matched to signal truth dv
        const xAOD::TruthVertex* IsSignalDV(const DataVector<xAOD::Muon> dv_muc);

        // trut tools --------------------------

        // reco match using dR method
        bool IsReconstructedAsMuon(const xAOD::TruthParticle* tp);

        // reco match using dR method
        bool IsReconstructedAsIDTrack(const xAOD::TruthParticle& tp);

        // find final state truth particles
        const xAOD::TruthParticle* FindFinalState(const xAOD::TruthParticle* tp);

        // muon matching tool
        //void ApplyMuonMatching(xAOD::Vertex& dv, xAOD::MuonContainer& muc);

        // match truth dv to reco dv
        bool IsReconstructed(const xAOD::TruthVertex* vertex_truth);

        // check if two muons if signal dv are reconstructed
        bool MuonsReconstructed(const xAOD::TruthVertex* tru_v);

        // check two muons pass acceptance ( eta < 2.4)
        bool PassAcceptance(const xAOD::TruthVertex* tru_v);

        // find delta between two muons from dv muon container
        float getDeltaR(const DataVector<xAOD::Muon> dv_muc);
        float getDeltaPhiMinusPi(const DataVector<xAOD::Muon> dv_muc);
        float getSumEta(const DataVector<xAOD::Muon> dv_muc);
        float getMinPT(const DataVector<xAOD::Muon> dv_muc);
        float getMaxPT(const DataVector<xAOD::Muon> dv_muc);

        // find delta pT between two muons from dv muon container
        float getDelta_pT(const DataVector<xAOD::Muon> dv_muc);

        // check if one muon of DV is matched to trigger
        bool TriggerMatching(const DataVector<xAOD::Muon> dv_muc);

        // check if two muons are combined muons
        bool IsCombinedMuon(const DataVector<xAOD::Muon> dv_muc);

        // get maximum d0 among outgoing particles
        float GetMaxd0(const xAOD::TruthVertex* tru_v);

        // check if truth track is signal muon
        bool isSignal (const xAOD::TruthParticle* p);
        // check if truth vertex is Z' vertex
        bool isSignalVertex (const xAOD::TruthVertex* v);
        // calculate invariant mass of truth vertex
        float TruthMass (const xAOD::TruthVertex* v);

        // check if ID track is LRT
        bool isLargeD0Track (const xAOD::TrackParticle* tp);
        // track selection for validation
        bool TrackSelection (const xAOD::TrackParticle* tp);
        bool TrackSelection (const xAOD::TruthParticle* tp);

        // accessor for original ID track
        SG::AuxElement::Accessor<ElementLink<xAOD::TrackParticleContainer>> m_accTr;
        // accessors for leptons associated to a DV
        SG::AuxElement::Accessor<std::shared_ptr<xAOD::MuonContainer>> m_accMu;

    private:

        ToolHandle<DDL::IMuonCuts> m_mc; //!
        ToolHandle<DDL::IDiLepDVCuts> m_dilepdvc; //!
        ToolHandle<Trig::IMatchingTool> m_tmt;


};

#endif //> !DVUTILS_H
