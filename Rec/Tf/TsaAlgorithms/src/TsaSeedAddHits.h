// $ Exp $
#ifndef _TsaSeedAddHits_H
#define _TsaSeedAddHits_H

#include <string>
#include <vector>
#include <algorithm>
#include <utility>

#include "ITsaSeedAddHits.h"

#include "GaudiAlg/GaudiTool.h"

#include "TsaKernel/SeedTrack.h"
#include "TsaKernel/TsaSeedingHit.h"
//#include "TsaKernel/SeedFunctor.h"
#include "TsaKernel/SeedTrack.h"
#include "TsaKernel/TsaConstants.h"

#include "TsaTStationHitManager.h"
#include "TfKernel/RecoFuncs.h"

#include "SeedLineFit.h"
#include "SeedParabolaFit.h"
#include "TfTsHitNumMap.h"

//#include "Kernel/STDataFunctor.h"
#include "Kernel/STChannelID.h"

#include "STDet/DeSTDetector.h"
#include "STDet/DeITStation.h"
#include "STDet/DeITBox.h"

namespace Tf
{
  namespace Tsa
  {

    /** @class SeedAddHits TsaSeedAddHits.h
     * Follow track and pick up hits
     * @author M. Needham
     **/

    class SeedAddHits: public GaudiTool,
                       virtual public ITsaSeedAddHits
    {

    public:

      /// constructer
      SeedAddHits(const std::string& type,
                     const std::string& name,
                     const IInterface* parent);

      // destructer
      virtual ~SeedAddHits();

      virtual StatusCode initialize();

      // execute method
      virtual StatusCode execute(SeedTracks* seeds, SeedHits* hits);

    private:

      //Tsa::STRange range(const int hint, const int station,
      //                   const int layer, const int box) const;

      SeedParabolaFit* m_parabolaFit;
      SeedLineFit* m_fitLine;

      std::string m_dataSvcType;
      std::string m_dataSvcName;

      typedef Tf::Tsa::TStationHitManager ITHitMan;
      typedef ITHitMan::HitRange Range;
      typedef ITHitMan::Hits     Hits;

      /// Pointer to the data manager
      ITHitMan * m_hitMan;

      TfTsHitNumMap m_hitNumMap;

      DeSTDetector* m_tracker;
      unsigned int m_nStations;
      unsigned int m_nBox;
      std::vector<double> m_zBox;
      std::vector<double> m_xMin;
      std::vector<double> m_xMax;
      std::vector<double> m_yMin;
      std::vector<double> m_yMax;
      double m_tol;
      double m_dCut;

    };

  }
}

#endif  // _TsaSeedAddHits_H



