// $Id: $
// Include files

// from Gaudi
#include "GaudiKernel/AlgFactory.h"
#include "Event/Track.h"
#include "Event/StateParameters.h"

// local
#include "PatPixelTracking.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PatPixelTracking
//
// 2011-12-16 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( PatPixelTracking )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  PatPixelTracking::PatPixelTracking( const std::string& name,
                                      ISvcLocator* pSvcLocator)
#ifdef DEBUG_HISTO
    : GaudiTupleAlg(name, pSvcLocator)
#else
    : GaudiAlgorithm ( name , pSvcLocator )
#endif

{
  declareProperty( "OutputTracksName",      m_outputLocation    = LHCb::TrackLocation::Velo );
  declareProperty( "MaxXSlope",             m_maxXSlope         =  0.400 );                     // 0.400
  declareProperty( "MaxYSlope",             m_maxYSlope         =  0.400 );                     // 0.300
  declareProperty( "ExtraTol",              m_extraTol          =  1.000 * Gaudi::Units::mm );  // 0.150 // (initial) extrapolation tolerance when adding hits
  declareProperty( "MaxChi2ToAdd",          m_maxChi2ToAdd      = 200.0 );  // 100.0 // (initial) limit on track chi2 when adding new hits
  declareProperty( "MaxChi2SameSensor",     m_maxChi2SameSensor = 16.0  );  // 16.0 // limit when adding hits from sensor that already has hits ?
  declareProperty( "MaxMissed",             m_maxMissed         =  4    );  // stops the extrapolation after that many sensors
  declareProperty( "MaxChi2PerHit",         m_maxChi2PerHit     = 25.0  );  // 16.0 // limit when removing worst hits
  declareProperty( "MaxChi2Short",          m_maxChi2Short      = 20.0  );  //  6.0 // short (3-hit) tracks are removed with this limit
  declareProperty( "ClearHits",             m_clearHits         = false );  // Clear hits if needed, for a rerun in same event

  declareProperty( "MaxZForRBeamCut",       m_maxZForRBeamCut   = 200.0 * Gaudi::Units::mm );  // 
  declareProperty( "MaxR2Beam",             m_maxR2Beam         =   1.0 * Gaudi::Units::mm2 ); // 1.0 // limit on squared track distance to the beam axis

  // Parameters for debugging
  declareProperty( "DebugToolName",         m_debugToolName     = ""    );
  declareProperty( "WantedKey",             m_wantedKey         = -100  );
  declareProperty( "TimingMeasurement",     m_doTiming          = false );
  declareProperty( "ClosestToBeamStateKalmanFit", m_stateClosestToBeamKalmanFit  = true ) ;
  declareProperty( "EndVeloStateKalmanFit",       m_stateEndVeloKalmanFit = false ) ;
  declareProperty( "AddFirstLastMeasurementStatesKalmanFit", m_addStateFirstLastMeasurementKalmanFit = false ) ;
}

