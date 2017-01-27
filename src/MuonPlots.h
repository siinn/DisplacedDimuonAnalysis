#ifndef DISPLACEDDIMUONANALYSIS_MUONPLOTS_H
#define DISPLACEDDIMUONANALYSIS_MUONPLOTS_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"
#include "xAODTracking/TrackParticleContainer.h"
#include "xAODTracking/TrackParticle.h"
#include "TriggerMatchingTool/IMatchingTool.h"



class MuonPlots: public ::AthAnalysisAlgorithm { 
    public: 
        MuonPlots( const std::string& name, ISvcLocator* pSvcLocator );
        virtual ~MuonPlots(); 
        
        virtual StatusCode  initialize();
        virtual StatusCode  execute();
        virtual StatusCode  finalize();
    
    private: 

        // define tool
        ToolHandle<Trig::IMatchingTool> m_tmt;

        // muon distribution
        TH1F* m_mu_pt; //!
        TH1F* m_mu_pt_low; //!

        TH1F* m_mu_trig_matched_pt; //!
        TH1F* m_mu_trig_matched_pt_low; //!

        TH1F* m_mu_trig_matched_pt_msonly; //!
        TH1F* m_mu_trig_matched_pt_low_msonly; //!
    
}; 

#endif //> !DISPLACEDDIMUONANALYSIS_MUONPLOTS_H
