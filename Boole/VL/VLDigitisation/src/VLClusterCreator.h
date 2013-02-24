#ifndef VL_CLUSTER_CREATOR_H
#define VL_CLUSTER_CREATOR_H 1

// STL
#include <vector>
// Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
// Event/MCEvent
#include "Event/MCVLDigit.h"
// Event/DigiEvent
#include "Event/VLCluster.h"
// Event/LinkerEvent
#include "Linker/LinkerWithKey.h"

/** @class VLClusterCreator VLClusterCreator.h
 *
 * Create VLClusters from MCVLDigits
 */

class DeVL;
class IStripNoiseTool;

class VLClusterCreator : public GaudiAlgorithm {

public:
  /// Constructor
  VLClusterCreator(const std::string& name, ISvcLocator* pSvcLocator);
  /// Destructor
  virtual ~VLClusterCreator() {}

  virtual StatusCode initialize();    ///< Algorithm initialization
  virtual StatusCode execute();       ///< Algorithm execution

private:

  void makeClusters();
  /// Try to create a cluster from a given digit.
  bool makeCluster(LHCb::MCVLDigit* digit);
  /// Try to add a digit to an existing cluster candidate.
  bool addChannel(LHCb::MCVLDigit* digit, int offset);
  /// Get iterators to the first and last digit on a given sensor.
  void getRange(const unsigned int sensor,
                std::pair<LHCb::MCVLDigits::iterator,
                          LHCb::MCVLDigits::iterator>& range);
  /// Calculate the S/N ratio for a given digit.
  double signalToNoise(LHCb::MCVLDigit* digit);
  /// Link clusters to the underlying MC hits and particles
  void linkClustersToHits();
  void linkClustersToParticles();

  LHCb::MCVLDigits* m_digits; 
  LHCb::VLClusters* m_clusters;
  /// Pointer to detector element
  DeVL* m_det;
  /// Digits forming a cluster candidate 
  std::vector<LHCb::MCVLDigit*> m_candidate;
  /// Channels of current sensor already used for clusters.
  std::vector<bool> m_used;

  std::string m_digitLocation;
  std::string m_clusterLocation;

  // Truth tables
  std::vector<std::pair<LHCb::VLCluster*, 
                        std::map<LHCb::MCHit*, double> > > m_hitTruthTable;
  std::vector<std::pair<LHCb::VLCluster*, 
                        std::map<const LHCb::MCParticle*, 
                                 double> > > m_particleTruthTable;
  /// S/N threshold for a seed digit
  double m_seedSignalToNoiseCut;
  /// S/N threshold for adding digits to an existing cluster
  double m_lowSignalToNoiseCut;
  /// Maximum number of clusters to create per event
  unsigned int m_maxClusters; 
  /// High ADC threshold 
  double m_highThreshold; 

  /// Flag to enable or disable MC truth association
  bool m_truth;
 
  /// Tool for estimating strip noise
  IStripNoiseTool* m_StripNoiseTool;
  /// Number of electrons per ADC count
  double m_electronsPerADC;

  // Output level
  bool m_debug;
  bool m_verbose;
  
};
#endif 
