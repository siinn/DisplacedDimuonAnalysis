#ifndef DVUTILS_H
#define DVUTILS_H

#include "AthenaBaseComps/AthAlgTool.h"
#include "IDVUtils.h"

#include "xAODTracking/Vertex.h"

class DVUtils : public AthAlgTool, virtual public IDVUtils {

    public:

        DVUtils (const std::string& type, const std::string& name, const IInterface* parent);
        virtual ~DVUtils();

        virtual StatusCode  initialize();
        virtual StatusCode  finalize();

        // add custom functions here
        float getR(const xAOD::Vertex& dv, const xAOD::Vertex& pv);

    private:


};

#endif //> !DVUTILS_H
