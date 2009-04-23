// $Id: NoPIDsParticleMaker.cpp,v 1.16 2009-04-23 10:39:31 pkoppenb Exp $
// Include files 

// from Gaudi
#include "GaudiKernel/DeclareFactoryEntries.h" 

// local
#include "NoPIDsParticleMaker.h"
using namespace Gaudi::Units;

//-----------------------------------------------------------------------------
// Implementation file for class : NoPIDsParticleMaker
//
// 2006-01-23 : I. Belyaev
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_ALGORITHM_FACTORY( NoPIDsParticleMaker );


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
NoPIDsParticleMaker::NoPIDsParticleMaker(  const std::string& name,ISvcLocator* pSvcLocator )
  : ChargedParticleMakerBase ( name , pSvcLocator )
  , m_CL     ( 50 * perCent  ) 
  , m_calls  ( 0 ) 
  , m_sum    ( 0 ) 
  , m_sum2   ( 0 )
  , m_longTracks ( true )
  , m_downstreamTracks ( false )  // set to false for HLT
  , m_vttTracks ( false )         // set to false for HLT
  , m_veloTracks ( false )         // 
{
  declareProperty ( "CL"       , m_CL     ) ; 
  declareProperty ( "UseLongTracks",     m_longTracks );
  declareProperty ( "UseDownstreamTracks", m_downstreamTracks );
  declareProperty ( "UseUpstreamTracks",      m_vttTracks );
  declareProperty ( "UseVeloTracks",      m_veloTracks );

}
//=============================================================================
// Destructor
//=============================================================================
NoPIDsParticleMaker::~NoPIDsParticleMaker() {} 

//=============================================================================
// Destructor
//=============================================================================
StatusCode NoPIDsParticleMaker::initialize() {
  StatusCode sc = ChargedParticleMakerBase::initialize();

  if ( !m_longTracks ) info() << "Filtering out long tracks"<< endmsg;
  if ( !m_downstreamTracks ) info() << "Filtering out downstream tracks" << endmsg;
  if ( !m_vttTracks ) info() << "Filtering out upstream tracks" << endmsg;
  if ( m_veloTracks ) info() << "Keeping velo tracks" << endmsg;
  if (!(( m_longTracks ) || ( m_downstreamTracks) || ( m_vttTracks)|| ( m_veloTracks))){
    Warning("You have chosen to veto all tracks.") ;
  }

  return StatusCode::SUCCESS;
  
} 

// ============================================================================
StatusCode NoPIDsParticleMaker::finalize() 
{
  const std::string stars ( 80 , '*' );
  info() << stars << endmsg ;
  
  if( 0 != m_ppSvc    ) { m_ppSvc  = 0 ; }
  
  double mean  = m_sum ;
  if ( 0 != m_calls  ) { mean  = m_sum  / m_calls                ; }
  double sigma = 0     ;
  if ( 0 != m_calls  ) { sigma = m_sum2 / m_calls - mean * mean  ; }
  if ( sigma > 0     ) { sigma = sqrt( sigma )                   ; }
  
  always()
    << " created '"    << m_pid 
    << "' and '"       << m_apid  
    << " : "           << m_sum
    << " per "         << m_calls
    << " calls ("      << mean 
    << "+-"            << sigma 
    << ")/event"       << endmsg ;
  // finalize the base 
  return ChargedParticleMakerBase::finalize ();
};
//=============================================================================
// Dispatch the making of particles 
//=============================================================================
StatusCode NoPIDsParticleMaker::makeParticles
( LHCb::Particle::Vector & particles ){
  
  // increase the counter 
  ++m_calls ; 
  
  size_t number = 0 ;
  // get all input data
  const LHCb::ProtoParticles* pps = protos() ;
  // loop over all protoparticles 
  for(LHCb:: ProtoParticles::const_iterator ipp = pps->begin() ; 
      pps->end() != ipp ; ++ipp ) 
  {
    const LHCb::ProtoParticle* pp = *ipp ;
    if ( 0 == pp                ) { continue ; }              // CONTINUE
      if ( 0 == pp -> charge ()   ) { continue ; }              // CONTINUE
      
      const LHCb::Track* ptrack = pp->track();
      if ( 0==ptrack ) {
        Warning("Charged protoparticle with no Track found. Ignoring.");
        continue ;
      }
      // Select tracks
      if (msgLevel(MSG::VERBOSE)) verbose() << "Trying Track " << ptrack->key() << endmsg;
      if ( !trSel()->accept(*ptrack) ) continue;
      if (msgLevel(MSG::VERBOSE)) {
        verbose() << " -> Track selected " << ptrack->key()  
                  << " " << ptrack->firstState().momentum() << endmsg;
      }
      LHCb::Particle* particle = new LHCb::Particle();
      
      StatusCode sc = StatusCode::FAILURE ;
      if      ( m_pp  -> charge() == pp -> charge () ) 
      { sc = fillParticle( pp , m_pp , particle ) ; }
      else if ( m_app -> charge() == pp -> charge () ) 
      { sc = fillParticle( pp , m_app , particle ) ; }
      
      if( sc.isFailure() ) 
      {
        Warning("Error from 'fillParticle'", sc );
        delete particle ; particle = 0 ; continue ;           // CONTINUE
      }
      
      // put particle into the output container 
      particles.push_back( particle );
      ++number ;
      
  } // loop over protoparticles
  
  m_sum   +=          number ;
  m_sum2  += number * number ;

  return StatusCode::SUCCESS ;  
}

// ============================================================================
/** Fill the particle from protoparticle using ID  
 *  @param proto    pointer to ProtoParticle
 *  @param property particle property information
 *  @param particle particle (output) 
 *  @return status code 
 */
// ============================================================================
StatusCode NoPIDsParticleMaker::fillParticle
( const LHCb::ProtoParticle*    proto    ,
  const LHCb::ParticleProperty* property , 
  LHCb::Particle*               particle ) const 
{
  if ( 0 == proto    ) 
  { return Error ( "fillParticle: ProtoParticle*    is NULL" , 120 ) ; }
  if ( 0 == property ) 
  { return Error ( "fillParticle: ParticleProperty* is NULL" , 121 ) ; }
  if ( 0 == particle ) 
  { return Error ( "fillParticle: Particle*         is NULL" , 122 ) ; }
  
  const double mass = property -> mass() ;
  
  particle -> setParticleID   ( property -> particleID () ) ;
  particle -> setConfLevel    ( m_CL ) ;
  particle -> setMeasuredMass ( mass ) ;
  
  particle -> setProto( proto ) ;
  //
  const LHCb::Track* track = proto->track() ;  
  //
  const LHCb::State* state   = 0 ;
  // default closest to the beam 
  if ( 0 == state ) { state = track->stateAt ( LHCb::State::ClosestToBeam    ) ; }
  if ( 0 == state ) { state = track->stateAt ( LHCb::State::FirstMeasurement ) ; }
  if ( 0 == state ) 
  {
    Warning("No state closest to beam or at first measurement for track. Using first state instead") ;
    state = &track->firstState() ;    
  }
  if ( msgLevel ( MSG::VERBOSE ) ) 
  { verbose() << "Using state at " << state->position() << endmsg ; }
  
  return p2s()->state2Particle(*state,*particle);
};
// ============================================================================
