// $Id: RecInit.cpp,v 1.3 2006-09-28 13:23:00 cattanem Exp $
// Include files 

// from Gaudi
#include "GaudiKernel/AlgFactory.h" 

// from LHCbKernel
#include "Kernel/IGenericTool.h"

// from EventBase
#include "Event/ProcessHeader.h"

// from DAQEvent
#include "Event/RawEvent.h"
#include "Event/RawBank.h"
#include "Event/ODIN.h"

// from RecEvent
#include "Event/RecHeader.h"
#include "Event/ProcStatus.h"

// local
#include "RecInit.h"

//-----------------------------------------------------------------------------
// Implementation file for class : RecInit
//
// 2006-03-14 : Marco Cattaneo
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( RecInit );


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
RecInit::RecInit( const std::string& name,
                  ISvcLocator* pSvcLocator)
  : LbAppInit ( name , pSvcLocator )
{

}
//=============================================================================
// Destructor
//=============================================================================
RecInit::~RecInit() {} 

//=============================================================================
// Initialization
//=============================================================================
StatusCode RecInit::initialize() {
  StatusCode sc = LbAppInit::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  debug() << "==> Initialize" << endmsg;

  // Private tool to plot the memory usage
  m_memoryTool = tool<IGenericTool>( "MemoryTool", "BrunelMemory", this, true );

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode RecInit::execute() {

  StatusCode sc = LbAppInit::execute(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by LbAppInit

  debug() << "==> Execute" << endmsg;
  // Plot the memory usage
  m_memoryTool->execute();

  // Get the run and event number from the ODIN bank
  LHCb::ODIN* odin = get<LHCb::ODIN> ( LHCb::ODINLocation::Default );

  unsigned int runNumber = odin->runNumber();
  ulonglong    evtNumber = odin->eventNumber();
  
  this->printEventRun( evtNumber, runNumber );

  // Initialize the random number
  std::vector<long int> seeds = getSeeds( runNumber, evtNumber );
  sc = this->initRndm( seeds );
  if ( sc.isFailure() ) return sc;  // error printed already by initRndm


  // Create the Reconstruction event header
  LHCb::RecHeader* header = new LHCb::RecHeader();
  header->setApplicationName( this->appName() );
  header->setApplicationVersion( this->appVersion() );
  header->setRunNumber( runNumber );
  header->setEvtNumber( evtNumber );
  header->setRandomSeeds( seeds );
  put( header, LHCb::RecHeaderLocation::Default );

  // Create a ProcStatus if it does not already exist
  if( !exist<LHCb::ProcStatus>( LHCb::ProcStatusLocation::Default ) ) {
    LHCb::ProcStatus* procStat = new LHCb::ProcStatus();
    put( procStat, LHCb::ProcStatusLocation::Default );
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
