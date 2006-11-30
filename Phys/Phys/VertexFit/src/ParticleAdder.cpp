// Include files 

// from Gaudi
#include "GaudiKernel/ToolFactory.h" 


#include "Event/Vertex.h"
#include "Event/Particle.h"

#include "GaudiKernel/IParticlePropertySvc.h"
#include "GaudiKernel/ParticleProperty.h"
// local
#include "ParticleAdder.h"

using namespace LHCb ;
using namespace Gaudi::Units;
//-----------------------------------------------------------------------------
// Implementation file for class : ParticleAdder
//
// 2006-11-30 : Yasmine Amhis & Olivier Deschamps
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
static const  ToolFactory<ParticleAdder>          s_factory ;
const        IToolFactory& ParticleAdderFactory = s_factory ; 


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
ParticleAdder::ParticleAdder( const std::string& type,
                                  const std::string& name,
                                  const IInterface* parent )
  : GaudiTool ( type, name , parent )
{
  declareInterface<IVertexFit>       (this);
  declareInterface<IParticleCombiner>(this);
}

//=============================================================================
// Destructor
//=============================================================================
ParticleAdder::~ParticleAdder() {};

//=============================================================================
// Initialize
//=============================================================================
StatusCode ParticleAdder::initialize(){
  StatusCode sc = GaudiTool::initialize();
  if (!sc) return sc;
  return sc;
};

//=============================================================================
// Fit the vertex from a vector of Particles
//=============================================================================
StatusCode ParticleAdder::fit( const LHCb::Particle::ConstVector& parts, 
                                 LHCb::Particle& P, LHCb::Vertex& V) const
{
  debug() << "start ParticleAdder fit " <<endreq;
  debug() << "using " << parts.size() <<" particles" <<endreq;
 
  P.clearDaughters();
  V.clearOutgoingParticles();
  V.setChi2(0.0);
  V.setNDoF(0);
  P.setEndVertex( &V );


  for(Particle::ConstVector::const_iterator iterP = parts.begin(); iterP != parts.end(); iterP++) {
    P.addToDaughters(*iterP);
    V.addToOutgoingParticles(*iterP);
  }
  const SmartRefVector<LHCb::Particle>&  daughters = P.daughters();


  Gaudi::LorentzVector momentum;
  Gaudi::SymMatrix4x4  matrix;
  for(SmartRefVector<LHCb::Particle>::const_iterator id = daughters.begin(); id !=daughters.end() ;++id){
    const LHCb::Particle* daughter = *id;
    momentum += daughter->momentum();
    matrix   += daughter->momCovMatrix();
  }  
  P.setMomentum( momentum );
  P.setMomCovMatrix( matrix);  
  P.setMeasuredMass( P.momentum().M() );

  return StatusCode::SUCCESS;  
}

//=============================================================================
// Fit the vertex from a vector of Particles
//=============================================================================
StatusCode ParticleAdder::fit( const LHCb::Particle::ConstVector& parts, 
                                 LHCb::Vertex& V) const{  
  LHCb::Particle tPart;
  return fit(parts, tPart, V);
}

//==================================================================
//  method to determine if a particle is a resonance
//==================================================================

bool ParticleAdder::isResonance(const LHCb::Particle* part) const {
  bool isRes=false;
  int id=part->particleID().pid();  
  ParticleProperty*  partProp = m_ppSvc->findByStdHepID(id );
  if( (*partProp).lifetime()*pow(10,-9) < pow(10,-15)) isRes=true;
  return isRes;

}

//==================================================================
//  method to determine if a particle has a reconstructed vertex 
//==================================================================

bool ParticleAdder::isVertexed(const LHCb::Particle* part) const{
  return false;
}



