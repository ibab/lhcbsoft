
//-----------------------------------------------------------------------------
/** @file RichCommonBase.h
 *
 *  Header file for RICH base class : RichCommonBase
 *
 *  CVS Log :-
 *  $Id: RichCommonBase.h,v 1.1 2005-10-13 15:03:41 jonrob Exp $
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2005-08-27
 */
//-----------------------------------------------------------------------------

#ifndef RICHKERNEL_RICHCOMMONBASE_H
#define RICHKERNEL_RICHCOMMONBASE_H 1

// Interfaces
#include "RichKernel/IRichToolRegistry.h"

// Gaudi
#include "GaudiKernel/IRegistry.h"

// Forward declarations
class IJobOptionsSvc;

//-----------------------------------------------------------------------------
/** @class RichCommonBase RichCommonBase.h RichKernel/RichCommonBase.h
 *
 *  Base class providing common functionality for all RICH tools and algorithms
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2005-08-27
 */
//-----------------------------------------------------------------------------

template <class PBASE>
class RichCommonBase : public PBASE
{

public:

  /// Standard algorithm-like constructor
  RichCommonBase( const std::string& name,
                  ISvcLocator* pSvcLocator );

  /// Standard tool-like constructor
  RichCommonBase( const std::string& type,
                  const std::string& name,
                  const IInterface* parent );

  /// Destructor
  virtual ~RichCommonBase( );

  /** Initialization of the algorithm after creation
   *
   * @return The status of the initialization
   * @retval StatusCode::SUCCESS Initialization was successful
   * @retval StatusCode::FAILURE Initialization failed
   */
  virtual StatusCode initialize();

  /** Finalization of the algorithm before deletion
   *
   * @return The status of the finalization
   * @retval StatusCode::SUCCESS Finalization was successful
   * @retval StatusCode::FAILURE Finalization failed
   */
  virtual StatusCode finalize();

public:

  /** Returns the full location of the given object in the Data Store
   *
   *  @param pObj Data object
   *
   *  @return Location of given data object
   */
  inline std::string objectLocation( const DataObject * pObj ) const
  {
    return ( !pObj ? "Null DataObject !" :
             (pObj->registry() ? pObj->registry()->identifier() : "UnRegistered") );
  }

  /** @brief Returns a pointer to the tool associated to a given nickname.
   *
   *  Uses the RichToolRegistry tool to convert tool nicknames
   *  in the appropriate class name.
   *
   *  @param tName   The nickname of the requested tool
   *  @param pTool   Returned pointer to the requested tool
   *  @param parent  Pointer to parent (used to access private tools)
   *
   *  @return Pointer to the tool associated to the given nickname
   */
  template <typename TOOL>
  inline const TOOL* acquireTool( const std::string & tName,
                                  const TOOL*& pTool,
                                  const IInterface * parent = 0 ) const
  {
    // Construct name
    const std::string fullname = ( parent ? tName : toolRegistry()->toolName(tName) );
    // If private tool - Check Context option
    if ( !parent )
    {
      if ( !setContext( toolRegistry()->toolName(tName) ) )
      {
        Error( "Problem setting Context for '"+fullname+"'" );
      }
    }
    // get tool
    pTool = this -> template tool<TOOL>( toolRegistry()->toolType(tName),
                                         fullname,
                                         parent );
    if ( msgLevel(MSG::DEBUG) )
    {
      debug() << " Acquired tool '" << pTool->name()
              << "' of type '" << toolRegistry()->toolType(tName) << "'" << endreq;
    }
    return pTool;
  }

  /** @brief Forced release of a particular tool
   *
   *  Tools are automatically released during finalisation, so this method
   *  only need be used to release a tool early, before finalisation.
   *
   *  @param pTool  Pointer to the tool to be released
   */
  template <typename TOOL>
  inline void releaseTool( TOOL *& pTool ) const
  {
    if ( pTool )
    {
      if ( msgLevel(MSG::DEBUG) )
      {
        debug() << " Forced release for tool '" << pTool->name() << "'" << endreq;
      }
      release( pTool );
      pTool = NULL;
    }
    else
    {
      Warning ( "Attempt to release a NULL Tool pointer" );
    }
  }

protected: // methods

  /** Returns pointer to RICH tool registry tool
   *  Used internally by base class to convert tool nicknames
   *  in the appropriate class name
   *
   *  @return Pointer to the IRichToolRegistry interface
   */
  inline const IRichToolRegistry * toolRegistry() const
  {
    if (!m_toolReg)
    {
      m_toolReg = this -> template tool < IRichToolRegistry > ( "RichToolRegistry", m_regName );
    }
    return m_toolReg;
  }

private: // private methods

  /** @brief Set the Context option for given public tool
   *
   *  Private solution to the problem that "Context" is not set for public tools
   *  This solution uses the context as defined by the Tool registry to set the
   *  Context for all public tools.
   *
   *  @param name Tool name
   *
   *  @return Status Code indicating if setting was successful or not
   */
  StatusCode setContext( const std::string & name ) const;

  /// Pointer to Job Options Service
  IJobOptionsSvc* joSvc() const;

protected: // data

  /// Pointer to tool registry
  mutable const IRichToolRegistry * m_toolReg;

  /// Pointer to job options service
  mutable IJobOptionsSvc * m_jos;

  /// Runtime name for RichToolRegistry
  std::string m_regName;

};

#endif // RICHKERNEL_RICHCOMMONBASE_H
