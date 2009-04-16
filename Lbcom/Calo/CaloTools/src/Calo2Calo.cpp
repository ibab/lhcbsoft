// $Id: Calo2Calo.cpp,v 1.3 2009-04-16 13:07:43 odescham Exp $
// Include files 

// from Gaudi
#include "GaudiKernel/ToolFactory.h" 

#include "Kernel/CaloCellCode.h"
// local
#include "Calo2Calo.h"

//-----------------------------------------------------------------------------
// Implementation file for class : Calo2Calo
//
// 2007-05-29 : Olivier Deschamps
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_TOOL_FACTORY( Calo2Calo );


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
Calo2Calo::Calo2Calo( const std::string& type,
                      const std::string& name,
                      const IInterface* parent )
  : GaudiTool ( type, name , parent )
    ,m_cells()
    ,m_digits()
    ,m_energy()
    ,m_count(0)
{
  declareInterface<ICalo2Calo>(this);
  declareProperty("IdealGeometry", m_geo = true );

}
//=============================================================================
// Destructor
//=============================================================================
Calo2Calo::~Calo2Calo() {}
//=============================================================================
StatusCode Calo2Calo::initialize(){
  StatusCode sc = GaudiTool::initialize();
  debug() << "Initialize Calo2Calo tool " << endreq;

  // CaloDigit locations
  m_loc["Ecal"]= LHCb::CaloDigitLocation::Ecal;
  m_loc["Hcal"]= LHCb::CaloDigitLocation::Hcal;
  m_loc["Prs"]= LHCb::CaloDigitLocation::Prs;
  m_loc["Spd"]= LHCb::CaloDigitLocation::Spd;
  // DeCalorimeter* pointers
  m_det["Ecal"]=getDet<DeCalorimeter>( DeCalorimeterLocation::Ecal );
  m_det["Hcal"]=getDet<DeCalorimeter>( DeCalorimeterLocation::Hcal );
  m_det["Prs"]=getDet<DeCalorimeter>( DeCalorimeterLocation::Prs   );
  m_det["Spd"]=getDet<DeCalorimeter>( DeCalorimeterLocation::Spd   );
  // CellSize reference (outer section)  Warning : factor 2 for Hcal
  m_refSize["Hcal"] = (*(m_det["Hcal"]->cellParams().begin())).size()/2.;
  m_refSize["Ecal"] = (*(m_det["Ecal"]->cellParams().begin())).size(); 
  m_refSize["Prs"]  = (*(m_det["Prs"]->cellParams().begin())).size(); 
  m_refSize["Spd"]  = (*(m_det["Spd"]->cellParams().begin())).size(); 

  return sc;
}

void Calo2Calo::reset(){
  m_digits.clear();
  m_cells.clear();
  m_energy = 0;
  m_count  = 0;
}

//=======================================================================================================
const std::vector<LHCb::CaloCellID>& Calo2Calo::cellIDs(LHCb::CaloCluster fromCluster, std::string toCalo){

  reset();

  for(LHCb::CaloCluster::Entries::const_iterator ent = fromCluster.entries().begin();
      ent != fromCluster.entries().end(); ++ent){
    SmartRef<LHCb::CaloDigit> digit = (*ent).digit();
    cellIDs( (*digit).cellID() , toCalo, false );
  }
  return m_cells;
}
//=======================================================================================================
const std::vector<LHCb::CaloCellID>& Calo2Calo::addCell( LHCb::CaloCellID id, std::string toCalo){
  // check duplicate
  for(std::vector<LHCb::CaloCellID>::const_iterator icel = m_cells.begin(); icel != m_cells.end();++icel){
    if( id == (*icel) )return m_cells;
  }
  // add the cells
  m_cells.push_back( id );
  LHCb::CaloDigits* digits = get<LHCb::CaloDigits>( m_loc[ toCalo ] );
  LHCb::CaloDigit* digit = digits->object( id );
  if( NULL != digit ) {
    m_digits.push_back( digit );
    m_energy += digit->e();
    m_count++;
  }
  return m_cells;
}


