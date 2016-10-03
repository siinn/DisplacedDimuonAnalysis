#ifndef DISPLACEDDIMUONANALYSIS_MUONEFFICIENCY_H
#define DISPLACEDDIMUONANALYSIS_MUONEFFICIENCY_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthParticle.h"
#include "xAODTracking/TrackParticleContainer.h"
#include "xAODTracking/TrackParticle.h"

// DVUtil
#include "DisplacedDimuonAnalysis/DVUtils.h"


class MuonEfficiency: public ::AthAnalysisAlgorithm { 
    public: 

        MuonEfficiency( const std::string& name, ISvcLocator* pSvcLocator );
        virtual ~MuonEfficiency(); 
        
        virtual StatusCode  initialize();
        virtual StatusCode  execute();
        virtual StatusCode  finalize();
        
        virtual StatusCode beginInputFile();

        //bool TrackSelection (const xAOD::TrackParticle* p);
        bool TrackSelection (const xAOD::TruthParticle* p);
        bool isSignal ( const xAOD::TruthParticle* p );
        bool decorateTruth(const xAOD::TruthParticle & particle);
    
    private: 

        ToolHandle<IDVUtils>  m_dvutils; //!

        int signal_truth;
        int signal_reconstructed;

        TProfile* m_eff_eta_01; //!
        TProfile* m_eff_pt_01; //!

}; 

#endif //> !DISPLACEDDIMUONANALYSIS_MUONEFFICIENCY_H
