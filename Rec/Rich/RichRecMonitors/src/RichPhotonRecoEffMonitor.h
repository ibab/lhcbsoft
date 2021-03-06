
//---------------------------------------------------------------------------
/** @file RichPhotonRecoEffMonitor.h
 *
 *  Header file for algorithm class : Rich::Rec::MC::PhotonRecoEffMonitor
 *
 *  @author Chris Jones       Christopher.Rob.Jones@cern.ch
 *  @date   05/04/2002
 */
//---------------------------------------------------------------------------

#ifndef RICHRECMONITOR_RICHTRACKRESOLUTIONMONI_H
#define RICHRECMONITOR_RICHTRACKRESOLUTIONMONI_H 1

// base class
#include "RichRecBase/RichRecHistoAlgBase.h"

// Event
#include "Event/MCRichOpticalPhoton.h"

// Interfaces
#include "RichRecBase/IRichTrackSelector.h"
#include "MCInterfaces/IRichRecMCTruthTool.h"
#include "RichRecBase/IRichCherenkovAngle.h"
#include "RichRecBase/IRichRecGeomTool.h"

// temporary histogramming numbers
#include "RichRecBase/RichDetParams.h"

namespace Rich
{
  namespace Rec
  {
    namespace MC
    {

      //---------------------------------------------------------------------------
      /** @class PhotonRecoEffMonitor RichPhotonRecoEffMonitor.h
       *
       *  Monitor the photon reconstruction efficiency
       *
       *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
       *  @date   05/04/2002
       */
      //---------------------------------------------------------------------------

      class PhotonRecoEffMonitor : public Rich::Rec::HistoAlgBase
      {

      public:

        /// Standard constructor
        PhotonRecoEffMonitor( const std::string& name,
                              ISvcLocator* pSvcLocator );

        virtual ~PhotonRecoEffMonitor( ); ///< Destructor

        virtual StatusCode initialize();    // Algorithm initialization
        virtual StatusCode execute   ();    // Algorithm execution

      private: // data

        /// Pointer to RichRecMCTruthTool interface
        const Rich::Rec::MC::IMCTruthTool* m_richRecMCTruth = nullptr; 

        /// Rich Cherenkov angle calculator tool
        const ICherenkovAngle * m_ckAngle = nullptr;

        /// Simple geometrical questions      
        const IGeomTool * m_geomTool = nullptr;   

        /// Forced photon creator
        const IPhotonCreator * m_forcedPhotCreator = nullptr;

        /// Track selector
        const ITrackSelector * m_trSelector = nullptr;  

      };

    }
  }
}

#endif // RICHRECMONITOR_RICHTRACKRESOLUTIONMONI_H
