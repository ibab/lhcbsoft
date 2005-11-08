// $Id: EvtGenDecay.cpp,v 1.2 2005-11-08 00:07:34 robbep Exp $
// Header file
#include "EvtGenDecay.h"

// Include files
#include <iostream>
#include <fstream>

// from Gaudi
#include "GaudiKernel/ToolFactory.h"
#include "GaudiKernel/IRndmGenSvc.h"
#include "GaudiKernel/IRndmGen.h"
#include "GaudiKernel/IParticlePropertySvc.h"
#include "GaudiKernel/ParticleProperty.h"
#include "GaudiKernel/GaudiException.h"

// from CLHEP
#include "CLHEP/Units/SystemOfUnits.h"
#include "EvtGenModels/EvtPythia.hh"
#include "EvtGenBase/EvtParticleFactory.hh"
#include "EvtGenBase/EvtDecayTable.hh"
#include "EvtGenBase/EvtDecayBase.hh"

// Calls to FORTRAN routines
#ifdef WIN32
extern "C" {
  void __stdcall PHOTOS_INIT() ;
  void __stdcall PYTHIAOUTPUT_INIT( int * ) ;
  void __stdcall PHOTOS_END () ;
}
#else
extern "C" {
  void photos_init_() ;
  void pythiaoutput_init_( int * ) ;
  void photos_end_()  ;
}
#endif

//-----------------------------------------------------------------------------
// Implementation file for class : EvtGenDecay
//
// 2003-10-15 : Patrick Robbe
//-----------------------------------------------------------------------------
// Declaration of the Tool Factory
static const  ToolFactory<EvtGenDecay>          s_factory ;
const        IToolFactory& EvtGenDecayFactory = s_factory ;

