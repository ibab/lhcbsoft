// $Id$
// ============================================================================
// Include files 
// ============================================================================
// STD & STL 
// ============================================================================
#include <algorithm>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/ToStream.h"
// ============================================================================
// Event 
// ============================================================================
#include "Event/ProtoParticle.h"
// ============================================================================
// LoKiCore 
// ============================================================================
#include "LoKi/Constants.h"
#include "LoKi/Info.h"
#include "LoKi/valid.h"
// ============================================================================
// LoKiPhys
// ===========================================================================
#include "LoKi/Particles12.h"
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
 *  @date 2006-02-23
 */
// ============================================================================
LoKi::Particles::ProtoHasInfo::ProtoHasInfo( const int info ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate() 
  , m_info ( info ) 
{}
// ============================================================================
LoKi::Particles::ProtoHasInfo::ProtoHasInfo
( const LoKi::Particles::ProtoHasInfo& right ) 
  : LoKi::AuxFunBase                      ( right )
  , LoKi::BasicFunctors<const LHCb::Particle*>::Predicate( right ) 
  , m_info ( right.m_info )
{}
// ============================================================================
LoKi::Particles::ProtoHasInfo::result_type 
LoKi::Particles::ProtoHasInfo::operator() 
  ( LoKi::Particles::ProtoHasInfo::argument p ) const 
{
  //
  if ( 0 == p ) 
  {
    Error ( " Invalid Particle, return 'false'" ) ;
    return false ;                                    // RETURN 
  }
  // 
  const LHCb::ProtoParticle* pp = p->proto() ;
  //
  return LoKi::ExtraInfo::hasInfo ( pp , m_info ) ;
}
// ============================================================================
std::ostream& 
LoKi::Particles::ProtoHasInfo::fillStream
( std::ostream& s ) const 
{ return s << "PPHASINFO(" << m_info << ")" ; }
// ============================================================================
LoKi::Particles::ProtoInfo::ProtoInfo
( const int    key , 
  const double def , 
  const double bad )
  : LoKi::BasicFunctors<const LHCb::Particle*>::Function() 
  , m_key ( key ) 
  , m_def ( def ) 
  , m_bad ( bad ) 
{}
// ============================================================================
LoKi::Particles::ProtoInfo::ProtoInfo
( const int    key , 
  const double def )
  : LoKi::BasicFunctors<const LHCb::Particle*>::Function() 
  , m_key ( key ) 
  , m_def ( def ) 
  , m_bad ( def ) 
{}
// ============================================================================
LoKi::Particles::ProtoInfo::ProtoInfo
( const LoKi::Particles::ProtoInfo& right ) 
  : LoKi::AuxFunBase                     ( right )
  , LoKi::BasicFunctors<const LHCb::Particle*>::Function( right ) 
  , m_key  ( right.m_key  )
  , m_def  ( right.m_def  )
  , m_bad  ( right.m_bad  )
{}
// ============================================================================
LoKi::Particles::ProtoInfo::result_type 
LoKi::Particles::ProtoInfo::operator() 
  ( LoKi::Particles::ProtoInfo::argument p ) const 
{
  //
  if ( 0 == p ) 
  {
    Error ( " Argument is invalid! return " + 
            Gaudi::Utils::toString ( m_bad ) ) ;
    return m_bad ;                                          // RETURN 
  }  
  //
  const LHCb::ProtoParticle* pp = p->proto() ;
  //
  if ( 0 == pp ) 
  {
    Error ( " ProtoParticle is invalid! return " + 
            Gaudi::Utils::toString ( m_bad ) ) ;
    return m_bad ;                                          // RETURN 
  }  
  //
  return LoKi::ExtraInfo::info ( pp , m_key , m_def ) ;     // RETURN 
}
// ============================================================================
std::ostream& 
LoKi::Particles::ProtoInfo::fillStream
( std::ostream& s ) const 
{ 
  s << "PPINFO(" << m_key << "," << m_def ;
  if ( m_bad != m_def ) { s << "," << m_bad ; }
  return s << ")" ;
}
// ============================================================================
//  MANDATORY: the only one essential method 
// ============================================================================
LoKi::Particles::ProtoHasRichPID::result_type
LoKi::Particles::ProtoHasRichPID::operator() 
  ( LoKi::Particles::ProtoHasRichPID::argument p ) const 
{
  //
  if ( 0 == p ) 
  {
    Error ( " Invalid Particle, return 'false'" ) ;
    return false ;                                    // RETURN 
  }
  // 
  const LHCb::ProtoParticle* pp = p->proto() ;
  //
  if ( 0 == pp ) 
  {
    Error ( " Invalid ProtoParticle, return 'false'" ) ;
    return false ;                                   // RETURN 
  }
  //
  return LoKi::valid ( pp->richPID() ) ;
} 
// ============================================================================
//  OPTIONAL: the specific printout 
// ============================================================================
std::ostream& 
LoKi::Particles::ProtoHasRichPID::fillStream ( std::ostream& s ) const 
{ return s << "HASRICH" ; }
// ============================================================================
//  MANDATORY: the only one essential method 
// ============================================================================
LoKi::Particles::ProtoHasMuonPID::result_type
LoKi::Particles::ProtoHasMuonPID::operator() 
  ( LoKi::Particles::ProtoHasMuonPID::argument p ) const 
{
  //
  if ( 0 == p ) 
  {
    Error ( " Invalid Particle, return 'false'" ) ;
    return false ;                                    // RETURN 
  }
  // 
  const LHCb::ProtoParticle* pp = p->proto() ;
  //
  if ( 0 == pp ) 
  {
    Error ( " Invalid ProtoParticle, return 'false'" ) ;
    return false ;                                   // RETURN 
  }
  //
  return LoKi::valid ( pp->muonPID() ) ;
} 
// ============================================================================
//  OPTIONAL: the specific printout 
// ============================================================================
std::ostream& 
LoKi::Particles::ProtoHasMuonPID::fillStream ( std::ostream& s ) const 
{ return s << "HASMUON" ; }
// ============================================================================
//  MANDATORY: the only one essential method 
// ============================================================================
LoKi::Particles::ProtoHasCaloHypos::result_type
LoKi::Particles::ProtoHasCaloHypos::operator() 
  ( LoKi::Particles::ProtoHasCaloHypos::argument p ) const 
{
  //
  if ( 0 == p ) 
  {
    Error ( " Invalid Particle, return 'false'" ) ;
    return false ;                                    // RETURN 
  }
  // 
  const LHCb::ProtoParticle* pp = p->proto() ;
  //
  if ( 0 == pp ) 
  {
    Error ( " Invalid ProtoParticle, return 'false'" ) ;
    return false ;                                   // RETURN 
  }
  //
  return !(pp->calo().empty()) ;                     // RETURN 
} 
// ============================================================================
//  OPTIONAL: the specific printout 
// ============================================================================
std::ostream& 
LoKi::Particles::ProtoHasCaloHypos::fillStream ( std::ostream& s ) const 
{ return s << "HASCALOS" ; }
// ============================================================================
LoKi::Particles::IsMuon::result_type 
LoKi::Particles::IsMuon::operator() 
  ( LoKi::Particles::IsMuon::argument p ) const
{
  //
  if ( 0 == p ) 
  {
    Error ( " Invalid Particle, return 'false'" ) ;
    return false ;                                    // RETURN 
  }
  //
  const LHCb::MuonPID* pid = muonPID ( p ) ;
  //
  if ( 0 == pid ) 
  {
    Error ( " Invalid MuonPID, return 'false'" ) ;
    return false ;                                   // RETURN 
  }
  //
  return pid -> IsMuon() ;                          // RETURN   
}
// ============================================================================
// "extract" muonPID form the particle 
// ============================================================================
const LHCb::MuonPID* LoKi::Particles::IsMuon::muonPID 
( const LHCb::Particle* p ) const 
{
  if ( 0 == p  )  { return 0 ; } ;                                // RETURN  
  //
  const LHCb::ProtoParticle* pp = p->proto() ;
  if ( 0 == pp ) 
  {
    Warning ("muonPID: Invalid ProtoParticle, return NULL") ;
    return 0 ;                                                    // RETURN 
  }
  //
  const LHCb::MuonPID* pid = pp->muonPID() ;
  if ( 0 != pid ) { return pid ; }                                // RETURN
  //  
  // Access the status word
  LHCb::ProtoParticle::ExtraInfo::const_iterator ifind = 
    pp->extraInfo().find ( LHCb::ProtoParticle::MuonPIDStatus );
  //
  if ( p->extraInfo().end () == ifind ) { return 0 ; }            // RETURN
  //
  m_pid.setStatus ( static_cast<unsigned int> ( ifind->second ) ) ;
  //
  return &m_pid ;  
}
// ============================================================================
//  OPTIONAL: the specific printout 
// ============================================================================
std::ostream& 
LoKi::Particles::IsMuon::fillStream ( std::ostream& s ) const 
{ return s << "ISMUON" ; }
// ============================================================================
LoKi::Particles::IsMuonLoose::result_type 
LoKi::Particles::IsMuonLoose::operator() 
  ( LoKi::Particles::IsMuonLoose::argument p ) const
{
  //
  if ( 0 == p ) 
  {
    Error ( " Invalid Particle, return 'false'" ) ;
    return false ;                                     // RETURN 
  }
  //
  const LHCb::MuonPID* pid = muonPID ( p ) ;
  //
  if ( 0 == pid ) 
  {
    Error ( " Invalid MuonPID, return 'false'" ) ;
    return false ;                                       // RETURN 
  }
  //
  return pid -> IsMuonLoose() ;                          // RETURN   
} 
// ============================================================================
//  OPTIONAL: the specific printout 
// ============================================================================
std::ostream& 
LoKi::Particles::IsMuonLoose::fillStream ( std::ostream& s ) const 
{ return s << "ISMUONLOOSE" ; }
// ============================================================================


// ============================================================================
// MANDATORY: virtual destructor 
// ============================================================================
LoKi::Particles::InMuonAcceptance::~InMuonAcceptance() {}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Particles::InMuonAcceptance*
LoKi::Particles::InMuonAcceptance::clone() const 
{ return new LoKi::Particles::InMuonAcceptance(*this); }
// ============================================================================
// MANDATORY: the only one essential method 
// ============================================================================
LoKi::Particles::InMuonAcceptance::result_type
LoKi::Particles::InMuonAcceptance::operator()
  ( LoKi::Particles::InMuonAcceptance::argument p ) const 
{
  //
  if ( 0 == p ) 
  {
    Error ( " Invalid Particle, return 'false'" ) ;
    return false ;                                     // RETURN 
  }
  //
  const LHCb::MuonPID* pid = muonPID ( p ) ;
  //
  if ( 0 == pid ) 
  {
    Error ( " Invalid MuonPID, return 'false'" ) ;
    return false ;                                       // RETURN 
  }
  //
  return pid -> InAcceptance () ;                        // RETURN   
}
// ============================================================================
//  OPTIONAL: the specific printout 
// ============================================================================
std::ostream& 
LoKi::Particles::InMuonAcceptance::fillStream ( std::ostream& s ) const 
{ return s << "INMUON" ; }
// ============================================================================


// ============================================================================
LoKi::Particles::TrackHasInfo::TrackHasInfo( const int info ) 
  : LoKi::BasicFunctors<const LHCb::Particle*>::Predicate() 
  , m_info ( info ) 
{}
// ============================================================================
LoKi::Particles::TrackHasInfo::result_type 
LoKi::Particles::TrackHasInfo::operator() 
  ( LoKi::Particles::TrackHasInfo::argument p ) const 
{
  //
  if ( 0 == p ) 
  {
    Error ( " Invalid Particle, return 'false'" ) ;
    return false ;                                    // RETURN 
  }
  // 
  const LHCb::ProtoParticle* pp = p->proto() ;
  if ( 0 == pp ) 
  {
    Error ( " Invalid ProtoParticle, return 'false'" ) ;
    return false ;                                    // RETURN 
  }
  //
  return LoKi::ExtraInfo::hasInfo ( pp->track() , m_info ) ;
}
// ============================================================================
std::ostream& 
LoKi::Particles::TrackHasInfo::fillStream
( std::ostream& s ) const 
{ return s << "THASINFO(" << m_info << ")" ; }
// ============================================================================


// ============================================================================
LoKi::Particles::TrackInfo::TrackInfo
( const int    key , 
  const double def , 
  const double bad )
  : LoKi::BasicFunctors<const LHCb::Particle*>::Function() 
  , m_key ( key ) 
  , m_def ( def ) 
  , m_bad ( bad ) 
{}
// ============================================================================
LoKi::Particles::TrackInfo::TrackInfo
( const int    key , 
  const double def )
  : LoKi::BasicFunctors<const LHCb::Particle*>::Function() 
  , m_key ( key ) 
  , m_def ( def ) 
  , m_bad ( def ) 
{}
// ============================================================================
LoKi::Particles::TrackInfo::result_type 
LoKi::Particles::TrackInfo::operator() 
  ( LoKi::Particles::TrackInfo::argument p ) const 
{
  //
  if ( 0 == p ) 
  {
    Error ( " Argument is invalid! return " + 
            Gaudi::Utils::toString ( m_bad ) ) ;
    return m_bad ;                                          // RETURN 
  }  
  //
  const LHCb::ProtoParticle* pp = p->proto() ;
  if ( 0 == pp ) 
  {
    Error ( " ProtoParticle is invalid! return " + 
            Gaudi::Utils::toString ( m_bad ) ) ;
    return m_bad ;                                          // RETURN 
  }
  //
  const LHCb::Track* track = pp->track() ;
  if ( 0 == track ) 
  {
    Error ( " Track is invalid! return " + 
            Gaudi::Utils::toString ( m_bad ) ) ;
    return m_bad ;                                          // RETURN 
  }  
  //
  return LoKi::ExtraInfo::info ( track , m_key , m_def ) ;     // RETURN 
}
// ============================================================================
std::ostream& LoKi::Particles::TrackInfo::fillStream ( std::ostream& s ) const 
{ 
  s << "TINFO(" << m_key << "," << m_def ;
  if ( m_bad != m_def ) { s << "," << m_bad ; }
  return s << ")" ;
}


// ============================================================================
// The END 
// ============================================================================

