// $Id: CaloHypoAlg.h,v 1.1.1.1 2002-11-13 20:46:40 ibelyaev Exp $
// ============================================================================
// CVS tag $Name: not supported by cvs2svn $
// ============================================================================
// $Log: not supported by cvs2svn $
// Revision 1.1  2002/09/02 18:19:16  ibelyaev
//  new generic algorithm for applying CaloHypo tools to CaloHypo objects
//
// Revision 1.3  2002/06/15 12:53:13  ibelyaev
//  version update
//
// ============================================================================
#ifndef CaloAlgs_CaloHypoAlg_H 
#define CaloAlgs_CaloHypoAlg_H 1
// Include files
// from STL
#include <string>
#include <vector>
// from CaloKernel
#include "CaloKernel/CaloAlgorithm.h"
// forward decclaration
class ICaloHypoTool ;

/** @class CaloHypoAlg CaloHypoAlg.h
 *  
 *  Generic CaloHypo Algorithm.
 *  It is just applies a set of ICaloHypoTools 
 *  to a container of CaloHypo objects
 *  @see ICaloHypoTool
 *  @see  CaloHypo
 *  @see  CaloAlgorithm
 *  @see      Algorithm
 *  @see     IAlgorithm
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   02/09/2002
 */

class CaloHypoAlg : public CaloAlgorithm 
{
  /// friend factory for instantiation
  friend class AlgFactory<CaloHypoAlg>;

public:
  
  /** standard algorithm initialization 
   *  @see CaloAlgorithm
   *  @see     Algorithm
   *  @see    IAlgorithm
   *  @return status code 
   */
  virtual StatusCode initialize();   
  
  /** standard algorithm execution 
   *  @see CaloAlgorithm
   *  @see     Algorithm
   *  @see    IAlgorithm
   *  @return status code 
   */
  virtual StatusCode execute   ();   
  
  /** standard algorithm finalization 
   *  @see CaloAlgorithm
   *  @see     Algorithm
   *  @see    IAlgorithm
   *  @return status code 
   */
  virtual StatusCode finalize  ();   
  
protected:
  
  /** Standard constructor
   *  @param   name   algorithm name 
   *  @param   svcloc pointer to service locator 
   */
  CaloHypoAlg( const std::string& name   , 
              ISvcLocator*       svcloc );
  
  /// destructor (virtual and protected)
  virtual ~CaloHypoAlg();
  
private:

  /// default  construstor  is  private 
  CaloHypoAlg(); 
  /// copy     construstor  is  private 
  CaloHypoAlg
  ( const CaloHypoAlg& );
  /// assignement operator  is  private 
  CaloHypoAlg& operator=
  ( const CaloHypoAlg& );

private:
  
  typedef std::vector<std::string>    Names ;
  typedef std::vector<ICaloHypoTool*> Tools ;
  
  /// list of tool type/names 
  Names   m_names ;
  /// list of tools 
  Tools   m_tools ;

};

// ============================================================================
// The End 
// ============================================================================
#endif // CaloHypoAlg_H
// ============================================================================
