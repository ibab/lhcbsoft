// $Id: MuonSystemLayout.h,v 1.4 2002-02-28 15:39:35 atsareg Exp $

#ifndef MUONKERNEL_MUONSYSTEMLAYOUT_H
#define MUONKERNEL_MUONSYSTEMLAYOUT_H 1   

// Include files
#include <vector>
#include "GaudiKernel/MsgStream.h"  

/** @class MuonSystemLayout MuonSystemLayout.h MuonKernel/MuonSystemLayout.h 
   
   Defines a Muon station (region) single plane logical layout
   together with various manipulation functions
   
   @author A.Tsaregorodtsev
   @date 6 April 2001 
*/

#include "MuonKernel/MuonStationLayout.h"

// Forward declarations

class MuonTileID;
class IMuonTile;

class MuonSystemLayout : public IMuonLayout {

public:
  /// Default constructor
  MuonSystemLayout();
  /// Constructor taking MuonStationLayout's for each station
  MuonSystemLayout(const MuonStationLayout& lq1,
                   const MuonStationLayout& lq2,
		   const MuonStationLayout& lq3,
                   const MuonStationLayout& lq4,
		   const MuonStationLayout& lq5);
  /// Constructor taking one MuonStationLayout  for all stations
  MuonSystemLayout(const MuonStationLayout& lq);		   
  
  /// Destructor
  virtual ~MuonSystemLayout();
    
  /// Accessor to region/station MuonLayout  
  MuonLayout regionLayout(int st, int reg) const { 
    return  m_station_layouts[st].regionLayout(reg) ;
  }  
  /// Accessor to station MuonStationLayout  
  MuonStationLayout stationLayout(int st) const { 
    return  m_station_layouts[st] ;
  } 

  /// Accessor to MuonLayout grid corresponding to the given MuonTileID
  /// reimplemented from IMuonLayout
  virtual std::pair<unsigned int, unsigned int> 
  grid(const  MuonTileID& id) const { 
    int reg = id.region();
    int st  = id.station();
    return std::make_pair(xGrid(st,reg),yGrid(st,reg)); 
  } 
  /// Accessor to X granularity
  unsigned int xGrid(int st, int reg) const { return stationLayout(st).xGrid(reg); }
  /// Accessor to Y granularity
  unsigned int yGrid(int st, int reg) const { return stationLayout(st).yGrid(reg); }  
  
  /** find a vector of MuonTileID's defined in terms of this MuonLayout
      which are touched by an area around a given MuonTileID defined 
      in its own MuonLayout 
      
      @param pad   :  tile defining the area covered
  */          

  virtual std::vector<MuonTileID> tiles(const MuonTileID& pad) const;
  
  /** find a vector of MuonTileID's defined in terms of this MuonSystemLayout
      which are touched by an area around a given MuonTileID defined 
      in its own MuonSystemLayout 
      
      @param pad   : central for the search
      @param areaX : limits of the search area in X
      @param areaY : limits of the search area in Y
  */          

  virtual std::vector<MuonTileID> tilesInArea(const MuonTileID& pad, 
			        		int areaX,
			        		int areaY) const;    
				    
  /// returns a vector of its MuonTileID's. 
  /// Implementation of the IMuonLayout	interface
  std::vector<MuonTileID> tiles() const;
  
  /// returns a vector of its MuonTileID's in a given quarter. 
  /// Implementation of the IMuonLayout	interface
  std::vector<MuonTileID> tiles(int quarter) const;
  
  /// returns a vector of its MuonTileID's in a given quarter and region. 
  /// Implementation of the IMuonLayout	interface
  std::vector<MuonTileID> tiles(int quarter, int region) const;	
    
  /// find a tile containing the argument tile
  virtual MuonTileID contains(const MuonTileID& pad) const;
  
  /// Get tiles touched by pad defined in terms of pregion region number
  virtual std::vector<MuonTileID> 
  tilesInRegion(const MuonTileID& pad, int pregion) const;
  
  /// find all the MuonTileID's which are neighbours of the argument tile
  virtual std::vector<MuonTileID> neighbours(const MuonTileID& pad) const;
  
  /** find all the MuonTileID's which are neighbours of the argument tile
      in the specified direction. 
      @param   pad   find the neighbours of this tile
      @param   dirX  horizontal direction in which to look for neighbours
      @param   dirY  vertical direction in which to look for neighbours
  */	
  virtual std::vector<MuonTileID> neighbours(const MuonTileID& pad,
                                             int dirX,
					     int dirY) const;

  /** find all the MuonTileID's which are neighbours of the argument tile
      in the specified direction. 
      @param   pad   find the neighbours of this tile
      @param   dirX  horizontal direction in which to look for neighbours
      @param   dirY  vertical direction in which to look for neighbours
      @param   depth depth of the band in which to look for neighbours
  */	
  virtual std::vector<MuonTileID> neighbours(const MuonTileID& pad,
                                             int dirX,
					     int dirY,
					     int depth) const;

  /// check if the given MuonTileID is valid for this layout
  virtual bool isValidID(const MuonTileID& mt) const ;
  

private:  
  MuonStationLayout m_station_layouts[5];
    
};   

#endif    // MUONKERNEL_MUONSYSTEMLAYOUT_H
