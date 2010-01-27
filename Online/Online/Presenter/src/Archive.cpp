#include <iostream>
#include <algorithm>

#include "Archive.h"
#include "DbRootHist.h"
#include "PresenterMainFrame.h"

#include <TFile.h>
#include <TH1.h>
#include <TList.h>

#include <TString.h>
#include <TPRegexp.h>
#include <TObjString.h>
#include <TObjArray.h>
#include <TSystem.h>
#include <TGMsgBox.h>

using namespace pres;
using namespace std;
using namespace boost::filesystem;
using namespace boost::posix_time;
using namespace boost::gregorian;

Archive::Archive(PresenterMainFrame* gui,
                 const std::string & savesetPath,
                 const std::string & referencePath) :
  m_mainFrame(gui),
  m_savesetPath(path(savesetPath)),
  m_referencePath(path(referencePath)),
  m_verbosity(Silent),
  m_analysisLib(NULL),
  m_msgBoxReturnCode(0)
{
  if (m_verbosity >= Verbose) {
    std::cout << "m_referencePath " << m_referencePath
              << "m_savesetPath " << m_savesetPath << std::endl;
  }
}
Archive::~Archive()
{
}

void Archive::setSavesetPath(const std::string & savesetPath)
{
  m_savesetPath = path(savesetPath);
  if (m_verbosity >= Verbose) {
    std::cout << "Archive m_savesetPath " << m_savesetPath << std::endl;
  }  
}
void Archive::setReferencePath(const std::string & referencePath)
{
  m_referencePath = path(referencePath);
  if (m_verbosity >= Verbose) {
    std::cout << "Archive m_referencePath " << m_referencePath << std::endl;
  }  
}
void Archive::refreshDirectory(const DirectoryType & directoryType,
                               const boost::gregorian::date_period & datePeriod,
                               const std::string & taskName)
{

  switch (directoryType) {
    case Savesets:
      m_foundSavesets = listAvailableRootFiles(m_savesetPath, datePeriod, taskName);
      break;
    case References:
      m_foundReferences = listAvailableRootFiles(m_referencePath, datePeriod, taskName);
      break;
    default:
      break;
  }
}
void Archive::fillHistogram(DbRootHist* histogram,
                            const std::string & timePoint,
                            const std::string & pastDuration)
{
  bool singleSaveset=false;
  if (m_verbosity >= Verbose) {
    std::cout << "Histogram to seek: " << histogram->identifier()
              << " timePoint " << timePoint
              << " pastDuration " << pastDuration << std::endl;
  }
  if ( ! (histogram->isAnaHist()) ) {
    // fill HistogramIdentifier from history mode
    std::string fakeDimName = s_H1D + "/" + histogram->identifier();
    histogram->setIdentifiersFromDim(fakeDimName);
    if (m_verbosity >= Debug) {
      std::cout <<"  Task="<<histogram->taskName()<<" RootName="<<histogram->rootName()
                << " full HName="<<histogram->histogramFullName()<<std::endl;
    }
    histogram->beRegularHisto();
    std::vector<path> foundRootFiles;
    std::vector<path> goodRootFiles;
    if (s_startupFile == timePoint) {
      singleSaveset = true;
      path filePath(pastDuration);
      if (s_rootFileExtension == extension(filePath) ) {
        if (exists(filePath)){
          foundRootFiles.push_back(filePath);
        } else if (exists(m_savesetPath/filePath)){
          foundRootFiles.push_back(m_savesetPath/filePath);
        } 
      }
    } else {
      
      foundRootFiles = findSavesets(histogram->taskName(),
                                    timePoint,
                                    pastDuration);
    } 
    if (!foundRootFiles.empty()) {
      if (m_verbosity >= Verbose) {
        std::cout << "Merging " << foundRootFiles.size() <<
                     " file(s)" << std::endl;
      }
      std::vector<path>::const_iterator foundRootFilesIt;
      foundRootFilesIt = foundRootFiles.begin();
      if (histogram->rootHistogram) {
        delete (histogram->rootHistogram);
        histogram->rootHistogram = NULL;
      }
      TList* list = new TList;
      while (foundRootFilesIt != foundRootFiles.end()) {
        TFile rootFile((*foundRootFilesIt).file_string().c_str());
        if (rootFile.IsZombie()) {
          cout << "Error opening Root file: " <<
            (*foundRootFilesIt).file_string() << endl;
        } else {
          TH1* archiveHisto;
          rootFile.GetObject((histogram->rootName()).c_str(),
                             archiveHisto);
          if (!archiveHisto) { // try w/o algorithm name (to be bkwd compat.)
            rootFile.GetObject((histogram->histogramFullName()).c_str(),
                               archiveHisto);
          }
          if (archiveHisto) {
            list->Add(archiveHisto);
            goodRootFiles.push_back(*foundRootFilesIt);
            if (! histogram->rootHistogram) {
              histogram->rootHistogram = (TH1*) (archiveHisto->Clone());
              histogram->rootHistogram->Reset();
            }
          }
        }
        ++foundRootFilesIt;
      }
      if (histogram->rootHistogram) { // at least one source is available
        if (false == m_mainFrame->isHistoryTrendPlotMode() || 
            histogram->rootHistogram->GetDimension()>1 ||
            singleSaveset) {
          (histogram->rootHistogram)->Merge(list);
        } else {
          // do a trend plot of mean and rms
          std::string histogramTitle("History plot for ");
          histogramTitle += histogram->rootHistogram->GetTitle();
          TH1* newh = new TH1F(histogramTitle.c_str(), histogramTitle.c_str(),
                               list->GetSize(), 0.5,  list->GetSize() +0.5);
          newh->Sumw2();
          for (int i=0 ; i<list->GetSize(); i++) {
            newh->SetBinContent(list->GetSize()-i, ((TH1*)list->At(i))->GetMean() );
            newh->SetBinError(list->GetSize()-i, ((TH1*)list->At(i))->GetMeanError() );
          }
          setHistoryLabels(newh, goodRootFiles);
          delete (histogram->rootHistogram);
          histogram->rootHistogram = newh;
          histogram->setHistoryTrendPlotMode(true);
        }
      }
      else {
        histogram->beEmptyHisto();
      }

      TH1* histo;
      TIter next(list);
      while ((histo = (TH1 *) next())) {
        list->Remove(histo);
        delete histo;
      }
      delete list;
      if (! histogram->rootHistogram) {
        histogram->beEmptyHisto();
      }
    } else {
      histogram->beEmptyHisto();
    }
  } else { //analysis histogram
    if (m_analysisLib) {
      unsigned int ns= (histogram->anaSources())->size();
      std::vector<TH1*> sources(ns);
      bool sourcesOk = true;
      for (unsigned int i=0; i< ns ; ++i) {
        fillHistogram((histogram->anaSources())->at(i),
                      timePoint,
                      pastDuration);
        sources[i]= ((histogram->anaSources())->at(i))->rootHistogram;
        if ( ((histogram->anaSources())->at(i))->isEmptyHisto() ||
             NULL == sources[i]) {
          sourcesOk = false;
        }
      }
      OMAHcreatorAlg* creator = dynamic_cast<OMAHcreatorAlg*>
                     (m_analysisLib->getAlg(histogram->creationAlgorithm()));
      if (creator && sourcesOk) {

        std::string htitle(histogram->onlineHistogram()->htitle());
        histogram->rootHistogram = creator->exec(&sources,
                                                 histogram->anaParameters(),
                                                 histogram->onlineHistogram()->identifier(),
                                                 htitle,
                                                 histogram->isEmptyHisto() ? NULL : histogram->rootHistogram);
        histogram->beRegularHisto();
      }
      else {
        histogram->beEmptyHisto(); 
      }
    }
    if (! (histogram->rootHistogram) ) {
      histogram->beEmptyHisto();
    }
  }
  histogram->setTH1FromDB();
}


