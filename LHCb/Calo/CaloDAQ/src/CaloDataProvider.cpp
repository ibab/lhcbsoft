// $Id: 
// Include files 

// from Gaudi
#include "GaudiKernel/ToolFactory.h"
// local
#include "CaloDataProvider.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CaloDataProvider
//
// 2005-01-10 : Olivier Callot
//-----------------------------------------------------------------------------

DECLARE_TOOL_FACTORY( CaloDataProvider );

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CaloDataProvider::CaloDataProvider( const std::string& type,
                                      const std::string& name,
                                      const IInterface* parent )
  : CaloReadoutTool ( type, name , parent )
    ,m_adcs()
    ,m_digits(){
  declareInterface<ICaloDataProvider>(this);
  
  // set default detectorName
  int index = name.find_last_of(".") +1 ; // return 0 if '.' not found --> OK !!
  m_detectorName = name.substr( index, 4 ); 
  if ( name.substr(index,3) == "Prs" ) m_detectorName = "Prs";
  if ( name.substr(index,3) == "Spd" ) m_detectorName = "Spd";
}
//=============================================================================
// Destructor
//=============================================================================
CaloDataProvider::~CaloDataProvider() {};


//=========================================================================
//  Initialisation, according to the name -> detector
//=========================================================================
StatusCode CaloDataProvider::initialize ( ) {
  StatusCode sc = GaudiTool::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm
  debug() << "==> Initialize " << name() << endmsg;

  if ( "Ecal" == m_detectorName ) {
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Ecal );
    m_packedType = LHCb::RawBank::EcalPacked;
    m_shortType  = LHCb::RawBank::EcalE;
  } else if ( "Hcal" == m_detectorName ) {
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Hcal );
    m_packedType = LHCb::RawBank::HcalPacked;
    m_shortType  = LHCb::RawBank::HcalE;
  } else if ( "Prs" == m_detectorName ) {
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Prs );
    m_packedType = LHCb::RawBank::PrsPacked;
    m_shortType  = LHCb::RawBank::PrsE;
  } else if ( "Spd" == m_detectorName ) {
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Prs ); // Prs FE for SPD
    m_packedType = LHCb::RawBank::PrsPacked;
    m_shortType  = LHCb::RawBank::PrsTrig;
  } else {
    error() << "Unknown detector name " << m_detectorName << endreq;
    return StatusCode::FAILURE;
  }
  
  m_pedShift = m_calo->pedestalShift();
  long nCells = m_calo->numberOfCells();
  long nPins  = m_calo->numberOfPins();
  m_adcs.reserve( nCells + nPins  );
  m_digits.reserve( nCells );
  clear();
  debug() << " Initialisation OK" << endreq;
  return StatusCode::SUCCESS;
}

//-------------------------------------
StatusCode CaloDataProvider::setBank( ) {
  debug() << "==> Reset " << name() << endmsg;
  // Reset all containers and get CaloBanks
  clear();
  return getCaloBanksFromRaw();
}

//-------------------------------------
void CaloDataProvider::clear( ) {
  m_adcs.clear();
  m_digits.clear();
  m_tell1s = 0;
}

//===================
//  Get ADC Container
//===================
CaloVector<LHCb::CaloAdc>& CaloDataProvider::adcs(int source){
  clear();
  decodeTell1(source);
  return m_adcs;
}

CaloVector<LHCb::CaloDigit>& CaloDataProvider::digits(int source){
  adcs(source);
  for(CaloVector<LHCb::CaloAdc>::iterator iadc = m_adcs.begin();iadc!=m_adcs.end();++iadc){
    int temp = (*iadc).adc();
    LHCb::CaloCellID id = (*iadc).cellID() ;
    double e = ( double(temp) - m_pedShift ) * m_calo->cellGain( id );
    LHCb::CaloDigit dig(id,e);
    m_digits.addEntry( dig , id);
  }
  return m_digits;
}


