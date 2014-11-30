// $Id: SelectionLine.cpp,v 1.3 2010-02-27 09:08:24 graven Exp $
// ============================================================================
// Include files
// ============================================================================
#include <cmath>
#include <vector>
#include <deque>
#include <iterator>
#include <utility>
#include <chrono>
// ============================================================================
// Boost
// ============================================================================
// ============================================================================
// AIDA
// ============================================================================
#include "AIDA/IHistogram1D.h"
#include "AIDA/IProfile1D.h"
#include "AIDA/IHistogram.h"
#include "AIDA/IAxis.h"
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/StringKey.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/IAlgManager.h"
#include "GaudiKernel/ListItem.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/IAlgContextSvc.h"
#include "GaudiKernel/IIncidentSvc.h"

// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/ISequencerTimerTool.h"
// ============================================================================
// HLT
// ============================================================================
#include "HistogramUtilities.h"
using namespace HistogramUtilities;
// ============================================================================
// HltEvent
// ============================================================================
#include "Event/HltDecReports.h"
// ============================================================================
// Local
// ============================================================================
#include "Kernel/SelectionLine.h"
// ============================================================================


//TODO: this  adds C++14 'make_unique'... remove once we move to C++14...
template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args )
{
        return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}

namespace 
{
  static const double timeHistoLowBound = -3;  

  bool isDefault(const std::string& s) { return s.empty(); } // empty is not constexpr???
  constexpr bool isDefault(double x) { return x == 0; }

  class populate_JobOptionsSvc_t {
    std::vector<std::unique_ptr<Property>> m_props;
    IJobOptionsSvc* m_jos;
    std::string m_name;

    template <typename T> void process(T&& t) {
        if (!isDefault(t.second)) m_props.push_back( make_unique< SimpleProperty<typename T::second_type>> ( std::move(t.first), std::move(t.second) ) );
    }
    template <typename T, typename... Args> void process(T&& t, Args&&... args) {
        process(std::forward<T>(t)); process(std::forward<Args>(args)...);
    }
    void check_veto() { // avoid changing properties expliclty present in the JOS...
        const auto* props = m_jos->getProperties(m_name);
        if (!props) return;
        for (const auto& i : *props ) {
            auto j = std::find_if( std::begin(m_props), std::end(m_props), [&i](const std::unique_ptr<Property>& prop) { 
                return prop->name() == i->name(); 
            } );
            if (j==std::end(m_props)) continue;
            m_props.erase( j );
            if (m_props.empty()) return; // done!
        }
    }

  public:
    template <typename... Args>
    populate_JobOptionsSvc_t( std::string name, IJobOptionsSvc* jos, Args&&... args ) : m_jos{jos},m_name{ std::move(name) }   {
        process(std::forward<Args>(args)...);
        if (!m_props.empty()) check_veto();
        std::for_each( std::begin(m_props), std::end(m_props), [&](const std::unique_ptr<Property>& i ) {
            m_jos->addPropertyToCatalogue( m_name, *i ).ignore();
        } );
    }
    ~populate_JobOptionsSvc_t() {
        std::for_each( std::begin(m_props), std::end(m_props), [&](const std::unique_ptr<Property>& i ) {
            m_jos->removePropertyFromCatalogue( m_name, i->name() ).ignore();
        } );
    }
  };
}


//-----------------------------------------------------------------------------
// Implementation file for class : HltLine
//
// 2008-09-13 : Gerhard Raven
//-----------------------------------------------------------------------------

void
Selection::Line::Stage::updateHandler(Property&)
{
  if ( !m_initialized ) 
  {
    m_dirty=true;
    return;
  }
  if ( m_algorithm ) m_algorithm->release();
  if ( !m_property.value().empty() )
  {
    m_algorithm = m_parent.getSubAlgorithm(m_property.value());
    if ( !m_algorithm ) throw GaudiException( "could not obtain algorithm for " ,
                                              m_property.value(),
                                              StatusCode::FAILURE );
    m_algorithm->addRef();
  }
  m_dirty = false;
}

