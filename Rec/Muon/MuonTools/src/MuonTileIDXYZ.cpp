// $Id: MuonTileIDXYZ.cpp,v 1.15 2002-08-05 12:53:04 dhcroft Exp $
// Include files 
#include <cstdio>
#include <cmath>

// from Gaudi
#include "GaudiKernel/ToolFactory.h"
#include "GaudiKernel/MsgStream.h" 
#include "GaudiKernel/SmartDataPtr.h"

// from MuonDet
#include "MuonDet/DeMuonRegion.h"
#include "MuonDet/DeMuonChamber.h"
#include "MuonDet/DeMuonGasGap.h"


// from MuonKernel
#include "MuonKernel/MuonSystemLayout.h"

// as regarding detector geometry
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/ILVolume.h"
#include "DetDesc/DetectorElement.h"
#include "DetDesc/SolidBox.h"

//From CLHEP
#include "CLHEP/Geometry/Transform3D.h"

// local
#include "MuonTileIDXYZ.h"
#include "MuonTools/MuonChannel.h"
#include "MuonTools/MuonGeometry.h"
#include "MuonTools/IMuonGeometryTool.h"

//-----------------------------------------------------------------------------
// Implementation file for class : MuonTileIDXYZ
//
// 07/03/2002 : David Hutchcroft
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
static const  ToolFactory<MuonTileIDXYZ>          s_factory ;
const        IToolFactory& MuonTileIDXYZFactory = s_factory ; 


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
MuonTileIDXYZ::MuonTileIDXYZ( const std::string& type,
                              const std::string& name,
                              const IInterface* parent )
  : AlgTool ( type, name , parent ) {
  declareInterface<IMuonTileXYZTool>(this);

  MsgStream log(msgSvc(), "MuonTileIDXYZ");
  log << MSG::DEBUG << "Initialising the MuonTile to position tool" << endreq;  
  // Locate the detector service needed by the this tool
  m_DDS = 0;
  if( serviceLocator() ) {
    StatusCode sc=StatusCode::FAILURE;
    sc = serviceLocator()->service( "DetectorDataSvc", m_DDS, true );
  }

  // make the chamber layout 

  MuonLayout R1Layout(MuonGeometry::chamberGridX[0],
                      MuonGeometry::chamberGridY[0]);

  MuonLayout R2Layout(MuonGeometry::chamberGridX[1],
                      MuonGeometry::chamberGridY[1]);

  MuonLayout R3Layout(MuonGeometry::chamberGridX[2],
                      MuonGeometry::chamberGridY[2]);

  MuonLayout R4Layout(MuonGeometry::chamberGridX[3],
                      MuonGeometry::chamberGridY[3]);

  MuonStationLayout chamberStationLayout(R1Layout,R2Layout,
                                         R3Layout,R4Layout);
                                         
  m_chamberSystemLayout = new MuonSystemLayout(chamberStationLayout);

  m_NStation = 0;
  m_NRegion = 0;

}

MuonTileIDXYZ::~MuonTileIDXYZ(){
  delete m_chamberSystemLayout;
}

StatusCode MuonTileIDXYZ::calcTilePos(const MuonTileID& tile, 
                                      double& x, double& deltax,
                                      double& y, double& deltay,
                                      double& z, double& deltaz){
  MsgStream log(msgSvc(), name());

  if( 0 == m_logChanVertGridX.size() ){
    StatusCode sc = fillGridSizes();
    if(!sc){
      return sc;
    }
  }

  // OK how to make this work.....
  // first locate the station and region from the tile
  unsigned int station = tile.station();
  unsigned int region  = tile.region();

  // now compare the layout parameter to possible "levels"
  // currently chamber, logical channel, pad
  if( MuonGeometry::chamberGridX[region] ==  tile.layout().xGrid() && 
      MuonGeometry::chamberGridY[region] ==  tile.layout().yGrid() ){
    // chambers
    log << MSG::DEBUG << "Found a tile laying out chambers" << endreq;
    
    StatusCode sc = getXYZChamberTile(tile,x,deltax,y,deltay,z,deltaz);
    if(!sc.isSuccess()){
      log << MSG::ERROR << "Failed to get xyz from chamber" << endreq;
      return sc;
    }
  }else if( m_padGridX[station*m_NRegion + region] == 
            tile.layout().xGrid() && 
            m_padGridY[station*m_NRegion + region] == 
            tile.layout().yGrid() ){
    // muon pads and logical channels with a 1:1 mapping to pads
    
    log << MSG::DEBUG 
        << "Found a tile laying out pads" 
        << endreq;
    
    StatusCode sc = getXYZPad(tile,x,deltax,y,deltay,z,deltaz);
    if(!sc.isSuccess()){
      log << MSG::ERROR << "Failed to get xyz from chamber" << endreq;
      return sc;
    }
  }else if( m_logChanHorizGridX[station*m_NRegion + region] == 
            tile.layout().xGrid() && 
            m_logChanHorizGridY[station*m_NRegion + region] == 
            tile.layout().yGrid() ){
    // horizontal logical channels            
    
    log << MSG::DEBUG 
        << "Found a tile laying out horizontal logical channels" 
        << endreq;
    
    StatusCode sc = getXYZLogical(tile,x,deltax,y,deltay,z,deltaz);
    if(!sc.isSuccess()){
      log << MSG::ERROR << "Failed to get xyz from chamber" << endreq;
      return sc;
    }
  }else if( m_logChanVertGridX[station*m_NRegion + region] == 
            tile.layout().xGrid() && 
            m_logChanVertGridY[station*m_NRegion + region] == 
            tile.layout().yGrid() ){
    // vertical logical channels            
    
    log << MSG::DEBUG 
        << "Found a tile laying out vertical logical channels" 
        << endreq;
    
    StatusCode sc = getXYZLogical(tile,x,deltax,y,deltay,z,deltaz);
    if(!sc.isSuccess()){
      log << MSG::ERROR << "Failed to get xyz from chamber" << endreq;
      return sc;
    }
  } else if( 1 == tile.layout().xGrid() && 1 == tile.layout().yGrid() ) {

    log << MSG::DEBUG 
        << "Found a tile laying out Twelfths" << endreq;
    
    StatusCode sc = getXYZTwelfth(tile,x,deltax,y,deltay,z,deltaz);
    if(!sc.isSuccess()){
      log << MSG::ERROR << "Failed to get xyz from twelfth" << endreq;
      return sc;
    }

  } else {
    log << MSG::ERROR 
        << "Did not understand the MuonTileID encoding" 
        << " xGrid=" << tile.layout().xGrid() 
        << " yGrid=" << tile.layout().yGrid() 
        <<endreq;
    return StatusCode::FAILURE;
  }
 
  return StatusCode::SUCCESS;
}

