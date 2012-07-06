#include "HLTFileEqualizer.h"
#include <math.h>
#include <time.h>

HLTFileEqualizer::HLTFileEqualizer()
{
  DimUpdatedInfo *sfinfo;
  m_InfoHandler = new DefHltInfoHandler(this);
  static int NoLink=-1;
  char sf[128];
  char row;
  m_nnodes = 0;
  m_nfiles = 0;
  m_nfiles2 = 0;
  m_low = -5;
  m_high = 50;
  m_enabledFarm.clear();
  for (row='b';row<='e';row++)
  {
    for (int rack=1;rack<=11;rack++)
    {
      if (rack==5) continue;
      sprintf(sf,"/RO/hlt%c%02d/ROpublish/HLTDefer",row,rack);
      sfinfo = new DimUpdatedInfo(sf,(void*)&NoLink,sizeof(int),m_InfoHandler);
      m_infoMap.insert(std::make_pair(std::string(sf),sfinfo));
      printf ("%s\n",sf);
    }
  }
  row = 'f';
  for (int rack=1;rack<=7;rack++)
  {
    if (rack==5) continue;
    sprintf(sf,"/RO/hlt%c%02d/ROpublish/HLTDefer",row,rack);
    sfinfo = new DimUpdatedInfo(sf,(void*)&NoLink,sizeof(int),m_InfoHandler);
    m_infoMap.insert(std::make_pair(std::string(sf),sfinfo));
    printf ("%s\n",sf);
  }
//  m_enabledFarm.insert(std::string("hltb01"));
//  m_enabledFarm.insert(std::string("hltb02"));
}
namespace
{
  inline void toLowerCase(std::string &s)
  {
    for (unsigned int i=0;i<s.size();i++)
    {
      s[i] = tolower(s[i]);
    }
  }
  inline void toUpperCase(std::string &s)
  {
    for (unsigned int i=0;i<s.size();i++)
    {
      s[i] = toupper(s[i]);
    }
  }
}
void HLTFileEqualizer::Analyze()
{
  myActionMap Actions;
  dim_lock();
  float av_files;
  float rms;
  if (0 != m_nnodes)
  {
    av_files = float(this->m_nfiles)/this->m_nnodes;
    rms = float(m_nfiles2)/m_nnodes-av_files*av_files;
    if (0.0<rms)
    {
      rms = sqrt(rms);
    }
    else
    {
      rms = 0.0;
    }
  }
  else
  {
    if (0 != m_nfiles)
    {
      av_files = 100000000.0;
    }
    else
    {
      av_files = 0.0;
    }
    rms = float(m_nfiles2)/m_nnodes-av_files*av_files;
    if (0.0<rms)
    {
      rms = ::sqrt(rms);
    }
    else
    {
      rms = 0.0;
    }
  }
  printf("Analysis Summary based on %d nodes: ",m_nnodes);
  {
    time_t rawtime;
    time(&rawtime);
//    struct tm *ttt;
//    ttt = localtime(&rawtime);
    printf("%s",asctime(localtime(&rawtime)));
  }
  int n_ena = 0;
  int n_dis = 0;
  int nfiles = 0;
  int nfiles2 = 0;
  int nnodes = 0;
  bool act = (m_DefStateInfo->getInt() == 1);
  printf("Analyzer: First round of analysis Average number of files per node: %f +/- %f\n",av_files,rms);
  for (myNodeMap::iterator nit=m_Nodes.begin();nit != m_Nodes.end();nit++)
  {
    myNode *nod = (*nit).second;
    if ((nod->m_nofiles > av_files+5.0*rms) || (nod->m_nofiles <av_files-5.0*rms))
    {
      continue;
    }
    nfiles += nod->m_nofiles;
    nfiles2 += nod->m_nofiles*nod->m_nofiles;
    nnodes++;
  }
  if (nnodes >0)
  {
    av_files = float(nfiles)/nnodes;
    rms = float(nfiles2)/nnodes - av_files*av_files;
    if (rms>0)
    {
      rms = ::sqrt(rms);
    }
    else
    {
      rms = 0.0;
    }
  }
  else
  {
    av_files = 0;
    rms = 0.0;
  }
  for (myNodeMap::iterator nit=m_Nodes.begin();nit != m_Nodes.end();nit++)
  {
    myNode *nod = (*nit).second;
    if (nod->m_nofiles > av_files+m_high)
    {
      if (nod->m_state == 1)
      {
        std::string farm;
        farm = nod->m_name.substr(0,6);
        Actions[farm].push_back(std::make_pair(nod->m_name,0));
        nod->m_state = 0;
      }
      n_dis++;
    }
    else if (nod->m_nofiles < av_files+m_low)
    {
      std::string farm;
      farm = nod->m_name.substr(0, 6);
      nod->m_state = 1;
      Actions[farm].push_back(std::make_pair(nod->m_name,1));
    }
    n_ena++;
  }
  printf("Analyzer: Second round (within +/- 5 sigma) of analysis Average number of files per node: %f +/- %f\n",av_files,rms);
  printf("%d Nodes enabled; %d Nodes disabled\n",n_ena,n_dis);
  m_nnodes = 0;
  m_nfiles = 0;
  m_nfiles2 = 0;
  if (act)
  {
    myActionMap::iterator fit;
    for (fit = Actions.begin(); fit != Actions.end(); fit++)
    {
      //    DimClient::setDnsNode((*fit).first.c_str());
      if (!m_enabledFarm.empty() && (m_enabledFarm.find((*fit).first)
          == m_enabledFarm.end()))
      {
        continue;
      }
      //    printf("On Farm %s:\n",(*fit).first.c_str());
      std::list<std::pair<std::string, int> >::iterator i;
      std::string sf_mesg = "";
      std::string endisSvc;
      endisSvc = (*fit).first + "_HLTDefBridge/EnDisCommand";
      for (i = (*fit).second.begin(); i != (*fit).second.end(); i++)
      {
        std::string svcname;
        std::string node = (*i).first;
        toUpperCase(node);
        svcname = node + "_MEPRx_01/setOverflow";
        char cmd[1024];
        sprintf(cmd, "%s %d|", svcname.c_str(), (*i).second);
        sf_mesg.append(cmd);

      }
      DimClient::sendCommandNB(endisSvc.c_str(), (char*) sf_mesg.c_str());
    }
  }
  else
  {
    printf("Defered HLT disabled. Not acting...\n");
  }
  printf ("==================\n");
  fflush(stdout);
  dim_unlock();
}

