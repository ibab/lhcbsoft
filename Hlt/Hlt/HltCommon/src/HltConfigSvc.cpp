// $Id: HltConfigSvc.cpp,v 1.30 2009-06-30 14:32:25 graven Exp $
// Include files 

#include <algorithm>

#include "boost/lexical_cast.hpp"
#include "boost/foreach.hpp"
#include "boost/lambda/lambda.hpp"
#include "boost/lambda/bind.hpp"
namespace bl=boost::lambda;

// from Gaudi
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/SvcFactory.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/GaudiException.h"

#include "Event/ODIN.h"
#include "Event/HltDecReports.h"

// local
#include "HltConfigSvc.h"

using namespace std;
using boost::lexical_cast;


//-----------------------------------------------------------------------------
// Implementation file for class : HltConfigSvc
//
// 2007-10-24 : Gerhard Raven
//-----------------------------------------------------------------------------

// Declaration of the Service Factory
DECLARE_SERVICE_FACTORY( HltConfigSvc );

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
HltConfigSvc::HltConfigSvc( const string& name, ISvcLocator* pSvcLocator)
  : PropertyConfigSvc( name , pSvcLocator )
  , m_configuredTCK(0)
  , m_evtSvc(0)
  , m_incidentSvc(0)
  // , m_decodeOdin(0)
  , m_taskNumber(~0u)
{
  //TODO: template this pattern of property + 'transformer' -> thing_I_really_want with callback support
  //TODO: Already done -- called propertyhandler...[:w
  declareProperty("TCK2ConfigMap", m_tck2config_)->declareUpdateHandler( &HltConfigSvc::updateMap, this);
  declareProperty("initialTCK", m_initialTCK_ )->declareUpdateHandler( &HltConfigSvc::updateInitial, this); 
  declareProperty("prefetchDir", m_prefetchDir);
  declareProperty("checkOdin", m_checkOdin = true);
  // declareProperty("decodeOdinOnDemand", m_decodeOdinOnDemand = true);
  declareProperty("maskL0TCK", m_maskL0TCK = false);
  //declareProperty( "HltDecReportsLocation", m_outputContainerName = std::string("/Event/")+LHCb::HltDecReportsLocation::Default );
  m_outputContainerName = std::string("/Event/")+LHCb::HltDecReportsLocation::Default ;
}

void HltConfigSvc::updateMap(Property&) {
  m_tck2config.clear();
  typedef std::pair<std::string,std::string> val_t;
  BOOST_FOREACH( const val_t& i, m_tck2config_ ) {
    m_tck2config.insert( make_pair( TCKrep( i.first ), i.second)  );
  }
}

void HltConfigSvc::updateInitial(Property&) {
    m_initialTCK = TCKrep(m_initialTCK_);
}

//=============================================================================
// Destructor
//=============================================================================
HltConfigSvc::~HltConfigSvc() {
   // if not released already by now, then do it now...
} 


