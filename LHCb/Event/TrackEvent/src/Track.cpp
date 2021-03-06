// $: Track.cpp,v 1.39 2007/05/15 06:57:34 wouter Exp $ 
// Include files

#include <functional>
#include <string>
#include <map>
#include <array>
#include <algorithm>

// from gsl
#include "gsl/gsl_cdf.h"

// local
#include "Event/Track.h"
#include "Event/TrackFunctor.h"
#include "Event/TrackFitResult.h"
#include "Event/Node.h"

using namespace Gaudi;
using namespace LHCb;

// ============================================================================

//-----------------------------------------------------------------------------
// Implementation file for class : Track
//
// 2004-12-14 : Jose Hernando, Eduardo Rodrigues
//-----------------------------------------------------------------------------

//=============================================================================
// Retrieve a pointer to the fit result
//=============================================================================
const TrackFitResult* Track::fitResult() const
{
  //if( m_fitResult == 0 ) const_cast<Track*>(this)->m_fitResult = new TrackFitResult() ;
  return m_fitResult ;
}

//=============================================================================
// Retrieve a pointer to the fit result
//=============================================================================
TrackFitResult* Track::fitResult()
{
  //if( m_fitResult == 0 ) m_fitResult = new TrackFitResult() ;
  return m_fitResult ;
}

//=============================================================================
// Set the fit result. This takes ownership.
//=============================================================================
void Track::setFitResult(LHCb::TrackFitResult* absfit)
{
  if ( m_fitResult != absfit )
  {
    delete m_fitResult ;
    m_fitResult = absfit ;
  }
}

//=============================================================================
// Get a range of nodes in this track
//=============================================================================
Track::ConstNodeRange Track::nodes() const
{
  if ( !m_fitResult ) { return Track::ConstNodeRange() ; }
  //
  const LHCb::TrackFitResult* _result = m_fitResult ;
  // cast the const container to a container of const pointers
  const LHCb::TrackFitResult::NodeContainer& nodes_ = _result->nodes() ;
  //
  typedef LHCb::TrackFitResult::NodeContainer::const_iterator Iterator1 ;
  typedef Track::ConstNodeRange::const_iterator               Iterator2 ;
  //
  const Iterator1 begin = nodes_ . begin () ;
  const Iterator1 end   = nodes_ . end   () ;
  //
  // Helper union to avoid reinterpret_cast
  union _IteratorCast
  {
    //
    //
    const Iterator1* input  ;
    const Iterator2* output ;
    //
    static_assert( sizeof(Iterator1) == sizeof(Iterator2), "iterator sizes must be equal" ) ;
  } ;
  // somehow, volatile didn't work here in gcc46
  static _IteratorCast _begin ;
  static _IteratorCast _end   ;
  //
  _begin . input = &begin ;
  _end   . input = &end   ;
  //
  return Track::ConstNodeRange ( *_begin.output , *_end.output ) ;
}

//=============================================================================
// Get the measurements on the track. Node that it does not return a
// reference. This is because I want to remove this container from
// fitresult.
//=============================================================================
Track::MeasurementContainer Track::measurements() const
{
  MeasurementContainer meas ;
  if( m_fitResult )
    meas.insert(meas.end(),m_fitResult->measurements().begin(),m_fitResult->measurements().end()) ;
  return meas ;
}

namespace {
    
  template <size_t N>
  std::array<double,N+1>  generate_chi2max(double limit) {
    std::array<double,N+1> c = { 0 };
    size_t i = 1;
    std::generate( std::next(std::begin(c)), std::end(c), 
                   [=]() mutable { return gsl_cdf_chisq_Qinv(limit, i++ ); } );
    return c;
  }

  // could put this into probChi2, but then the table is generated at
  // first use of probChi2, i.e. during the event loop. 
  // This way, it tends to be generated when libTrackEvent.so is 
  // dynamically linked into the executable, i.e. very early.
  static const auto chi2max = generate_chi2max<256>(1e-15);
}

//=============================================================================
// Retrieve the probability of chi^2
//=============================================================================
double Track::probChi2() const
{
  double val(0) ;
  if ( nDoF() > 0 )
  {
    // what to do if nDoF is bigger than the lookup table? 
    // let's just do a range-checked acess, and have it throw 
    // an exception... maybe not the most elegant solution...
    // alternative: chi2max[  std::min( nDoF(), int(chi2max.size() ) ]
    // in which case for unreasonable nDoF we don't go until 1e-15...
    val = ( chi2() < chi2max.at( nDoF() ) ? gsl_cdf_chisq_Q(chi2(),nDoF()) : 0 );
  }
  return val ;
}

