// $Id: MuonStationLayout.h,v 1.4 2002-02-28 15:39:34 atsareg Exp $

#ifndef MUONKERNEL_MUONSTATIONLAYOUT_H
#define MUONKERNEL_MUONSTATIONLAYOUT_H 1   

// Include files
#include <vector>
#include "GaudiKernel/MsgStream.h"  

/** @class MuonStationLayout MuonStationLayout.h MuonKernel/MuonStationLayout.h 
   
   Defines a Muon station (region) single plane logical layout
   together with various manipulation functions
   
   @author A.Tsaregorodtsev
   @date 6 April 2001 
*/

#include "MuonKernel/MuonLayout.h"
#include "MuonKernel/MuonTileID.h"

// Forward declarations

class MuonStationLayout : public IMuonLayout {

public:
  /// Default constructor
  MuonStationLayout();
  /// Constructor taking MuonLayout's for each region
  MuonStationLayout(const MuonLayout& lq1,
                    const MuonLayout& lq2,
		    const MuonLayout& lq3,
                    const MuonLayout& lq4);
  /// Constructor taking one MuonLayout for all regions
  MuonStationLayout(const MuonLayout& lq);		    
  
  /// Destructor
  virtual ~MuonStationLayout();
    
   
    
  /// Accessor to region layout
  MuonLayout regionLayout(int reg) const { return m_layouts[reg] ; }
  
  /// Accessor to MuonLayout grid corresponding to the given MuonTileID
  /// reimplemented from IMuonLayout
  virtual std::pair<unsigned int, unsigned int> grid(const  MuonTileID& id) const { 
    int reg = id.region();
    return std::make_pair(xGrid(reg),yGrid(reg)); 
  }
  /// Accessor to X granularity
  unsigned int xGrid(int reg) const { return regionLayout(reg).xGrid(); }
  /// Accessor to Y granularity
  unsigned int yGrid(int reg) const { return regionLayout(reg).yGrid(); }  
  
  /** find a vector of MuonTileID's defined in terms of this MuonLayout
      which are touched by an area around a given MuonTileID defined 
      in its own MuonLayout 
      
      @param pad   :  tile defining the area covered
  */          

  virtual std::vector<MuonTileID> tiles(const MuonTileID& pad) const;  
  
  /** find a vector of MuonTileID's defined in terms of this MuonStationLayout
      which are touched by an area around a given MuonTileID defined 
      in its own MuonStationLayout 
      
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
      @param   depth  depth of the band in which to look for neighbours
  */	
  virtual std::vector<MuonTileID> neighbours(const MuonTileID& pad,
                                             int dirX,
					     int dirY,
					     int depth) const;
  
  /// check if the given MuonTileID is valid for this layout
  virtual bool isValidID(const MuonTileID& mt) const ;
  

private:  
  MuonLayout m_layouts[4];
    
};   

#endif    // MUONKERNEL_MUONSTATIONLAYOUT_H