//=======================================================================================================
const std::vector<LHCb::CaloCellID>& Calo2Calo::cellIDs(LHCb::CaloCellID fromId, std::string toCalo , bool init){

  if( init )reset();

  m_fromCalo = CaloCellCode::CaloNameFromNum( fromId.calo() );
  m_toCalo   = toCalo;
  LHCb::CaloCellID toId = fromId;
  // ---- Assume ideal geometry : trivial mapping for detectors having the same granularity (Prs/Spd/Ecal)
  if( ( m_geo && (m_fromCalo == "Ecal" || m_fromCalo == "Prs" || m_fromCalo == "Spd") 
        && (m_toCalo == "Ecal" || m_toCalo == "Prs" || m_toCalo == "Spd") ) 
      || m_fromCalo == m_toCalo ){
    toId.setCalo( CaloCellCode::CaloNumFromName( m_toCalo ));
    return addCell( toId , m_toCalo);
  }
  

  // ---- Else use the actual geometry to connet detectors
  DeCalorimeter* fromDet = m_det[ m_fromCalo ];
  DeCalorimeter* toDet   = m_det[ m_toCalo ];
  double scale = 1.;
  Gaudi::XYZPoint  center = fromDet->cellCenter( fromId );
  if( m_fromCalo != m_toCalo ){
    // z-scaling
    scale = m_refSize[ m_toCalo ] / m_refSize[ m_fromCalo ] ;
    center  = toDet->plane(CaloPlane::Middle).ProjectOntoPlane( fromDet->cellCenter( fromId )*scale );
    // connect
    toId  = toDet->Cell( center );
  }
  double fromSize = fromDet->cellSize( fromId )*scale;
  //cell-center is outside 'toCalo' - check corners  
  if( LHCb::CaloCellID() == toId){
    for( int i = 0 ; i != 2; ++i){
      for( int j = 0 ; j != 2; ++j){
        double x = fromDet->cellCenter( fromId ).X() + (i*2-1) * fromSize;        
        double y = fromDet->cellCenter( fromId ).Y() + (j*2-1) * fromSize;
        Gaudi::XYZPoint  corner = Gaudi::XYZPoint(x,y,center.Z());
        LHCb::CaloCellID cornerId  = toDet->Cell( corner );
        if( LHCb::CaloCellID() == cornerId)continue;
        toId=cornerId;
      }   
    }
  }
  if( LHCb::CaloCellID() == toId)return m_cells;
  int pad = 1;
  double x0  = center.X();
  double y0  = center.Y();
  if( fromDet != toDet ){
    double toSize   = toDet->cellSize( toId ) ;
    pad =  (int) floor( fromSize  / toSize + 0.25); // warning int precision
    if(pad < 1)pad=1;
    x0 = center.X() - (pad-1)*fromSize/2./pad;
    y0 = center.Y() - (pad-1)*fromSize/2./pad;  
  }
  for( int i = 0 ; i != pad; ++i){
    for( int j = 0 ; j != pad; ++j){
      double x = x0 + i * fromSize/pad;
      double y = y0 + j * fromSize/pad;    
      Gaudi::XYZPoint  pos(x,y,center.Z());
      if( fromDet != toDet ) toId  = toDet->Cell( pos ) ;
      if( LHCb::CaloCellID() == toId)continue;
      addCell( toId, m_toCalo);
    }        
  }
  return m_cells;
}


// Digits
const std::vector<LHCb::CaloDigit*>& Calo2Calo::digits(LHCb::CaloCellID fromId, std::string toCalo){  
  cellIDs( fromId, toCalo);
  return m_digits;
}
const std::vector<LHCb::CaloDigit*>& Calo2Calo::digits(LHCb::CaloCluster fromCluster, std::string toCalo){  
  cellIDs( fromCluster, toCalo);
  return m_digits;
}  
// Energy
double Calo2Calo::energy(LHCb::CaloCellID fromId, std::string toCalo){
  cellIDs(fromId, toCalo);
  return m_energy;
}
int Calo2Calo::multiplicity(LHCb::CaloCellID fromId, std::string toCalo){
  cellIDs(fromId, toCalo);
  return m_count;
}
double Calo2Calo::energy(LHCb::CaloCluster fromCluster, std::string toCalo){
  cellIDs(fromCluster, toCalo);
  return m_energy;
}
int Calo2Calo::multiplicity(LHCb::CaloCluster fromCluster, std::string toCalo){
  cellIDs(fromCluster, toCalo);
  return m_count;
}


// Additional method : isLocalMax
bool Calo2Calo::isLocalMax(LHCb::CaloDigit digit){
  LHCb::CaloCellID id = digit.cellID();
  std::string calo = CaloCellCode::CaloNameFromNum( id.calo() );
   DeCalorimeter* det   = m_det[ calo ];
   std::vector<LHCb::CaloCellID> neighbors = det->neighborCells( id );
   for(std::vector<LHCb::CaloCellID>::iterator iid = neighbors.begin(); iid!=neighbors.end();++iid){
     LHCb::CaloDigits* digits = get<LHCb::CaloDigits>( m_loc[ calo ] );
     LHCb::CaloDigit* neighbor = digits->object( *iid );
     if( NULL == neighbor )continue;
     if( digit.e() <= neighbor->e() )return false;
   }
   return true;   
}


