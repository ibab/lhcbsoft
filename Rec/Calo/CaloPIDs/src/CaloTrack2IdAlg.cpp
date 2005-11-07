// $Id: CaloTrack2IdAlg.cpp,v 1.7 2005-11-07 12:16:09 odescham Exp $
// ============================================================================
// CVS tag $Name: not supported by cvs2svn $ , version $Revision: 1.7 $
// ============================================================================
// $Log: not supported by cvs2svn $
// Revision 1.6  2005/05/08 09:34:06  ibelyaev
//  eliminate all *associators*
// 
// ============================================================================
// Include files
// ============================================================================
// Relations
// ============================================================================
#include "Relations/Relation1D.h"
// ============================================================================
// AIDA
// ============================================================================
#include "AIDA/IHistogram2D.h"
#include "AIDA/IAxis.h"
// ============================================================================
// from Gaudi
// ============================================================================
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/IHistogramSvc.h"
// ============================================================================
/// Event 
// ============================================================================
#include "Event/Track.h"
// ============================================================================
// CaloUtils 
// ============================================================================
#include  "CaloUtils/CaloHistoTrackAlg.h"
// ============================================================================

/** @class CaloTrack2IdAlg CaloTrack2IdAlg.cpp
 *  
 *  The generic algorithm to convert the relation table 
 *  from between tracks and caloestimators to 
 *  the relation table between tracks and difference 
 *  in loglikelihoods.
 * 
 *  Each concrete instance requires the 
 *   2 histograms (for signal and background)
 * 
 *  For current implementation the histograms will be 
 *  created from options, in future better solution 
 *  need to be found 
 *  
 *  The obvious concrete implementation:
 *   - Prs  ID for e+/e-
 *   - Hcal ID for e+/e- 
 *   - Hcal ID for mu+/mu-
 *
 *  The major properties of teh algorithm
 * 
 *    - "Input"
 *    The location in TES the relation table Track -> CaloEstimator
 *
 *    - "Output"
 *    The location in TES the relation table Track -> DLL 
 *
 *    - "ValueNorm"
 *    The normalization for "estimator"
 *
 *    - "MomnetumNorm"
 *    The normalization for "momentum"
 *
 *    - "HistoForSignal"
 *    Location in THS the historgam for "signal"
 *
 *    - "HistoForBackground"
 *    Location in THS the historgam for "background"
 *
 *    - "HistoForSignalNorm"
 *    Location in THS the historgam for "signal" (normalized)
 *
 *    - "HistoForBackgroundNorm"
 *    Location in THS the historgam for "background" (normalized)
 * 
 *    - "HistoForDLL"
 *    Location in THS the hisgorgam for "DLL"
 *
 *    - the properties for track selection are from CaloHistoTrackAlg base 
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   2002-11-14
 */
