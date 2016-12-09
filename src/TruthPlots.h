#ifndef DISPLACEDDIMUONANALYSIS_TRUTHPLOTS_H
#define DISPLACEDDIMUONANALYSIS_TRUTHPLOTS_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthParticle.h"

// DVUtil
#include "DisplacedDimuonAnalysis/DVUtils.h"


class TruthPlots: public ::AthAnalysisAlgorithm { 
    public: 
        TruthPlots( const std::string& name, ISvcLocator* pSvcLocator );
        virtual ~TruthPlots(); 
        
        virtual StatusCode  initialize();
        virtual StatusCode  execute();
        virtual StatusCode  finalize();
    
    private: 
        ToolHandle<IDVUtils>  m_dvutils; //!

        // signal muon distribution
        TH1F* m_truth_pt_slice1; //!
        TH1F* m_truth_pt_slice2; //!
        TH1F* m_truth_pt_slice3; //!
        TH1F* m_truth_pt_slice4; //!

        TH1F* m_truth_eta_slice1; //!
        TH1F* m_truth_eta_slice2; //!
        TH1F* m_truth_eta_slice3; //!
        TH1F* m_truth_eta_slice4; //!

        TProfile* m_truth_muon_acceptance; //!

        TH2F* m_truth_pt_vs_prodVtxR; //!
        TH2F* m_truth_eta_vs_prodVtxR; //!

        // signal Z' distribution
        TH1F* m_zp_eta_slice1;
        TH1F* m_zp_eta_slice2;
        TH1F* m_zp_eta_slice3;
        TH1F* m_zp_eta_slice4;
        TH2F* m_zp_eta_vs_prodVtxR; //!

        TH1F* m_zp_pt_slice1;
        TH1F* m_zp_pt_slice2;
        TH1F* m_zp_pt_slice3;
        TH1F* m_zp_pt_slice4;

        TProfile* m_truth_zp_acceptance; //!

    
}; 

#endif //> !DISPLACEDDIMUONANALYSIS_TRUTHPLOTS_H