extern MsgStream * evtgenStream ;

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
EvtGenDecay::EvtGenDecay( const std::string& type,
                        const std::string& name,
                        const IInterface* parent )
  : GaudiTool ( type, name , parent ),
    m_gen(0),
    m_randomEngine(0),
    // Minimum value for ctau in particle property data.
    // Below, it is set to 0
    m_minctau( 1.e-4 * mm ) ,
    // Maximum value for ctau in particle property data.
    // Above, it is set to 0
    m_maxctau( 1.e+16 * mm ) ,
    // Minimum value for Gamma in particle property data.
    // Below, it is set to 0
    m_minwidth( 1.5e-6 * GeV ) {
    // Declare IEvtGenDecay interface
    declareInterface<IDecayTool>( this ) ;
    // Declare properties for EvtGen
    // The generic decay file
    declareProperty("DecayFile" , m_decayFile = "empty" ) ;
    // The signal decay file
    declareProperty("UserDecayFile" , m_userDecay = "empty" ) ;
    // Keep temporary evt.pdl file
    declareProperty("KeepTempEvtFile" , m_keepTempEvtFile = false ) ;
    // Generate Polarized Lambda_b decays
    declareProperty("PolarizedLambdad" , m_generatePolLambdab = false ) ;
    
  // Initialize signalId
  m_signalId = EvtId( -1, -1 ) ;
}
//=============================================================================
// Destructor
//=============================================================================
EvtGenDecay::~EvtGenDecay( ) { }
//=============================================================================
// Initialize method
//=============================================================================
StatusCode EvtGenDecay::initialize( ) {
  MsgStream msg ( msgSvc() , name() ) ;
  evtgenStream = new MsgStream( msg ) ;
  StatusCode sc = GaudiTool::initialize( ) ;
  if ( sc.isFailure() ) return Error( "Base class is not initalized" , sc ) ;

  // Find Generic DECAY.DEC file
  // Default location (if not specified in job options is
  // $DECFILESROOT/dkfiles/DECAY.DEC
  if ( m_decayFile.empty() || "empty" == m_decayFile ) {
    if ( 0 != getenv("DECFILESROOT") ) {
      m_decayFile  = getenv( "DECFILESROOT" ) ;
      m_decayFile += "/dkfiles/DECAY.DEC" ;
    }
  }

  if ( m_decayFile.empty() || "empty" == m_decayFile )
    return Error("DECAY.DEC location name not set. Data will not be found.");

  // create temporary evt.pdl file filled with Gaudi ParticlePropertySvc
  std::string tempEvtPdlFile( "tempPdlFile.txt" ) ;
  sc = createTemporaryEvtFile( tempEvtPdlFile ) ;
  if ( ! sc.isSuccess() )
    return Error ( std::string( "unable to create temporary evt.pdl " ) +
                   std::string( "file from Particle Property Service." ) ,
                   sc ) ;

  // create random engine for EvtGen
  IRndmGenSvc * randSvc( 0 ) ;
  try { randSvc = svc< IRndmGenSvc >( "RndmGenSvc" , true ) ; }
  catch ( const GaudiException & exc ) {
    Exception( "RndmGenSvc not found to initialize EvtGen random engine" ) ;
  }

  StatusCode resultCode ;
  m_randomEngine = new EvtGenGaudiRandomEngine( randSvc , resultCode ) ;
  if ( ! resultCode.isSuccess() )
    return Error( "Cannot initialize EvtGenGaudiRandomService" , resultCode ) ;
  release( randSvc ) ;

  // create EvtGen engine from decay file, evt.pdl file and random engine
  m_gen = new EvtGen ( m_decayFile.c_str() , tempEvtPdlFile.c_str() ,
                       m_randomEngine ) ;
  (*evtgenStream) << endreq ;

  // Remove temporary file if not asked to keep it
  if ( ! m_keepTempEvtFile ) {
    std::string delcmd ( "rm -f " ) ;
    delcmd += tempEvtPdlFile ;
#ifndef WIN32
    system( delcmd.c_str() ) ;
#endif
  }

  // Read the optional signal decay file
  if ( ! m_userDecay.empty() && "empty" != m_userDecay )
    m_gen -> readUDecay( m_userDecay.c_str() ) ;
  (*evtgenStream) << endreq ;

  int arg ;
  if ( msgLevel( MSG::DEBUG ) ) {
    arg = 0 ;
#ifdef WIN32
    PYTHIAOUTPUT_INIT( &arg ) ;
#else
    pythiaoutput_init_( &arg ) ;
#endif
  } else {
    arg = 1 ;
#ifdef WIN32
    PYTHIAOUTPUT_INIT( &arg ) ;
    PHOTOS_INIT() ;
#else
    pythiaoutput_init_( &arg ) ;
    photos_init_();
#endif
  }
  
  // Initialize Pythia
  EvtPythia::pythiaInit( 0 ) ;
  (*evtgenStream) << endreq ;

  debug() << "EvtGenDecay initialized" << endreq ;
  
  return StatusCode::SUCCESS ;
}

//=============================================================================
// Finalize method
//=============================================================================
StatusCode EvtGenDecay::finalize() {
  delete m_gen ;
  (*evtgenStream) << endreq ;
  delete m_randomEngine ;
  
  debug() << "EvtGenDecay finalized" << endreq ;

  if ( ! msgLevel( MSG::DEBUG ) ) {
#ifdef WIN32
    PHOTOS_END() ;
#else
    photos_end_() ;
#endif
  }  
 
	std::string delcmd ( "rm -f " ) ;
	delcmd += "photos_init.tmp" ;
#ifndef WIN32
	system( delcmd.c_str() ) ;
#endif
	
  delete evtgenStream ;

  return GaudiTool::finalize( ) ;
}

//=============================================================================
// Generate a Decay tree from a particle theMother in the event theEvent
//=============================================================================
StatusCode EvtGenDecay::generateDecay( HepMC::GenEvent * theEvent ,
                                       HepMC::GenParticle * theMother ) const {
  // Call EvtGen for the particle to generate
  checkParticle( theMother ) ;

  EvtParticle * part( 0 ) ;
  StatusCode sc = callEvtGen( part , theMother , EvtId( -1, -1 ) ) ;
  if ( ! sc.isSuccess( ) ) return sc ;

  // Update HepMCEvent theEvent and HepMCParticle theMother

  // sets PDG Id of theMother
  // because EvtGen might have asked to change the original one
  // for CP modes (if flip is enabled)
  theMother -> set_pdg_id( EvtPDL::getStdHep( part->getId() ) ) ;

  // Get reference position in space and time to be able to assign
  // correct vertex for daughter particles of theMother
  // because EvtGen gives position with respect to the "root" particle
  // This reference position is production vertex of theMother
  HepLorentzVector theOriginPosition = 
    theMother -> production_vertex() -> position() ;

  // Fill HepMC event theEvent with EvtGen decay tree part
  // starting from theMother
  makeHepMC( part , theEvent , theMother , theOriginPosition , -999 ) ;

  // delete EvtGen particle and all its daughters
  part -> deleteTree ( ) ;

  // Set status to 888
  theMother -> set_status( 888 ) ;

  return StatusCode::SUCCESS ;
}

