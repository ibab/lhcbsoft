// $Id: PVRelatorAlg.cpp,v 1.2 2009-05-23 17:25:20 jpalac Exp $
// Include files 

// from Gaudi
#include "GaudiKernel/AlgFactory.h" 
// from LHCb
#include "Kernel/IOnOffline.h"
#include "Kernel/IRelatedPVFinder.h"
#include "Event/RecVertex.h"
#include "Event/Particle.h"
#include "Kernel/Particle2Vertex.h"

// local
#include "PVRelatorAlg.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PVRelatorAlg
//
// 2009-05-23 : Juan Palacios
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( PVRelatorAlg );


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PVRelatorAlg::PVRelatorAlg( const std::string& name,
                            ISvcLocator* pSvcLocator)
  : 
  GaudiAlgorithm ( name , pSvcLocator ),
  m_particleInputLocation(""),
  m_PVInputLocation(LHCb::RecVertexLocation::Primary),
  m_P2PVInputLocation(""),
  m_P2PVOutputLocation(""),
  m_useTable(false),
  m_OnOffline(0),
  m_pvRelator(0)
{
  declareProperty("ParticleInputLocation",  m_particleInputLocation);
  declareProperty("PrimaryVertexInputLocation",  m_PVInputLocation);
  declareProperty("P2PVRelationsInputLocation",  m_P2PVInputLocation);
  //  declareProperty("P2PVRelationsOutputLocation",  m_P2PVOutputLocation);
  
}
//=============================================================================
// Destructor
//=============================================================================
PVRelatorAlg::~PVRelatorAlg() {} 

//=============================================================================
// Initialization
//=============================================================================
StatusCode PVRelatorAlg::initialize() {

  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;
  if (m_P2PVInputLocation!="") m_useTable=true;
  
  if ( m_useTable && 
       ( m_particleInputLocation!="" ||
         m_PVInputLocation!=LHCb::RecVertexLocation::Primary) ) {
    return Error("You have set P2PVRelationsLocation and one of ParticleInputLocation and PrimaryVertexInputLocation.",
                 StatusCode::FAILURE, 
                 1 );    
  }
  
  m_OnOffline = tool<IOnOffline>("OnOfflineTool",this);
  if (0==m_OnOffline) return Error("Failed to get IOnOffline tool");
  
  m_pvRelator = tool<IRelatedPVFinder>(m_OnOffline->relatedPVFinderType(), 
                                       this);
 
  if (0==m_pvRelator) return Error("Failed to get IRelatedPVFinder tool");

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode PVRelatorAlg::execute() {

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  return StatusCode::SUCCESS;
}
//=============================================================================
const Particle2Vertex::Table* PVRelatorAlg::table() 
{

  const LHCb::Particle::Container* particles =
    i_get<LHCb::Particle::Container>(m_particleInputLocation);

  const LHCb::RecVertex::Container* vertices = 
    i_get<LHCb::RecVertex::Container>(m_PVInputLocation);

  Particle2Vertex::Table* table = new Particle2Vertex::Table();

  return table;
  
//   const LHCb::Particle*  bCand = 
//     const Particle2Vertex::Table* table = 
//     get<Particle2Vertex::Table>(m_Particle2RefittedPVRelationsLoc);
//   const  Particle2Vertex::Range range =  table->relations( bCand );
//   const LHCb::RecVertex* bestPV =  range.empty() ? 0 : 
//     dynamic_cast<const LHCb::RecVertex*>( range.back().to() );
 
}
//=============================================================================
//=============================================================================
const Particle2Vertex::Table* PVRelatorAlg::tableFromTable() 
{

  Particle2Vertex::Table* table = new Particle2Vertex::Table();

  return table;  
}
//=============================================================================
//=============================================================================
//  Finalize
//=============================================================================
StatusCode PVRelatorAlg::finalize() {

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Finalize" << endmsg;

  return GaudiAlgorithm::finalize();  // must be called after all other actions
}
//=============================================================================
