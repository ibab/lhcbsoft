#ifndef ONLINE_GAUCHO_BUSYPUB_H
#define ONLINE_GAUCHO_BUSYPUB_H

#include "GaudiKernel/Service.h"
#include "GaudiKernel/IToolSvc.h"
#include "GauchoAppl/MonAdder.h"
#include "PubSvc.h"

// Forward declarations
class DimService;
//class ISimpleTrendWriter;

class BusyPub : public PubSvc
{
public:
  BusyPub(const std::string& name, ISvcLocator* sl);
  virtual ~BusyPub();
  //IInterface pure virtual member functions
  StatusCode queryInterface(const InterfaceID& riid, void** ppvIF);
  StatusCode start();
  StatusCode initialize();
  StatusCode finalize();
  void analyze(void *buff, int siz,MonMap* mmap);
//  unsigned long long m_prevupdate;
  bool m_enableTrending;
  DimService *m_FarmLoad;
  DimService *m_Cores;
  DimService *m_Moores;
  float m_fLoad;
  std::string m_ForcePartition;
  bool m_GlobalPublisher;
  static double &globalBogos();
//  double m_GlobalBogos;
//  ISimpleTrendWriter* m_trender;
};
#endif // ONLINE_GAUCHO_BUSYPUB_H