DefHltInfoHandler::DefHltInfoHandler(HLTFileEqualizer *e)
{
  m_Equalizer = e;
//      m_subfarm = sf;
}
void DefHltInfoHandler::infoHandler()
{
  int siz;
//  int status;
  siz = this->itsService->getSize();
//      gettimeofday()
  if (siz == sizeof(int)) return;
  m_sfstatus = (_S*)this->itsService->getData();
  const _S* stats = m_sfstatus;
  const _N& nodes = *(stats->nodes());
//      _N::const_iterator i;
  for (_N::const_iterator i = nodes.begin();i!= nodes.end();i=nodes.next(i))
  {
    const _R& runs = (*i).runs;
    std::string nname;
    nname = (*i).name;
    myNodeMap::iterator nit;
    myNodeMap::iterator anit;
    anit = m_Equalizer->m_AllNodes.find(nname);
    if (anit == m_Equalizer->m_AllNodes.end())
    {
      myNode* nod = new myNode(nname);
      m_Equalizer->m_AllNodes.insert(std::make_pair(nname,nod));
    }
//    printf("%s: ",(*i).name);
    int nfiles=0;
    for (_R::const_iterator j = runs.begin();j!= runs.end();j=runs.next(j))
    {
      nfiles += (*j).second;
    }
//    printf("%d Files\n",nfiles);
    if (nfiles>0)
    {
      nit = m_Equalizer->m_Nodes.find(nname);
      if (nit == m_Equalizer->m_Nodes.end())
      {
        anit = m_Equalizer->m_AllNodes.find(nname);
        myNode* nod;
        if (anit == m_Equalizer->m_AllNodes.end())
        {
          nod = new myNode(nname);
          m_Equalizer->m_AllNodes.insert(std::make_pair(nname,nod));
          m_Equalizer->m_Nodes.insert(std::make_pair(nname,nod));
        }
        else
        {
          nod = (*anit).second;
          m_Equalizer->m_Nodes.insert(std::make_pair(nname,nod));
        }
      }
      m_Equalizer->m_Nodes[nname]->m_nofiles = nfiles;
//      m_Equalizer->m_Nodes[nname]->m_state *= (*i).overflowState;
      m_Equalizer->m_nfiles += nfiles;
      m_Equalizer->m_nfiles2 += nfiles*nfiles;
      m_Equalizer->m_nnodes++;
    }
  }
//  status =1;
  return;
}