StatusCode MuonTileIDXYZ::locateChamberTile(const double& x, 
                                            const double& y, 
                                            const double& z, 
                                            MuonTileID& tile){
  // this code takes an x,y,z position and finds and returns the chamber
  // containing the point: 
  // actually look in box defined by the extent of the active gas gaps

  // this function ignores gas gap pointer
  DeMuonGasGap *tmpGap;
  StatusCode sc = locateChamberTileAndGap(x, y, z, tile, tmpGap);
  return sc;
}


StatusCode MuonTileIDXYZ::locateChamberTileAndGap(const double& x, 
                                                  const double& y, 
                                                  const double& z, 
                                                  MuonTileID& tile,
                                                  DeMuonGasGap* &pGasGap){
  // this code takes an x,y,z position and finds and returns the chamber
  // containing the point: 
  // actually look in box defined by the extent of the active gas gaps
  MsgStream log(msgSvc(), name());

  if( m_stationExtent.empty() ){
    // need to fill the stationExtent
    StatusCode sc = fillStationExtent();
    if(!sc.isSuccess()){
      log << MSG::ERROR << "Something went wrong getting station positions"
          << endreq;
    }
  }

  int station = 0;
  // add 1 mm for saftey against rounding errors
  while(station < m_NStation && 
        ( fabs(m_stationExtent[station].z - z) >= 
          1. + m_stationExtent[station].dz )){
    station++;
  }
  if(station == m_NStation) {
    log << MSG::WARNING << "Could not locate z in a station" << endreq;
    return StatusCode::FAILURE;
  }
    
  // loop over regions to see if we can find one or more that matches
  int region;
  for( region=0; region < m_NStation ; region++){
    // look in twelfths for this region
    int twelfth;
    for( twelfth=0; twelfth<12; twelfth++){
      // look if x,y is in the box extent +1mm for rounding
      if( ( fabs(twelfthExtent(station,region,twelfth).x - x ) <= 
            1. + (twelfthExtent(station,region,twelfth).dx ) &&
            ( fabs(twelfthExtent(station,region,twelfth).y - y ) <= 
              1. + twelfthExtent(station,region,twelfth).dy ) ) ) {
        // have a twelfth to try

        log << MSG::DEBUG << "Trying station:" << station
            << " region:" << region 
            << " twelfth:" << twelfth
            << endreq;

        StatusCode sc = locateGasGapFromXYZ(station,region,twelfth,
                                            x,y,z,tile,pGasGap);
        if( !sc.isSuccess() ) {
          log << MSG::ERROR << "Problem locating chamber tile" << endreq;
          return sc;
        }
        if( tile.isValid() ){
          log << MSG::DEBUG << " Found tile for chamber " << endreq;
          /***************** NOTE EXIT (SUCCESS) FROM ROUTINE IS HERE ******/
          return StatusCode::SUCCESS;
        }
      }
    }
  }
  
  log << MSG::WARNING 
      << " Tried every estimated combination and could not locate chamber"
      << endreq;
  return StatusCode::FAILURE;
}

StatusCode 
MuonTileIDXYZ::locateFEFromXYZ(const double& x, 
                               const double& y, 
                               const double& z, 
                               MuonTileID &chamber,
                               std::vector<MuonChannel> &muonChannels,
                               DeMuonGasGap* &pGasGap){
  MsgStream log(msgSvc(), name());

  // locate the chamber first
  StatusCode sc = locateChamberTileAndGap(x,y,z,chamber,pGasGap);
  if(!sc.isSuccess()){
    log << MSG::ERROR << "Could not locate chamberTile or gap" << endreq;
  }
  // now to determine the position of the FE channel within the chamber
  
  double chamX,chamY,chamZ,chamDx,chamDy,chamDz;
  sc = getXYZChamberTile(chamber,chamX,chamDx,chamY,chamDy,chamZ,chamDz);
  
  double localX = x - (chamX - chamDx); // x of hit within chamber
  double localY = y - (chamY - chamDy); // y of hit within chamber

  // FE sizes are attached to regions, two levels above the gasgaps in the TDS
  IDetectorElement *ide = 
    pGasGap->parentIDetectorElement()->parentIDetectorElement();
  DeMuonRegion *region = dynamic_cast<DeMuonRegion*>(ide);
  if(!region){
    log << MSG::ERROR << "Grandparent of " << pGasGap->name()
        << " could not be found " << endreq;
  }


  if(region->FEAnodeX() > 0){
    // there is an Anode readout in this region
    double padSizeX = (2.*chamDx)/(static_cast<double>(region->FEAnodeX()));
    double padSizeY = (2.*chamDy)/(static_cast<double>(region->FEAnodeY()));
    
    int xPos = static_cast<int>(floor(localX/padSizeX));
    int yPos = static_cast<int>(floor(localY/padSizeY));

    muonChannels.push_back(MuonChannel(xPos,yPos,
                                       MuonParameters::Anode));
  }
    
  if(region->FECathodeX() > 0){
    // there is an Cathode readout in this region
    double padSizeX = (2.*chamDx)/(static_cast<double>(region->FECathodeX()));
    double padSizeY = (2.*chamDy)/(static_cast<double>(region->FECathodeY()));

    int xPos = static_cast<int>(floor(localX/padSizeX));
    int yPos = static_cast<int>(floor(localY/padSizeY));

    muonChannels.push_back(MuonChannel(xPos,yPos,
                                       MuonParameters::Cathode));
  }
  
  return StatusCode::SUCCESS;
   
}

StatusCode MuonTileIDXYZ::getXYZChamberTile(const MuonTileID& tile, 
                                            double& x, double& deltax,
                                            double& y, double& deltay,
                                            double& z, double& deltaz){
  MsgStream log(msgSvc(), name());
  
  unsigned int chamberNum = getChamberNumber(tile);

  unsigned int station = tile.station();
  unsigned int region  = tile.region();

  StatusCode sc = getXYZChamber(station,region,chamberNum,
                                x,deltax,y,deltay,z,deltaz);
  if(!sc.isSuccess()){
    log << MSG::ERROR << "Failed to get chamber " << chamberNum << endreq;
  }

  return StatusCode::SUCCESS;
}

