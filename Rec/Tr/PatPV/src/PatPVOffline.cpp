// $Id: PatPVOffline.cpp,v 1.3 2007-12-04 11:13:06 witekma Exp $
// Include files:
// from Gaudi
#include "GaudiKernel/SystemOfUnits.h"
#include "GaudiKernel/AlgFactory.h"
// from Event
#include "Event/Track.h"
#include "Event/State.h"
#include "Event/RecVertex.h"
// Local
#include "PatPVOffline.h"

DECLARE_ALGORITHM_FACTORY(PatPVOffline);

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PatPVOffline::PatPVOffline(const std::string& name,
                           ISvcLocator* pSvcLocator)
  : GaudiAlgorithm(name,pSvcLocator) {
  declareProperty("OutputVertices",m_outputVertices = 
                                   LHCb::RecVertexLocation::Primary);
}

//=============================================================================
// Destructor
//=============================================================================
PatPVOffline::~PatPVOffline() {
};

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode PatPVOffline::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize();
  if (!sc) return sc;
  debug() << "Initialisation" << endmsg;
  // Access PVOfflineTool
  m_pvsfit = tool<IPVOfflineTool>("PVOfflineTool",this);
  if(!m_pvsfit) {
    err() << "Unable to retrieve the PVOfflineTool" << endmsg;
    return  StatusCode::FAILURE;
  }
  return StatusCode::SUCCESS;
};

//=============================================================================
// Execution
//=============================================================================
StatusCode PatPVOffline::execute() {
  if(msgLevel(MSG::DEBUG)) debug() << "Execute" << endmsg;

  LHCb::RecVertices* v2tes = new LHCb::RecVertices();
  put(v2tes,m_outputVertices);  

  std::vector<LHCb::RecVertex> rvts;
  StatusCode scfit = m_pvsfit->reconstructMultiPV(rvts);
  if (scfit != StatusCode::SUCCESS) {
    return StatusCode::SUCCESS;
  }

  for(std::vector<LHCb::RecVertex>::iterator iv = rvts.begin(); iv != rvts.end(); iv++) {
    LHCb::RecVertex* vertex = new LHCb::RecVertex();
    *vertex = *iv;
    vertex->setTechnique(LHCb::RecVertex::Primary);
    v2tes->insert(vertex);
  }
 

// ---> Debug
  if(msgLevel(MSG::DEBUG)) {
    debug() << endmsg;
    debug() << "TES location filled with ";
    debug() << v2tes->size() << " PrimVertices" << endmsg;
    int nVtx = 0;
    for(LHCb::RecVertices::iterator itVtx = v2tes->begin();
        v2tes->end() != itVtx; itVtx++, nVtx++) {
      LHCb::RecVertex* vertex = (*itVtx);
      debug() << " Vertex " << nVtx << endmsg;
      debug() << " x, y, z: " 
              << vertex->position().x() << " "
              << vertex->position().y() << " "
              << vertex->position().z() << endmsg;
      debug() << " Errors : "
              << sqrt(vertex->covMatrix()(0,0)) << " "
              << sqrt(vertex->covMatrix()(1,1)) << " "
              << sqrt(vertex->covMatrix()(2,2)) << endmsg;
      debug() << " Number of tracks: " 
              << vertex->tracks().size() << endmsg;
      debug() << " Chi2/DoF: " << vertex->chi2()/vertex->nDoF() << endmsg;
    }      
    debug() << endmsg;
  }

  return StatusCode::SUCCESS;
}


//=============================================================================
//  Finalize
//=============================================================================
StatusCode PatPVOffline::finalize() {

  debug() << "==> Finalize" << endmsg;

  return GaudiAlgorithm::finalize();  // must be called after all other actions
}
