// $Id: PIDFilterCriterion.cpp,v 1.1 2002-03-27 20:35:00 gcorti Exp $
// Include files 

// from Gaudi
#include "GaudiKernel/ToolFactory.h"
#include "GaudiKernel/MsgStream.h" 
#include "GaudiKernel/IParticlePropertySvc.h"
#include "GaudiKernel/ParticleProperty.h"

// local
#include "PIDFilterCriterion.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PIDFilterCriterion
//
// 19/03/2002 : Paul Colrain
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
static const  ToolFactory<PIDFilterCriterion>          s_factory ;
const        IToolFactory& PIDFilterCriterionFactory = s_factory ; 


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PIDFilterCriterion::PIDFilterCriterion( const std::string& type,
                                        const std::string& name,
                                        const IInterface* parent )
  : AlgTool ( type, name , parent ) {

  // declare additional interface
  declareInterface<IFilterCriterion>(this);

  // declare properties
  declareProperty( "ParticleNames", m_partNames );  
  declareProperty( "ParticleCodes", m_partCodes );  
  declareProperty( "ConfidenceLevels", m_confLevels );  

  // get pointer to ParticlePropertySvc
  m_ppSvc = 0;
  if ( serviceLocator() ) {
    StatusCode sc = StatusCode::FAILURE;
    sc = serviceLocator()->service( "ParticlePropertySvc", m_ppSvc );
  }

}

//=============================================================================
// initialize() method
//=============================================================================
StatusCode PIDFilterCriterion::initialize() {

  MsgStream          log( msgSvc(), name() );

  if ( m_partNames.size() == 0 && m_partCodes.size() == 0 ) {
    log << MSG::ERROR << ">>>   PIDFilterCriterion::initialize() "
        << endreq;
    log << MSG::ERROR << ">>>   Properties have not been set "
        << endreq;
    return StatusCode::FAILURE;
  }

  if ( m_partNames.size() != 0 && m_partCodes.size() != 0 ) {
    log << MSG::ERROR << ">>>   PIDFilterCriterion::initialize() "
        << endreq;
    log << MSG::ERROR << ">>>   Attempt made to set Particle Identity "
        << "both by name and code! "
        << endreq;
    return StatusCode::FAILURE;
  }

  // get codes corresponding to names
  if ( m_partNames.size() > 0 ) {
    std::vector< std::string >::iterator in;  
    for ( in = m_partNames.begin(); in != m_partNames.end(); in++ ) {    
      ParticleProperty* partProp = m_ppSvc->find( *in );
      if ( partProp == 0 ) {
        log << MSG::ERROR << "Cannot retrieve properties for particle \""
            << *in << "\" " << endreq;
         return StatusCode::FAILURE;
      }
      m_partCodes.push_back( partProp->jetsetID() );  
    }
  }
  
  // get names corresponding to codes
  if ( m_partNames.size() == 0 && m_partCodes.size() > 0 ) {
    std::vector< int >::iterator ic;  
    for ( ic = m_partCodes.begin(); ic != m_partCodes.end(); ic++ ) {    
      ParticleProperty* partProp = m_ppSvc->findByStdHepID( *ic );
      if ( partProp == 0 ) {
        log << MSG::ERROR << "Cannot retrieve properties for particle code "
            << *ic << endreq;
         return StatusCode::FAILURE;
      }
      m_partNames.push_back( partProp->particle() );  
    }
  }

  if ( m_confLevels.size() > 0 && m_partNames.size() != m_confLevels.size() ) {
  log << MSG::ERROR << ">>>   PIDFilterCriterion::initialize() "
      << endreq;
  log << MSG::ERROR << ">>>   Wrong number of Confidence Levels provided "
      << endreq;
  return StatusCode::FAILURE;   
  }

  log << MSG::DEBUG << ">>>   PIDFilterCriterion::initialize() " 
      << endreq;
  log << MSG::DEBUG << ">>>   Cuts are " << endreq;
  log << MSG::DEBUG << ">>>   Particle names    ";
  for (std::vector< std::string >::iterator in = m_partNames.begin(); 
                                            in != m_partNames.end(); in++ ) {
    log << MSG::DEBUG << *in << "  ";    
  }
  log << MSG::DEBUG << endreq;
  log << MSG::DEBUG << ">>>   Particle codes    ";
  for (std::vector< int >::iterator ic = m_partCodes.begin();
                                    ic != m_partCodes.end(); ic++ ) {
    log << MSG::DEBUG << *ic << "  ";    
  }
  log << MSG::DEBUG << endreq;
  log << MSG::DEBUG << ">>>   Confidence levels    ";
  for (std::vector< double >::iterator icl = m_confLevels.begin();
                                       icl != m_confLevels.end(); icl++ ) {
    log << MSG::DEBUG << *icl << "  ";    
  }
  log << MSG::DEBUG << endreq;
  //log << MSG::DEBUG << "test " << m_partCodes[0] << endreq;
  //log << MSG::DEBUG << "test " << m_partCodes[1] << endreq;
  //log << MSG::DEBUG << "test " << m_partCodes[2] << endreq;
  

  return StatusCode::SUCCESS;
}

//=============================================================================
// Test if filter is satisfied
//=============================================================================
bool PIDFilterCriterion::isSatisfied( const Particle* const & part ) {

  bool passed = false;  

  for ( unsigned int i = 0; i < m_partCodes.size(); i++ ) {
    if ( part->particleID().id() == m_partCodes[i] &&
         part->confLevel() > m_confLevels[i] ) passed = true;
  }

  return passed;
  
}

//=============================================================================
// Test if filter is satisfied
//=============================================================================
bool PIDFilterCriterion::operator()( const Particle* const & part ) {

  bool passed = true;  

  for ( unsigned int i = 0; i < m_partCodes.size(); i++ ) {
    if ( part->particleID().id() == m_partCodes[i] &&
         part->confLevel() > m_confLevels[i] ) passed = true;
  }

  return passed;

}

//=============================================================================
