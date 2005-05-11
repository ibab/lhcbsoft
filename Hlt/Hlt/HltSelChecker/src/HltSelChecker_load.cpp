
#include "GaudiKernel/DeclareFactoryEntries.h"
#include "GaudiKernel/ObjectFactory.h"

DECLARE_FACTORY_ENTRIES( HltSelChecker ) {

  // Algorithms
  DECLARE_ALGORITHM( TestHltScore );
  DECLARE_ALGORITHM( TrackNTuple );
}