StatusCode
Selection::Line::Stage::initialize(ISequencerTimerTool* timer)
{
  m_initialized = true;
  if ( m_dirty ) updateHandler(m_property);
  // TODO: bind timer call...
  if ( timer && algorithm() ) setTimer( timer->addTimer( algorithm()->name() ) );
  // empty transition is allowed...
  return ( algorithm() ? algorithm()->sysInitialize() : StatusCode::SUCCESS );
}

StatusCode
Selection::Line::Stage::execute(ISequencerTimerTool* timertool)
{
  assert(!m_dirty);
  if ( !algorithm()               ) return StatusCode::SUCCESS;
  if ( !algorithm()->isEnabled()  ) return StatusCode::SUCCESS;
  if (  algorithm()->isExecuted() ) return StatusCode::SUCCESS;
  // TODO: bind timer at init time
  if ( timertool ) timertool->start( timer() );
  StatusCode result = algorithm()->sysExecute();
  algorithm()->setExecuted( true );
  if ( timertool ) timertool->stop( timer() );
  return result;
}

Selection::Line::SubAlgos
Selection::Line::retrieveSubAlgorithms() const
{
  std::deque<std::pair<const Algorithm*,unsigned> > subAlgo;
  subAlgo.emplace_back( this,0 );
  for (auto i = std::begin(subAlgo);  i != std::end(subAlgo); ++i ) 
  {
    auto* subs = i->first->subAlgorithms();
    if ( !subs->empty() ) {
      auto depth = i->second+1;
      auto j = std::next(i);
      for ( const auto& k : *subs ) subAlgo.insert( j, { k, depth } );
    }
  }
  subAlgo.pop_front(); // remove ourselves...

  if ( msgLevel(MSG::DEBUG) )
  {
    debug() << "Dumping sub algorithms :" << endmsg;
    for ( const auto& ii : subAlgo )
    {
      debug() << std::string( 3 + 3*ii.second , ' ' ) << ii.first->name() << endmsg;
    }
  }

  // transform map such that it has algo, # of sub(sub(sub()))algorithms
  SubAlgos table;
  for ( auto ii = std::begin(subAlgo); ii != std::end(subAlgo); ++ii )
  {
    auto j = std::next(ii);
    while ( j != subAlgo.end() && j->second > ii->second ) ++j;
    table.emplace_back(  ii->first, std::distance(ii,j) );
  }
  return table;
}

IANNSvc& Selection::Line::annSvc() const 
{
  if ( !m_hltANNSvc ) 
  {
    const StatusCode sc = serviceLocator()->service(s_ANNSvc, m_hltANNSvc);
    Assert( sc.isSuccess() && m_hltANNSvc, " no ANNSvc??");
  }
  return *m_hltANNSvc;
}

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
Selection::Line::Line( const std::string& name,
                       ISvcLocator* pSvcLocator)
  : GaudiHistoAlg ( name , pSvcLocator )
  , m_timerTool{ nullptr }
  , m_jos{ nullptr }
  , m_algMgr{ nullptr }
  , m_errorHisto{ nullptr }
  , m_timeHisto{ nullptr }
  , m_stepHisto{ nullptr }
  , m_candHisto{ nullptr }
  , m_hltANNSvc{ nullptr }
  , m_acceptCounter{ nullptr }
  , m_errorCounter{ nullptr }
  , m_slowCounter{ nullptr }
  , m_timer(0)
  , m_nAcceptOnError(0)
{
  for ( unsigned i = 0; i < m_stages.size(); ++i )
  {
    m_stages[i] = make_unique<Stage>(*this, transition(stage(i)));
    declareProperty( m_stages[i]->property().name() , m_stages[i]->property() );
  }
  declareProperty( "HltDecReportsLocation", 
                   m_outputContainerName = LHCb::HltDecReportsLocation::Default );
  //TODO: install updateHandler, refuse changes after initialize...
  declareProperty( "DecisionName"         , m_decision       = name+"Decision");
  declareProperty( "ANNSvc"               , s_ANNSvc         = "HltANNSvc");
  declareProperty( "IgnoreFilterPassed"   , m_ignoreFilter   = false );
  declareProperty( "MeasureTime"          , m_measureTime    = false );
  declareProperty( "ReturnOK"             , m_returnOK       = false );
  declareProperty( "Turbo"                , m_turbo          = false );
  declareProperty( "AcceptOnError"        , m_acceptOnError  = true );
  declareProperty( "AcceptOnIncident"     , m_acceptOnIncident = true );
  declareProperty( "AcceptIfSlow"         , m_acceptIfSlow   = false );
  declareProperty( "MaxAcceptOnError"     , m_maxAcceptOnError   = -1 ); // -1: no quota # TODO: make this a throttelable rate...
  declareProperty( "FlagAsSlowThreshold"  , m_slowThreshold  = 500000, "microseconds"  );
  declareProperty( "IncidentsToBeFlagged" , m_incidents);

}