//=============================================================================
// Generate a Decay tree from a particle theMother in the event theEvent
//=============================================================================
StatusCode EvtGenDecay::generateSignalDecay( HepMC::GenEvent * theEvent ,
                                             HepMC::GenParticle * theMother ,
                                             bool & flip) const {
  // Call EvtGen for the particle to generate
  checkParticle( theMother ) ;

  // get signal alias
  EvtId decayId = getSignalAlias( theMother -> pdg_id() ) ;

  EvtParticle * part( 0 ) ;
  StatusCode sc = callEvtGen( part , theMother , decayId ) ;
  if ( ! sc.isSuccess( ) ) return sc ;

  // Update HepMCEvent theEvent and HepMCParticle theMother

  // sets PDG Id of theMother
  // because EvtGen might have asked to change the original one
  // for CP modes (if flip is enabled)
  theMother -> set_pdg_id( EvtPDL::getStdHep( part->getId() ) ) ;
  flip = ( decayId != part -> getId() ) ;

  // Get reference position in space and time to be able to assign
  // correct vertex for daughter particles of theMother
  // because EvtGen gives position with respect to the "root" particle
  // This reference position is production vertex of theMother
  HepLorentzVector theOriginPosition = 
    theMother -> production_vertex() -> position() ;

  // Fill HepMC event theEvent with EvtGen decay tree part
  // starting from theMother
  makeHepMC( part , theEvent , theMother , theOriginPosition , -999 ) ;

  // delete EvtGen particle and all its daughters
  part -> deleteTree ( ) ;

  // Set status to 889
  theMother -> set_status( 889 ) ;

  return StatusCode::SUCCESS ;
}

//=============================================================================
// Generate a Decay tree from a particle theMother in the event theEvent
// It stops when the particle to decay has a mass larger than theMassLimit
// or is of the type targetId
//=============================================================================
StatusCode EvtGenDecay::generateDecayWithLimit( HepMC::GenEvent * theEvent ,
                                               HepMC::GenParticle * theMother ,
                                               int targetId ) const {
  checkParticle( theMother ) ;

  EvtParticle * part( 0 ) ;
  StatusCode sc = callEvtGen( part , theMother , EvtId( -1 , -1 ) ) ;
  if ( ! sc.isSuccess() ) return sc ;
  
  // Update HepMCEvent theEvent and HepMCParticle theMother

  // sets PDG Id of theMother
  // because EvtGen might have asked to change the original one
  // for CP modes (if flip is enabled)
  theMother -> set_pdg_id( EvtPDL::getStdHep( part->getId() ) ) ;

  // Get reference position in space and time to be able to assign
  // correct vertex for daughter particles of theMother
  // because EvtGen gives position with respect to the "root" particle
  // This reference position is production vertex of theMother  
  HepLorentzVector theOriginPosition = 
    theMother -> production_vertex() -> position() ;

  // Fill HepMC event theEvent with EvtGen decay tree part
  // starting from theMother and stopping at particle with pdgId
  // equals to targetId
  makeHepMC( part , theEvent , theMother , theOriginPosition , targetId ) ;
  
  // delete EvtGen particle and all its daughters
  part -> deleteTree ( ) ;

  // Set particle status to 888
  theMother -> set_status( 888 ) ;

  return StatusCode::SUCCESS ;
}

