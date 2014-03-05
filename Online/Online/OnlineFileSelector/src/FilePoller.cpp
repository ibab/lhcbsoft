// $Header:
//  ====================================================================
//  
//  --------------------------------------------------------------------
//
//  Author    : Ioannis Chalkiadakis
//
//  ====================================================================
#include "OnlineFileSelector/FilePoller.h"
#include "GaudiKernel/ISvcLocator.h"  
#include "GaudiKernel/SvcFactory.h"
#include <string>
#include <dirent.h>
#include <stdlib.h>



DECLARE_NAMESPACE_SERVICE_FACTORY(LHCb,FilePoller)

using namespace std;
using namespace LHCb;

FilePoller::FilePoller(const string& nam,ISvcLocator* svc)
                           : OnlineService::OnlineService(nam,svc),
                           m_scanDirectory(),
                           m_alrmTime()
{
  declareProperty("scanDirectory",m_scanDirectory);
  declareProperty("alarmTime",m_alrmTime);
}

FilePoller::~FilePoller() {}


/// Implementation of IInterface::queryInterface()
StatusCode FilePoller::queryInterface(const InterfaceID& riid, void** ppvIF) {
  if(IHandleListenerSvc::interfaceID().versionMatch(riid))
    *ppvIF = (IHandleListenerSvc*)this;
  else if(IAlarmHandler::interfaceID().versionMatch(riid))
    *ppvIF = (IAlarmHandler*)this;
  else
    return OnlineService::queryInterface(riid, ppvIF);
  addRef();
  return StatusCode::SUCCESS;
}



/// Implementation of IOnlineService::initialize()
StatusCode FilePoller::initialize()  {
  StatusCode status = OnlineService::initialize();
  
  if (!lib_rtl_create_lock(0,&m_listenerLock))
    return issueAlarm("Problem creating listener list lock.");

  return status;
}

/// Implementation of IOnlineService::finalize()
StatusCode FilePoller::finalize()  {
  StatusCode status = OnlineService::finalize();

  if (!lib_rtl_delete_lock(m_listenerLock))
    return issueAlarm("Problem deleting listener list lock.");
  
  return status;
}


/// Implementation of IOnlineService::start()
StatusCode FilePoller::start()
{
  StatusCode status = OnlineService::start();
  DimTimer::start(FilePoller::m_alrmTime);
  return status;
}

/// Implementation of IOnlineService::stop()
StatusCode FilePoller::stop()
{
  StatusCode status = OnlineService::stop();
  DimTimer::stop();
  return status;
}

/// Implementation of IAlarmHandler.
const StatusCode FilePoller::issueAlarm(const string& msg) 
{
    error(msg);
    return StatusCode::FAILURE;
}

/// Implementation of polling method.
StatusCode FilePoller::poller(string scan_path)
{
  struct dirent d_entry;                  	
	struct dirent *d_res;
	int i;	
	DIR *dir,*nested_dir;
	char *path,*prev_path;
  StatusCode status;
  string n_path,dname;
  
  path = (char*)calloc(sizeof(char),1024);
  prev_path = (char*)calloc(sizeof(char),1024);
  if ((!path)||(!prev_path)){
    status = FilePoller::issueAlarm(OnlineService::name() + " calloc memory problem");
    return status; 
  }

  path = strncpy(path,scan_path.c_str(),1023);
  dir = opendir(path);
  
  if ( 0 == dir )  {
    return error("The directory "+scan_path+" is not valid!");
  }
  
  while ((0==readdir_r(dir,&d_entry,&d_res)) && (NULL!=d_res)) {
    
    prev_path = strncpy(prev_path,path,1023);
    if (!prev_path)
        return error("The directory prev_path is not valid!");
    if (!strcmp(d_entry.d_name,".") || !strcmp(d_entry.d_name,".."))
        continue;
    
    dname = string(d_entry.d_name);
    n_path = string(path);
    
    if (d_entry.d_type != DT_DIR) {
      
      //Place path of the file in buffer.
      if (find( m_fileNames.begin(),  m_fileNames.end(),n_path) ==  m_fileNames.end()) 
      {
        FilePoller::addTofileNames(n_path + "/" + dname);
      }
      
    }
    else {

      n_path = n_path + "/" + dname;
      nested_dir = opendir(n_path.c_str());
      
		  if (!nested_dir) {
          status = FilePoller::issueAlarm(OnlineService::name()+": Error opening directory.");
          return status;
      }
      status = FilePoller::poller(n_path);
      if (StatusCode::FAILURE == status) {
         status = FilePoller::issueAlarm(OnlineService::name()+": The poller encountered an error while recursing.");
         return status;
      }
      i = closedir(nested_dir);
      if (-1 == i){
         status = FilePoller::issueAlarm(OnlineService::name()+": Problem closing directory.");
        return status;
      }
    }
    
    path = strncpy(path,prev_path,1023);
    if (!path)
        return error("The directory path is not valid!");
  }
  
  return StatusCode::SUCCESS;
}

