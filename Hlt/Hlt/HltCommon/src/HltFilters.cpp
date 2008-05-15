// from Gaudi
#include "GaudiKernel/AlgFactory.h" 

// local
#include "HltFilters.h"
#include "HltTFilter.icpp"

//-----------------------------------------------------------------------------
// Implementation file for class : HltFilters
//
// 2007-11-27 : Jose Angel Hernando Morata
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( HltTrackFilter );

HltTrackFilter::HltTrackFilter( const std::string& name, 
                                ISvcLocator* pSvcLocator ) 
  : HltTFilter<LHCb::Track>(name,pSvcLocator, "HltTrackFunctionFactory") 
{}


DECLARE_ALGORITHM_FACTORY( HltVertexFilter );

HltVertexFilter::HltVertexFilter( const std::string& name, 
                                ISvcLocator* pSvcLocator ) :
  HltTFilter<LHCb::RecVertex>(name,pSvcLocator, "HltVertexFunctionFactory")
{}


DECLARE_ALGORITHM_FACTORY( HltTrackPrepare );

HltTrackPrepare::HltTrackPrepare( const std::string& name, 
                                ISvcLocator* pSvcLocator ) :
  HltTFilter<LHCb::Track>(name,pSvcLocator, "HltTrackFunctionFactory")
{
  m_tesInput = true;
}


DECLARE_ALGORITHM_FACTORY( HltVertexPrepare );

HltVertexPrepare::HltVertexPrepare( const std::string& name, 
                                    ISvcLocator* pSvcLocator ) :
  HltTFilter<LHCb::RecVertex>(name,pSvcLocator, "HltVertexFunctionFactory")
{
  m_tesInput = true;
}
