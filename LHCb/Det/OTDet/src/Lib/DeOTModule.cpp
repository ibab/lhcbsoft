// $Id: DeOTModule.cpp,v 1.8 2006-02-23 17:30:00 ebos Exp $

// Kernel
#include "Kernel/SystemOfUnits.h"
#include "Kernel/LineTraj.h"

// DetDesc
#include "DetDesc/SolidBox.h"

// local
#include "OTDet/DeOTModule.h"

/** @file DeOTModule.cpp
 *
 *  Implementation of class :  DeOTModule
 *
 *  @author Jeroen van Tilburg jtilburg@nikhef.nl
 */

using namespace LHCb;

DeOTModule::DeOTModule( const std::string& name ) :
  DetectorElement( name ),
  m_moduleID(0),
  m_quarterID(0),
  m_layerID(0),
  m_stationID(0),
  m_uniqueModuleID(0),
  m_nStraws(0),
  m_longModule(true),
  m_stereoAngle(0.0),
  m_sinAngle(0.0),
  m_cosAngle(0.0),
  m_pitch(0.0),
  m_cellRadius(0.0),
  m_inefficientRegion(0.0),
  m_nModules(0),
  m_ySizeModule(0.0)
{ }


DeOTModule::~DeOTModule() { }

const CLID& DeOTModule::clID() const { 
  return classID();
}


StatusCode DeOTModule::initialize() 
{
 
  IDetectorElement* quarter = this->parentIDetectorElement();
  IDetectorElement* layer = quarter->parentIDetectorElement();
  IDetectorElement* station = layer->parentIDetectorElement();
  IDetectorElement* ot = station->parentIDetectorElement();

  // Get specific parameters from the module
  m_moduleID = (unsigned int) param<int>("moduleID");
  m_quarterID = (unsigned int) quarter->params()->param<int>("quarterID");
  m_layerID = (unsigned int) layer->params()->param<int>("layerID");
  m_stationID = (unsigned int) station->params()->param<int>("stationID");
  m_nStraws = (unsigned int) param<int>("nStraws");
  m_longModule = 
    ((this->geometry())->lvolumeName() == "/dd/Geometry/OT/Modules/lvLModule");
  m_stereoAngle = layer->params()->param<double>("stereoAngle");
  m_sinAngle = sin(m_stereoAngle);
  m_cosAngle = cos(m_stereoAngle);  
  OTChannelID aChannel = OTChannelID( m_stationID, m_layerID, 
                                      m_quarterID, m_moduleID, 0);
  m_uniqueModuleID = aChannel.uniqueModule();

  // Get some general parameters for the OT
  m_pitch = ot->params()->param<double>("strawPitch");
  m_cellRadius = ot->params()->param<double>("cellRadius");
  m_inefficientRegion = ot->params()->param<double>("inefficientRegion");
  m_nModules = (unsigned int) ot->params()->param<int>("nModules");

  // Get the lenght of the module
  const ILVolume* lv = (this->geometry())->lvolume();
  const ISolid* solid = lv->solid();
  const SolidBox* mainBox = dynamic_cast<const SolidBox*>( solid );
  m_ySizeModule = mainBox->ysize();

  return StatusCode::SUCCESS;
}


double DeOTModule::localUOfStraw(const int straw) const
{
  int tmpStraw = straw;
  double uLeftStraw =  -(m_nStraws/2. - 0.25) * m_pitch;
  if (this->monoLayerB(straw)) { // monoLayer B
    tmpStraw -= m_nStraws;
    uLeftStraw += 0.5 * m_pitch;
  }
  tmpStraw -= 1; // straw numbering starts at 1
  
  return uLeftStraw + tmpStraw * m_pitch;
}


