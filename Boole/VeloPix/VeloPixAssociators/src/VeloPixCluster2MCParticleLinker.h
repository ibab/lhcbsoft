// $Id: VeloPixCluster2MCParticleLinker.h,v 1.1.1.1 2009-12-04 14:34:46 marcin Exp $
#ifndef VELOPIXCLUSTER2MCPARTICLELINKER_H
#define VELOPIXCLUSTER2MCPARTICLELINKER_H 1
// Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
// Event
#include "Event/MCParticle.h"

namespace LHCb {
  class MCParticle;
};

/** @class VeloPixCluster2MCParticleLinker VeloPixCluster2MCParticleLinker.h
 *  @Based on ST code
 *  
 *  @author Marcin Kucharczyk
 *  @date   01/12/2009
 */

class VeloPixCluster2MCParticleLinker : public GaudiAlgorithm {
      
      friend class AlgFactory<VeloPixCluster2MCParticleLinker>;
  
public:
  /// Standard constructor
  VeloPixCluster2MCParticleLinker(const std::string& name,
                                  ISvcLocator* pSvcLocator);
  virtual ~VeloPixCluster2MCParticleLinker(); ///< Destructor
  virtual StatusCode initialize();            ///< Algorithm initialization
  virtual StatusCode execute   ();            ///< Algorithm execution
  virtual StatusCode finalize  ();            ///< Algorithm finalise
  typedef std::pair<const LHCb::MCParticle*,double> PartPair;
  typedef std::map<const LHCb::MCParticle*,double> ParticleMap;
  // Path to put table
  std::string outputData() const;
 
protected:

private:

  void refsToRelate(std::vector<PartPair>& selectedRefs,
                    const ParticleMap& partMap, 
                    LHCb::MCParticles* particles) const;
  StatusCode associateToTruth(const LHCb::VeloPixCluster* aCluster,
                              ParticleMap& partMap);
  std::string m_asctLocation;
  std::string m_inputData; 
  std::string m_outputData;
  bool m_addSpillOverHits; 
  double m_minFrac;        
  bool m_oneRef;
};

inline std::string VeloPixCluster2MCParticleLinker::outputData() const {
  return  m_outputData;
}

#endif // VELOPIXCLUSTER2MCPARTICLELINKER_H







