#ifndef OTDAQ_OTCHANNELMAP_H_
#define OTDAQ_OTCHANNELMAP_H_

#include <vector>
#include "Kernel/OTChannelID.h"

class OTChannelMapTool ;

namespace OTDAQ
{
  enum { OffsetStations=1, OffsetLayers=0, OffsetQuadrants=0, OffsetModules=1, OffsetStraws=1 } ;

  enum { NumStations=3, NumLayers=4, NumQuadrants=4, NumModules=9, NumStraws=128 } ;

  enum { NumChanPerOtis=32 } ;

  enum EModuleLayout {OddLayerPosY=0,
		      OddLayerNegY,
		      EvenLayerPosY,
		      EvenLayerNegY,
		      OddLayerPosYShort,
		      OddLayerNegYShort,
		      EvenLayerPosYShort,
		      EvenLayerNegYShort,
		      NumDefaultModuleLayouts
  } ;
  
  namespace ChannelMap
  {

    // channel = 32 * otis + channelInOtis

    class Module
    {
    public:
      Module() : m_nchannels(0) {}
      unsigned int straw( unsigned int channel ) const { return m_channelToStraw[channel] ; }
      unsigned int channel( unsigned int straw ) const { return m_strawToChannel[straw-OffsetStraws] ; }
      friend class OTChannelMapTool ;
    private:
      void fillStrawToChannelMap() ;
    private:
      unsigned char m_channelToStraw[NumStraws] ;
      unsigned char m_strawToChannel[NumStraws] ;
      unsigned char m_nchannels ;
    } ;
    
    inline void Module::fillStrawToChannelMap() {
      // Fills the inverse map
      memset(m_strawToChannel,0,NumStraws*sizeof(unsigned char)) ;
      for(unsigned char ichan = 0; ichan < m_nchannels; ++ichan)
        m_strawToChannel[m_channelToStraw[ichan]-OffsetStraws] = ichan ;
    }
    
    class Detector
    {
    public:
  
      Detector() : m_modules(NumDefaultModuleLayouts) {}
      
      const Module& module(const LHCb::OTChannelID& modid) const {
        return module(modid.station(),modid.layer(),modid.quarter(),modid.module()) ;
      }
      
      const Module& module( unsigned int station,  unsigned int layer,
                            unsigned int quadrant, unsigned int module) const {
        return m_modules[layout(station,layer,quadrant,module)] ;
      }
      ;
      unsigned int straw( unsigned int station,  unsigned int layer, 
                          unsigned int quadrant, unsigned int moduleid,
                          unsigned int channelid                        ) const {
        return module(station,layer,quadrant,moduleid).straw(channelid) ;
      }
      unsigned int channel( unsigned int station,  unsigned int layer,
                            unsigned int quadrant, unsigned int moduleid,
                            unsigned int strawid                        ) const {
        return module(station,layer,quadrant,moduleid).channel(strawid) ;
      }
      
      unsigned char layout( unsigned int station,  unsigned int layer,
                            unsigned int quadrant, unsigned int module ) const {
        return m_modulepointers[station-OffsetStations][layer-OffsetLayers][quadrant-OffsetQuadrants][module-OffsetModules] ;
      } ;
      
      unsigned char layout(const LHCb::OTChannelID& modid) const {
        return layout( modid.station(),modid.layer(),modid.quarter(),modid.module()) ;
      } ;
      
      private:
      friend class OTChannelMapTool ;
      
      Module& module( unsigned int station,  unsigned int layer,
                      unsigned int quadrant, unsigned int module) {
        return m_modules[m_modulepointers[station-OffsetStations]
                                         [layer-OffsetLayers]
                                         [quadrant-OffsetQuadrants]
                                         [module-OffsetModules]] ;
      }
      
      void setModuleLayout( const LHCb::OTChannelID& modid, unsigned char layout) {
        m_modulepointers[modid.station()-OffsetStations]
                        [modid.layer()-OffsetLayers]
                        [modid.quarter()-OffsetQuadrants]
                        [modid.module()-OffsetModules] = layout ;
      }
      
      private:  
      unsigned char m_modulepointers[NumStations][NumLayers][NumQuadrants][NumModules];
      std::vector<Module> m_modules ;
    } ;
  }
}

#endif