//=============================================================================
// Destructor
//=============================================================================
Selection::Line::~Line() 
{
}

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode Selection::Line::initialize() 
{
  // initialize the base:
  StatusCode status = GaudiHistoAlg::initialize();
  if ( status.isFailure() ) return status;

  /// lock the context
  Gaudi::Utils::AlgContext lock1 ( this , contextSvc() ) ;

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endreq;
  m_jos    = svc<IJobOptionsSvc>( "JobOptionsSvc"  );
  m_algMgr = svc<IAlgManager>   ( "ApplicationMgr" );

  // register for incidents...
  IIncidentSvc* incidentSvc = svc<IIncidentSvc>( "IncidentSvc" );
  for(  const std::string& s : m_incidents )
  {
    bool rethrow = false; bool oneShot = false; long priority = 0;
    incidentSvc->addListener(this,s,priority,rethrow,oneShot);
  }

  m_timerTool = tool<ISequencerTimerTool>( "SequencerTimerTool" );
  if ( m_timerTool->globalTiming() ) m_measureTime = true;

  if ( m_measureTime ) 
  {
    m_timer = m_timerTool->addTimer( name() );
    m_timerTool->increaseIndent();
  }
  else
  {
    release( m_timerTool );
    m_timerTool = 0;
  }

  //== Initialize the stages
  for ( auto& i : m_stages) 
  {
    const StatusCode sc = i->initialize(m_timerTool);
    if ( sc.isFailure() ) 
    { return Error( "Failed to initialize " + i->name(), sc ); }
  }

  //== pick up (recursively!) our sub algorithms and their depth count
  //   so we can figure out in detail where we stalled...
  m_subAlgo = retrieveSubAlgorithms();

  //NOTE: when checking filterPassed: a sequencer can be 'true' even if some member is false...
  //ANSWER: in case positive, we skip checking all algos with depth count > current one...
  //        in case negative, we descend, and repeat there, or, if next entry has no
  //        depth count larger, we fill bin at current position..

  //== Create the monitoring histograms
  m_errorHisto = book1D("error",name()+" error",-0.5,7.5,8);
  m_timeHisto  = book1D("walltime",name()+" log(wall time/ms)",timeHistoLowBound,6);
  m_stepHisto  = book1D("rejection stage", name()+ " rejection stage",
                        -0.5,m_subAlgo.size()-0.5,m_subAlgo.size() );
  m_candHisto  = bookProfile1D("candidates accepted", name()+" candidates accepted",
                               -0.5,m_subAlgo.size()-0.5,m_subAlgo.size() );
  // if possible, add labels to axis...
  // Remove common part of the name for easier label reading (assumes name is suff. descriptive)
  std::vector<std::string> stepLabels;
  const std::string common = name();
  for ( const auto& i :  m_subAlgo ) {
    std::string stepname = i.first->name();
    const std::string::size_type j = stepname.find(common);
    if (j!=std::string::npos) stepname.erase(j,common.size());
    stepLabels.push_back( std::move(stepname) );
  }
  if ( !setBinLabels( m_stepHisto, stepLabels ) ) 
  {
    error() << "Could not set bin labels in step histo" << endmsg;
  }
  if ( !setBinLabels( m_candHisto, stepLabels ) )
  {
    error() << "Could not set bin labels in cand histo" << endmsg;
  }

  //== and the counters
  m_acceptCounter = &counter("#accept");
  declareInfo("COUNTER_TO_RATE[#accept]",*m_acceptCounter,
              std::string("Events accepted by ") + m_decision);
  m_errorCounter = &counter("#errors"); // do not export to DIM -- we have a histogram for this..
  //declareInfo("#errors",counter("#errors"),std::string("Errors seen by ") + m_decision);
  m_slowCounter = &counter("#slow events");// do not export to DIM -- we have a histogram for this..

  m_nAcceptOnError = 0;

  if ( m_measureTime ) m_timerTool->decreaseIndent();

  // Is the line a Turbo line?
  // If so inform the HltDecReports
  IAlgorithm* myIAlg{nullptr};
  auto isTurbo = m_algMgr->getAlgorithm( std::string{"TurboMIAB"} +m_decision.substr(4, m_decision.size()-12), myIAlg );
  if ( m_turbo != isTurbo.isSuccess() ) {
      std::string msg = std::string{ "Line is " } + (m_turbo?" ":"not " ) + "flagged as turbo, but TurboMIAB"+name()+" is " + (myIAlg?" ":"not") +" present";
      error() << msg << endmsg;
      return Error( msg, StatusCode::FAILURE);
  }
  return status;
}

