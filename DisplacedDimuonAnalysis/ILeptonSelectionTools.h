#ifndef I_LEPTONSELECTIONTOOLS_H
#define I_LEPTONSELECTIONTOOLS_H

#include "GaudiKernel/IAlgTool.h"
#include "xAODTracking/Vertex.h"

static const InterfaceID IID_ILeptonSelectionTools("ILeptonSelectionTools", 1, 0);


class ILeptonSelectionTools: virtual public IAlgTool {

    public: 

        virtual ~ILeptonSelectionTools() {}; 
        static const InterfaceID& interfaceID(){return IID_ILeptonSelectionTools;};

        virtual StatusCode  initialize()=0;
        virtual StatusCode finalize()=0;

        // Remove electrons from bad cluster 
        virtual void BadClusterRemoval(xAOD::Vertex& dv) = 0;

        // Kenematic cut
        virtual void ElectronKinematicCut(xAOD::Vertex& dv) = 0;

        // Electron ID
        virtual void ElectronID(xAOD::Vertex& dv) = 0;

        // Muon selection
        virtual void MuonSelection(xAOD::Vertex& dv) = 0;
    
    private:
    
}; 

#endif // LEPTONSELECTIONTOOLS_H
