// $Id: ICaloCosmicsTool.h,v 1.1 2008-05-22 22:48:36 odescham Exp $
#ifndef ICALOCOSMICSTOOL_H 
#define ICALOCOSMICSTOOL_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
// from LHCb
#include "Event/CaloAdc.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
#include "CaloDet/DeCalorimeter.h"

static const InterfaceID IID_ICaloCosmicsTool ( "ICaloCosmicsTool", 1, 0 );

/** @class ICaloCosmicsTool ICaloCosmicsTool.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2008-04-07
 */
class ICaloCosmicsTool : virtual public IAlgTool {
public: 

  // Return the interface ID
  static const InterfaceID& interfaceID() { return IID_ICaloCosmicsTool; }

  virtual StatusCode finalize()=0;
  virtual StatusCode processing()=0;

  virtual Gaudi::XYZPoint referencePoint()=0;
  virtual Gaudi::XYZPoint referencePointVariance()=0;
  virtual const std::pair<Gaudi::XYZPoint,Gaudi::XYZPoint>& extrema()=0;
  virtual const double deposit()=0;
  virtual const double phi()=0;
  virtual const double phiVariance()=0;
  virtual const double asymmetry()=0;
  virtual const double asymmetryVariance()=0;
  virtual const double slot()=0;
  virtual const double time()=0;
  virtual const double timeVariance()=0;
  virtual const double timeDispersion()=0;
  virtual DeCalorimeter* det()=0;
  virtual StatusCode tupling(unsigned int unit)=0;
  virtual bool tracked()=0;
  virtual bool timed()=0;
 

protected:

private:


};
#endif // ICALOCOSMICSTOOL_H
