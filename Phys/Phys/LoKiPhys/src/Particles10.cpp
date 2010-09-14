// $Id$
// ============================================================================
// Include files 
// ============================================================================
// STD & STL 
// ============================================================================
#include <algorithm>
// ============================================================================
// Event 
// ============================================================================
#include "Event/RecVertex.h"
// ============================================================================
// LoKiCore 
// ============================================================================
#include "LoKi/Constants.h"
#include "LoKi/Print.h"
#include "LoKi/Keeper.h"
#include "LoKi/UniqueKeeper.h"
// ============================================================================
// LoKiPhys
// ============================================================================
#include "LoKi/Particles10.h"
#include "LoKi/PhysAlgs.h"
#include "LoKi/VertexCast.h"
// ============================================================================
/** @file
 *
 *  Implementation file for functions from namespace  LoKi::Particles
 *
 *  This file is a part of LoKi project - 
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
 *  contributions and advices from G.Raven, J.van Tilburg, 
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-02-22
 */
// ============================================================================
LoKi::Particles::HasTracksFromPV::HasTracksFromPV
( const LHCb::RecVertex* pv ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate  (   ) 
  , m_cut ( LHCb::Track::Vector() ) 
{ addVertex ( pv ) ; } 
// ============================================================================
LoKi::Particles::HasTracksFromPV::HasTracksFromPV
( const LHCb::RecVertex::Vector& pv ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate  (   ) 
  , m_cut ( LHCb::Track::Vector() ) 
{ addVertices ( pv.begin() , pv.end() ) ; } 
// ============================================================================
LoKi::Particles::HasTracksFromPV::HasTracksFromPV
( const LHCb::RecVertex::ConstVector& pv ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate  (   ) 
  , m_cut ( LHCb::Track::Vector() ) 
{ addVertices ( pv.begin() , pv.end() ) ; } 
// ============================================================================
LoKi::Particles::HasTracksFromPV::HasTracksFromPV
( const LHCb::RecVertices* pv ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate  (   ) 
  , m_cut ( LHCb::Track::Vector() ) 
{ if ( 0 != pv ) { addVertices ( pv->begin() , pv->end() ) ; } } 
// ============================================================================
LoKi::Particles::HasTracksFromPV::HasTracksFromPV
( const LoKi::Keeper<LHCb::RecVertex>& pv ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate  (   ) 
  , m_cut ( LHCb::Track::Vector() ) 
{ addVertices ( pv.begin() , pv.end() ) ; } 
// ============================================================================
LoKi::Particles::HasTracksFromPV::HasTracksFromPV
( const LoKi::UniqueKeeper<LHCb::RecVertex>& pv ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate  (   ) 
  , m_cut ( LHCb::Track::Vector() ) 
{ addVertices ( pv.begin() , pv.end() ) ; } 
// ============================================================================
LoKi::Particles::HasTracksFromPV::HasTracksFromPV
( const LHCb::VertexBase* pv ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate  (   ) 
  , m_cut ( LHCb::Track::Vector() ) 
{ addVertex ( pv ) ; } 
// ============================================================================
LoKi::Particles::HasTracksFromPV::HasTracksFromPV
( const LHCb::VertexBase::Vector& pv ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate  (   ) 
  , m_cut ( LHCb::Track::Vector() ) 
{ addVertices ( pv.begin() , pv.end() ) ; } 
// ============================================================================
LoKi::Particles::HasTracksFromPV::HasTracksFromPV
( const LHCb::VertexBase::ConstVector& pv ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate  (   ) 
  , m_cut ( LHCb::Track::Vector() ) 
{ addVertices ( pv.begin() , pv.end() ) ; } 
// ============================================================================
LoKi::Particles::HasTracksFromPV::HasTracksFromPV
( const LoKi::PhysTypes::VRange& pv ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate  (   ) 
  , m_cut ( LHCb::Track::Vector() ) 
{ addVertices ( pv.begin() , pv.end() ) ; } 
// ============================================================================
LoKi::Particles::HasTracksFromPV::HasTracksFromPV
( const LoKi::Keeper<LHCb::VertexBase>& pv ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate  (   ) 
  , m_cut ( LHCb::Track::Vector() ) 
{ addVertices ( pv.begin() , pv.end() ) ; } 
// ============================================================================
LoKi::Particles::HasTracksFromPV::HasTracksFromPV
( const LoKi::UniqueKeeper<LHCb::VertexBase>& pv ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate  (   ) 
  , m_cut ( LHCb::Track::Vector() ) 
{ addVertices ( pv.begin() , pv.end() ) ; } 
// ============================================================================
LoKi::Particles::HasTracksFromPV::HasTracksFromPV
( const LoKi::Particles::HasTracksFromPV& right ) 
  : LoKi::AuxFunBase                        ( right )
  , LoKi::BasicFunctors<const LHCb::Particle*>::Predicate  ( right ) 
  , m_cut ( right.m_cut )  
{}
// ============================================================================
size_t LoKi::Particles::HasTracksFromPV::addVertex
( const LHCb::RecVertex* pv ) 
{
  if ( 0 != pv ) 
  { m_cut.addObjects ( pv->tracks().begin() , pv->tracks().end() ) ; }
  return size() ;
} ;
// ============================================================================
size_t LoKi::Particles::HasTracksFromPV::addVertex
( const LHCb::VertexBase* pv ) 
{
  const LHCb::RecVertex* vertex = LoKi::Vertices::base2rec( pv ) ;
  if ( 0 == vertex || !pv->isPrimary() ) { return m_cut.size() ; }
  return addVertex( vertex ) ;
} 
// ============================================================================
LoKi::Particles::HasTracksFromPV::result_type 
LoKi::Particles::HasTracksFromPV::operator() 
  ( LoKi::Particles::HasTracksFromPV::argument p ) const 
{
  if ( 0 == p ) 
  {
    Error ( " Argument is invalid! return 'false' " ) ;
    return false ;                                              
  }
  if ( empty() ) 
  {
    Warning ( " Empty list of tracks is specified! return 'false' " ) ;
    return false ;
  }
  return m_cut ( p ) ;
} 
// ============================================================================
std::ostream& 
LoKi::Particles::HasTracksFromPV::fillStream
( std::ostream& stream ) const
{ return stream << "PVTRACK" ; }
// ============================================================================



// ============================================================================
LoKi::Particles::HasTracksInTreeFromPV::HasTracksInTreeFromPV 
( const LHCb::RecVertex*               pv  ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate() 
  , m_cut ( pv ) {}
// ============================================================================
LoKi::Particles::HasTracksInTreeFromPV::HasTracksInTreeFromPV 
( const LHCb::RecVertex::Vector& pv  ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate() 
  , m_cut ( pv ) {}
// ============================================================================
LoKi::Particles::HasTracksInTreeFromPV::HasTracksInTreeFromPV 
( const LHCb::RecVertex::ConstVector& pv  ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate() 
  , m_cut ( pv ) {}
// ============================================================================
LoKi::Particles::HasTracksInTreeFromPV::HasTracksInTreeFromPV 
( const LHCb::RecVertices* pv  ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate() 
  , m_cut ( pv ) {}
// ============================================================================
LoKi::Particles::HasTracksInTreeFromPV::HasTracksInTreeFromPV 
( const LoKi::Keeper<LHCb::RecVertex>& pv  ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate() 
  , m_cut ( pv ) {}
// ============================================================================
LoKi::Particles::HasTracksInTreeFromPV::HasTracksInTreeFromPV 
( const LoKi::UniqueKeeper<LHCb::RecVertex>& pv  ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate() 
  , m_cut ( pv ) {}
// ============================================================================
LoKi::Particles::HasTracksInTreeFromPV::HasTracksInTreeFromPV 
( const LHCb::VertexBase*               pv  ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate() 
  , m_cut ( pv ) {}
// ============================================================================
LoKi::Particles::HasTracksInTreeFromPV::HasTracksInTreeFromPV 
( const LHCb::VertexBase::Vector& pv  ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate() 
  , m_cut ( pv ) {}
// ============================================================================
LoKi::Particles::HasTracksInTreeFromPV::HasTracksInTreeFromPV 
( const LHCb::VertexBase::ConstVector& pv  ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate() 
  , m_cut ( pv ) {}
// ============================================================================
LoKi::Particles::HasTracksInTreeFromPV::HasTracksInTreeFromPV 
( const LoKi::Keeper<LHCb::VertexBase>& pv  ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate() 
  , m_cut ( pv ) {}
// ============================================================================
LoKi::Particles::HasTracksInTreeFromPV::HasTracksInTreeFromPV 
( const LoKi::PhysTypes::VRange& pv  ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate() 
  , m_cut ( pv ) {}
// ============================================================================
LoKi::Particles::HasTracksInTreeFromPV::HasTracksInTreeFromPV 
( const LoKi::UniqueKeeper<LHCb::VertexBase>& pv  ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate() 
  , m_cut ( pv ) {}
// ============================================================================
LoKi::Particles::HasTracksInTreeFromPV::HasTracksInTreeFromPV
( const LoKi::Particles::HasTracksInTreeFromPV& right ) 
  : LoKi::AuxFunBase                       ( right )
  , LoKi::BasicFunctors<const LHCb::Particle*>::Predicate ( right       ) 
  , m_cut                                  ( right.m_cut ) 
{}
// ============================================================================
LoKi::Particles::HasTracksInTreeFromPV::result_type 
LoKi::Particles::HasTracksInTreeFromPV::operator() 
  ( LoKi::Particles::HasTracksInTreeFromPV::argument p ) const
{
  if ( 0 == p ) 
  {
    Error ( " Argument is invalid! return 'false' " ) ;
    return false ;                                              
  }
  if ( empty() ) 
  {
    Warning ( " Empty list of tracks is specified! return 'false' " ) ;
    return false ;
  }
  // scan the decay tree 
  return LoKi::PhysAlgs::found ( p , m_cut ) ;
} 
// ============================================================================
std::ostream& 
LoKi::Particles::HasTracksInTreeFromPV::fillStream
( std::ostream& stream ) const
{ return stream << "PVTRACKINTREE" ; }
// ============================================================================



// ============================================================================
// The END 
// ============================================================================

