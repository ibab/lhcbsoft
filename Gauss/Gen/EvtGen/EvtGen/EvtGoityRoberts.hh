//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of the EvtGen package developed jointly
//      for the BaBar and CLEO collaborations.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/COPYRIGHT
//      Copyright (C) 1998      Caltech, UCSB
//
// Module: EvtGen/EvtGoityRoberts.hh
//
// Description:
//
// Modification history:
//
//    DJL/RYD     August 11, 1998         Module created
//
//------------------------------------------------------------------------

#ifndef EVTGOITYROBERTS_HH
#define EVTGOITYROBERTS_HH

#include "EvtGen/EvtDecayAmp.hh"
#include "EvtGen/EvtParticle.hh"
#include "EvtGen/EvtId.hh"

class EvtGoityRoberts:public  EvtDecayAmp  {

public:

  EvtGoityRoberts() {}
  virtual ~EvtGoityRoberts();

  void getName(EvtString& name);
  EvtDecayBase* clone();
  
  void init();
  void decay(EvtParticle *p); 
  void initProbMax();
  
 private:

  void DecayBDstarpilnuGR(EvtParticle *pb,EvtId ndstar,
                                        EvtId npion, EvtId nlep, EvtId nnu);

  void DecayBDpilnuGR(EvtParticle *pb,EvtId nd,
                                        EvtId npion, EvtId nlep, EvtId nnu);



};

#endif
