#ifndef TRACKPARABOLICEXTRAPOLATOR_H
#define TRACKPARABOLICEXTRAPOLATOR_H 1

// Include files
#include "TrackExtrapolator.h"

// Forward declaration
class IMagneticFieldSvc;


/** @class TrackParabolicExtrapolator TrackParabolicExtrapolator.h \
 *         "TrackParabolicExtrapolator.h"
 *
 *  A TrackParabolicExtrapolator is a ITrackExtrapolator that does a transport 
 *  using a parabolic expansion of the trajectory. It doesn't take into
 *  account Multiple Scattering.
 *
 *  @author Edwin Bos (added extrapolation methods)
 *  @date   05/07/2005
 *  @author Jose A. Hernando (13-03-2005)
 *  @author Matt Needham
 *  @date   22-04-2000
 */

class TrackParabolicExtrapolator: public TrackExtrapolator 
{

public:
  /// Constructor
  TrackParabolicExtrapolator( const std::string& type, 
                              const std::string& name, 
                              const IInterface* parent);
  
  /// Destructor
  virtual ~TrackParabolicExtrapolator();
  
  /// initialize
  virtual StatusCode initialize();
  
  /// Propagate a state to a given z-position
  virtual StatusCode propagate( LHCb::State& state,
                                double z,
                                LHCb::ParticleID pid = LHCb::ParticleID(211) );

  
  /// Propagate a state to the closest position to the specified point
  StatusCode propagate( LHCb::State& state,
                        const Gaudi::XYZPoint& point,
                        LHCb::ParticleID pid = LHCb::ParticleID(211) );

protected:

  /// update transport matrix
  virtual void updateTransportMatrix( const double dz,
                                      LHCb::State& State );

protected:

  double             m_ax;
  double             m_ay;
  Gaudi::XYZVector   m_B;
  IMagneticFieldSvc* m_pIMF; ///< Pointer to the magnetic field service

};

#endif // TRACKPARABOLICEXTRAPOLATOR_H
