// $Id: ContextTool.cpp,v 1.8 2008-08-04 16:31:29 pkoppenb Exp $
// Include files 

// from Gaudi
#include "GaudiKernel/ToolFactory.h" 
#include "GaudiKernel/IToolSvc.h" 

// from Event model
#include "Event/RecVertex.h" 
// from DaVinci
#include "Kernel/DVAlgorithm.h" 
#include "Kernel/IOnOffline.h" 
// local
#include "ContextTool.h"

//-----------------------------------------------------------------------------
// Implementation file for class : ContextTool
//
// 2005-06-08 : Patrick KOPPENBURG
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_TOOL_FACTORY( ContextTool );


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
ContextTool::ContextTool( const std::string& type,
                              const std::string& name,
                              const IInterface* parent )
  : GaudiTool ( type, name , parent )
  , m_desktop()
  , m_fitter(){
  declareInterface<IContextTool>(this);

}
//=============================================================================
// Destructor
//=============================================================================
ContextTool::~ContextTool() {}; 

//=============================================================================
// initialize
//=============================================================================
StatusCode ContextTool::initialize(){
  StatusCode sc = GaudiTool::initialize();
  if (!sc) return sc ;
  
  return getTools();
   
}

//=========================================================================
// get parent DVAlgorithm
//=========================================================================
const DVAlgorithm*  ContextTool::getDVAlgorithm ( )const {
  const DVAlgorithm* dvalgo = 0 ;
  const IAlgTool* atool = this ;
  // get last tool in chain
  if (msgLevel(MSG::DEBUG)) debug() << "Looking for parents of " << atool->name() << endmsg ;
  while ( 0!=dynamic_cast<const IAlgTool*>(atool->parent())){
    atool = dynamic_cast<const IAlgTool*>(atool->parent());
    if (msgLevel(MSG::DEBUG)) debug() << "... tool is owned by tool " << atool->name() << endmsg ;
  }
  // check it's not the ToolSvc
  const IToolSvc* tsvc = dynamic_cast<const IToolSvc*>( atool->parent() );
  if ( 0!=tsvc ){
    warning() << "Parent of " << atool->name() << " is the ToolSvc." << endmsg ;
    warning() << "OnOffline tool will return default tools" << endmsg ;
  } else {
    // check if it is an algorithm
    const DVAlgorithm* dvalgo = dynamic_cast<const DVAlgorithm*>( atool->parent() );
    if (0==dvalgo) warning() << "parent of " << atool->name() << " is not a DVAlgorithm" << endmsg ;
  }
  return dvalgo ;
}
//=============================================================================
// get tools
//=============================================================================
StatusCode ContextTool::getTools(){

  const DVAlgorithm* dvalgo = getDVAlgorithm() ;

  if (0!=dvalgo){
    if (msgLevel(MSG::DEBUG)) debug() << name() << " is owned by " << dvalgo->name() << endmsg ;
    m_desktop = dvalgo->desktop() ;
    if (0==m_desktop) {
      err() << "No desktop found" << endmsg;
      return StatusCode::FAILURE;
    } else if (msgLevel(MSG::DEBUG)) debug() << "Desktop of parent DVAlgorithm found" << endmsg ;
    
    m_dist = dvalgo->distanceCalculator() ;
    if (0==m_dist) {
      err() << "No DistanceCalculator found" << endmsg;
      return StatusCode::FAILURE;
    } else if (msgLevel(MSG::DEBUG)) debug() << "distanceCalculator of parent DVAlgorithm found" << endmsg ;
    
    m_fitter = dvalgo->vertexFitter() ;
    if (0==m_fitter) {
      err() << "No Vertex Fitter found" << endmsg;
      return StatusCode::FAILURE;
    } else if (msgLevel(MSG::DEBUG)) debug() << "Vertex Fitter of parent DVAlgorithm found" << endmsg ;
  }
  
  // now get default tools if needed 
  if ( 0==m_fitter ) m_fitter = tool<IVertexFit>(m_onOffline->vertexFitter(),this);
  if ( 0==m_onOffline ) m_onOffline = tool<IOnOffline>("OnOfflineTool",this);
  if ( 0==m_dist ) m_dist = tool<IDistanceCalculator>("DistanceCalculator",this);
  
  return StatusCode::SUCCESS;
}
//=============================================================================
// get PVs
//=============================================================================
LHCb::RecVertex::ConstVector ContextTool::primaryVertices() const {
  if (0!=m_desktop){
    if (msgLevel(MSG::DEBUG)) debug() << "Getting PVs from desktop" << endmsg;
    return desktop()->primaryVertices();
  }
  LHCb::RecVertex::ConstVector PV ;
  verbose() << "Will get PVs from OnOffline tool at : " << m_onOffline->getPVLocation() << endmsg ;
  if ( !exist<LHCb::RecVertices>( m_onOffline->getPVLocation() ) ) return PV ; // no PV
  LHCb::RecVertices* verts = get<LHCb::RecVertices>( m_onOffline->getPVLocation() );
  for( LHCb::RecVertices::const_iterator ivert = verts->begin();
       ivert != verts->end(); ivert++ ) {
    verbose() << "    Vertex coordinates =  "
              << (*ivert)->position()  << ", Chi^2 = " << (*ivert)->chi2()
              << endmsg;
    PV.push_back(*ivert);
  }
  return PV ;
}

//=============================================================================
// get the obsolete geom tool - load on demand only.
//=============================================================================
const IGeomDispCalculator* ContextTool::geomTool() const {

  const IGeomDispCalculator* obsoleteGeom = 0 ;
  const DVAlgorithm* dvalgo = getDVAlgorithm() ;
  if ( 0==dvalgo ){
    warning() << "Ancestor of " << name() 
              << " is not a DVAlgorithm. Gettting own instance of obsolete geometry tool." << endmsg ;
    obsoleteGeom = tool<IGeomDispCalculator>("GeomDispCalculator",this);
  } else {
    obsoleteGeom = dvalgo->obsoleteGeomDispCalculator() ;
    if (0==obsoleteGeom) {
      Exception("No geomDispCalculator found");
    } else {
      if (msgLevel(MSG::DEBUG)) debug() << "geomDispCalculator of parent DVAlgorithm found" << endmsg ;
    }
  }
  
  return obsoleteGeom;
}
