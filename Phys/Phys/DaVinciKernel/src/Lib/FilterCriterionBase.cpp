// $Id: FilterCriterionBase.cpp,v 1.3 2006-09-14 17:58:02 pkoppenb Exp $
// Include files 

// local
#include "Kernel/FilterCriterionBase.h"

//-----------------------------------------------------------------------------
// Implementation file for class : FilterCriterionBase
//
// 2005-06-22 : Cristina Lazzeroni
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
FilterCriterionBase::FilterCriterionBase( const std::string& type, 
                                          const std::string& name,
                                          const IInterface* parent )
  : GaudiTool ( type, name , parent ),
    m_tesTool ( 0 )
  , m_relatedPV( 0 )
{
  declareProperty( "SaveFilterResults", m_saveResults = false );
  declareProperty( "FlaggingMode",      m_flagMode    = false );
}

//=============================================================================
// Destructor
//=============================================================================
FilterCriterionBase::~FilterCriterionBase() {}; 

//=============================================================================

StatusCode FilterCriterionBase::initialize( )
{
  const StatusCode sc = GaudiTool::initialize() ;
  if (!sc) return sc ;

  if ( m_saveResults )
    info() << "Will save filter results to TES" << endreq;
  
  if ( m_flagMode )
    info() << "Flagging mode enabled. Will always return true" << endreq;

  m_relatedPV = tool<IRelatedPV>("RelatedPV",this);  

  return sc;
}

// wrapper operator method around isSatisfied
bool FilterCriterionBase::operator()( const LHCb::Particle* const & part ) 
{
  return isSatisfied(part);
}

bool FilterCriterionBase::isSatisfied( const LHCb::Particle* const & part ) 
{
  // run the actual filter in derived class
  const bool result = testParticle( part );
  
  // print out result
  if ( msgLevel(MSG::DEBUG) )
  {
    debug() << "Filter " << name() << " result " << result << endreq;
  }

  // save if requested
  if ( m_saveResults )
  {
    tesTool()->saveFilterResult( this, const_cast<LHCb::Particle*>(part), result );
  }

  // return result of filter, or true if Flagging mode is enabled
  return ( m_flagMode ? true : result );
}
