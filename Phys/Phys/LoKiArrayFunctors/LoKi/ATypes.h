// $Id: ATypes.h,v 1.2 2007-11-28 14:55:54 ibelyaev Exp $
// ============================================================================
#ifndef LOKI_ATYPES_H 
#define LOKI_ATYPES_H 1
// ============================================================================
// Include files
// ============================================================================
// Event
// ============================================================================
#include "Event/Particle.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Functions.h"
#include "LoKi/PhysTypes.h"
// ============================================================================
namespace LoKi 
{
  namespace ATypes
  {
    /** The actual argument type for Exclusive HLT processing, 
     *  as requested by Gerhard RAVEN and Patrick KOPPENBURG
     */
    typedef LHCb::Particle::ConstVector                        Combination ;
    /// type of 'cuts' for the combination (interface)
    typedef LoKi::BasicFunctors<Combination>::Predicate              ACuts ;
    /// type of 'functions' for the combination (interface)
    typedef LoKi::BasicFunctors<Combination>::Function               AFunc ;
    /// type of 'cuts' for combination (assignable)
    typedef LoKi::BasicFunctors<Combination>::PredicateFromPredicate ACut  ;
    /// type of 'functions' for combination (assignable)
    typedef LoKi::BasicFunctors<Combination>::FunctionFromFunction   AFun  ;
  } // end of namespace LoKi::ATypes 
  namespace Types
  {
    /// type of 'cuts' for the combination (interface)
    typedef LoKi::ATypes::ACuts                                     ACuts ;
    /// type of 'functions' for the combination (interface)
    typedef LoKi::ATypes::AFunc                                     AFunc ;
    /// type of 'cuts' for combination (assignable)
    typedef LoKi::ATypes::ACut                                      ACut  ;
    /// type of 'functions' for combination (assignable)
    typedef LoKi::ATypes::AFun                                      AFun  ;
  } // end of namespace LoKi::Types 
} // end of namespace LoKi 
// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_ATYPES_H
// ============================================================================
