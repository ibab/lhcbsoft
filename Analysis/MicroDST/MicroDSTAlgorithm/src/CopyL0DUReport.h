// $Id: CopyL0DUReport.h,v 1.7 2009-04-30 15:43:48 jpalac Exp $
#ifndef COPYL0DUREPORT_H 
#define COPYL0DUREPORT_H 1

// Include files
// from MicroDST
#include "MicroDST/ObjectClonerAlg.h"
// from LHCb
#include "Event/L0DUReport.h"
/** @class CopyL0DUReport CopyL0DUReport.h
 *  
 *  MicroDSTAlgorithm to clone an LHCb::L0DUReport from one TES location to 
 *  another one. It inherits the std::string properties InputLocation and 
 *  OutputPrefix from MicroDSTCommon. The LHCb::L0DUReport is taken from the 
 *  TES location defined by InputLocation, and is cloned and put in 
 *  TES location "/Event" + OutputPrefix + InputLocation. 
 *  If InputLocation already contains a leading "/Event" it is removed.
 *
 *  <b>Example</b>: Clone the LHCb::L0DUReport from default location ("Trig/L0/L0DUReport") 
 *  to "/Event/MyLocation/Trig/L0/L0DUReport"
 *  @code
 *  mySeq = GaudiSequencer("SomeSequence")
 *  copyL0DU = CopyL0DUReport()
 *  copyL0DU.InputLocation = "Trig/L0/L0DUReport"
 *  copyL0DU.OutputPrefix = "MyLocation"
 *  mySeq.Members += [copyL0DU]
 *  @endcode
 *
 *  @author Juan PALACIOS juan.palacios@nikhef.nl
 *  @date   2008-04-16
 */
//=============================================================================
template <> struct Location<LHCb::L0DUReport> 
{
  const static std::string Default;
};
const std::string Location<LHCb::L0DUReport>::Default = LHCb::L0DUReportLocation::Default;
//=============================================================================
typedef MicroDST::ObjectClonerAlg<LHCb::L0DUReport> CopyL0DUReport;
// Declaration of the Algorithm Factory
DECLARE_NAMED_ALGORITHM_FACTORY( CopyL0DUReport, CopyL0DUReport )
#endif // COPYL0DUREPORT_H
