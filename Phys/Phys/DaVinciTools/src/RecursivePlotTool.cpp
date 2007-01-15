// $Id: RecursivePlotTool.cpp,v 1.5 2007-01-15 10:32:37 jpalac Exp $
// Include files 

// from Gaudi
#include "GaudiKernel/ToolFactory.h" 
#include "GaudiKernel/SmartIF.h" 
#include "GaudiKernel/IParticlePropertySvc.h"
#include "GaudiKernel/ParticleProperty.h"

// local
#include "RecursivePlotTool.h"

//-----------------------------------------------------------------------------
// Implementation file for class : RecursivePlotTool
//
// 2005-01-07 : Patrick KOPPENBURG
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_TOOL_FACTORY( RecursivePlotTool );

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
RecursivePlotTool::RecursivePlotTool( const std::string& type,
                                      const std::string& name,
                                      const IInterface* parent )
  : GaudiTool ( type, name , parent )
  , m_simplePlotTool(0)
{
  declareInterface<IPlotTool>(this);

  declareProperty( "Variables",    m_variables  );
  declareProperty( "Minima",       m_minima   );
  declareProperty( "Maxima",       m_maxima   );
}
//=============================================================================
// Destructor
//=============================================================================
RecursivePlotTool::~RecursivePlotTool() {}; 

//=============================================================================
// Initialisation
//=============================================================================
StatusCode RecursivePlotTool::initialize() {
  StatusCode sc = GaudiTool::initialize();
  if (!sc) return sc;

  m_ppSvc = svc<IParticlePropertySvc>("ParticlePropertySvc", true);
  if( !m_ppSvc ) {
    err() << "Unable to locate Particle Property Service" << endreq;
    return StatusCode::FAILURE;
  }          
  m_simplePlotTool = tool<IPlotTool>("SimplePlotTool",this);
  if( !m_simplePlotTool ) {
    err() << "Unable to get SimplePlotTool" << endreq;
    return StatusCode::FAILURE;
  }
  SmartIF<IProperty> algt(m_simplePlotTool);
  
  if ( algt ) {
    sc = algt->setProperty(this->getProperty("Variables"));
    if (sc) sc = algt->setProperty(this->getProperty("Minima"));
    if (sc) sc = algt->setProperty(this->getProperty("Maxima"));
    if (!sc) return sc;
  } else {
    err() << "Unable to get SmartIF<IProperty> algt(m_simplePlotTool)" << endreq;
    return StatusCode::FAILURE;    
  }
  sc = m_simplePlotTool->initialize(); // re-initialize
  if (!sc) return sc;
  
  return StatusCode::SUCCESS;    
}
//=============================================================================
StatusCode RecursivePlotTool::finalize(){
  if (m_simplePlotTool) toolSvc()->releaseTool(m_simplePlotTool);
  return GaudiTool::finalize();
}
//=============================================================================
// 
//=============================================================================
StatusCode RecursivePlotTool::setPath(const std::string& path) {
  debug() << "Setting path to " << path << endmsg;
  //  setHistoDir(path);
  m_simplePlotTool->setPath(path);
  return StatusCode::SUCCESS;  
}; 
//=============================================================================
// Fill plots for a vector of particles
//=============================================================================
StatusCode RecursivePlotTool::fillPlots(const LHCb::Particle::ConstVector& PV,
                                        const std::string trailer) {
  for ( LHCb::Particle::ConstVector::const_iterator p = PV.begin() ; p != PV.end() ; ++p ){
    StatusCode sc = fillPlots(*p,trailer);
    if (!sc) return sc;
  }
  return StatusCode::SUCCESS;
}; 
//=============================================================================
// Fill plots for particle and daughters
//=============================================================================
StatusCode RecursivePlotTool::fillPlots(const LHCb::Particle* p,
                                        const std::string trailer) {
  debug() << "Filling plots for particle " << p->particleID().pid() 
          << " " << p->momentum() << endmsg ;
  StatusCode sc = m_simplePlotTool->fillPlots(p,trailer);
  debug() << "Filled plots for particle " << p->particleID().pid() 
          << " with " << sc << endmsg ;
  if (!sc) return sc;
  if (!p->isBasicParticle()){
    debug() << "Particle has " << p->daughtersVector().size() << " daughters" << endmsg ;
    const LHCb::Particle::ConstVector pv = p->daughtersVector();
    debug() << "This particle has " << pv.size() << " daughters!" << endmsg ;
    std::string newtrailer ;
    ParticleProperty *pp = m_ppSvc->findByPythiaID(abs(p->particleID().pid()));
    if (pp) newtrailer = "from "+pp->particle();
    // loop on daughters
    for ( LHCb::Particle::ConstVector::const_iterator ip = pv.begin();
          ip!=pv.end();++ip) {
      StatusCode sc = this->fillPlots(*ip,newtrailer); // call this recursively
      if (!sc) return StatusCode::FAILURE;
    } 
  } else debug() << p->particleID() << " is basic" << endmsg ;
  return StatusCode::SUCCESS;
}; 

