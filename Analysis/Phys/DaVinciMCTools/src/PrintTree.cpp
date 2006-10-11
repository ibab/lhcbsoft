// $Id: PrintTree.cpp,v 1.4 2006-10-11 14:19:19 jpalac Exp $
// Include files 

// from Gaudi
#include "GaudiKernel/AlgFactory.h" 
// from DaVinci
#include "Kernel/IDebugTool.h"

// local
#include "PrintTree.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PrintTree
//
// 2004-09-10 : Patrick KOPPENBURG
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
static const  AlgFactory<PrintTree>          s_factory ;
const        IAlgFactory& PrintTreeFactory = s_factory ; 


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PrintTree::PrintTree( const std::string& name,
                      ISvcLocator* pSvcLocator)
  : DVAlgorithm ( name , pSvcLocator )
    , m_debug(0)
{

}
//=============================================================================
// Destructor
//=============================================================================
PrintTree::~PrintTree() {}; 

//=============================================================================
// Initialization
//=============================================================================
StatusCode PrintTree::initialize() {

  StatusCode sc = GaudiAlgorithm::initialize();
  if (!sc) return sc;
  
  debug() << "==> Initialize" << endmsg;
  m_debug = tool<IDebugTool>( "DebugTool", this );

  return StatusCode::SUCCESS;
};

//=============================================================================
// Main execution
//=============================================================================
StatusCode PrintTree::execute() {

  debug() << "==> Execute" << endmsg;

  // code goes here  
  LHCb::Particle::ConstVector parts = desktop()->particles();
  LHCb::Particle::ConstVector::iterator iL;
  for ( iL = parts.begin() ; iL != parts.end() ; iL++ ) {
    m_debug->printTree( (*iL) );
  }// - loop
  if (!parts.empty()) setFilterPassed(true); 
  else {
    setFilterPassed(false);  
    debug() << "No particles found to print in a tree" << endreq;
  }
  
  return StatusCode::SUCCESS;
};


//=============================================================================