//TODO: on a runchange, reset all counters like m_nAcceptOnError...

//=============================================================================
// Main execution
//=============================================================================
StatusCode Selection::Line::execute() 
{
  auto startClock = std::chrono::high_resolution_clock::now();

  /// lock the context
  Gaudi::Utils::AlgContext lock1 ( this , contextSvc() ) ;

  if ( m_measureTime ) m_timerTool->start( m_timer );

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endreq;
  StatusCode result = StatusCode::SUCCESS;
  LHCb::HltDecReports* reports = 
    getOrCreate<LHCb::HltDecReports,LHCb::HltDecReports>(m_outputContainerName);
  if ( reports->hasDecisionName(m_decision) ) 
  {
    return Error( "HltDecReports already contains report" );
  }

  std::pair<std::string,unsigned> key = id();

  //TODO: add c'tor with only ID
  LHCb::HltDecReport report( 0, 0, 0, key.second );
  if ( report.invalidIntDecisionID() ) 
  {
    warning() << " DecisionName=" << key.first 
              << " has invalid intDecisionID=" << key.second << endmsg;
  }
  bool accept = !m_stages.empty(); // make sure an empty line always rejects events...
  m_caughtIncident = false; // only interested in incidents during stages->execute...

  for  ( unsigned i = 0; i < m_stages.size(); ++i ) {
    result = m_stages[i]->execute();
    if (m_caughtIncident) {
      report.setErrorBits(report.errorBits() | 0x02);
      m_caughtIncident = false;
    }
    if (result.isFailure()) {
      report.setErrorBits(report.errorBits() | 0x01);
      break;
    }
    accept = m_stages[i]->passed();
    if ( !accept ) break;
    report.setExecutionStage( i+1 );
  }
  // Is the line a Turbo line? If so, inform the HltDecReports
  if ( m_turbo ) report.setExecutionStage( 254 ); // TODO: if (m_turbo) report.setExecutionStage( 0x80 | report.executionStage() );

  // plot the wall clock time spent..
  using ms = std::chrono::duration<double, std::chrono::milliseconds::period>;
  auto elapsedTime = std::chrono::duration_cast<ms>(std::chrono::high_resolution_clock::now() - startClock);
  // protect against 0 
  const double logElapsedTimeMS = elapsedTime.count() > 0 ? log10(elapsedTime.count()) : timeHistoLowBound;
  
  fill( m_timeHisto, logElapsedTimeMS ,1.0);
  if (elapsedTime.count()>m_slowThreshold) report.setErrorBits( report.errorBits() | 0x4 );

  // did not(yet) accept, but something bad happened...
  if ( !accept && report.errorBits() != 0 && 
       ( m_nAcceptOnError < m_maxAcceptOnError || m_maxAcceptOnError<0) ) {
    accept =  ( m_acceptOnError    && ( (report.errorBits()&0x01)!=0) )
           || ( m_acceptOnIncident && ( (report.errorBits()&0x02)!=0) )
           || ( m_acceptIfSlow     && ( (report.errorBits()&0x04)!=0) );
    if (accept) ++m_nAcceptOnError;
  }

  report.setDecision(accept ? 1u : 0u);
  report.setNumberOfCandidates( numberOfCandidates() );
  if ( !m_ignoreFilter ) setFilterPassed( accept );
  setExecuted( true );

  //TODO: allow insert at the beginning, and non-const access to update...
  reports->insert( key.first , report );

  // update monitoring
  *m_acceptCounter += accept;
  // don't flag slow events as error, so mask the bit
  *m_errorCounter  += ( (report.errorBits()&~0x4)!=0 ) ;
  *m_slowCounter   += ( (report.errorBits()& 0x4)!=0 ) ;

  fill( m_errorHisto, report.errorBits(), 1.0);
  // make stair plot
  auto last = m_subAlgo.begin();
  while ( last != m_subAlgo.end() ) {
    if (last->first->filterPassed()) {
      last+=last->second;
    } else {
      if (last->second==1) break; // don't have subalgos, so this is where we stopped
      ++last; // descend into subalgorithms, figure out which one failed.....
      // Note: what to do if subalgos pass, but parent failed??  (yes, this is possible!)
      // actually need to invert parent/daughters, such that if daughters OK,
      // but parent isn't, we enter the plot at the _parent_, but that should appear
      // _after_ the daughters (which may be confusing)...
    }
  }
  fill( m_stepHisto, std::distance(m_subAlgo.begin(),last), 1.0);
  
  for ( SubAlgos::iterator i = std::begin(m_subAlgo); i != last; ++i ) {
    fill(m_candHisto,std::distance( std::begin(m_subAlgo),i),numberOfCandidates(i->first),1.0);
  }
  if ( m_measureTime ) m_timerTool->stop( m_timer );

  return m_returnOK ? StatusCode::SUCCESS : result;
}

