// $Id: OTCluster2MCParticleAlg.cpp,v 1.7 2002-12-19 09:31:47 cattanem Exp $

// Event
#include "Event/OTCluster.h"
#include "Event/OTDigit.h"
#include "Event/MCOTDigit.h"
#include "Event/MCOTDeposit.h"
#include "Event/MCParticle.h"

// from Gaudi
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/MsgStream.h" 
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/SmartDataPtr.h"

// local
#include "OTCluster2MCParticleAlg.h"
#include "OTAssociators/OTCluster2MCParticleAsct.h"

/** @file OTCluster2MCParticleAlg.cpp 
 *
 *  Implementation of the OTCluster2MCParticleAlg class
 *  
 *  @author J. van Tilburg
 *  @date   23/05/2002
 */

// Declaration of the Algorithm Factory
static const  AlgFactory<OTCluster2MCParticleAlg>          s_factory ;
const        IAlgFactory& OTCluster2MCParticleAlgFactory = s_factory ; 

OTCluster2MCParticleAlg::OTCluster2MCParticleAlg( const std::string& name,
                                        ISvcLocator* pSvcLocator)
  : Algorithm (name,pSvcLocator) 
{
  // constructor
  declareProperty( "OutputData", m_outputData  = OTCluster2MCParticleLocation );
  declareProperty( "associatorName", m_nameAsct = "OTCluster2MCHitAsct" );
}

OTCluster2MCParticleAlg::~OTCluster2MCParticleAlg() {
  // destructor
}

StatusCode OTCluster2MCParticleAlg::initialize() {

  MsgStream log(msgSvc(), name());
  StatusCode sc;
  log << MSG::DEBUG << "==> Initialize" << endreq;

  sc = toolSvc()->retrieveTool(m_nameAsct, m_hAsct);
  if( sc.isFailure() || 0 == m_hAsct) {
    log << MSG::FATAL << "Unable to retrieve Associator tool" << endreq;
    return sc;
  }
 
  return StatusCode::SUCCESS;
}


StatusCode OTCluster2MCParticleAlg::execute() {

  typedef Relation1D<OTCluster, MCParticle>    Table;

  // get OTClusters
  SmartDataPtr<OTClusters> clusterCont(eventSvc(),OTClusterLocation::Default);
  if (0 == clusterCont){ 
    MsgStream log(msgSvc(), name());
    log << MSG::WARNING << "Failed to find OTClusters" << endreq;
    return StatusCode::FAILURE;
  }

  // create an association table 
  Table* aTable = new Table();

  // loop and link OTClusters to MC truth
  OTClusters::const_iterator iterClus;
  for(iterClus = clusterCont->begin(); 
      iterClus != clusterCont->end(); iterClus++){
    std::vector<MCParticle*> partVector;
    associateToTruth(*iterClus, partVector);
    std::vector<MCParticle*>::iterator iPart = partVector.begin();
    while (iPart != partVector.end()) {
      aTable->relate(*iterClus, *iPart);
      iPart++;
    }
  } // loop iterClus

  // register table in store
  StatusCode sc = eventSvc()->registerObject(outputData(), aTable);
  if( sc.isFailure() ) {
    MsgStream log(msgSvc(), name());
    log << MSG::FATAL << "     *** Could not register " << outputData()
        << endreq;
    delete aTable;
    return StatusCode::FAILURE;
  }

  return StatusCode::SUCCESS;
}

StatusCode OTCluster2MCParticleAlg::finalize() {

  MsgStream log(msgSvc(), name());
  log << MSG::DEBUG << "==> Finalize" << endreq;

  // Release tools
  if( m_hAsct ) toolSvc()->releaseTool( m_hAsct );

  return StatusCode::SUCCESS;
}

StatusCode OTCluster2MCParticleAlg::associateToTruth(const OTCluster* aCluster,
                                         std::vector<MCParticle*>& partVector) {
  // make truth link to MCParticle
  StatusCode sc = StatusCode::SUCCESS;

  // retrieve table
  OTCluster2MCHitAsct::DirectType* aTable = m_hAsct->direct();
  if (0 == aTable){
    MsgStream log(msgSvc(), name());
    log << MSG::WARNING << "Failed to find table" << endreq;
    return StatusCode::FAILURE;
  }
 
  OTCluster2MCHitAsct::MCHits range = aTable->relations(aCluster);
  if ( !range.empty() ) {
    OTCluster2MCHitAsct::MCHitsIterator iterHit;
    for (iterHit = range.begin(); iterHit != range.end(); iterHit++) {
      MCHit* aHit = iterHit->to();
      if (0 != aHit) {
        MCParticle* aParticle = aHit->mcParticle();
        if (0 != aParticle) partVector.push_back(aParticle);
      }
    }
  }

  return sc;
}
