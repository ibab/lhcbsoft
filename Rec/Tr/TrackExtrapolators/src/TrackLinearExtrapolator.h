#ifndef TRACKEXTRAPOLATORS_TRLINEAREXTRAPOLATOR_H 
#define TRACKEXTRAPOLATORS_TRLINEAREXTRAPOLATOR_H 1

// Include files
// -------------
// from TrackExtrapolators
#include "TrackExtrapolator.h"

/** @class TrackLinearExtrapolator TrackLinearExtrapolator.h TrackExtrapolators/TrackLinearExtrapolator.h
 *
 *  A TrackLinearExtrapolator is a ITrackExtrapolator which does a 'linear'
 *  (i.e. straight line) extrapolation of a State. It doesn't take into
 *  account MS.
 *
 *  @author Edwin Bos (added extrapolation method)
 *  @date   05/07/2005
 *
 *  @author Eduardo Rodrigues (changes and new features for new track event model)
 *  @date   25/11/2004
 *
 *  @author Rutger van der Eijk
 *  @date   07-04-1999
 */

class TrackLinearExtrapolator: public TrackExtrapolator {

public:
  /// constructor
  TrackLinearExtrapolator( const std::string& type, 
			   const std::string& name, 
			   const IInterface* parent );

  /// destructor
  virtual ~TrackLinearExtrapolator();

  /// Propagate a state to a given z-position
  StatusCode propagate( LHCb::State& state,
                        double z,
                        LHCb::ParticleID pid = LHCb::ParticleID(211) );
  
  /// Propagate a state to the closest position to the specified point
  StatusCode propagate( LHCb::State& state,
                        const Gaudi::XYZPoint& point,
                        LHCb::ParticleID pid = LHCb::ParticleID(211) );

};

#endif // TRACKEXTRAPOLATORS_TRLINEAREXTRAPOLATOR_H
