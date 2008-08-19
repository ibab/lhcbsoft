// $Id: AnalysisTask.cpp,v 1.4 2008-08-19 22:45:32 ggiacomo Exp $


// from Gaudi
#include "GaudiKernel/DeclareFactoryEntries.h" 
// local
#include "OMAlib/OMAlib.h"
#include "OMAlib/IOMAMsgTool.h"
#include "OMAlib/AnalysisTask.h"
#include "OMAlib/SavesetFinder.h"

//-----------------------------------------------------------------------------
// Implementation file for class : AnalysisTask
//
// 2008-02-18 : Giacomo Graziani
//-----------------------------------------------------------------------------


AnalysisTask::AnalysisTask( const std::string& name,
                            ISvcLocator* pSvcLocator)
  : OMAlib(NULL),
    GaudiAlgorithm ( name , pSvcLocator ),
    m_inputFiles(0),
    m_inputTasks(0)
{
  declareProperty ( "useHistDB"    , m_useDB = false);
  declareProperty ( "HistDB"       , m_DB = OnlineHistDBEnv_constants::DB );
  declareProperty ( "HistDBuser"   , m_DBuser = OnlineHistDBEnv_constants::ACCOUNT );
  declareProperty ( "HistDBpw"     , m_DBpw = OnlineHistDBEnv_constants::PASSWORD );
  declareProperty ( "HistRefRoot"  , m_myRefRoot = "default");
  declareProperty ( "InputFiles"   , m_inputFiles);
  declareProperty ( "InputTasks"   , m_inputTasks);
  declareProperty ( "MessageTool"  , m_MessageTool = "OMAMsgStdOut" );
}

AnalysisTask::~AnalysisTask() {
  std::vector<SavesetFinder*>::iterator iSS;
  for(iSS = m_saveset.begin() ; iSS != m_saveset.end() ; iSS++) {
    delete (*iSS);
  }
} 


StatusCode AnalysisTask::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); 
  if ( sc.isFailure() ) return sc;  

  // where to send messages 
  m_msgTool = tool<IOMAMsgTool>(m_MessageTool);

  // use HistDB if requested
  if(m_useDB)
    openDBSession( m_DBpw, m_DBuser, m_DB );

  if( "default" != m_myRefRoot) 
    setRefRoot(m_myRefRoot);


  if ( ! m_inputFiles.empty() ) {
    // single shot: analyze these files now 
    std::vector<std::string>::iterator iF;
    for(iF =  m_inputFiles.begin() ; iF != m_inputFiles.end() ; iF++) {
      info() <<"Analyzing histograms on file "<<*iF<<endmsg;
      analyze( *iF , "any" );
    }
  }
  else {
    std::vector<std::string>::iterator iF;
    for(iF =  m_inputTasks.begin() ; iF != m_inputTasks.end() ; iF++) {
      m_saveset.push_back( new SavesetFinder(this, *iF) );
    }
  }
  
  return StatusCode::SUCCESS;
}

StatusCode AnalysisTask::execute() {
  return StatusCode::SUCCESS;
}


StatusCode AnalysisTask::finalize() {
  return GaudiAlgorithm::finalize();  
}


