// $Id: PuVetoAlg.cpp,v 1.7 2002-07-13 17:03:17 ocallot Exp $
// Include files 

// from Gaudi
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/MsgStream.h" 
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/ObjectVector.h"

// from VeloEvent
#include "Event/MCVeloFE.h"
// from L0Event
#include "Event/L0PuVeto.h"
// local
#include "PuVetoAlg.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PuVetoAlg
//
// 11/01/2002 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
static const  AlgFactory<PuVetoAlg>          s_factory ;
const        IAlgFactory& PuVetoAlgFactory = s_factory ; 


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PuVetoAlg::PuVetoAlg( const std::string& name,
                      ISvcLocator* pSvcLocator)
  : Algorithm ( name , pSvcLocator )
  , m_inputContainer      ( MCVeloFELocation::PuVeto  )
  , m_outputContainer     ( L0PuVetoLocation::Default )
  , m_threshold           (   20. * keV )
  , m_lowThreshold        (    2.     )
  , m_highThreshold       (    2.     )
  , m_highPosition        (    0. *mm )
  , m_secondPosition      ( -500. *mm )
{
  declareProperty( "InputContainer"     , m_inputContainer  );
  declareProperty( "OutputContainer"    , m_outputContainer );
  declareProperty( "SignalThreshold"    , m_threshold       );
  declareProperty( "LowThreshold"       , m_lowThreshold    );
  declareProperty( "HighThreshold"      , m_highThreshold   );
  declareProperty( "HighPosition"       , m_highPosition    );
  declareProperty( "SecondPosition"     , m_secondPosition  );
}

//=============================================================================
// Destructor
//=============================================================================
PuVetoAlg::~PuVetoAlg() {}; 

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode PuVetoAlg::initialize() {

  MsgStream log(msgSvc(), name());
  log << MSG::DEBUG << "==> Initialise" << endreq;

  SmartDataPtr<DeVelo> velo( detSvc(), "/dd/Structure/LHCb/Velo" );
  if ( 0 == velo ) {
    log << MSG::ERROR << "Unable to retrieve Velo detector element." << endreq;
    return StatusCode::FAILURE;
  }
  m_velo = velo;

  m_lowBound.push_back( -150. * mm );
  m_lowBound.push_back( -100. * mm );
  m_lowBound.push_back(  -50. * mm );
  m_lowBound.push_back(   40. * mm );
  m_lowBound.push_back(  150. * mm );

  m_step.push_back( 1. * mm );
  m_step.push_back( 2. * mm );
  m_step.push_back( 3. * mm );
  m_step.push_back( 5. * mm );

  m_nBin.push_back( 0 );
  m_nBin.push_back( zBin( m_lowBound[1]-0.001)+1 );
  m_nBin.push_back( zBin( m_lowBound[2]-0.001)+1 );
  m_nBin.push_back( zBin( m_lowBound[3]-0.001)+1 );
  m_nBin.push_back( zBin( m_lowBound[4]-0.001)+1 );

  m_totBin = m_nBin[ m_nBin.size()-1 ];

  for ( int ks=0 ; m_velo->nbPuSensor() > ks ; ks++ ) {
    VetoInput a( m_velo->zSensor(ks+100) );
    m_input.push_back( a );
  }

  double binCenter;
  
  for ( unsigned int k=0 ; m_step.size() > k ; k++ ) {
    binCenter = m_lowBound[k] + .5 * m_step[k];
    log << MSG::DEBUG 
        << " zone " << k 
        << " low " << m_lowBound[k]
        << " high " << m_lowBound[k+1]
        << " step " << m_step[k] 
        << " nBin " << m_nBin[k] 
        << " binCenter(beg) " << binCenter
        << endreq;
    while ( binCenter < m_lowBound[k+1] ) {
      m_hist.push_back( 0 );
      m_binCenter.push_back( binCenter );
      binCenter += m_step[k];
    }
  }
  return StatusCode::SUCCESS;
};