std::vector< const Algorithm* > Selection::Line::algorithms() const 
{
  std::vector< const Algorithm* > subs; subs.reserve( m_subAlgo.size() );
  for( const auto& i : m_subAlgo ) subs.push_back(i.first);
  return subs;
}

//=========================================================================
// reset the executed status of all members
//=========================================================================
void Selection::Line::resetExecuted ( ) 
{
  Algorithm::resetExecuted();
  // algorithm doesn't call resetExecuted of subalgos! should it???
  for ( auto& i : m_stages ) { i->resetExecuted(); }
}

//=========================================================================
// listen for incident during processing...
//=========================================================================
void Selection::Line::handle( const Incident& ) 
{
  m_caughtIncident = true;
}

//=========================================================================
//  Obtain pointer to an instance of the requested algorithm.
//=========================================================================
Algorithm* Selection::Line::getSubAlgorithm(const std::string& algname) 
{
  ListItem name(algname);
  IAlgorithm* myIAlg{nullptr};
  Algorithm *myAlg{nullptr};
  //= Check wether the specified algorithm already exists. 
  if ( m_algMgr->getAlgorithm( name.name(), myIAlg ).isSuccess() ) {
    myAlg = dynamic_cast<Algorithm*>(myIAlg);
    subAlgorithms()->push_back(myAlg) ;
    return myAlg;
  }
  // It doesn't. Create it, and while doing so, ensure some magic properties are propagated...
  populate_JobOptionsSvc_t populate{ name.name(), m_jos,
       std::make_pair( "Context",          context() ),
       std::make_pair( "RootInTES",        rootInTES() ),
       std::make_pair( "GlobalTimeOffset", globalTimeOffset() )
  };
  return createSubAlgorithm( name.type(), name.name(), myAlg ).isSuccess() ? myAlg : nullptr;
}
//=============================================================================
