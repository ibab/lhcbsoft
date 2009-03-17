// $Id: STNZSMonitor.cpp,v 1.2 2009-03-17 11:23:30 nchiapol Exp $

// Gaudi
#include "GaudiKernel/AlgFactory.h"

// LHCbKernel
#include "Kernel/STDetSwitch.h"
#include "Kernel/STDAQDefinitions.h"
#include "Kernel/STBoardMapping.h"
#include "Kernel/LHCbConstants.h"

// STTELL1Event
#include "Event/STTELL1Data.h"

// AIDA
#include "AIDA/IHistogram1D.h"

// standard
#include "gsl/gsl_math.h"
#include "boost/lexical_cast.hpp"

// local
#include "STNZSMonitor.h"

using namespace LHCb;
using namespace AIDA;
using namespace STDAQ;
using namespace STBoardMapping;

DECLARE_ALGORITHM_FACTORY( STNZSMonitor);

//--------------------------------------------------------------------
//
//  STNZSMonitor
//
//--------------------------------------------------------------------

STNZSMonitor::STNZSMonitor( const std::string& name, ISvcLocator* pSvcLocator ) :
  GaudiHistoAlg(name, pSvcLocator) 
{
  // constructer
  declareProperty("DetType", m_detType = "TT" );
  declareProperty("InputData", m_dataLocation = STTELL1DataLocation::TTFull );
  declareProperty("UseSourceID", m_useSourceID = true );
  declareProperty("BaseNameHisto", m_basenameHisto = "raw" );
  declareProperty("FollowPeriod", m_followingPeriod = 2000);
  declareProperty("UpdateRate", m_updateRate = -1);  
  declareProperty("ResetRate", m_resetRate = -1);  
  declareProperty("SkipEvents", m_skipEvents = -1 );
}

STNZSMonitor::~STNZSMonitor()
{
  // destructer
}

StatusCode STNZSMonitor::initialize()
{
  // Initialize GaudiHistoAlg
  StatusCode sc = GaudiHistoAlg::initialize();
  if (sc.isFailure()) {
    return Error("Failed to initialize", sc);
  }
  
  // Set the top directory to IT or TT.
//  if( "" == histoTopDir() ) {
//    setHistoTopDir(m_detType+"/");
//  }

  STDetSwitch::flip(m_detType,m_dataLocation);

  //const Map *NumberToSourceIDMap;
  //const std::map<unsigned int, unsigned int> *NumberToSourceIDMap;
  if (m_detType == "TT") {
    m_TELL1Mapping      = &TTSourceIDToNumberMap();
    //NumberToSourceIDMap = &TTNumberToSourceIDMap();
  } else {
    m_TELL1Mapping      = &ITSourceIDToNumberMap();
    //NumberToSourceIDMap = &ITNumberToSourceIDMap();
  }

  m_basenameHisto += "_$tell";
  m_evtNumber = 0;

//        std::map<unsigned int, unsigned int>::const_iterator iterMap = NumberToSourceIDMap->begin();
//  const std::map<unsigned int, unsigned int>::const_iterator endMap  = NumberToSourceIDMap->end();
//  for ( ; iterMap != endMap ; iterMap++ ) {
//    int tellID = iterMap->first;
//    if (m_useSourceID) {
//      tellID = iterMap->second;
//    }
//    std::string title = m_basenameHisto + boost::lexical_cast<std::string>(tellID);
//    book(title, 0, nStripsPerBoard, nStripsPerBoard);
//  }

  return StatusCode::SUCCESS;
}

