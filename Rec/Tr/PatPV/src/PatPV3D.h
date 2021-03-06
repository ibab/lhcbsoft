#ifndef PATPV_PATPV3D_H
#define PATPV_PATPV3D_H 1

// Include files
// -------------
// From Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
// Interfaces
#include "TrackInterfaces/IPVOfflineTool.h"

/** @class PatPV3D PatPV3D.h
 *  Algorithm to find the primary vertices at the HLT.
 *
 *  @author Eduardo Rodrigues
 *  @date   2004-02-17
 */

//-----------------------------------------------------------------------------

class PatPV3D : public GaudiAlgorithm {
public:
  /// Standard constructor
  PatPV3D( const std::string& name, ISvcLocator* pSvcLocator );

  ~PatPV3D( ) override = default;    ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  std::string m_recVertexLocation ;     /// Output location of PVs of type RecVertex
  std::string m_primaryVertexLocation ; /// Output location of PVs of type PrimaryVertex
  bool m_refitpv ;                      /// Flag to refit PVs when converting to type PrimaryVertex

  IPVOfflineTool* m_pvsfit;    // PV fitting tool

};
#endif // PATPV_PATPV3D_H
