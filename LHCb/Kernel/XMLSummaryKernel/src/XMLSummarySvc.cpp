#include "XMLSummaryKernel/XMLSummarySvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/SvcFactory.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiUtils/RegEx.h"
#include "GaudiUtils/IIODataManager.h"
//#include "GaudiUtils/IODataManager.h"
//With regex libraries

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DECLARE_SERVICE_FACTORY(XMLSummarySvc)

using namespace std;

namespace {
  /// @FIXME: (MCl) work-around to fix the warning
  ///
  ///   warning: deprecated conversion from string constant to ����char*����
  ///
  /// that occurs when a string constant (e.g.: "abc", "") is passed to a function
  /// expecting char *
  inline char *chr(const char*c){ return const_cast<char*>(c); }
}

//
///////////////////////////////////////////////////////////////////////////
//

XMLSummarySvc::XMLSummarySvc(const std::string& name, ISvcLocator* svc )
  : base_class( name, svc ),
    m_incSvc(0),
    m_ioDataManager(0),
    m_counterList(0),
    m_statEntityList(0),
    m_xmlfile(""),
    m_xsdfile(""),
    m_summary(0),
    m_handled(),
    m_addedCounters(0),
    m_filename(""),
    m_fidMap()
{

  //declare the filename to write. If the string is empty, no file will be written
  declareProperty("xmlfile",m_xmlfile="summary.xml");
  //declare the schema to read.
  declareProperty("xsdfile",m_xsdfile="$XMLSUMMARYBASEROOT/xml/XMLSummary.xsd");
  //declare the list of simple counters to write. 
  //These counters must be passed, "declared", by their algorithms. RegularExpressions are allowed
  declareProperty("CounterList",
                  m_counterList=std::vector<std::string>(1,".*"));
  //declare the list of stat entities to write. 
  //These counters must be passed, "declared", by their algorithms. RegularExpressions are allowed
  declareProperty("StatEntityList",
                  m_statEntityList=std::vector<std::string>(1,".*"));
  
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

XMLSummarySvc::~XMLSummarySvc()
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


StatusCode
XMLSummarySvc::initialize()
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
  
  //get the python object, then initialize it
  sc=prepareXML();
  if(!sc.isSuccess()) return sc;
  
  PyObject_CallMethod(m_summary, chr("set_step"), chr("s"), chr("initialize"));

  //temp
  m_filename="PFN:/path/filename.dst";
  //todo fill the list of file/GUIDs from somewhere
  std::vector<std::string> filenames(0);
  //try this
  std::vector<Gaudi::IDataConnection*> connections=m_ioDataManager->connections(NULL);
  for(std::vector<Gaudi::IDataConnection*>::const_iterator it=connections.begin();
      it!=connections.end(); it++)
  {
    if((*it) == NULL) continue;
    if((*it)->pfn() != "") filenames.push_back((*it)->pfn());
  }
  

  //fill the initial list of filenames
  for(std::vector<std::string>::const_iterator i=filenames.begin();
      i!=filenames.end();
      i++)
  {
    PyObject_CallMethod(m_summary,
			chr("fill_input"),
			chr("s"),
			chr(i->c_str())
			);
  }
  //output the file, and write to the stdout
  writeXML(MSG::INFO).ignore();
  printXML(MSG::DEBUG).ignore();


  log << MSG::DEBUG << "initialized successfully" << endmsg;
  return StatusCode::SUCCESS;

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

StatusCode
XMLSummarySvc::finalize()
{

  MsgStream log( msgSvc(), name() );

  log << MSG::DEBUG << "finalize" << endmsg;

  //add own counter
  addCounter(name(),"handled",m_handled);

  //write collected counters
  PyObject_CallMethod(m_summary, chr("set_step"), chr("s,i"), chr("finalize"), 1);
  fillcounters().ignore();
  writeXML(MSG::INFO).ignore();
  printXML(MSG::DEBUG).ignore();
	
  return Service::finalize();

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void XMLSummarySvc::addCounter(
			       const std::string alg,
			       const std::string name,
			       const StatEntity & count,
			       const Gaudi::CounterSummary::SaveType saveType)
{
  std::string full_name=alg+"/"+name;
  NameStatPair c(full_name,count);
  NameStatTypePair ct(c,saveType);
  m_addedCounters.push_back(ct);
  
  return;
}

void XMLSummarySvc::addCounter(
			       const std::string alg,
			       const std::string name,
			       const Stat & count,
			       const Gaudi::CounterSummary::SaveType saveType)
{
  StatEntity newEnt=*(count.entity());
  return addCounter(alg,name,newEnt,saveType);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


void XMLSummarySvc::handle( const Incident& incident )
{
  MsgStream log( msgSvc(), name() );
  if(incident.type()!=IncidentType::EndEvent) 
    log << MSG::VERBOSE << incident.type() << ":" << incident.source() << endmsg;
  //std::string GUID="";m_GUID;
  std::string filename=m_filename;
  std::string method="fill_input";
  std::string status="";
  int addevents=0;
  
  if(incident.type()==IncidentType::BeginInputFile)
    {
      status="part";
      //update current filename and GUID
      filename=m_filename=incident.source();
    }
  else if(incident.type()==IncidentType::BeginOutputFile)
    {
      status="none";
      method="fill_output";
      //update current filename and GUID
      filename=incident.source();//TODO ..something;
    }
  else if(incident.type()==IncidentType::EndInputFile)
    {
      status="full";
    }
  else if(incident.type()==IncidentType::EndOutputFile)
    {
      status="full";
      method="fill_output";
      filename=incident.source();//TODO ..something;
    }
  else if(incident.type()==IncidentType::FailInputFile)
    {
      status="fail";
      filename=m_filename=incident.source();//TODO ..something;  

    }
  else if(incident.type()==IncidentType::FailedOutputFile)
    {
      status="fail";
      method="fill_output";
      filename=incident.source();//TODO ..something; 
    }
  else if(incident.type()==IncidentType::EndEvent)
    {
      status="part";
      addevents=1;
    }
  else if(incident.type()==IncidentType::WroteToOutputFile)
    {
      status="part";
      method="fill_output";
      addevents=1;
      filename=incident.source();//TODO ..something;
    }
  //actually add to the summary
  std::string GUID=file2GUID(filename);
  if(incident.type()!=IncidentType::EndEvent) 
    log << MSG::VERBOSE << method <<"(" << filename << "," << GUID << "," << status << "," << addevents << ")" << endmsg;
  PyObject_CallMethod(m_summary,
		      chr(method.c_str()),
		      chr("s,s,s,d"),
		      chr(filename.c_str()),
		      chr(GUID.c_str()),
		      chr(status.c_str()),
		      double(addevents));
  m_handled++;

  if ( incident.type()==IncidentType::EndInputFile
       || incident.type()==IncidentType::FailInputFile )
    {
      PyObject_CallMethod(m_summary, chr("set_step"), chr("s"), chr("execute"));
      writeXML();
      printXML();
    }

  return;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

StatusCode XMLSummarySvc::fillcounters()
{

  //fill all counters, in correct order
  Gaudi::CounterSummary::SaveType saveType=Gaudi::CounterSummary::SaveSimpleCounter;
  while(true)
    {
      for (NameStatList::const_iterator i=m_addedCounters.begin();
	   i!=m_addedCounters.end(); i++)
	{
	  if(saveType!=i->second) continue;
	  fillcounter(*i).ignore();
	}
      
      if(saveType==Gaudi::CounterSummary::SaveAlwaysStatEntity) break;
      saveType=Gaudi::CounterSummary::SaveType(saveType+1);
    }
  
  return StatusCode::SUCCESS;
}

StatusCode XMLSummarySvc::fillcounter(const NameStatTypePair & count)
{
  std::string type_name="fill_counter";
  bool stat_ent=false;
  bool check=false;
  switch (count.second)
    {
    case Gaudi::CounterSummary::SaveSimpleCounter:
      type_name="fill_counter"; 
      check=Gaudi::Utils::RegEx::matchOr(count.first.first,m_counterList);
      break;
    case Gaudi::CounterSummary::SaveStatEntity:
      type_name="fill_counter"; 
      check=Gaudi::Utils::RegEx::matchOr(count.first.first,m_statEntityList);
      stat_ent=true; 
      break;
    case Gaudi::CounterSummary::SaveAlwaysSimpleCounter:
      type_name="fill_lumi"; 
      check=true;
      break;
    case Gaudi::CounterSummary::SaveAlwaysStatEntity:
      type_name="fill_lumi"; 
      stat_ent=true; 
      check=true;
      break;
    default:
      check=false;
      break;
    }
  //not supposed to write this one out
  if(!check) return StatusCode::SUCCESS;
  if(stat_ent)
    {
      PyObject_CallMethod(m_summary, chr(type_name.c_str()),
			  chr("s,d,d,d,d,d"),
			  chr(count.first.first.c_str()),
			  double(count.first.second.flag()),
			  double(count.first.second.nEntries()),
			  double(count.first.second.flag2()),
			  double(count.first.second.flagMin()),
			  double(count.first.second.flagMax())
			  );
      return StatusCode::SUCCESS;
    }
  PyObject_CallMethod(m_summary, chr(type_name.c_str()),
		      chr("s,d"),
		      chr(count.first.first.c_str()),
		      double(count.first.second.flag())
		      );
	
  return StatusCode::SUCCESS;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

StatusCode XMLSummarySvc::writeXML(MSG::Level lev)
{
  MsgStream log( msgSvc(), name() );
  if (m_xmlfile=="")
  {
    log << lev << "no xml file to be written " << m_xmlfile << endmsg;
    return StatusCode::SUCCESS;
  }
  
  PyObject_CallMethod(m_summary, chr("write"), chr("s"),chr(m_xmlfile.c_str()));
  log << lev << "Wrote xml file " << m_xmlfile << endmsg;
  return StatusCode::SUCCESS;
}
StatusCode XMLSummarySvc::printXML(MSG::Level lev)
{
  MsgStream log( msgSvc(), name() );
  log << lev << PyString_AsString(PyObject_CallMethod(m_summary, chr("xml"), chr(""))) << endmsg;
  return StatusCode::SUCCESS;
}

StatusCode XMLSummarySvc::prepareIncSvc()
{
  StatusCode sc=service("IncidentSvc", m_incSvc, false);
  if(!sc.isSuccess() || m_incSvc== NULL) return StatusCode::FAILURE;

  m_incSvc->addListener( this, IncidentType::BeginInputFile);
  m_incSvc->addListener( this, IncidentType::BeginOutputFile);
  m_incSvc->addListener( this, IncidentType::EndEvent);
  m_incSvc->addListener( this, IncidentType::WroteToOutputFile);
  m_incSvc->addListener( this, IncidentType::FailedOutputFile);
  m_incSvc->addListener( this, IncidentType::FailInputFile);
  m_incSvc->addListener( this, IncidentType::EndOutputFile);
  m_incSvc->addListener( this, IncidentType::EndInputFile);
  
  MsgStream log( msgSvc(), name() );
  log << MSG::DEBUG << "registered with incSvc" << endmsg;
  
  return StatusCode::SUCCESS;
}

StatusCode XMLSummarySvc::prepareXML()
{

  MsgStream log( msgSvc(), name() );
  
  //Initialize the python session... may not be needed?
  Py_Initialize();
  
  //Import the python module
  //import XMLSummaryBase.summary
  PyObject* pName=PyString_FromString("XMLSummaryBase.summary");
  m_summary = PyImport_Import(pName);
  if (m_summary==NULL) return StatusCode::FAILURE;
  
  //Get the summary class object from the module
  //XMLSummaryBase.summary.Summary
  m_summary = PyObject_GetAttrString(m_summary, "Summary");
  if (m_summary==NULL) return StatusCode::FAILURE;
  
  //Call the class to make an instance
  //XMLSummaryBase.summary.Summary(m_xsdfile)
  PyObject* schema=PyString_FromString(chr(m_xsdfile.c_str()));
  m_summary=PyObject_CallFunctionObjArgs(m_summary, schema, NULL);
  if (m_summary==NULL) return StatusCode::FAILURE;
  
  log << MSG::DEBUG << "xml summary object created" << endmsg;
  return StatusCode::SUCCESS;
}
std::string XMLSummarySvc::file2GUID(const std::string & filename)
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
std::string XMLSummarySvc::AFN2name(const std::string & filename)
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

