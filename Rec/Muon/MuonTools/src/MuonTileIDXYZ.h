// $Id: MuonTileIDXYZ.h,v 1.2 2002-03-20 18:07:59 dhcroft Exp $
#ifndef MUONTILEIDXYZ_H 
#define MUONTILEIDXYZ_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/AlgTool.h"

// local
#include "MuonTools/IMuonTileXYZTool.h"

/** @class MuonTileIDXYZ MuonTileIDXYZ.h
 *  
 *  Convert an MuonTileID into an xyz position in the detector (with size)
 *  No abstract interface as I do not want to make more than one of these...
 *
 *  @author David Hutchcroft
 *  @date   07/03/2002
 */
class MuonTileIDXYZ : public AlgTool, 
                      virtual public IMuonTileXYZTool {
public:
  /// Standard constructor
  MuonTileIDXYZ( const std::string& type, 
                 const std::string& name,
                 const IInterface* parent);
  
  virtual ~MuonTileIDXYZ( ) ; ///< Destructor

  /** Calculate the x,y,z and dx,dy,dz of a MuonTileID in mm
   * this ignores gaps: these can never be read out independently
   */
  virtual StatusCode calcTilePos(const MuonTileID& tile, 
                                 double& x, double& deltax,
                                 double& y, double& deltay,
                                 double& z, double& deltaz);
  
  /// locate the MuonTileID of the chamber containing x,y,z (in mm)
  virtual StatusCode locateChamberTile(const double& x, 
                                       const double& y, 
                                       const double& z, 
                                       MuonTileID& tile);

  /// locate the MuonTileID of the chamber containing x,y,z (in mm)
  /// and get the associated gas gap pointer
  virtual StatusCode locateChamberTileAndGap(const double& x, 
                                             const double& y, 
                                             const double& z, 
                                             MuonTileID& tile,
                                             DeMuonGasGap* &pGasGap);

  /** Calculate the position of the Muon front end Channel
   *  containing x,y,z (in mm)
   */
  virtual StatusCode locateFEFromXYZ(const double& x, 
                                     const double& y, 
                                     const double& z, 
                                     MuonTileID &chamber,
                                     std::vector<MuonChannel> &muonChannels,
                                     DeMuonGasGap* &pGasGap);
private:

  /// get position of chamber from a tile
  StatusCode getXYZChamberTile(const MuonTileID& tile, 
                               double& x, double& deltax,
                               double& y, double& deltay,
                               double& z, double& deltaz);

  /// get position of a "named" chamber 
  /// NOTE: station and region are indexed from 0 (C style)
  /// chamberNum is the real chamber number (from 0)
  StatusCode getXYZChamber(const int& station,
                           const int& region,
                           const int& chamberNum,
                           double& x, double& deltax,
                           double& y, double& deltay,
                           double& z, double& deltaz);

  /// get position of a "named" gasgap
  /// NOTE: station, region and gapNum are indexed from 0 (C style)
  /// chamberNum is the real chamber number (from 0)
  StatusCode getXYZGasGap(const int& station,
                          const int& region,
                          const int& chamberNum,
                          const int& gapNum,
                          double& x, double& deltax,
                          double& y, double& deltay,
                          double& z, double& deltaz);
  
  /// get position of chamber or gas gap with caching of results and pointers
  /// NOTE: station, region and gapNum are indexed from 0 (C style)
  /// chamberNum is the real chamber number (from 0)
  StatusCode getXYZAndCache(const int& station,
                            const int& region,
                            const int& chamberNum,
                            const int& gapNum,
                            double& x, double& deltax,
                            double& y, double& deltay,
                            double& z, double& deltaz);

  // fill the array with the position and size of the stations
  StatusCode fillStationExtent();

  /// get the chamber number (counts from 1) for the tile
  int getChamberNumber(const MuonTileID& tile);

  /// get postion of logical channel (may be multiple chambers)
  StatusCode getXYZLogical(const MuonTileID& tile, 
                           double& x, double& deltax,
                           double& y, double& deltay,
                           double& z, double& deltaz);
  
  /// get xyz of specific pad
  StatusCode getXYZPad(const MuonTileID& tile, 
                       double& x, double& deltax,
                       double& y, double& deltay,
                       double& z, double& deltaz);

  /// locate the GasGap given the station, region and quater  
  /// and return the gas gap pointer
  StatusCode locateGasGapFromXYZ( const int& station,
                                  const int& region,
                                  const int& quarter,
                                  const double& x,
                                  const double& y,
                                  const double& z,
                                  MuonTileID& tile,
                                  DeMuonGasGap* &pGasGap);

  // store x,y,z,dx,dy,dz of chambers locally (Cached) to avoid multiple
  // TDS lookups (the other choice is to cache the smartpointers)
  class muonExtent_ {
  public:
    double x;
    double y;
    double z;
    double dx;
    double dy;
    double dz;
  } ;

  muonExtent_ stationExtent[5];

  class chamberExtent_ : public muonExtent_ {
  public:
    DeMuonChamber *pChamber;
  };

  // only split by region to save a little space 
  //(about 2000 copies of muonExtent)
  chamberExtent_ chamR1[5][12];
  chamberExtent_ chamR2[5][24];
  chamberExtent_ chamR3[5][48];
  chamberExtent_ chamR4[5][192];

  class gapExtent_ : public muonExtent_ {
  public:
    DeMuonGasGap *pGasGap;
  };

  // R1, R2 all 4 gap MWPCs, R3 and R4 in M4 and M5 are 2 gap RPCs
  gapExtent_ gapR1[5][12][4];
  gapExtent_ gapR2[5][24][4]; 
  gapExtent_ gapR3MWPC[3][48][4];
  gapExtent_ gapR3RPC[2][48][2];
  gapExtent_ gapR4MWPC[3][192][4];
  gapExtent_ gapR4RPC[2][192][2];
 
  // store chamber layout locally
  MuonSystemLayout * m_chamberSystemLayout;

private:
  IDataProviderSvc* m_DDS;

};
#endif // MUONTILEIDXYZ_H
