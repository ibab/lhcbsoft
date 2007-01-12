// $Id: FixedNInteractions.cpp,v 1.3 2007-01-12 15:17:36 ranjard Exp $
// Include files 

// local
#include "FixedNInteractions.h"

// from Gaudi
#include "GaudiKernel/DeclareFactoryEntries.h"

#include "GaudiKernel/SystemOfUnits.h"

//-----------------------------------------------------------------------------
// Implementation file for class : FixedNInteractions
//
// 2005-08-17 : Patrick Robbe
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory

DECLARE_TOOL_FACTORY( FixedNInteractions );


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
FixedNInteractions::FixedNInteractions( const std::string& type,
                                        const std::string& name,
                                        const IInterface* parent )
  : GaudiTool ( type, name , parent ) {
    declareInterface< IPileUpTool >( this ) ;
    declareProperty ( "NInteractions" , m_nInteractions = 1 ) ;
    declareProperty ( "Luminosity" , m_luminosity = 2.e32 / Gaudi::Units::cm2/ Gaudi::Units::s ) ;
}

//=============================================================================
// Destructor 
//=============================================================================
FixedNInteractions::~FixedNInteractions( ) { ; }

//=============================================================================
// Initialize method
//=============================================================================
StatusCode FixedNInteractions::initialize( ) {
  StatusCode sc = GaudiTool::initialize( ) ;
  if ( sc.isFailure() ) return sc ;

  if ( 1 == m_nInteractions ) 
    info() << "Single Interaction Mode" << endmsg ;
  else info() << "Fixed Number of Interactions per Event = : "
              << m_nInteractions << endmsg ;

  return sc ;
}

//=============================================================================
// Compute the number of pile up to generate according to beam parameters
//=============================================================================
unsigned int FixedNInteractions::numberOfPileUp( double & currentLuminosity ) {
  currentLuminosity = m_luminosity ;
  return m_nInteractions ;
}

//=============================================================================
// Print the specific pile up counters
//=============================================================================
void FixedNInteractions::printPileUpCounters( ) { } ;