StatusCode MuonTileIDXYZ::getXYZChamber(const int& station,
                                        const int& region,
                                        const int& chamberNum,
                                        double& x, double& deltax,
                                        double& y, double& deltay,
                                        double& z, double& deltaz){
  StatusCode sc = getXYZAndCache(station,region,chamberNum,-1,
                                 x,deltax,y,deltay,z,deltaz);
  return sc;
}

StatusCode MuonTileIDXYZ::getXYZGasGap(const int& station,
                                       const int& region,
                                       const int& chamberNum,
                                       const int& gapNum,
                                       double& x, double& deltax,
                                       double& y, double& deltay,
                                       double& z, double& deltaz){
  StatusCode sc = getXYZAndCache(station,region,chamberNum,gapNum,
                                 x,deltax,y,deltay,z,deltaz);
  return sc;
}

StatusCode MuonTileIDXYZ::getXYZAndCache(const int& station,
                                         const int& region,
                                         const int& chamberNum,
                                         const int& gapNum,
                                         double& x, double& deltax,
                                         double& y, double& deltay,
                                         double& z, double& deltaz){
  MsgStream log(msgSvc(), name());

  // Alright I agree this is officially very ugly code
  // I probably should use some stl stuff here somewhere
  // this pointer arimetic is just asking for trouble

  if( m_NGap.empty() ){
    StatusCode sc = fillNGaps();
    if(!sc){
      return sc;
    }
  }
  int nGap = m_NGap[station*(m_NRegion) + region];
  chamberExtent_ *currCham = &chamExtent(station,region,chamberNum);

  // test if have information already  
  if( 0. >= currCham->z ) {
    //TDS path t chamber is of the form /dd/Structure/LHCb/Muon/M1/R1/Cham001
    // Name this chamber
    char charPath[80];
    sprintf(charPath,"/dd/Structure/LHCb/Muon/M%1i/R%1i/Cham%03i",
            station+1,region+1,chamberNum);
    std::string chamberString = charPath;

    log << MSG::DEBUG << "asking TDS for " << charPath << endreq;
    SmartDataPtr<DeMuonChamber> muChamber(m_DDS, chamberString);
    if(!muChamber){
      log << MSG::ERROR << "Could not read chamber "
          << chamberString << " from TDS" << endreq;
      return StatusCode::FAILURE;
    }

    currCham->pChamber = muChamber;

    // active volume is defined by the gas gaps, want first and last
    IDetectorElement::IDEContainer::const_iterator itGap;
    int gapIndex;
    for(itGap = muChamber->childBegin(), gapIndex = 0; 
        itGap != muChamber->childEnd();
        itGap++, gapIndex++){
      log << MSG::DEBUG << "asking TDS for " << (*itGap)->name()
          << endreq;    
      SmartDataPtr<DeMuonGasGap> muGap(m_DDS,(*itGap)->name());
      if( !muGap ){
        log << MSG::ERROR << "Could not read gas gaps "
            << (*itGap)->name() << " from TDS" << endreq;
        return StatusCode::FAILURE;
      }
      gapExtent_ *currGap = &gapExtent(station,region,chamberNum,gapIndex);
      
      currGap->pGasGap = muGap;

      HepTransform3D vTransForm = muGap->geometry()->matrixInv();
      Hep3Vector vtrans = vTransForm.getTranslation();
      
      currGap->x = vtrans.x();
      currGap->y = vtrans.y();
      currGap->z = vtrans.z();

      // get ILVolume pointer
      const ILVolume *logVol = muGap->geometry()->lvolume();
      // Get the solid
      const ISolid *solid = logVol->solid();
      // check these really are boxes (they ought to be!)
      const SolidBox *box = dynamic_cast<const SolidBox *>(solid);
      if( !box ){
        log << MSG::ERROR << "Could not cast gas gap solid to box" 
            << endreq;
        return StatusCode::FAILURE;
      }
    
      currGap->dx = box->xHalfLength();
      currGap->dy = box->yHalfLength();
      currGap->dz = box->zHalfLength();
    }    

    currCham->x = ( gapExtent(station,region,chamberNum,0).x + 
                    gapExtent(station,region,chamberNum,nGap-1).x ) / 2.0;
    currCham->y = ( gapExtent(station,region,chamberNum,0).y + 
                    gapExtent(station,region,chamberNum,nGap-1).y ) / 2.0;
    currCham->z = ( gapExtent(station,region,chamberNum,0).z + 
                    gapExtent(station,region,chamberNum,nGap-1).z ) / 2.0;

    log << MSG::DEBUG 
        << " chamber average of first and last gas gaps reported at x="
        << currCham->x << "mm y= " << currCham->y 
        << "mm z="  << currCham->z << "mm" << endreq;

    currCham->dx = gapExtent(station,region,chamberNum,0).dx;
    currCham->dy = gapExtent(station,region,chamberNum,0).dy;
    currCham->dz = 
      (fabs( gapExtent(station,region,chamberNum,0).z - 
             gapExtent(station,region,chamberNum,nGap-1).z )/2.0)
      + gapExtent(station,region,chamberNum,0).dz;
    
    log << MSG::DEBUG 
        << " chamber size x/2=" << currCham->dx << "mm "
        << "y/2 =" << currCham->dy << "mm "
        << "z/2 =" << currCham->dz << "mm " 
        << endreq;
  }
  
  if( -1 == gapNum ){
    x = currCham->x;
    y = currCham->y;
    z = currCham->z;
    deltax = currCham->dx;
    deltay = currCham->dy;
    deltaz = currCham->dz;
  }else{
    x = gapExtent(station,region,chamberNum,gapNum).x;
    y = gapExtent(station,region,chamberNum,gapNum).y;
    z = gapExtent(station,region,chamberNum,gapNum).z;
    deltax = gapExtent(station,region,chamberNum,gapNum).dx;
    deltay = gapExtent(station,region,chamberNum,gapNum).dy;
    deltaz = gapExtent(station,region,chamberNum,gapNum).dz;
  }
  
  return StatusCode::SUCCESS;
}

