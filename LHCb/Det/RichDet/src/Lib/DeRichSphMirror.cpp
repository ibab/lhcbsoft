
/** @file DeRichSphMirror.cpp
 *
 *  Implementation file for detector description class : DeRichSphMirror
 *
 *  $Id: DeRichSphMirror.cpp,v 1.18 2005-12-16 15:09:17 jonrob Exp $
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2004-06-18
 */

#define DERICHSPHMIRROR_CPP

// Include files
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/RegistryEntry.h"
#include "GaudiKernel/SmartDataPtr.h"

#include "Kernel/Vector3DTypes.h"
#include "Kernel/Transform3DTypes.h"
#include "Kernel/SystemOfUnits.h"

/// Detector description classes
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/SolidSphere.h"
#include "DetDesc/SolidBoolean.h"
#include "DetDesc/Surface.h"

#include "RichDet/DeRichSphMirror.h"
#include "RichDet/DeRich.h"

//======================================================================

const CLID& CLID_DeRichSphMirror = 12030;  // User defined

// Standard Constructor
DeRichSphMirror::DeRichSphMirror()
  : m_alignmentConstantX(0.0),
    m_alignmentConstantY(0.0),
    m_mirrorNumber(-1)
{}

// Standard Destructor
DeRichSphMirror::~DeRichSphMirror() {}

// Retrieve Pointer to class defininition structure
const CLID& DeRichSphMirror::classID()
{
  return CLID_DeRichSphMirror;
}

