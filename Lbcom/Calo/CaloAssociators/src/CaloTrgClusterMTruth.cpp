// $Id: CaloTrgClusterMTruth.cpp,v 1.1 2005-05-08 09:19:51 ibelyaev Exp $
// ============================================================================
// CVS tag $Name: not supported by cvs2svn $, version $Revision: 1.1 $ 
// ============================================================================
// $Log: not supported by cvs2svn $ 
// ============================================================================
// Include files 
// ============================================================================
// Relations 
// ============================================================================
#include "Relations/RelationWeighted1D.h"
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/DataObject.h"
// ============================================================================
// CaloKernel
// ============================================================================
#include "CaloKernel/CaloAlgorithm.h"
// ============================================================================
// Event 
// ============================================================================
#include "Event/CaloDigit.h"
#include "Event/CaloCluster.h"
#include "Event/CaloDataFunctor.h"
#include "Event/CaloMCTools.h"
#include "Event/MCParticle.h"
#include "Event/TrgCaloCluster.h"
// ============================================================================

/** @class CaloTrgClusterMCTruth CaloTrgClusterMCTruth.cpp
 *  
 *  Simple class to create TrgCaloCluster->MCParticle 
 *  relation table basing on information from 
 *  CaloDigit->MCParticle relations table 
 * 
 *  The important  properties of algorithm:
 *  
 *    - "Input"
 *      The defautl value is "Rec/Relations/CaloDigits2MCParticles"
 *      Name of relation table for CaloDigit->MCParticle relations 
 *
 *    - "Inputs"
 *      The default value is   TrgCaloClusterLocation::Ecal
 *      List of addresses for TrgCaloClusters containers 
 *
 *    - "Digits"
 *      The default value is CaloDigitLocation::Ecal 
 *      Name of container of CaloDigits 
 *
 *    - "Output"
 *      The default valeu is "Rec/Relations/TrgCaloClusters2MCParticles" 
 *      Name of (output) relation table TrgCaloCluster->MCParticle
 *  
 *  @author Vanya BELYAEV Ivan.Belyaev@lapp.in2p3.fr 
 *  @date   2005-05-03
 */
class CaloTrgClusterMCTruth: public CaloAlgorithm
{
  friend class AlgFactory<CaloTrgClusterMCTruth> ;
public:
  /// algorithm execution 
  virtual StatusCode execute    () ;
protected:
  /** standard constructor 
   *  @param name algorithm instance name 
   *  @param pSvc pointer to service locator
   */
  CaloTrgClusterMCTruth
  ( const std::string& name , 
    ISvcLocator*       pSvc ) 
    : CaloAlgorithm ( name , pSvc ) 
    , m_digits ( CaloDigitLocation::Ecal )
  {
    declareProperty ( "Digits"               , m_digits           ) ;
    // set the appropriate default values for input data 
    setInputData    ( "Rec/Relations/CaloDigits2MCParticles"      ) ;
    // set the appropriate default values for input data (clusters)
    addToInputs     ( TrgCaloClusterLocation :: Ecal              ) ;
    // set the appropriate default value  for output data
    setOutputData   ( "Rec/Relations/TrgCaloClusters2MCParticles" ) ;
  };
  /// virtual destructor (protected)
  virtual ~CaloTrgClusterMCTruth() {}
private:
  // default constructor  is disabled 
  CaloTrgClusterMCTruth ();
  // copy    constructor  is disabled  
  CaloTrgClusterMCTruth ( const CaloTrgClusterMCTruth& ) ;
  // assignement operator is disabled  
  CaloTrgClusterMCTruth& operator=( const CaloTrgClusterMCTruth& ) ;
private:
  std::string   m_digits ;
};
// ============================================================================

// ============================================================================
/// anonymous namespace to prevent an export of concrete factory
// ============================================================================
namespace 
{
  // ==========================================================================
  /** @var s_Factory
   *  (local) concrete algorithm factory for instantiaton of 
   *   objects of type CaloTrgClusterMCTruth 
   *  @see CaloTrgClusterMCTruth
   *  @author Vanya BELYAEV Ivan.Belyaev@lapp.in2p3.fr
   *  @date 2005-05-03
   */
  // ==========================================================================
  const  AlgFactory<CaloTrgClusterMCTruth>         s_Factory ;
  // ==========================================================================
};
// ============================================================================
/** @var CaloTrgClusterMCTruthFactory
 *  (exported) abstract algorithm factory for instantiaton of 
 *  objects of type CaloTrgClusterMCTruth 
 *  @see CaloTrgClusterMCTruth
 *  @author Vanya BELYAEV Ivan.Belyaev@lapp.in2p3.fr
 *  @date 2005-05-03
 */
const   IAlgFactory&CaloTrgClusterMCTruthFactory = s_Factory ; 
// ============================================================================

