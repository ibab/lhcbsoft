// $Id: RichRecoQC.h,v 1.4 2003-10-13 16:12:42 jonrob Exp $
#ifndef RICHRECQC_RICHRECOQC_H
#define RICHRECQC_RICHRECOQC_H 1

// base class
#include "RichRecBase/RichRecAlgBase.h"

// from Gaudi
#include "GaudiKernel/AlgFactory.h"

// Relations
#include "Relations/IAssociatorWeighted.h"
#include "Relations/IRelationWeighted.h"

// Interfaces
#include "RichRecBase/IRichRecMCTruthTool.h"

// temporary histogramming numbers
#include "RichRecBase/RichDetParams.h"

// Event
#include "Event/RichRecStatus.h"

// Histogramming
#include "AIDA/IHistogram1D.h"
#include "AIDA/IHistogram2D.h"

// interfaces
#include "RichRecBase/IRichParticleProperties.h"
#include "RichRecBase/IRichCherenkovAngle.h"

/** @class RichRecoQC RichRecoQC.h
 *
 *  Quality control monitor for Rich Reconstruction
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   2002-07-02
 */

class RichRecoQC : public RichRecAlgBase {

public:

  /// Standard constructor
  RichRecoQC( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~RichRecoQC( ); ///< Destructor

  virtual StatusCode initialize();    ///< Algorithm initialization
  virtual StatusCode execute   ();    ///< Algorithm execution
  virtual StatusCode finalize  ();    ///< Algorithm finalization

private: // methods

  /// Book histograms
  StatusCode bookHistograms();

  /// Book histograms that require MC truth
  StatusCode bookMCHistograms();

private: // data

  // Pointers to tool instances
  IRichParticleProperties * m_richPartProp; ///< Rich Particle properties
  IRichCherenkovAngle * m_ckAngle;  ///< Pointer to RichCherenkovAngle tool

  /// Pointer to RichRecMCTruthTool interface
  IRichRecMCTruthTool* m_richRecMCTruth;

  // job options
  std::string m_mcHistPth; ///< Output MC truth histogram path
  std::string m_histPth;   ///< Output histogram path
  double m_minBeta;        ///< minimum beta value for 'saturated' tracks

  // Histograms
  IHistogram1D* m_ckTrueDTheta[Rich::NRadiatorTypes];       ///< Cherenkov angle resolution for beta=1 tracks
  IHistogram1D* m_trueSignalPhots[Rich::NRadiatorTypes];    ///< Number of observed p.e.s for beta=1 tracks 

};
#endif // RICHRECQC_RICHRECOQC_H
