// $Id: RichRecTools_load.cpp,v 1.8 2003-09-04 07:12:52 jonrob Exp $

#include "GaudiKernel/DeclareFactoryEntries.h"

DECLARE_FACTORY_ENTRIES( RichRecTools ) {

  // Rich reconstruction working object creators
  DECLARE_TOOL( RichTrackCreatorFromTrStoredTracks );
  DECLARE_TOOL( RichSegmentCreator );
  DECLARE_TOOL( RichPixelCreatorFromRichDigits );
  DECLARE_TOOL( RichPixelCreatorFromSICB ); // temporary
  DECLARE_TOOL( RichPhotonCreator );
  DECLARE_TOOL( RichPhotonPredictor );
  DECLARE_TOOL( RichStatusCreator );

  // geometrical efficiency calculators
  DECLARE_TOOL( RichGeomEffDetailed );
  DECLARE_TOOL( RichGeomEffFixedValue );

  // signal calculators
  DECLARE_TOOL( RichExpectedTrackSignal );
  DECLARE_TOOL( RichPhotonSignalHPD );
  DECLARE_TOOL( RichPhotonSignalMaPMT );
  DECLARE_TOOL( RichTabulatedHPDSignalDetectionEff );
  DECLARE_TOOL( RichSignalDetectionEffSICB ); // temporary

  // MC truth tools
  DECLARE_TOOL( RichRecMCTruthTool );

  // refractive index tools
  DECLARE_TOOL( RichTabulatedRefractiveIndex );
  DECLARE_TOOL( RichRefractiveIndexSICB );   // temporary

  // Physical properties
  DECLARE_TOOL( RichSellmeirFunc );
  DECLARE_TOOL( RichSellmeirFuncSICB );   // temporary
  DECLARE_TOOL( RichParticleProperties );
  DECLARE_TOOL( RichFunctionalRayleighScatter );
  DECLARE_TOOL( RichTabulatedRayleighScatter );

  // Cherenkov angle tools
  DECLARE_TOOL( RichCherenkovAngle );
  DECLARE_TOOL( RichBinnedCherenkovResolution );
  DECLARE_TOOL( RichInterpCherenkovResolution );

  // The tool registry
  DECLARE_TOOL( RichToolRegistry );

};
