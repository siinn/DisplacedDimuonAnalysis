#ifndef DISPLACEDDIMUONANALYSIS_DISPLACEDDIMUONANALYSISALG_H
#define DISPLACEDDIMUONANALYSIS_DISPLACEDDIMUONANALYSISALG_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"

// STL
#include <map>
#include <string>
#include <vector>

// EDM
#include "AsgTools/ToolHandle.h"

// DV
#include "DVAnalyses/AlgBase.h"
#include "DVCuts/IDVCuts.h"
#include "DVCuts/IEventCuts.h"
//#include "DVCuts/IDiLepCosmics.h"
#include "DVCuts/IDiLepDVCuts.h"
//#include "DVTools/IOverlapRemoval.h"
//#include "DVTools/IPhotonMatch.h"

// DVUtil
#include "DisplacedDimuonAnalysis/DVUtils.h"


class DisplacedDimuonAnalysisAlg: public ::AthAnalysisAlgorithm { 
    public: 

        DisplacedDimuonAnalysisAlg( const std::string& name, ISvcLocator* pSvcLocator );
        virtual ~DisplacedDimuonAnalysisAlg(); 
        
        virtual StatusCode  initialize();
        virtual StatusCode  execute();
        virtual StatusCode  finalize();
        
        virtual StatusCode beginInputFile();

    
    private:

        // debug
        //StoreGateSvc* m_StoreGate;

        // tool for muon matching to dv
        ToolHandle<DV::IDiLepDVCuts> m_dilepdvc; //!
        ToolHandle<DV::IEventCuts> m_evtc; //!
        ToolHandle<IDVUtils>  m_dvutils; //!
        ToolHandle<DV::IDVCuts> m_dvc; //!

        // DV mass accessor
        SG::AuxElement::ConstAccessor<float> m_accMass;

        // output
        TH1F* m_dv_M; //!
        TH1D* m_dv_cutflow; //!
        TH1D* m_event_cutflow; //!

        // dimuon vertices
        TH1F* m_dv_dimuon_M; //!
        TH1F* m_dv_dimuon_R; //!
        TH1F* m_dv_dimuon_z; //!
        TH1F* m_dv_dimuon_r; //!

        // ABCD method
        TH2F* m_dv_dimuon_R_M; //!

        // check vertex fit
        TH1F* m_chi2_ndof; //!

        // only for MC

        // dimuon vertices
        TH1F* m_dv_dimuon_M_matched; //!
        TH1F* m_dv_dimuon_R_matched; //!

        // ABCD method
        TH2F* m_dv_dimuon_R_M_matched; //!

          
    
}; 

#endif //> !DISPLACEDDIMUONANALYSIS_DISPLACEDDIMUONANALYSISALG_H