std::vector<path> Archive::listAvailableRootFiles(const path & dirPath,
                                                  const date_period & datePeriod,
                                                  const std::string & taskName)
{
  std::vector<path> foundRootFiles;
  std::string partition(m_mainFrame->currentPartition());
  for (day_iterator dateIterator(datePeriod.begin());
      dateIterator <= datePeriod.end(); ++dateIterator) {
    if (m_verbosity >= Verbose) {
      std::cout << "Date: " << to_simple_string(*dateIterator) << std::endl;
    }
    tm d_tm = to_tm(*dateIterator);
    Int_t year, month, day;
    year = d_tm.tm_year + 1900;
    month = d_tm.tm_mon + 1; // http://xkcd.com/163/
    day = d_tm.tm_mday; // + 0 (!?);

    std::stringstream  dayLocation;
    dayLocation << dirPath.string() << s_slash <<
               std::setfill('0') << std::setw(4) << year << s_slash <<
               partition << s_slash << taskName << s_slash <<
               std::setfill('0') << std::setw(2) << month << s_slash <<
               std::setfill('0') << std::setw(2) << day   << s_slash;

    path pathOfTheDay(dayLocation.str());
    if (m_verbosity >= Verbose) {
      std::cout << "Seeking in: " << pathOfTheDay << std::endl;
    }
    directory_iterator end_itr;
    if (exists(pathOfTheDay)) {
      for (directory_iterator itr(pathOfTheDay); itr != end_itr; ++itr) {
        if (is_regular(itr->path()) &&
            s_rootFileExtension == extension(itr->path()) ) {
          foundRootFiles.push_back(itr->path());
        }
      }
    }
   }

// legacy: year overlapping intervals with old format not supported.
  if (foundRootFiles.empty()) {
    tm d_tm = to_tm(datePeriod.end());
    int endYear = d_tm.tm_year + 1900;
    std::stringstream  year;
    year << std::setfill('0') << std::setw(4) << endYear;
    path oldDirPath = m_savesetPath/path(year.str())/path(partition)/path(taskName);
    if (m_verbosity >= Verbose) {
      std::cout << "Legacy mode, seeking in: " << oldDirPath << std::endl;
    }
    if (exists(oldDirPath)) {
      directory_iterator end_itr;
      for (directory_iterator itr(oldDirPath); itr != end_itr; ++itr) {
        if (is_regular(itr->path()) &&
            s_rootFileExtension == extension(itr->path()) ) {
          foundRootFiles.push_back(itr->path());
        }
      }
    }
  }

  sort(foundRootFiles.begin(), foundRootFiles.end());
  if (m_verbosity >= Debug &&
      !foundRootFiles.empty()) {
    m_foundFilesIt = foundRootFiles.begin();
    while (m_foundFilesIt != foundRootFiles.end()) {
      cout << "available file: " << *m_foundFilesIt << endl;
      ++m_foundFilesIt;
    }
  }
  return foundRootFiles;
}
std::vector<std::string> Archive::listPartitions() {
  std::vector<std::string> foundPartitionDirectories;
  if (exists(m_savesetPath)) {
    directory_iterator year_end_itr;
    for (directory_iterator year_itr(m_savesetPath); year_itr != year_end_itr; ++year_itr) {
      if (is_directory(year_itr->path())) {
        directory_iterator partition_end_itr;
        for (directory_iterator partition_itr(year_itr->path()); partition_itr != partition_end_itr; ++partition_itr) {
          if (is_directory(partition_itr->path()) &&
              string::npos == partition_itr->path().string().find("ByRun")) { // Skip GG's run aggregator
            foundPartitionDirectories.push_back(partition_itr->path().leaf());
          }
        }
      }
    }
  }
  sort(foundPartitionDirectories.begin(), foundPartitionDirectories.end());
  if (m_verbosity >= Debug &&
      !foundPartitionDirectories.empty()) {
    m_foundPartitionsIt = foundPartitionDirectories.begin();
    while (m_foundPartitionsIt != foundPartitionDirectories.end()) {
      ++m_foundPartitionsIt;
    }
  }
  return foundPartitionDirectories;  
}
path Archive::findFile(const path & dirPath, const string & fileName)
{
  path foundFile("");
  if (exists(dirPath)) {
    // default construction yields past-the-end
    directory_iterator end_itr;
    for (directory_iterator itr(dirPath); itr != end_itr; ++itr) {
      if (is_directory(itr->status())) {
        foundFile = findFile(itr->path(), fileName);
      } else if (itr->path().leaf() == fileName) {
        foundFile = itr->path();
      }
      if (is_regular(foundFile)) { return foundFile; }
    }
  }
  return foundFile;
}
std::string Archive::createIsoTimeString(int& year, int& month, int& day,
                                         int& hour, int& min, int& sec)
{
  std::stringstream isoTimeStringStream;
  isoTimeStringStream << std::setfill('0') << std::setw(4) << year <<
                         std::setfill('0') << std::setw(2) << month <<
                         std::setfill('0') << std::setw(2) << day << "T" <<
                         std::setfill('0') << std::setw(2) << hour <<
                         std::setfill('0') << std::setw(2) << min <<
                         std::setfill('0') << std::setw(2) << sec; 
  return isoTimeStringStream.str();
}
std::string Archive::createIsoTimeString(const tm &pt_tm)
{
  Int_t year, month, day, hour, min, sec;
  year = pt_tm.tm_year + 1900;
  month = pt_tm.tm_mon + 1; // http://xkcd.com/163/
  day = pt_tm.tm_mday;
  hour = pt_tm.tm_hour;
  min = pt_tm.tm_min;
  sec = pt_tm.tm_sec;
  return createIsoTimeString(year, month, day, hour, min, sec); 
}

