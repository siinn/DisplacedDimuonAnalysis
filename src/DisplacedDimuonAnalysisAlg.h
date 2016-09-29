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
#include "DVCuts/IDiLepCosmics.h"
#include "DVCuts/IDiLepDVCuts.h"
#include "DVTools/IOverlapRemoval.h"
#include "DVTools/IPhotonMatch.h"

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

        // tool for muon matching to dv
        ToolHandle<DV::IDiLepDVCuts> m_dilepdvc; //!
        ToolHandle<DV::IEventCuts> m_evtc; //!
        ToolHandle<IDVUtils>  m_dvutils; //!

        // DV mass accessor
        SG::AuxElement::ConstAccessor<float> m_accMass;

        // output

        int n_dv;
        int n_muon_dv;
       
        TH1F* m_dv_mass_all; //!
        TH1F* m_dv_mass_dimuon; //!
        TH1F* m_dv_R; //!
          
    
}; 

#endif //> !DISPLACEDDIMUONANALYSIS_DISPLACEDDIMUONANALYSISALG_H
