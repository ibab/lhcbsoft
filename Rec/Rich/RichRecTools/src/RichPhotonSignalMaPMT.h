// $Id: RichPhotonSignalMaPMT.h,v 1.1 2003-09-04 07:12:52 jonrob Exp $
#ifndef RICHRECTOOLS_RICHPHOTONSIGNALMAPMT_H
#define RICHRECTOOLS_RICHPHOTONSIGNALMAPMT_H 1

// from Gaudi
#include "GaudiKernel/ToolFactory.h"
#include "GaudiKernel/SmartDataPtr.h"

// base class
#include "RichRecBase/RichRecToolBase.h"

// Event
#include "Event/RichRecPhoton.h"

// Detector Description
#include "RichDet/DeRich1.h"
#include "RichDet/DeRich2.h"

// Interfaces
#include "RichRecBase/IRichPhotonSignal.h"
#include "RichRecBase/IRichExpectedTrackSignal.h"
#include "RichRecBase/IRichCherenkovAngle.h"
#include "RichRecBase/IRichCherenkovResolution.h"

// Rich Kernel
#include "RichKernel/MessageSvcStl.h"

// CLHEP
#include "CLHEP/Units/PhysicalConstants.h"

/** @class RichPhotonSignalMaPMT RichPhotonSignalMaPMT.h
 *
 *  Tool to calculate for a given photon the probabilities of it
 *  being a signal or scattered photon, and its predicted contribution
 *  to its associated RichRecPixel, under a certain mass hypothesis.
 *  This version is tuned for MaPMTs
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   15/03/2002
 */

class RichPhotonSignalMaPMT : public RichRecToolBase,
                              virtual public IRichPhotonSignal {

public:

  /// Standard constructor
  RichPhotonSignalMaPMT( const std::string& type,
                         const std::string& name,
                         const IInterface* parent );

  /// Destructor
  virtual ~RichPhotonSignalMaPMT(){}

  /// Initialize method
  StatusCode initialize();

  /// Finalize method
  StatusCode finalize();

  /// Predicted pixel signal for a given reconstructed photon under a given mass hypothesis
  double predictedPixelSignal( RichRecPhoton * photon,
                               const Rich::ParticleIDType id );
  /// Signal Probability for a given reconstructed photon under a given mass hypothesis
  double signalProb( RichRecPhoton * photon,
                     const Rich::ParticleIDType id );

  /// Scatter Probability for a given reconstructed photon under a given mass hypothesis
  double scatterProb( RichRecPhoton * photon,
                      const Rich::ParticleIDType id );

private: // private data

  /// Pointer to RichExpectedTrackSignal interface
  IRichExpectedTrackSignal * m_signal;

  /// Pointer to RichCherenkovAngle interface
  IRichCherenkovAngle * m_ckAngle;

  /// Pointer to RichCherenkovResolution interface
  IRichCherenkovResolution * m_ckRes;

  /// Temporary local value for Radii of curvature
  double m_radiusCurv[Rich::NRiches];

  /// Temporary local value pixel area
  double m_pixelArea;

};

#endif // RICHRECTOOLS_RICHPHOTONSIGNALMAPMT_H
