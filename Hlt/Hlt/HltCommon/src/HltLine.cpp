// $Id: HltLine.cpp,v 1.20 2009-04-18 11:51:19 graven Exp $
// ============================================================================
// Include files
// ============================================================================
// Boost
// ============================================================================
#include "boost/foreach.hpp"
#include "boost/lambda/lambda.hpp"
#include "boost/lambda/construct.hpp"
// ============================================================================
// AIDA 
// ============================================================================
#include "AIDA/IHistogram1D.h"
#include "AIDA/IHistogram.h"
#include "AIDA/IAxis.h"
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/IAlgManager.h"
#include "GaudiKernel/ListItem.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/IAlgContextSvc.h"

// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/ISequencerTimerTool.h"
// ============================================================================
// HLT
// ============================================================================
#include "HltBase/HltHistogramUtilities.h"
using namespace Hlt::HistogramUtilities;
// ============================================================================
// HltEvent 
// ============================================================================
#include "Event/HltDecReports.h"
// ============================================================================
// Local 
// ============================================================================
#include "HltLine.h"
// ============================================================================

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( HltLine );

//-----------------------------------------------------------------------------
// Implementation file for class : HltLine
//
// 2008-09-13 : Gerhard Raven
//-----------------------------------------------------------------------------
void
HltLine::HltStage::updateHandler(Property&) {
    if (!m_initialized) { 
        m_dirty=true;
        return;
    } 
    if (m_algorithm!=0) m_algorithm->release();
    if (!m_property.value().empty()) {
        m_algorithm = m_parent.getSubAlgorithm(m_property.value());
        if (!m_algorithm) throw GaudiException( "could not obtain algorithm for " , m_property.value(), StatusCode::FAILURE);
        m_algorithm->addRef();
    } else {
    }
    m_dirty = false;
}

StatusCode
HltLine::HltStage::initialize(ISequencerTimerTool* timer) {
    m_initialized=true;
    if ( m_dirty ) updateHandler(m_property);
    // TODO: bind timer call...
    if ( timer!=0 && algorithm()!=0 ) setTimer( timer->addTimer( algorithm()->name() ) );
    // empty transition is allowed...
    return algorithm()!=0 ? algorithm()->sysInitialize() : StatusCode::SUCCESS;
}

StatusCode
HltLine::HltStage::execute(ISequencerTimerTool* timertool) {
    assert(!m_dirty);
    if (!algorithm()              ) return StatusCode::SUCCESS;
    if (!algorithm()->isEnabled() ) return StatusCode::SUCCESS;
    if ( algorithm()->isExecuted()) return StatusCode::SUCCESS;
    // TODO: bind timer at init time
    if ( timertool ) timertool->start( timer() );
    StatusCode result = StatusCode::FAILURE;
    try {
        result = algorithm()->sysExecute();
        algorithm()->setExecuted( true );
    } catch (...) {
        result = StatusCode::FAILURE;
    }
    if ( timertool ) timertool->stop( timer() );
    return result;
}

HltLine::SubAlgos 
HltLine::retrieveSubAlgorithms() const {
 typedef std::list<std::pair<const Algorithm*,unsigned> > SubAlgoList;
 SubAlgoList subAlgo;
 subAlgo.push_back( std::make_pair(this,0));
 SubAlgoList::iterator i = subAlgo.begin();
 while ( i != subAlgo.end() ) {
    std::vector<Algorithm*> *subs = i->first->subAlgorithms();
    if (!subs->empty()) {
        unsigned depth = i->second+1;
        SubAlgoList::iterator j = i; 
        ++j;
        for (std::vector<Algorithm*>::const_iterator k = subs->begin();k!=subs->end();++k) 
            subAlgo.insert(j, std::make_pair( *k, depth ) );
    }
    ++i;
 }
 subAlgo.pop_front(); // remove ourselves...
 debug() << " dumping sub algorithms: " << endmsg;
 for (SubAlgoList::const_iterator i = subAlgo.begin(); i!= subAlgo.end();++i) {
    debug() << std::string(3+3*i->second,' ') << i->first->name() << endmsg;
 }
 // transform map such that it has algo, # of sub(sub(sub()))algorightms

 SubAlgos table;
 for (SubAlgoList::const_iterator i = subAlgo.begin(); i!= subAlgo.end();++i) {
    SubAlgoList::const_iterator j = i; ++j;
    while ( j!=subAlgo.end() && j->second > i->second ) ++j;
    table.push_back(std::make_pair( i->first, std::distance(i,j) ) );
 }
 return table;
}


