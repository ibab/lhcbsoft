
//-----------------------------------------------------------------------------
/** @file RichRawDataSize.cpp
 *
 *  Implementation file for monitor : Rich::DAQ::RawDataSize
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2008-10-14
 */
//-----------------------------------------------------------------------------

// from Gaudi
#include "GaudiKernel/AlgFactory.h"

// local
#include "RichRawDataSize.h"

//-----------------------------------------------------------------------------

using namespace Rich::DAQ;

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( RawDataSize );

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
RawDataSize::RawDataSize( const std::string& name,
                          ISvcLocator* pSvcLocator)
  : HistoAlgBase       ( name , pSvcLocator ),
    m_SmartIDDecoder   ( NULL  ),
    m_RichSys          ( NULL  ),
    m_taeEvents        ( 1, "" )
{
  declareProperty( "RawEventLocations", m_taeEvents   );
  declareProperty( "FillHPDPlots", m_hpdPlots = false );
  setProperty( "HistoPrint", false );
}

//=============================================================================
// Destructor
//=============================================================================
RawDataSize::~RawDataSize() {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode RawDataSize::initialize()
{
  const StatusCode sc = HistoAlgBase::initialize();
  if ( sc.isFailure() ) return sc;

  // get tools
  acquireTool( "RichSmartIDDecoder", m_SmartIDDecoder, 0, true );

  // RichDet
  m_RichSys = getDet<DeRichSystem>( DeRichLocations::RichSystem );

  return sc;
}

StatusCode RawDataSize::prebookHistograms()
{

  const Rich::DAQ::Level1HardwareIDs & l1IDs = m_RichSys->level1HardwareIDs();
  for ( Rich::DAQ::Level1HardwareIDs::const_iterator iL = l1IDs.begin();
        iL != l1IDs.end(); ++iL )
  {
    std::ostringstream title, ID;
    title << "Data Size (32bit words) : L1HardwareID " << *iL;
    ID << "L1s/L1HardwareID" << *iL;
    richHisto1D( ID.str(), title.str(), -0.5, 500.5, 501 );
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode RawDataSize::execute()
{
  StatusCode sc = StatusCode::SUCCESS;

  for ( std::vector<std::string>::const_iterator iLoc = m_taeEvents.begin();
        iLoc != m_taeEvents.end(); ++iLoc )
  {
    sc = sc && processTAEEvent( *iLoc );
  }

  return sc;
}

StatusCode RawDataSize::processTAEEvent( const std::string & taeEvent )
{

  // Map of words per L1 board
  Rich::Map<const Rich::DAQ::Level1HardwareID,unsigned int> l1SizeMap;

  // full TES location for this raw event
  const std::string reLoc = ( taeEvent.empty() ?
                              LHCb::RawEventLocation::Default :
                              taeEvent + "/" + LHCb::RawEventLocation::Default );

  // load the raw RICH data at this location
  if ( exist<LHCb::RawEvent>(reLoc) )
  {
    LHCb::RawEvent * rawEvent = get<LHCb::RawEvent>(reLoc);

    // Get the banks for the Rich
    const LHCb::RawBank::Vector & richBanks = rawEvent->banks( LHCb::RawBank::Rich );

    // Loop over data banks
    for ( LHCb::RawBank::Vector::const_iterator iBank = richBanks.begin();
          iBank != richBanks.end(); ++iBank )
    {
      const unsigned int L1size = (*iBank)->size() / 4; // size in # 32 bit words
      const Level1HardwareID L1ID ( (*iBank)->sourceID() );
      l1SizeMap[L1ID] += L1size;
    } // raw banks

    // get the decoded data for this tae event
    const Rich::DAQ::L1Map & l1Map = m_SmartIDDecoder->allRichSmartIDs(taeEvent);

    // abort checks if bad HPD detected, as in this case difficult to make sure
    // the test is valid
    bool abortSizeCheck = false;

    // loop over decoded data
    for ( Rich::DAQ::L1Map::const_iterator iL1Map = l1Map.begin();
          iL1Map != l1Map.end(); ++iL1Map )
    {
      const Rich::DAQ::Level1HardwareID l1HardID = iL1Map->first;
      const Rich::DAQ::IngressMap & ingressMap  = iL1Map->second;

      // Start with 1 word per HPD ingress header
      unsigned int nL1Words(Rich::DAQ::NumIngressPerL1);

      for ( Rich::DAQ::IngressMap::const_iterator iIngressMap = ingressMap.begin();
            iIngressMap != ingressMap.end(); ++iIngressMap )
      {
        const Rich::DAQ::IngressInfo & ingressInfo = iIngressMap->second;
        const Rich::DAQ::HPDMap & hpdMap = ingressInfo.hpdData();

        for ( Rich::DAQ::HPDMap::const_iterator iHPDMap = hpdMap.begin();
              iHPDMap != hpdMap.end(); ++iHPDMap )
        {
          const Rich::DAQ::HPDInfo & hpdInfo           = iHPDMap->second;
          const LHCb::RichSmartID  & hpdID             = hpdInfo.hpdID();
          const Rich::DAQ::HPDInfo::Header & hpdHeader = hpdInfo.header();
          const Rich::DAQ::HPDInfo::Footer & hpdFooter = hpdInfo.footer();

          // Only use valid HPD blocks to include data size 
          const bool hpdOK = !hpdHeader.inhibit() && hpdID.isValid();
          if ( !hpdOK ) abortSizeCheck = true;
          
          // words for this HPD
          const unsigned int nHPDwords = ( hpdHeader.nHeaderWords() +
                                           hpdFooter.nFooterWords() +
                                           hpdHeader.nDataWords()   );

          // count words per L1 board
          nL1Words += nHPDwords;

          if ( hpdOK && m_hpdPlots )
          {
            // use a try block in case of DB lookup errors
            try
            {
              // Get the HPD hardware ID
              const Rich::DAQ::HPDHardwareID hpdHardID = m_RichSys->hardwareID(hpdID);
              // fill plots
              std::ostringstream title, ID;
              title << "Data Size (32bit words) : HPDHardwareID " << hpdHardID;
              ID << "hpds/HPDHardwareID" << hpdHardID;
              richHisto1D( ID.str(), title.str(), -0.5, 50.5, 51 ) -> fill( nHPDwords );
            }
            catch ( const GaudiException & excpt )
            {
              Error( excpt.message() ).ignore();
            }
          } // do individual HPD plots

        } // loop over HPDs

      } // loop over ingresses

      std::ostringstream ID;
      ID << "L1s/L1HardwareID" << l1HardID;
      richHisto1D( ID.str() ) -> fill ( nL1Words );

      // Disable until better understood
      // Compare to the value obtained direct from the RawEvent(s)
      //if ( !abortSizeCheck && (nL1Words != l1SizeMap[l1HardID]) )
      //{
      //  std::ostringstream mess;
      //  mess << "L1 raw data size mis-match for L1HardwareID " << l1HardID;
      //  Error( mess.str() ).ignore();
      //  error() << mess.str() << " " << nL1Words << " " << l1SizeMap[l1HardID] << endmsg;
      //}

    } // loop over L1 boards


  } // raw event exists

  return StatusCode::SUCCESS;
}
