// $Id: ICaloCosmicsTrackTool.h,v 1.1 2008-05-22 22:48:36 odescham Exp $
#ifndef ICALOCOSMICSTRACKTOOL_H 
#define ICALOCOSMICSTRACKTOOL_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
// from LHCb
#include "CaloInterfaces/ICaloCosmicsTool.h"
#include "GaudiKernel/Point3DTypes.h"
#include "Event/Track.h"

static const InterfaceID IID_ICaloCosmicsTrackTool ( "ICaloCosmicsTrackTool", 1, 0 );

/** @class ICaloCosmicsTrackTool ICaloCosmicsTrackTool.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2008-05-17
 */
class ICaloCosmicsTrackTool : virtual public IAlgTool {
public: 

  // Return the interface ID
  static const InterfaceID& interfaceID() { return IID_ICaloCosmicsTrackTool; }

  virtual StatusCode finalize()=0;
  virtual StatusCode processing()=0;
  virtual StatusCode tupling(unsigned int unit)=0;
  //virtual  const double time()=0;
  virtual const double phi()=0;
  virtual const double phiVariance()=0;
  virtual const double theta()=0;
  virtual const double thetaVariance()=0;
  virtual const Gaudi::XYZVector slopes()=0;
  virtual const Gaudi::XYZPoint  referencePoint()=0;
  virtual const Gaudi::SymMatrix3x3 slopesCovariance()=0;
  virtual const Gaudi::SymMatrix3x3 referencePointCovariance()=0;
  virtual const bool   forward()=0;
  virtual const bool   tracked()=0;
  virtual const double time()=0;
  virtual const double timeVariance()=0;
  virtual const bool   timed()=0;  
  virtual const LHCb::Track& track()=0;
  virtual ICaloCosmicsTool* ecal()=0;
  virtual ICaloCosmicsTool* hcal()=0;

  virtual StatusCode propagate(Gaudi::Plane3D plane)=0;
  virtual StatusCode propagate(double z )=0;


protected:

private:

};
#endif // ICALOCOSMICSTRACKTOOL_H
