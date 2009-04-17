// $Id: CtrlNodeDisplay.cpp,v 1.1 2009-04-17 13:17:04 frankb Exp $
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
// $Header: /afs/cern.ch/project/cvs/reps/lhcb/Online/ROMon/src/CtrlNodeDisplay.cpp,v 1.1 2009-04-17 13:17:04 frankb Exp $

// Framework include files
#include "ROMon/TaskSupervisor.h"
#include "ROMon/FarmDisplay.h"
#include "ROMon/CPUMon.h"
#include "CPP/Event.h"
#include "SCR/scr.h"
// C++ include files
#include <cstdlib>

using namespace ROMon;
using namespace SCR;
using namespace std;

CtrlNodeDisplay::CtrlNodeDisplay(FarmDisplay* parent, const string& title) 
  : InternalDisplay(parent,title), m_node(0)
{
  ::scrc_create_display(&m_display,55,130,MAGENTA,ON,"Node Control display for node:");
}

void CtrlNodeDisplay::update(const void* data) {
  char txt[255], name[255];
  time_t tim = ::time(0);
  int node=0, cnt=0, line=1;
  Cluster* c = (Cluster*)data;
  if ( c ) {
    const Cluster::Nodes& n = c->nodes;
    //const char* fmt = "  %-24s %12s %17zd / %-5zd     %17zd / %-5zd      %s";

    ::scrc_put_chars(m_display,"",NORMAL,++line,1,1);
    ::sprintf(txt, "  Cluster:%s  status:%12s  last update:%s",c->name.c_str(),c->status.c_str(),c->time.c_str());
    ::scrc_put_chars(m_display,txt,NORMAL,++line,1,1);
    for(Cluster::Nodes::const_iterator i=n.begin(); i!=n.end();++i, ++node) {
      if ( node == m_node ) {
        const Cluster::Node& n = (*i).second;
        Cluster::Node::Tasks::const_iterator j;
        ::strftime(name,sizeof(name),"%H:%M:%S",::localtime(&tim));
        ::sprintf(txt,"Task Control display for node:%s  [%s]",n.name.c_str(),name);
        ::scrc_set_border(m_display,txt,BLUE|INVERSE);

        ::sprintf(txt, "  Node:%s  last update:%s",n.name.c_str(),n.time.c_str());
        ::scrc_put_chars(m_display,txt,NORMAL,++line,1,1);

        ::sprintf(txt," %-12s %8s    Tasks       Connections  %6s %6s %6s %6s %3s %3s",
                  "","","RSS","Stack","Data","VSize","CPU","MEM");
        ::scrc_put_chars(m_display,txt,INVERSE,++line,1,1);
        ::sprintf(txt," %-12s %8s found/missing found/missing %6s %6s %6s %6s %3s %3s %s",
                  "Node","Status","[MB]","[MB]","[MB]","[MB]","[%]","[%]","Boot time");
        ::scrc_put_chars(m_display,txt,INVERSE,++line,1,1);
        ::sprintf(txt," %-12s %8s %5zd/%-7zd %5zd/%-7zd %6d %6d %6d %6d %3.0f %3.0f %s",
                  n.name.c_str(),n.status.c_str(),
                  n.taskCount,n.missTaskCount,n.connCount,n.missConnCount,
                  int(n.rss/1024),int(n.stack/1024),int(n.data/1024),int(n.vsize/1024),
                  n.perc_cpu, n.perc_mem, n.boot.c_str());
        ::scrc_put_chars(m_display,txt,NORMAL,++line,1,1);

        //::sprintf(txt,"  %-24s %12s %17s / Missing   %17s / Missing    %s","Node","Status","Tasks found","Connections found","Timestamp");
        //::scrc_put_chars(m_display,txt,INVERSE,++line,1,1);
        //::sprintf(txt,fmt,n.name.c_str(),n.status.c_str(),n.taskCount,n.missTaskCount,n.connCount,n.missConnCount,n.time.c_str());
        //::scrc_put_chars(m_display,txt,NORMAL,++line,1,1);
        if ( n.status == "DEAD" ) {
          ::scrc_put_chars(m_display,"",INVERSE|RED|BOLD,++line,1,1);
          ::scrc_put_chars(m_display,"                        -------------------------------------",INVERSE|RED|BOLD,++line,1,1);
          ::scrc_put_chars(m_display,"                        Node is DEAD. Severe problem present.",INVERSE|RED|BOLD,++line,1,1);
          ::scrc_put_chars(m_display,"                        -------------------------------------",INVERSE|RED|BOLD,++line,1,1);
          ::scrc_put_chars(m_display,"",INVERSE|RED|BOLD,++line,1,1);
        }
        else {
          typedef map<string,bool> _O;
          int l, x;
          _O ord;
          _O::iterator k;
          ::scrc_put_chars(m_display,"",NORMAL,++line,1,1);
          if ( n.taskCount > 0 ) {
            ::scrc_put_chars(m_display,"  Task Summary:",n.missTaskCount > 0 ? INVERSE|BOLD|RED : INVERSE|GREEN,++line,1,1);
            for(j=n.tasks.begin(); j != n.tasks.end(); ++j) ord[(*j).first]=(*j).second;
            for(k=ord.begin(),cnt=0,l=line,x=1; k !=ord.end(); ++k,++cnt) {
              bool ok = (*k).second;
              if ( cnt==int((ord.size()+1)/2) ) l = line, x=61;
              ::sprintf(txt,"   [%03d]  %-32s %-10s",cnt,(*k).first.c_str(),ok ? "OK" : "MISSING");
              ::scrc_put_chars(m_display,txt,ok ? NORMAL : INVERSE|RED,++l,x,1);
            }
            line += (ord.size()+3)/2;
            ord.clear();
          }
          else {
            ::scrc_put_chars(m_display,"  No tasks found",INVERSE|BOLD|RED,++line,1,1);
          }
          if ( n.projects.size() > 0 ) {
            cnt=0;
            ::sprintf(txt,"%-15s %-16s %-10s %-10s %-10s %-10s %-10s",
                      "PVSS Summary:","Project name","Event Mgr","Data Mgr","Dist Mgr","FSM Server","Dev Handler");
            ::scrc_put_chars(m_display,txt,INVERSE|GREEN,++line,1,1);
            for(Cluster::Projects::const_iterator q=n.projects.begin(); q != n.projects.end(); ++q,++cnt)  {
              const Cluster::PVSSProject& p = *q;
              bool ok = p.eventMgr && p.dataMgr && p.distMgr;
              ::sprintf(txt,"   [%03d]        %-16s %-10s %-10s %-10s %-10s %-10s",cnt,p.name.c_str(), 
                        p.eventMgr ? "RUNNING" : "DEAD",
                        p.dataMgr  ? "RUNNING" : "DEAD",
                        p.distMgr  ? "RUNNING" : "DEAD",
                        p.fsmSrv   ? "RUNNING" : "DEAD",
                        p.devHdlr  ? "RUNNING" : "DEAD");
              ::scrc_put_chars(m_display,txt,ok ? NORMAL : INVERSE|RED,++line,1,1);
            }
            ::scrc_put_chars(m_display,"",NORMAL,++line,1,1);
          }
          if ( n.connCount > 0 ) {
            ::scrc_put_chars(m_display,"  Connection Summary:",n.missConnCount > 0 ? INVERSE|BOLD|RED : INVERSE|GREEN,++line,1,1);
            for(j=n.conns.begin(); j != n.conns.end(); ++j) ord[(*j).first]=(*j).second;
            for(k=ord.begin(),cnt=0,l=line,x=1; k !=ord.end(); ++k,++cnt) {
              bool ok = (*k).second;
              if ( cnt==int((ord.size()+1)/2) ) l = line, x=61;
              ::sprintf(txt,"   [%03d]  %-32s %-10s",cnt,(*k).first.c_str(),ok ? "OK" : "MISSING");
              ::scrc_put_chars(m_display,txt,ok ? NORMAL : INVERSE|RED,++l,x,1);
            }
            line += (ord.size()+3)/2;
            ord.clear();
          }
          else {
            ::scrc_put_chars(m_display,"  No connections found",INVERSE|BOLD|RED,++line,1,1);
          }
          ::scrc_put_chars(m_display,"",NORMAL,++line,1,1);
          if ( n.missTaskCount == 0 && n.missConnCount == 0 )
            ::scrc_put_chars(m_display,"  Node looks fine. No missing tasks/network connections found.",INVERSE|GREEN,++line,1,1);
          else if ( n.missTaskCount == 0 )
            ::scrc_put_chars(m_display,"  No missing tasks found, but missing network connections .....",INVERSE|RED,++line,1,1);
          else if ( n.missConnCount == 0 )
            ::scrc_put_chars(m_display,"  No missing network connections found, but missing tasks .....",INVERSE|RED,++line,1,1);
          ::scrc_put_chars(m_display,"",NORMAL,++line,1,1);
        }
      }
    }
  }
  else {
    ::scrc_put_chars(m_display,"   Unknown Node. No data found.",INVERSE|BOLD,++line,1,1);
    ::strftime(txt,sizeof(txt),"           %H:%M:%S",::localtime(&tim));
    ::scrc_put_chars(m_display,txt,INVERSE|BOLD,++line,1,1);
    ::scrc_set_border(m_display,"Unknown Node. No data found.",INVERSE|RED|BOLD);
  }
  ::memset(txt,' ',m_display->cols);
  txt[m_display->cols-1]=0;
  while(line<m_display->rows)
    ::scrc_put_chars(m_display,txt,NORMAL,++line,1,1);
}
