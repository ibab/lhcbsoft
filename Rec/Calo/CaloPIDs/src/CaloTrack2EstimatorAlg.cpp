// $Id: CaloTrack2EstimatorAlg.cpp,v 1.4 2004-03-08 14:03:23 cattanem Exp $
// ============================================================================
// CVS tag $Name: not supported by cvs2svn $
// ============================================================================
// Include files
// from LHcbKernel
#include "Relations/Relation1D.h"
// from Gaudi
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/MsgStream.h"
// CaloInterfaces 
#include "CaloInterfaces/ICaloTrackIdEval.h"
// Event 
#include "Event/TrStoredTrack.h"
// local
#include "CaloTrack2EstimatorAlg.h"

// ============================================================================
/** @file 
 *  
 *  Implementation file for class CaloTrack2EstimatorAlg
 * 
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   2002-11-14
 */
// ============================================================================

// ============================================================================
/** @var CaloTrack2EstimatorAlgFactory
 *  Declaration of the Algorithm Factory
 */
// ============================================================================
static const  AlgFactory<CaloTrack2EstimatorAlg>         s_factory ;
const        IAlgFactory&CaloTrack2EstimatorAlgFactory = s_factory ; 
// ============================================================================

// ============================================================================
/** Standard constructor
 *  @param name algorithm name 
 *  @param pSvc service locator 
 */
// ============================================================================
CaloTrack2EstimatorAlg::CaloTrack2EstimatorAlg
( const std::string& name ,
  ISvcLocator*       pSvc )
  : CaloAlgorithm ( name , pSvc ) 
  , m_evalType    (          )
  , m_evalName    (          )
  , m_eval        (  0       )
  , m_low         ( -1.0e+20 ) // no default low limit 
  , m_high        (  1.0e+20 ) // no default high limit 
  , m_skip        (  0       ) 
  ///
  , m_unique      ( false ) // Use ALL tracks
  ///
  , m_error       ( false ) // Do not use Error tracks
  ///
  , m_forward     ( true   ) // Use forward tracks 
  , m_matched     ( true   ) // Use matched tracks 
  , m_seed        ( true   ) // Use seed  tracks 
  , m_velo        ( false  ) // DO NOT use forward tracks 
  , m_veloTT      ( false  ) // DO NOT use forward tracks 
  , m_veloBack    ( false  ) // DO NOT use veloback tracks 
  , m_downstream  ( true   ) // Use dowstream tracks (new naming convention!)
{
  declareProperty ( "EvaluatorType" , m_evalType    ) ;
  declareProperty ( "EvaluatorName" , m_evalName    ) ;
  // track flags 
  declareProperty ( "UseUnique"     , m_unique      ) ;
  declareProperty ( "UseError"      , m_error       ) ;
  declareProperty ( "UseForward"    , m_forward     ) ;
  declareProperty ( "UseMatched"    , m_matched     ) ;
  declareProperty ( "UseVelo"       , m_velo        ) ;
  declareProperty ( "UseVeloTT"     , m_veloTT      ) ;
  declareProperty ( "UseVeloBack"   , m_veloBack    ) ;
  declareProperty ( "UseSeed"       , m_seed        ) ;
  declareProperty ( "UseDownstream" , m_downstream  ) ;
  //
  declareProperty ( "LowLimit"      , m_low         ) ;
  declareProperty ( "HighLimit"     , m_high        ) ;
  // define the default appropriate input data
  setInputData    ( TrStoredTrackLocation:: Default ) ;
};
// ============================================================================

// ============================================================================
/// Destructor
// ============================================================================
CaloTrack2EstimatorAlg::~CaloTrack2EstimatorAlg() {}; 
// ============================================================================

// ============================================================================
/** standard algorithm initialization 
 *  @see CaloAlgorithm
 *  @see     Algorithm
 *  @see    IAlgorithm
 *  @return status code 
 */
// ============================================================================
StatusCode CaloTrack2EstimatorAlg::initialize() 
{
  MsgStream msg(msgSvc(), name());
  msg << MSG::DEBUG << "==> Initialise" << endreq;
  
  StatusCode sc = CaloAlgorithm::initialize() ;
  if( sc.isFailure() ) 
    { return Error("Base class could not be initialized",sc);}
  
  m_eval = tool<ICaloTrackIdEval>( m_evalType , m_evalName );
  if( 0 == m_eval ) { return Error("Evaluator is not located");}
  
  return StatusCode::SUCCESS;
};
// ============================================================================

// ============================================================================
/** standard algorithm finalization 
 *  @see CaloAlgorithm
 *  @see     Algorithm
 *  @see    IAlgorithm
 *  @return status code 
 */
// ============================================================================
StatusCode CaloTrack2EstimatorAlg::finalize() 
{  
  if( 0 != m_skip ) 
  { always () << " Number of 'skips' is " << m_skip << endreq ; } 
  
  return CaloAlgorithm::finalize() ;
};
// ============================================================================

// ============================================================================
/** standard algorithm execution 
 *  @see CaloAlgorithm
 *  @see     Algorithm
 *  @see    IAlgorithm
 *  @return status code 
 */
// ============================================================================
StatusCode CaloTrack2EstimatorAlg::execute() 
{

  // avoid long names 
  typedef const TrStoredTrack              Track   ;
  typedef const TrStoredTracks             Tracks  ;
  typedef Relation1D<TrStoredTrack,float>  Table   ;
  
  // get the tracks from the store 
  const Tracks* tracks = get<Tracks>( inputData() );
  if( 0 == tracks      ) { return StatusCode::FAILURE ; }           // RETURN
  
  // create and register new relation object 
  Table*     table = new Table( tracks->size() );
  StatusCode sc    = put( table , outputData() );
  if( sc.isFailure()   ) { return StatusCode::FAILURE ; }            // RETURN
  
  // loop over all tracks 
  for( Tracks::const_iterator itrack = tracks->begin() ; 
       tracks->end() != itrack ; ++itrack ) 
  {
    const Track* track = *itrack ;
    
    // skip NULLs 
    if( 0 == track                              ) { continue ; }
    
    // use only unique  tracks ? 
    if(  m_unique   && 1 != track->unique    () ) { continue ; }
    
    // use 'error'   tracks ?
    if( !m_error    && 0 != track->errorFlag () ) { continue ; }
    
    // use 'forward'   tracks ?
    if( !m_forward  && 1 == track->forward   () ) { continue ; }
    
    // use 'match'     tracks ?
    if( !m_matched  && 1 == track->match     () ) { continue ; }
    
    // use 'seed'      tracks ?
    if( !m_seed     && 1 == track->seed      () ) { continue ; }
    
    // use 'velo'      tracks ?
    if( !m_velo     && 1 == track->velo      () ) { continue ; }      
    
    // use 'veloTT'    tracks ?
    if( !m_veloTT   && 1 == track->veloTT    () ) { continue ; }      
    
    // use 'veloBack'    tracks ?
    if( !m_veloBack && 1 == track->veloBack  () ) { continue ; }      
    
    // use 'downstream'  tracks ? (New naming convention!)
    if( !m_downstream && 1 == track->isDownstream  () ) { continue ; }
    
    // perform the actual evaluation 
    const double value = (*m_eval)( track );
    
    // skip 
    if( value < m_low || value > m_high ) { ++m_skip ; continue ; }
    
    // fill the relation table 
    table->relate( track , value );
    
  };
  
  debug() << " The total number of booked relations " 
          << table->relations().size() << endreq;
  
  return StatusCode::SUCCESS;
};
// ============================================================================

// ============================================================================
// The END 
// ============================================================================


