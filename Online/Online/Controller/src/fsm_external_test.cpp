/*============================================================
**
**      Ctrl process management on a HLT worker node
**
**  AUTHORS:
**
**      M.Frank
**
**==========================================================*/
// Framework include files
#include "FiniteStateMachine/Slave.h"
#include "FiniteStateMachine/Machine.h"
#include "FiniteStateMachine/FSMTypes.h"
#include "FiniteStateMachine/Functors.h"
#include "FiniteStateMachine/TestAutoTrans.h"
#include "FiniteStateMachine/Controller.h"
#include "FiniteStateMachine/DimSlave.h"

#include "CPP/IocSensor.h"
#include "RTL/rtl.h"
#include "dis.hxx"

#include <boost/assign/std/vector.hpp>

// C/C++ include files
#include <iostream>
#include <cstdio>

using namespace std;
using namespace boost::assign; // bring 'operator+=()' into scope
using namespace FiniteStateMachine;
using namespace FiniteStateMachine::DAQ;
typedef FSM::ErrCond ErrCond;

namespace   {
  struct ExternalSlave : public DimSlave {
    ExternalSlave(const Type* typ, const string& nam, Machine* machine) : DimSlave(typ,nam,machine)   {
      m_cmd = "/afs/cern.ch/user/f/frankb/cmtuser/Gaudi_v23r5/InstallArea/x86_64-slc5-gcc46-dbg/bin/external_fsm_client.exe";
      m_argv += name(),"-name="+name();
      cloneEnv();
    }
    virtual ~ExternalSlave() {}
  };
}

static void help_fun() {}
static string make_slave_name(int i) {
  char text[32];
  ::sprintf(text,"SLAVE_%d",i);
  return text;
}

extern "C" int external_fsm_client(int argc, char** argv)  {
  int print = -0;
  string dim_name = "SLAVE";
  RTL::CLI cli(argc, argv, help_fun);
  cli.getopt("name",2,dim_name);
  cli.getopt("print",1,print);
  TypedObject::setPrintLevel(print);
  DAQCommandTarget(dim_name).run();
  return 1;
}

extern "C" int external_fsm_test(int argc, char** argv)  {
  string dim_name = "Ctrl";
  int print = 0, rounds = -1, num_slaves=2;
  RTL::CLI cli(argc, argv, help_fun);
  bool auto_start = cli.getopt("auto",2);
  cli.getopt("name",2,dim_name);
  cli.getopt("rounds",1,rounds);
  cli.getopt("print",1,print);
  cli.getopt("slaves",1,num_slaves);

  TypedObject::setPrintLevel(print);

  const Type*    d = fsm_type("DAQ");
  Machine*       m = new Machine(d,"daq++");
  TestAutoTrans* a = new TestAutoTrans(m,rounds);
  a->configDAQ().sleep["load"] = 2;

  for(int i=0; i<num_slaves; ++i)
    m->addSlave(new ExternalSlave(d,make_slave_name(i),m));

  m->setHandler(a);
  if ( auto_start ) a->go_to(ST_NAME_NOT_READY);

  DimServer::autoStartOn();
  DimServer::start(dim_name.c_str());
  IocSensor::instance().run();
  return 1;
}

extern "C" int controller_fsm_test(int argc, char** argv)  {
  string dim_name = "Ctrl";
  int    print = 0, num_slaves=2;
  RTL::CLI cli(argc, argv, help_fun);
  cli.getopt("name",2,dim_name);
  cli.getopt("print",1,print);
  cli.getopt("slaves",1,num_slaves);
  TypedObject::setPrintLevel(print);

  const Type* d = fsm_type("DAQ");
  Machine*    m = new Machine(d,dim_name+"::daq++");
  for(int i=0; i<num_slaves; ++i)
    m->addSlave(new ExternalSlave(d,make_slave_name(i),m));

  Controller* target = new Controller(dim_name,m);
  m->setHandler(target);
  target->display(target->ALWAYS,"Controller task started...");
  DimServer::autoStartOn();
  DimServer::start(dim_name.c_str());
  target->run();
  return 1;
}
