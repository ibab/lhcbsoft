
#include "CLHEP/Geometry/Point3D.h"

#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

//
#include "GaudiKernel/MsgStream.h"

// GiGa 
#include "GiGa/GiGaSensDetFactory.h"


/// local
#include "GiGaSensDetPrint.h"




/////////////////////////////////////////////////////////////////////////////////////////////////
static const GiGaSensDetFactory<GiGaSensDetPrint>                      s_GiGaSensDetPrintFactory;
const       IGiGaSensDetFactory&             GiGaSensDetPrintFactory = s_GiGaSensDetPrintFactory;
/////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////
GiGaSensDetPrint::GiGaSensDetPrint( const std::string& Name , ISvcLocator* Loc ) 
  : GiGaSensDetBase     ( Name , Loc ) 
  , G4VSensitiveDetector( Name )
{};
/////////////////////////////////////////////////////////////////////////////////////////////////
GiGaSensDetPrint::~GiGaSensDetPrint(){};
/////////////////////////////////////////////////////////////////////////////////////////////////
StatusCode GiGaSensDetPrint::initialize () { return GiGaSensDetBase::initialize() ; } ;
/////////////////////////////////////////////////////////////////////////////////////////////////
StatusCode GiGaSensDetPrint::finalize   () { return GiGaSensDetBase::initialize() ; } ;
/////////////////////////////////////////////////////////////////////////////////////////////////
bool       GiGaSensDetPrint::ProcessHits( G4Step* step , G4TouchableHistory* /* history */ )  /// "the main" method :-))
{
  if( 0 == step ) { return false ; } 
  
  double     edep = step->GetTotalEnergyDeposit();
  HepPoint3D pos  = step->GetPostStepPoint()->GetPosition(); 
  
  /// 
  G4TouchableHistory* TT =  (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
  G4VPhysicalVolume*  PV =   TT->GetVolume();
  G4LogicalVolume*    LV =   PV->GetLogicalVolume();
  
  MsgStream log( msgSvc() , name() );
  log << MSG::INFO << "ProcessStep() " 
      << " Pos=("  << pos.x() << "," << pos.y() << "," << pos.z() << ")" 
      << " PV="    << PV->GetName() 
      << " LV="    << LV->GetName() 
      << " edep="  << edep << endreq; 
  
  return false; 

};
/////////////////////////////////////////////////////////////////////////////////////////////////////