StatusCode STNZSMonitor::execute()
{ 
  m_evtNumber++;
  
  // Skip first m_skipEvents. Useful when running over CMS data.
  if( m_evtNumber < m_skipEvents ) {
    return StatusCode::SUCCESS;
  }
 
  // Skip if there is no Tell1 data
  if (!exist<STTELL1Datas>(m_dataLocation)) {
    return StatusCode::SUCCESS;
  }

  // Get the data
  const STTELL1Datas* data = get<STTELL1Datas>(m_dataLocation);
  //debug() << "Found " << data->size() << " boards." << endmsg;

  // loop over the data
  STTELL1Datas::const_iterator iterBoard = data->begin(); 
  for (; iterBoard != data->end() ; ++iterBoard){

    // get the tell board and the data headers
    int sourceID = (*iterBoard)->TELL1ID();
    const STTELL1Data::Data& dataValues = (*iterBoard)->data();

    // Create a name for the tell1
    int tellID = m_useSourceID ? sourceID :
      (m_TELL1Mapping->find(sourceID))->second;

    // Reset the maps for each tell1 and book the histogram
    if ( m_meanMap.find(tellID) == m_meanMap.end() ) {
      m_meanMap[tellID].resize(3072,0.0);
      m_meanSqMap[tellID].resize(3072, 0.0);
      m_nEvents[tellID] = 0;
    }
    m_nEvents[tellID]++;

    // Cumulative average up to m_followingPeriod; after that
    // exponential moving average
    int nEvt = m_nEvents[tellID];
    if( m_followingPeriod > 0 && nEvt > m_followingPeriod ) 
      nEvt = m_followingPeriod;
    
    // Loop over the links (i.e. Beetles)
    unsigned int iBeetle= 0;
    for ( ; iBeetle < noptlinks; ++iBeetle ){

      // Loop over the strips in this link
      unsigned int iStrip = 0;
      for ( ; iStrip < LHCbConstants::nStripsInBeetle ; ++iStrip){

        // Get the ADC value
        const int value = dataValues[iBeetle][iStrip];

        // Calculate the pedestal and the pedestal squared
        int strip = iStrip + iBeetle * LHCbConstants::nStripsInBeetle;
        m_meanMap[tellID][strip] = (m_meanMap[tellID][strip]*(nEvt-1)
                                      + value ) / nEvt;
        m_meanSqMap[tellID][strip] = (m_meanSqMap[tellID][strip]*(nEvt-1) 
                                        + gsl_pow_2(value) ) / nEvt;
      } // strip
    }  // beetle

    // Update the noise histogram
    if( m_updateRate > 0  && m_nEvents[tellID]%m_updateRate == 0 ) {
      updateNoiseHistogram( tellID );
    }

    // Resets the event counter
    if( m_resetRate > 0  && m_nEvents[tellID]%m_resetRate == 0 ) {
      m_nEvents[tellID] = 0;
    }

  } // boards
  return StatusCode::SUCCESS;
}

StatusCode STNZSMonitor::finalize()
{
  //printHistos();
  // Update al histograms at the end
  DataMap::const_iterator iTell = m_meanMap.begin();
  for( ; iTell != m_meanMap.end(); ++iTell ) { 
    updateNoiseHistogram( (*iTell).first );
  } 

  return StatusCode::SUCCESS;
}

void STNZSMonitor::updateNoiseHistogram(int tellID)
{
  // Create a title for the histogram
  //std::string title = m_basenameHisto + boost::lexical_cast<std::string>(tellID);
  std::string strTellID  = boost::lexical_cast<std::string>(tellID);
  std::string histoID    = m_basenameHisto + strTellID;
  std::string histoTitle = "Noise for Tell" + strTellID;

  IHistogram1D* hist = histo1D( histoID ) ;
  //if ( hist == 0 ) {
    //error() << "Histogram " << histoID << " not found" << endmsg;
    //return;
  //}
  //hist->reset();

  // Loop over strips in tell1
  for (unsigned int strip = 0u; strip < nStripsPerBoard; ++strip) {
    double rms = sqrt( m_meanSqMap[tellID][strip] 
                       - gsl_pow_2(m_meanMap[tellID][strip]));
    if ( hist ) {
      hist->reset();
      hist->fill(strip, rms);
    } else {
      plot1D( strip, histoID, histoTitle, 0, nStripsPerBoard, nStripsPerBoard, rms );
    }
  }
}
