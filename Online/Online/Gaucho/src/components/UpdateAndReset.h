#ifndef GAUCHO_UPDATEANDRESET_H
#define GAUCHO_UPDATEANDRESET_H 1

// Include files
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/Property.h"
#include "Gaucho/IGauchoMonitorSvc.h"
#include "GauchoTimer.h"
#include "dis.hxx"

class UpdateAndReset : public GaudiAlgorithm, public DimTimer {
public:
   /// Constructor of this form must be provided
   UpdateAndReset(const std::string& name, ISvcLocator* pSvcLocator); 
   
  /// Three mandatory member functions of any algorithm
  StatusCode initialize();
  StatusCode execute();
  StatusCode finalize();
  int getRunNumber();
  
  void timerHandler();
  
  void changeCycle(bool isRunNumberChanged, int currentRunNumber);
  
private:
  
  int m_generatedRunNumber;
  int m_countExecutes;

  IGauchoMonitorSvc* m_pGauchoMonitorSvc; ///< Online Gaucho Monitoring Service
  
  // MonRate information
  int    m_runNumber;       // Maybe we have to use double
  int    m_cycleNumber;
  longlong m_timeFirstEvInRun;
  
  int    m_runNumberDelayed;       // Maybe we have to use double
  int    m_cycleNumberDelayed;
  longlong m_timeFirstEvInRunDelayed;
  longlong m_timeLastEvInCycleDelayed; // Maybe we have to use double, have to check serialization
  
  int m_deltaTCycle;
  
  double m_runTestElapsedTime;
  GauchoTimer m_runTestTimer;
  double m_deltaTRunTest;
  
};  

#endif  // GAUCHO_UPDATEANDRESET_H