//=============================================================================
// Update the HepMC tree ( theMother ) with a EvtGen tree ( the EvtGenPart)
// in a given HepMC event ( theEvent ). The origin is the production 
// position of the root particle in the decay tree. The conversion will stop
// when the particle is of requested type targetId (if the value is negative
// -- which is the default value -- it will be ignored and all decay chain
// will be converted)
//=============================================================================
StatusCode EvtGenDecay::makeHepMC( EvtParticle * theEvtGenPart,
                                  HepMC::GenEvent * theEvent ,
                                  HepMC::GenParticle * theMother ,
                                  const HepLorentzVector & theOrigin ,
                                  int targetId ) const {
  if ( 0 != theEvtGenPart->getNDaug() ) {
    // Find the vertex
    // In EvtGen all positions are defined with respect to the
    // root mother particle whose production vertex is theOrigin
    EvtVector4R position = theEvtGenPart -> getDaug( 0 ) -> get4Pos() ;
    double ct = position . get( 0 ) + theOrigin.t() ;
    double x  = position . get( 1 ) + theOrigin.x() ;
    double y  = position . get( 2 ) + theOrigin.y() ;
    double z  = position . get( 3 ) + theOrigin.z() ;

    // Create a new vertex corresponding to the decay vertex of
    // theMother and add it to theEvent
    HepMC::GenVertex * end_vtx = 
      new HepMC::GenVertex( HepLorentzVector( x , y , z , ct ) ) ;
    theEvent->add_vertex( end_vtx ) ;
    end_vtx->add_particle_in( theMother ) ;
    
    for ( int it = 0 ; it < theEvtGenPart -> getNDaug() ; ++it ) {
      // For each daughter create a new HepMCParticle with correct
      // 4 momentum and PDG Id and with status 777 (= decayed with EvtGen)
      EvtVector4R momentum = theEvtGenPart -> getDaug( it ) -> getP4Lab() ;
      double e  = momentum . get( 0 ) ;
      double px = momentum . get( 1 ) ;
      double py = momentum . get( 2 ) ;
      double pz = momentum . get( 3 ) ;
      
      int id = EvtPDL::getStdHep( theEvtGenPart->getDaug( it )->getId() ) ;
      int status = 777 ;
      
      HepMC::GenParticle * prod_part = new
        HepMC::GenParticle( HepLorentzVector(px,py,pz,e) , id , status ) ;
      
      end_vtx->add_particle_out( prod_part ) ;
      
      // Fill event with the daughter (recursive procedure)
      // if the particle is not of requested type
      if ( abs( id ) != targetId )
          makeHepMC( theEvtGenPart->getDaug(it) , theEvent , prod_part , 
                     theOrigin , targetId ) ;
      // otherwise (particle has targetId pdg ID) it can be decayed further
      // by EvtGen (it is the signal particle in pratice) so give it 
      // status 888 unless it is a stable particle and then give it status 999
      else 
        {
          if ( theEvtGenPart->getDaug(it)->getNDaug() > 0 ) 
            prod_part -> set_status ( 888 ) ;
          else prod_part -> set_status( 999 ) ;
        }
    }
  }
  else {
    theMother -> set_status( 999 ) ;
  }
  
  return StatusCode::SUCCESS ;
}

//=============================================================================
// Check if given Pdg Id is defined in the decay table
//=============================================================================
bool EvtGenDecay::isKnownToDecayTool( int pdgId ) const {
  EvtId id = EvtPDL::evtIdFromStdHep( pdgId ) ;
  if ( id == EvtId(-1,-1) ) return false ;
  if ( 0  == EvtDecayTable::getNMode( id.getAlias() ) ) return false ;
  return true ;
}

