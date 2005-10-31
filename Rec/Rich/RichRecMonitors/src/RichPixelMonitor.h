
//-----------------------------------------------------------------------------
/** @file RichPixelMonitor.h
 *
 *  Header file for algorithm class : RichPixelMonitor
 *
 *  CVS Log :-
 *  $Id: RichPixelMonitor.h,v 1.3 2005-10-31 13:30:58 jonrob Exp $
 *
 *  @author Chris Jones       Christopher.Rob.Jones@cern.ch
 *  @date   05/04/2002
 */
//-----------------------------------------------------------------------------

#ifndef RICHRECMONITOR_RICHPIXELMONITOR_H
#define RICHRECMONITOR_RICHPIXELMONITOR_H 1

// base class
#include "RichRecBase/RichRecHistoAlgBase.h"

// from Gaudi
#include "GaudiKernel/AlgFactory.h"

// RichKernel
#include "RichKernel/RichMap.h"

// temporary histogramming numbers
#include "RichRecBase/RichDetParams.h"

// Interfaces
#include "RichRecBase/IRichRecMCTruthTool.h"

//-----------------------------------------------------------------------------
/** @class RichPixelMonitor RichPixelMonitor.h
 *
 *  Monitor class for RichRecPixels.
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   05/04/2002
 */
//-----------------------------------------------------------------------------

class RichPixelMonitor : public RichRecHistoAlgBase
{

public:

  /// Standard constructor
  RichPixelMonitor( const std::string& name,
                    ISvcLocator* pSvcLocator );

  virtual ~RichPixelMonitor( ); ///< Destructor

  virtual StatusCode initialize();    // Algorithm initialization
  virtual StatusCode execute   ();    // Algorithm execution
  virtual StatusCode finalize  ();    // Algorithm finalization

private: // data

  const IRichRecMCTruthTool* m_richRecMCTruth; ///< Pointer to RichRecMCTruthTool interface

  // variables for average PD positions
  RichMap<RichSmartID::KeyType, double> m_xHits;
  RichMap<RichSmartID::KeyType, double> m_yHits;
  RichMap<RichSmartID::KeyType, double> m_zHits;
  RichMap<RichSmartID::KeyType, int> m_hitCount;

};

#endif // RICHRECMONITOR_RICHPIXELMONITOR_H
