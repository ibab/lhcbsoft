#ifndef GAUDIKERNEL_AdderSVC_H
#define GAUDIKERNEL_AdderSVC_H

#include "GaudiKernel/Service.h"
//#include "GaudiKernel/IMonitorSvc.h"
//#include "Gaucho/IGauchoMonitorSvc.h"
//#include "GaudiKernel/StatEntity.h"
#include "Gaucho/HistAdder.h"
#include "Gaucho/CounterAdder.h"
#include "Gaucho/SaveTimer.h"
#include <string>
#include <map>
#include <set>

// Forward declarations
class ISvcLocator;
//class MonObject;
//class MonSys;
//class MonSubSys;
//class CntrMgr;

//namespace LHCb
//{

class MyErrh : public DimErrorHandler
{
public:
  bool m_flag;
  void errorHandler(int severity, int code, char *msg)
  {
    if (m_flag)
    {
      printf("[INFO] DIM Error from AdderSvc: Severity %d, Code %x %s\n",severity,code,msg);
    }
    return;
  }
  MyErrh () : DimErrorHandler()
  {
    m_flag = true;
  }
  void start()
  {
    m_flag = true;
  }
  void stop()
  {
    m_flag = false;
  }
};
class AdderSvc : public Service
{
public:
  AdderSvc(const std::string& name, ISvcLocator* sl);
  virtual ~AdderSvc();
  //IInterface pure virtual member functions
  StatusCode queryInterface(const InterfaceID& riid, void** ppvIF);
  // Service pure virtual member functions
  StatusCode configure(){return StatusCode::SUCCESS;};
  StatusCode initialize();
  StatusCode start();
  StatusCode stop();
  StatusCode finalize();

private:

  std::string m_utgid;
  MonAdder *m_adder;
  MonAdder *m_EoRadder;
  std::string m_TaskName;
  std::string m_PartitionName;
  std::string m_SaverTaskName;
  std::string m_MyName;
  std::string m_ServiceName;
  std::string m_InputDNS;
  std::string m_OutputDNS;
  std::string m_TaskPattern;
  std::string m_ServicePattern;
  std::string m_AdderType;
  bool m_TopLevel;
  bool m_ExpandRate;
  bool m_isSaver;
  bool m_isEOR;
  bool m_started;
  SaveTimer *m_SaveTimer;
  SaveTimer *m_EoRSaver;
  std::string m_SaveRootDir;
  int m_SaveInterval; //in seconds
  MyErrh *m_errh;


  // MonObjetc to convert conters in rates
//  MonRate  *m_monRate;


public:

};
//}
#endif // GAUDIKERNEL_AdderSVC_H