IANNSvc& HltLine::annSvc() const {
  if (m_hltANNSvc == 0) {
    StatusCode sc = serviceLocator()->service("HltANNSvc", m_hltANNSvc);
    Assert( sc.isSuccess() && m_hltANNSvc != 0, " no HltANNSvc??");
  }
  return *m_hltANNSvc;
}

IHltDataSvc& HltLine::dataSvc() const {
  if (m_hltDataSvc == 0) {
    StatusCode sc = serviceLocator()->service("HltDataSvc", m_hltDataSvc);
    Assert( sc.isSuccess() && m_hltDataSvc != 0, " no HltDataSvc??");
  }
  return *m_hltDataSvc;
}
//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
HltLine::HltLine( const std::string& name,
                  ISvcLocator* pSvcLocator)
  : GaudiHistoAlg ( name , pSvcLocator )
  , m_timerTool( 0 )
  , m_jos(0)
  , m_algMgr(0)
  , m_stageHisto(0)
  , m_errorHisto(0)
  , m_hltANNSvc(0)
  , m_hltDataSvc(0)
  , m_selection(0)
  , m_acceptRate(0)
{
  for (unsigned i=0; i<m_stages.size(); ++i) {
    m_stages[i] = new HltStage(*this, transition(stage(i)));
    declareProperty( m_stages[i]->property().name() , m_stages[i]->property() );
  }
  declareProperty( "HltDecReportsLocation", m_outputContainerName   = LHCb::HltDecReportsLocation::Default );
  declareProperty( "DecisionName"         , m_decision       = name+"Decision"); //TODO: install updateHandler, refuse changes after initialize...
  declareProperty( "IgnoreFilterPassed"   , m_ignoreFilter   = false );
  declareProperty( "MeasureTime"          , m_measureTime    = false );
  declareProperty( "ReturnOK"             , m_returnOK       = false );
  declareProperty( "AcceptOnError"        , m_acceptOnError  = false );

}
//=============================================================================
// Destructor
//=============================================================================
HltLine::~HltLine() { };



//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode HltLine::initialize() {
  /// initialize the base:

  StatusCode status = GaudiHistoAlg::initialize();
  if ( !status.isSuccess() ) return status;

  /// lock the context 
  Gaudi::Utils::AlgContext lock1 ( this , contextSvc() ) ;
  
  debug() << "==> Initialize" << endreq;
  m_jos    = svc<IJobOptionsSvc>( "JobOptionsSvc"  );
  m_algMgr = svc<IAlgManager>   ( "ApplicationMgr" );

  m_timerTool = tool<ISequencerTimerTool>( "SequencerTimerTool" );
  if ( m_timerTool->globalTiming() ) m_measureTime = true;

  if ( m_measureTime ) {
    m_timer = m_timerTool->addTimer( name() );
    m_timerTool->increaseIndent();
  } else {
    release( m_timerTool );
    m_timerTool = 0;
  }

  //== Initialize the stages
  StatusCode sc;
  BOOST_FOREACH( HltStage* i, m_stages) {
    sc = i->initialize(m_timerTool);
    if (!sc.isSuccess()) Error( "Failed to initialize " + i->name(), sc );
  }
  //== now pick up (if exists) the selection created by the 'decision stage'
  //   if it exists, then it was just created during the above 'initialize' of 
  //   the stages...
  stringKey key(m_decision);
  m_selection = dataSvc().selection(key,this);

  //== pick up (recursively!) our sub algorithms and their depth count
  //   so we can figure out in detail where we stalled...
 m_subAlgo = retrieveSubAlgorithms();

 //NOTE: when checking filterPassed: a sequencer can be 'true' even if some member is false...
 //ANSWER: in case positive, we skip checking all algos with depth count > current one...
 //        in case negative, we descend, and repeat there, or, if next entry has no 
 //        depth count larger, we fill bin at current position..

  //== Create the monitoring histogram
  m_errorHisto = book1D(name()+" error",name()+" error",-0.5,7.5,8);
  m_stageHisto = book1D(m_decision,     m_decision,     -0.5,7.5,8);
  m_timeHisto  = book1D(name()+" walltime",name()+" log(wall time/ms)",-3,6);
  m_stepHisto  = book1D(name()+" rejection stage", name()+ " rejection stage",-0.5,m_subAlgo.size()-0.5,m_subAlgo.size() );
  // if possible, add labels to axis...
  std::vector<std::string> stepLabels;
  for (SubAlgos::const_iterator i = m_subAlgo.begin();i!=m_subAlgo.end();++i) {
      stepLabels.push_back( i->first->name() );
  }
  if (!setBinLabels( m_stepHisto, stepLabels )) {
    error() << " Could not set bin labels in step histo " << endmsg;
  }

  //== and the counters
  declareInfo("#accept","",&counter("#accept"),0,std::string("Events accepted by ") + m_decision);
  counter("#errors"); // do not export to DIM -- we have a histogram for this..
  //declareInfo("#errors","",&counter("#errors"),0,std::string("Errors seen by ") + m_decision);

  m_acceptRate=0;
  declareInfo("COUNTER_TO_RATE["+m_decision+"Accept]", m_acceptRate, m_decision + " Accept Rate");

  if ( m_measureTime ) m_timerTool->decreaseIndent();
  
  return StatusCode::SUCCESS;
};

