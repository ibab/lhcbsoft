// $Id: RichPhotonPredictorUsingMCRichOpticalPhotons.h,v 1.1 2004-06-10 14:40:50 jonesc Exp $
#ifndef RICHRECMCTOOLS_RICHPHOTONPREDICTORUSINGMCRICHOPTICALPHOTOS_H
#define RICHRECMCTOOLS_RICHPHOTONPREDICTORUSINGMCRICHOPTICALPHOTOS_H 1

// from Gaudi
#include "GaudiKernel/ToolFactory.h"

// base class
#include "RichRecBase/RichRecToolBase.h"

// Event model
#include "Event/RichRecPixel.h"
#include "Event/RichRecSegment.h"
#include "Event/MCRichOpticalPhoton.h"

// Interfaces
#include "RichRecBase/IRichPhotonPredictor.h"
#include "RichRecBase/IRichRecMCTruthTool.h"

/** @class RichPhotonPredictorUsingMCRichOpticalPhotons RichPhotonPredictorUsingMCRichOpticalPhotons.h
 *
 *  Tool which performs the association between RichRecTracks and
 *  RichRecPixels to form RichRecPhotons
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   15/03/2002
 */

class RichPhotonPredictorUsingMCRichOpticalPhotons : public RichRecToolBase,
                                                     virtual public IRichPhotonPredictor {

public:

  /// Standard constructor
  RichPhotonPredictorUsingMCRichOpticalPhotons( const std::string& type,
                                                const std::string& name,
                                                const IInterface* parent );

  /// Destructor
  virtual ~RichPhotonPredictorUsingMCRichOpticalPhotons(){}

  /// Initialize method
  StatusCode initialize();

  /// Finalize method
  StatusCode finalize();

  /// Is it possible to make a photon candidate using this segment and pixel.
  bool photonPossible( RichRecSegment * segment,
                       RichRecPixel * pixel ) const;

private: // private data

  // Pointers to tool instances
  IRichRecMCTruthTool * m_mcRecTool;  ///< MC Truth tool

};

#endif // RICHRECMCTOOLS_RICHPHOTONPREDICTORUSINGMCRICHOPTICALPHOTOS_H
