#ifndef HISTADDER_H
#define HISTADDER_H
#include "Gaucho/MonTypes.h"
#include "Gaucho/MonInfo.h"
#include "Gaucho/ObjService.h"
#include <map>
#include <string>
#include "Gaucho/AddSerializer.h"
#include "Gaucho/MonAdder.h"

class ObjService;
class MonServer;
class ObjRPC;
//class HistAdder;
class MyTProfile;

class HistAdder : public MonAdder
{
public:
  DimHistbuff1 *m_RateBuff;
  std::string m_MyName;
  std::string m_NamePrefix;
  DimHistbuff1 *m_oldProf;
  void add(void *buffer, int siz, MonInfo *h);
  HistAdder(char *taskname, char *myName, char * serviceName/*, bool expand = false*/);
  ~HistAdder();
  void Configure(void);
//  void TaskHandler(char*,int);
//  void ServiceHandler(DimInfo *,char*,int);
};
#endif
