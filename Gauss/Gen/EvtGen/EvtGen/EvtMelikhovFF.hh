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
// Module: EvtGen/EvtMelikhovFF.hh
//
// Description: Form factors for EvtMelikhov model
//
// Modification history:
//
//    DJL     April 20, 1998         Module created
//
//------------------------------------------------------------------------

#ifndef EVTMELIKHOVFF_HH
#define EVTMELIKHOVFF_HH

#include "EvtGen/EvtSemiLeptonicFF.hh"
#include "EvtGen/EvtId.hh"

class EvtMelikhovFF : public EvtSemiLeptonicFF {

public:
  EvtMelikhovFF(double par1);
  void getvectorff( EvtId parent, EvtId daught,
                       double t, double mass, double *a1f,
                       double *a2f, double *vf, double *a0f );

private:
  int whichfit;

};

#endif

