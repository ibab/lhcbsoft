// $Id: HistogramIdentifier.h,v 1.19 2010-06-03 21:27:40 robbep Exp $

#ifndef HISTOGRAMIDENTIFIER_H_
#define HISTOGRAMIDENTIFIER_H_

#ifdef WIN32
#pragma warning( push )
#pragma warning( disable : 4800 )
#endif
#include <TString.h>
#ifdef WIN32
#pragma warning( pop )
#endif

// Class to handle histogram identifier parsing / splitting
class HistogramIdentifier {
 public:
  HistogramIdentifier(const std::string & histogramUrl);
  virtual ~HistogramIdentifier();
  
  void setIdentifiersFromDim(std::string newDimServiceName);
  
  std::string histogramIdentifier() const { return m_identifier; };
  std::string histogramType() const { return m_histogramType; };
  std::string partitionName() const { return m_partitionName; };
  std::string nodeName() const { return m_nodeName; };
  std::string taskName() const { return m_taskName; };
  std::string instanceOnNode() const { return m_instanceOnNode; };
  std::string histogramUTGID() const { return m_histogramUTGID; };
  std::string algorithmName() const {return m_algorithmName; };
  bool isFromHistogramSet() const {return m_isFromHistogramSet; };
  std::string histogramSetName() const { return m_setName; };
  std::string histogramName() const { return m_histogramName; };
  std::string histogramFullName() const { return m_histogramFullName; };
  std::string rootName() const { return m_algorithmName+"/"+m_histogramFullName; };
  std::string lastName() const { return m_lastName; };
  std::string fileName() const { return m_fileName; };
  bool isPlausible() const { return m_isPlausible; };
  bool isDimFormat() const { return m_isDimFormat; };
  bool isEFF() const { return m_isEFF; };
  std::string histogramUrl() const { return m_histogramUrlTS.Data(); };
  std::string dbDimServiceName() const { return m_dbDimServiceName; };
  int dbHistogramType() const { return m_dbHistogramType; };
  
 protected:
  TString m_histogramUrlTS;
  std::string m_identifier;
  std::string m_histogramType;
  std::string m_partitionName;
  std::string m_nodeName;
  std::string m_taskName;
  std::string m_instanceOnNode;
  std::string m_histogramUTGID;
  std::string m_algorithmName; 
  bool m_isFromHistogramSet;
  bool m_isEFF;
  std::string m_setName;
  std::string m_histogramName;// is the sub-name in case of set
  std::string m_histogramFullName;
  std::string m_lastName;
  std::string m_fileName;
  bool m_isPlausible;
  bool m_isDimFormat;
  std::string m_dbDimServiceName;
  int m_dbHistogramType;
};

#endif /*HISTOGRAMIDENTIFIER_H_*/
