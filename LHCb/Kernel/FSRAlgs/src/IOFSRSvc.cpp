#include "IOFSRSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/IAppMgrUI.h"
#include "GaudiKernel/SvcFactory.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiUtils/RegEx.h"
#include "GaudiUtils/IIODataManager.h"
//#include "GaudiUtils/IODataManager.h"
//With regex libraries

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLARE_SERVICE_FACTORY(IOFSRSvc)

using namespace std;

namespace {
  /// @FIXME: (MCl) work-around to fix the warning
  ///
  ///   warning: deprecated conversion from string constant to ‘char*’
  ///
  /// that occurs when a string constant (e.g.: "abc", "") is passed to a function
  /// expecting char *
  inline char *chr(const char*c){ return const_cast<char*>(c); }
}

//
///////////////////////////////////////////////////////////////////////////
// Constructor/Destructor
///////////////////////////////////////////////////////////////////////////

IOFSRSvc::IOFSRSvc(const std::string& name, ISvcLocator* svc )
  : base_class( name, svc ),
    m_incSvc(0),
    m_ioDataManager(0),
    m_handled(),
    m_filename(""),
    m_hasinput(false),
    m_seenByMap(),
    m_readFromMap(),
    m_writtenToMap(),
    m_jobOutputMap(),
    m_jobInputMap(),
    m_provenanceMap(),
    m_fidMap(),
    m_eventCount(0),
    m_navigatorTool(0),
    m_fileRecordSvc(0),
    m_trace(LHCb::IOFSR::UNCHECKED),
    m_merged(false)
    