//=============================================================================
// Destructor
//=============================================================================
PatPixelTracking::~PatPixelTracking() {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode PatPixelTracking::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;

  m_hitManager = tool<PatPixelHitManager>("PatPixelHitManager");

  m_debugTool   = 0;
  if ( "" != m_debugToolName ) m_debugTool = tool<IPatDebugTool>( m_debugToolName );

  if ( m_doTiming) {
    m_timerTool = tool<ISequencerTimerTool>( "SequencerTimerTool/Timer", this );
    m_timeTotal   = m_timerTool->addTimer( "Total" );
    m_timerTool->increaseIndent();
    m_timePrepare = m_timerTool->addTimer( "Prepare" );
    m_timePairs   = m_timerTool->addTimer( "Find by pairs" );
    m_timeFinal   = m_timerTool->addTimer( "Store tracks" );
    m_timerTool->decreaseIndent();
  }
#ifdef DEBUG_HISTO
  setHistoTopDir("VP/");
#endif
  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode PatPixelTracking::execute() {

  // printf("PatPixelTracking::execute() =>\n");

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;
  m_isDebug   = msgLevel( MSG::DEBUG );
  // m_isDebug = 1;

  if ( m_doTiming ){
    m_timerTool->start( m_timeTotal );
    m_timerTool->start( m_timePrepare );
  }

  LHCb::Tracks* outputTracks = new LHCb::Tracks();
  put( outputTracks, m_outputLocation );

  if ( m_clearHits ) m_hitManager->clearHits();
  m_hitManager->buildHits();                                        // import hits from VeloLite Clusters
  m_hitManager->sortByX();                                          // sort by X-pos. within each sensor for faster search

  if ( m_isDebug ) {
    for ( unsigned int sensorNb = m_hitManager->firstSensor(); m_hitManager->lastSensor() >= sensorNb; ++sensorNb ) {
      for ( PatPixelHits::const_iterator itH = m_hitManager->hits(sensorNb).begin();
            m_hitManager->hits(sensorNb).end() != itH; ++itH ) {
        printHit( *itH );
      }
    }
  }

  if ( 0 <= m_wantedKey ) {
    info() << "--- Looking for track " << m_wantedKey << endmsg;
    for ( unsigned int sensorNb = m_hitManager->firstSensor(); m_hitManager->lastSensor() >= sensorNb; ++sensorNb ) {
      for ( PatPixelHits::const_iterator itH = m_hitManager->hits(sensorNb).begin();
            m_hitManager->hits(sensorNb).end() != itH; ++itH ) {
        if ( matchKey( *itH ) ) printHit( *itH );
      }
    }
  }

  // m_hitManager->print();
  // printf(" m_hitManager->nbHits() => %d\n", m_hitManager->nbHits());

  if ( m_doTiming ) m_timerTool->stop( m_timePrepare );
  //==========================================================================
  // Search by finding a pair, then extrapolating
  //==========================================================================
  if ( m_doTiming ) m_timerTool->start( m_timePairs );
  searchByPair();                                                    // search by attempting to extrapolate every pair (withtn slope limits)
  if ( m_doTiming ) m_timerTool->stop( m_timePairs );
  //==========================================================================
  // Final storage of tracks
  //==========================================================================
  if ( m_doTiming ) m_timerTool->start( m_timeFinal );

  // m_hitManager->print();
#ifdef DEBUG_HISTO
  int nbHits = m_hitManager->nbHits();
  plot(nbHits,          "HitsPerEvent",   "PatPixelTracking: Number of hits per event",   0.0, 8000.0, 80);
  plot(m_tracks.size(), "TracksPerEvent", "PatPixelTracking: Number of tracks per event", 0.0,  800.0, 80);
  int nbHitsUsed = m_hitManager->nbHitsUsed();
  if(nbHits>0)
    plot((100.0*nbHitsUsed)/nbHits, "PercentUsedHitsPerEvent", "PatPixelTracking: Percent of hits assigned to tracks", 0.0, 100.0, 100);
  for(PatPixelTracks::const_iterator itT = m_tracks.begin(); itT != m_tracks.end(); ++itT)
  { if((*itT).size()<=3) continue;
    double minR, maxR;
    itT->getMinMaxRadius(minR, maxR);
    plot(minR, "MinHitRadiusPerTrack", "PatPixelTracking: Smallest hit radius [mm] per track (of 4 or more hits)", 0.0, 50.0, 100);
    plot(maxR, "MaxHitRadiusPerTrack", "PatPixelTracking: Largest hit radius [mm] per track (of 4 or more hits)",  0.0, 50.0, 100);
  }
#endif

  // for(PatPixelTracks::const_iterator itT = m_tracks.begin(); itT != m_tracks.end(); ++itT) (*itT).print();

  makeLHCbTracks( outputTracks );                                    // convert out tracks to LHCb tracks, as well: clears the track list !

  if ( m_doTiming ) {
    m_timerTool->stop( m_timeFinal );
    m_timerTool->stop( m_timeTotal );
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode PatPixelTracking::finalize() {

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Finalize" << endmsg;

  return GaudiAlgorithm::finalize();  // must be called after all other actions
}

//== Extend downstream, on both sides of the detector as soon as one hit is missed
void PatPixelTracking::trackDownstream(void)
{ 
  int extraStep = 2;                                              // scan every 2nd sensor to stay on the same side (right or left)
  int extraSensIdx = m_track.lastSensor()-extraStep;              // start two sensors behind the last one
  int nbMissed = 0;                                               // count sensors without hits found
  while ( extraSensIdx >= 0 )                                     // loop over sensors before the seed
  { PatPixelSensor* extra = m_hitManager->sensor( extraSensIdx ); // sensor we try to extrapolate to

    double tol     = m_extraTol;                                  // extrapolation tolerance
    double maxChi2 = m_maxChi2ToAdd;                              // maximum Chi2 for a hit we want to add
    bool added = addHitsOnSensor( extra, tol, maxChi2 );          // attempt to add new hits from this sensor with given tolarances

    if ( added )
    { nbMissed = 0; }                                             // if some hits added: reset missed hit counter
    else                                                          // if none found:
    { if(extraStep==2)                                            // is still every second sensor scan:
        added=addHitsOnSensor( m_hitManager->sensor(extraSensIdx+1), tol, maxChi2 ); // go back one sensor, the hit might be there.
      if(!added) nbMissed += extraStep;
      extraStep = 1; }                                            // switch to every sensor scan, not every 2nd
    if ( m_maxMissed < nbMissed ) break;
    extraSensIdx -= extraStep;
  }
}

void PatPixelTracking::trackUpstream(void)
{ 
  int extraStep = 1;
  int extraSensIdx = m_track.lastSensor() + extraStep;
  int nbMissed = 0;
  while ( extraSensIdx <= (int)(m_hitManager->lastSensor()) )
  { PatPixelSensor* extra = m_hitManager->sensor( extraSensIdx );

    // double tol     = m_extraTol;                                  // extrapolation tolerance
    // double maxChi2 = m_maxChi2ToAdd;                              // maximum Chi2 for a hit we want to add
    bool added = addHitsOnSensor( extra, m_extraTol, m_maxChi2ToAdd );

    if ( added ) { nbMissed = 0; }
    else { nbMissed += extraStep; }
    if ( m_maxMissed < nbMissed ) break;
    extraSensIdx += extraStep;
  }
}

//=========================================================================
//  Search starting with a pair of consecutive sensors.
//=========================================================================
void PatPixelTracking::searchByPair() {
  // printf("PatPixelTracking::searchByPair() =>\n");
  int lastSensor  = m_hitManager->lastSensor();                       // the very last sensor index (largest Z, most forward)
  int firstSensor = m_hitManager->firstSensor() + 2;                  // the one-before-last sensor index (almost smallest Z)
  for ( int sens0 = lastSensor; firstSensor <= sens0; sens0 -= 1 )    // loop over sensors, start with the last sensor
  { int sens1 = sens0 - 2;                                            // pick-up the "paired" sensor as one station backwards
    PatPixelSensor* sensor0 = m_hitManager->sensor( sens0 );          // more forward sensor (larger Z)
    PatPixelSensor* sensor1 = m_hitManager->sensor( sens1 );          // less forward sensor (smaller Z)
    double z0 = sensor0->z();                                         // Z's of the sensor pair
    double z1 = sensor1->z();
    double dz = z0-z1;
    // if(fabs(dz)>60.0) continue;                                      // does it make sense not to start on pairs very far apart ?
#ifdef DEBUG_HISTO
  plot(dz, "SeedPairDeltaZ", "Separation in Z [mm] between the seed pair sensors", -200.0, +200.0, 400);
#endif
    double dxMax = m_maxXSlope * fabs(dz);                          // limits on dX/dY from the slope limit
    double dyMax = m_maxYSlope * fabs(dz);

    PatPixelHits::const_iterator itH0;
    for ( itH0 = sensor0->hits().begin(); sensor0->hits().end() != itH0; ++itH0 )   // loop over hits in the first sensor (larger Z) in the pair
    { if ( (*itH0)->isUsed() ) continue;                                            // skip hits already assigned to tracks
      double x0  = (*itH0)->x();                                                    // hit X
      double y0  = (*itH0)->y();                                                    // hit Y
      double xMin = x0 - dxMax;                                                     // X-pos. limits for the other sensor
      double xMax = x0 + dxMax;

      if (  0 != m_debugTool && matchKey( *itH0 ) )                                 // print debug if enabled
      { info() << format( "s1%3d xMin%9.3f xMax%9.3f ", sens1, xMin, xMax );
        printHit ( *itH0,   "St0" ); }

      PatPixelHits::const_iterator itH1; // , first1 = sensor1->hits().begin();
      for (itH1 = sensor1->hits().begin(); sensor1->hits().end() != itH1; ++itH1 )   // loop over hits in the second sensor (smaller Z) in the pair
      { double x1 = (*itH1)->x();                                                    // note: hits are sorted accord. to their X
        if ( x1 < xMin ) continue; // { first1 = itH1+1; continue; }                 // skip hits below the X-pos. limit
        if ( x1 > xMax ) break;                                                      // stop search when above the X-pos. limit
        if ( (*itH1)->isUsed() ) continue;                                           // skip hits already assigned to tracks

        //== Check y compatibility...
        double y1  = (*itH1)->y();
        if ( fabs( y1 - y0 ) > dyMax ) continue;                                     // skip hits out of Y-pos. limit (based on max. slope)

        m_debug = m_isDebug;
        if ( 0 != m_debugTool )
        { if ( matchKey( *itH0 ) && matchKey( *itH1 ) ) m_debug = true;
          if ( m_debug )
          { info() << format( "s1%3d dxRel %7.3f dyRel %7.3f    ", 
                              sens1, (x1-xMin)/(xMax-xMin), fabs((*itH1)->y()-y0)/dyMax );
            printHit( *itH1 );
          }
        }

        m_track.set( *itH0, *itH1 );                           // make a track out of the two hits that pass slope criteria and use it as a seed

        // double zBeam  = m_track.zBeam();
        // if(fabs(zBeam)>200.0) continue;

/*
        //== Cut on R2Beam if needed : backward tracks, i.e zBeam > first hit
        if ( sensor0->z() < m_maxZForRBeamCut )                // if 1st sensor below the Z-limit
        { double zBeam  = m_track.zBeam();                     // where (which Z) the track passes closest to the beam
          if ( zBeam > sensor0->z() )                          // 
          { double r2Beam = m_track.r2AtZ( zBeam ) ;           // how close the track passes to the beam ?
            if ( m_debug ) info() << format( "  zbeam %9.2f r2 %7.3f", zBeam, r2Beam ) << endmsg;
            if ( r2Beam > m_maxR2Beam ) continue;
          }
        }
*/
        // printf("PatPixelTracking::searchByPair() Seed pair: zBeam=%+6.1f\n", m_track.zBeam() );
        // (*itH0)->print();
        // (*itH1)->print();

        //== Extend downstream, on both sides of the detector as soon as one hit is missed
        trackDownstream();
        trackUpstream();

        if ( m_track.hits().size() < 3 ) continue;

/*      this part is unclear why would it help ?
        //== Add compatible hits in sens0 and sens1.
        if ( (itH0+1) != sensor0->hits().end() )
        { if ( m_track.chi2( *(itH0+1) ) < m_maxChi2SameSensor )
          { ++itH0; m_track.addHit( *itH0 ); }
        }
        if ( (itH1+1) != sensor1->hits().end() )
        { if ( m_track.chi2( *(itH1+1) ) < m_maxChi2SameSensor )
          { ++itH1; m_track.addHit( *itH1 ); }
        }
*/
        // m_track.print();
        // printf(" Track[%3d]: %2d/%2d hists, Chi2=%3.1f zBeam=%+6.1f\n",
        //        m_tracks.size(), m_track.hits().size(), m_track.nbUnused(), m_track.chi2(), m_track.zBeam() );

        //== Final check: if only 3 hits, all should be unused and chi2 good.
        if ( m_track.hits().size() == 3 )
        { if ( !m_track.all3SensorsAreDifferent() )
          { if ( m_debug ) 
            { info() << "  -- reject, not 3 different sensors." << endmsg;
              printTrack( m_track ); }
            // printf("Rejected: 3 hits but not different sensors\n");
            continue;
          }
          if ( m_track.nbUnused() != 3 )
          { if ( m_debug )
            { info() << "  -- reject, only " << m_track.nbUnused() << " unused hits." << endmsg;
              printTrack( m_track ); }
            // printf("Rejected: 3 hits but some used by other tracks\n");
            continue;
          }
        } else
        { if ( m_track.nbUnused() < 0.5 * m_track.hits().size() )
          { if ( m_debug )
            { info() << "  -- reject, only " << m_track.nbUnused() << " unused hits." << endmsg;
              printTrack( m_track ); }
            // printf("Rejected: more than 3 hits but too many used by other tracks\n");
            continue;
          }
        }

        m_tracks.push_back( m_track );
        if ( m_debug ) {
          // info() << "=== Store track Nb " << m_tracks.size() << " chi2 " << m_track.chi2perDoF() << endmsg;
          info() << "=== Store track Nb " << m_tracks.size() << endmsg;
          printTrack( m_track );
        }
        if ( m_track.hits().size() > 3 ) {
          m_track.tagUsedHits();
          break;
        }
      } // itH1
    } // itH0
  } // sens0

  // printf("PatPixelTracking::searchByPair() => %3d tracks\n", m_tracks.size());
}

//=========================================================================
//  Convert the local tracks to LHCb tracks
//=========================================================================
void PatPixelTracking::makeLHCbTracks( LHCb::Tracks* outputTracks ) {
  // printf("PatPixelTracking::makeLHCbTracks() =>\n");
#ifdef DEBUG_HISTO
  int Fwd_Count=0; int Bwd_Count=0;
#endif
  unsigned int key = 0;
  for ( PatPixelTracks::iterator itT = m_tracks.begin(); m_tracks.end() != itT; ++itT ) {
    if ( (*itT).hits().size() == 3 && (*itT).nbUnused() != 3 ) continue;          // avoid 3-hit tracks with double-used hits ?
    LHCb::Track *newTrack = new LHCb::Track( key++ );
    newTrack->setType( LHCb::Track::Velo );
    newTrack->setHistory( LHCb::Track::PatFastVelo );
    newTrack->setPatRecStatus( LHCb::Track::PatRecIDs );
    if ( m_debug ) {
      // info() << "=== Store track Nb " << outputTracks->size() << " chi2 " << (*itT).chi2perDoF()<< endmsg;
      info() << "=== Store track Nb " << outputTracks->size() << endmsg;
      printTrack( *itT );
    }

    double zMax = -1.e9;

    for ( PatPixelHits::iterator itR = (*itT).hits().begin();
          (*itT).hits().end() != itR; ++itR ) {               // loop over hits
      newTrack->addToLhcbIDs( (*itR)->id() );
      if ( (*itR)->z() > zMax ) zMax = (*itR)->z();           // find highest Z for all hits of this track
    }

    //== Define backward as z closest to beam downstream of hits
    double zBeam = (*itT).zBeam();                            // Z where the track passes closest to the beam
    bool backward = zBeam > zMax;                             // decide: forward or backward track
    newTrack->setFlag( LHCb::Track::Backward, backward );

    // get the state from the straight line fit
    LHCb::State state;
    state.setLocation( LHCb::State::ClosestToBeam );
    state.setState( (*itT).state( zBeam ) );
    state.setCovariance( (*itT).covariance( zBeam ) );

    // parameters for kalmanfit scattering. calibrated on MC, shamelessly hardcoded:
    const double tx = state.tx() ; const double ty = state.ty() ;
    const double scat2 = 1e-8 + 7e-6*(tx*tx+ty*ty) ;

    // the logic is a bit messy in the following, so I hope we got all cases right
    if( m_stateClosestToBeamKalmanFit || m_addStateFirstLastMeasurementKalmanFit) {
      // run a K-filter with scattering to improve IP resolution
      LHCb::State upstreamstate ;
      (*itT).fitKalman( upstreamstate, backward ? 1 : -1 , scat2 ) ;
      // add this state as state at first measurement if requested
      if( m_addStateFirstLastMeasurementKalmanFit ) {
	upstreamstate.setLocation( LHCb::State::FirstMeasurement ) ;
	newTrack->addToStates( upstreamstate );
      }
      // transport the state to the closestToBeam position
      if( m_stateClosestToBeamKalmanFit ) {
	upstreamstate.setLocation( LHCb::State::ClosestToBeam );
	upstreamstate.linearTransportTo( zBeam ) ;
	newTrack->addToStates( upstreamstate );
      }
    }
    if(!m_stateClosestToBeamKalmanFit) {
      newTrack->addToStates( state );
    }
    
    // set state at last measurement, if requested
    if( (!backward && m_stateEndVeloKalmanFit) || m_addStateFirstLastMeasurementKalmanFit ) {
      LHCb::State downstreamstate ;
      (*itT).fitKalman( downstreamstate, backward ? -1 : +1 , scat2 ) ;
      if( m_addStateFirstLastMeasurementKalmanFit ) {
	downstreamstate.setLocation( LHCb::State::LastMeasurement ) ;
	newTrack->addToStates( downstreamstate );
      }
      if( m_stateEndVeloKalmanFit ) {
	state = downstreamstate ;
      }
    } 
    
    // add state at end of velo
    if( !backward ) {
      state.setLocation( LHCb::State::EndVelo ) ;
      state.linearTransportTo( StateParameters::ZEndVelo )  ;
      newTrack->addToStates( state ) ;
    }

    // set the chi2/dof
    // newTrack->setNDoF(2*((*itT).hits().size()-2)); newTrack->setChi2PerDoF( (*itT).chi2perDoF() );
    outputTracks->insert( newTrack );

    // printf(" pseudoRapidity=%3.1f Chi2/DoF=%3.1f nDoF=%d\n",
    //  newTrack->pseudoRapidity(), newTrack->chi2PerDoF(), newTrack->nDoF() );

#ifdef DEBUG_HISTO
    int HitsPerTrack = (*itT).hits().size();
    if(backward)
    {
    plot(HitsPerTrack,                               "Bwd_HitsPerTrack", "PatPixelTracking: Number of hits per backward track",
         0.5, 40.5, 40);
    plot(newTrack->chi2PerDoF(),                     "Bwd_Chi2PerTrack", "PatPixelTracking: Chi2/DoF of backward tracks",
         0.0, 10.0, 50);
    plot(newTrack->pseudoRapidity(),                 "Bwd_EtaOfTracks", "PatPixelTracking: pseudoRapidity of backward tracks",
         1.0, 6.0, 50);
    plot(newTrack->phi()*(180.0/M_PI),               "Bwd_PhiOfTracks", "PatPixelTracking: Phi-angle of backward tracks",
         -180.0, 180.0, 60);
    plot2D(newTrack->pseudoRapidity(), HitsPerTrack, "Bwd_HitsPerTrackVsEta", "PatPixelTracking: hits/track vs pseudoRapidity of backward tracks",
           1.0, 6.0, 0.5, 15.5, 50, 15);
    plot2D(newTrack->pseudoRapidity(), newTrack->chi2PerDoF(), "Bwd_Chi2VsEta", "PatPixelTracking: Chi2/DoF vs pseudoRapidity of backward tracks",
           1.0, 6.0, 0.0, 10.0, 50, 20);
    plot2D(HitsPerTrack, newTrack->chi2PerDoF(),     "Bwd_Chi2VsHitsPerTrack", "PatPixelTracking: Chi2/DoF vs hits/backward track",
           0.5, 15.5, 0.0, 10.0, 15, 20);
    Bwd_Count++;
    }
    else
    {
    plot(HitsPerTrack,                               "Fwd_HitsPerTrack", "PatPixelTracking: Number of hits per forward track",
         0.5, 40.5, 40);
    plot(newTrack->chi2PerDoF(),                     "Fwd_Chi2PerTrack", "PatPixelTracking: Chi2/DoF of forward tracks",
         0.0, 10.0, 50);
    plot(newTrack->pseudoRapidity(),                 "Fwd_EtaOfTracks", "PatPixelTracking: pseudoRapidity of forward tracks",
         1.0, 6.0, 50);
    plot(newTrack->phi()*(180.0/M_PI),               "Fwd_PhiOfTracks", "PatPixelTracking: Phi-angle of forward tracks",
         -180.0, 180.0, 60);
    plot2D(newTrack->pseudoRapidity(), HitsPerTrack, "Fwd_HitsPerTrackVsEta", "PatPixelTracking: hits/track vs pseudoRapidity of forward tracks",
           1.0, 6.0, 0.5, 15.5, 50, 15);
    plot2D(newTrack->pseudoRapidity(), newTrack->chi2PerDoF(), "Fwd_Chi2VsEta", "PatPixelTracking: Chi2/DoF vs pseudoRapidity of forward tracks",
           1.0, 6.0, 0.0, 10.0, 50, 20);
    plot2D(HitsPerTrack, newTrack->chi2PerDoF(),     "Fwd_Chi2VsHitsPerTrack", "PatPixelTracking: Chi2/DoF vs hits/forward track",
           0.5, 15.5, 0.0, 10.0, 15, 20);
    Fwd_Count++;
    }
#endif // of DEBUG_HISTO

  }

#ifdef DEBUG_HISTO
  plot(Fwd_Count, "Fwd_TracksPerEvent", "PatPixelTracking: Number of forward tracks per event", 0.0, 400.0, 40);
  plot(Bwd_Count, "Bwd_TracksPerEvent", "PatPixelTracking: Number of backward tracks per event", 0.0, 400.0, 40);
#endif

  m_tracks.clear();
}

/*
//=========================================================================
//  Remove the worst hit until all chi2 are good.
//=========================================================================
void PatPixelTracking::removeWorstHit ( double maxChi2 ) {
  double topChi2 = 1.e9;
  while( topChi2 > maxChi2 ) {
    topChi2 = 0.0;                              // find the hit with highest Chi2 (but stop anyway when reaching the limit)
    PatPixelHit* worst = NULL;
    for (  PatPixelHits::const_iterator itH = m_track.hits().begin(); m_track.hits().end() != itH; ++itH ) {
      double myChi2 = m_track.chi2( *itH );
      if ( myChi2 > topChi2 )
      { topChi2 = myChi2; worst   = *itH; }
#ifdef DEBUG_HISTO
  plot( myChi2 , "HitChi2", "PatPixelTracking: Hit chi^2 in tracks", 1.0, 21.0, 100);
#endif
    }
    if ( topChi2 > maxChi2 )                    // if highest hit chi2 above limit
    { m_track.removeHit( worst );               // remove it
      // printf(" - [%+6.1f]: [%+5.1f,%+5.1f] chi2=%5.1f <%5.1f>\n", worst->z(), worst->x(), worst->y(), topChi2, m_track.chi2perDoF() );
      if ( m_debug )
      { info() << "After worst hit removal" << endmsg;
        printTrack( m_track ); }
    }  
  }
}
*/

//=========================================================================
//  Add hits from the specified sensor to the track
//=========================================================================
bool PatPixelTracking::addHitsOnSensor( PatPixelSensor* sensor, double xTol, double maxChi2 )
{ if ( 0 == sensor->hits().size() ) return false;                  // if no hits on this sensor: return (no hits added)
  // printf("PatPixelTracking::addHitsOnSensor(%+6.1f)\n", sensor->z() );
  // double xGuess = m_track.xAtZ( sensor->z() ) - xTol;              // upper X-limit on hit search
                                                                   // Note: sensor->z() is an average Z of the silicon ladders
                                                                   // thus is not exactly equal to the Z of the ladderhit by the track extrapolation

  double xGuess,yGuess,dist;
  if( m_track.extrapolate(xGuess,yGuess,dist, sensor->z() )<=0 ) return false;    // new: extrapolate with only the edge two hits.
  xGuess-=xTol;

  if ( sensor->hits().back()->x() < xGuess ) return false;         // if the first hit is below this limit: return (no hits added)
                                                                   // hits on a sensor are already sorted in X to speed up the search
  PatPixelHits::const_iterator itStart = sensor->hits().begin();   // binary search through the hits (they are sorted in X)
  unsigned int step = sensor->hits().size();
  while ( 2 < step )                                               // quick skip of hits that are above the X-limit
  { step = step/2;
    if ( (*(itStart+step))->x() < xGuess ) itStart += step;
  }

  int Count=0;
  double maxScatter=0.004;
  double bestScatter=maxScatter;
  PatPixelHit *bestHit = 0; // double bestChi2 = maxChi2;             // find the hit that matches best
  for ( PatPixelHits::const_iterator itH = itStart; sensor->hits().end() != itH; ++itH )
  { // double xPred = m_track.xAtHit( *itH );                         // predict X at Z of this hit
    double xPred,yPred,dist;                                        // new: extrapolate with two edge hits only
    if( m_track.extrapolate(xPred,yPred,dist, *itH )<=0 ) continue;

    if ( m_debug ) printHitOnTrack( *itH );
#ifdef DEBUG_HISTO
  plot( ((*itH)->x()-xPred)/xTol , "HitExtraErrPerTol", "PatPixelTracking: Hit X extrapolation error / tolerance", -4.0, +4.0, 400);
/*
  plot2D( (*itH)->z()-m_track.meanZ(), (*itH)->x()-xPred,
           "HitExtraErrVsZdist", "PatPixelTracking: Hit X [mm] extrapolation error vs extr. Z [mm] distance",
          -500.0, +500.0, -5.0, +5.0, 100, 200);
*/
#endif
    if ( (*itH)->x() + xTol < xPred ) continue;                   // if hit's X is above prediction+tolerance, keep looking
    if ( (*itH)->x() - xTol > xPred ) break;                      // if hit's X is below prediction-tolerance, stop the search
    double dx = xPred-(*itH)->x();
    double dy = yPred-(*itH)->y();
    double scatter = sqrt(dx*dx+dy*dy)/dist;
    if(scatter<bestScatter) { bestHit = *itH; bestScatter=scatter; }
    if(scatter<maxScatter) Count++;
#ifdef DEBUG_HISTO
    plot ( scatter, "HitScatter", "PatPixelTracking: hit scatter [rad]",
         0.0, 0.5, 500);
#endif
    // double chi2 = m_track.chi2(*itH); 
    // if(chi2<bestChi2) { bestHit = *itH; bestChi2=chi2; }
    // if(chi2<maxChi2) Count++;
  }
#ifdef DEBUG_HISTO
  plot(Count, "HitExtraCount", "Number of hits within the extrapolation window with chi2 within limits", 0.0, 10.0, 10);
#endif
  if(bestHit==0)
  { // printf(" + [%+6.1f]: [%+5.1f,%+5.1f] ---\n", sensor->z(), xGuess, yGuess );
    return false; }

#ifdef DEBUG_HISTO
  plot ( bestScatter, "HitBestScatter", "PatPixelTracking: best hit scatter [rad]",
         0.0, 0.1, 100);
#endif

  if ( m_debug ) printHitOnTrack( bestHit, false );
  m_track.addHit(bestHit);
  // printf(" + [%+6.1f]: [%+5.1f,%+5.1f] %4.2f:%5.3f <%5.1f>\n",
  //       sensor->z(), bestHit->x(), bestHit->y(), m_track.distance(bestHit), bestScatter, m_track.chi2perDoF() );

  return true;
}

//=========================================================================
//  Debug the content of a hit
//=========================================================================
void PatPixelTracking::printHit ( const PatPixelHit* hit, std::string title )
{ if ( "" != title ) info() << title;
  info() << format( " sensor%3d x%8.3f y%8.3f z%8.2f used%2d",
                    hit->sensor(), hit->x(), hit->y(), hit->z(), hit->isUsed() );
  LHCb::LHCbID myId =  hit->id();
  if ( 0 != m_debugTool ) {
    info() << " MC: ";
    m_debugTool->printKey( info(), myId );
    if ( matchKey( hit ) ) info() << " ***";
  }
  info() << endreq;
}

//=========================================================================
//  Print a track, with distance and chi2
//=========================================================================
void PatPixelTracking::printTrack ( PatPixelTrack& track ) {
  for (  PatPixelHits::const_iterator itH = track.hits().begin(); track.hits().end() != itH; ++itH )
  { // info() << format( "Dist%8.3f chi%7.3f ", track.distance( *itH ), track.chi2( *itH ) );
    printHit( *itH );
  }
}

//=========================================================================
//  Print a hit on a track, with its distance.
//=========================================================================
void PatPixelTracking::printHitOnTrack ( PatPixelHit* hit, bool ifMatch ) {
  bool isMatching = matchKey( hit );
  isMatching = (isMatching && ifMatch) || (!isMatching && !ifMatch );
  if ( isMatching ) {
    // info() << format( "  sens %3d dx  %9.3f chi2 %8.3f ", hit->sensor(), m_track.xAtHit( hit )-hit->x(), m_track.chi2( hit ) );
    printHit( hit, "   " );
  }
}
//=============================================================================