//=============================================================================
// Main execution
//=============================================================================
StatusCode HltLine::execute() {
  longlong startClock = System::currentTime( System::microSec );
  
  /// lock the context 
  Gaudi::Utils::AlgContext lock1 ( this , contextSvc() ) ;
  
  if ( m_measureTime ) m_timerTool->start( m_timer );

  debug() << "==> Execute" << endreq;
  StatusCode result = StatusCode::SUCCESS;
  LHCb::HltDecReports* reports = getOrCreate<LHCb::HltDecReports,LHCb::HltDecReports>(m_outputContainerName);
  if (reports->hasSelectionName( m_decision ) ) { 
    error() << "HltDecReports already contains report" << endmsg;
    return StatusCode::FAILURE;
  }

  // TODO: do this upfront, 'lock' updates after initialize...
  static const std::string major1("Hlt1SelectionID");
  static const std::string major2("Hlt2SelectionID");
  const std::string* major = 0;
  if (m_decision.find("Hlt1")!=std::string::npos) {
    major =  &major1;
  } else if (m_decision.find("Hlt2")!=std::string::npos) {
    major = &major2;
  } else {
    error() << " Could not find Hlt1/Hlt2 in decision " << m_decision << " ???? " << endmsg;
  }


  boost::optional<IANNSvc::minor_value_type> key = annSvc().value(*major,m_decision);

  if (!key) {
    warning() << " selectionName=" << m_decision << " not known under major " << *major << endmsg;
  }
  //TODO: add c'tor with only selID
  LHCb::HltDecReport report( 0, 0, 0, key->second );
  if (report.invalidIntSelectionID()) {
    warning() << " selectionName=" << key->first << " has invalid intSelectionID=" << key->second << endmsg;
  } 
  bool accept = !m_stages.empty();
  for (unsigned i=0;i<m_stages.size();++i) {
     result = m_stages[i]->execute();
     if (result.isFailure()) {
        report.setErrorBits(1);   //TODO: different value depending on error type..
        accept = m_acceptOnError; //TODO: don't allow infinite # of accepts on error...
        break;
     }
     accept = m_stages[i]->passed();
     if ( !accept ) break;
     report.setExecutionStage( i+1 );
  }
  // plot the wall clock time spent...
  double elapsedTime = double(System::currentTime( System::microSec ) - startClock);
  fill( m_timeHisto, log10(elapsedTime)-3 ,1.0); // convert to millisec

  if (elapsedTime>5000) { //TODO: flag an error..
      report.setErrorBits( report.errorBits() | 0x2 );
  }

  report.setDecision(accept ? 1u : 0u);
  report.setNumberOfCandidates( m_selection != 0 ? m_selection->size() : 0 );
  if ( !m_ignoreFilter ) setFilterPassed( accept );
  setExecuted( true );

  //TODO: allow insert at the beginning, and non-const access to update...
  reports->insert( key->first , report );

  // update monitoring
  counter("#accept") += accept;
  if (accept) ++m_acceptRate;
  counter("#errors") += ( report.errorBits()!=0);

  fill( m_stageHisto, report.executionStage(),1.0);
  fill( m_errorHisto, report.errorBits(),1.0);
  // make stair plot
  SubAlgos::const_iterator i = m_subAlgo.begin();
  while ( i != m_subAlgo.end() ) {
     if (i->first->filterPassed()) {
        i+=i->second;
     } else {
        if (i->second==1) break; // don't have subalgos, so this is where we stopped
        ++i; // descend into subalgorithms, figure out which one failed.....
        // Note: what to do if subalgos pass, but parent failed?? 
        // actually need to invert parent/daughters, such that if daugthers OK,
        // but parent isn't, we enter the plot at the parent, but that should be
        // _after_ the daughters...
     }
  }
  fill( m_stepHisto, i-m_subAlgo.begin(), 1.0);
  if ( m_measureTime ) m_timerTool->stop( m_timer );

  return m_returnOK ? StatusCode::SUCCESS : result;
};

