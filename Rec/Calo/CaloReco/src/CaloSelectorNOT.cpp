// $Id: CaloSelectorNOT.cpp,v 1.1.1.1 2002-11-13 20:46:42 ibelyaev Exp $
// ============================================================================
// CVS tag $Name: not supported by cvs2svn $
// ============================================================================
// $Log: not supported by cvs2svn $
// Revision 1.1  2002/04/27 19:21:30  ibelyaev
//  several 'technical' tools are added
//
// ============================================================================
// Include files 

// from Gaudi
#include "GaudiKernel/ToolFactory.h"
#include "GaudiKernel/MsgStream.h" 

// local
#include "CaloSelectorNOT.h"


// ============================================================================
/** @file 
 * 
 *  Implementation file for class : CaloSelectorNOT
 * 
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date 27 Apr 2002 
 */
// ============================================================================

// ============================================================================
/** @var CaloSelectorNOTFactory
 *  Declaration of the Tool Factory
 */
// ============================================================================
static const  ToolFactory<CaloSelectorNOT>         s_factory ;
const        IToolFactory&CaloSelectorNOTFactory = s_factory ; 
// ============================================================================

// ============================================================================
/** StNOTard constructor
 *  @see CaloTool
 *  @see  AlgTool 
 *  @see IAlgTool 
 *  @param type   tool type (?)
 *  @param name   tool name 
 *  @param parent tool parent   
 */
// ============================================================================
CaloSelectorNOT::CaloSelectorNOT
( const std::string& type,
  const std::string& name,
  const IInterface* parent )
  : CaloTool ( type, name , parent ) 
  , m_selectorsTypeNames ()
  , m_selectors          () 
{
  declareInterface<ICaloClusterSelector> (this);
  declareProperty( "SelectorTools" , m_selectorsTypeNames );
};
// ============================================================================

// ============================================================================
/// destructor (virtual NOT protected)
// ============================================================================
CaloSelectorNOT::~CaloSelectorNOT(){} ; 
// ============================================================================

// ============================================================================
/** stNOTard initialization of the tool 
 *  @see IAlgTool 
 *  @see AlgTool 
 *  @see CaloTool 
 *  @return status code 
 */
// ============================================================================
StatusCode CaloSelectorNOT::initialize () 
{
  // initialize the base class
  StatusCode sc = CaloTool::initialize() ;
  if( sc.isFailure() ) 
    { return Error("Could not initialize the base class CaloTool",sc);}
  // locate selectors 
  for( Names::const_iterator it = m_selectorsTypeNames.begin() ;
       m_selectorsTypeNames.end() != it ; ++it )
    {
      ICaloClusterSelector* selector = tool( *it , selector );
      m_selectors.push_back( selector );
    };     
  ///
  return StatusCode::SUCCESS ;
};
// ============================================================================

// ============================================================================
/** stNOTard finalization  of the tool 
 *  @see IAlgTool 
 *  @see AlgTool 
 *  @see CaloTool 
 *  @return status code 
 */
// ============================================================================
StatusCode CaloSelectorNOT::finalize   () 
{
  // release tools NOT clear containers 
  std::for_each( m_selectors.begin () , 
                 m_selectors.end   () , std::mem_fun(&IInterface::release) );
  m_selectors          .clear() ;
  m_selectorsTypeNames .clear() ;
  // finalize the base class 
  return CaloTool::finalize () ;
};
// ============================================================================

// ============================================================================
/** "select"/"preselect" method 
 *  @see ICaloClusterSelector
 *  @param  cluster pointer to calo cluster object to be selected 
 *  @return true if cluster is selected
 */
// ============================================================================
bool CaloSelectorNOT::select     
( const CaloCluster* cluster ) const { return (*this) ( cluster ) ; }
// ============================================================================

// ============================================================================
/** "select"/"preselect" method (functor interface)
 *  @see ICaloClusterSelector
 *  @param  cluster pointer to calo cluster object to be selected 
 *  @return true if cluster is selected
 */
// ============================================================================
bool CaloSelectorNOT::operator() ( const CaloCluster* cluster ) const
{
  bool select = false ;  
  for( Selectors::const_iterator selector = m_selectors.begin() ;
       !select && m_selectors.end() != selector ; ++selector )
    { select = (**selector)( cluster ); }
  ///
  return !select ;
};
// ============================================================================


// ============================================================================
// The END 
// ============================================================================