/// Implementation of IHandleListenerSvc::addListener.
StatusCode FilePoller::addListener(IAlertSvc* Listener) 
{
  
  if (lib_rtl_lock(m_listenerLock)) {
      m_fileListeners.push_back(Listener);
  }
  else
      return FilePoller::issueAlarm("Problem acquiring the lock.");
  if (!lib_rtl_unlock(m_listenerLock))
      return FilePoller::issueAlarm("Problem releasing the lock.");
  return StatusCode::SUCCESS;
}

/// Implementation of IHandleListenerSvc::remListener.
StatusCode FilePoller::remListener(IAlertSvc* Listener)
{
  deque<IAlertSvc*>::iterator iter;
  
  if (lib_rtl_lock(m_listenerLock)) {
      
   if (!m_fileListeners.empty())
   {
     iter = find(m_fileListeners.begin(),m_fileListeners.end(),Listener);
     if (*iter == m_fileListeners.front()) 
         m_fileListeners.pop_front();
     else if (iter == m_fileListeners.end())
         return FilePoller::issueAlarm("Logic bug: remove non-existent listener");
     else
         m_fileListeners.erase(iter);
   }
   else 
     info(OnlineService::name()+": No more listeners available.");
   if (!lib_rtl_unlock(m_listenerLock))
       return FilePoller::issueAlarm("Problem releasing the lock.");
   return StatusCode::SUCCESS;
  }
  return FilePoller::issueAlarm("Problem acquiring the lock.");
}


/// Implementation of IHandleListenerSvc::showListeners.
const StatusCode FilePoller::showListeners()
{
  IAlertSvc* Listener; 
  deque<IAlertSvc*>::iterator iter;
  
  
  if (!m_fileListeners.empty())
  {
   
    for(iter = m_fileListeners.begin(); iter != m_fileListeners.end(); ++iter)
    {
      Listener = *iter;
      if (!Listener)
        return FilePoller::issueAlarm(OnlineService::name()+": Error retrieving service");
        cout << Listener->getSvcName() << endl;
    }
    
    
    return StatusCode::SUCCESS;
  }
  FilePoller::issueAlarm("No listeners at the moment.");
  return StatusCode::FAILURE;
  
}


void FilePoller::addTofileNames(const string& newFile)
{
  m_fileNames.push_back(newFile);
}


string FilePoller::remFromfileNames()
{
  if (!m_fileNames.empty())
  {
    string path_file = m_fileNames.front();
    m_fileNames.pop_front();
    return path_file;
  }
  else 
  {
    issueAlarm("No available files.");
    return "";
  }
}

  
/// Implementation of IHandleListenerSvc::statusReport.
StatusCode FilePoller::statusReport(StatusCode status, string file)
{
  if (StatusCode::FAILURE == status)
  {
    // no book-keeping
    info(OnlineService::name()+": The file has already been processed.");
  }
  else if (StatusCode::SUCCESS == status)
  {
    // book-keep the file
    StatusCode sc = markBookKept(file);
  }
  
  return status;

}


/// Implementation of DimTimer::timerHandler.
void FilePoller::timerHandler()
{
  StatusCode sc;
  string path_name;
  
  DimTimer::stop();
  StatusCode poll_st = FilePoller::poller(FilePoller::m_scanDirectory);
  poll_st = showListeners();

  while (!m_fileListeners.empty() && !m_fileNames.empty()){
    
    
      path_name = remFromfileNames();
      //check if it is book-kept
      sc = isBookKept(path_name);
      //if not, alertSvc(), if yes, dismiss and pick next file
      if (StatusCode::FAILURE == sc)
         sc = ((IAlertSvc*)m_fileListeners.front())->alertSvc(path_name);
      else
         break; //?? CHECK!!
      
      sc = remListener(m_fileListeners.front());

  }
  
  FilePoller::start();

} 



/// Implementation of IOnlineBookkeep::getRunFileNumber.
string FilePoller::getRunFileNumber(const string file_path) {

  return file_path.substr(57); // Is it standard???
  
}

/// Implementation of IOnlineBookkeep::markBookKept.
StatusCode FilePoller::markBookKept(const std::string file) {

  m_ProcessedFiles.push_back(file);
  
  return StatusCode::SUCCESS;
  
}

/// Implementation of IOnlineBookkeep::isBookKept.  
StatusCode FilePoller::isBookKept(const std::string file) {

  vector<string>::iterator iter;

  iter = find(m_ProcessedFiles.begin(),m_ProcessedFiles.end(),file);
  if (iter == m_ProcessedFiles.end())
     return StatusCode::FAILURE;
  
  return StatusCode::SUCCESS;
  
}

  



  
 
