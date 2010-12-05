// $Id: $
// ============================================================================
// Include files 
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Vertices3.h"
#include "LoKi/Constants.h"
#include "LoKi/Algs.h"
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
 *  with the campain of Dr.O.Callot et al.: 
 *  ``No Vanya's lines are allowed in LHCb/Gaudi software.''
 *  
 *  @author Vanya BELYAEV ibelyaev@cern.ch
 *  @date   2010-12-05
 *   
 *                    $Revision$
 *  Last Modification $Date$
 *                 by $Author$
 */
// ============================================================================
// constructor from track cuts and "bad" value 
// ============================================================================
LoKi::Vertices::RecVertex2TrackMin::RecVertex2TrackMin
( const LoKi::Functor<const LHCb::Track*,double>& fun   , 
  const double                                    bad   ) 
  : LoKi::BasicFunctors<const LHCb::VertexBase*>::Function () 
  , m_fun   ( fun   ) 
  , m_bad   ( bad   )   
{}
// ============================================================================
// constructor from track cuts and "bad" value 
// ============================================================================
LoKi::Vertices::RecVertex2TrackMin::RecVertex2TrackMin
( const LoKi::Functor<const LHCb::Track*,double>& fun   )
  : LoKi::BasicFunctors<const LHCb::VertexBase*>::Function () 
  , m_fun   ( fun   ) 
  , m_bad   ( LoKi::Constants::PositiveInfinity ) 
{}
// ============================================================================
// MANDATORY: virtual destructor 
// ============================================================================
LoKi::Vertices::RecVertex2TrackMin::~RecVertex2TrackMin(){}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Vertices::RecVertex2TrackMin*
LoKi::Vertices::RecVertex2TrackMin::clone() const 
{ return new LoKi::Vertices::RecVertex2TrackMin(*this) ; }
// ============================================================================
// MANDATORY: the conly one essential method 
// ============================================================================
LoKi::Vertices::RecVertex2TrackMin::result_type 
LoKi::Vertices::RecVertex2TrackMin::operator()  
  ( LoKi::Vertices::RecVertex2TrackMin::argument v ) const 
{
  if ( 0 == v ) 
  {
    Error ("LHCb::VertexBase* points to NULL, return 'bad'");
    return m_bad ;
  }
  //
  const LHCb::VertexBase* vb = v ;
  const LHCb::RecVertex*  rv = dynamic_cast<const LHCb::RecVertex*> ( vb ) ;
  //
  if ( 0 == rv ) 
  {
    Error ("LHCb::VertexBase* is not LHCb::RecVertex*, return 'bad'");
    return m_bad ;
  }
  //
  typedef SmartRefVector<LHCb::Track> TRACKS ;
  const TRACKS& tracks = rv->tracks() ;
  //
  std::pair<TRACKS::const_iterator,double> r = 
    LoKi::Algs::extremum 
    ( tracks.begin ()     , 
      tracks.end   ()     , 
      m_fun               ,
      std::less<double>() ) ;
  //
  if ( tracks.end() == r.first ) {  return m_bad ; }
  //
  return r.second ;
}
// ============================================================================
// OPTIONAL: the specific printout 
// ============================================================================
std::ostream& LoKi::Vertices::RecVertex2TrackMin::fillStream
( std::ostream& s ) const
{ 
  s << "RV_TrMIN( " << m_fun ;
  if ( LoKi::Constants::PositiveInfinity != m_bad ) { s << "," << m_bad ; }
  return s << ")" ;
}  
// ============================================================================