std::string Archive::timeDiff(const std::string & startTimeIsoString,
                              const std::string & endTimeIsoString)
{
  ptime startTime(from_iso_string(startTimeIsoString));
  ptime endTime(from_iso_string(endTimeIsoString));
  time_duration diff= endTime - startTime;
  return to_simple_string(diff);
}
std::string Archive::addIsoTimeDate(const std::string & startTimeIsoString,
                                    const std::string & deltaTimeString)
{
  ptime startTime(from_iso_string((startTimeIsoString)));
  time_duration diffTime(duration_from_string((deltaTimeString)));

  ptime newPTime = startTime + diffTime;

  return createIsoTimeString(to_tm(newPTime)); //to_iso_string(newTime);
}
std::string Archive::substractIsoTimeDate(const std::string & startTimeIsoString,
                                          const std::string & deltaTimeString)
{
  ptime startTime(from_iso_string((startTimeIsoString)));
  time_duration diffTime(duration_from_string((deltaTimeString)));

  ptime newPTime = startTime - diffTime;
  
  return createIsoTimeString(to_tm(newPTime)); //to_iso_string(newTime);
}
std::string Archive::taskNameFromFile(const std::string & fileName) {
  TObjArray* fileNameMatchGroup = 0;
  std::string taskNameFound("");   
  fileNameMatchGroup = s_fileDateRegexp.MatchS(path(fileName).leaf());
  if (!fileNameMatchGroup->IsEmpty()) {
    taskNameFound = (((TObjString *)fileNameMatchGroup->At(1))->GetString()).Data();
  } else {
    if (fileNameMatchGroup) {
      fileNameMatchGroup->Delete();
      delete fileNameMatchGroup;
      fileNameMatchGroup = 0;
    }
    // try with run-aggregated saveset
    fileNameMatchGroup = s_fileRunRegexp.MatchS(path(fileName).leaf());
    if (!fileNameMatchGroup->IsEmpty()) {
    taskNameFound = (((TObjString *)fileNameMatchGroup->At(1))->GetString()).Data();
    } 
    else {
      // try with offline DQ saveset
      if (fileNameMatchGroup) {
        fileNameMatchGroup->Delete();
        delete fileNameMatchGroup;
        fileNameMatchGroup = 0;
      }
      fileNameMatchGroup = s_offlineJobRegexp.MatchS(path(fileName).leaf());
      if (!fileNameMatchGroup->IsEmpty()) {
        taskNameFound = (((TObjString *)fileNameMatchGroup->At(1))->GetString()).Data();
      }
    }
  }
  if (fileNameMatchGroup) {
    fileNameMatchGroup->Delete();
    delete fileNameMatchGroup;
    fileNameMatchGroup = 0;
  }
  return taskNameFound;
}        
std::vector<path> Archive::findSavesets(const std::string & taskname,
                                        const std::string & endTimeIsoString,
                                        const std::string & durationTimeString)
{
  std::vector<path> foundRootFiles;
  ptime endTime;
  if ("Now" == endTimeIsoString) {
    endTime = ptime(second_clock::local_time());
  } else {
    endTime = ptime(from_iso_string(endTimeIsoString));
  }

  if (m_verbosity >= Verbose) {
    std::cout << "Archive::findSavesets: " << taskname
              << " timePoint " << to_iso_string(endTime)
              << " pastDuration " << durationTimeString << std::endl;
  }

  time_duration timeDuration(duration_from_string(durationTimeString));
  ptime startTime = endTime - timeDuration;

  date startDate(startTime.date());
  date endDate(endTime.date());
  date_period datePeriod(startDate, endDate);

  refreshDirectory(Archive::Savesets, datePeriod, taskname);

  string taskNameFound;
  string fileTimeFound;
  ptime fileTime;

  if (!m_foundSavesets.empty()) {
    m_foundSavesetsIt = m_foundSavesets.end(); --m_foundSavesetsIt;
    while (m_foundSavesetsIt >= m_foundSavesets.begin()) {
      TObjArray* fileDateMatchGroup = 0;
      fileDateMatchGroup = s_fileDateRegexp.MatchS((*m_foundSavesetsIt).leaf());
      if (!fileDateMatchGroup->IsEmpty()) {
        taskNameFound = (((TObjString *)fileDateMatchGroup->At(1))->GetString()).Data();
        fileTimeFound = (((TObjString *)fileDateMatchGroup->At(3))->GetString()).Data();
        fileTime = from_iso_string(fileTimeFound);
        if (taskNameFound == taskname && !fileTime.is_not_a_date_time()) {
          if (fileTime <= endTime &&
              fileTime >= startTime) {
            if (m_verbosity >= Verbose) {
              cout << "using file: " << (*m_foundSavesetsIt).leaf() << endl;
            }
            foundRootFiles.push_back(*m_foundSavesetsIt);
          } else if (fileTime < startTime) {
            if (fileDateMatchGroup) {
              fileDateMatchGroup->Delete();
              delete fileDateMatchGroup;
              fileDateMatchGroup = 0;
            }
            break;
          }
        }
      }
      if (fileDateMatchGroup) {
        fileDateMatchGroup->Delete();
        delete fileDateMatchGroup;
        fileDateMatchGroup = 0;
      }
      --m_foundSavesetsIt;
    }
  }
  return foundRootFiles;
}