//=============================================================================
// Retrieve the reference to the state closest to the given z-position
//=============================================================================
State & Track::closestState( double z )
{
  auto iter = std::min_element( m_states.begin(),m_states.end(),
                                TrackFunctor::distanceAlongZ<State>(z) );
  if ( iter == m_states.end() )
    throw GaudiException( "No state closest to z","Track.cpp",
                          StatusCode::FAILURE );
  return *(*iter);
}

//=============================================================================
// Retrieve the (const) reference to the state closest to the given z-position
//=============================================================================
const State & Track::closestState( double z ) const
{
  if ( m_fitResult && !m_fitResult->nodes().empty() ) {
    auto iter = std::min_element( m_fitResult->nodes().begin(),m_fitResult->nodes().end(),
                                  TrackFunctor::distanceAlongZ<Node>(z) );
    if ( iter == m_fitResult->nodes().end() )
      throw GaudiException( "No state closest to z","Track.cpp",
                            StatusCode::FAILURE );
    return (*iter)->state();
  } else {
    auto iter = std::min_element( m_states.begin(),m_states.end(),
                                  TrackFunctor::distanceAlongZ<State>(z) );
    if ( iter == m_states.end() )
      throw GaudiException( "No state closest to z","Track.cpp",
                            StatusCode::FAILURE );
    return *(*iter);
  }
}

//=============================================================================
// Retrieve the (const) reference to the state closest to the given plane
//=============================================================================
const State & Track::closestState( const Gaudi::Plane3D& plane ) const
{
  if ( m_fitResult && !m_fitResult->nodes().empty() ) {
    auto iter = std::min_element( m_fitResult->nodes().begin(),m_fitResult->nodes().end(),
                                  TrackFunctor::distanceToPlane<Node>(plane) );
    if ( iter == m_fitResult->nodes().end() )
      throw GaudiException( "No state closest to z","Track.cpp",
                            StatusCode::FAILURE );
    return (*iter)->state();
  } else {
    auto iter = std::min_element( m_states.begin(),m_states.end(),
                                  TrackFunctor::distanceToPlane<State>(plane) );
    if ( iter == m_states.end() )
      throw GaudiException( "No state closest to plane","Track.cpp",
                            StatusCode::FAILURE );
    return *(*iter);
  }
}

//=============================================================================
// check the existence of a state at a certain predefined location
//=============================================================================
bool Track::hasStateAt( const LHCb::State::Location& location ) const
{
  return stateAt(location) != nullptr;
}

//=============================================================================
// Retrieve the pointer to the state closest to the given plane
//=============================================================================
State* Track::stateAt( const LHCb::State::Location& location )
{
  auto iter = std::find_if( m_states.begin(),m_states.end(),
                            TrackFunctor::HasKey<State,const LHCb::State::Location&>
                            (&State::checkLocation,location) );
  return iter != m_states.end() ? *iter : nullptr;
}

//=============================================================================
// Retrieve the (const) pointer to the state at a given location
//=============================================================================
const State* Track::stateAt( const LHCb::State::Location& location ) const
{
  auto iter = std::find_if( m_states.begin(),m_states.end(),
                            TrackFunctor::HasKey<State,const LHCb::State::Location&>
                            (&State::checkLocation,location) );
  return iter != m_states.end() ? *iter : nullptr;
}

//=============================================================================
// Add a State to the list of States associated to the Track
//=============================================================================
void Track::addToStates( const State& state )
{
  State* local = state.clone();
  const int order = ( checkFlag(Track::Backward) ? -1 : 1 );
  auto ipos =
    std::upper_bound(m_states.begin(),
                     m_states.end(),
                     local,
                     TrackFunctor::orderByZ<State>(order));
  m_states.insert(ipos,local);
}

//=============================================================================
// Add a list of states to the list associated to the Track. This takes ownership.
//=============================================================================
void Track::addToStates( StateContainer& states )
{
  // Make sure that the incoming states are properly sorted. The 'if' is ugly, but more efficient than using 'orderByZ'.
  bool backward = checkFlag(Track::Backward) ;
  if(backward) std::sort(states.begin(),states.end(),TrackFunctor::decreasingByZ<State>());
  else         std::sort(states.begin(),states.end(),TrackFunctor::increasingByZ<State>());
  // Now append and use std::inplace_merge. Make sure there is enough capacity such that iterators stay valid.
  m_states.reserve( states.size() + m_states.size()) ;
  auto middle = m_states.end() ;
  m_states.insert(middle, states.begin(), states.end()) ;
  if(backward) std::inplace_merge(m_states.begin(),middle,m_states.end(),TrackFunctor::decreasingByZ<State>());
  else         std::inplace_merge(m_states.begin(),middle,m_states.end(),TrackFunctor::increasingByZ<State>());
}

