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
    
    private: 

        ToolHandle<IDVUtils>  m_dvutils; //!

        int signal_truth;
        int signal_reconstructed;

        TProfile* m_eff_eta; //!
        TProfile* m_eff_pt; //!
        TProfile* m_eff_R; //!
        TProfile* m_eff_d0; //!

        // 2D efficiency plots
        TH2F* m_eff_pt_vs_prodVtxR_num; //!
        TH2F* m_eff_pt_vs_prodVtxR_den; //!

        TH2F* m_eff_eta_vs_prodVtxR_num; //!
        TH2F* m_eff_eta_vs_prodVtxR_den; //!

        TH2F* m_eff_pt_vs_d0_num; //!
        TH2F* m_eff_pt_vs_d0_den; //!

        TH2F* m_eff_eta_vs_d0_num; //!
        TH2F* m_eff_eta_vs_d0_den; //!

}; 

#endif //> !DISPLACEDDIMUONANALYSIS_MUONEFFICIENCY_H