StatusCode MuonTileIDXYZ::fillNGaps(){
  MsgStream log(msgSvc(), name());
  
  // TDS path to the Muon system  is of the form /dd/Structure/LHCb/Muon
  SmartDataPtr<DetectorElement> muonSystem(m_DDS,"/dd/Structure/LHCb/Muon");
  if(!muonSystem){
    log << MSG::ERROR << "Could not read /dd/Structure/LHCb/Muon from TDS" 
        << endreq;
    return StatusCode::FAILURE;
  }

  // Childern of the Muon system detector element are the stations
  std::vector<IDetectorElement*> muonStations = 
    muonSystem->childIDetectorElements();
  
  m_NStation = muonStations.size();

  // loop over the stations
  std::vector<IDetectorElement*>::const_iterator iIDE;
  int station;
  for(iIDE = muonStations.begin(), station = 0 ; 
      iIDE != muonStations.end() ; 
      iIDE++, station++){

    // now get the regions for this station
    std::vector<IDetectorElement*> muonRegions = 
      (*iIDE)->childIDetectorElements();

    m_NRegion = muonRegions.size();
    
    // loop over the regions
    std::vector<IDetectorElement*>::const_iterator iIDER;
    int region;
    for(iIDER = muonRegions.begin(), region = 0 ; 
        iIDER != muonRegions.end() ; 
        iIDER++, region++ ){

      // now get the first chamber of this region
      std::vector<IDetectorElement*> muonChambers = 
        (*iIDER)->childIDetectorElements();

      // muonGaps array from chamber
      std::vector<IDetectorElement*> muonGaps = 
        muonChambers[0]->childIDetectorElements();      

      log << MSG::DEBUG << "Number of gaps from chamber " 
          << muonChambers[0]->name() << " is " << muonGaps.size() << endreq;

      // insert order is [station*(m_Region) + region]
      m_NGap.push_back(muonGaps.size()); 
    }
  }

  // null the chamExtent and gapExtent vectors
  chamberExtent_ nullChamExtent;
  nullChamExtent.x=0.;
  nullChamExtent.y=0.;
  nullChamExtent.z=0.;
  nullChamExtent.dx=0.;
  nullChamExtent.dy=0.;
  nullChamExtent.dz=0.;
  nullChamExtent.pChamber = 0;
  
  // make m_chamExtent the correct length, double vector format here
  m_chamExtent.clear();
  m_chamExtent.resize(m_NStation*m_NRegion);
  for( station = 0 ; station < m_NStation ; station++ ) {
    int region;
    for( region = 0 ; region < m_NRegion ; region++ ) {
      m_chamExtent[station*m_NRegion + region].
        resize(numberOfChambers(region), nullChamExtent);
    }
  }
  

  // same for m_gapExtent
  gapExtent_ nullGapExtent;
  nullGapExtent.x=0.;
  nullGapExtent.y=0.;
  nullGapExtent.z=0.;
  nullGapExtent.dx=0.;
  nullGapExtent.dy=0.;
  nullGapExtent.dz=0.;
  nullGapExtent.pGasGap = 0;

  // same for gas gaps
  m_gapExtent.clear();
  m_gapExtent.resize(m_NStation*m_NRegion);
  for( station = 0 ; station < m_NStation ; station++ ) {
    int region;
    for( region = 0 ; region < m_NRegion ; region++ ) {
      m_gapExtent[station*m_NRegion + region].
        resize(numberOfChambers(region) * 
               m_NGap[station*m_NRegion + region], nullGapExtent);
    }
  }
  
  return StatusCode::SUCCESS;
}
 
StatusCode MuonTileIDXYZ::fillGridSizes(){
  
  MsgStream log(msgSvc(), name());

  IMuonGeometryTool *m_geomTool;

  // get geometry tool for pad and channel sizes
  StatusCode sc =
    toolSvc()->retrieveTool("MuonGeometryTool", m_geomTool);
  if( sc.isFailure() ) {
    log << MSG::FATAL << "    Unable to create MuonGeometryTool " << endreq;
    return sc;
  }
  
  // Check m_NStation and m_NRegion are OK
  if( 0 == m_NStation ){
    sc = fillNGaps();
    if(!sc){
      return sc;
    }
  }

  std::cout << "fillGridSizes sizes : " << m_NStation
            <<" "<< m_NRegion << std::endl;

  // make arrays correct size
  m_logChanVertGridX.resize(m_NStation*m_NRegion);
  m_logChanVertGridY.resize(m_NStation*m_NRegion);
  m_logChanHorizGridX.resize(m_NStation*m_NRegion);
  m_logChanHorizGridY.resize(m_NStation*m_NRegion);
  m_padGridX.resize(m_NStation*m_NRegion);
  m_padGridY.resize(m_NStation*m_NRegion);

  std::cout << " logChanVertGridX : " << m_logChanVertGridX.size()
            << " logChanVertGridY : " << m_logChanVertGridY.size()
            << " logChanHorizGridX : " << m_logChanHorizGridX.size()
            << " logChanHorizGridY : " << m_logChanHorizGridY.size()
            << " padGridX : " << m_padGridX.size()
            << " padGridY : " << m_padGridY.size() 
            << std::endl;


  int station,region;
  for( station = 0 ; station < m_NStation ; station++ ){
    for( region = 0 ; region < m_NRegion ; region++ ){

      // get sizes from MuonGeometryTool
      m_logChanVertGridX[station*m_NRegion + region] = 
        m_geomTool->logChanVertGridX(station,region);

      m_logChanVertGridY[station*m_NRegion + region] = 
        m_geomTool->logChanVertGridY(station,region);

      m_logChanHorizGridX[station*m_NRegion + region] = 
        m_geomTool->logChanHorizGridX(station,region);

      m_logChanHorizGridY[station*m_NRegion + region] = 
        m_geomTool->logChanHorizGridY(station,region);

      m_padGridX[station*m_NRegion + region] = 
        m_geomTool->padGridX(station,region);

      m_padGridY[station*m_NRegion + region] = 
        m_geomTool->padGridY(station,region);

    }
  }
  return StatusCode::SUCCESS;
  
}
 

