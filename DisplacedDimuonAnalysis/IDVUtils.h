#ifndef I_DVUTILS_H
#define I_DVUTILS_H

#include "GaudiKernel/IAlgTool.h"
#include "xAODTracking/Vertex.h"


static const InterfaceID IID_IDVUtils("IDVUtils", 1, 0);


class IDVUtils: virtual public IAlgTool {

    public: 

        virtual ~IDVUtils() {}; 
        static const InterfaceID& interfaceID(){return IID_IDVUtils;};

        virtual StatusCode  initialize()=0;
        virtual StatusCode finalize()=0;

        // add custom functions here

        virtual float getR(const xAOD::Vertex& dv, const xAOD::Vertex& pv) = 0;

    
    private:
    
}; 

#endif // DVUTILS_H
