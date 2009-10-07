// $Id: TrackAddNNGhostId.h,v 1.1 2009-10-07 10:44:53 albrecht Exp $
#ifndef TRACKADDNNGHOSTID_H 
#define TRACKADDNNGHOSTID_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"


/** @class TrackAddNNGhostId TrackAddNNGhostId.h
 *  
 *
 *  @author Johannes Albrecht
 *  @date   2009-10-06
 */
class ITrackManipulator;

class TrackAddNNGhostId : public GaudiAlgorithm {
public: 
  /// Standard constructor
  TrackAddNNGhostId( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~TrackAddNNGhostId( ); ///< Destructor

  virtual StatusCode initialize();    ///< Algorithm initialization
  virtual StatusCode execute   ();    ///< Algorithm execution
  virtual StatusCode finalize  ();    ///< Algorithm finalization

protected:

private:
  ITrackManipulator* m_ghostTool;
  
  std::string m_inputLocation;
  std::string m_ghostToolName;

};
#endif // TRACKADDNNGHOSTID_H