StatusCode DeOTModule::calculateHits(const Gaudi::XYZPoint& entryPoint,
                                     const Gaudi::XYZPoint& exitPoint,
                                     std::vector<OTChannelID>& channels, 
                                     std::vector<double>& driftDistances) const
{
  MsgStream msg( msgSvc(), name() );

  // Before we do anything crazy let's first check if the 
  // entry and exit points are inside the module.
  if ( !isInside(entryPoint) || !isInside(exitPoint) ) {
    return StatusCode::FAILURE;
  }

  const IGeometryInfo* gi = this->geometry();
  Gaudi::XYZPoint point1 = gi->toLocal(entryPoint);
  Gaudi::XYZPoint point2 = gi->toLocal(exitPoint);

  double x1 = point1.x();
  double y1 = point1.y();
  double z1 = point1.z();
  double x2 = point2.x();
  double y2 = point2.y();
  double z2 = point2.z();
  double distXY = ( point2 - point1 ).Rho();
  //double distXY=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));

  // ensure correct z ordering
  if ( z1 > z2 ) {
    std::swap(x1,x2);
    std::swap(y1,y2);
    std::swap(z1,z2);
  }
  
  double uLow, uHigh;      
  if ( x1 < x2 ) {
    uLow  = x1;
    uHigh = x2;
  } else {
    uLow  = x2;
    uHigh = x1;
  }
  
  //if ( fabs(z1-z2) > 0.5*mm ) {
  if ( fabs(z1-z2) > .1*m_cellRadius ) { // Track in cell
    double yMiddle = (y1 + y2)/2.;
    double dUdZ = (x2 - x1)/(z2 - z1);
    double uAtMonoA = x1 +  dUdZ*(localZOfStraw(1) - z1);
    double uAtMonoB = x1 +  dUdZ*(localZOfStraw(m_nStraws+1) - z1);    
    int strawA = hitStrawA(uAtMonoA);
    int strawB = hitStrawB(uAtMonoB);
    OTChannelID tmpChan;
    
    // layer A
    double cosTrack = 1.0/sqrt(1.0 + dUdZ*dUdZ);
    double uHitStrawA = localUOfStraw(strawA);
    double dist = (uAtMonoA - uHitStrawA) * cosTrack;
  	bool betweenU = ( uHitStrawA > uLow-m_pitch ) && 
                    ( uHitStrawA < uHigh+m_pitch);
    if ( fabs(dist) < m_cellRadius && betweenU && isEfficientA(yMiddle)) {
      tmpChan = OTChannelID( m_stationID, m_layerID,
			     m_quarterID, m_moduleID, strawA);
      channels.reserve(64); // nChannels is 128 except for S3module, 64
      channels.push_back(tmpChan);
      driftDistances.reserve(64); // nChannels is 128 except for S3module, 64
      driftDistances.push_back(dist);

      // check straws to the right
      int rStrawA = strawA;
      while ( ++rStrawA <= (int) m_nStraws) {
        uHitStrawA = localUOfStraw(rStrawA);
        dist = (uAtMonoA - uHitStrawA) * cosTrack;
        betweenU = ( uHitStrawA > uLow-m_pitch ) &&
                   ( uHitStrawA < uHigh+m_pitch);
        if ( fabs(dist) < m_cellRadius && betweenU ) {
          tmpChan = OTChannelID( m_stationID, m_layerID, 
                                 m_quarterID, m_moduleID, rStrawA);
          channels.push_back(tmpChan);
          driftDistances.push_back(dist);
        } else {
          rStrawA = m_nStraws;
        }
      }
      // check straws to the left
      int lStrawA = strawA;
      while ( --lStrawA >= 1) {
        uHitStrawA = localUOfStraw(lStrawA);
        dist = (uAtMonoA - uHitStrawA) * cosTrack;
        betweenU = ( uHitStrawA > uLow-m_pitch ) &&
                   ( uHitStrawA < uHigh+m_pitch);
        if ( fabs(dist) < m_cellRadius && betweenU ) {
          tmpChan = OTChannelID( m_stationID, m_layerID, 
                                 m_quarterID, m_moduleID, lStrawA);
          channels.push_back(tmpChan);
          driftDistances.push_back(dist);
        } else {
          lStrawA = 1;
        }
      }
    }
    
    // layer B
    double uHitStrawB = localUOfStraw(strawB);
    dist = (uAtMonoB - localUOfStraw(strawB)) * cosTrack;
  	betweenU = ( uHitStrawB > uLow-m_pitch ) &&
               ( uHitStrawB < uHigh+m_pitch);
    if ( fabs(dist) < m_cellRadius && betweenU && isEfficientB(yMiddle) ) {
      tmpChan = OTChannelID( m_stationID, m_layerID, 
                             m_quarterID, m_moduleID, strawB);
      channels.push_back(tmpChan);
      driftDistances.push_back(dist);
      
      // check straws to the right
      int rStrawB = strawB;
      while ( ++rStrawB <= (int) (2*m_nStraws) ) {
        uHitStrawB = localUOfStraw(rStrawB);
        dist = (uAtMonoB - uHitStrawB) * cosTrack;
        betweenU = ( uHitStrawB > uLow-m_pitch ) &&
                   ( uHitStrawB < uHigh+m_pitch);
        if ( fabs(dist) < m_cellRadius && betweenU ) {
          tmpChan = OTChannelID( m_stationID, m_layerID, 
                                 m_quarterID, m_moduleID, rStrawB);
          channels.push_back(tmpChan);
          driftDistances.push_back(dist);
        } else {
          rStrawB = 2*m_nStraws;
        }
      }
      // check straws to the left
      int lStrawB = strawB;
      while ( --lStrawB >= (int) m_nStraws+1 ) {
        uHitStrawB = localUOfStraw(lStrawB);
        dist = (uAtMonoB - uHitStrawB) * cosTrack;
        betweenU = ( uHitStrawB > uLow-m_pitch ) &&
                   ( uHitStrawB < uHigh+m_pitch);
        if ( fabs(dist) < m_cellRadius && betweenU ) {
          tmpChan = OTChannelID( m_stationID, m_layerID, 
                                 m_quarterID, m_moduleID, lStrawB);
          channels.push_back(tmpChan);
          driftDistances.push_back(dist);
        } else {
          lStrawB = m_nStraws+1;
        }
      }
    }
  } else {  // dz < 0.5 mm 
    // treatment of curling tracks. Code should be made more readable.
    //msg << MSG::DEBUG << "We've got a curly track :-); z1 = " << z1 << ", z2 = " << z2 << ", and fabs(z1 -z2) = " << fabs(z1-z2) << endreq;
    double z3Circ,zCirc,uCirc,rCirc;
    double zfrac,zint;

    // zfrac is between 0 and 1. 2.7839542167 means nothing.
    // This seems to acts as a random number generator.
    // if distance xy entry-exit is small generate z3 close
    // to the z1 and z2 ( z1 is close to z2)
    double u1 = (entryPoint.z() > exitPoint.z()) ? 
      exitPoint.x() : entryPoint.x();
    double v1 = (entryPoint.z() > exitPoint.z()) ? 
      exitPoint.y() : entryPoint.y();
    zfrac = modf( (fabs(u1)+fabs(v1))/2.7839542167, &zint);
    if (distXY > 2.0 * m_pitch ) {
      z3Circ = 2.0 * (zfrac-0.5)*m_pitch;
    }
    else {
      if ( z1 < 0 ) {
        z3Circ = - zfrac * m_pitch; 
      } 
      else {
        z3Circ = zfrac * m_pitch; 
      }
    }

    sCircle(z1, x1, z2, x2, z3Circ, zCirc, uCirc, rCirc);

    // monolayer A
    int strawA = hitStrawA(uLow);
    double zStrawA = localZOfStraw(strawA);
    double uStep = uLow;
    while ( (uStep < uHigh) && strawA != 0 ) {
      uStep = localUOfStraw(strawA);
      double distCirc = 
         sqrt((zCirc-zStrawA)*(zCirc-zStrawA) + (uCirc-uStep)*(uCirc-uStep));
      double distTmp = fabs(distCirc-rCirc);
      int straw = strawA;
      double ambTmp = - (uStep-(x1+x2)/2.) * (distCirc-rCirc);

      if ( distTmp < m_cellRadius ) {
        if (ambTmp < 0.0 ) distTmp *= -1.0;
        OTChannelID tmpChan = OTChannelID( m_stationID, m_layerID, 
                                           m_quarterID, m_moduleID, straw);
        channels.push_back(tmpChan);
        driftDistances.push_back(distTmp);
      }
      strawA = nextRightStraw(straw);
    }

    // monolayer B
    int strawB = hitStrawB(uLow);
    double zStrawB = localZOfStraw(strawB);
    uStep = uLow;
    while ( (uStep < uHigh) && strawB != 0 ) {
      uStep = localUOfStraw(strawB);
      double distCirc = 
        sqrt((zCirc-zStrawB)*(zCirc-zStrawB) + (uCirc-uStep)*(uCirc-uStep));
      double distTmp = fabs(distCirc-rCirc);
      int straw = strawB;

      double ambTmp = - (uStep-(x1+x2)/2.) * (distCirc-rCirc);
      if ( distTmp < m_cellRadius ) {
        if (ambTmp < 0.0) distTmp *= -1.0;
        OTChannelID tmpChan = OTChannelID( m_stationID, m_layerID, 
                                           m_quarterID, m_moduleID, straw);
        channels.push_back(tmpChan);
        driftDistances.push_back(distTmp);
      }
      strawB = nextRightStraw(straw);
    }
  } // curling tracks

  return StatusCode::SUCCESS; 
}