//=============================================================================
// Main execution
//=============================================================================
StatusCode PuVetoAlg::execute() {

  MsgStream  log( msgSvc(), name() );
  log << MSG::DEBUG << "==> Execute" << endreq;

  int ks;
  for ( ks=0 ; m_velo->nbPuSensor() > ks ; ks++ ) {
    m_input[ks].strips()->clear();
  }
  //*** get the input data

  SmartDataPtr< MCVeloFEs > fes ( eventSvc() , m_inputContainer );
  if( 0 == fes ) {
    log << MSG::ERROR << "Unable to retrieve input data container="
        << m_inputContainer << endreq;
    return StatusCode::FAILURE;
  }

  //========================================================================
  // Apply a threshold on each strip, OR them by 4 and store the central
  // strip coordinate, to be transformed in R later.
  //========================================================================

  for ( MCVeloFEs::const_iterator itFe = fes->begin(); 
        fes->end() != itFe ; itFe++  ) {
    if ( m_threshold < (*itFe)->charge() ) {
      unsigned int sensor = (*itFe)->sensor();
      if ( 100 > sensor || 103 < sensor ) {
        log << MSG::INFO << "Unexpected sensor " << sensor 
            << " in " << m_inputContainer << endreq;
        continue;
      }
      sensor -= 100;
      int fired           = 4 * ( (*itFe)->strip()/4 ) + 2;
      std::vector<int>* strips = m_input[sensor].strips();
      bool toAdd = true;

      log << MSG::VERBOSE << "PU Sensor " << sensor << " strip " << fired;
      
      for ( std::vector<int>::const_iterator itS = strips->begin();
            strips->end() != itS; itS++) {
        if ( (*itS) == fired ) {
          toAdd = false;
          log << " exists.";
        }
      }
      if ( toAdd ) {
        strips->push_back( fired );
        log << " added.";
      }
      log << endreq;
    }
  }

  fillHisto(  );

  // We have filled the 'histogram'. Search for maximum.
  double height1, sum1, pos1;
  double width;
  double height2, sum2, pos2;
  double integral;
  
  pos1 = peakValue( height1, sum1, width);
  
  log << MSG::DEBUG << " Peak1 : Max " << height1 << " at z= " << pos1 
      << " integral " << sum1 << endreq;

  // If a peak was found, mask the contributing hits, fill again and find the
  // second peak. It is searched always, to have initialized values for the
  // result.

  if ( 0 < height1 ) {
    double zTol = 3.75 * width;
  
    log << MSG::DEBUG << " Mask around z = " << pos1 
      << " window " << zTol << endreq;
    

    maskHits( pos1, zTol );
    fillHisto( );
  }

  pos2 = peakValue( height2, sum2, width );
      
  log << MSG::DEBUG << " 2nd Max " << height2 << " at z= " << pos2
      << " integral " << sum2 << endreq;
  integral = fullIntegral();
  
  // Now take the decision

  int decision;
  if ( ( m_lowThreshold > sum2 ) ||
       ( (m_highThreshold > sum2) && (m_highPosition > pos2) ) ||
       ( m_secondPosition > pos2 ) ) {
    decision = 0;
  } else {
    decision = 1;    // Multiple interaction
  }

  L0PuVeto* pileUp = new L0PuVeto();
  pileUp->setDecision( decision );
  pileUp->setHeightPeak1( height1 );
  pileUp->setSumPeak1( sum1 );
  pileUp->setZPosPeak1( pos1 );
  pileUp->setHeightPeak2( height2 );
  pileUp->setSumPeak2( sum2 );
  pileUp->setZPosPeak2( pos2 );
  //pileUp->setSTot( integral );
  int totMult = 0;
  for ( ks=0 ; m_velo->nbPuSensor() > ks ; ks++ ) {
    totMult += m_input[ks].strips()->size();
  }
  pileUp->setSTot( totMult );

  log << MSG::DEBUG << "== Decision " << decision 
      << " Peak1 z,h,s " << pos1 << " " << height1 << " " << sum1
      << " Peak2 z,h,s " << pos2 << " " << height2 << " " << sum2
      << " Off peaks " << integral << " TotMult " << totMult
      << endreq;
  
  StatusCode sc = eventSvc()->registerObject( m_outputContainer, pileUp );
  if ( sc.isFailure() ) {
    delete pileUp;
    log << MSG::ERROR
        << "Unable to register output to " << m_outputContainer
        << endreq;
    return StatusCode::FAILURE;
  }
  
  for ( ks=0 ; m_velo->nbPuSensor() > ks ; ks++ ) {
    m_input[ks].strips()->clear();
  }

  return StatusCode::SUCCESS;
};

//=============================================================================
//  Finalize
//=============================================================================
StatusCode PuVetoAlg::finalize() {

  MsgStream log(msgSvc(), name());
  log << MSG::DEBUG << "==> Finalize" << endreq;

  return StatusCode::SUCCESS;
}