{
  
  declareProperty("BeginEventIncident", m_beginIncident = IncidentType::BeginEvent);
  declareProperty("EndEventIncident", m_endIncident = IncidentType::EndEvent);
  declareProperty( "FileRecordLocation" , m_FileRecordName    = "/FileRecords"   );
  declareProperty( "FSRName"            , m_FSRName           = "/IOFSR"       );
  declareProperty("OverrideStatus", m_overrideStatus=false);
  //The status to start with if nothing else is known
  declareProperty("DefaultStatus",m_defaultStatusStr="UNCHECKED");
  
  //declareProperty( "NavigatorToolName"  , m_ToolName          = "FSRNavigator"   );
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

IOFSRSvc::~IOFSRSvc()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


///////////////////////////////////////////////////////////////////////////
// Gaudi Parts
///////////////////////////////////////////////////////////////////////////

StatusCode IOFSRSvc::initialize()
{

  StatusCode sc = Service::initialize();
  if (!sc.isSuccess()) return sc;

  MsgStream log( msgSvc(), name() );
  log << MSG::DEBUG << "initialize" << endmsg;

  //prepare the incident service
  sc = prepareIncSvc(); 
  if (!sc.isSuccess()) log << MSG::WARNING << "could not init incSvc" << endmsg;
  
  //prepare the IODataManager service
  sc=service("IODataManager", m_ioDataManager);//, false);//,false);
  if(!sc.isSuccess() || m_ioDataManager== NULL) return StatusCode::FAILURE;
  
  //temp
  m_filename="PFN:/path/filename.dst";
  
  // get the File Records service
  SmartIF<IDataProviderSvc> fsrSvc(serviceLocator()->service<IDataProviderSvc>("FileRecordDataSvc"));
  if (fsrSvc) m_fileRecordSvc=fsrSvc;
  else return StatusCode::FAILURE;
  
  //sc=service("FileRecordDataSvc", m_fileRecordSvc);//, false);//,false);
  //if(m_fileRecordSvc== NULL) return StatusCode::FAILURE;
  //m_fileRecordSvc = svc<IDataProviderSvc>("FileRecordDataSvc", true);
  
  // prepare navigator tool
  SmartIF<IToolSvc> toolSvc(serviceLocator()->service<IToolSvc>("ToolSvc"));
  if (!toolSvc) return StatusCode::FAILURE;
  
  //const IInterface* TestInterface =0;
  
  
  sc=toolSvc->retrieveTool("FSRNavigator" , m_navigatorTool, this);//m_ToolName
  //if(!sc.isSuccess() || TestInterface== NULL) return StatusCode::FAILURE;
  //m_navigatorTool = (const IFSRNavigator *)TestInterface;
  
  if(!sc.isSuccess() || m_navigatorTool== NULL) return StatusCode::FAILURE;
  //m_navigatorTool = tool<IFSRNavigator>( "FSRNavigator" );
  
  m_defaultStatus=LHCb::IOFSR::StatusFlagToType(m_defaultStatusStr);
  
  log << MSG::DEBUG << "initialized successfully" << endmsg;
  return StatusCode::SUCCESS;

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

StatusCode IOFSRSvc::finalize()
{

  MsgStream log( msgSvc(), name() );

  log << MSG::DEBUG << "finalize" << endmsg;
  log << MSG::DEBUG << "handled " << m_handled << " incidents" << endmsg;
  
  SmartIF<IToolSvc> toolSvc(serviceLocator()->service<IToolSvc>("ToolSvc"));
  if (!toolSvc) return StatusCode::FAILURE;
  
  StatusCode sc=toolSvc->releaseTool(m_navigatorTool);//m_ToolName
  if(!sc.isSuccess()) return StatusCode::FAILURE;
  m_navigatorTool=0;
  
  
  return Service::finalize();

}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
StatusCode IOFSRSvc::stop()
{
  MsgStream log( msgSvc(), name() );
  log << MSG::DEBUG << "STOPPED" << endmsg;
  return Service::stop();
  
}




///////////////////////////////////////////////////////////////////////////
// Incident Listening
///////////////////////////////////////////////////////////////////////////

StatusCode IOFSRSvc::prepareIncSvc()
{
  SmartIF<IIncidentSvc> incSvc(serviceLocator()->service<IIncidentSvc>("IncidentSvc"));
  if (!incSvc) return StatusCode::FAILURE;
  m_incSvc=incSvc;//explicit cast.
  
  //StatusCode sc=service("IncidentSvc", m_incSvc, false);
  //if(!sc.isSuccess() || m_incSvc== NULL) return StatusCode::FAILURE;

  m_incSvc->addListener( this, m_endIncident);
  //m_incSvc->addListener( this, m_beginIncident);
  
  //check extended file incidents are defined
  m_incSvc->addListener( this, IncidentType::BeginInputFile);
  m_incSvc->addListener( this, IncidentType::BeginOutputFile);
  m_incSvc->addListener( this, IncidentType::WroteToOutputFile);
  //m_incSvc->addListener( this, IncidentType::FailOutputFile);
  //m_incSvc->addListener( this, IncidentType::FailInputFile);
  //failures not important here, since they will either be read or not
  //m_incSvc->addListener( this, IncidentType::EndOutputFile);
  //IOFSR doesn't need to know if the file was fully read or not at this stage
  //m_incSvc->addListener( this, IncidentType::EndInputFile);
  
  MsgStream log( msgSvc(), name() );
  log << MSG::DEBUG << "registered with incSvc" << endmsg;
  
  return StatusCode::SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void IOFSRSvc::handle( const Incident& incident )
{
  
  MsgStream log( msgSvc(), name() );
  
  if(incident.type()!=m_endIncident && incident.type()!=m_beginIncident) 
    log << MSG::VERBOSE << incident.type() << ":" << incident.source() << endmsg;
  //elif(incident.source()=="EventLoopMgr")
    
  //std::string GUID="";m_GUID;
  std::string filename=m_filename;
  
  if(incident.type()==IncidentType::BeginInputFile)
    {
      //update current filename and GUID
      m_filename=AFN2name(incident.source());
      m_hasinput=true;
      //prompt filling of ID maps
      file2GUID(incident.source());
      
    }
  else if(incident.type()==IncidentType::BeginOutputFile)
    {
      filename=incident.source();
      //prompt filling of ID maps
      file2GUID(incident.source());
    }
  else if(incident.type()==m_endIncident)
    {
      m_eventCount++;
      if (m_hasinput)
      {
        m_jobInputMap[m_filename]++;
        //prompt filling of ID maps
        file2GUID(m_filename);
      }
      
    }
  else if(incident.type()==IncidentType::WroteToOutputFile)
    {
      filename=incident.source();
      m_jobOutputMap[filename]++;
      //prompt filling of ID maps
      file2GUID(incident.source());
    }
  
  
  m_handled++;
  
  
  return;

  
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

std::string IOFSRSvc::file2GUID(const std::string & filename)
{
  MsgStream log( msgSvc(), name() );
  
  FidMap::iterator j = m_fidMap.find(filename);
  if (j != m_fidMap.end() ) return j->second;
  std::string shortname=AFN2name(filename);
  j = m_fidMap.find(shortname);
  if (j != m_fidMap.end() ) return j->second;
  
  const Gaudi::IDataConnection * con=m_ioDataManager->connection(shortname);
  if(!con) con=m_ioDataManager->connection(shortname);
  if(!con) return "";
  m_fidMap[filename]=con->fid();
  m_fidMap[shortname]=con->fid();
  return con->fid();
  //return "";
  
}
std::string IOFSRSvc::AFN2name(const std::string & filename) const
{
  std::string me="LFN:";
  size_t pos=filename.find(me);
  if ((pos==0) && (pos+me.size()<filename.size()))
    return filename.substr(pos+me.size());
  me="PFN:";
  pos=filename.find(me);
  if ((pos==0) && (pos+me.size()<filename.size()))
    return filename.substr(pos+me.size());
  return filename;

}


///////////////////////////////////////////////////////////////////////////
// Accessors for IO information
///////////////////////////////////////////////////////////////////////////

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::vector< std::string > IOFSRSvc::parents()
{
  std::vector< std::string > retVec(0);
  for (LHCb::IOFSR::FileEventMap::iterator af=m_jobInputMap.begin(); af!=m_jobInputMap.end(); ++af)
  {
    retVec.push_back(file2GUID(af->first));
  }
  return retVec;
  
}

// Return list of GUIDs as input for this job, and events read from them
LHCb::IOFSR::FileEventMap IOFSRSvc::jobInputMap()
{
  LHCb::IOFSR::FileEventMap retMap;
  for (LHCb::IOFSR::FileEventMap::const_iterator af=m_jobInputMap.begin(); af!=m_jobInputMap.end(); ++af)
  {
    retMap[file2GUID(af->first)]=af->second;
  }
  return retMap;
  
}


// Return list of GUIDs as input for this job, and events read from them
ulonglong IOFSRSvc::jobOutput(const std::string & PFN)
{
  LHCb::IOFSR::FileEventMap::iterator j = m_jobOutputMap.find(AFN2name(PFN));
  if (j != m_jobOutputMap.end() ) return j->second;
  return 0;
  
}

// Return map of input GUID : #events for all GUIDs, events read __from__ those files
LHCb::IOFSR::FileEventMap IOFSRSvc::eventsReadFrom()
{
  //add in the daughter files read so far
  for (LHCb::IOFSR::FileEventMap::const_iterator af=m_jobInputMap.begin(); af!=m_jobInputMap.end(); ++af)
  {
    m_readFromMap[file2GUID(af->first)]=af->second;
  }
  return m_readFromMap;
  
}

///////////////////////////////////////////////////////////////////////////
// Manipulators of existing FSRs
///////////////////////////////////////////////////////////////////////////

  //merge existing IOFSRs into the maps of this service
StatusCode IOFSRSvc::mergeIOFSRs()
{
  if (m_merged) return StatusCode::SUCCESS;
  
  MsgStream log( msgSvc(), name() );
  log << MSG::DEBUG << "Merging IOFSRs" << endmsg;
  
  // make an inventory of the FileRecord store
  std::string fileRecordRoot = m_FileRecordName; 
  std::vector< std::string > addresses = m_navigatorTool->navigate(fileRecordRoot, m_FSRName);
  
  std::map<std::string, LHCb::IOFSR* > ioFSRs;
  
  log << MSG::DEBUG << addresses.size() << " IOFSRs found" << endmsg;
  log << MSG::DEBUG << "My Maps Before Merging: prov " << m_provenanceMap.size() 
      << ", seen " << m_seenByMap.size() << ", read " << m_readFromMap.size() 
      << ", write " << m_writtenToMap.size() << endmsg;
  
  // print
  if ( msgLevel(MSG::DEBUG) ) 
  {
    for(std::vector< std::string >::iterator iAddr = addresses.begin() ; iAddr != addresses.end() ; ++iAddr )
    {
      log << MSG::DEBUG << "address: " << (*iAddr) << endmsg;
    }
  }
  
  //get

  for(std::vector< std::string >::iterator a = addresses.begin() ; a!= addresses.end() ; ++a )
  {  
    
    std::string fileRecordAddress = (*a);   
    SmartDataPtr<LHCb::IOFSR> ioFSR(m_fileRecordSvc, fileRecordAddress);
    
    //if ( !exist<LHCb::IOFSR>(m_fileRecordSvc, fileRecordAddress) ) //can't find them!!
    if (0 != ioFSR)
    {
      LHCb::IOFSR * ioFSRCast=ioFSR;
      
      ioFSRs[fileRecordAddress]=ioFSRCast;
    } 
    else 
    {
      log << MSG::ERROR << fileRecordAddress << " not found" << endmsg ;
      
    }
  }
  
  // insert their provenance maps, writtenTo, seenBy, readFrom maps
  std::map<std::string, LHCb::IOFSR * >::iterator iofsr;    
  for ( iofsr = ioFSRs.begin(); iofsr != ioFSRs.end(); ++iofsr ) 
  {
    //debug() << *(*tsfsr) << endmsg; 
    LHCb::IOFSR::ProvenanceMap prov=iofsr->second->provenance();
    LHCb::IOFSR::FileEventMap seen=iofsr->second->eventsSeenBy();
    LHCb::IOFSR::FileEventMap read=iofsr->second->eventsReadFrom();
    LHCb::IOFSR::FileEventMap write=iofsr->second->eventsWrittenTo();
    
    //TODO, check for duplication here...
    
    for( LHCb::IOFSR::ProvenanceMap::const_iterator it=prov.begin();it!=prov.end();++it)
    {
      m_provenanceMap[it->first]=it->second;
    }
    for( LHCb::IOFSR::FileEventMap::const_iterator it=seen.begin();it!=seen.end();++it)
    {
      m_seenByMap[it->first]=it->second;
    }
    for( LHCb::IOFSR::FileEventMap::const_iterator it=read.begin();it!=read.end();++it)
    {
      m_readFromMap[it->first]=it->second;
    }
    for( LHCb::IOFSR::FileEventMap::const_iterator it=write.begin();it!=write.end();++it)
    {
      m_writtenToMap[it->first]=it->second;
    }
    
  }
      
  log << MSG::DEBUG << "My Maps Before Leading Info: prov " << m_provenanceMap.size() 
      << ", seen " << m_seenByMap.size() << ", read " << m_readFromMap.size() 
      << ", write " << m_writtenToMap.size() << endmsg;
  
  // insert their "leading" information
  for ( iofsr = ioFSRs.begin(); iofsr != ioFSRs.end(); ++iofsr ) 
  {
    std::string GUID=address2GUID(iofsr->first);
    m_seenByMap[GUID]=iofsr->second->eventsSeen();
    m_writtenToMap[GUID]=iofsr->second->eventsOutput();
  }
    
  log << MSG::DEBUG << "My Maps After Leading Info: prov " << m_provenanceMap.size() 
      << ", seen " << m_seenByMap.size() << ", read " << m_readFromMap.size() 
      << ", write " << m_writtenToMap.size() << endmsg;
  //check if any traces have failed
  if (m_trace==LHCb::IOFSR::UNCHECKED)
  {
    
    for ( iofsr = ioFSRs.begin(); iofsr != ioFSRs.end(); ++iofsr ) 
    {
      if (iofsr->second->statusFlag() != LHCb::IOFSR::UNCHECKED && iofsr->second->statusFlag() != LHCb::IOFSR::VERIFIED)
      {
        m_trace=iofsr->second->statusFlag();
        break;
      }
      
    }
    
  }
  
  
  // clean up original FSRs
  for(std::vector< std::string >::iterator a = addresses.begin() ; a!= addresses.end() ; ++a )
  {
    // get FSR as keyed object and cleanup the original ones - this only cleans lumiFSRs
    m_fileRecordSvc->unlinkObject( *a ).ignore();         // get it out of TES
  }
  
  m_merged=true;
  
  
  return StatusCode::SUCCESS;
}

//find the last GUID in the object address
std::string IOFSRSvc::address2GUID(const std::string& address)
{
  size_t foundEnd=address.find_last_of("/");
  size_t foundStart=address.find_last_of("/",foundEnd);
  return address.substr(foundStart+1,foundEnd-1);
  
  
}

//determine event count reliability, following down the tree 
//of input/output until something doesn't match
LHCb::IOFSR::StatusFlag IOFSRSvc::traceCounts()
{
  if (m_overrideStatus) return m_defaultStatus;
  
  MsgStream log( msgSvc(), name() );
  log << MSG::DEBUG << "Tracing IOFSRs" << endmsg;
  
  if (!m_merged)
  {
    log << MSG::ERROR << "You did not merge the FSRs yet, so the tracing cannot be done, call merge before trace" << endmsg;
    return LHCb::IOFSR::ERROR;
  }
  
  if (m_trace!=LHCb::IOFSR::UNCHECKED) return m_trace;
  //check that all files have been fully read
  for(LHCb::IOFSR::FileEventMap::iterator it=m_writtenToMap.begin(); it!=m_writtenToMap.end(); ++it)
  {
    //check if FSR for that was found ...
    LHCb::IOFSR::FileEventMap::iterator j = m_readFromMap.find(it->first);
    //perhaps the FSR never existed
    if (j == m_readFromMap.end() ) m_trace=LHCb::IOFSR::ERROR;
    else if (it->second == j->second) continue;
    else if (it->second > j->second) m_trace=LHCb::IOFSR::EVENTSKIPPED;
    else if (it->second < j->second) m_trace=LHCb::IOFSR::EVENTDOUBLED;
    break;
  }
  
  if (m_trace!=LHCb::IOFSR::UNCHECKED) return m_trace;
  
  log << MSG::DEBUG << "Tracing provenance, OK" << endmsg;
  //check that the seen counts are the sum of the daughter counts right now.
  //Will be done at every level, so I only need to check this top level.
  
  ulonglong expected=0;
  
  for (std::vector< std::string >::iterator it=parents().begin(); it!=parents().end();++it)
  {
    //check if FSR for that was found ...
    LHCb::IOFSR::FileEventMap::iterator j = m_writtenToMap.find(*it);
    if (j == m_writtenToMap.end() ) 
    {
      m_trace=LHCb::IOFSR::ERROR;
      break;
    }
    
    else expected+=j->second;
  }
  
  if (m_trace!=LHCb::IOFSR::UNCHECKED) return m_trace;
  log << MSG::DEBUG << "Traced parents, OK" << endmsg;
  
  if (expected==m_eventCount) m_trace=LHCb::IOFSR::VERIFIED;
  else if (expected > m_eventCount) m_trace=LHCb::IOFSR::EVENTSKIPPED;
  else if (expected < m_eventCount) m_trace=LHCb::IOFSR::EVENTDOUBLED;
  
  
  log << MSG::DEBUG << "Final result " << m_trace << endmsg;
  
  return m_trace;
  
}

//remove any IOFSR at the top level. To be called before requesting a new FSR.
StatusCode IOFSRSvc::cleanTopFSR()
{
  return m_fileRecordSvc->unlinkObject( LHCb::IOFSRLocation::Default);
  
}

///////////////////////////////////////////////////////////////////////////
// Create the IOFSR, and pass back to the requestor
///////////////////////////////////////////////////////////////////////////

//Create a new IOFSR, pass it back so the requesting alg can store as needed
LHCb::IOFSR* IOFSRSvc::buildIOFSR(const std::string & outputName)
{
  mergeIOFSRs().ignore();
  
  LHCb::IOFSR* ioFSR = new LHCb::IOFSR;
  ioFSR->setEventsOutput(jobOutput(outputName));
  ioFSR->setEventsSeen(eventsSeen());
  ioFSR->setFilesRead(parents());
  ioFSR->setProvenance(provenanceMap());
  ioFSR->setEventsWrittenTo(eventsWrittenTo());
  ioFSR->setEventsReadFrom(eventsReadFrom());
  ioFSR->setEventsSeenBy(eventsSeenBy());
  ioFSR->setStatusFlag(traceCounts());
  
  return ioFSR;
  
}
