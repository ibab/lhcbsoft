// $Id: GiGaGeomCnvSvc.h,v 1.5 2002-01-22 18:24:43 ibelyaev Exp $ 
// ============================================================================
// CVS tag $Name: not supported by cvs2svn $ 
// ============================================================================
// $Log: not supported by cvs2svn $
// Revision 1.4  2001/08/12 17:24:52  ibelyaev
// improvements with Doxygen comments
//
// Revision 1.3  2001/07/24 11:13:55  ibelyaev
// package restructurization(III) and update for newer GiGa
//
// Revision 1.2  2001/07/15 20:45:10  ibelyaev
// the package restructurisation
// 
// ============================================================================
#ifndef  GIGACNV_GIGAGEOMCNVSVC_H 
#define  GIGACNV_GIGAGEOMCNVSVC_H 1 
// ============================================================================
/// STL 
#include <string> 
#include <vector> 
/// GiGa 
#include "GiGaCnv/GiGaCnvSvcBase.h" 
#include "GiGaCnv/IGiGaGeomCnvSvc.h" 
///
class G4VPhysicalVolume; 
class G4VSolid;
class G4LogicalVolume; 
///
class IDataSelector; 
class SolidBoolean;
///
class IGiGaSensDet;
class IGiGaSensDetFactory;
///
class IGiGaMagField;
class IGiGaMagFieldFactory;
///
template <class SERVICE> 
class SvcFactory;

/** @class GiGaGeomCnvSvc GiGaGeomCnvSvc.h
 *  
 *  Convertersion service for convertiong Gaugi detector 
 *  and geometry description into Geant4 geometry and 
 *  detector description 
 *  
 *  @author  Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date    07/08/2000
 */

