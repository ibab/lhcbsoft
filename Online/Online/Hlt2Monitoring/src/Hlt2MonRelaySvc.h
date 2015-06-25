#ifndef HLT2MONRELAYSVC_H
#define HLT2MONRELAYSVC_H

// STD & STL
#include <string>
#include <map>
#include <set>

// Gaudi
#include <GaudiKernel/Service.h>
#include <GaudiKernel/IIncidentListener.h>

// ZeroMQ
#include "Hlt2MonBaseSvc.h"

class ISvcLocator;
class IIncidentSvc;

/** @class Hlt2MonRelaySvc

 */

class Hlt2MonRelaySvc : public Hlt2MonBaseSvc {
public:

   Hlt2MonRelaySvc(const std::string& name, ISvcLocator* sl);
   virtual ~Hlt2MonRelaySvc();

   // Service pure virtual member functions
   virtual StatusCode initialize();

   // define the function to run in the thread
   virtual void function() override;

private:

   // properties
   std::string m_hostRegex;

};

#endif // HLT2MONRELAYSVC_H