// ============================================================================
// constructor from track cuts and "bad" value 
// ============================================================================
LoKi::Vertices::RecVertex2TrackMax::RecVertex2TrackMax
( const LoKi::Functor<const LHCb::Track*,double>& fun   , 
  const double                                    bad   )
  : LoKi::Vertices::RecVertex2TrackMin ( fun , bad ) 
{}
// ============================================================================
// constructor from track cuts and "bad" value 
// ============================================================================
LoKi::Vertices::RecVertex2TrackMax::RecVertex2TrackMax
( const LoKi::Functor<const LHCb::Track*,double>& fun   )
  : LoKi::Vertices::RecVertex2TrackMin ( fun , LoKi::Constants::NegativeInfinity ) 
{}
// ============================================================================
// MANDATORY: virtual destructor 
// ============================================================================
LoKi::Vertices::RecVertex2TrackMax::~RecVertex2TrackMax(){}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Vertices::RecVertex2TrackMax*
LoKi::Vertices::RecVertex2TrackMax::clone() const 
{ return new LoKi::Vertices::RecVertex2TrackMax(*this) ; }
// ============================================================================
// MANDATORY: the conly one essential method 
// ============================================================================
LoKi::Vertices::RecVertex2TrackMax::result_type 
LoKi::Vertices::RecVertex2TrackMax::operator()  
  ( LoKi::Vertices::RecVertex2TrackMax::argument v ) const 
{
  if ( 0 == v ) 
  {
    Error ("LHCb::VertexBase* points to NULL, return 'bad'");
    return m_bad ;
  }
  //
  const LHCb::VertexBase* vb = v ;
  const LHCb::RecVertex*  rv = dynamic_cast<const LHCb::RecVertex*> ( vb ) ;
  //
  if ( 0 == rv ) 
  {
    Error ("LHCb::VertexBase* is not LHCb::RecVertex*, return 'bad'");
    return m_bad ;
  }
  //
  typedef SmartRefVector<LHCb::Track> TRACKS ;
  const TRACKS& tracks = rv->tracks() ;
  //
  std::pair<TRACKS::const_iterator,double> r = 
    LoKi::Algs::extremum 
    ( tracks.begin ()        , 
      tracks.end   ()        , 
      m_fun                  ,
      std::greater<double>() ) ;
  //
  if ( tracks.end() == r.first ) {  return m_bad ; }
  //
  return r.second ;
}
// ============================================================================
// OPTIONAL: the specific printout 
// ============================================================================
std::ostream& LoKi::Vertices::RecVertex2TrackMax::fillStream
( std::ostream& s ) const
{ 
  s << "RV_TrMAX( " << m_fun ;
  if ( LoKi::Constants::NegativeInfinity != m_bad ) { s << "," << m_bad ; }
  return s << ")" ;
}  
// ============================================================================


// ============================================================================
// constructor from track cuts and "bad" value 
// ============================================================================
LoKi::Vertices::RecVertex2TrackSum::RecVertex2TrackSum
( const LoKi::Functor<const LHCb::Track*,double>& fun   , 
  const double                                    bad   )
  : LoKi::Vertices::RecVertex2TrackMax ( fun , bad ) 
{}
// ============================================================================
// MANDATORY: virtual destructor 
// ============================================================================
LoKi::Vertices::RecVertex2TrackSum::~RecVertex2TrackSum(){}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Vertices::RecVertex2TrackSum*
LoKi::Vertices::RecVertex2TrackSum::clone() const 
{ return new LoKi::Vertices::RecVertex2TrackSum(*this) ; }
// ============================================================================
// MANDATORY: the conly one essential method 
// ============================================================================
LoKi::Vertices::RecVertex2TrackSum::result_type 
LoKi::Vertices::RecVertex2TrackSum::operator()  
  ( LoKi::Vertices::RecVertex2TrackSum::argument v ) const 
{
  if ( 0 == v ) 
  {
    Error ("LHCb::VertexBase* points to NULL, return 'bad'");
    return m_bad ;
  }
  //
  const LHCb::VertexBase* vb = v ;
  const LHCb::RecVertex*  rv = dynamic_cast<const LHCb::RecVertex*> ( vb ) ;
  //
  if ( 0 == rv ) 
  {
    Error ("LHCb::VertexBase* is not LHCb::RecVertex*, return 'bad'");
    return m_bad ;
  }
  //
  typedef SmartRefVector<LHCb::Track> TRACKS ;
  const TRACKS& tracks = rv->tracks() ;
  //
  return 
    LoKi::Algs::accumulate 
    ( tracks.begin ()     , 
      tracks.end   ()     , 
      m_fun               ,
      0.0                 , 
      std::plus<double>() ) ;
}
// ============================================================================
// OPTIONAL: the specific printout 
// ============================================================================
std::ostream& LoKi::Vertices::RecVertex2TrackSum::fillStream
( std::ostream& s ) const
{ return s << "RV_TrSUM( " << m_fun << "," << m_bad << ")" ; }  
// ============================================================================