//=============================================================================
// Remove an LHCbID from the list of LHCbIDs associated to the Track
//=============================================================================
void Track::removeFromLhcbIDs( const LHCbID& value )
{
  auto pos = std::lower_bound( m_lhcbIDs.begin(), m_lhcbIDs.end(), value ) ;
  if (pos!=m_lhcbIDs.end() && *pos==value) m_lhcbIDs.erase( pos ) ;
}

//=============================================================================
// Remove a State from the list of States associated to the Track
//=============================================================================
void Track::removeFromStates( State* state )
{
  TrackFunctor::deleteFromList<State>(m_states,state);
}

//=============================================================================
// Add LHCbIDs to track
//=============================================================================
bool Track::addToLhcbIDs( const LHCb::LHCbID& value )
{
  auto pos = std::lower_bound( m_lhcbIDs.begin(),m_lhcbIDs.end(),value ) ;
  const bool rc = (pos == m_lhcbIDs.end()) || !( (*pos) == value ) ;
  if ( rc ) m_lhcbIDs.insert( pos, value ) ;
  return rc ;
}

//=============================================================================
// Add LHCbIDs to track
//=============================================================================
bool Track::addSortedToLhcbIDs( const LHCbIDContainer& ids )
{
  LHCbIDContainer result; result.reserve( ids.size() + m_lhcbIDs.size() ) ;
  std::set_union( ids.begin(), ids.end(),
                  m_lhcbIDs.begin(), m_lhcbIDs.end(),
                  std::back_inserter(result) );
  auto rc = ( result.size() == ids.size()+m_lhcbIDs.size() );
  m_lhcbIDs = std::move(result);
  return rc ;
}

//=============================================================================
// Compute the number of LHCbIDs that two tracks have in common
//=============================================================================
size_t Track::nCommonLhcbIDs(const Track& rhs) const
{
  // adapted from std::set_intersection
  size_t rc(0) ;
  auto first1 = m_lhcbIDs.begin() ;
  auto last1  = m_lhcbIDs.end() ;
  auto first2 = rhs.m_lhcbIDs.begin() ;
  auto last2  = rhs.m_lhcbIDs.end() ;
  while (first1 != last1 && first2 != last2) {
    if ( *first1 < *first2 ) {
      ++first1;
    } else if ( *first2 < *first1 ) {
      ++first2;
    } else {
      ++first1;
      ++first2;
      ++rc ;
    }
  }
  return rc ;
}

//=============================================================================
// Check whether the given LHCbID is on the Track
//=============================================================================
bool Track::isOnTrack( const LHCb::LHCbID& value ) const
{
  auto pos = std::lower_bound( m_lhcbIDs.begin(), m_lhcbIDs.end(), value ) ;
  return pos != m_lhcbIDs.end() && *pos == value ;
}

//=============================================================================
// Return the Measurement on the Track corresponding to the input LHCbID
//=============================================================================
const Measurement* Track::measurement( const LHCbID& value ) const
{
  return m_fitResult ? m_fitResult->measurement(value) : nullptr;
}

//=============================================================================
// reset the track
//=============================================================================
void Track::reset()
{
  setChi2PerDoF ( 0 );
  setNDoF       ( 0 );
  setFlags      ( 0 );
  setGhostProbability ( 999 );
  setLikelihood ( 999 );

  m_lhcbIDs.clear();
  std::for_each( m_states.begin(), m_states.end(), TrackFunctor::deleteObject() );
  m_states.clear();
  m_ancestors.clear();
  m_extraInfo.clear();
  delete m_fitResult ;
  m_fitResult = nullptr;
}

//=============================================================================
// Clone the track keeping the key
//=============================================================================
Track* Track::cloneWithKey( ) const
{
  Track* tr = new Track( this -> key() );
  tr -> copy( *this );
  return tr;
}

//=============================================================================
// Clone the track
//=============================================================================
Track* Track::clone() const
{
  Track* tr = new Track();
  tr -> copy( *this );
  return tr;
}

//=============================================================================
// Copy the info from the argument track into this track
//=============================================================================
void Track::copy( const Track& track )
{
  reset();

  setChi2PerDoF( track.chi2PerDoF() );
  setNDoF( track.nDoF() );
  setFlags( track.flags() );
  setLhcbIDs( track.lhcbIDs() );
  setExtraInfo( track.extraInfo() );
  setGhostProbability( track.ghostProbability() );
  setLikelihood( track.likelihood() );

  // copy the states
  m_states.reserve( track.states().size() ) ;
  std::transform( track.states().begin(), track.states().end(),
                  std::back_inserter(m_states),
                  [](const LHCb::State* s) { return s->clone(); });

  // copy the track fit info
  if ( track.m_fitResult ) { m_fitResult = track.m_fitResult->clone(); }

  // copy the ancestors
  const SmartRefVector<Track>& ancestors = track.ancestors();
  for ( const auto a : ancestors ) {
    addToAncestors(*a);
  }

}

