//====================================================================
//  DetDesc_load.cpp
//--------------------------------------------------------------------
//
//  Package    : DetDesc
//
//  Description: Implementation of <Package>_load routine. This routine 
//               is needed for forcing the linker to load all the components
//               of the library.. 
//
//====================================================================

#include "GaudiKernel/ICnvFactory.h"
#include "GaudiKernel/ISvcFactory.h"
#include "GaudiKernel/IAlgFactory.h"

#include "GaudiKernel/IObjectFactory.h"

#include "GaudiKernel/ObjectFactory.h"




#define DLL_DECL_SERVICE(x)       extern const ISvcFactory&        x##Factory; x##Factory.addRef();
#define DLL_DECL_CONVERTER(x)     extern const ICnvFactory&        x##Factory; x##Factory.addRef();
#define DLL_DECL_ALGORITHM(x)     extern const IAlgFactory&        x##Factory; x##Factory.addRef();


#include "DetDesc/LVolume.h"
_ImplementDataObjectFactory( LVolume           ) ;  

#include "DetDesc/DetectorElement.h"
_ImplementDataObjectFactory( DetectorElement   ) ; 

#include "DetDesc/Isotope.h"
_ImplementDataObjectFactory( Isotope           ) ;  

#include "DetDesc/Element.h"
_ImplementDataObjectFactory( Element           ) ;  

#include "DetDesc/Mixture.h"
_ImplementDataObjectFactory( Mixture           ) ;  

#include "DetDesc/TabulatedProperty.h"
_ImplementDataObjectFactory( TabulatedProperty ) ; 

#include "DetDesc/Surface.h"
_ImplementDataObjectFactory( Surface           ) ; 


void DetDesc_load() {

  // Declare services 
  DLL_DECL_SERVICE( XmlCnvSvc    );
  DLL_DECL_SERVICE( TransportSvc );
  
  // Now declare all available converters

  DLL_DECL_CONVERTER( XmlCatalogCnv           );
  DLL_DECL_CONVERTER( XmlDetectorElementCnv   );
  DLL_DECL_CONVERTER( XmlLVolumeCnv           );
  DLL_DECL_CONVERTER( XmlIsotopeCnv           );
  DLL_DECL_CONVERTER( XmlElementCnv           );
  DLL_DECL_CONVERTER( XmlMixtureCnv           );
  DLL_DECL_CONVERTER( XmlTabulatedPropertyCnv );
  DLL_DECL_CONVERTER( XmlSurfaceCnv           );
  
  // DataObject Factories
  
  DLL_DECL_OBJECTFACTORY( LVolume           );
  DLL_DECL_OBJECTFACTORY( DetectorElement   );

  DLL_DECL_OBJECTFACTORY( Isotope           );
  DLL_DECL_OBJECTFACTORY( Element           );
  DLL_DECL_OBJECTFACTORY( Mixture           );

  DLL_DECL_OBJECTFACTORY( TabulatedProperty );
  DLL_DECL_OBJECTFACTORY( Surface           );
  
}

///
///
///

extern "C" void DetDesc_loadRef() { DetDesc_load(); }