// ============================================================================
// constructor from track cuts, track index and "bad" value 
// ============================================================================
LoKi::Vertices::RecVertex2TrackFun::RecVertex2TrackFun
( const LoKi::Functor<const LHCb::Track*,double>& fun   , 
  const unsigned short                            index , 
  const double                                    bad   ) 
  : LoKi::Vertices::RecVertex2TrackSum ( fun , bad ) 
  , m_index ( index ) 
{}
// ============================================================================
// constructor from track cuts, track index and "bad" value 
// ============================================================================
LoKi::Vertices::RecVertex2TrackFun::RecVertex2TrackFun
( const LoKi::Functor<const LHCb::Track*,double>& fun   , 
  const unsigned short                            index )
  : LoKi::Vertices::RecVertex2TrackSum ( fun , LoKi::Constants::NegativeInfinity ) 
  , m_index ( index ) 
{}
// ============================================================================
// MANDATORY: virtual destructor 
// ============================================================================
LoKi::Vertices::RecVertex2TrackFun::~RecVertex2TrackFun(){}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Vertices::RecVertex2TrackFun*
LoKi::Vertices::RecVertex2TrackFun::clone() const 
{ return new LoKi::Vertices::RecVertex2TrackFun(*this) ; }
// ============================================================================
// MANDATORY: the conly one essential method 
// ============================================================================
LoKi::Vertices::RecVertex2TrackFun::result_type 
LoKi::Vertices::RecVertex2TrackFun::operator()  
  ( LoKi::Vertices::RecVertex2TrackFun::argument v ) const 
{
  if ( 0 == v ) 
  {
    Error ("LHCb::VertexBase* points to NULL, return 'bad'");
    return m_bad ;
  }
  //
  const LHCb::VertexBase* vb = v ;
  const LHCb::RecVertex*  rv = dynamic_cast<const LHCb::RecVertex*> ( vb ) ;
  //
  if ( 0 == rv ) 
  {
    Error ("LHCb::VertexBase* is not LHCb::RecVertex*, return 'bad'");
    return m_bad ;
  }
  //
  typedef SmartRefVector<LHCb::Track> TRACKS ;
  const TRACKS& tracks = rv->tracks() ;
  if ( tracks.size() <= m_index ) 
  {
    Error ("Invalid track index , return 'bad'");
    return m_bad ;
  }
  //
  // finally: use the function
  //
  return m_fun ( tracks [ m_index ] ) ;
}
// ============================================================================
// OPTIONAL: the specific printout 
// ============================================================================
std::ostream& LoKi::Vertices::RecVertex2TrackFun::fillStream
( std::ostream& s ) const
{
  s << "RV_TrFUN( " << m_fun << "," << m_index ;
  if ( LoKi::Constants::NegativeInfinity != m_bad ) { s << "," << m_bad ; }
  // 
  return s << ")" ; 
}
// ============================================================================



// ============================================================================
// constructor from track cuts
// ============================================================================
LoKi::Vertices::RecVertex2TrackHas::RecVertex2TrackHas
( const LoKi::Functor<const LHCb::Track*,bool>&   cut )
  : LoKi::BasicFunctors<const LHCb::VertexBase*>::Predicate () 
  , m_cut   ( cut   ) 
{}
// ============================================================================
// MANDATORY: virtual destructor 
// ============================================================================
LoKi::Vertices::RecVertex2TrackHas::~RecVertex2TrackHas (){}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Vertices::RecVertex2TrackHas*
LoKi::Vertices::RecVertex2TrackHas::clone() const 
{ return new LoKi::Vertices::RecVertex2TrackHas(*this) ; }
// ============================================================================
// MANDATORY: the conly one essential method 
// ============================================================================
LoKi::Vertices::RecVertex2TrackHas::result_type 
LoKi::Vertices::RecVertex2TrackHas::operator()  
  ( LoKi::Vertices::RecVertex2TrackHas::argument v ) const 
{
  if ( 0 == v ) 
  {
    Error ("LHCb::VertexBase* points to NULL, return 'false'");
    return false ;
  }
  //
  const LHCb::VertexBase* vb = v ;
  const LHCb::RecVertex*  rv = dynamic_cast<const LHCb::RecVertex*> ( vb ) ;
  //
  if ( 0 == rv ) 
  {
    Error ("LHCb::VertexBase* is not LHCb::RecVertex*, return 'false'");
    return false ;
  }
  //
  typedef SmartRefVector<LHCb::Track> TRACKS ;
  const TRACKS& tracks = rv->tracks() ;
  //
  return LoKi::Algs::found ( tracks.begin () , 
                             tracks.end   () , m_cut ) ;
}
// ============================================================================
// OPTIONAL: the specific printout 
// ============================================================================
std::ostream& LoKi::Vertices::RecVertex2TrackHas::fillStream
( std::ostream& s ) const
{ return s << "RV_TrHAS( " << m_cut << ")" ; }





