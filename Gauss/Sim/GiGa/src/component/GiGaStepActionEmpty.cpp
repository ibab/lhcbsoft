#include "CLHEP/Geometry/Point3D.h"
///
/// GiGa 
#include "GiGa/GiGaStepActionFactory.h"
/// local
#include "GiGaStepActionEmpty.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
static const GiGaStepActionFactory<GiGaStepActionEmpty>                      s_GiGaStepActionEmptyFactory;
const       IGiGaStepActionFactory&             GiGaStepActionEmptyFactory = s_GiGaStepActionEmptyFactory;
/////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////
GiGaStepActionEmpty::GiGaStepActionEmpty( const std::string& Name , ISvcLocator* Loc ) 
  : GiGaStepActionBase            ( Name , Loc ) 
{};
/////////////////////////////////////////////////////////////////////////////////////////////////
GiGaStepActionEmpty::~GiGaStepActionEmpty(){};
/////////////////////////////////////////////////////////////////////////////////////////////////
StatusCode GiGaStepActionEmpty::initialize () { return GiGaStepActionBase::initialize() ; } ;
/////////////////////////////////////////////////////////////////////////////////////////////////
StatusCode GiGaStepActionEmpty::finalize   () { return GiGaStepActionBase::initialize() ; } ;
/////////////////////////////////////////////////////////////////////////////////////////////////
