// $Id: HltSelectionDataToTes.h,v 1.1 2007-07-02 22:11:59 hernando Exp $
#ifndef HLTSELECTIONDATATOTES_H 
#define HLTSELECTIONDATAFROMTES_H 1

// Include files
// from Gaudi
#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiAlg/GaudiAlgorithm.h"

#include "Event/HltSummary.h"
#include "HltBase/HltTypes.h"
#include "HltBase/HltSummaryHelper.h"
#include "Event/Track.h"
#include "Event/RecVertex.h"

/** @class HltSelectionDataToTes HltSelectionDataToTes.h
 *  
 *  functionality (TODO):
 *         select a track from a list of track container
 *         plots efficiency of a list of selection summary and L0 
 *    as a function of Pt of the selected track.
 *
 *  @author Jose Angel Hernando Morata
 *  @date   2006-09-01
 */
class HltSelectionDataToTes : public GaudiAlgorithm {
public: 
  /// Standard constructor
  HltSelectionDataToTes( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~HltSelectionDataToTes( ); ///< Destructor

  virtual StatusCode initialize();    ///< Algorithm initialization
  virtual StatusCode execute   ();    ///< Algorithm execution
  virtual StatusCode finalize  ();    ///< Algorithm finalization

protected:

  StringArrayProperty m_tracksLocations;
  StringArrayProperty m_verticesLocations;

  std::vector<int> m_tracksSelIDs;
  std::vector<int> m_verticesSelIDs;

  template <class T, class TESCONT>
  TESCONT* copy(int id) 
  {
    typedef typename std::vector<T*> CONT;
    typedef typename CONT::const_iterator CONTITER; 
    if (!m_datasummary->selectionSummary(id).decision()) return NULL;
    const CONT& cont = HltSummaryHelper::retrieve<CONT>(*m_datasummary,id);
    TESCONT* tcont = new TESCONT();
    for (CONTITER it = cont.begin(); it != cont.end(); ++it) {
      const T& t = *(*it); T* tc = t.clone(); tcont->insert(tc);
    }
    return tcont;
  }
  

protected:

  std::string m_dataSummaryLocation;

  IDataProviderSvc* m_hltSvc;
  Hlt::Configuration* m_conf;
  LHCb::HltSummary* m_datasummary;

};
#endif // HLTTRACKFROMTES_H
