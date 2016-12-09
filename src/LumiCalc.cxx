// DisplacedDimuonAnalysis includes
#include "LumiCalc.h"
#include "xAODRootAccess/TEvent.h" //needs in requirements file: use xAODRootAccess xAODRootAccess-* Control
#include "xAODLuminosity/LumiBlockRangeContainer.h" //needs in requirements file: use xAODLuminosity xAODLuminosity-* Event/xAOD
#include "xAODLuminosity/LumiBlockRangeAuxContainer.h" //needs in requirements file: use xAODLuminosity xAODLuminosity-* Event/xAOD

//#include "xAODEventInfo/EventInfo.h"

#include "GaudiKernel/ITHistSvc.h"
//#include "TTree.h"
//#include "TH1D.h"



LumiCalc::LumiCalc( const std::string& name, ISvcLocator* pSvcLocator ) :
AthAnalysisAlgorithm( name, pSvcLocator )
{
    //declareProperty( "Property", m_nProperty ); //example property declaration
}


LumiCalc::~LumiCalc() {}


StatusCode LumiCalc::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");

    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());
    ServiceHandle<StoreGateSvc> metastore("StoreGateSvc/MetaDataStore",name());

    //use xAOD::RootAccess to create a metadata file
    xAOD::TEvent evt;
    //TFile* outFile = new TFile("metadata.out.root","RECREATE");
    TFile* outFile = new TFile("output.root","RECREATE");
    if(evt.writeTo( outFile ).isFailure()) return StatusCode::FAILURE;
    
    std::vector<std::string> keys;
    metastore->keys<xAOD::LumiBlockRangeContainer>(keys);
    for(auto& key : keys) {
        auto lbs = metastore->retrieveUniquePrivateCopy<xAOD::LumiBlockRangeContainer>(key); //returns unique_ptr
        auto lbsAux = metastore->retrieveUniquePrivateCopy<xAOD::LumiBlockRangeAuxContainer>(key+"Aux.");  //returns unique_ptr
        if( evt.recordMeta( lbs.release() , key ).isFailure() ) return StatusCode::FAILURE;
        if( evt.recordMeta( lbsAux.release() , key+"Aux.").isFailure() ) return StatusCode::FAILURE;
    }
    if( evt.finishWritingTo( outFile ).isFailure() ) return StatusCode::FAILURE;;

    

    TTree* metatree = dynamic_cast<TTree*>(outFile->Get("MetaData"))->CloneTree(); 
    metatree->SetDirectory(0);
    metatree->Scan();

    CHECK( histSvc->regTree("/DV/MetaData",metatree) );
    
    
    return StatusCode::SUCCESS;
}

StatusCode LumiCalc::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");
    //
    //Things that happen once at the end of the event loop go here
    //
    
    
    return StatusCode::SUCCESS;
}

StatusCode LumiCalc::execute() {  
    ATH_MSG_DEBUG ("Executing " << name() << "...");
    setFilterPassed(false); //optional: start with algorithm not passed
    
    
    
    //
    //Your main analysis code goes here
    //If you will use this algorithm to perform event skimming, you
    //should ensure the setFilterPassed method is called
    //If never called, the algorithm is assumed to have 'passed' by default
    //
    
    
    //HERE IS AN EXAMPLE
    //const xAOD::EventInfo* evtInfo = 0;
    //CHECK( evtStore()->retrieve( evtInfo, "EventInfo" ) );
    //ATH_MSG_INFO("eventNumber=" << evtInfo->eventNumber() );
    
    
    setFilterPassed(true); //if got here, assume that means algorithm passed
    return StatusCode::SUCCESS;
}

StatusCode LumiCalc::beginInputFile() { 
    //
    //This method is called at the start of each input file, even if
    //the input file contains no events. Accumulate metadata information here
    //
    
    //example of retrieval of CutBookkeepers: (remember you will need to include the necessary header files and use statements in requirements file)
    // const xAOD::CutBookkeeperContainer* bks = 0;
    // CHECK( inputMetaStore()->retrieve(bks, "CutBookkeepers") );
    
    //example of IOVMetaData retrieval (see https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/AthAnalysisBase#How_to_access_file_metadata_in_C)
    //float beamEnergy(0); CHECK( retrieveMetadata("/TagInfo","beam_energy",beamEnergy) );
    //std::vector<float> bunchPattern; CHECK( retrieveMetadata("/Digitiation/Parameters","BeamIntensityPattern",bunchPattern) );



  return StatusCode::SUCCESS;
}