void DeOTModule::sCircle(const double z1, const double u1, const double z2, 
                         const double u2, const double z3c,
                         double& zc, double& uc, double& rc) const
{
  double zw,uw;
  
  zw=(z1+z2)/2.;
  uw=(u2+u1)/2.;
  zc=0.5*(z3c*z3c-zw*zw-(u1-uw)*(u1-uw))/(z3c-zw);
  uc=uw;
  rc=fabs(zc-z3c);
}


double DeOTModule::distanceToWire(const unsigned int straw, 
                                  const Gaudi::XYZPoint& aPoint, 
                                  const double tx, const double ty) const
{
  // go to the local coordinate system
  Gaudi::XYZVector vec(tx, ty, 1.);
  Gaudi::XYZPoint localPoint = (this->geometry())->toLocal(aPoint);
  Gaudi::XYZVector localVec = (this->geometry())->toLocal(aPoint+vec) - localPoint;

  // calculate distance to the straw
  double z = this->localZOfStraw(straw);
  double u = localPoint.x() + localVec.x() * (z - localPoint.z());
  double cosU = 1./sqrt( 1. + pow( (localVec.x()/localVec.z()) , 2 ));
  double dist = (u - localUOfStraw(straw)) * cosU;
  
  return dist;
}


double DeOTModule::distanceAlongWire(const double xHit, 
                                     const double yHit) const
{ 
  Gaudi::XYZPoint globalPoint(xHit, yHit, 0);
  Gaudi::XYZPoint localPoint = (this->geometry())->toLocal(globalPoint);

  // For the upper modules of the station the readout is above.
  return ( this->topModule() ) ? 
    m_ySizeModule/2. - localPoint.y() : m_ySizeModule/2. + localPoint.y();
}