// ============================================================================
/// algorithm execution 
// ============================================================================
StatusCode CaloTrgClusterMCTruth::execute    () 
{
  using namespace CaloDataFunctor ;
  using namespace CaloMCTools     ;
  
  /// the actual type of Digit  
  typedef const CaloDigit                                     Digit    ;
  /// the actual type of container of CaloDigit objects 
  typedef const CaloDigits                                    Digits   ;
  /// the actual type of Cluster 
  typedef const TrgCaloCluster                                Cluster  ;
  /// the actual type of Cluster container 
  typedef const TrgCaloClusters                               Clusters ;
  /// the actual type of relation table 
  typedef RelationWeighted1D<TrgCaloCluster,MCParticle,float> Table    ;
  /// the relation table CaloDigit->MCParticle
  typedef const IRelationWeighted<CaloDigit,MCParticle,float> DigTable ;
  /// the actual type of cluster entries 
  typedef const TrgCaloCluster::TrgClusterCells               Entries  ;
  
  // create and register the relation table 
  Table* table = new Table( 1000 ) ;
  StatusCode sc = put( table , outputData() ) ;
  if ( sc.isFailure() ) { return sc ; }
  
  if ( inputs().empty() ) 
  { return Error ( "No inputs are specified!" ) ; }
  
  // get CaloDigit->MCParticle relation from TES 
  DigTable* digTable = get<DigTable> ( inputData() ) ;
  if ( 0 == digTable ) { return StatusCode::FAILURE ; }
  
  // get container of CaloDigits 
  Digits*   digits   = get<Digits>   ( m_digits    ) ;
  if ( 0 == digits   ) { return StatusCode::FAILURE ; }
  
  // loop over all containers of clusters 
  for ( Inputs::const_iterator input = inputs().begin() ; 
        inputs().end() != input ; ++input ) 
  {    
    // get the container of clusters 
    Clusters* clusters = get<Clusters> ( *input ) ;
    if ( 0 == clusters ) { return StatusCode::FAILURE ; }
    
    // loop over all clusters in the container  
    for ( Clusters::const_iterator icluster = clusters->begin() ; 
          clusters->end() != icluster ; ++icluster ) 
    {
      const Cluster* cluster = *icluster ;
      if ( 0 == cluster ) { continue ; }
      
      // auxillary container to collect all links from individual digits 
      CaloMCMap mcMap ;
      
      // loop over all digits in the cluster and collect 
      // the links from individual  digits
      const Entries& entries = cluster->cells() ;
      for ( Entries::const_iterator entry = entries.begin() ; 
            entries.end() != entry ; ++entry ) 
      {
        // get unique ID for the cluster entry 
        const CaloCellID& cellID = entry -> second ;
        // use container of digits to "convert" cellID to CaloDigit
        const Digit* digit = (*digits) ( cellID ) ;
        if ( 0 == digit ) 
        { Warning ( "No appropriate cell is found!" ) ; continue ; }
        
        // get all MC relations from this digit
        DigTable::Range range = digTable->relations ( digit ) ;
        // loop over all MC entries and collect the enegy 
        for ( DigTable::iterator item = range.begin() ; 
              range.end() != item ; ++item ) 
        {
          const MCParticle* particle = item -> to     () ;
          const double      energy   = item -> weight () ;
          // accumulate the energy from the same particle 
          mcMap[particle] += energy ;
        }
      }; // end of loop over all entries in Cluster
      
      if ( mcMap.empty() ) 
      { Warning ( "No MC information for the cluster is found" ) ; }
      
      // loop over auxillary container of merged depositions
      // and fill relation table entries for given cluster 
      for ( CaloMCMap::iterator imap = mcMap.begin() ; 
            mcMap.end() != imap ; ++imap ) 
      {
        // MC particle  
        const MCParticle* particle = imap -> first  ;
        if ( 0 == particle  ) { continue ; }
        // its cumulative energy deposition to the cluster 
        const double      energy   = imap -> second ;
        // fill the relation table:         ATTENTION "i_push" is used!
        table->i_push ( cluster , particle , energy ) ; // NB: "i_push"
      } ;
      
    } ; // end of loop over clusters
    
  } ; // end of loop over containers of clusters 
  
  // mandatory after "i_push" ;
  table->i_sort()  ;                                     // NB: "i_sort"
  
  if ( table->relations().empty() ) 
  { Warning ( " The relations table '"+outputData() + "' is empty!") ; }
  
  if ( msgLevel ( MSG::DEBUG )  ) 
  {
    debug() << " Number of established relations are #"
            << table->relations().size() << endreq ;
  }
  
  return StatusCode::SUCCESS ;  
};

// ============================================================================
// The END 
// ============================================================================