class GiGaGeomCnvSvc:  
  public virtual  IGiGaGeomCnvSvc , 
  public           GiGaCnvSvcBase    
{ 
  ///
  friend class SvcFactory<GiGaGeomCnvSvc>;
  ///  
public:
  ///
  typedef  std::vector<IGiGaSensDet*>                SDobjects; 
  typedef  std::vector<IGiGaMagField*>               MFobjects; 
  ///
protected:
  
  /** standard constructor 
   *  @param name  name of the service 
   *  @param loc   pointer to service locator 
   */
  GiGaGeomCnvSvc( const std::string& name , 
                  ISvcLocator* loc );
  /// virtual destructor
  virtual ~GiGaGeomCnvSvc(){};
  
public: 
  
  /** standard initialization method 
   *  @return status code 
   */
  virtual StatusCode initialize    ()                              ;
  
  /** standard finalization method 
   *  @return status code 
   */
  virtual StatusCode finalize      ()                              ; 
  
  /** Convert the transient object to the requested representation.
   *  e.g. conversion to persistent objects.
   *  @param     object  Pointer to location of the object 
   *  @param     address Reference to location of pointer with the 
   *                     object address.
   *  @return    status code indicating success or failure
   */
  virtual StatusCode createRep     
  ( DataObject*      object ,
    IOpaqueAddress*& address ) ;
  
  /** standard method foe query the interface
   *  @return status code 
   */
  virtual StatusCode queryInterface 
  ( const InterfaceID& , 
    void**             ) ;
  
  /** Retrieve the pointer to top-level "world" volume,
   *  needed for Geant4 - root for the whole Geant4 geometry tree 
   *  @see class IGiGaGeoSrc 
   *  @return pointer to constructed(converted) geometry tree 
   */  
  virtual G4VPhysicalVolume*  world    () ;

 /** Retrieve the pointer for G4 materials from G4MaterialTable, 
   *  (could trigger the conversion of the (DetDesc) Material)
   *  @param  name    name/address/location of Material object 
   *  @return pointer to converted G4Material object 
   */
  virtual G4Material*  material 
  ( const std::string& name      )  ;
  
  /** Retrive the pointer to converter volumes/assemblies 
   *  (could trigger the conversion of the (DetDesc) LVolume/LAssembly)    
   *  @param  name    name/address/location of Volume/Assembly object 
   *  @return pointer to converted GiGaVolume  object 
   */
  virtual GiGaVolume   volume          
  ( const std::string& name      )  ;
  
  /** convert (DetDesc) Solid object into (Geant4) G4VSolid object 
   *  @param  Solid pointer to Solid object 
   *  @return pointer to converter G4VSolid object 
   */
  virtual G4VSolid*    solid  
  ( const ISolid*      Solid     )  ;
  
  /** Instantiate the Sensitive Detector Object 
   *  @param name  Type/Name of the Sensitive Detector Object
   *  @param det   reference to Densitive Detector Object 
   *  @return  status code 
   */
  virtual StatusCode   sensitive   
  ( const std::string& name      , 
    IGiGaSensDet*&     det       )  ;
  
  /** Instantiate the Magnetic Field Object 
   *  @param name  Type/Name of the Magnetic Field Object
   *  @param mag   reference to Magnetic Field Object 
   *  @return  status code 
   */
  virtual StatusCode   magnetic 
  ( const std::string& name      , 
    IGiGaMagField*&    mag       )  ;

  /** Retrieve the pointer to top-level "world" volume,
   *  needed for Geant4 - root for the whole Geant4 geometry tree 
   *  @att obsolete method!
   *  @see class IGiGaGeoSrc 
   *  @return pointer to constructed(converted) geometry tree 
   */  
  virtual G4VPhysicalVolume*  G4WorldPV() ;

  /** Retrieve the pointer for G4 materials from G4MaterialTable, 
   *  (could trigger the conversion of the (DetDesc) Material)
   *  @att obsolete method 
   *  @param  Name    name/address/location of Material object 
   *  @return pointer to converted G4Material object 
   */
  virtual G4Material*         g4Material 
  ( const std::string& Name ) ;
  
  /** Retrive the pointer to G4LogicalVolume  from G4LogicalvolumeStore,
   * (could trigger the conversion of the (DetDesc) LVolume)    
   *  @att obsolete method 
   *  @param  Name    name/address/location of LVolume object 
   *  @return pointer to converted G4LogicalVolume object 
   */
  virtual G4LogicalVolume*    g4LVolume  
  ( const std::string& Name )  ; 
  
  /** convert (DetDesc) Solid object into (Geant4) G4VSolid object 
   *  @att obsolete method 
   *  @param  Solid pointer to Solid object 
   *  @return pointer to converter G4VSolid object 
   */
  virtual G4VSolid*           g4Solid    
  ( const ISolid*  Solid     ) ; 
  
  /** Instantiate the Sensitive Detector Object 
   *  @att obsolete method 
   *  @param Name  Type/Name of the Sensitive Detector Object
   *  @param Det   reference to Densitive Detector Object 
   *  @return  status code 
   */
  virtual StatusCode sensDet   
  ( const std::string& Name , 
    IGiGaSensDet*&     Det  ) ;
  
  /** Instantiate the Magnetic Field Object 
   *  @att obsolete method 
   *  @param Name  Type/Name of the Magnetic Field Object
   *  @param Mag   reference to Magnetic Field Object 
   *  @return  status code 
   */
  virtual StatusCode magField  
  ( const std::string& Name , 
    IGiGaMagField*&    Mag  ) ;

protected:
  ///
  G4VSolid* g4BoolSolid( const SolidBoolean * );
  ///
private:
  ///
  GiGaGeomCnvSvc()                                  ;
  GiGaGeomCnvSvc           ( const GiGaGeomCnvSvc& );
  GiGaGeomCnvSvc& operator=( const GiGaGeomCnvSvc& );
  ///
private:
  ///
  G4VPhysicalVolume*               m_worldPV       ;
  ///
  std::string                      m_worldNamePV   ; 
  std::string                      m_worldNameLV   ; 
  std::string                      m_worldMaterial ; 
  ///
  float                            m_worldX        ;
  float                            m_worldY        ;
  float                            m_worldZ        ;
  ///
  std::string  m_worldMagField ; ///< global magnetic field 
  ////
  SDobjects    m_SDs  ; ///< created sensitive detectors 
  ///
  MFobjects    m_MFs  ; ///< created magnetic field objects 
  ///
};        


// ============================================================================
// end 
// ============================================================================
#endif  //   GIGACNV_GIGAGEOMCNVSVC_H__ 
// ============================================================================