//=========================================================================
// reset the executed status of all members
//=========================================================================
void HltLine::resetExecuted ( ) {
  Algorithm::resetExecuted();
  // algorithm doesn't call resetExecuted of subalgos! should it???
  BOOST_FOREACH( HltStage* i, m_stages) i->resetExecuted();
}

//=========================================================================
//  Obtain pointer to an instance of the requested algorithm.
//=========================================================================
Algorithm* HltLine::getSubAlgorithm(const std::string& algname) {
    ListItem name(algname);
    IAlgorithm* myIAlg(0);
    Algorithm *myAlg(0);
    //== Check wether the specified algorithm already exists. If not, create it
    StatusCode result = m_algMgr->getAlgorithm( name.name(), myIAlg );
    if ( result.isSuccess() ) {
        myAlg = dynamic_cast<Algorithm*>(myIAlg);
        subAlgorithms()->push_back(myAlg) ;
        return myAlg;
    }
    bool addedContext = false;  //= Have we added the context ?
    bool addedRootInTES = false;  //= Have we added the rootInTES ?
    bool addedGlobalTimeOffset = false;  //= Have we added the globalTimeOffset ?

    //== Set the Context if not in the jobOptions list
    if ( ""  != context() ||
         ""  != rootInTES() ||
         0.0 != globalTimeOffset() ) {
      bool foundContext = false;
      bool foundRootInTES = false;
      bool foundGlobalTimeOffset = false;
      const std::vector<const Property*>* properties = m_jos->getProperties( name.name() );
      if ( 0 != properties ) {
        // Iterate over the list to set the options
        for ( std::vector<const Property*>::const_iterator itProp = properties->begin();
             itProp != properties->end();
             itProp++ )   {
            if (!foundContext) foundContext = ( "Context" == (*itProp)->name() ) ;
            if (!foundRootInTES) foundRootInTES = ( "RootInTES" == (*itProp)->name() ) ;
            if (!foundGlobalTimeOffset) foundGlobalTimeOffset = ( "GlobalTimeOffset" == (*itProp)->name() );
        }
      }
      if ( !foundContext && "" != context() ) {
        m_jos->addPropertyToCatalogue( name.name(), StringProperty( "Context", context() ) ).ignore();
        addedContext = true;
      }
      if ( !foundRootInTES && "" != rootInTES() ) {
        m_jos->addPropertyToCatalogue( name.name(), StringProperty( "RootInTES", rootInTES() ) ).ignore();
        addedRootInTES = true;
      }
      if ( !foundGlobalTimeOffset && 0.0 != globalTimeOffset() ) {
        m_jos->addPropertyToCatalogue( name.name(), DoubleProperty( "GlobalTimeOffset", globalTimeOffset() ) ).ignore();
        addedGlobalTimeOffset = true;
      }
    }

    result = createSubAlgorithm( name.type(), name.name(), myAlg );
    if (result.isFailure()) { return 0; }

    //== Remove the property, in case this is not a GaudiAlgorithm...
    // TODO: weird: we add it, and if we added it, we remove it again???
    if ( addedContext ) {
      m_jos->removePropertyFromCatalogue( name.name(), "Context" ).ignore();
    }
    if ( addedRootInTES ) {
      m_jos->removePropertyFromCatalogue( name.name(), "RootInTES" ).ignore();
    }
    if ( addedGlobalTimeOffset ) {
      m_jos->removePropertyFromCatalogue( name.name(), "GlobalTimeOffset" ).ignore();
    }
    return myAlg;
}
//=============================================================================
