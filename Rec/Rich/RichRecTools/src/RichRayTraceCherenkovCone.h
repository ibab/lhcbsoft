
//-----------------------------------------------------------------------------
/** @file RichRayTraceCherenkovCone.h
 *
 *  Header file for tool : RichRayTraceCherenkovCone
 *
 *  CVS Log :-
 *  $Id: RichRayTraceCherenkovCone.h,v 1.6 2005-02-02 10:08:59 jonrob Exp $
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   15/03/2002
 */
//-----------------------------------------------------------------------------

#ifndef RICHRECBASE_RICHRAYTRACECHERENKOVCONE_H
#define RICHRECBASE_RICHRAYTRACECHERENKOVCONE_H 1

// from Gaudi
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/ToolFactory.h"

// base class
#include "RichRecBase/RichRecToolBase.h"

// interfaces
#include "RichRecBase/IRichRayTraceCherenkovCone.h"
#include "RichRecBase/IRichCherenkovAngle.h"
#include "RichKernel/IRichRayTracing.h"
#include "RichKernel/IRichSmartIDTool.h"

// Event
#include "Event/RichRecRing.h"
#include "Event/RichRecSegment.h"

//-----------------------------------------------------------------------------
/** @class RichRayTraceCherenkovCone RichRayTraceCherenkovCone.h
 *
 *  Tool to ray trace cherenkov photons in a cone around a given
 *  RichRecSegment direction, at the given angle or the angle correct 
 *  for a given mass hypothesis.
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   15/03/2002
 */
//-----------------------------------------------------------------------------

class RichRayTraceCherenkovCone : public RichRecToolBase,
                                  virtual public IRichRayTraceCherenkovCone {

public: // Methods for Gaudi Framework

  /// Standard constructor
  RichRayTraceCherenkovCone( const std::string& type,
                             const std::string& name,
                             const IInterface* parent );

  /// Destructor
  virtual ~RichRayTraceCherenkovCone(){}

  // Initialize method
  StatusCode initialize();

  // Finalize method
  StatusCode finalize();

public: // methods (and doxygen comments) inherited from public interface

  // Ray trace the Cherenkov cone for the given segment and mass hypothesis to the detector plane
  StatusCode rayTrace ( RichRecSegment * segment,
                        const Rich::ParticleIDType id,
                        std::vector<HepPoint3D> & points,
                        const RichTraceMode mode = RichTraceMode() ) const;

  // Ray trace the Cherenkov cone for the given segment and cherenkov angle to the detector plane
  StatusCode rayTrace ( RichRecSegment * segment,
                        const double ckTheta,
                        std::vector<HepPoint3D> & points,
                        const RichTraceMode mode = RichTraceMode() ) const;

  // Ray trace the Cherenkov cone using the given emission point, direction and Cherenkov angle
  StatusCode rayTrace ( const Rich::DetectorType rich,
                        const HepPoint3D & emissionPoint,
                        const HepVector3D & direction,
                        const double ckTheta,
                        std::vector<HepPoint3D> & points,
                        const RichTraceMode mode = RichTraceMode() ) const;

  // Ray trace the Cherenkov cone for the given ring to the detector plane, in Global coordinates
  const std::vector<HepPoint3D> & rayTrace ( RichRecRing * ring,
                                             const RichTraceMode mode = RichTraceMode() ) const;

  // Ray trace the Cherenkov cone for the given ring to the detector plane, in Local coordinates
  const std::vector<HepPoint3D> & rayTraceLocal ( RichRecRing * ring,
                                                  const RichTraceMode mode = RichTraceMode() ) const;


private: // data

  // Pointers to tool instances
  IRichRayTracing * m_rayTrace;
  IRichCherenkovAngle * m_ckAngle;
  IRichSmartIDTool * m_smartIDTool;

  // Number of photons to use in ray tracing
  int m_nRayTrace;

  // Cherenkov phi incrementation
  double m_incPhi;

};

#endif // RICHRECBASE_RICHRAYTRACECHERENKOVCONE_H
