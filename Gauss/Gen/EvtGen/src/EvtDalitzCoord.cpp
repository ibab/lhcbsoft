/*******************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: EvtGenBase
 *    File: $Id: EvtDalitzCoord.cpp,v 1.1 2003-10-02 17:34:48 robbep Exp $
 *  Author: Alexei Dvoretskii, dvoretsk@slac.stanford.edu, 2001-2002
 *
 * Copyright (C) 2002 Caltech
 *******************************************************************************/

#ifdef WIN32 
  #pragma warning( disable : 4786 ) 
  // Disable anoying warning about symbol size 
#endif 
#include <assert.h>
#include <iostream>
#include "EvtGenBase/EvtDalitzCoord.hh"
using EvtCyclic3::Pair;


// For coordinates it's good to alway have a
// default ctor. Initialize to something invalid.

EvtDalitzCoord::EvtDalitzCoord()
  : _i1(EvtCyclic3::AB), _i2(EvtCyclic3::BC), _q1(-1.), _q2(-1.)
{}

EvtDalitzCoord::EvtDalitzCoord(const EvtDalitzCoord& other)
  : _i1(other._i1), _i2(other._i2), _q1(other._q1), _q2(other._q2)
{}


EvtDalitzCoord::EvtDalitzCoord(Pair i1, double q1, Pair i2, double q2)
  : _i1(i1), _i2(i2),_q1(q1),_q2(q2)
{} 


EvtDalitzCoord::~EvtDalitzCoord()
{}


bool EvtDalitzCoord::operator==(const EvtDalitzCoord& other) const
{
  return (_i1 == other._i1 && _i2 == other._i2 && 
	  _q1 == other._q1 && _q2 == other._q2);
}

void EvtDalitzCoord::print(std::ostream& os) const
{
  os << _i1 << " " << _q1 << std::endl;
  os << _i2 << " " << _q2 << std::endl;
}


std::ostream& operator<<(std::ostream& os,const EvtDalitzCoord& p)
{
  p.print(os);
  return os;
}