class CaloTrack2IdAlg : public CaloHistoTrackAlg 
{
  // friend factory for instantiation
  friend class AlgFactory<CaloTrack2IdAlg>;
public:
  /// standard algorithm initialization 
  virtual StatusCode initialize();
  /// standard algorithm execution 
  virtual StatusCode execute   ();  
protected:
  /** Standard constructor
   *  @param name algorithm name 
   *  @param pSvc service locator 
   */
  CaloTrack2IdAlg
  ( const std::string& name , 
    ISvcLocator*       pSvc );
  // destructor 
  virtual ~CaloTrack2IdAlg(){} ;
protected:
  /// transformation function for momentum 
  inline double pFunc( const double value ) const 
  { return tanh( value / m_pNorm ); };
  /// transformation function for estimator 
  inline double vFunc( const double value ) const 
  { return tanh( value / m_vNorm ); };
protected:
  /** prepare DeltaLL histogram (load or calculate)
   *  @return pointer to DeltaLL histogram 
   */
  AIDA::IHistogram2D* makeDeltaLL () const ;
  /** prepare SignalN histogram (load or calculate)
   *  @return pointer to SignalN histogram 
   */
  AIDA::IHistogram2D* makeSignalN () const ;
  /** prepare BackgrN histogram (load or calculate)
   *  @return pointer to BackgrN histogram 
   */
  AIDA::IHistogram2D* makeBackgrN () const ;
  /** load the histogram 
   *  @param address address in the file/store 
   *  @return pointer to the histogram 
   */
  AIDA::IHistogram2D* loadHisto   ( const std::string& address ) const ;
  /** perform the apropriate normalization of the histogram 
   *  @param histo histogram to be normalized
   *  @param ID1   ID to be used for new histogram 
   *  @param ID2   ID to be used for new histogram 
   *  @return pointer to new (normalized histogram) 
   */
  AIDA::IHistogram2D* normalize   
  ( const AIDA::IHistogram2D* histo , 
    const int                 ID1   , 
    const int                 ID2   ) const ;
  /** compare the specifications of 2 histograms 
   *  @param h1 pointer to the histogram  
   *  @param h2 pointer to the histogram  
   *  @return true if histogram hame the sam especifications 
   */
  bool   compare 
  ( const AIDA::IHistogram2D* h1  , 
    const AIDA::IHistogram2D* h2  ) const ;
  /** get the x-coordinate of the bin 
   *  @param histo histogram 
   *  @param bin bin number 
   *  @return coordinate of the bin center 
   */
  double xCoord  
  ( const AIDA::IHistogram2D* histo , 
    const int                 bin   ) const ;
  /** get the y-coordinate of the bin 
   *  @param histo histogram 
   *  @param bin bin number 
   *  @return coordinate of the bin center 
   */
  double yCoord  
  ( const AIDA::IHistogram2D* histo , 
    const int                 bin   ) const ;
private:
  //
  double              m_vNorm          ;
  double              m_pNorm          ;
  //
  std::string         m_signalHisto    ;
  std::string         m_backgrHisto    ;
  std::string         m_signalNhisto   ;
  std::string         m_backgrNhisto   ;
  std::string         m_deltaLLhisto   ;
  
  AIDA::IHistogram2D*  m_deltaLL       ; // delta Log Likelihood
  
};

// ============================================================================
/** @var CaloTrack2IdAlgFactory
 *  Declaration of the Algorithm Factory
 */
// ============================================================================
static const  AlgFactory<CaloTrack2IdAlg>         s_factory ;
const        IAlgFactory&CaloTrack2IdAlgFactory = s_factory ; 
// ============================================================================

// ============================================================================
/** Standard constructor
 *  @param name algorithm name 
 *  @param pSvc service locator 
 */
// ============================================================================
CaloTrack2IdAlg::CaloTrack2IdAlg
( const std::string& name ,
  ISvcLocator*       pSvc )
  : CaloHistoTrackAlg ( name , pSvc ) 
  ///
  , m_vNorm       ( 0 ) 
  , m_pNorm       ( 0 )
  ///
  , m_signalHisto  ()
  , m_backgrHisto  ()
  , m_signalNhisto ()
  , m_backgrNhisto ()
  , m_deltaLLhisto ()
  ///
  , m_deltaLL     ( 0 ) 
{
  //
  declareProperty ( "ValueNorm"              , m_vNorm          ) ;
  declareProperty ( "MomentumNorm"           , m_pNorm          ) ;
  //
  declareProperty ( "HistoForSignal"         , m_signalHisto    ) ;
  declareProperty ( "HistoForBackground"     , m_backgrHisto    ) ;
  declareProperty ( "HistoForSignalNorm"     , m_signalNhisto   ) ;
  declareProperty ( "HistoForBackgroundNorm" , m_backgrNhisto   ) ;
  declareProperty ( "HistoForDeltaLL"        , m_deltaLLhisto   ) ;
  //
};
// ============================================================================

// ============================================================================
/** standard algorithm initialization 
 *  @see CaloAlgorithm
 *  @see     Algorithm
 *  @see    IAlgorithm
 *  @return status code 
 */
