//------------------------------------------------------------
// Author : Siinn Che, siinn.che@cern.ch
//
// Calculate efficiency to reconstruct signal DV by comparing
// the truth muon and reco muon from DV with dR method
//------------------------------------------------------------

// DisplacedDimuonAnalysis includes
#include "DVEfficiency.h"
#include "GaudiKernel/ITHistSvc.h"
#include "AthenaKernel/errorcheck.h"

// xAOD
#include "xAODEventInfo/EventInfo.h"
#include "xAODTruth/TruthVertex.h"
#include "xAODTruth/TruthVertexContainer.h"
#include "xAODTracking/Vertex.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthParticle.h"
#include "xAODTracking/TrackParticleContainer.h"
#include "xAODTracking/TrackParticle.h"
#include "xAODCore/ShallowCopy.h"
#include "xAODBase/IParticleHelpers.h"
#include "xAODMuon/MuonContainer.h"
#include "xAODEgamma/ElectronContainer.h"

// tools
#include "PathResolver/PathResolver.h"

// plots 
#include "TProfile.h"
#include "TProfile2D.h"
#include "TH2D.h"
#include "cmath"
#include "vector"




DVEfficiency::DVEfficiency( const std::string& name, ISvcLocator* pSvcLocator ) : 
AthAnalysisAlgorithm( name, pSvcLocator ),
m_dilepdvc("DDL::DiLepDVCuts/DiLepDVCuts"),
m_evtc("DDL::EventCuts/DiLepEventCuts"),
m_dvc("DDL::DVCuts/DiLepBaseCuts"),
m_cos("DDL::DiLepCosmics/DiLepCosmics"),
m_dvutils("DVUtils"),
m_leptool("LeptonSelectionTools"),
m_costool("CosmicTools"),
m_grlTool("GoodRunsListSelectionTool/GRLTool"),
m_tdt("Trig::TrigDecisionTool/TrigDecisionTool"),
m_or("DDL::OverlapRemoval/OverlapRemoval"),
m_accMu("DDL_Muons"),
m_accEl("DDL_Electrons"),
m_accMass("mass")
{
    // initialize tools
    declareProperty("DiLepDVCuts", m_dilepdvc);
    declareProperty("DVUtils", m_dvutils);
    declareProperty("LeptonSelectionTool", m_leptool);
    declareProperty("CosmicTool", m_costool);
    declareProperty("DiLepEventCuts", m_evtc);
    declareProperty("DiLepBaseCuts", m_dvc);
    declareProperty("GRLTool",  m_grlTool, "The private GoodRunsListSelectionTool" );
    declareProperty("TrigDecisionTool", m_tdt);
    declareProperty("DiLepCosmics", m_cos);
    declareProperty("OverlapRemoval", m_or);
}


DVEfficiency::~DVEfficiency() {}


