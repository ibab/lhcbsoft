// Include files 

#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "DetDesc/IDetectorElement.h"
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/ILVolume.h"
#include "DetDesc/IPVolume.h"

// local
#include "DetElemFinder.h"

DECLARE_SERVICE_FACTORY( DetElemFinder )

//-----------------------------------------------------------------------------
// Implementation file for class : DetElemFinder
//
// 2006-09-01 : Marco CLEMENCIC
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
DetElemFinder::DetElemFinder( const std::string& name, ISvcLocator* svcloc ):
  base_class(name,svcloc)
{
  declareProperty( "DetectorDataSvc", m_detDataSvcName = "DetectorDataSvc" );
  declareProperty( "RootElement",     m_rootElement    = "/dd/Structure/LHCb" );
  declareProperty( "DumpMap",         m_dumpMap        = false );
}
//=============================================================================
// Destructor
//=============================================================================
DetElemFinder::~DetElemFinder() = default;

//=============================================================================
// Find the detector element
//=============================================================================
const IDetectorElement * DetElemFinder::detectorElementForPath(const std::string &path) const
{
  return m_map(path);
}


//=============================================================================
// Initialization
//=============================================================================
StatusCode DetElemFinder::initialize ( )
{
  // base class initialization
  StatusCode sc = Service::initialize();
  if (!sc.isSuccess()) return sc;

  // local initialization
  MsgStream log(msgSvc(),name());
  if( log.level() <= MSG::DEBUG )
    log << MSG::DEBUG << "--- initialize ---" << endmsg;

  auto detSvc = service<IDataProviderSvc>(m_detDataSvcName,true);
  if (!detSvc) {
		log << MSG::ERROR << "Unable to get a handle to the detector data service ("
        << m_detDataSvcName << ")" << endmsg; 
    return StatusCode::FAILURE;
  }
  
  DataObject *obj;
  sc = detSvc->retrieveObject(m_rootElement,obj);
  if (!sc.isSuccess()) {
		log << MSG::ERROR << "Unable to retrieve object '" << m_rootElement << "'" << endmsg; 
    return sc;
  }
  IDetectorElement *de = dynamic_cast<IDetectorElement *>(obj);
  if (!de) {
		log << MSG::ERROR << "Object '" << m_rootElement << "' is not a DetectorElement" << endmsg; 
    return sc;
  }

  // add all the detector elements to the map
  return insert(de);
}
//=============================================================================
// Finalization
//=============================================================================
StatusCode DetElemFinder::finalize ( )
{
	// local finalization
	MsgStream log(msgSvc(),name());
  if( log.level() <= MSG::DEBUG )
    log << MSG::DEBUG << "--- finalize ---" << endmsg;

  if ( m_dumpMap ) {
    for ( const auto& i : m_map ) {
      log << MSG::ALWAYS << i.first << " -> " << i.second->name() << endmsg;
    } 
  }

	// base class finalization
  return Service::finalize();
}

//=========================================================================
//  get the PV path of the given DE
//=========================================================================
StatusCode DetElemFinder::detector_element_path(const IDetectorElement *de, std::string &path, const std::string &parent_path)  const
{
  StatusCode sc = StatusCode::SUCCESS;

  // initialize the output variable
  path = parent_path;
  
  // get the parent geometryInfo (supportIGeometryInfo() is public)
  IGeometryInfo* sgi = de->geometry()->supportIGeometryInfo();

  if (!sgi) return sc;
  
  // If the parent path was not specified, I have to find it recursively
  if (parent_path.empty()){
    if (de->parentIDetectorElement()) {
      sc = detector_element_path(de->parentIDetectorElement(),path);
      if (!sc.isSuccess()) return sc;
    } else { // this should never happen
      return sc;
    }
  }
  
  // get support LVolume
  const ILVolume* lv = sgi->lvolume();
  
  // define a PVolumePath (this is simply std::vector<const IPVolume*> )
  
  ILVolume::PVolumePath volumePath;
  
  // Fill it with PVolume* corresponding to the supportPath().
  sc = lv->traverse( de->geometry()->supportPath().begin(),
                     de->geometry()->supportPath().end(),
                     volumePath );
  if (!sc.isSuccess()) {
    MsgStream log(msgSvc(),name());
    log << MSG::ERROR << "Cannot traverse the support path" << endmsg;
    return sc;
  }
  
  // build the string
  path = std::accumulate( volumePath.begin(),volumePath.end(), path,
                          [](std::string p, const IPVolume* vp) { 
                              return p + "/" + vp->name();
  });
  return sc;
}


//=========================================================================
//  
//=========================================================================
StatusCode DetElemFinder::insert ( const IDetectorElement *de, const std::string &parent_path) {

  MsgStream log(msgSvc(),name());
  if( log.level() <= MSG::DEBUG )
    log << MSG::DEBUG << "Processing detector element " << de->name() << endmsg;

  StatusCode sc = StatusCode::SUCCESS;

  std::string path;

  // discover the identifier of the DE from the parent:
  sc = detector_element_path(de,path,parent_path);
  if ( ! sc.isSuccess() ) {
    return sc;
  }
  
  auto x = m_map.find(path);
  if ( x != m_map.end() ) {
    log << MSG::ERROR << "Cannot insert duplicated path \"" << path << "\" for \"" << de->name() << "\"" << endmsg;
    log << MSG::ERROR << "Already used for \"" << x->second->name() << "\"" << endmsg;
    return StatusCode::FAILURE;
  }
  else {
    if( log.level() <= MSG::DEBUG )
      log << MSG::DEBUG << "Insert path \"" << path << "\"" << endmsg;
    m_map[path] = de;
  }

  // insert all children
  for ( auto child = de->childBegin(); sc.isSuccess() && child != de->childEnd(); ++child ){
    sc = insert(*child,path);
    if ( ! sc.isSuccess() ) return sc;
  }
  return sc;
}
//=============================================================================
