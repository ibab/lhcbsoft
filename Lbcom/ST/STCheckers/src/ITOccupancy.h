// $Id: ITOccupancy.h,v 1.7 2008-12-29 12:55:16 mneedham Exp $
#ifndef ITOccupancy_H
#define ITOccupancy_H 1

#include "Kernel/STHistoAlgBase.h"

namespace AIDA {
  class IHistogram1D;
}

namespace LHCb{
 class STChannelID;
};

class DeSTDetector;

class ISTSignalToNoiseTool;

/** @class ITOccupancy ITOccupancy.h
 *
 *  Class for checking STDigits
 *
 *  @author M.Needham
 *  @author J. van Tilburg
 *  @date   04/12/2006
 */


template <class PBASE>
class ITOccupancy : public ST::HistoAlgBase {

public:
 
  /// constructer
  ITOccupancy( const std::string& name, ISvcLocator *svcloc );

  /// destructer
  virtual ~ITOccupancy();

  /// initialize
  StatusCode initialize();

  /// execute
  StatusCode execute();

private:

  const std::string& dataLocation() const;
 
  const std::string histoDirName() const;

  double defaultThreshold() const;

  void initHistograms();
  void fillHistograms(const PBASE* obj);
  
  unsigned int weight(const PBASE* obj) const; 

  int uniqueInt(const LHCb::STChannelID aChan) const;

  std::map<int,int> m_mapping;
  std::vector<double> m_nStripsVector;
  std::vector<double> m_layerOccVector;
  std::vector<AIDA::IHistogram1D*> m_layerOccHistos;
  std::vector<AIDA::IHistogram1D*> m_stripOccVector;
  
  ISTSignalToNoiseTool* m_sigNoiseTool;  
 
  // job options
  std::vector<double> m_threshold;   ///< List of threshold values
  std::string m_sigNoiseToolName;    ///< Name of the S/N tool 
  int m_binSize;                     ///< Number of channels in each bin
  std::string m_dataLocation;        ///< Location of the digits
 
};

#endif // ITOccupancy_H