StatusCode DVEfficiency::initialize() {
    ATH_MSG_INFO ("Initializing " << name() << "...");

    //--------------------------------------------------------
    // Event cut
    //--------------------------------------------------------

    ServiceHandle<ITHistSvc> histSvc("THistSvc",name());

    m_dv_mass = new TH1F( "m_dv_mass", "Invariant mass of all signal vertex", 50, 0, 1500 ); // GeV

    // efficiency plots
    m_dv_eff_eta = new TEfficiency( "m_dv_eff_eta", "DV reconstruction efficiency vs eta", 50, -4.0, 4.0);
    m_dv_eff_phi = new TEfficiency( "m_dv_eff_phi", "DV reconstruction efficiency vs phi", 50, -M_PI, M_PI);
    m_dv_eff_mass = new TEfficiency( "m_dv_eff_mass", "DV reconstruction efficiency vs mass", 150, 0, 1500); // GeV
    m_dv_eff_R = new TEfficiency( "m_dv_eff_R", "DV reconstruction efficiency vs R", 100, 0, 400); // mm
    m_dv_eff_d0 = new TEfficiency( "m_dv_eff_d0", "DV reconstruction efficiency vs d0", 100, 0, 400); // mm

    // efficiency as a function of Z' parameters
    m_dv_eff_zp_eta = new TEfficiency("m_dv_eff_zp_eta", "DV reconstruction efficiency vs Z' eta", 25, -3.0, 3.0);
    m_dv_eff_zp_beta = new TEfficiency("m_dv_eff_zp_beta", "DV reconstruction efficiency vs Z' beta", 25, 0, 1.0);
    m_dv_eff_zp_pt = new TEfficiency("m_dv_eff_zp_pt", "DV reconstruction efficiency vs Z' pt", 25, 0, 500); // GeV

    // efficiency map (Z' pt and eta)
    m_dv_eff_map_pt_eta_den = new TH2F("m_dv_eff_map_pt_eta_den", "DV reconstruction efficiency map, pt vs eta (den)", 10, 0., 1000., 10, -3.0, 3.0); // GeV
    m_dv_eff_map_pt_eta_num = new TH2F("m_dv_eff_map_pt_eta_num", "DV reconstruction efficiency map, pt vs eta (num)", 10, 0., 1000., 10, -3.0, 3.0); // GeV
    m_dv_eff_map_pt_eta = new TH2F("m_dv_eff_map_pt_eta", "DV reconstruction efficiency map, pt vs eta", 10, 0., 1000., 10, -3.0, 3.0); // GeV
    //m_dv_eff_map_pt_eta = new TEfficiency("m_dv_eff_map_pt_eta", "DV reconstruction efficiency map, pt vs eta", 10, 0., 1000., 10, -3.0, 3.0); // GeV


    // error on reco DV
    m_dv_R_err_tight = new TH1F("dv_R_err_tight","Error on R of DV, tight [mm]",200,-1.,1.);
    m_dv_z_err_tight = new TH1F("dv_z_err_tight","Error on z of DV, tight [mm]",200,-1.,1.);
    m_dv_m_err_tight = new TH1F("dv_m_err_tight","Error on m of DV, tight [GeV]",200,-1.,1.);

    m_dv_R_err_loose = new TH1F("dv_R_err_loose","Error on R of DV, loose [mm]",200,-1.,1.);
    m_dv_z_err_loose = new TH1F("dv_z_err_loose","Error on z of DV, loose [mm]",200,-1.,1.);
    m_dv_m_err_loose = new TH1F("dv_m_err_loose","Error on m of DV, loose [GeV]",200,-1.,1.);
 

    // output
    CHECK( histSvc->regHist("/DV/truth/dv_mass", m_dv_mass) );

    // efficiency plots
    CHECK( histSvc->regGraph("/DV/truth/efficiency/dv/dv_eff_eta", reinterpret_cast<TGraph*>(m_dv_eff_eta)));
    CHECK( histSvc->regGraph("/DV/truth/efficiency/dv/dv_eff_phi", reinterpret_cast<TGraph*>(m_dv_eff_phi)));
    CHECK( histSvc->regGraph("/DV/truth/efficiency/dv/dv_eff_mass",reinterpret_cast<TGraph*>(m_dv_eff_mass)));
    CHECK( histSvc->regGraph("/DV/truth/efficiency/dv/dv_eff_R",reinterpret_cast<TGraph*>(m_dv_eff_R)) );
    CHECK( histSvc->regGraph("/DV/truth/efficiency/dv/dv_eff_d0",reinterpret_cast<TGraph*>(m_dv_eff_d0)) );

    CHECK( histSvc->regGraph("/DV/truth/efficiency/zp/dv_eff_zp_eta",reinterpret_cast<TGraph*>(m_dv_eff_zp_eta)) );
    CHECK( histSvc->regGraph("/DV/truth/efficiency/zp/dv_eff_zp_beta",reinterpret_cast<TGraph*>(m_dv_eff_zp_beta)) );
    CHECK( histSvc->regGraph("/DV/truth/efficiency/zp/dv_eff_zp_pt",reinterpret_cast<TGraph*>(m_dv_eff_zp_pt)) );

    CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_pt_eta", m_dv_eff_map_pt_eta) );
    ////CHECK( histSvc->regHist("/DV/truth/efficiency/zp/dv_eff_map_pt_eta", reinterpret_cast<TH2*>(m_dv_eff_map_pt_eta)) );



    CHECK( histSvc->regHist("/DV/truth/err/dv/tight/dv_R_err", m_dv_R_err_tight) );
    CHECK( histSvc->regHist("/DV/truth/err/dv/tight/dv_z_err", m_dv_z_err_tight) );
    CHECK( histSvc->regHist("/DV/truth/err/dv/tight/dv_m_err", m_dv_m_err_tight) );
    CHECK( histSvc->regHist("/DV/truth/err/dv/loose/dv_R_err", m_dv_R_err_loose) );
    CHECK( histSvc->regHist("/DV/truth/err/dv/loose/dv_z_err", m_dv_z_err_loose) );
    CHECK( histSvc->regHist("/DV/truth/err/dv/loose/dv_m_err", m_dv_m_err_loose) );

    return StatusCode::SUCCESS;
}

StatusCode DVEfficiency::finalize() {
    ATH_MSG_INFO ("Finalizing " << name() << "...");

    m_dv_eff_map_pt_eta->Divide(m_dv_eff_map_pt_eta_num, m_dv_eff_map_pt_eta_den,1,1, "b");
    
    return StatusCode::SUCCESS;
}