// ============================================================================
// constructor from track cuts, track index and "bad" value 
// ============================================================================
LoKi::Vertices::RecVertex2TrackCut::RecVertex2TrackCut
( const LoKi::Functor<const LHCb::Track*,bool>&   cut   , 
  const unsigned short                            index )
  : LoKi::Vertices::RecVertex2TrackHas ( cut ) 
  , m_index ( index )
{}
// ============================================================================
// MANDATORY: virtual destructor 
// ============================================================================
LoKi::Vertices::RecVertex2TrackCut::~RecVertex2TrackCut(){}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Vertices::RecVertex2TrackCut*
LoKi::Vertices::RecVertex2TrackCut::clone() const 
{ return new LoKi::Vertices::RecVertex2TrackCut(*this) ; }
// ============================================================================
// MANDATORY: the conly one essential method 
// ============================================================================
LoKi::Vertices::RecVertex2TrackCut::result_type 
LoKi::Vertices::RecVertex2TrackCut::operator()  
  ( LoKi::Vertices::RecVertex2TrackCut::argument v ) const 
{
  if ( 0 == v ) 
  {
    Error ("LHCb::VertexBase* points to NULL, return 'false'");
    return false ;
  }
  //
  const LHCb::VertexBase* vb = v ;
  const LHCb::RecVertex*  rv = dynamic_cast<const LHCb::RecVertex*> ( vb ) ;
  //
  if ( 0 == rv ) 
  {
    Error ("LHCb::VertexBase* is not LHCb::RecVertex*, return 'false'");
    return false ;
  }
  //
  typedef SmartRefVector<LHCb::Track> TRACKS ;
  const TRACKS& tracks = rv->tracks() ;
  if ( tracks.size() <= m_index ) 
  {
    Error ("Invalid track index , return 'false'");
    return false ;
  }
  //
  // finally: use the function
  //
  return m_cut ( tracks [ m_index ] ) ;
}
// ============================================================================
// OPTIONAL: the specific printout 
// ============================================================================
std::ostream& LoKi::Vertices::RecVertex2TrackCut::fillStream
( std::ostream& s ) const
{ return s << "RV_TrCUT( " << m_cut << "," << m_index << ")" ; }
// ============================================================================




// ============================================================================
// constructor from track cuts and "bad" value 
// ============================================================================
LoKi::Vertices::RecVertex2TrackNum::RecVertex2TrackNum
( const LoKi::Functor<const LHCb::Track*,bool>& cut   , 
  const double                                  bad   ) 
  : LoKi::BasicFunctors<const LHCb::VertexBase*>::Function () 
  , m_cut   ( cut   ) 
  , m_bad   ( bad   )   
{}
// ============================================================================
// MANDATORY: virtual destructor 
// ============================================================================
LoKi::Vertices::RecVertex2TrackNum::~RecVertex2TrackNum(){}
// ============================================================================
// MANDATORY: clone method ("virtual constructor")
// ============================================================================
LoKi::Vertices::RecVertex2TrackNum*
LoKi::Vertices::RecVertex2TrackNum::clone() const 
{ return new LoKi::Vertices::RecVertex2TrackNum(*this) ; }
// ============================================================================
// MANDATORY: the conly one essential method 
// ============================================================================
LoKi::Vertices::RecVertex2TrackNum::result_type 
LoKi::Vertices::RecVertex2TrackNum::operator()  
  ( LoKi::Vertices::RecVertex2TrackNum::argument v ) const 
{
  if ( 0 == v ) 
  {
    Error ("LHCb::VertexBase* points to NULL, return 'bad'");
    return m_bad ;
  }
  //
  const LHCb::VertexBase* vb = v ;
  const LHCb::RecVertex*  rv = dynamic_cast<const LHCb::RecVertex*> ( vb ) ;
  //
  if ( 0 == rv ) 
  {
    Error ("LHCb::VertexBase* is not LHCb::RecVertex*, return 'bad'");
    return m_bad ;
  }
  //
  typedef SmartRefVector<LHCb::Track> TRACKS ;
  const TRACKS& tracks = rv->tracks() ;
  //
  return LoKi::Algs::count_if 
    ( tracks.begin ()     , 
      tracks.end   ()     , 
      m_cut               ) ;
}
// ============================================================================
// OPTIONAL: the specific printout 
// ============================================================================
std::ostream& LoKi::Vertices::RecVertex2TrackNum::fillStream
( std::ostream& s ) const
{ 
  s << "RV_TrNUM( " << m_cut ;
  if ( -1 != m_bad ) { s << "," << m_bad ; }
  return s << ")" ;
}  
// ============================================================================


// ============================================================================
// The END 
// ============================================================================