//=============================================================================
// Create a temporary evt.pdl file taking available informations
// from the Particle Property Service
//=============================================================================
StatusCode EvtGenDecay::createTemporaryEvtFile( const std::string& newFile ) 
const {
  // Create temporary file
  std::ofstream g( newFile.c_str() ) ;
  if ( ! g.is_open() ) return Error( "Cannot create file " + newFile ) ;

  // Write description of the file columns
  // Lines beginning with * are comments
  g << "* " << std::endl 
    << "*                  name                  id      mass/GeV    "
    << "width/GeV     "
    << "max_Dm/GeV    3*charge   2*spin    lifetime*c/mm    PythiaId   " 
    << std::endl ;

  g << std::left << std::fixed ;
  
  // retrieve Gaudi particle property service  
  IParticlePropertySvc * ppSvc( 0 ) ;
  try { ppSvc = 
          svc< IParticlePropertySvc > ( "ParticlePropertySvc" , true ) ; }
  catch ( const GaudiException & exc ) {
    Exception( "Cannot open ParticlePropertySvc to fill EvtGen" , exc ) ;
  }

  // Loop over particle properties
  // and make a evt.pdl file with them

  // evry line starts with this command
  std::string cmnd = "add  p Particle " ;
  double      mass , ctau , pwidth ;
  int         charge , spin2 ;
  
  for ( IParticlePropertySvc::const_iterator i = ppSvc->begin() ;
        i != ppSvc->end() ; ++i ) {
    if ( 0 == *i ) continue ;
    // if particle is not known to EvtGen, skip it
    if ( "unknown" == (*i)->evtGenName() ) continue ;

    g << cmnd << " " << std::scientific ;
    // Particle Name (EvtGen Name)
    g << std::setw( 22 ) << std::left << (*i) -> evtGenName ( ) << " " ;
    // PDG Id
    g << std::right << std::setw( 13 ) << (*i)->jetsetID() << " " ;
    // Mass in GeV 
    mass   = (*i) -> mass() / GeV ;
    // ctau in mm 
    ctau   = (*i) -> lifetime() * EvtConst::c /s ;
    // Apply limits for ctau. If outside limits, sets it to 0
    // (that is to say 0 lifetime particle or stable particle)
    if ( ( ctau * mm < m_minctau ) || ( ctau * mm > m_maxctau ) ) 
      ctau = 0. ;
    // Width in GeV 
    if ( (*i) -> lifetime() > 0. ) {
      pwidth = ( hbarc / ( (*i) -> lifetime() * c_light ) ) / GeV ; 
    } else {
      pwidth = 0. ;
    }
    // Apply limit for the width. If width is below the limit,
    // set it to 0 and do not generate a Breit-Wigner lineshape
    // for this particle
    if ( pwidth * GeV < m_minwidth ) pwidth = 0. ;
  
    g << std::setw( 14 ) << std::setprecision( 7 ) << mass << " " ;
    g << std::setw( 14 ) << std::setprecision( 7 ) << pwidth << " " ;
    // Max width deviation in GeV 
    g << std::setw( 14 ) << std::setprecision( 7 ) 
      << (*i) -> maxWidth() / GeV << " " ;
    // 3 times particle charge
    charge = (int) floor( 3 * (*i) -> charge( ) + 0.5 ) ;
    g << std::setw( 5 ) << charge << " " ;
    // 2 times particle spin
    ParticleID pid ( (*i)->jetsetID() ) ;
    if ( pid.jSpin() > 0 ) spin2 = pid.jSpin() - 1 ;
    else spin2 = getParticleSpin( pid ) - 1 ;
    g << std::setw( 5 ) << spin2 << " " ;
    // ctau
    g << std::setw( 14 ) << std::setprecision( 7 )
      << ctau << " " ;
    // PythiaID (ID with which Pythia knows the particle)
    // enables EvtGen to update particle masses and width in Pythia
    // common blocks
    g << std::setw( 10 ) << (*i) -> pythiaID() << std::left << std::endl ;
  }

  // File must en with key word "end"
  g << "end" << std::endl ;
  g.close() ;
  release( ppSvc ) ;

  return StatusCode::SUCCESS ;
}

//============================================================================
// Return the alias of the signal particle corresponding to the pdg Id
//============================================================================
const EvtId EvtGenDecay::getSignalAlias( int pdgId ) const {
  EvtId trueId = EvtPDL::evtIdFromStdHep( pdgId ) ;
  if ( EvtPDL::getStdHep( trueId ) == EvtPDL::getStdHep( m_signalId ) ) 
    return m_signalId ;
  else if ( EvtPDL::getStdHep( trueId ) == 
            EvtPDL::getStdHep( EvtPDL::chargeConj( m_signalId ) ) )
    return EvtPDL::chargeConj( m_signalId ) ;
  
  Exception( "There is no signal ID corresponding to the pdgId" ) ;
  return m_signalId ;
}

