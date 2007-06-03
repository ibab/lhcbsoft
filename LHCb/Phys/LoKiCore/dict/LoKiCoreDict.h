// $Id: LoKiCoreDict.h,v 1.2 2007-06-03 20:38:24 ibelyaev Exp $
// ============================================================================
// CVS tag $Name: not supported by cvs2svn $, version $Revision: 1.2 $
// ============================================================================
// $Log: not supported by cvs2svn $
// Revision 1.1  2007/06/01 11:35:27  ibelyaev
//  prepare for v3r0
// 
// ============================================================================
#ifndef LOKI_LOKICOREDICT_H 
#define LOKI_LOKICOREDICT_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GaudiTool.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/LoKiCore.h"
#include "LoKi/Services.h"
#include "LoKi/Dicts.h"
// ============================================================================

namespace 
{
  struct _Interfaces 
  {
    // 
    LoKi::Interface<LoKi::ILoKiSvc>  m_1 ;
    LoKi::Interface<LoKi::IReporter> m_2 ;
    LoKi::Interface<GaudiTool>       m_3 ;
    LoKi::Interface<GaudiAlgorithm>  m_4 ;
    /// fictive constructor 
    _Interfaces() ;
  } ;  
}
// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_LOKICOREDICT_H
// ============================================================================
