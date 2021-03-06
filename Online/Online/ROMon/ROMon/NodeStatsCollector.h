// $Id: NodeStatsCollector.h,v 1.1 2008-11-11 18:31:09 frankb Exp $
//====================================================================
//  ROMon
//--------------------------------------------------------------------
//
//  Package    : ROMon
//
//  Description: Readout monitoring in the LHCb experiment
//
//  Author     : M.Frank
//  Created    : 29/1/2008
//
//====================================================================
// $Header: /afs/cern.ch/project/cvs/reps/lhcb/Online/ROMon/ROMon/NodeStatsCollector.h,v 1.1 2008-11-11 18:31:09 frankb Exp $
#ifndef ROMON_NODESTATSCOLLECTOR_H
#define ROMON_NODESTATSCOLLECTOR_H

// C++ include files
#include <cstdlib>

// Framework include files
#include "ROMon/ROMonServer.h"
#include "ROMon/RODimFSMListener.h"

/*
 *   ROMon namespace declaration
 */
namespace ROMon  {
  // Forward declarations
  class SysInfo;

  /**@class NodeStatsCollector ROMon.h GaudiOnline/ROMon.h
   *
   * Readout monitor DIM server for a single node
   *
   * @author M.Frank
   */
  class NodeStatsCollector {
  protected:
    SysInfo*          m_sys;
    int               m_print;
    bool              m_verbose;
    int               m_mbmDelay;
    int               m_mbmSvc;
    size_t            m_mbmSize;
    char*             m_mbmBuffer;
    int               m_statDelay;
    int               m_statSvc;
    size_t            m_statSize;
    char*             m_statBuffer;
    size_t            m_hltSize;
    int               m_hlt_1_Svc;
    int               m_hlt_2_Svc;
    char*             m_hlt_1_Buffer;
    char*             m_hlt_2_Buffer;
    int               m_overflow;
    int               m_overflowSvc;
    long              m_overflowTime;
    ROMonServer       m_mbm;
    RODimFSMListener  m_fsm;

  public:
    /// Standard constructor
    NodeStatsCollector(int argc, char** argv);
    /// Default destructor
    virtual ~NodeStatsCollector();
    /// Start monitoring activity
    virtual int monitor();
    /// Monitor Node statistics information
    virtual int monitorStats(const std::vector<std::string>& dir_names);
    /// Monitor deferred HLT statistics information
    virtual int monitorHLT(char* buffer, const std::vector<std::string>& dir_names);
    /// Monitor task and MBM information
    virtual int monitorTasks();
    /// Feed data to disk monitor services
    void feedHLT(char* data_buff, void** buf, int* size, int* first); 
    /// Help printout in case of -h /? or wrong arguments
    static void help();
    /// Feed data to DIS when updating data
    static void feedStats(void* tag, void** buf, int* size, int* first);
    static void feedMBM(void* tag, void** buf, int* size, int* first);
    static void feedHLT_1(void* tag, void** buf, int* size, int* first);
    static void feedHLT_2(void* tag, void** buf, int* size, int* first);
    /// Dim callback to retrieve the overflow status
    static void overflowHandler(void* tag, void* address, int* size);
  };
}      // End namespace ROMon
#endif /* ROMON_NODESTATSCOLLECTOR_H */