// ============================================================================
StatusCode CaloTrack2IdAlg::initialize() 
{
  
  StatusCode sc = CaloHistoTrackAlg::initialize() ;
  if ( sc.isFailure() ) { return sc ; } 
  
  if ( 0 >= m_vNorm ) { return Error("Invalid 'ValueNorm'    property" ) ; }
  if ( 0 >= m_pNorm ) { return Error("Invalid 'MomentumNorm' property" ) ; }
  
  if      (   !m_deltaLLhisto.empty() ) { } // final histo isprovided 
  else if ( ( !m_signalNhisto.empty() || !m_signalHisto.empty() ) && 
            ( !m_backgrNhisto.empty() || !m_backgrHisto.empty() )  ) {}
  else { return Error( "Invalid setup for input histograms!"); }
  
  
  if( 0 == histoSvc() ) { return Error("histoSvc() points to NULL ");}
  
  m_deltaLL = makeDeltaLL() ;
  if ( 0 == m_deltaLL )
  { return Error("Unable to retrieve/create/prepare 'DeltaLL' histogram"); }
  
  return StatusCode::SUCCESS;
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
StatusCode CaloTrack2IdAlg::execute() 
{
  
  /// avoid long names 
  typedef const IRelation<Track,float> Table  ;
  typedef Table::Range                         Range  ;
  typedef Relation1D<Track,float>      Output ;
  
  // get the table from TES
  const Table* input = get<Table> ( inputData() ) ;
  if ( 0 == input ) { return StatusCode::FAILURE ; } // RETURN 
  
  // "convert" "input" to "output"
  Range range = input->relations();
  
  /// create and register in the store the output tables 
  Output* output     = new Output ( range.size() ) ;
  StatusCode sc      = put ( output , outputData() ) ;
  if ( sc.isFailure() ) { return StatusCode::FAILURE ; } // RETURN
  
  /// convert the old table into the new table 
  for ( Range::const_iterator rel = range.begin() ; 
        range.end() != rel ; ++rel ) 
  {
    Track* track = rel->from() ; 
    // skip the invalid tracks 
    if (  0   == track   ) { continue ; }
    //
    if ( !use  ( track ) ) { continue ; }
    
    // get the momentum from the state nearest to 0,0,0
    const State state   = track->closestState( 0.0 ) ;
    //OD if ( 0 == state ){ Warning("Track has no state closest to z=0"    ); continue ; } 
    //const TrStateP* stateP = dynamic_cast<const TrStateP*>( state );
    //if ( 0 == stateP ) 
    //{ Warning("Momentum information is not available"); continue ; } 
    
    // get the momentum and transform it 
    const double p       = pFunc ( state.p  () ) ; //OD
    // get the estimator and transform it 
    const double v       = vFunc ( rel    -> to () ) ;
    
    const double deltaLL = 
      m_deltaLL -> binHeight( m_deltaLL -> coordToIndexX ( p ) ,
                              m_deltaLL -> coordToIndexY ( v ) );
    
    // fill relation table 
    output      -> relate( track , deltaLL ) ; 
  }
  
  if ( msgLevel ( MSG::DEBUG ) ) 
  {
    debug () << " The total number of booked relations " 
             << output     -> relations().size()  << endreq ;
  }
  
  return StatusCode::SUCCESS;
};
// ============================================================================

// ============================================================================
/** load the histogram 
 *  @param address address in the file/store 
 *  @return pointer to the histogram 
 */
// ============================================================================
AIDA::IHistogram2D* CaloTrack2IdAlg::loadHisto   
( const std::string& address ) const 
{ 
  // do not waste of time for trivial case  
  if ( address.empty () ) { return 0 ; }
  
  // locate the histogram 
  SmartDataPtr<AIDA::IHistogram2D> histo( histoSvc() , address );
  if( !histo ) 
  {
    Warning( " loadHisto: no histogram with '"+address+"' is found");
    return 0 ;
  };
  
  return histo ;
};
// ============================================================================

// ============================================================================
/** prepare SignalN histogram (load or calculate)
 *  @return pointer to SignalN histogram 
 */
// ============================================================================
AIDA::IHistogram2D* CaloTrack2IdAlg::makeSignalN () const 
{
  AIDA::IHistogram2D* histo = loadHisto( m_signalNhisto ) ;
  if( 0 != histo ) { return histo ; }                            // RETURN
  // locate non-normalized histogram
  histo = loadHisto( m_signalHisto ) ;
  if( 0 == histo ) 
  {
    Error ("Neither 'SignalN' nor 'Signal' histohrams available!" ) ;
    return 0 ;
  }
  // normalize 
  histo  = normalize ( histo , 103 , 203 );
  if( 0 == histo ) 
  { Error ( "Unable to perform proper normalization for 'Signal'" ) ; };
  return histo ;
};
// ============================================================================

// ============================================================================
/** prepare BackgrN histogram (load or calculate)
 *  @return pointer to BackgrN histogram 
 */
// ============================================================================
AIDA::IHistogram2D* CaloTrack2IdAlg::makeBackgrN () const 
{
  AIDA::IHistogram2D* histo = loadHisto( m_backgrNhisto ) ;
  if( 0 != histo ) { return histo ; }                            // RETURN
  // locate non-normalized histogram
  histo = loadHisto( m_backgrHisto ) ;
  if( 0 == histo ) 
  {
    Error("Neither 'BackgrN' nor 'Backgr' histohrams available!");
    return 0 ;
  }
  // normalize 
  histo  = normalize ( histo , 104 , 204 );
  if( 0 == histo ) 
  { Error("Unable to perform proper normalization for 'Backgr'"); };
  return histo ;
};
// ============================================================================

// ============================================================================
/** perform the apropriate normalization of the histogram 
 *  @param histo histogram to be normalized
 *  @param ID1   ID to be used for new histogram 
 *  @param ID2   ID to be used for new histogram 
 *  @return pointer to new (normalized histogram) 
 */
// ============================================================================
AIDA::IHistogram2D* CaloTrack2IdAlg::normalize   
( const AIDA::IHistogram2D* histo , 
  const int  ID1                  , 
  const int  ID2                  ) const 
{
  // trivial case
  if( 0 == histo ) { return 0 ; }
  
  std::string dir    ( dirHbookName ( "Calo/" + name () )  );  
  //
  std::string title1 ( "(Adjusted): " + histo -> title() );
  AIDA::IHistogram2D* norm1 = 
    histoSvc() -> book ( dir , ID1 , title1 ,
                         //
                         histo -> xAxis () . bins      () , 
                         histo -> xAxis () . lowerEdge () , 
                         histo -> xAxis () . upperEdge () , 
                         //
                         histo -> yAxis () . bins      () , 
                         histo -> yAxis () . lowerEdge () , 
                         histo -> yAxis () . upperEdge () );  
  if ( 0 == norm1 ) 
  {
    Error ( "Unable to book '" + title1 + "' histogram" ) ;
    return 0 ; 
  };
  
  if( !compare( histo , norm1 ) ) 
  {
    Error( "Incompatible specifications for histograms '" 
           + histo -> title() + "' and '" + norm1 -> title() + "'");
    return 0 ;
  }
  
  std::string title2 ( "(Normalzed): " + histo -> title() );
  AIDA::IHistogram2D* norm2 = 
    histoSvc() -> book ( dir , ID2 , title2 ,
                         //
                         norm1 -> xAxis () . bins      () , 
                         norm1 -> xAxis () . lowerEdge () , 
                         norm1 -> xAxis () . upperEdge () , 
                         //
                         norm1 -> yAxis () . bins      () , 
                         norm1 -> yAxis () . lowerEdge () , 
                         norm1 -> yAxis () . upperEdge () );  
  
  if ( 0 == norm2 ) 
  {
    Error ( "Unable to book '" + title2 + "' histogram" ) ;
    return 0 ; 
  };
  
  if( !compare( norm1 , norm2 ) ) 
  {
    Error( "Incompatible specifications for histograms '" 
           + norm1 -> title() + "' and '" + norm2 -> title() + "'");
    return 0 ;
  }
  
  const int xBins = norm1 -> xAxis() . bins() ;
  const int  yBins = norm1 -> yAxis() . bins() ;
  
  // 1) copy and adjust the content 
  for    ( int  ix = 0 ; ix < xBins ; ++ix ) 
  { 
    for  ( int  iy = 0 ; iy < yBins ; ++iy ) 
    {
      double value = histo -> binHeight ( ix , iy ) ;
      if( 0 >= value ) 
      { 
        Warning( "Substitute non-positive weight with 1 for '"+ title1 + "'" );
        value = 1 ;
      }
      
      const double x = xCoord ( histo , ix ) ;
      const double y = yCoord ( histo , iy ) ;
      norm1 -> fill( x , y , value ) ;
    }
  }
  
  // 2) perform the actual normalization 
  for    ( int ix = 0 ; ix < xBins ; ++ix ) 
  {    
    const double A =         norm1 -> binHeightX ( ix ) ;
    if( 0 >= A ) 
    {
      Error("Illegal histogram '" + norm1 -> title() + "' column content") ;
      return 0 ;
    }
    
    for ( int iy = 0 ; iy < yBins ; ++iy ) 
    {
      const double a = norm1 -> binHeight ( ix , iy ) / A ;
      if( 0 >= a ) 
      {
        Error( "Illegal histogram '" + norm1 -> title() + "' bin content") ;
        return 0 ;
      }
      
      const double x = xCoord ( norm1 , ix ) ;
      const double y = yCoord ( norm1 , iy ) ;
      norm2 -> fill( x , y , a ) ;
    }
  }
  
  return norm2 ;
};
// ============================================================================

// ============================================================================
/** prepare DeltaLL histogram (load or calculate)
 *  @return pointer to DeltaLL histogram 
 */
// ============================================================================
AIDA::IHistogram2D* CaloTrack2IdAlg::makeDeltaLL () const 
{
  // try toload it 
  AIDA::IHistogram2D* histo = loadHisto ( m_deltaLLhisto ) ;
  if ( 0 != histo ) { return histo ; }
  
  // try to prepare it
  AIDA::IHistogram2D* signalN = makeSignalN() ;
  if ( 0 == signalN ) 
  { Error(" 'SignalN' histogram is not available ") ; return 0 ; }
  
  AIDA::IHistogram2D* backgrN = makeBackgrN() ;
  if ( 0 == backgrN ) 
  { Error(" 'BackgrN' histogram is not available ") ; return 0 ; }
  
  if( !compare( signalN , backgrN ) ) 
  {
    Error("Different specifications histograms '"
          + signalN->title() + "' and '" + backgrN->title() + "'");
    return 0 ;
  }
  
  std::string dir    ( dirHbookName ( "Calo/" + name () )   );
  std::string title  ( "(Recalculated) Delta Log Likelihood from '" + 
                       signalN->title() + "' and '" + backgrN->title() + "' ") ;
  histo = 
    histoSvc() -> book ( dir , 105 , title , 
                         //
                         signalN -> xAxis () . bins      () , 
                         signalN -> xAxis () . lowerEdge () , 
                         signalN -> xAxis () . upperEdge () , 
                         //
                         signalN -> yAxis () . bins      () , 
                         signalN -> yAxis () . lowerEdge () , 
                         signalN -> yAxis () . upperEdge () ) ;
  if( 0 == histo ) 
  { Error ( "Unable to book histogram '" + title + "'" ) ; return 0 ; };
  
  if      ( !compare( histo , signalN )  )
  {
    Error ( "Incompatible specifications for histograms '"
            + histo -> title() + "' and '" + signalN -> title() + "'");
    return 0 ;
  }
  else if ( !compare( histo , backgrN )  )
  {
    Error ( "Incompatible specifications for histograms '"
            + histo -> title() + "' and '" + backgrN -> title() + "'");
    return 0 ;
  }
  
  const int xBins = histo -> xAxis() . bins() ;
  const int yBins = histo -> yAxis() . bins() ;
  
  for   ( int ix = 0 ; ix < xBins ; ++ix ) 
  {
    for ( int iy = 0 ; iy < yBins ; ++iy ) 
    {
      const double S = signalN -> binHeight( ix , iy ) ;
      const double B = backgrN -> binHeight( ix , iy ) ;
      if ( 0 >= S  ) 
      { 
        Error ( "Non-positive entry in normalized/adjusted 'SignalN'"); 
        return 0 ;
      }
      if ( 0 >= B  ) 
      {
        Error ( "Non-positive entry in normalized/adjusted 'BackgrN'");
        return 0 ;
      }
      
      const double x = xCoord ( signalN , ix ) ;
      const double y = yCoord ( signalN , iy ) ;
      
      // fill histo! 
      histo->fill ( x , y , log( S / B ) ) ;
      
    }
  }
  
  return histo ;
};
// ============================================================================

// ============================================================================
/** compare the specifications of 2 histograms 
 *  @param h1 pointer to the histogram  
 *  @param h2 pointer to the histogram  
 *  @return true if histogram hame the sam especifications 
 */
// ============================================================================
bool CaloTrack2IdAlg::compare 
( const AIDA::IHistogram2D* h1  , 
  const AIDA::IHistogram2D* h2  ) const 
{
  
  if ( 0 == h1 || 0 == h2 ) 
  {
    Error(" compare(): AIDA::IHistorgam2D* points to NULL");
    return false ;                                                  // RETURN 
  }
  
  if ( h1 == h2 ) { return true ; }                                 // RETURN 
  
  if ( h1 -> xAxis () .bins      () != 
       h2 -> xAxis () .bins      () ) { return false ; }             // RETURN
  if ( h1 -> xAxis () .lowerEdge () != 
       h2 -> xAxis () .lowerEdge () ) { return false ; }             // RETURN 
  if ( h1 -> xAxis () .upperEdge () != 
       h2 -> xAxis () .upperEdge () ) { return false ; }             // RETURN 
  if ( h1 -> yAxis () .bins      () != 
       h2 -> yAxis () .bins      () ) { return false ; }             // RETURN
  if ( h1 -> yAxis () .lowerEdge () != 
       h2 -> yAxis () .lowerEdge () ) { return false ; }             // RETURN 
  if ( h1 -> yAxis () .upperEdge () != 
       h2 -> yAxis () .upperEdge () ) { return false ; }             // RETURN
  
  
  { // check x-binning 
    const int bins = h1 -> xAxis () . bins () ;
    for   ( int i = 0 ; i < bins ; ++i ) 
    {
      if ( h1 -> xAxis () . binLowerEdge ( i ) != 
           h2 -> xAxis () . binLowerEdge ( i ) )   { return false ; } // RETURN 
      if ( h1 -> xAxis () . binUpperEdge ( i ) != 
           h2 -> xAxis () . binUpperEdge ( i ) )   { return false ; } // RETURN
    }
  }
  
  { // check y-binning 
    const int bins = h1 -> yAxis () . bins () ;
    for   ( int i = 0 ; i < bins ; ++i ) 
    {
      if ( h1 -> yAxis () . binLowerEdge ( i ) != 
           h2 -> yAxis () . binLowerEdge ( i ) )   { return false ; } // RETURN 
      if ( h1 -> yAxis () . binUpperEdge ( i ) != 
           h2 -> yAxis () . binUpperEdge ( i ) )   { return false ; } // RETURN
    }
  }
  
  return true ;
};
// ============================================================================

// ============================================================================
/** get the x-coordinate of the bin 
 *  @param histo histogram 
 *  @param bin bin number 
 *  @return coordinate of the bin center 
 */
// ============================================================================
double CaloTrack2IdAlg::xCoord 
( const AIDA::IHistogram2D* histo , 
  const int                 bin   ) const 
{
  if( 0 == histo ) 
  {
    Error(" xCoord(): AIDA::Ihistogram2D* pointes to NULL");
    return -1000 ;
  };
  
  return 0.5 * ( histo -> xAxis () .binLowerEdge ( bin ) + 
                 histo -> xAxis () .binUpperEdge ( bin )  ) ;
};
// ============================================================================

// ============================================================================
/** get the y-coordinate of the bin 
 *  @param histo histogram 
 *  @param bin bin number 
 *  @return coordinate of the bin center 
 */
// ============================================================================
double CaloTrack2IdAlg::yCoord  
( const AIDA::IHistogram2D* histo , 
  const int                 bin   ) const 
{
  if( 0 == histo ) 
  {
    Error(" yCoord(): AIDA::Ihistogram2D* pointes to NULL");
    return -1000 ;
  };
  
  return 0.5 * ( histo -> yAxis () .binLowerEdge ( bin ) + 
                 histo -> yAxis () .binUpperEdge ( bin )  ) ;
};
// ============================================================================


// ============================================================================
// The END 
// ============================================================================


