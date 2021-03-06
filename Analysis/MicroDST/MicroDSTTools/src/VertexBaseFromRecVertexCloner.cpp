// $Id: VertexBaseFromRecVertexCloner.cpp,v 1.1 2010-08-11 12:48:36 jpalac Exp $
// Include files

// from Gaudi
#include "GaudiKernel/ToolFactory.h"

// from LHCb
#include "Event/RecVertex.h"

// local
#include "VertexBaseFromRecVertexCloner.h"

//-----------------------------------------------------------------------------
// Implementation file for class : VertexBaseFromRecVertexCloner
//
// 2007-12-05 : Juan PALACIOS
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
VertexBaseFromRecVertexCloner::
VertexBaseFromRecVertexCloner( const std::string& type,
                               const std::string& name,
                               const IInterface* parent )
  : base_class ( type, name , parent ) 
{ 
  //setProperty( "OutputLevel", 2 );
}

//=============================================================================

LHCb::VertexBase*
VertexBaseFromRecVertexCloner::operator() ( const LHCb::VertexBase* vertex )
{
  const LHCb::RecVertex* recVertex = dynamic_cast<const LHCb::RecVertex*>(vertex);
  return ( recVertex ? this->clone(recVertex) : NULL );
}

//=============================================================================

LHCb::RecVertex*
VertexBaseFromRecVertexCloner::operator() ( const LHCb::RecVertex* vertex )
{
  return this->clone(vertex);
}

//=============================================================================

LHCb::RecVertex* 
VertexBaseFromRecVertexCloner::clone( const LHCb::RecVertex* vertex )
{
  LHCb::RecVertex * clone_v = NULL;
  
  const bool veto = isVetoed(vertex);
  
  if ( veto ) 
  {
    clone_v = const_cast<LHCb::RecVertex*>(vertex);
  }
  else
  {
    if ( msgLevel(MSG::VERBOSE) )
      verbose() << "Cloning RecVertex at " << tesLocation(vertex) << endmsg;
    clone_v = cloneKeyedContainerItem<PVCloner>(vertex);
  }
  
  return clone_v;
}

//=============================================================================
// Destructor
//=============================================================================
VertexBaseFromRecVertexCloner::~VertexBaseFromRecVertexCloner() {}

//=============================================================================

// Declaration of the Tool Factory
DECLARE_TOOL_FACTORY( VertexBaseFromRecVertexCloner )

//=============================================================================