void Archive::saveAsReferenceHistogram(DbRootHist* histogram)
{
  path referenceFilePath(m_referencePath/histogram->taskName());
    bool out(false);
    try {
      create_directories(referenceFilePath);
    } catch (const filesystem_error & error) {
      std::cout << "exception: "
                << error.what()
                << std::endl;
    if (m_verbosity >= Verbose) {}
    }

    std::stringstream refFile;
    refFile << referenceFilePath.file_string() << s_slash
            << histogram->dataType() << s_savesetToken
            << histogram->startRun() << s_rootFileExtension;

    TH1* m_reference = (TH1*) (histogram->rootHistogram)->Clone(histogram->onlineHistogram()->hname().c_str());
    TFile* f = new TFile(refFile.str().c_str(), "UPDATE");
    if (f) {
      if (false == f->IsZombie() ) {
        // keeps old versions.. use Write("",TObject::kOverwrite) to
        // overwrite them
        m_reference->Write();
        f->Write();
        f->Close();
        out = true;
//        histogram->onlineHistogram()->getTask()->setReference(refFile.str());
      }
      delete f;
      f = NULL;
    }
    m_reference->SetDirectory(0);

    if (!out) {
      std::cout << "ERROR in DbRootHist::setReference : could not save reference into file "
                << refFile.str() << std::endl;
    }
}
//void deleteReferenceHistogram(TH1* reference, int revision){}
void Archive::closeRootFiles()
{
//  std::vector<path>::const_iterator foundRootFilesIt;
//  foundRootFilesIt = foundRootFiles.begin();
//  while (foundRootFilesIt != foundRootFiles.end()) {
//    if (rootFile.IsOpen()) {
//      rootFile.Close();
//    }
//    ++foundRootFilesIt;
//  }
}