//============================================================================
// Return 2J+1 for particles which are not supported by ParticleID
//============================================================================
int EvtGenDecay::getParticleSpin( const ParticleID & pid ) const {
  // leptons
  if ( pid.isLepton() ) return 2 ;
  // quarks
  if ( 8 >= pid.abspid() ) return 2 ;
  // photon
  if ( 22 == pid.abspid() ) return 3 ;
  // He3 et Tritium
  if ( 490000000 == pid.abspid() ) return 2 ;
  if ( 460000000 == pid.abspid() ) return 2 ;
  // Cerenkov
  if ( 20022 == pid.abspid() ) return 3 ;
  // W+, W- and Z0
  if ( ( 23 == pid.abspid() ) || ( 24 == pid.abspid() ) ) return 3 ;
  // gluon or vpho
  if ( ( 21 == pid.abspid() ) || ( 10022 == pid.abspid() ) ) return 3 ;
  // Z', W' 
  if ( ( 32 == pid.abspid() ) || ( 33 == pid.abspid() ) ||
       ( 34 == pid.abspid() ) || ( 41 == pid.abspid() ) ) return 3 ;

  return 1 ;
}

//=============================================================================
// get the total branching fraction of the signal decay mode in the generic
// decay table.
//=============================================================================
double EvtGenDecay::getSignalBr( ) const {
  if ( EvtId( -1, -1) == m_signalId ) return 0. ; 
  if ( ! checkSignalPresence( ) ) return 0. ;
  return branching( m_signalId ) ;
}

//=============================================================================
// get branching of EvtId id in the generic decay table
//=============================================================================
double EvtGenDecay::branching( const EvtId& id ) const {
  double result = 0. ;

  if ( ! id.isAlias( ) ) return 1. ;

  for ( int i = 0 ; i < EvtDecayTable::getNMode( id.getAlias() ) ; ++i ) {
    // find corresponding decay mode in generic table
    EvtDecayBase * theDecAlias = EvtDecayTable::getDecay( id.getAlias() , i ) ;
    EvtId daugs_scratch[ 50 ] ;
    if ( theDecAlias -> getNDaug( ) > 50 ) return 1. ;
    for ( int k = 0 ; k < theDecAlias -> getNDaug( ) ; ++k ) {
      daugs_scratch[ k ] = EvtId ( theDecAlias -> getDaugs()[ k ].getId() ,
                                   theDecAlias -> getDaugs()[ k ].getId() ) ;
    }
    
    int index = EvtDecayTable::inChannelList( EvtId( id.getId(), id.getId() ),
                                           theDecAlias -> getNDaug( ) ,
                                           daugs_scratch ) ;
    EvtDecayBase * theTrueDecay = 
      EvtDecayTable::getDecay( id.getId() , index ) ;

    double tempBr = 1. ;
    
    for ( int j = 0 ; j < theDecAlias -> nRealDaughters( ) ; ++j ) {
      tempBr *= branching( theDecAlias -> getDaugs( )[ j ] ) ;
    }

    result += theTrueDecay -> getBranchingFraction( ) /
      EvtDecayTable::totalBr( id.getId( ) ) * tempBr ;
  }

  return result ;
}

//=============================================================================
// check if the signal decay mode is in the generic decay table.
//=============================================================================
bool EvtGenDecay::checkSignalPresence( ) const {
  if ( EvtId( -1 , -1 ) == m_signalId ) return false ;
  return checkGeneric( m_signalId ) ;
}

//=============================================================================
// Check if id exist in the generic decay table
//=============================================================================
bool EvtGenDecay::checkGeneric( const EvtId& id ) const {
  bool result = true ;
  
  if ( ! id.isAlias( ) ) return true ;
  
  for ( int i = 0 ; i < EvtDecayTable::getNMode( id.getAlias() ) ; ++i ) {
    EvtDecayBase * theDecAlias = EvtDecayTable::getDecay( id.getAlias() , i ) ;
    
    if ( theDecAlias -> getNDaug( ) > 50 ) return false ;
    EvtId daugs_scratch[ 50 ] ;
    for ( int k = 0 ; k < theDecAlias -> getNDaug( ) ; ++k ) {
      daugs_scratch[ k ] = EvtId ( theDecAlias -> getDaugs()[ k ].getId() ,
                                   theDecAlias -> getDaugs()[ k ].getId() ) ;
    }

    int index = EvtDecayTable::inChannelList( EvtId( id.getId(), id.getId() ),
                                              theDecAlias -> getNDaug( ) ,
                                              daugs_scratch ) ;

    if ( -1 == index ) return false ;
    
    for ( int j = 0 ; j < theDecAlias -> nRealDaughters( ) ; ++j ) {
      result = result && checkGeneric( theDecAlias -> getDaugs( ) [ j ] ) ;
    }
  }
  
  return result ;    
}