//=========================================================================
//  Fill the histogram
//=========================================================================
void PuVetoAlg::fillHisto ( ) {
  MsgStream  log( msgSvc(), name() );
  // clear histo
  for ( unsigned int j=0 ; m_hist.size() > j ; j++ ) {
    m_hist[j] = 0;
  }

  double zA, zB, z;
  double rA, rB;
  int    zoneA, zoneB;
  int    bin;

  std::vector<VetoInput>::iterator itSens = m_input.begin();
  VetoInput* sensA;
  VetoInput* sensB;
  std::vector<int>::const_iterator dA;
  std::vector<int>::const_iterator dB;
  
  for ( unsigned int i1 = 0 ; 2 > i1 ; i1++, itSens++ ) {
    sensA = &(*itSens);
    sensB = &(*(itSens+2));

    zA = sensA->zSensor();
    zB = sensB->zSensor();

    std::vector<int>* digsA = sensA->strips();
    std::vector<int>* digsB = sensB->strips() ;

    log << MSG::VERBOSE << "Loop on Sensor " << i1 
        << " z = " << zA << " " << zB
        << " Mult " << digsA->size() << " and " << digsB->size() << endreq;
    for ( dA = digsA->begin() ;  digsA->end() != dA ; dA++ ) {
      if ( 10000 < (*dA) ) { continue; }
      double stripA = (double) (*dA) ;
      rA = m_velo->rOfStrip( stripA, zoneA );
      
      for ( dB = digsB->begin();  digsB->end() != dB ; dB++ ) {
        if ( 10000 < (*dB) ) { continue; }
        double stripB = (double) (*dB);
        rB = m_velo->rOfStrip( stripB, zoneB );

        // Basic Phi matching... Corresponding zones in the sensor.

        if ( !m_velo->matchingZones( zoneA, zoneB ) ) {  continue; }

        if ( rB < rA ) {
          z = ( zB*rA - zA*rB) / ( rA - rB );
          bin = zBin( z );
          if ( 0 <= bin ) { 
            m_hist[bin]++;

            log << MSG::VERBOSE << "  A: " << rA << " B: " << rB 
                << " z " << z << " bin " << bin << endreq;
          }
        }
      }
    }
  }
}

//=========================================================================
//  Mask the hits contributing to the vertex
//=========================================================================
void PuVetoAlg::maskHits ( double zVertex,
                           double zTol   ) {
  MsgStream  log( msgSvc(), name() );
  double stripA, stripB;
  double zA, zB, z;
  double rA, rB;
  int    zoneA, zoneB;

  std::vector<VetoInput>::iterator itSens = m_input.begin();
  VetoInput* sensA;
  VetoInput* sensB;
  std::vector<int>::iterator dA; 
  std::vector<int>::iterator dB; 
  
  for ( unsigned int i1 = 0 ; 2 > i1 ; i1++, itSens++ ) {
    sensA = &(*itSens);
    sensB = &(*(itSens+2));
    zA = sensA->zSensor();
    zB = sensB->zSensor();

    std::vector<int>* digsA = sensA->strips();
    std::vector<int>* digsB = sensB->strips() ;

    for ( dA = digsA->begin() ;  digsA->end() != dA ; dA++ ) {
      if ( 10000 < (*dA) ) { 
        stripA = (double) ((*dA)-10000 );
      } else {
        stripA = (double) (*dA) ;
      }
      
      rA = m_velo->rOfStrip( stripA, zoneA );
      
      for ( dB = digsB->begin() ; digsB->end() != dB ; dB++ ) {
        if ( 10000 < (*dB) ) { 
          stripB = (double) ((*dB)-10000 );
        } else {
          stripB = (double) (*dB);
        }
        rB = m_velo->rOfStrip( stripB, zoneB );

        // Basic Phi matching... Corresponding zones in the sensor.
        if ( !m_velo->matchingZones( zoneA, zoneB ) ) {  continue; }

        if ( rB < rA ) {
          z = ( zB*rA - zA*rB) / ( rA - rB );
          if ( zTol > fabs( zVertex-z ) ) {

            log << MSG::VERBOSE << "  A: " << rA << " B: " << rB 
                << " z " << z << " MASK " << endreq;

            if ( 10000 > (*dA) ) {
              (*dA) += 10000;
            }
            if ( 10000 > (*dB) ) {
              (*dB) += 10000;
            }
          }
        }
      }
    }
  }
}

//=========================================================================
//  
//=========================================================================
double PuVetoAlg::peakValue ( double& height, double& sum, double& width) {
  double pos = -999.;
  height = 0.;
  sum    = 0.;
  width  = 0.;
  int bin = -1;
  for ( int jj=0 ; m_totBin > jj ; jj++ ) {
    if ( height < m_hist[jj] ) {
      height = m_hist[jj];
      bin    = jj;
    }
  }

  if ( (0 < bin) && (m_totBin-1 > bin) ) {
    sum   = m_hist[bin-1] + m_hist[bin] + m_hist[bin+1];
    pos   = m_binCenter[bin-1] * m_hist[bin-1] +
            m_binCenter[bin  ] * m_hist[bin  ] +
            m_binCenter[bin+1] * m_hist[bin+1];
    pos   = pos / sum;
    m_hist[bin-1] = 0.;
    m_hist[bin  ] = 0.;
    m_hist[bin+1] = 0.;
    width = .5 * (m_binCenter[bin+1] - m_binCenter[bin-1]);
  }
  return pos;
}
//=============================================================================
