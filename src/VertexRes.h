#ifndef DISPLACEDDIMUONANALYSIS_VERTEXRES_H
#define DISPLACEDDIMUONANALYSIS_VERTEXRES_H 1

#include "AthAnalysisBaseComps/AthAnalysisAlgorithm.h"
#include "DDLBase/IDiLepDVCuts.h"
#include "DDLBase/IEventCuts.h"

// DVUtil
#include "DisplacedDimuonAnalysis/DVUtils.h"


class VertexRes: public ::AthAnalysisAlgorithm { 
 public: 
  VertexRes( const std::string& name, ISvcLocator* pSvcLocator );
  virtual ~VertexRes(); 

  virtual StatusCode  initialize();
  virtual StatusCode  execute();
  virtual StatusCode  finalize();

 private: 
        ToolHandle<DDL::IDiLepDVCuts> m_dilepdvc; //!
        ToolHandle<DDL::IEventCuts> m_evtc; //!
        ToolHandle<IDVUtils>  m_dvutils; //!

        // DV mass accessor
        SG::AuxElement::ConstAccessor<float> m_accMass;

        // R resolution
        TH1F* m_dv_dimuon_res_R; //!

        // mass resolution
        TH1F* m_dv_dimuon_res_M; //!

}; 

#endif //> !DISPLACEDDIMUONANALYSIS_VERTEXRES_H