Gaudi::XYZPoint DeOTModule::centerOfStraw(const unsigned int straw) const
{
  // get the global coordinate of the middle of the channel
  Gaudi::XYZPoint localPoint(this->localUOfStraw(straw), 
                        0., 
                        this->localZOfStraw(straw));
  return (this->geometry())->toGlobal(localPoint);
}

Gaudi::XYZPoint DeOTModule::centerOfModule() const
{
  // get the global coordinate of the middle of the module
  Gaudi::XYZPoint localPoint(0., 0., 0.);
  return (this->geometry())->toGlobal(localPoint);
}

double DeOTModule::z() const
{
  // get the global z-coordinate of the module
  Gaudi::XYZPoint localPoint(0., 0., 0.);
  Gaudi::XYZPoint centerPoint = (this->geometry())->toGlobal(localPoint);
  return centerPoint.z();
}

/// Returns a Trajectory representing the wire identified by the LHCbID
/// The offset is zero for all OT Trajectories
LHCb::Trajectory* DeOTModule::trajectory( const OTChannelID& aChan,
                                          const double /*offset*/ ) const 
{
  LineTraj* traj = 0;
  if( contains( aChan) == true ) {
    double halfSizeModule = m_ySizeModule/2.;
    // Trajectory points from beamline towards readout.
    if( bottomModule() ) halfSizeModule *= -1.;
    const unsigned int straw = aChan.straw();
    Gaudi::XYZPoint lowerLocal( this->localUOfStraw(straw),
                                -halfSizeModule,
                                this->localZOfStraw(straw) );
    Gaudi::XYZPoint upperLocal( this->localUOfStraw(straw),
                                halfSizeModule,
                                this->localZOfStraw(straw) );
    Gaudi::XYZPoint lowerGlobal = (this->geometry())->toGlobal(lowerLocal);
    Gaudi::XYZPoint upperGlobal = (this->geometry())->toGlobal(upperLocal);
    traj = new LineTraj( lowerGlobal, upperGlobal );
  }
  else { throw GaudiException( "Failed to make trajectory!", "DeOTModule.cpp",
                               StatusCode::FAILURE ); 
  }
  
  return traj;
}
