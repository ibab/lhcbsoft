// ============================================================================
/// CVS tag $Name: not supported by cvs2svn $ 
// ============================================================================
/// $Log: not supported by cvs2svn $
/// Revision 1.8  2001/07/27 14:28:59  ibelyaev
/// bug fix
///
/// Revision 1.7  2001/07/23 13:11:43  ibelyaev
/// the package restructurisation(II)
/// 
// ============================================================================
#ifndef     GIGA_GiGaTrackActionBase_H
#define     GIGA_GiGaTrackActionBase_H 1 
// ============================================================================

// from STL
#include <vector> 
// base class 
#include "GiGa/IGiGaTrackAction.h" 
#include "GiGa/GiGaBase.h" 
//
class G4Track;
//

/** @class GiGaTrackActionBase GiGaTrackActionBase.h
 *  
 *  Base class for implementation of concrete Tracking Action for GiGa
 *  
 *  @author  Vanya Belyaev
 *  @date    23/01/2001
 */

class GiGaTrackActionBase: virtual public IGiGaTrackAction   ,
                           public  GiGaBase
{
  ///
protected:
  /// constructor 
  GiGaTrackActionBase( const std::string& , ISvcLocator* );
  virtual ~GiGaTrackActionBase();
  ///
public:
  /// ident 
  virtual const std::string&  name () const { return GiGaBase::name() ; }; 
  /// initialize 
  virtual StatusCode   initialize () ; 
  /// finalize 
  virtual StatusCode   finalize   () ; 
  /// Query Interface
  virtual StatusCode   queryInterface ( const InterfaceID& , void** ) ;
  ///
public:
  ///
  virtual void PreUserTrackingAction  ( const G4Track* );
  virtual void PostUserTrackingAction ( const G4Track* );
  ///
protected:
  
  /** accessor to Geant4 Tracking Manager 
   *  @return pointer to Geant4 Tracking Manager 
   */
  inline G4TrackingManager* trackMgr() const 
  { return G4UserTrackingAction::fpTrackingManager; }
  
private:
  ///
  GiGaTrackActionBase (); ///< no default constructor 
  GiGaTrackActionBase           ( const GiGaTrackActionBase& ); ///< no copy
  GiGaTrackActionBase& operator=( const GiGaTrackActionBase& ); ///< no =
  ///
private:
  ///
};
///
 
// ============================================================================
#endif   ///< GIGA_GiGaTrackActionBase_H 
// ============================================================================














