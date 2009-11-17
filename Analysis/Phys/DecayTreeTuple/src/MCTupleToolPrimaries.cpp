// $Id: MCTupleToolPrimaries.cpp,v 1.1 2009-11-17 12:33:12 pkoppenb Exp $
// Include files 

// from Gaudi
#include "GaudiKernel/ToolFactory.h" 
#include "Event/MCHeader.h"
#include "Event/MCVertex.h"
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/TupleObj.h"
// local
#include "MCTupleToolPrimaries.h"

//-----------------------------------------------------------------------------
// Implementation file for class : MCTupleToolPrimaries
//
// 2009-11-16 : Patrick Koppenburg
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_TOOL_FACTORY( MCTupleToolPrimaries );


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
MCTupleToolPrimaries::MCTupleToolPrimaries( const std::string& type,
                                            const std::string& name,
                                            const IInterface* parent )
  : GaudiTool ( type, name , parent )
{
  declareInterface<IEventTupleTool>(this);

}
//=============================================================================
// Destructor
//=============================================================================
MCTupleToolPrimaries::~MCTupleToolPrimaries() {} 

//=============================================================================
StatusCode MCTupleToolPrimaries::fill( Tuples::Tuple& tuple ){
  if (msgLevel(MSG::VERBOSE)) verbose() << "getting MCHeader" << endmsg;
  if (!exist<LHCb::MCHeader>(LHCb::MCHeaderLocation::Default)){
    Warning("No MCHeader at "+LHCb::MCHeaderLocation::Default,StatusCode::SUCCESS,1);
    return StatusCode::SUCCESS ;
  }
  const LHCb::MCHeader* mch = get<LHCb::MCHeader>(LHCb::MCHeaderLocation::Default);
  std::vector<double> pvx, pvy, pvz, pvt;
  for ( SmartRefVector< LHCb::MCVertex >::const_iterator i = mch->primaryVertices().begin() ;
        i!=mch->primaryVertices().end() ; ++i){
    if ((*i)->isPrimary()){
      pvx.push_back((*i)->position().X());
      pvy.push_back((*i)->position().Y());
      pvz.push_back((*i)->position().Z());
      pvt.push_back((*i)->time());      
    }
  }
  if (!(tuple->farray( "MCPVX", pvx, "MCPVs" , 50 ))) return StatusCode::FAILURE;
  if (!(tuple->farray( "MCPVY", pvy, "MCPVs" , 50 ))) return StatusCode::FAILURE;
  if (!(tuple->farray( "MCPVZ", pvz, "MCPVs" , 50 ))) return StatusCode::FAILURE;
  if (!(tuple->farray( "MCPVT", pvt, "MCPVs" , 50 ))) return StatusCode::FAILURE;
  
  return StatusCode::SUCCESS ;
}