//=============================================================================
// Clear the state vector
//=============================================================================
void Track::clearStates()
{
  std::for_each( m_states.begin(),
                 m_states.end(),TrackFunctor::deleteObject() );
  m_states.clear();
}

//=============================================================================
/** Check the presence of the information associated with
 *  a given key
 *
 *  @code
 *
 *  const Track* p = ... ;
 *
 *  Track::Key key = ... ;
 *  bool hasKey = p->hasInfo( key ) ;
 *
 *  @endcode
 *  @param    key key to be checked
 *  @return  'true' if there is informaiton with the 'key',
 *           'false' otherwise
 */
//=============================================================================
bool LHCb::Track::hasInfo ( const int key ) const
{
  return m_extraInfo.end() != m_extraInfo.find(key);
}

//=============================================================================
/** Add new information, associated with the specified key.
 *  This method cannot be used to modify information for an already existing key
 *
 *  @code
 *
 *  Track* p = ... ;
 *
 *  Track::Key  key   = ... ;
 *  Track::Info info  = ... ;
 *
 *  bool inserted = p->addInfo( key , info ) ;
 *
 *  @endcode
 *
 *  @param key key for the information
 *  @param info information to be associated with the key
 *  @return 'true' if information is inserted,
 *         'false' if information was not inserted, due to already existing key
 */
//=============================================================================
bool  LHCb::Track::addInfo ( const int key, const double info )
{ 
  return m_extraInfo.insert( key , info ).second;
}

//=============================================================================
/** extract the information associated with the given key
 *  If there is no such infomration the default value will
 *  be returned
 *
 *  @code
 *
 *  const Track* p = ... ;
 *
 *  Track::Key  key   = ... ;
 *
 *  // extract the information
 *  Track::Info info = p->info( key, -999 ) ;
 *
 *  @endcode
 *
 *  @param key key for the information
 *  @param def the default value to be returned
 *         in the case of missing info
 *  @return information associated with the key if there
 *          is such information, the default value otherwise
 */
//=============================================================================
double LHCb::Track::info( const int key, const double def ) const
{
  auto i = m_extraInfo.find( key ) ;
  return m_extraInfo.end() == i ? def : i->second;
}

//=============================================================================
// Count the number of outliers
//=============================================================================

unsigned int LHCb::Track::nMeasurementsRemoved() const
{
  return m_fitResult ? m_fitResult->nOutliers() : 0;
}

//=============================================================================
// Count the number of outliers
//=============================================================================

unsigned int LHCb::Track::nMeasurements() const
{
  return m_fitResult ? m_fitResult->nActiveMeasurements() : 0;
}

//=============================================================================
/** erase the information associated with the given key
 *
 *  @code
 *
 *  Track* p = ... ;
 *
 *  Track::Key  key   = ... ;
 *
 *  int erased = p->eraseInfo( key ) ;
 *
 *  @endcode
 *
 *  @param key key for the information
 *  @return return number of erased elements
 */
//=============================================================================
LHCb::Track::ExtraInfo::size_type
LHCb::Track::eraseInfo( const int key )
{
  return m_extraInfo.erase( key ) ;
}

//=============================================================================
// Fill stream
//=============================================================================
std::ostream& LHCb::Track::fillStream(std::ostream& os) const
{
  os << "*** Track ***" << std::endl
     << " key        : " << key() << std::endl
     << " type       : " << type() << std::endl
     << " history    : " << history() << std::endl
     << " fitstatus  : " << fitStatus() << std::endl
     << " # ids      : " << nLHCbIDs() << std::endl
     << " # meas     : " << nMeasurements() << std::endl
     << " chi2PerDoF : " << (float)m_chi2PerDoF << std::endl
     << " nDoF       : " << m_nDoF << std::endl
     << " GhostProb  : " << ghostProbability() << std::endl
     << " Likelihood : " << likelihood() << std::endl;

  os << " extraInfo : [";
  for ( const auto& i : extraInfo() ) {
    const LHCb::Track::AdditionalInfo info =
      static_cast<LHCb::Track::AdditionalInfo>(i.first);
    os << " " << info << "=" << i.second << " ";
  }
  os << "]" << std::endl;

  if ( !m_states.empty() ) {
    os << " p  : " << (float) firstState().p() <<std::endl
       << " pt : " << (float) firstState().pt() <<std::endl
       << " " << nStates() << " states at z =";
    for ( const auto& s : states() ) {
      if (s) os << " " << s->z();
    }
    os << "  :-" << std::endl;
    for ( const auto& s : states() ) {
      os << " " << *s;
    }
    os << std::endl;
  } else {
    os << " # states : " << nStates() << std::endl;
  }

  return os;
}
