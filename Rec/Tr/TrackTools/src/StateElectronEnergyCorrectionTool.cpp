// Include files
// -------------
#include <cmath>

// from DetDesc
#include "DetDesc/Material.h"

// local
#include "StateElectronEnergyCorrectionTool.h"

//-----------------------------------------------------------------------------
// Implementation file for class : StateElectronEnergyCorrectionTool
//
// 2006-08-18 : Eduardo Rodrigues
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_TOOL_FACTORY( StateElectronEnergyCorrectionTool )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
StateElectronEnergyCorrectionTool::StateElectronEnergyCorrectionTool( const std::string& type,
                                                                      const std::string& name,
                                                                      const IInterface* parent )
  : GaudiTool ( type, name , parent )
{
  declareInterface<IStateCorrectionTool>(this);
  
  declareProperty( "MaximumRadLength", m_maxRadLength  = 10. );
}

//=============================================================================
// Correct a State for electron dE/dx energy losses
//=============================================================================
void StateElectronEnergyCorrectionTool::correctState( LHCb::State& state,
                                                      const Material* material,
                                                      double wallThickness,
                                                      bool upstream,  
                                                      LHCb::ParticleID  ) const
{
  //hard energy loss for electrons
  double t = wallThickness / material -> radiationLength()
    * sqrt( 1. + std::pow(state.tx(),2) + std::pow(state.ty(),2) );
  if ( ! upstream ) t *= -1.;
  
  // protect against t too big
  if ( fabs(t) > m_maxRadLength )  t = std::copysign( 1.0, t ) * m_maxRadLength;
  
  // apply correction
  Gaudi::TrackVector&    tX = state.stateVector();
  Gaudi::TrackSymMatrix& tC = state.covariance();
  
  tC(4,4) += std::pow(tX[4],2) * ( exp(-t*log(3.0)/log(2.0))-exp(-2.0*t) );
  tX[4]   *= exp(-t);
}

//=============================================================================
// Destructor
//=============================================================================
StateElectronEnergyCorrectionTool::~StateElectronEnergyCorrectionTool() {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode StateElectronEnergyCorrectionTool::initialize()
{
  StatusCode sc = GaudiTool::initialize();
  if (sc.isFailure()) return sc;  // error already reported by base class
  
  return StatusCode::SUCCESS;
}

//=============================================================================
