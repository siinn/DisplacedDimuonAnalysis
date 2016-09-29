// DisplacedDimuonAnalysis includes
#include "DisplacedDimuonAnalysis/DVUtils.h"
#include "xAODTracking/VertexContainer.h"

// DV

DVUtils::DVUtils( const std::string& type,const std::string& name, const IInterface* parent) :
AthAlgTool( type, name, parent )
{
    declareInterface<IDVUtils>(this);

}


DVUtils::~DVUtils() {}


StatusCode DVUtils::initialize() {
    
    //ATH_MSG_INFO ("Initializing " << name() << "...");
    //ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    // define histograms
    //m_dv_mass_all = new TH1F("dv_mass_all","All DV mass in GeV",50,0.,500.);
    //m_dv_mass_dimuon = new TH1F("dv_mass_dimuon","Dimuon DV mass in GeV",50,0.,500.);

    // registor for output
    //CHECK( histSvc->regHist("/DV/SecondaryVertex/dv_mass_all", m_dv_mass_all) );
    //CHECK( histSvc->regHist("/DV/SecondaryVertex/dv_mass_dimuon", m_dv_mass_dimuon) );

    //int n_dv = 0;
    //int n_muon_dv = 0;

    return StatusCode::SUCCESS;
}

StatusCode DVUtils::finalize() {
    return StatusCode::SUCCESS;
}

float DVUtils::getR(const xAOD::Vertex& dv, const xAOD::Vertex& pv) {
    // This function calculate perpendicular distance from hard-scattering
    // primary vertex to dv

    // access position of vertices
    auto dv_pos = dv.position();
    auto pv_pos = pv.position();

    // distance in 3d vector
    auto dist = pv_pos - dv_pos;

    // return distance in transverse plane
    return dist.perp();

}