StatusCode MuonTileIDXYZ::fillStationExtent(){
  
  MsgStream log(msgSvc(), name());
  
  // This uses the twelfths to get the size of the station
  if( m_twelfthExtent.empty() ){
    //need to fill twelfthExtent
    StatusCode sc = fillTwelfthsExtent();
    if(!sc.isSuccess()){
      log << MSG::ERROR << "Something went wrong getting Twelfths positions"
          << endreq;
      return sc;
    }
  }
  // So get the TDS representations of the stations
  // to fill stationExtent with the outer edges of the chambers in the
  // corners of region 4
  
  m_stationExtent.resize(m_NStation);

  int station;
  for(station = 0 ; station < m_NStation ; station++){

    double xMax,xMin,yMax,yMin,zMax,zMin;

    int region;
    for( region = 0 ; region < m_NRegion ; region++ ){

      int twelfth;
      for( twelfth = 0 ; twelfth < 12 ; twelfth++ ){
        
        // fill the local x,y,z mans/min for the twelfth
        double txMax = twelfthExtent(station,region,twelfth).x + 
          twelfthExtent(station,region,twelfth).dx;
        double txMin = twelfthExtent(station,region,twelfth).x -
          twelfthExtent(station,region,twelfth).dx;
        double tyMax = twelfthExtent(station,region,twelfth).y + 
          twelfthExtent(station,region,twelfth).dy;
        double tyMin = twelfthExtent(station,region,twelfth).y -
          twelfthExtent(station,region,twelfth).dy;
        double tzMax = twelfthExtent(station,region,twelfth).z + 
          twelfthExtent(station,region,twelfth).dz;
        double tzMin = twelfthExtent(station,region,twelfth).z -
          twelfthExtent(station,region,twelfth).dz;

        if(0 == twelfth && 0 == region) {
          xMax = txMax;
          xMin = txMin;
          yMax = tyMax;
          yMin = tyMin;
          zMax = tzMax;
          zMin = tzMin;
        }else{
          if ( txMax > xMax ) { xMax = txMax; }
          if ( txMin < xMin ) { xMin = txMin; }
          if ( tyMax > yMax ) { yMax = tyMax; }
          if ( tyMin < yMin ) { yMin = tyMin; }
          if ( tzMax > zMax ) { zMax = tzMax; }
          if ( tzMin < zMin ) { zMin = tzMin; }
        }
      }
    }
    
    m_stationExtent[station].x = ( xMax + xMin ) / 2.; // all these should
    m_stationExtent[station].y = ( yMax + yMin ) / 2.; // be zero
    m_stationExtent[station].z = ( zMax + zMin ) / 2.; // diff due to rounding

    m_stationExtent[station].dx = ( xMax - xMin ) / 2.;
    m_stationExtent[station].dy = ( yMax - yMin ) / 2.;
    m_stationExtent[station].dz = ( zMax - zMin ) / 2.;

    log << MSG::DEBUG 
        << " Station " << station
        << " position x=" << m_stationExtent[station].x << "mm "
        << "y =" << m_stationExtent[station].y << "mm "
        << "z =" << m_stationExtent[station].z << "mm " 
        << endreq;

    log << MSG::DEBUG 
        << " Station " << station
        << " size x/2=" << m_stationExtent[station].dx << "mm "
        << "y/2 =" << m_stationExtent[station].dy << "mm "
        << "z/2 =" << m_stationExtent[station].dz << "mm " 
        << endreq;
  }

  return StatusCode::SUCCESS;
}

StatusCode MuonTileIDXYZ::fillTwelfthsExtent(){

  MsgStream log(msgSvc(), name());
  
  // check station sizes are OK
  if( m_NGap.empty() ){
    StatusCode sc = fillNGaps();
    if(!sc){
      return sc;
    }
  }

  muonExtent_ nullExtent;
  nullExtent.x = 0.;
  nullExtent.y = 0.;
  nullExtent.z = 0.;
  nullExtent.dx = 0.;
  nullExtent.dy = 0.;
  nullExtent.dz = 0.;

  m_twelfthExtent.resize(m_NStation*m_NRegion*12,nullExtent);  

  // So get the TDS representations of the stations
  // to fill twelfthsExtent and quaterExtent with the outer 
  // edges of the chambers in the
  // corners of the twelfths
  
  int station;
  for(station = 0 ; station < m_NStation ; station++){

    int region;
    for(region = 0 ; region < m_NRegion ; region++){

      int twelfth;
      for(twelfth = 0 ; twelfth < 12 ; twelfth++){
          
        double xMax,xMin,yMax,yMin,zMax,zMin;
        double x,y,z;
        double dx,dy,dz;
        
        int chamCorner; // only need 3 corners here
        for( chamCorner = 0 ; chamCorner < 4 ; chamCorner++ ){
          int nChamber = getTwelfthCorner(region,twelfth,chamCorner);
          // get the chamber
          StatusCode sc = 
            getXYZChamber(station,region,nChamber,x,dx,y,dy,z,dz);
          if(!sc.isSuccess()){
            log << MSG::ERROR << "Could not get corner chamber" << endreq;
            return sc;
          }
          
          if(0 == chamCorner) {
            xMax = x;
            xMin = x;
            yMax = y;
            yMin = y;
            zMax = z;
            zMin = z;
          }else{
            if ( x > xMax ) { xMax = x; }
            if ( x < xMin ) { xMin = x; }
            if ( y > yMax ) { yMax = y; }
            if ( y < yMin ) { yMin = y; }
            if ( z > zMax ) { zMax = z; }
            if ( z < zMin ) { zMin = z; }
          }
        } // end loop over chamCorner
        
        // strore the extent of the twelfth
        twelfthExtent(station,region,twelfth).x = 
          ( xMax + xMin ) / 2.; 
        twelfthExtent(station,region,twelfth).y = 
          ( yMax + yMin ) / 2.; 
        twelfthExtent(station,region,twelfth).z = 
          ( zMax + zMin ) / 2.; 
          
        twelfthExtent(station,region,twelfth).dx = 
          (( xMax - xMin ) / 2.) + dx;
        twelfthExtent(station,region,twelfth).dy = 
          (( yMax - yMin ) / 2.) + dy;
        twelfthExtent(station,region,twelfth).dz = 
          (( zMax - zMin ) / 2.) + dz;
          
      }// end of loop over twelfths


    }// end loop over regions

  }//end loop over stations

  return StatusCode::SUCCESS;
}

int MuonTileIDXYZ::getTwelfthCorner(const int& region, 
                                    const int& twelfth,
                                    const int& chamberNum){
  int xPos=0;
  int yPos=0;
  getTwelfthCornerIndex(region,twelfth,chamberNum,xPos,yPos);
  // get the actual chamber number
  if(0 == region){
    return MuonGeometry::chamberR1[xPos][yPos];
  }else if(1 == region){
    return MuonGeometry::chamberR2[xPos][yPos];
  }else if(2 == region){
    return MuonGeometry::chamberR3[xPos][yPos];
  }else{
    return MuonGeometry::chamberR4[xPos][yPos];
  }
}