//=============================================================================
// Finalization
//=============================================================================
StatusCode HltConfigSvc::finalize() {
  m_evtSvc->release();      m_evtSvc=0;
  if (m_incidentSvc!=0) { m_incidentSvc->release(); m_incidentSvc=0; }
  return PropertyConfigSvc::finalize();
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode HltConfigSvc::initialize() {
  // see if we're running online... HLTXYY_ZZZZ_#
  std::string taskName( System::argv()[0] );
#if 0
  if (match) { 
    m_taskNumber = ...;
#if linux
    stuct hostent* host = gethostent()

    m_host = gethostent()
#endif
#endif
  


  StatusCode status = PropertyConfigSvc::initialize();
  if ( !status.isSuccess() ) return status;

  if (!service( "EventDataSvc", m_evtSvc).isSuccess()) return StatusCode::FAILURE;

  //if (m_decodeOdinOnDemand) {
  //  m_decodeOdin = tool<IGenericTool>( ... );
  //}


  if (m_checkOdin) {
      if (!service( "IncidentSvc", m_incidentSvc).isSuccess()) return StatusCode::FAILURE;
      // add listener to be triggered by first BeginEvent with low priority
      // so it gets called first
      bool rethrow = false;
      bool oneShot = false;
      m_incidentSvc->addListener(this,IncidentType::BeginEvent,
                                 std::numeric_limits<long>::min(),rethrow,oneShot);
  }

  
  // load all TCKs... (brute force, but OK for now...)
  std::vector<ConfigTreeNodeAlias> tcks = cas()->configTreeNodeAliases(ConfigTreeNodeAlias::alias_type( std::string("TCK/") ));
  BOOST_FOREACH( const ConfigTreeNodeAlias& tck, tcks ) tck2id( TCKrep( tck.alias().str().substr(4) ) );

  // find the ID of the initial TCK
  ConfigTreeNode::digest_type initialID = tck2id( TCKrep( m_initialTCK ) );

  if (std::count(m_prefetchDir.begin(),m_prefetchDir.end(),'/')!=0) {
    error() << " prefetch directory "  << m_prefetchDir << " is too specific" << endmsg;
    return StatusCode::FAILURE;
  }

  // load all TOPLEVEL aliases for specified release
  std::vector<ConfigTreeNodeAlias> tops = cas()->configTreeNodeAliases(ConfigTreeNodeAlias::alias_type( std::string("TOPLEVEL/")+m_prefetchDir+'/' ));
  std::vector<ConfigTreeNodeAlias>::const_iterator initTop = std::find_if( tops.begin(), tops.end(), bl::bind(&ConfigTreeNodeAlias::ref,bl::_1)==initialID );
  if (initTop == tops.end() ) {
    error() << " initial TCK ( " << m_initialTCK << " -> " << initialID << " ) not amongst entries in prefetch directory "  << m_prefetchDir << endmsg;
    return StatusCode::FAILURE;
  }

  // get pointer into initTop->alias(), to the end of TOPLEVEL/+prefetch, to pick up which type this is...
  std::string::size_type pos = initTop->alias().str().find('/', std::string("TOPLEVEL/"+m_prefetchDir+'/').size() );
  if (pos == std::string::npos) {
    error() << " could not determine type from " << initTop->alias() << " with prefetchdir = " << m_prefetchDir << endmsg;
    return StatusCode::FAILURE;
  }
  ++pos;
  // and fetch them, and preload their configurations...
  info() << " prefetching  from " << initTop->alias().str().substr(0,pos) << endmsg;
  std::vector<ConfigTreeNodeAlias> sameTypes = cas()->configTreeNodeAliases(ConfigTreeNodeAlias::alias_type( initTop->alias().str().substr(0,pos) ));

  for (std::vector<ConfigTreeNodeAlias>::const_iterator i = sameTypes.begin(); i!=sameTypes.end(); ++i ) {
     debug() << " considering " << i->alias().str() << endmsg; 
     if ( i->alias().str().substr(0,pos) != initTop->alias().str().substr(0,pos) ) continue;
     debug() << " loading config " << i->alias().str() << " -> " << i->ref() << endmsg; 
     if ( !loadConfig( i->ref() ) ) {
        error() << " failed to load config " << *i << endmsg; 
        return StatusCode::FAILURE;
     }
  }

  // configure everyone from the a-priori specified TCK
        
  status = configure( initialID, false );
  if (status.isSuccess()) m_configuredTCK = TCKrep( m_initialTCK );
  return status;
}


//=============================================================================
// Perform mapping from TCK to onfiguration ID
//=============================================================================
ConfigTreeNode::digest_type
HltConfigSvc::tck2id(TCKrep tck) const {
    // if upper bit set, ignore L0... in that case, the L0 TCK is generated by PVSS
    // and not by us, and hence we only know it with the L0 part blanked out...
    if ( m_maskL0TCK || (tck.uint() & 0x80000000)!=0 ) {
        tck &= 0xFFFF0000;
        debug() << " masked L0 part of TCK -- now have " << tck << endmsg;
    }
    ConfigTreeNode::digest_type id = ConfigTreeNode::digest_type::createInvalid();
    TCKMap_t::const_iterator i = m_tck2config.find( tck );
    if (i != m_tck2config.end()) {
        ConfigTreeNode::digest_type id = ConfigTreeNode::digest_type::createFromStringRep(i->second);
        debug() << " TCK " << tck << " mapped (by explicit option) to " << id << endmsg;
        return id;
    }

    // NOTE: we need to access to the IConfigAccessSvc of the parent to make
    // sure we are consistent...

    i = m_tck2configCache.find( tck );
    if ( i!=m_tck2configCache.end() )  {
        id = ConfigTreeNode::digest_type::createFromStringRep(i->second);
    } else {
        ConfigTreeNodeAlias::alias_type alias( std::string("TCK/") +  tck.str()  );
        boost::optional<ConfigTreeNode> n = cas()->readConfigTreeNodeAlias( alias );
        if (!n) {
            error() << "Could not resolve TCK " <<  tck  << " : no alias '" << alias << "' found " << endmsg;
            return id;
        }
        id = n->digest(); // need a digest, not an object itself...
        // add to cache...
        m_tck2configCache.insert( make_pair(  tck , id.str() ) );
    }
    debug() << "mapping TCK" <<  tck  << " to configuration ID" << id << endmsg;
    return id;
}

//=============================================================================
// Check TCK on 'beginEvent' incident
//=============================================================================
void HltConfigSvc::dummyCheckOdin() {
  // check if TCK still the same -- if not, reconfigure... 
  TCKrep currentTCK = m_configuredTCK;
  static unsigned nEvent(0);
  debug() << "nEvent: " << nEvent << endmsg;
  if (++nEvent%100==0) { 
      always()   << "\n ********************************************\n"
               << " *********INCREASING TCK !!!!****************\n"
               << " ********************************************\n" << endl;
      ++currentTCK;
  }
  if (m_configuredTCK != currentTCK) {
      info() << "updating configuration from TCK " << m_configuredTCK << " to TCK " << currentTCK << endl;
      StatusCode sc = reconfigure( tck2id(currentTCK) );
      if (sc.isSuccess()) m_configuredTCK = currentTCK;
  }
}

void HltConfigSvc::checkOdin() {

    SmartDataPtr<LHCb::ODIN> odin( m_evtSvc , LHCb::ODINLocation::Default );
    //if (!odin && m_decodeOdinOnDemand ) {
        // TODO: invoke ODINDecodeTool if configured to do so..
    //    m_decodeOdin->execute()
    //}
    if (!odin) {
        error() << " Could not locate ODIN... " << endmsg;
        m_incidentSvc->fireIncident(Incident(name(),IncidentType::AbortEvent));
        return;
    }
    unsigned int TCK = odin->triggerConfigurationKey();

    debug() << "checkOdin: TCK in ODIN bank: " << TCK << endmsg;
    debug() << "checkOdin: currently configured TCK: " << m_configuredTCK << endmsg;

    if ( m_configuredTCK != TCK ) {
        TCKrep TCKr(TCK);
        info() << "requesting configuration update from TCK " 
               << m_configuredTCK 
               << " to TCK " << TCKr << endmsg;
        if (reconfigure( tck2id(TCKr) ).isSuccess()) { 
            m_configuredTCK = TCKr;
        } else {
            error()   << "\n\n\n\n\n"
                      << "            ****************************************\n"
                      << "            ****************************************\n"
                      << "            ****************************************\n"
                      << "            ********                        ********\n"
                      << "            ********   RECONFIGURE FAILED   ********\n"
                      << "            ********                        ********\n"
                      << "            ****************************************\n"
                      << "            ****************************************\n"
                      << "            ****************************************\n"
                      << "\n\n\n\n\n"
                      << endmsg;
            m_incidentSvc->fireIncident(Incident(name(),IncidentType::AbortEvent));
            return;
        }
    }

    //TODO: put HltDecReports into event with current TCK...
    std::auto_ptr<LHCb::HltDecReports> hdr( new LHCb::HltDecReports() );
    hdr->setConfiguredTCK(m_configuredTCK.uint());
    // hdr->setTaskNumber(...);
    m_evtSvc->registerObject(m_outputContainerName,hdr.release());
}

void HltConfigSvc::handle(const Incident& /*incident*/) {
     //dummyCheckOdin();
    checkOdin();
}
