#ifndef DISPLACEDDIMUONANALYSIS_TRUTHPLOTS_H
#define DISPLACEDDIMUONANALYSIS_TRUTHPLOTS_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthParticle.h"

// pile-up reweighting
#include "AsgAnalysisInterfaces/IPileupReweightingTool.h"

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
        ToolHandle<CP::IPileupReweightingTool> m_prw; //!

        // use PRW?
        bool m_usePRW;

        // pileup weight
        float p_weight = 1;

        // maximum lepton pT in each event
        double muon_pt_max;
        double el_pt_max;

        // pile-up distribution
        TH1F* m_pileup; //!
        TH1F* m_pileup_reweighted; //!

        // signal el distribution
        TH1F* m_el_pt; //!
        TH1F* m_el_pt_max; //!
        TH1F* m_el_pt_low; //!
        TH1F* m_el_eta; //!
        TH1F* m_el_m; //!
        TH1F* m_el_pdgId; //!
        TH1F* m_el_phi; //!
        TH1F* m_el_R; //!
        TH1F* m_el_z; //!
        TH1F* m_el_d0; //!
        TH1F* m_el_z0; //!

        // signal muon distribution
        TH1F* m_muon_pt; //!
        TH1F* m_muon_pt_max; //!
        TH1F* m_muon_pt_low; //!
        TH1F* m_muon_eta; //!
        TH1F* m_muon_m; //!
        TH1F* m_muon_pdgId; //!
        TH1F* m_muon_phi; //!
        TH1F* m_muon_R; //!
        TH1F* m_muon_z; //!
        TH1F* m_muon_d0; //!
        TH1F* m_muon_z0; //!

        // signal dilep distribution
        TH1F* m_dilep_m; //!
        TH1F* m_dilep_pt; //!
        TH1F* m_dilep_dr; //!

        TProfile* m_muon_acceptance; //!
        TH2F* m_muon_pt_vs_prodVtxR; //!
        TH2F* m_muon_eta_vs_prodVtxR; //!

        // signal Z' distribution
        TH1F* m_zp_m; //!
        TH1F* m_zp_pt; //!
        TH1F* m_zp_eta; //!
        TH1F* m_zp_phi; //!
        TH1F* m_zp_pdgId; //!
        TH1F* m_zp_beta; //!
        TH1F* m_zp_ctau0; //!

        TH1F* m_zp_R; //!
        TH1F* m_zp_R_low; //!
        TH1F* m_zp_l; //!
        TH1F* m_zp_z; //!
        TH1F* m_zp_t_barrel; //!
        TH1F* m_zp_t_endcap; //!

        TProfile* m_fraction_dv_cut; //!

        TH2F* m_zp_eta_vs_prodVtxR; //!
        TProfile* m_truth_zp_acceptance; //!

    
}; 

#endif //> !DISPLACEDDIMUONANALYSIS_TRUTHPLOTS_H
