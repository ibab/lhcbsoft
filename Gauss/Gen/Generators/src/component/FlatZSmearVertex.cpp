// $Id: FlatZSmearVertex.cpp,v 1.2 2005-12-31 17:32:01 robbep Exp $
// Include files 

// local
#include "FlatZSmearVertex.h"

// from Gaudi
#include "GaudiKernel/ToolFactory.h"
#include "GaudiKernel/IRndmGenSvc.h" 

// from LHCb
#include "Kernel/SystemOfUnits.h"
#include "Kernel/Vector4DTypes.h"

// from Event
#include "Event/HepMCEvent.h"

//-----------------------------------------------------------------------------
// Implementation file for class : LHCbAcceptance
//
// 2005-08-17 : Patrick Robbe
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
static const  ToolFactory<FlatZSmearVertex>          s_factory ;
const        IToolFactory& FlatZSmearVertexFactory = s_factory ; 


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
FlatZSmearVertex::FlatZSmearVertex( const std::string& type,
                                    const std::string& name,
                                    const IInterface* parent )
  : GaudiTool ( type, name , parent ) {
    declareInterface< IVertexSmearingTool >( this ) ;
    declareProperty( "SigmaX" , m_sigmaX = 0.07 * mm ) ;
    declareProperty( "SigmaY" , m_sigmaY = 0.07 * mm ) ;
    declareProperty( "ZMin"   , m_zmin   = -800. * mm ) ;
    declareProperty( "ZMax"   , m_zmax   =  900. * mm ) ;
    
    declareProperty( "Xcut" , m_xcut = 4. ) ; // times SigmaX 
    declareProperty( "Ycut" , m_ycut = 4. ) ; // times SigmaY
}

//=============================================================================
// Destructor 
//=============================================================================
FlatZSmearVertex::~FlatZSmearVertex( ) { ; }

//=============================================================================
// Initialize 
//=============================================================================
StatusCode FlatZSmearVertex::initialize( ) {
  StatusCode sc = GaudiTool::initialize( ) ;
  if ( sc.isFailure() ) return sc ;
  
  IRndmGenSvc * randSvc = svc< IRndmGenSvc >( "RndmGenSvc" , true ) ;
  sc = m_gaussDist.initialize( randSvc , Rndm::Gauss( 0. , 1. ) ) ;
  if ( ! sc.isSuccess() ) 
    return Error( "Could not initialize gaussian random number generator" ) ;
  if ( m_zmin > m_zmax ) return Error( "zMin > zMax !" ) ;
  sc = m_flatDist.initialize( randSvc , Rndm::Flat( m_zmin , m_zmax ) ) ;
  if ( ! sc.isSuccess() ) 
    return Error( "Could not initialize flat random number generator" ) ;

  debug() << "Vertex smearing activated : " << endmsg ;
  debug() << "Smearing of interaction point with transverse Gaussian "
          << " distribution " << endmsg ;
  debug() << "and flat longitudinal z distribution" << endmsg ;
  debug() << " with sigma(X) = " << m_sigmaX / mm << " mm troncated at " 
          << m_xcut << " sigma(X)" << endmsg ;
  debug() << " with sigma(Y) = " << m_sigmaY / mm << " mm troncated at " 
          << m_ycut << " sigma(Y)" << endmsg ;
  debug() << " with z between " << m_zmin / mm << " mm and " 
          << m_zmax / mm << " mm." << endmsg ;

  release( randSvc ) ;
 
  return sc ;
}

//=============================================================================
// Smearing function
//=============================================================================
StatusCode FlatZSmearVertex::smearVertex( LHCb::HepMCEvent * theEvent ) {
  double dx , dy , dz ;
  
  do { dx = m_gaussDist( ) ; } while ( fabs( dx ) > m_xcut ) ;
  dx = dx * m_sigmaX ;
  do { dy = m_gaussDist( ) ; } while ( fabs( dy ) > m_ycut ) ;
  dy = dy * m_sigmaY ;
  dz = m_flatDist( ) ;

  Gaudi::LorentzVector dpos( dx / mm , dy / mm , dz / mm , 0. ) ;
  
  HepMC::GenEvent::vertex_iterator vit ;
  HepMC::GenEvent * pEvt = theEvent -> pGenEvt() ;
  for ( vit = pEvt -> vertices_begin() ; vit != pEvt -> vertices_end() ; 
        ++vit ) {
    Gaudi::LorentzVector pos ( (*vit) -> position() ) ;
    pos += dpos ;
    (*vit) -> set_position( HepLorentzVector( pos.x() , pos.y() , pos.z() ,
                                              pos.t() ) ) ;
  }

  return StatusCode::SUCCESS ;      
}