//=============================================================================
// Check if the HepMC particle is a good candidate
//=============================================================================
void EvtGenDecay::checkParticle( const HepMC::GenParticle * thePart ) const {
  if ( 0 == thePart -> production_vertex() ) 
    Exception( "Trying to decay a particle without production vertex" ) ;
  if ( 0 != thePart -> end_vertex ( ) ) 
    Exception( std::string( "Trying to add an end vertex to a " ) +
               std::string( "particle which has already one !" ) ) ;
}
  
//=============================================================================
// Generate with EvtGen
//=============================================================================
StatusCode EvtGenDecay::callEvtGen( EvtParticle * &thePart , 
                                   const HepMC::GenParticle * theHepMCPart ,
                                   const EvtId & eid ) 
const {
  // Get parameters of particle to generate ( theHepMCPart )
  // Type of particle and 4-momentum
  // The mother is created outside EvtGenDecay
  HepLorentzVector lVect = theHepMCPart -> momentum() ;
  
  EvtVector4R p_init ( lVect.e() , lVect.px() , lVect.py() , lVect.pz() ) ;
  
  // Create the corresponding EvtGen particle
  // If eid is not specified (no alias) take PID from theHepMCPart
  if ( eid == EvtId( -1, -1 ) ) {
    EvtId pid = EvtPDL::evtIdFromStdHep( theHepMCPart -> pdg_id() ) ;
    thePart   =  EvtParticleFactory::particleFactory( pid , p_init ) ;
  }
  else 
    thePart = EvtParticleFactory::particleFactory( eid , p_init ) ; 
 
  if ( 0 == thePart ) 
    return Error( std::string( "Could not create EvtParticle. \n" ) +
                  std::string( "There must be a configuration error." ) ) ;
  
  // sets diagonal polarization matrix for unpolarized decays for the
  // moment
  // or generate Polarized Lambda_b if requested
  if ( ( m_generatePolLambdab) && 
       ( abs( theHepMCPart -> pdg_id() ) == 5122 ) ) {
    EvtSpinDensity rho ;
    rho.SetDim( 2 ) ;
    rho.Set( 0 , 0 , 1.0 ) ;
    rho.Set( 0 , 1 , 0.0 ) ;
    rho.Set( 1 , 0 , 0.0 ) ;
    rho.Set( 1 , 1 , 0.0 ) ;
    thePart -> setSpinDensityForwardHelicityBasis( rho ) ;
  } else {
    thePart -> setDiagonalSpinDensity( ) ;
  }

  // Generate decay tree of part with EvtGen  
  m_gen -> generateDecay( thePart ) ;
  (*evtgenStream) << endreq ;
  thePart -> printTree() ;
  (*evtgenStream) << endreq ;

  return StatusCode::SUCCESS ;
}

//=============================================================================
// Set signal ID
//=============================================================================
void EvtGenDecay::setSignal( const int pdgSignal ) 
{
  EvtId theSignal = EvtPDL::evtIdFromStdHep( abs( pdgSignal ) ) ;
  std::string nameSignal = EvtPDL::name( theSignal ) + "sig" ;
  m_signalId = EvtPDL::getId( nameSignal ) ;
}

//=============================================================================
// Implementation of class EvtGenGaudiRandomEngine
//=============================================================================
//=============================================================================
// Constructor
//=============================================================================
EvtGenGaudiRandomEngine::EvtGenGaudiRandomEngine( IRndmGenSvc* i , 
                                                  StatusCode & sc ) {
  // Initialize Random Engine with flat generator
  sc = m_randomgaudi.initialize( i , Rndm::Flat( 0 , 1 ) ) ;      
}

//=============================================================================
// Destructor
//=============================================================================
EvtGenGaudiRandomEngine::~EvtGenGaudiRandomEngine( ) { }

