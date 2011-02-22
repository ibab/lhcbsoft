#ifndef COUNTERADDER_H
#define COUNTERADDER_H
#include "Gaucho/MonTypes.h"
#include "Gaucho/MonInfo.h"
#include "Gaucho/ObjService.h"
#include <map>
#include <string>
#include "Gaucho/AddSerializer.h"
#include "Gaucho/MonAdder.h"
#include "RTL/rtl.h"

class MonService;
//class HistServer;
class ObjRPC;

class CounterAdder : public MonAdder
{
public:
    DimHistbuff1 *m_RateBuff;
    std::string m_MyName;
    std::string m_NamePrefix;
    std::string m_outsvcname;
    DimHistbuff1 *m_oldProf;
    bool m_isSaver;
    void add(void *buffer, int siz, MonInfo *h);
    CounterAdder(char *taskname, char *myName, char * serviceName/*, bool expand = false*/);
    ~CounterAdder();
    void Configure(void);
    void TaskHandler(char*,int);
    void ServiceHandler(DimInfo *,char*,int);
    void setIsSaver(bool p)
    {
      if (p)
      {
        lib_rtl_create_lock(0,&m_lockid);
      }
      m_isSaver = p;
      return;
    };
    lib_rtl_lock_t m_lockid;
    lib_rtl_lock_t m_maplock;
    int Lock()
    {
      if (m_lockid != 0)
      {
        int status;
  //      printf("HistAdder Locking\n");
        status = lib_rtl_lock(m_lockid);
  //      printf("HistAdder Locked\n");
        return status;
      }
      else
      {
        return 0;
      }
    };
    int UnLock()
    {
      if (m_lockid != 0)
      {
        int status;
  //      printf("HistAdder Un-Locking\n");
        status = lib_rtl_unlock(m_lockid);
  //      printf("HistAdder UnLocked\n");
        return status;
      }
      else
      {
        return 0;
      }
    };
    int LockMap()
    {
      if (m_maplock != 0)
      {
        int status;
        status = lib_rtl_lock(m_maplock);
        return status;
      }
      else
      {
        return 0;
      }
    };
    int UnLockMap()
    {
      if (m_maplock != 0)
      {
        int status;
        status = lib_rtl_unlock(m_maplock);
        return status;
      }
      else
      {
        return 0;
      }
    };
  };
#endif