void MuonTileIDXYZ::getTwelfthCornerIndex(const int& region, 
                                          const int& twelfth,
                                          const int& chamberNum,
                                          int &xPos, int &yPos){
  // chambers defining the corners of the twelfths
  // here the indexing in region, quater, twelfth
  // numbering clockwise:
  //  
  //  +-------+ +-------+           ChamberNum
  //  | 4   3 | | 2   1 |           counts (no particular reason) 
  //  |   +---+ +---+   |           +------+
  //  | 5 |         | 0 |           | 2  3 |
  //  +---+         +---+           | 0  1 |
  //  +---+         +---+           +------+
  //  | 6 |         |11 |
  //  |   +---+ +---+   |
  //  | 7   8 | | 9  10 |    
  //  +-------+ +-------+
  // map to whole station numbering scheme

  if(0 == twelfth){
    xPos   = 3*MuonGeometry::chamberGridX[region];
    yPos   = 2*MuonGeometry::chamberGridY[region];
  }else if(1 == twelfth){
    xPos   = 3*MuonGeometry::chamberGridX[region];
    yPos   = 3*MuonGeometry::chamberGridY[region];
  }else if(2 == twelfth){
    xPos   = 2*MuonGeometry::chamberGridX[region];
    yPos   = 3*MuonGeometry::chamberGridY[region];
  }else if(3 == twelfth){
    xPos   = 1*MuonGeometry::chamberGridX[region];
    yPos   = 3*MuonGeometry::chamberGridY[region];
  }else if(4 == twelfth){
    xPos   = 0;
    yPos   = 3*MuonGeometry::chamberGridY[region];
  }else if(5 == twelfth){
    xPos   = 0;
    yPos   = 2*MuonGeometry::chamberGridY[region];
  }else if(6 == twelfth){
    xPos   = 0;
    yPos   = 1*MuonGeometry::chamberGridY[region];
  }else if(7 == twelfth){
    xPos   = 0;
    yPos   = 0;
  }else if(8 == twelfth){
    xPos   = 1*MuonGeometry::chamberGridX[region];
    yPos   = 0;
  }else if(9 == twelfth){
    xPos   = 2*MuonGeometry::chamberGridX[region];
    yPos   = 0;
  }else if(10 == twelfth){
    xPos   = 3*MuonGeometry::chamberGridX[region];
    yPos   = 0;
  }else{
    xPos   = 3*MuonGeometry::chamberGridX[region];
    yPos   = 1*MuonGeometry::chamberGridY[region];
  }

  // correct to positions of 1,2,3
  if(1 == chamberNum){
    xPos = xPos + (MuonGeometry::chamberGridX[region]-1);
  }else if(2 == chamberNum){
    yPos = yPos + (MuonGeometry::chamberGridY[region]-1);
  }else if(3 == chamberNum){
    xPos = xPos + (MuonGeometry::chamberGridX[region]-1);
    yPos = yPos + (MuonGeometry::chamberGridY[region]-1);
  }
}

int MuonTileIDXYZ::getChamberNumber(const MuonTileID& tile){

  MsgStream log(msgSvc(), name());
  // look-up chamber number from tile
  // use the fact that the chamber numbering is constitent between stations
  
  // quarter counts 
  //      +y          
  //    3 | 0
  //   ---+--- +x
  //    2 | 1
  // in quarter 0 x=x,               y=y
  //            1 x=x                y=Size_y - 1 - y
  //            2 x=Size_x - 1 - x   y=Size_y - 1 - y
  //            3 x=Size_x - 1 - x   y=y
  //
  // size_x = 2*xGrid  (xGrid count across only a twelfth of a region)
  // same for y

  unsigned int chamberNumber; // counts from 1 as per TDS
  // position of chamber number in arrays 
  unsigned int xIndex,yIndex; 
  
  if ( 0 == tile.quarter() ){
    xIndex = (2*tile.layout().xGrid()) + tile.nX();
    yIndex = (2*tile.layout().yGrid()) + tile.nY();
  } else if ( 1 == tile.quarter() ){
    xIndex = (2*tile.layout().xGrid()) + tile.nX();
    yIndex = (2*tile.layout().yGrid()) - (1+tile.nY());
  } else if ( 2 == tile.quarter() ){
    xIndex = (2*tile.layout().xGrid()) - (1+tile.nX());
    yIndex = (2*tile.layout().yGrid()) - (1+tile.nY());
  } else if ( 3 == tile.quarter() ){
    xIndex = (2*tile.layout().xGrid()) - (1+tile.nX());
    yIndex = (2*tile.layout().yGrid()) + tile.nY();
  } 
  
  if( 0 == tile.region() ) {
    chamberNumber = 
      MuonGeometry::chamberR1[xIndex][yIndex];
  }else if ( 1 == tile.region() ){
    chamberNumber = 
      MuonGeometry::chamberR2[xIndex][yIndex];
  }else if ( 2 == tile.region() ){
    chamberNumber = 
      MuonGeometry::chamberR3[xIndex][yIndex];
  }else if ( 3 == tile.region() ){
    chamberNumber = 
      MuonGeometry::chamberR4[xIndex][yIndex];
  }

  log << MSG::DEBUG << "Chamber " << chamberNumber 
      << " in R" << tile.region()+1
      << " xIndex " << xIndex
      << " yIndex " << yIndex
      << " was found for tile " << tile << endreq;
  
  return chamberNumber;
}

