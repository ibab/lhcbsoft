// $Id: MCPIDOperators.h,v 1.1.1.1 2006-01-26 16:13:39 ibelyaev Exp $
// ============================================================================
// CVS tag $Name: not supported by cvs2svn $ , version $Revision: 1.1.1.1 $
// ============================================================================
// $Log: not supported by cvs2svn $
// ============================================================================
#ifndef LOKI_MCPIDOPERATORS_H 
#define LOKI_MCPIDOPERATORS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL 
// ============================================================================
#include <string>
// ============================================================================
// LHCnKernel
// ============================================================================
#include "Kernel/ParticleID.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Functions.h"
#include "LoKi/Operators.h"
// ============================================================================
// forwarde declarations 
// ============================================================================
namespace LHCb
{
  class MCParticle ;
  class ParticleID ;
} ;
// ============================================================================
namespace LoKi
{
  namespace MCParticles 
  {
    class    Identifier ;
    class AbsIdentifier ;    
  };
};
// ============================================================================


// ============================================================================
/** @file
 *
 *  This file is a part of LoKi project - 
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
 *  contributions and advices from G.Raven, J.van Tilburg, 
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  By usage of this code one clearly states the disagreement 
 *  with the campain of Dr.O.Callot at al.: 
 *  "No Vanya's lines are allowed in LHCb/Gaudi software."
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23 
 */
// ============================================================================


LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator==( const LoKi::MCParticles::Identifier& id   , 
            const LHCb::ParticleID&              pid  ) ;

LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator!=( const LoKi::MCParticles::Identifier& id   , 
            const LHCb::ParticleID&              pid  ) ;

LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator==( const LHCb::ParticleID&              pid  ,
            const LoKi::MCParticles::Identifier& id   ) ;

LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator!=( const LHCb::ParticleID&              pid  ,
            const LoKi::MCParticles::Identifier& id   ) ;

// ============================================================================

inline 
LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator==( const long                           pid  ,
            const LoKi::MCParticles::Identifier& id   ) 
{ return LHCb::ParticleID( pid ) == id ; }

inline
LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator!=( const long                           pid  ,
            const LoKi::MCParticles::Identifier& id   ) 
{ return LHCb::ParticleID( pid ) != id ; }

inline 
LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator==( const LoKi::MCParticles::Identifier& id   , 
            const long                           pid  ) 
{ return id == LHCb::ParticleID( pid ) ; }

inline 
LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator!=( const LoKi::MCParticles::Identifier& id   , 
            const long                           pid  ) 
{ return id != LHCb::ParticleID( pid ) ; }

// ============================================================================

LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator==( const LoKi::MCParticles::Identifier& id   , 
            const std::string&                   name ) ;
LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator!=( const LoKi::MCParticles::Identifier& id   , 
            const std::string&                   name ) ;
LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator==( const std::string&                   name , 
            const LoKi::MCParticles::Identifier& id   ) ;
LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator!=( const std::string&                   name , 
            const LoKi::MCParticles::Identifier& id   ) ;

// ============================================================================

LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator==( const LoKi::MCParticles::AbsIdentifier& id   , 
            const LHCb::ParticleID&                 pid  ) ;

LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator!=( const LoKi::MCParticles::AbsIdentifier& id   , 
            const LHCb::ParticleID&                 pid  ) ;

LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator==( const LHCb::ParticleID&                 pid  ,
            const LoKi::MCParticles::AbsIdentifier& id   ) ;

LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator!=( const LHCb::ParticleID&                 pid  ,
            const LoKi::MCParticles::AbsIdentifier& id   ) ;

// ============================================================================

inline 
LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator==( const long                              pid  ,
            const LoKi::MCParticles::AbsIdentifier& id   ) 
{ return LHCb::ParticleID( pid ) == id ; }

inline
LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator!=( const long                              pid  ,
            const LoKi::MCParticles::AbsIdentifier& id   ) 
{ return LHCb::ParticleID( pid ) != id ; }

inline 
LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator==( const LoKi::MCParticles::AbsIdentifier& id   , 
            const long                              pid  ) 
{ return id == LHCb::ParticleID( pid ) ; }

inline 
LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator!=( const LoKi::MCParticles::AbsIdentifier& id   , 
            const long                            pid  ) 
{ return id != LHCb::ParticleID( pid ) ; }

// ============================================================================

LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator==( const LoKi::MCParticles::AbsIdentifier& id   , 
            const std::string&                      name ) ;
LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator!=( const LoKi::MCParticles::AbsIdentifier& id   , 
            const std::string&                      name ) ;
LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator==( const std::string&                      name , 
            const LoKi::MCParticles::AbsIdentifier& id   ) ;
LoKi::PredicateFromPredicate<const LHCb::MCParticle*> 
operator!=( const std::string&                      name , 
            const LoKi::MCParticles::AbsIdentifier& id   ) ;


// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_MCPIDOPERATORS_H
// ============================================================================
