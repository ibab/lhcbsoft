// $Id: MuonPIDCloner.cpp,v 1.1 2010-08-13 13:21:08 jpalac Exp $
// Include files 

// from Gaudi
#include "GaudiKernel/ToolFactory.h" 

// from LHCb
#include "Event/MuonPID.h"

// local
#include "MuonPIDCloner.h"

//-----------------------------------------------------------------------------
// Implementation file for class : MuonPIDCloner
//
// 2008-04-01 : Juan PALACIOS
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_TOOL_FACTORY( MuonPIDCloner );


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
MuonPIDCloner::MuonPIDCloner( const std::string& type,
                              const std::string& name,
                              const IInterface* parent )
  : 
  base_class ( type, name , parent )
{
}
//=============================================================================
StatusCode MuonPIDCloner::initialize() 
{

  debug() << "==> Initialize" << endmsg;

  StatusCode sc = base_class::initialize();
  
  if (! sc.isSuccess() ) return sc;

  return StatusCode::SUCCESS;
}
//=============================================================================
LHCb::MuonPID* MuonPIDCloner::operator() (const LHCb::MuonPID* pid)
{
  return this->clone(pid);
}
//=============================================================================
LHCb::MuonPID* MuonPIDCloner::clone(const LHCb::MuonPID* pid)
{

  if (!pid) return 0;

  LHCb::MuonPID* pidClone = 
    cloneKeyedContainerItem<BasicMuonPIDCloner>(pid);

  LHCb::Track* idTrackClone = 
    cloneKeyedContainerItem<BasicTrackCloner>(pid->idTrack());

  LHCb::Track* muonTrackClone = 
    cloneKeyedContainerItem<BasicTrackCloner>(pid->muonTrack());

  pidClone->setIDTrack(idTrackClone);
  
  pidClone->setMuonTrack(muonTrackClone);

  return pidClone;
  
}
//=============================================================================
// Destructor
//=============================================================================
MuonPIDCloner::~MuonPIDCloner() {} 

//=============================================================================