void Archive::setHistoryLabels(TH1* h, std::vector<boost::filesystem::path>& rootFiles) {
  if (!h) return;
  unsigned int n=h->GetNbinsX();
  if (n != rootFiles.size()) {
    std::cout << " Error in setHistoryLabels: uncompatible file list" << std::endl;
    return;
  }
  int bin=0,lastbin=-100;
  std::string ts("20020131T235959");
  ptime lastTime(from_iso_string(ts));
  time_duration maxdelta= hours(1); 
  int maxnbinUnlabeled = n/4;
  
  std::vector<path>::const_iterator rootFilesIt = rootFiles.end();
  while ( rootFilesIt > rootFiles.begin() ) {
    rootFilesIt--;
    bin++;
    TObjArray* fileDateMatchGroup = 0;
    fileDateMatchGroup = s_fileDateRegexp.MatchS((*rootFilesIt).leaf());
    ptime fileTime = from_iso_string((((TObjString *)fileDateMatchGroup->At(3))->GetString()).Data());
    time_duration delta = (fileTime-lastTime);
    if ( delta > maxdelta || 
         (bin-lastbin) > maxnbinUnlabeled ) { // add a label
      h->GetXaxis()->SetBinLabel(bin, Form("%d/%d %02d:%02d", 
                                           (int) (fileTime.date().day()),
                                           (int) (fileTime.date().month()),
                                           (int) (fileTime.time_of_day().hours()),
                                           (int) (fileTime.time_of_day().minutes()) ) );
      lastTime = fileTime;
      lastbin = bin;
    }
  }

}