StatusCode DeRichSphMirror::initialize()
{
  MsgStream msgStart( msgSvc(), "DeRichSphMirror" );
  msgStart << MSG::VERBOSE << "Initializing spherical mirror" << endmsg;
  //this->printOut(msg);

  // find if this is a primary or secondary mirror
  bool secondary( true );
  const std::string::size_type secPos = name().find("SecMirror");
  if ( std::string::npos == secPos ) secondary = false;
  std::string localName = ( secondary ? "DeRichSecMirror" : "DeRichSphMirror" );
  MsgStream msg( msgSvc(), localName );

  // find if this mirror is in Rich1 or Rich2
  const std::string::size_type pos = name().find("Rich");
  if ( std::string::npos != pos ) {
    m_name = name().substr(pos);
    std::string richNum = m_name.substr(4,1);
    if ( richNum == "1" )
      m_rich = Rich::Rich1;
    else
      if ( richNum == "2")
        m_rich = Rich::Rich2;
      else {
        msg << MSG::FATAL<<"Could not identify Rich (1/2=="<<richNum<<" )"<<endmsg;
        return StatusCode::FAILURE;
      }
  } else {
    m_name = "DeRichSphMirror_NO_NAME";
    msg << MSG::FATAL << "Cannot identify Rich number!" << endmsg;
    return StatusCode::FAILURE;
  }

  // extract mirror number from detector element name
  const std::string::size_type pos2 = name().find(':');
  if ( std::string::npos == pos2 ) {
    msg << MSG::FATAL << "A spherical mirror without a number!" << endmsg;
    return StatusCode::FAILURE;
  }
  m_mirrorNumber = atoi( name().substr(pos2+1).c_str() );
  const std::string mirNumString = name().substr(pos2+1);

  std::string rich1GeomLoc( "Rich1" );
  std::string rich2GeomLoc( "Rich2" );
  std::string rich2Location;
  if ( name().find("Magnet") == std::string::npos )
    rich2Location = DeRichLocation::Rich2_old;
  else {
    rich2Location = DeRichLocation::Rich2;
    rich1GeomLoc = "BeforeMagnetRegion/"+rich1GeomLoc;
    rich2GeomLoc = "AfterMagnetRegion/"+rich2GeomLoc;
  }

  SmartDataPtr<DeRich> deRich2(dataSvc(), rich2Location);
  if (!deRich2) {
    msg << MSG::FATAL << "Cannot locate " << rich2Location << endmsg;
    return StatusCode::FAILURE;
  }
    
  double hexRadius = 510.0*mm;
  if ( deRich2->exists("Rich2SphMirrorHexDiameter") )
    hexRadius = deRich2->param<double>("Rich2SphMirrorHexDiameter")/2.0;
  //const double flatToCentre = hexRadius*sin(60*degree);

  m_solid = geometry()->lvolume()->solid();
  const std::string type = m_solid->typeName();

  const SolidSphere* sphereSolid = 0;
  // find the sphere of the spherical mirror
  if ( type == "SolidSphere" ) {
    sphereSolid = dynamic_cast<const SolidSphere*>(m_solid);
  } else {
    //assume bollean solid
    const SolidBoolean* compSolid = dynamic_cast<const SolidBoolean*>(m_solid);
    //compSolid->printOut();

    if ( compSolid->first()->typeName() == "SolidSphere") {
      sphereSolid = dynamic_cast<const SolidSphere*>(compSolid->first());
    }  else {
      SolidBoolean::SolidChildrens components;
      SolidBoolean::SolidChildrens::const_iterator iter;
      for (iter=compSolid->childBegin(); iter!=compSolid->childEnd(); ++iter) {
        if ( (*iter)->solid()->typeName() == "SolidSphere")
          sphereSolid = dynamic_cast<const SolidSphere*>((*iter)->solid());

      }
    }
    if ( !sphereSolid) {
      msg << MSG::FATAL << "Problem finding a sphere solid in :- " << endmsg;
      compSolid->printOut();
      return StatusCode::FAILURE;
    }
  }

  // Now that we have the sphere, try to find out its radius
  m_radius = sphereSolid->insideRadius();

  // and its centre
  const Gaudi::XYZPoint zero(0.0, 0.0, 0.0);
  Gaudi::Polar3DVector toSphCentre(1.0, 0.0, 0.0);
  if ( 0.0 != sphereSolid->startThetaAngle() ) {
    toSphCentre.SetCoordinates( 1.0,
                                sphereSolid->startThetaAngle() +
                                sphereSolid->deltaThetaAngle()/2.0,
                                sphereSolid->startPhiAngle() +
                                sphereSolid->deltaPhiAngle()/2.0 );

  }

  ISolid::Ticks sphTicks;
  const unsigned int sphTicksSize = sphereSolid->
    intersectionTicks(zero, Gaudi::XYZVector( toSphCentre ), sphTicks);
  if (sphTicksSize != 2) {
    msg << MSG::FATAL << "Problem getting mirror radius, noTicks: "
        << sphTicksSize << endmsg;
    return StatusCode::FAILURE;
  }
  const Gaudi::XYZPoint localMirrorCentre(sphTicks[0]*toSphCentre);
  m_mirrorCentre = geometry()->toGlobal(localMirrorCentre);

  // right and left middle points are for verification of the hex segment position
  //const Gaudi::XYZPoint middleRightSide
  //  (sqrt(m_radius*m_radius-flatToCentre*flatToCentre),0.0,flatToCentre);
  //const Gaudi::XYZPoint middleLeftSide
  //  (sqrt(m_radius*m_radius-flatToCentre*flatToCentre),0.0,-flatToCentre);

  // go back to the global coord system.
  // Ignore the rotation, it has been taken care of
  m_centreOfCurvature = geometry()->toGlobal(zero);

  msg << MSG::DEBUG << "Mirror #" << m_mirrorNumber << " " << m_rich << " Radius:"
      << m_radius << " Centre of curvature " << m_centreOfCurvature << endmsg;
  msg << MSG::DEBUG << "Centre of mirror " << m_mirrorCentre << endmsg;

  // the following lines can be uncommented for debug
  //if( m_rich == Rich::Rich2 ) {
  //  if( (m_mirrorNumber == 31) || (m_mirrorNumber < 28 && m_mirrorNumber != 0) )
  //    msg << MSG::VERBOSE << "Right middle "
  //        << geometry()->toGlobal(middleRightSide) << endmsg;
  //  else
  //    msg << MSG::VERBOSE << "Left middle "
  //        << geometry()->toGlobal(middleLeftSide) << endmsg;
  //}

  // centre normal vector
  const Gaudi::XYZVector localCentreNormal
    (Gaudi::XYZVector(-localMirrorCentre.x(),-localMirrorCentre.y(),
                      -localMirrorCentre.z()).unit() );
  m_centreNormal = geometry()->matrixInv()*localCentreNormal;
  msg << MSG::DEBUG << "Normal vector at the centre" << m_centreNormal << endmsg;

  m_centreNormalPlane = Gaudi::Plane3D(m_centreNormal, m_mirrorCentre);
  msg << MSG::DEBUG << "centreNormalPlane " << m_centreNormalPlane << endmsg;

  // find surface properties
  std::string surfLocation, sphMirrorName, surfName;
  if ( m_rich == Rich::Rich1 ) {
    surfLocation = "/dd/Geometry/"+rich1GeomLoc+"/Rich1Surfaces";
    sphMirrorName = "Mirror1";
    surfName = ":"+mirNumString;
  }
  else{
    surfLocation = "/dd/Geometry/"+rich2GeomLoc+"/Rich2Surfaces";
    sphMirrorName = ( secondary  ? "SecMirror" : "SphMirror");
    surfName = "Seg"+mirNumString;
  }

  bool foundSurface( false );

  SmartDataPtr<DataObject> rich2SurfCat(dataSvc(),surfLocation);
  if (!rich2SurfCat) {
    msg << MSG::FATAL << "Cannot locate suface for mirror " + name() << endmsg;
    return StatusCode::FAILURE;
  }

  DataSvcHelpers::RegistryEntry* rich2Reg = dynamic_cast<DataSvcHelpers::RegistryEntry*>
    (rich2SurfCat->registry());
  IRegistry* storeReg = 0;

  // find the surface in the registry
  for (DataSvcHelpers::RegistryEntry::Iterator child = rich2Reg->begin();
       (child != rich2Reg->end() && !foundSurface); ++child){
    // child is a const_iterator of vector<IRegistry*>
    const std::string::size_type pos3 = (*child)->name().find(sphMirrorName);
    if ( std::string::npos != pos3 ) {
      const std::string::size_type pos4 = (*child)->name().find(surfName);
      if ( std::string::npos != pos4 ) {
        storeReg = (*child);
        foundSurface = true;
      }
    }
  }

  bool foundRefl( false );
  // get the surface, get the tabulated properties and find REFLECTIVITY
  if ( foundSurface ) {
    SmartDataPtr<DataObject> obj (dataSvc(), storeReg->identifier());
    DataObject* pObj = obj;
    msg << MSG::VERBOSE << "Dynamic cast to surface " << obj->name() << endmsg;
    Surface* surf = dynamic_cast<Surface*> (pObj);
    const Surface::Tables surfTabProp = surf->tabulatedProperties();
    for (Surface::Tables::const_iterator table_iter = surfTabProp.begin();
         table_iter != surfTabProp.end(); ++table_iter) {
      if ( (*table_iter)->type() == "REFLECTIVITY" ) {
        m_reflectivity = (*table_iter);
        foundRefl = true;
        break;
      }
    }
    if ( !foundRefl ) {
      msg << MSG::FATAL <<"Could not find REFLECTIVITY "<< surf->name() << endmsg;
      return StatusCode::FAILURE;
    }
  }
  else {
    msg << MSG::FATAL <<"Could not find surface for mirror "<< myName() << endmsg;
    return StatusCode::FAILURE;
  }

  msg << MSG::DEBUG << "Reflectivity is from TabProp "
      << m_reflectivity->name() << endmsg;
  msg << MSG::VERBOSE <<"End initialisation for DeRichSphMirror" << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================

StatusCode DeRichSphMirror:: intersects(const Gaudi::XYZPoint& globalP,
                                        const Gaudi::XYZVector& globalV,
                                        Gaudi::XYZPoint& intersectionPoint ) const
{
  const Gaudi::XYZPoint pLocal( geometry()->toLocal(globalP) );
  Gaudi::XYZVector vLocal( geometry()->matrix()*globalV );

  ISolid::Ticks ticks;
  const unsigned int noTicks = m_solid->intersectionTicks(pLocal, vLocal, ticks);

  if ( 0 == noTicks ) {
    return StatusCode::FAILURE;
  }
  else {
    intersectionPoint = geometry()->toGlobal( pLocal + ticks[0]*vLocal );
    return StatusCode::SUCCESS;
  }

}

//=============================================================================

StatusCode DeRichSphMirror::intersects( const Gaudi::XYZPoint& globalP,
                                        const Gaudi::XYZVector& globalV ) const
{
  Gaudi::XYZVector vLocal( geometry()->matrix()*globalV );

  ISolid::Ticks ticks;
  const unsigned int noTicks = m_solid->intersectionTicks(geometry()->toLocal(globalP),
                                                          vLocal, ticks);

  return ( 0 == noTicks ? StatusCode::FAILURE : StatusCode::SUCCESS );
}