//========== 
//  Get data
//==========
double CaloDataProvider::digit (LHCb::CaloCellID id){
  if( 0 >  m_digits.index(id) ){
    int temp = adc(id);
    double e = ( double(temp) - m_pedShift ) * m_calo->cellGain( id );
    LHCb::CaloDigit dig(id,e);
    m_digits.addEntry( dig , id);
    return e;
  }
  return m_digits[id].e();
}
//-------------------------------------------------------
int CaloDataProvider::adc (LHCb::CaloCellID id){
  if( 0 >  m_adcs.index(id) )decodeCell( id );
  return m_adcs[id].adc();
}
//-------------------------------------------------------
StatusCode CaloDataProvider::decodeCell(LHCb::CaloCellID id ){
  int card = m_calo->cardNumber (id)   ; // Fe-Card from cellId
  if(card<0)return StatusCode::FAILURE;
  int tell1 = m_calo->cardToTell1(card); // Tell1 from FE-Card
  if(tell1<0)return StatusCode::FAILURE;
  return decodeTell1( tell1 );
}
//-------------------------------------------------------
StatusCode  CaloDataProvider::decodeTell1 (int source) {
  StatusCode sc = StatusCode::FAILURE;
  int sourceID  ;
  for( std::vector<LHCb::RawBank*>::const_iterator itB = m_banks->begin(); 
       itB != m_banks->end() ; ++itB ) {
    sourceID       = (*itB)->sourceID();
    if( source >= 0 && source != sourceID )continue;
    if( "Spd" == m_detectorName ){
      sc = decodePrsTriggerBank( *itB);
    }
    else{
      sc = decodeBank ( *itB );
    }
    if( !sc.isSuccess() ) break;
    m_tell1s++; // count the number of decoded TELL1
  } 
  if( !sc.isSuccess() ){
    error() << " Error when decoding bank " << sourceID  << endreq;
  }
  return sc;
}
//==================================
// Main method to decode the rawBank 
//==================================
StatusCode CaloDataProvider::decodeBank( LHCb::RawBank* bank ){
  // Get bank info
  unsigned int* data = bank->data();
  int size           = bank->size()/4;  // Bank size is in bytes
  int version        = bank->version();
  int sourceID       = bank->sourceID();
  debug() << "Decode bank " << bank << " source " << sourceID 
          << " version " << version << " size " << size << endreq;

  // Get the FE-Cards associated to that bank (via condDB)
  std::vector<int> feCards = m_calo->tell1ToCards( sourceID );
  int nCards = feCards.size();
  debug() << nCards << " FE-Cards are expected to be readout : " 
          << feCards << " in Tell1 bank " << sourceID << endreq;
  

  if ( 1 > version || 3 < version ) {
    warning() << "Bank type " << bank->type() << " sourceID " << sourceID 
              << " has version " << version 
              << " which is not supported" << endreq;

  } else if ( 1 == version ) {
    //******************************************************************
    //**** Simple coding, ID + adc in 32 bits.
    //******************************************************************
    while( 0 != size ) {
      int id   = ((*data) >> 16) & 0xFFFF;
      int adc  =  (*data) & 0xFFFF;
      if ( 32767 < adc ) adc |= 0xFFFF0000;  //= negative value
      LHCb::CaloCellID cellId( id );

      //event dump
      verbose() << " |  Tell1 : " << sourceID
                << " |  FeBoard : " << m_calo->cardNumber(cellId)
                << " |  CaloCell " << cellId
                << " |  valid ? " << m_calo->valid(cellId)
                << " |  ADC value = " << adc << endreq;
      
      LHCb::CaloAdc temp(cellId,adc);
      m_adcs.addEntry(temp  ,cellId );

      ++data;
      --size;
    }

  } else if ( 2 == version ) {
    //******************************************************************
    //**** 1 MHz compression format, Ecal and Hcal
    //******************************************************************
    while( 0 != size ) {
      // Skip 
      unsigned int word = *data++;
      size--;
      // Read bank header
      int lenTrig = word & 0x7F;
      int code    = (word >> 14 ) & 0x1FF;
      // access chanID via condDB
      std::vector<LHCb::CaloCellID> chanID  ;
      // look for the FE-Card in the Tell1->cards vector
      int card = findCardbyCode(feCards,code);
      if( 0 <= card ){
        chanID = m_calo->cardChannels( feCards[card] );
        feCards.erase(feCards.begin()+card);
      }else{
        error() << " FE-Card w/ [code : " << code << "] not associated with TELL1 bank " << sourceID
                << " in condDB :  Cannot read that bank" << endreq;
        return StatusCode::FAILURE;
      }
      // Start readout of the FE-board
      // First skip trigger bank ...
      int nSkip = (lenTrig+3)/4;   //== is in bytes, with padding
      data     += nSkip;
      size     -= nSkip;
      unsigned int pattern  = *data++;
      int offset   = 0;
      unsigned int lastData = *data++;
      size -= 2;
      // ... and readout data
      for ( int bitNum = 0; 32 > bitNum; bitNum++ ) {
        int adc;
        if ( 31 < offset ) {
          offset  -= 32;
          lastData =  *data++;
          size--;
        }
        if ( 0 == ( pattern & (1<<bitNum) ) ) {  //.. short coding
          adc = ( ( lastData >> offset ) & 0xF ) - 8;
          offset += 4;
        } else {
          adc =  ( ( lastData >> offset ) & 0xFFF ); 
          if ( 24 == offset ) adc &= 0xFF;
          if ( 28 == offset ) adc &= 0xF;  //== clean-up extra bits
          offset += 12;
          if ( 32 < offset ) {  //.. get the extra bits on next word
            lastData = *data++;
            size--;
            offset -= 32;        
            int temp = (lastData << (12-offset) ) & 0xFFF;
            adc += temp;
          }
          adc -= 256;
        }
        LHCb::CaloCellID id = chanID[ bitNum ];

        // event dump
        verbose() << " |  Tell1 : " << sourceID
                  << " |  FeBoard : " << m_calo->cardNumber(id)
                  << " |  Channel : " << bitNum
                  << " |  CaloCell " << id
                  << " |  valid ? " << m_calo->valid(id)
                  << " |  ADC value = " << adc << endreq;
        
        
        //== Keep only valid cells
        if ( 0 != id.index() ){
          LHCb::CaloAdc temp(id,adc);
          m_adcs.addEntry( temp ,id );
        }        
      }
    }
    // Check All cards have been read
    checkCards(nCards,feCards);

  } else if ( 3 == version ) {
    //******************************************************************
    //**** 1 MHz compression format, Preshower + SPD
    //******************************************************************
    while( 0 != size ) {
      // Skip
      unsigned int word = *data++;
      size--;
      // Read bank header
      int lenTrig = word & 0x7F;
      int lenAdc  = (word >> 7 ) & 0x7F;
      int code  = (word >>14 ) & 0x1FF;
      // access chanID via condDB
      std::vector<LHCb::CaloCellID> chanID  ;
      // look for the FE-Card in the Tell1->cards vector
      int card = findCardbyCode(feCards,code);
      if( 0 <= card ){
        chanID = m_calo->cardChannels( feCards[card] );
        feCards.erase(feCards.begin()+card);
      }else{
        error() << " FE-Card w/ [code : " << code << "] not associated with TELL1 bank " << sourceID
                << " in condDB :  Cannot read that bank" << endreq;
        return StatusCode::FAILURE;
      }

      // Read the FE-Board
      // skip the trigger bits
      int nSkip = (lenTrig+3)/4;  //== Length in byte, with padding
      size -= nSkip;
      data     += nSkip;


      // read data
      int offset   = 32;  //== force read the first word, to have also the debug for it.
      unsigned int lastData = 0;
      
      while ( 0 < lenAdc ) {
        if ( 32 == offset ) {
          lastData =  *data++;
          size--;
          offset = 0;
        }
        int adc = ( lastData >> offset ) & 0x3FF;
        int num = ( lastData >> (offset+10) ) & 0x3F;
        LHCb::CaloCellID id = chanID[ num ];

        // event dump
        verbose() << " |  Tell1 : " << sourceID
                << " |  FeBoard : " << m_calo->cardNumber(id)
          //<< " |  Channel : " << bitNum
                << " |  CaloCell " << id
                << " |  valid ? " << m_calo->valid(id)
                << " |  ADC value = " << adc << endreq;


        LHCb::CaloAdc temp(id,adc);
        m_adcs.addEntry( temp ,id );
        lenAdc--;
        offset += 16;
      }
    } //== DataSize
    // Check All cards have been read
    checkCards(nCards,feCards);    
  } //== versions

  return StatusCode::SUCCESS;
}