class ExitCommand : public DimCommand
{
public:
  myNodeMap *m_nodemap;
  ExitCommand(const char *name, char *format, myNodeMap *nodm):  DimCommand(name, format)
  {
    m_nodemap = nodm;
  }
  void ableAll(int StateValue)
  {
  myActionMap Actions;
  myActionMap::iterator fit;
  myNodeMap::iterator nit;
  for (nit = m_nodemap->begin();nit != m_nodemap->end();nit++)
  {
    Actions[(*nit).second->m_subfarm].push_back(std::make_pair((*nit).first,StateValue));
//      std::string svcname;
//      std::string node = (*nit).first;
//      toUpperCase(node);
//      svcname = node+"_MEPRx_01/setOverflow";
////      DimClient::sendCommand(svcname.c_str(), (*i).second);
//      char cmd[1024];
//      sprintf(cmd,"dim_send_command.exe %s %d -dns %s -s -i&",svcname.c_str(),1,(*nit).second->m_subfarm.c_str());
//      ::system(cmd);
  }
  for (fit = Actions.begin();fit!=Actions.end();fit++)
  {
//      if (!m_enabledFarm.empty() && (m_enabledFarm.find((*fit).first) == m_enabledFarm.end()))
//      {
//        continue;
//      }
    std::list<std::pair<std::string,int> >::iterator i;
    std::string sf_mesg = "";
    std::string endisSvc;
    endisSvc = (*fit).first+"_HLTDefBridge/EnDisCommand";
    for (i =(*fit).second.begin();i != (*fit).second.end();i++)
    {
      std::string svcname;
      std::string node = (*i).first;
      toUpperCase(node);
      svcname = node+"_MEPRx_01/setOverflow";
//      DimClient::sendCommand(svcname.c_str(), (*i).second);
      char cmd[1024];
//      sprintf(cmd,"dim_send_command.exe %s %d -dns %s -s -i&",svcname.c_str(),(*i).second,(*fit).first.c_str());
      sprintf(cmd,"%s %d|",svcname.c_str(),StateValue);
      sf_mesg.append(cmd);

//      ::system(cmd);
//      printf("\tMEPRX on Node %s (%s) value %d\n",(*i).first.c_str(),svcname.c_str(),(*i).second);
    }
    DimClient::sendCommandNB(endisSvc.c_str(),(void*)(sf_mesg.c_str()),sf_mesg.size());
    printf("message to Subfarm %s:\n%s\n",(*fit).first.c_str(),sf_mesg.c_str());
  }
//  ::sleep(5);
//  ::exit(0);
}

  void enableAll()
  {
    ableAll(1);
  }
  void disableAll()
  {
    ableAll(0);
  }
  virtual void commandHandler()
  {
    int command = getInt();
    switch (command)
    {
      case 0:
      {
        ableAll(0);
        break;
      }
      case 1:
      {
        ableAll(1);
        break;
      }
      case 2:
      {
        ableAll(1);
        ::exit(0);
      }

    }
  }
};

int main(int argc, char **argv)
{
  DimClient::setDnsNode("ecs03");
  DimServer::setDnsNode("ecs03");
  HLTFileEqualizer elz;
  int m_DefState = -1;
  DimServer::start("HLTFileEqualizer");
  DimServer::autoStartOn();
  int low,high;
  if (argc == 3)
  {
    sscanf(argv[1],"%d",&low);
    sscanf(argv[2],"%d",&high);
    elz.m_low = low;
    elz.m_high = high;
  }
  DimInfo *defstate=new DimInfo("RunInfo/LHCb/DeferHLT",m_DefState);
  elz.m_DefStateInfo = defstate;
  ExitCommand EnableandExit("HLTFileEqualizer/EnableAndExit",(char*)"I",&elz.m_AllNodes);
  while (1)
  {
    sleep (60);
    elz.Analyze();
  }
  return 0;
}