StatusCode MuonTileIDXYZ::getXYZPad(const MuonTileID& tile, 
                                    double& x, double& deltax,
                                    double& y, double& deltay,
                                    double& z, double& deltaz){
  MsgStream log(msgSvc(), name());

  // to find the x,y,z of the pad one must first find the chamber
  // to find the chamber one must know the tile of the chamber
  unsigned int station = tile.station();
  unsigned int region  = tile.region();
  
  MuonLayout chambersLayout = 
    m_chamberSystemLayout->regionLayout(station,region);
  // locate the chamber
  MuonTileID chamTile = chambersLayout.contains(tile);

  // chamber size in x and y (z is same as pad)
  double cx,cy,cDeltax,cDeltay;
  
  StatusCode sc = getXYZChamberTile(chamTile,cx,cDeltax,cy,cDeltay,z,deltaz);
  if(!sc.isSuccess()){
    log << MSG::ERROR << "Failed to get chamber for pad tile: " << tile
        << endreq;
    return sc;
  }

  // number of pads or logical channels in a chamber is : 
  // layout.xGrid() / chamberLayoutX , padLayoutY / chamberLayoutY
  int xRatio = tile.layout().xGrid() / 
    MuonGeometry::chamberGridX[region];
  int yRatio = tile.layout().yGrid() / 
    MuonGeometry::chamberGridY[region];

  // need to work out offset within chamber
  // initally ignore the "reflectional" nature of the quarters
  int xOffset = tile.nX() - (xRatio * chamTile.nX());
  int yOffset = tile.nY() - (yRatio * chamTile.nY());
  
  // do the reflections of the tileID structure into Cartesian coordinates
  if ( 0 == tile.quarter() ){
    xOffset = xOffset;
    yOffset = yOffset;
  } else if ( 1 == tile.quarter() ){
    xOffset = xOffset;
    yOffset = yRatio - (1 + yOffset);
  } else if ( 2 == tile.quarter() ){
    xOffset = xRatio - (1 + xOffset);
    yOffset = yRatio - (1 + yOffset);
  } else if ( 3 == tile.quarter() ){
    xOffset = xRatio - (1 + xOffset);
    yOffset = yOffset;
  } 

  log << MSG::DEBUG << "Pad xOffset within chamber:" << xOffset 
      << " yOffset within chamber:" << yOffset 
      << " xratio: " << xRatio << " yratio: " << yRatio
      << endreq;
  
  // now make the calculations of x and z (and dx and dy)
  
  x = ( cx - cDeltax ) + 
    ( ( static_cast<double>(xOffset) / static_cast<double>(xRatio) ) *
      (2.0 * cDeltax ) );
  y = ( cy - cDeltay ) + 
    ( ( static_cast<double>(yOffset) / static_cast<double>(yRatio) ) *
      (2.0 * cDeltay ) );
  
  deltax = cDeltax / (static_cast<double>(xRatio));
  deltay = cDeltay / (static_cast<double>(yRatio));

  // correct to the center of the pad
  x = x + deltax;
  y = y + deltay;

  return StatusCode::SUCCESS;
}  

StatusCode MuonTileIDXYZ::getXYZTwelfth(const MuonTileID& tile, 
                                        double& x, double& deltax,
                                        double& y, double& deltay,
                                        double& z, double& deltaz){
  MsgStream log(msgSvc(), name());

  // This uses the twelfths to get the size of the station
  if( m_twelfthExtent.empty() ){
    //need to fill twelfthExtent
    StatusCode sc = fillTwelfthsExtent();
    if(!sc.isSuccess()){
      log << MSG::ERROR << "Something went wrong getting Twelfths positions"
          << endreq;
      return sc;
    }
  }

  unsigned int station = tile.station();
  unsigned int region  = tile.region();
  unsigned int quarter = tile.quarter();

  unsigned int twelfth;
  if(0 == quarter){
    if(1 == tile.nX() && 0 == tile.nY()){
      twelfth = 0;
    }else if(1 == tile.nX() && 1 == tile.nY()){
      twelfth = 1;
    }else{
      twelfth = 2;
    }
  }else if(1 == quarter){
    if(0 == tile.nX() && 1 == tile.nY()){
      twelfth = 3;
    }else if(1 == tile.nX() && 1 == tile.nY()){
      twelfth = 4;
    }else{
      twelfth = 5;
    }
  }else if(2 == quarter){
    if(1 == tile.nX() && 0 == tile.nY()){
      twelfth = 6;
    }else if(1 == tile.nX() && 1 == tile.nY()){
      twelfth = 7;
    }else{
      twelfth = 8;
    }
  }else{
    if(0 == tile.nX() && 1 == tile.nY()){
      twelfth = 9;
    }else if(1 == tile.nX() && 1 == tile.nY()){
      twelfth = 10;
    }else{
      twelfth = 11;
    }
  }

  x = twelfthExtent(station,region,twelfth).x;
  deltax = twelfthExtent(station,region,twelfth).dx;
  y = twelfthExtent(station,region,twelfth).y;
  deltay = twelfthExtent(station,region,twelfth).dy;
  z = twelfthExtent(station,region,twelfth).z;
  deltaz = twelfthExtent(station,region,twelfth).dz;
  
  return StatusCode::SUCCESS;
}  

StatusCode MuonTileIDXYZ::getXYZLogical(const MuonTileID& tile, 
                                        double& x, double& deltax,
                                        double& y, double& deltay,
                                        double& z, double& deltaz){
  MsgStream log(msgSvc(), name());

  // If we get here then the logical strip is potenitally bigger than a chamber
  // if not then we can subcontract to getXYZPad directly

  // to find the x,y,z of the pad one must first find the chamber
  // to find the chamber one must know the tile of the chamber

  unsigned int station = tile.station();
  unsigned int region  = tile.region();
  
  MuonLayout chambers = m_chamberSystemLayout->regionLayout(station,region);

  if( tile.layout().xGrid() >= chambers.xGrid() && 
      tile.layout().yGrid() >= chambers.yGrid() ) {
    // ok logical channels are within a single chamber, delegate to getXYZpad
    StatusCode sc = getXYZPad(tile,x,deltax,y,deltay,z,deltaz);
    if(!sc.isSuccess()){
      log << MSG::ERROR << "Passed a logical channel to getXYZPad: failed"
          << " tile ID=" << tile << endreq;
      return sc;
    }
  }else if(tile.layout().xGrid() >= chambers.xGrid() &&
           tile.layout().yGrid() < chambers.yGrid() ) {
    // This logical channel crosses chambers in y, break it down into 
    // chamber hight pads then combine the chamber extents later
    // number of tiles to make:
    int nTile = chambers.yGrid() / tile.layout().yGrid();
    log << MSG::DEBUG << "Making " << nTile << " temporary MuonTileIDs"
        << " to get all chamber locations"
        << endreq;

    // width of pad (in x) is the same, hight (yGrid) that of the chamber
    MuonLayout tempPadLayout(tile.layout().xGrid(),chambers.yGrid());
    
    std::vector<MuonTileID> tempTiles;

    int i;
    for( i=0 ; i<nTile ; i++ ){
      int yTile = ( tile.nY() * nTile ) + i ;
      MuonTileID tTile(station,0,0,tempPadLayout,region,tile.quarter(),
                       tile.nX(),yTile);
      tempTiles.push_back(tTile);
    }
    
    // loop over the tiles and get max/min x,y,z of the combinations
    double xmin, xmax, ymin, ymax, zmin, zmax;
    double padDx,chamDy,chamDz;   
    std::vector<MuonTileID>::const_iterator iTile;
    for(iTile = tempTiles.begin() ; iTile != tempTiles.end() ; iTile++){
      double lx,ly,lz;
      StatusCode sc = getXYZPad(*iTile,lx,padDx,ly,chamDy,lz,chamDz);
      if(!sc.isSuccess()){
        log << MSG::ERROR << "Passed a temporary channel to getXYZPad: failed"
            << " tile ID=" << *iTile << endreq;
        return sc;
      }      
      if(iTile == tempTiles.begin()){
        xmin = lx;
        xmax = lx;
        ymin = ly;
        ymax = ly;
        zmin = lz;
        zmax = lz;
      }else{
        if( xmax < lx ) { xmax = lx; }
        if( xmin > lx ) { xmin = lx; }
        if( ymax < ly ) { ymax = ly; }
        if( ymin > ly ) { ymin = ly; }
        if( zmax < lz ) { zmax = lz; }
        if( zmin > lz ) { zmin = lz; }
      }
    }
    
    x = (xmax + xmin) / 2.0;
    y = (ymax + ymin) / 2.0;
    z = (zmax + zmin) / 2.0;
    
    deltax = ((xmax - xmin)/2.0) + padDx;
    deltay = ((ymax - ymin)/2.0) + chamDy;
    deltaz = ((zmax - zmin)/2.0) + chamDz;

  }else{
    log << MSG::WARNING 
        << "You requested a logical channel wider than a chamber, failing" 
        << endreq;
    return StatusCode::FAILURE;
  }

  return StatusCode::SUCCESS;
}  


