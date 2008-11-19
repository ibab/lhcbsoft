// $Id: AlarmListener.cpp,v 1.4 2008-11-19 11:09:38 frankb Exp $
//====================================================================
//  ROLogger
//--------------------------------------------------------------------
//
//  Package    : ROLogger
//
//  Description: Readout monitoring in the LHCb experiment
//
//  Author     : M.Frank
//  Created    : 29/1/2008
//
//====================================================================
// $Header: /afs/cern.ch/project/cvs/reps/lhcb/Online/ROLogger/src/AlarmListener.cpp,v 1.4 2008-11-19 11:09:38 frankb Exp $

// Framework include files
#include "ROLogger/AlarmListener.h"
#include "UPI/UpiSensor.h"
#include "CPP/IocSensor.h"
#include "UPI/upidef.h"
#include "ROLoggerDefs.h"

#include <cstring>
#include <vector>
#include <memory>

extern "C" {
#include "dic.h"
}
#ifdef _WIN32
#include <winsock2.h>
#else
#include <netdb.h>
#include <arpa/inet.h>
#endif

using namespace ROLogger;
using namespace std;
typedef vector<string> _SV;
static std::string facility = "alarms";

/// Standard constructor with object setup through parameters
AlarmListener::AlarmListener(Interactor* parent,const string& n) : m_parent(parent), m_name(n)
{
  auto_ptr<_SV> f(new _SV());
  string nam = "RunInfo/" + m_name + "/HLTsubFarms";
  m_subFarmDP = ::dic_info_service((char*)nam.c_str(),MONITORED,0,0,0,subFarmHandler,(long)this,0,0);
  ::upic_write_message2("Subfarm content for %s_RunInfo from:%s",m_name.c_str(),nam.c_str());
  f->push_back(fmcLogger("STORECTL01",facility));
  f->push_back(fmcLogger("MONA08",facility));
  f->push_back(fmcLogger("MONA09",facility));
  if ( name() == "LHCb" ) f->push_back(fmcLogger("CALD07",facility));
  IocSensor::instance().send(m_parent,CMD_UPDATE_FARMS,f.release());
}

/// Standard destructor
AlarmListener::~AlarmListener() {
  ::dic_release_service(m_subFarmDP);
}

/// DIM command service callback
void AlarmListener::subFarmHandler(void* tag, void* address, int* size) {
  auto_ptr<_SV> f(new _SV());
  AlarmListener* h = *(AlarmListener**)tag;
  for(const char* data = (char*)address, *end=data+*size;data<end;data += strlen(data)+1)
    f->push_back(fmcLogger(data,facility));
  f->push_back(fmcLogger("STORECTL01",facility));
  f->push_back(fmcLogger("MONA08",facility));
  f->push_back(fmcLogger("MONA09",facility));
  if ( h->name() == "LHCb" ) f->push_back(fmcLogger("CALD07",facility));
  IocSensor::instance().send(h->m_parent,CMD_UPDATE_FARMS,f.release());
}