//==================================
StatusCode CaloDataProvider::decodePrsTriggerBank( LHCb::RawBank* bank ) {
  unsigned int* data = bank->data();
  int size           = bank->size()/4;  // size in byte
  int version        = bank->version();
  int sourceID       = bank->sourceID();
  int lastData       = 0;
  debug() << "Decode Prs bank " << bank << " source " << sourceID 
          << " version " << version << " size " << size << endreq;
  // Get the FE-Cards associated to that bank (via condDB)
  std::vector<int> feCards = m_calo->tell1ToCards( sourceID );
  int nCards = feCards.size();
  debug() << nCards << " FE-Cards are expected to be readout : " 
          << feCards << " in Tell1 bank " << sourceID << endreq;

  //=== Offline coding: a CellID, 8 SPD bits, 8 Prs bits
  if ( 1 == version ) {
    while ( 0 != size ) {
      int spdData = (*data >> 8 ) & 0xFF;
      int prsData = (*data) & 0xFF;
      int lastID  = (*data) >> 16;
        ++data;
        --size;
        for ( unsigned int kk = 0; 8 > kk; ++kk ) {

          LHCb::CaloCellID id( lastID+kk );
          LHCb::CaloCellID spdId( (lastID+kk) & 0x3FFF );


          if ( spdData & 1 ){
            LHCb::CaloAdc temp(spdId,1);
            m_adcs.addEntry( temp ,spdId) ;
          }
          

          //event dump
          verbose() << " |  Tell1 : " << sourceID
                    << " |  FeBoard : " << m_calo->cardNumber(id)
                    << " |  CaloCell " << id
                    << " |  valid ? " << m_calo->valid(id)
                    << " |  Prs/Spd  = " << (prsData & 1) << "/" << (spdData & 1) << endreq;

          spdData = spdData >> 1;
          prsData = prsData >> 1;
        }
    }
    //=== Compact coding: a CellID, and its Prs/SPD bits
  } else if ( 2 == version ) {
    while ( 0 != size ) {
      int word = *data;
      while ( 0 != word ) {
        int item = word & 0xFFFF;
        word = ( word >> 16 ) & 0xFFFF;
        int spdId =  (item&0xFFFC) >> 2;

        //event dump
        LHCb::CaloCellID prsId( spdId + 0x4000 );   // Prs
        verbose() << " |  Tell1 : " << sourceID
                  << " |  FeBoard : " << m_calo->cardNumber( prsId )
                  << " |  CaloCell " << prsId
                  << " |  valid ? " << m_calo->valid( prsId )
                  << " |  Prs/Spd  = " << (item&1) << "/" << (item&2) << endreq;
        
        
        if ( 0 != (item & 2) ) {
          LHCb::CaloCellID id ( spdId );   // SPD
          LHCb::CaloAdc temp(id,1);
          m_adcs.addEntry( temp ,id);
        }
      }
      ++data;
      --size;
    }
    //==== Codage for 1 MHz
  } else if ( 3 == version ) {
    int offset   = 0;
    int lenAdc   = 0;
    int lenTrig  = 0;
    while( 0 != size ) {
      int word = *data++;
      size--;
      lenTrig = word & 0x7F;
      lenAdc  = (word >> 7 ) & 0x7F;
      if ( msgLevel( MSG::DEBUG) ) {
        debug() << format( "  Header data %8x size %4d lenAdc%3d lenTrig%3d",
                           word, size, lenAdc, lenTrig )
                << endreq;
      }
      int code  = (word >>14 ) & 0x1FF;
      debug() << "Read FE-board ["<< code << "] linked to TELL1 bank " << sourceID << endreq;      
      // access chanID via condDB
      std::vector<LHCb::CaloCellID> chanID  ;
      // look for the FE-Card in the Tell1->cards vector
      int card = findCardbyCode(feCards,code);
      if( 0 <= card ){
        chanID = m_calo->cardChannels( feCards[card] );
        feCards.erase(feCards.begin()+card);
      }else{
        error() << " FE-Card w/ [code : " << code << "] not associated with TELL1 bank " << sourceID
                << " in condDB :  Cannot read that bank" << endreq;
        return StatusCode::FAILURE;
      }

      // Process FE-data decoding
      offset   = 32;      
      while ( 0 < lenTrig ) {
        if ( 32 == offset ) {
          lastData = *data++;
          size--;
          offset = 0;
        }
        int num   = ( lastData >> offset ) & 0x3F;
        int isPrs = ( lastData >> (offset +6)) & 1;
        int isSpd = ( lastData >> (offset +7)) & 1;
        offset += 8;
        lenTrig--;
        LHCb::CaloCellID id =  chanID[ num ];


        // event dump
        verbose() << " |  Tell1 : " << sourceID
                  << " |  FeBoard : " << code 
          //<< " |  Channel : " << bitNum
                  << " |  CaloCell " << id
                  << " |  valid ? " << m_calo->valid(id)
                  << " |  Prs/Spd  = " << isPrs << "/" << isSpd << endreq;
        


        if ( 0 != isSpd ) {
          LHCb::CaloCellID spdId( 0, id.area(), id.row(), id.col() );
          LHCb::CaloAdc temp(spdId,1);
          m_adcs.addEntry( temp ,spdId);
        }
      }
      int nSkip = (lenAdc+1 ) / 2;  // Length in number of words
      size     -= nSkip;
      data     += nSkip;
    } //== DataSize
    // Check All cards have been read
    checkCards(nCards,feCards);
  } //== versions
  
  
  return StatusCode::SUCCESS;
}