StatusCode MuonTileIDXYZ::locateGasGapFromXYZ( const int& station,
                                               const int& region,
                                               const int& twelfth,
                                               const double& x,
                                               const double& y,
                                               const double& z,
                                               MuonTileID& tile,
                                               DeMuonGasGap* &pGasGap){
  MsgStream log(msgSvc(), name());
  // OK this is the sledgehammer approach (try everything until it works)

  int ixBegin,ixEnd; // loop indices in X
  int iyBegin,iyEnd; // loop indices in Y

  // map to whole station numbering scheme
  getTwelfthCornerIndex(region,twelfth,0,ixBegin,iyBegin);
  getTwelfthCornerIndex(region,twelfth,3,ixEnd,iyEnd);

  // correct to be after last chamber
  ixEnd++;
  iyEnd++;

  // loop over chamber indices
  int ix,iy;
  for(ix = ixBegin; ix < ixEnd ; ix++){
    for(iy= iyBegin; iy < iyEnd ; iy++){
      int nCham;
      if(0 == region){
        nCham = MuonGeometry::chamberR1[ix][iy];
      }else if(1 == region){
        nCham = MuonGeometry::chamberR2[ix][iy];
      }else if(2 == region){
        nCham = MuonGeometry::chamberR3[ix][iy];
      }else{
        nCham = MuonGeometry::chamberR4[ix][iy];
      }
      
      double chamX,chamY,chamZ;
      double chamDx,chamDy,chamDz;
      StatusCode sc = getXYZChamber(station,region,nCham,
                                    chamX,chamDx,chamY,chamDy,chamZ,chamDz);
      if(!sc.isSuccess()){
        log << MSG::ERROR
            << "Could not get chamber: " << nCham 
            << " in station " << station
            << " region " << region
            << endreq;
        return sc;
      }

      if( (fabs(x-chamX) < 0.1 + chamDx) &&
          (fabs(y-chamY) < 0.1 + chamDy) &&
          (fabs(z-chamZ) < 0.1 + chamDz) ) {
        // at last... have the correct chamber, now to make a tile from it

        int xTile,yTile;
        // need to do the reflections to quarter 1 again....
        int quarter;
        
        if( 0 == twelfth || 1 == twelfth || 2 == twelfth){ // Q=0
          quarter = 0;
          xTile = ix - (2*MuonGeometry::chamberGridX[region]);
          yTile = iy - (2*MuonGeometry::chamberGridY[region]);
        } else if ( 9 == twelfth || 10 == twelfth || 11 == twelfth){ // Q=1
          quarter = 1;
          xTile = ix - (2*MuonGeometry::chamberGridX[region]);
          yTile = (2*MuonGeometry::chamberGridY[region]) - (1+iy);
        } else if ( 6 == twelfth || 7 == twelfth || 8 == twelfth){ // Q=2
          quarter = 2;
          xTile = (2*MuonGeometry::chamberGridX[region]) - (1+ix);
          yTile = (2*MuonGeometry::chamberGridY[region]) - (1+iy);
        } else if ( 3 == twelfth || 4 == twelfth || 5 == twelfth){ // Q=3
          quarter = 3;
          xTile = (2*MuonGeometry::chamberGridX[region]) - (1+ix);
          yTile = iy - (2*MuonGeometry::chamberGridY[region]);
        }  

        // locate the correct GasGap object
        int nGap;
        if(m_NGap.empty()){
          StatusCode sc = fillNGaps();
          if(!sc){
            return sc;
          }
        }
        nGap = m_NGap[station*m_NRegion + region];
        int iGap;
        for(iGap = 0 ; iGap < nGap ; iGap++){

          double gapX,gapY,gapZ;
          double gapDx,gapDy,gapDz;

          sc = getXYZGasGap(station,region,nCham,iGap,
                            gapX,gapDx,gapY,gapDy,gapZ,gapDz);
          if(!sc.isSuccess()){
            log << MSG::ERROR
                << "Could not get GasGap: " <<iGap 
                << " in chamber: " << nCham 
                << " in station " << station
                << " region " << region
                << endreq;
            return sc;
          }
          if( (fabs(x-gapX) < 0.1 + gapDx) &&
              (fabs(y-gapY) < 0.1 + gapDy) &&
              (fabs(z-gapZ) < 0.1 + gapDz) ) {
            // also have the correct gasGap!

            // copy to pass back to calling routine
            tile = MuonTileID(station,
                              region,
                              iGap,
                              MuonLayout(MuonGeometry::chamberGridX[region],
                                         MuonGeometry::chamberGridY[region]),
                              region,
                              quarter,
                              xTile,
                              yTile);               
            pGasGap = gapExtent(station,region,nCham,iGap).pGasGap;
            return StatusCode::SUCCESS;
          }
        }
      }
    }
  }
  // if we got here no chamber was found
  // make an invalid tile to indicate this to the calling routine
  tile = MuonTileID();
  pGasGap = 0;
  return StatusCode::SUCCESS;
}
    
int MuonTileIDXYZ::numberOfChambers(const int &region){
  if(0 == region){ //R1
    return 12;
  }else if(1 == region){
    return 24;
  }else if(2 == region){
    return 48;
  }else{
    return 192;
  }
}

//=============================================================================
