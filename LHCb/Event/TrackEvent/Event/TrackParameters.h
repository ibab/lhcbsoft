#ifndef TRACKEVENT_TRACKPARAMETERS_H
#define TRACKEVENT_TRACKPARAMETERS_H 1

// Include files
#include "CLHEP/Units/PhysicalConstants.h"

/** @namespace TrackParameters
 *
 * This namespace defines some useful items 
 *
 * @author unknown
 * created Wed Feb  9 15:43:46 2005
 * 
 */


namespace TrackParameters
{
  /// enum
  enum TrackStatusCode{ PROBLEMS=3 };

  static const double hiTolerance     = 1e-40;          ///< High tolerance
  static const double lowTolerance    = 1e-20;          ///< Low tolerance
  static const double looseTolerance  = 1e-5;           ///< Loose tolerance
  static const double moliereFactor   = 13.6*MeV;       ///< Moliere factor

};

// -----------------------------------------------------------------------------
//   end of namespace
// -----------------------------------------------------------------------------

#endif /// TRACKEVENT_TRACKPARAMETERS_H
