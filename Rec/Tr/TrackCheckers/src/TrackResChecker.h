// $Id: TrackResChecker.h,v 1.3 2008-06-03 06:39:08 cattanem Exp $
#ifndef TRACKRESCHECKER_H
#define TRACKRESCHECKER_H 1
 
// Include files
#include <vector>
 
// from Gaudi
#include "TrackCheckerBase.h"

// linkers
#include "Linker/LinkedTo.h"

#include "Event/MCParticle.h"
#include "Event/OTTime.h"

class ITrackProjectorSelector;

namespace LHCb{
  class Track;
  class State;
  class OTMeasurement;
}

/** @class TrackResChecker TrackResChecker.h
 * 
 * Class for track monitoring
 *  @author M. Needham.
 *  @date   6-5-2007
 */                 
                                                           
class TrackResChecker : public TrackCheckerBase {
                                                                             
 public:
                                                                             
  /** Standard construtor */
  TrackResChecker( const std::string& name, ISvcLocator* pSvcLocator );                                                                             /** Destructor */
  virtual ~TrackResChecker();

  /** Algorithm initialize */
  virtual StatusCode initialize();

  /** Algorithm execute */
  virtual StatusCode execute();

 private:

 void resolutionHistos(const LHCb::Track* track, 
                       const LHCb::MCParticle* mcPart,
                       const std::string& type) const;

 void pullplots(const LHCb::State& trueState, const LHCb::State& recState,
                const std::string& location ) const;
  
 void checkAmbiguity(const LHCb::Track* track, 
                     const LHCb::MCParticle* mcPart,
                     const std::string& type ) const;

 bool checkAmbiguity(const LHCb::MCParticle* mcPart, 
                     const LHCb::OTMeasurement* otMeas) const;

 void TrackResChecker::plotsByMeasType(const LHCb::Track* track, 
                                       const LHCb::MCParticle* mcPart,
                                       const std::string& type) const;

 std::vector<double> m_zPositions;
 bool m_plotsByMeasType;

 typedef LinkedTo<LHCb::MCParticle,LHCb::OTTime> OTLinks;
 mutable OTLinks m_otLinker;
 unsigned int  m_minToCountAmb;
 bool m_checkAmbiguity;
 double m_minAmbDist;

 ITrackProjectorSelector* m_projectorSelector;

};

#endif // TRACKRESCHECKER_H