StatusCode DVEfficiency::execute() {  
    ATH_MSG_DEBUG ("Executing " << name() << "...");

    // retrieve event info
    const xAOD::EventInfo* evtInfo = nullptr;
    CHECK( evtStore()->retrieve( evtInfo, "EventInfo" ) );

    // flag to check if data or MC
    bool isMC = evtInfo->eventType(xAOD::EventInfo::IS_SIMULATION);

    // retrieve lepton container
    const xAOD::MuonContainer* muc = nullptr;
    CHECK( evtStore()->retrieve( muc, "Muons" ));

    const xAOD::ElectronContainer* elc = nullptr;
    CHECK( evtStore()->retrieve( elc, "Electrons" ));

    // make copies of leptons
    auto muc_copy = xAOD::shallowCopyContainer(*muc);
    xAOD::setOriginalObjectLink(*muc, *muc_copy.first);

    auto elc_copy = xAOD::shallowCopyContainer(*elc);
    xAOD::setOriginalObjectLink(*elc, *elc_copy.first);

    // apply overlap removal
    m_or->FindOverlap(*elc_copy.first, *muc_copy.first);

    // retrieve primary vertices
    const xAOD::VertexContainer* pvc = nullptr;
    CHECK( evtStore()->retrieve( pvc, "PrimaryVertices" ));

    // get primary vertex
    auto pv = m_evtc->GetPV(*pvc);

    // retrieve secondary vertices
    const xAOD::VertexContainer* dvc = nullptr;
    CHECK( evtStore()->retrieve( dvc, "VrtSecInclusive_SecondaryVertices" ));

    // make a copy of vertex containers
    auto dvc_copy = xAOD::shallowCopyContainer(*dvc);

    // perform lepton matching
    for(auto dv: *dvc_copy.first) {
        m_dilepdvc->ApplyLeptonMatching(*dv, *elc_copy.first, *muc_copy.first);
    }

    // set global flag
    bool dv_matched = false;

    // cut flow
    for(auto dv: *dvc_copy.first) {

        // access invariant mass
        float dv_mass = std::fabs(m_accMass(*dv)) / 1000.; // in GeV

        // collect leptons from this dv
        auto dv_muc = m_accMu(*dv);
        auto dv_elc = m_accEl(*dv);

        // remove overlapping muon
        m_dilepdvc->ApplyOverlapRemoval(*dv);

        // muon selection tool
        m_leptool->MuonSelection(*dv);

        // remove bad electrons
        m_leptool->BadClusterRemoval(*dv);

        // kinematic cut
        m_leptool->ElectronKinematicCut(*dv);

        // Electron identification
        m_leptool->ElectronID(*dv);

        // find decay channel of dv
        std::string channel = m_dvutils->DecayChannel(*dv);

        // only select mumu, ee, or emu
        if (!((channel == "mumu") or (channel == "emu") or (channel == "ee"))) continue;

        // Trigger matching
        m_dilepdvc->DoTriggerMatching(*dv);
        if(!m_dilepdvc->PassTriggerMatching(*dv)) continue;

        // vertex fit quality
        if(!m_dvc->PassChisqCut(*dv)) continue;

        // minimum distance from pv (from 0 for MC)
        if(!m_dvc->PassDistCut(*dv, *pvc)) continue;

        // charge requirements
        if(!m_dvc->PassChargeRequirement(*dv)) continue;

        // disabled module
        if(!m_dvc->PassDisabledModuleVeto(*dv)) continue;

        if ((channel == "emu") or (channel == "ee")) {
            if(!m_dvc->PassMaterialVeto(*dv)) continue;
        }

        // DESD filter
        //if(!m_dilepdvc->PassDESDMatching(*dv)) continue;

        // cosmic veto
        if(!PassCosmicVeto(*dv_muc, *dv_elc, channel)) continue;

        // delta R 
        float deltaR_min = 0.5;
        if(m_costool->getDeltaR(*dv_muc, *dv_elc, channel) < deltaR_min) break;

        // mark this event as reconstructed
        dv_matched = true;

        if (isMC) {
            // find truth dv matched to this dv
            const xAOD::TruthVertex* tru_v_tight = m_dvutils->IsSignalDV(*dv_muc, *dv_elc, channel);
            const xAOD::TruthVertex* tru_v_loose = m_dvutils->IsSignalDV_loose(*dv_muc, *dv_elc, channel, *dv);

            if (tru_v_tight != nullptr) {

                // flag this truth vertex as reconstructed
                //tru_v_tight->auxdecor<int>("reconstructed") = 1;

                // fill error
                float R_err = tru_v_tight->perp() - (*dv).position().perp();
                float z_err = tru_v_tight->z() - (*dv).position().z();
                float m_err = (m_dvutils->TruthMass(tru_v_tight) - std::fabs(m_accMass(*dv)) ) / m_dvutils->TruthMass(tru_v_tight); // GeV
                m_dv_R_err_tight->Fill(R_err);    // in mm
                m_dv_z_err_tight->Fill(z_err);    // in mm
                m_dv_m_err_tight->Fill(m_err);    // in mm
            }

            if (tru_v_loose != nullptr) {

                // flag this truth vertex as reconstructed
                tru_v_loose->auxdecor<int>("reconstructed") = 1;

                // fill error
                float R_err = tru_v_loose->perp() - (*dv).position().perp();
                float z_err = tru_v_loose->z() - (*dv).position().z();
                float m_err = (m_dvutils->TruthMass(tru_v_loose) - std::fabs(m_accMass(*dv)) ) / m_dvutils->TruthMass(tru_v_loose); // GeV
                m_dv_R_err_loose->Fill(R_err);    // in mm
                m_dv_z_err_loose->Fill(z_err);    // in mm
                m_dv_m_err_loose->Fill(m_err);    // in mm
            }

        } // end of isMC
    } // end of dv loop


    // GRL
    if (!isMC and !m_grlTool->passRunLB(*evtInfo)) dv_matched = false;

    // event cleaning
    if(!m_evtc->PassEventCleaning(*evtInfo)) dv_matched = false;

    // trigger check
    if(!m_evtc->PassTrigger()) dv_matched = false;

    //-----------------------------------------------------------------
    // end of cut flow                                                -
    // below is for dv reconstruction efficiency                      -
    //-----------------------------------------------------------------

    // retrieve truth vertex container
    const xAOD::TruthVertexContainer* tru_vc = nullptr;
    evtStore()->retrieve( tru_vc, "TruthVertices");

    // loop over the truth vertex container
    for (auto tru_v: *tru_vc){

        // only selecting signal truth
        if (!m_dvutils->isSignalVertex(tru_v)) continue;

        // -------- replace this with global dv_matched -------------
        // create accessor for reconstruction flag
        //static SG::AuxElement::ConstAccessor<int> acc_tru_v("reconstructed");
        //bool dv_matched = acc_tru_v.isAvailable(*tru_v);

        // fill truth signal vertex mass
        float DVMass = m_dvutils->TruthMass(tru_v) / 1000.;
        m_dv_mass->Fill(DVMass);
     
        // fill efficiency plots
        m_dv_eff_eta->Fill(dv_matched, tru_v->eta());
        m_dv_eff_phi->Fill(dv_matched, tru_v->phi());
        m_dv_eff_mass->Fill(dv_matched, DVMass);
        m_dv_eff_R->Fill(dv_matched, tru_v->perp());
        m_dv_eff_d0->Fill(dv_matched,m_dvutils->GetMaxd0(tru_v));

        // efficiency as a function of Z'
        float zp_eta = tru_v->incomingParticle(0)->eta();
        float zp_pt = tru_v->incomingParticle(0)->pt();
        float zp_m = tru_v->incomingParticle(0)->m();
        float zp_e = tru_v->incomingParticle(0)->e();
        float zp_beta = sqrt(1 - (zp_m/zp_e)*(zp_m/zp_e));

        m_dv_eff_zp_eta->Fill(dv_matched, zp_eta);
        m_dv_eff_zp_beta->Fill(dv_matched, zp_beta);
        m_dv_eff_zp_pt->Fill(dv_matched, zp_pt / 1000.);

        // fill efficiency map
        m_dv_eff_map_pt_eta_den->Fill(zp_pt / 1000., zp_eta);
        if (dv_matched) m_dv_eff_map_pt_eta_num->Fill(zp_pt / 1000., zp_eta); 


    } // end of efficiency loop


    return StatusCode::SUCCESS;
}

bool DVEfficiency::PassCosmicVeto(const DataVector<xAOD::Muon> dv_muc, const DataVector<xAOD::Electron> dv_elc, std::string channel) {

    bool PassCosmicVeto = true;

    float Rcos_min = 0.04;
    float deltaR_min = 0.5;
    float deltaPhiMinusPi = m_costool->getDeltaPhiMinusPi(dv_muc, dv_elc, channel);
    float sumEta = m_costool->getSumEta(dv_muc, dv_elc, channel);

    float Rcos = std::sqrt(sumEta * sumEta + deltaPhiMinusPi * deltaPhiMinusPi);
    float deltaR = m_costool->getDeltaR(dv_muc, dv_elc, channel);

    if (Rcos < Rcos_min) PassCosmicVeto = false;

    return PassCosmicVeto;
}

