
//-----------------------------------------------------------------------------
/** @file RichPhotonCreatorBase.cpp
 *
 *  Implementation file for tool base class : Rich::Rec::PhotonCreatorBase
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   20/05/2005
 */
//-----------------------------------------------------------------------------

// local
#include "RichRecBase/RichPhotonCreatorBase.h"

// boost
#include "boost/format.hpp"
#include "boost/assign/list_of.hpp"

//-----------------------------------------------------------------------------

// RICH
namespace Rich
{
  namespace Rec
  {

    // Standard constructor, initializes variables
    PhotonCreatorBase::PhotonCreatorBase( const std::string& type,
                                          const std::string& name,
                                          const IInterface* parent )
      : RichRecToolBase         ( type, name, parent ),
        m_hasBeenCalled         ( false ),
        m_photonPredictor       ( NULL ),
        m_photonSignal          ( NULL ),
        m_ckAngle               ( NULL ),
        m_ckRes                 ( NULL ),
        m_richPartProp          ( NULL ),
        m_Nevts                 ( 0    ),
        m_photons               ( NULL ),
        m_photCount             ( Rich::NRadiatorTypes, 0 ),
        m_photCountLast         ( Rich::NRadiatorTypes, 0 )
    {
      using namespace boost::assign;

      // Define the interface
      declareInterface<IPhotonCreator>(this);

      // defaults                 Aero    R1Gas   R2Gas
      m_minCKtheta     = list_of (0.075) (0.005) (0.005) ;
      m_maxCKtheta     = list_of (0.320) (0.075) (0.035) ;
      m_minPhotonProb  = list_of (1e-15) (1e-15) (1e-15) ;
      if ( contextContains("HLT") )
      {
        m_nSigma = list_of (3.5) (2.8) (3.0) ;

      }
      else // Offline settings
      {
        m_nSigma = list_of (3.8) (3.3) (3.3) ;
      }

      // set properties
      declareProperty( "RichRecPhotonLocation",
                       m_richRecPhotonLocation = contextSpecificTES(LHCb::RichRecPhotonLocation::Default),
                       "The TES location for the transient RichRecPhoton objects" );
      declareProperty( "DoBookKeeping", m_bookKeep = true,
                       "Turn on/off the book keeping of which pixels have been made" );
      declareProperty( "PhotonPredictor", m_photPredName = "RichPhotonPredictor",
                       "The photon predictor(pre-selection) tool name" );
      declareProperty( "MinAllowedCherenkovTheta", m_minCKtheta,
                       "The minimum allowed CK theta values for each radiator (Aero/R1Gas/R2Gas)" );
      declareProperty( "MaxAllowedCherenkovTheta", m_maxCKtheta,
                       "The maximum allowed CK theta values for each radiator (Aero/R1Gas/R2Gas)" );
      declareProperty( "MinPhotonProbability", m_minPhotonProb,
                       "The minimum allowed photon probability values for each radiator (Aero/R1Gas/R2Gas)" );
      declareProperty( "NSigma", m_nSigma,
                       "The CK theta # sigma selection range for each radiator (Aero/R1Gas/R2Gas)");
      declareProperty( "MaxPhotons", m_maxPhotons = 9999999,
                       "The maximum number of photon candidates to allow per event" );

    }

    StatusCode PhotonCreatorBase::initialize()
    {
      // base class initilize
      const StatusCode sc = RichRecToolBase::initialize();
      if ( sc.isFailure() ) { return sc; }

      if ( m_richRecPhotonLocation.empty() )
      {
        return Error( "RichRecPhoton location is undefined. Please set" );
      }
      if ( msgLevel(MSG::DEBUG) )
      {
        debug() << "RichRecPhoton location : " << m_richRecPhotonLocation << endmsg;
      }

      // get tools
      acquireTool( m_photPredName, "Predictor", m_photonPredictor, this );
      acquireTool( "RichPhotonSignal", m_photonSignal );
      acquireTool( "RichCherenkovAngle",  m_ckAngle   );
      acquireTool( "RichCherenkovResolution", m_ckRes );
      acquireTool( "RichParticleProperties",  m_richPartProp );

      // Get the HPD panels
      DetectorElement* deRich1 = getDet<DetectorElement>(DeRichLocations::Rich1);
      DetectorElement* deRich2 = getDet<DetectorElement>(DeRichLocations::Rich2);

      if ( deRich1->exists("HPDPanelDetElemLocations") )
      {
        const std::vector<std::string> r1PanelLoc = deRich1->paramVect<std::string>("HPDPanelDetElemLocations");
        m_hpdPanels[Rich::Rich1][Rich::top]    = getDet<DeRichHPDPanel>(r1PanelLoc[0]);
        m_hpdPanels[Rich::Rich1][Rich::bottom] = getDet<DeRichHPDPanel>(r1PanelLoc[1]);
        const std::vector<std::string> r2PanelLoc = deRich2->paramVect<std::string>("HPDPanelDetElemLocations");
        m_hpdPanels[Rich::Rich2][Rich::left]   = getDet<DeRichHPDPanel>(r2PanelLoc[0]);
        m_hpdPanels[Rich::Rich2][Rich::right]  = getDet<DeRichHPDPanel>(r2PanelLoc[1]);
      }
      else
      {
        m_hpdPanels[Rich::Rich1][Rich::top]    = getDet<DeRichHPDPanel>(DeRichLocations::Rich1TopPanel);
        m_hpdPanels[Rich::Rich1][Rich::bottom] = getDet<DeRichHPDPanel>(DeRichLocations::Rich1BottomPanel);
        m_hpdPanels[Rich::Rich2][Rich::left]   = getDet<DeRichHPDPanel>(DeRichLocations::Rich2LeftPanel);
        m_hpdPanels[Rich::Rich2][Rich::right]  = getDet<DeRichHPDPanel>(DeRichLocations::Rich2RightPanel);
      }

      // Setup incident services
      incSvc()->addListener( this, IncidentType::BeginEvent );
      incSvc()->addListener( this, IncidentType::EndEvent   );

      // info printout
      for ( int rad = 0; rad < Rich::NRadiatorTypes; ++rad )
      {
        std::string trad = Rich::text((Rich::RadiatorType)rad);
        trad.resize(8,' ');
        info() << trad << " : CK theta range " << boost::format("%5.3f") % m_minCKtheta[rad]
               << " -> " << boost::format("%5.3f") % m_maxCKtheta[rad]
               << " rad : Tol. " << boost::format("%5.3f") % m_nSigma[rad] << " sigma "
               << endmsg;
      }

      m_pidTypes = m_richPartProp->particleTypes();
      info() << "Particle types considered = " << m_pidTypes << endmsg;

      info() << "Maximum number of photon candidates per event = " << m_maxPhotons << endmsg;

      return sc;
    }

    StatusCode PhotonCreatorBase::finalize()
    {
      // print stats
      printStats();

      // base class finalize
      return RichRecToolBase::finalize();
    }

    // Method that handles various Gaudi "software events"
    void PhotonCreatorBase::handle ( const Incident & incident )
    {
      // Update prior to start of event. Used to re-initialise data containers
      if ( IncidentType::BeginEvent == incident.type() )
      {
        InitNewEvent();
      }
      // Debug printout at the end of each event
      else if ( IncidentType::EndEvent == incident.type() )
      {
        FinishEvent();
      }
    }

    void PhotonCreatorBase::printStats() const
    {

      if ( nEvents() > 0
           && !( m_photCount[Rich::Aerogel]  == 0 &&
                 m_photCount[Rich::Rich1Gas] == 0 &&
                 m_photCount[Rich::Rich2Gas] == 0  ) )
      {

        // statistical tool
        const StatDivFunctor occ("%10.2f +-%7.2f");

        // Print out final stats
        info() << "=================================================================" << endmsg
               << "  Photon candidate summary : " << nEvents() << " events :-" << endmsg
               << "    Aerogel   : "
               << occ(m_photCount[Rich::Aerogel],nEvents())  << "  photons/event" << endmsg
               << "    Rich1Gas  : "
               << occ(m_photCount[Rich::Rich1Gas],nEvents()) << "  photons/event" << endmsg
               << "    Rich2Gas  : "
               << occ(m_photCount[Rich::Rich2Gas],nEvents()) << "  photons/event" << endmsg
               << "=================================================================" << endmsg;

      }
      else
      {
        Warning( "No photon candidates produced", StatusCode::SUCCESS );
      }

    }

    StatusCode PhotonCreatorBase::reconstructPhotons() const
    {

      // Check for event aborts. If already aborted stop here
      if ( !procStatus()->aborted() )
      {

        if ( msgLevel(MSG::DEBUG) )
        {
          debug() << "Found " << trackCreator()->richTracks()->size()
                  << " RichRecTracks and " << pixelCreator()->richPixels()->size()
                  << " RichRecPixels" << endmsg;
        }
        if ( !trackCreator()->richTracks()->empty() &&
             !pixelCreator()->richPixels()->empty() )
        {

          // make a rough guess at a size to reserve based on number of pixels
          if ( richPhotons()->empty() )
            richPhotons()->reserve( ( trackCreator()->richTracks()->size() *
                                      pixelCreator()->richPixels()->size() ) / 20 );

          // Iterate over all tracks
          bool abortPhotonReco(false);
          for ( LHCb::RichRecTracks::const_iterator iTrack =
                  trackCreator()->richTracks()->begin();
                iTrack != trackCreator()->richTracks()->end();
                ++iTrack )
          {
            LHCb::RichRecTrack * track = *iTrack;

            if ( !track->inUse() ) continue; // skip tracks not "on"
            if ( !track->allPhotonsDone() )
            {

              if ( msgLevel(MSG::VERBOSE) )
              {
                verbose() << "Trying track " << track->key() << endmsg
                          << " -> Found " << track->richRecSegments().size()
                          << " RichRecSegments" << endmsg;
              }

              // Iterate over segments
              for ( LHCb::RichRecTrack::Segments::const_iterator iSegment =
                      track->richRecSegments().begin();
                    iSegment != track->richRecSegments().end();
                    ++iSegment)
              {
                LHCb::RichRecSegment * segment = *iSegment;

                //if ( msgLevel(MSG::VERBOSE) )
                //{
                // verbose() << " -> Trying segment " << segment->key() << " "
                //           << segment->trackSegment().radiator() << endmsg;
                //}

                if ( !segment->allPhotonsDone() )
                {
                  // search all hits in associated RICH/panel

                  // only make photons if reco has not been aborted
                  if ( !abortPhotonReco )
                  {

                    // Which Rich
                    const Rich::DetectorType rich = segment->trackSegment().rich();

                    // get appropriate pixel range
                    const bool has1 = segment->hasPhotonsIn(Rich::top);
                    const bool has2 = segment->hasPhotonsIn(Rich::bottom);
                    IPixelCreator::PixelRange range = ( has1 && has2 ?
                                                        pixelCreator()->range(rich) :
                                                        has1 ? pixelCreator()->range(rich,Rich::top) :
                                                        pixelCreator()->range(rich,Rich::bottom) );
                    for ( IPixelCreator::PixelRange::const_iterator iPixel = range.begin();
                          iPixel != range.end(); ++iPixel )
                    {
                      //if ( msgLevel(MSG::VERBOSE) )
                      //{
                      //  verbose() << " -> Trying pixel " << (*iPixel)->key() << endmsg;
                      //}
                      reconstructPhoton( segment, *iPixel );
                    } // pixel loop

                  } // reco not aborted

                  // check on size of photon container
                  abortPhotonReco = ( richPhotons()->size() > m_maxPhotons );

                  segment->setAllPhotonsDone(true);

                } // segment not already done

              } // segment loop

              track->setAllPhotonsDone(true);

            } // track not already done

          } // track loop

          if ( abortPhotonReco )
          {
            procStatus()->addAlgorithmStatus( name(), "RICH", "ReachedRichPhotonLimit",
                                              Rich::Rec::ReachedRichPhotonLimit, true );
            std::ostringstream mess;
            mess << "Number of photon candidates exceeds maximum of "
                 << m_maxPhotons << " -> Processing aborted";
            Warning( mess.str(), StatusCode::SUCCESS, 0 ).ignore();
            richPhotons()->clear();
            deleteAllCrossReferences();
          }

        } // have tracks and pixels

      } // ProcStat OK

      return StatusCode::SUCCESS;
    }

    LHCb::RichRecPhoton *
    PhotonCreatorBase::checkForExistingPhoton( LHCb::RichRecSegment * segment,
                                               LHCb::RichRecPixel * pixel ) const
    {
      // Form the key for this photon
      const Rich::Rec::PhotonKey photonKey( pixel->key(), segment->key() );

      // Try and find the photon
      return richPhotons()->object( photonKey );
    }

    LHCb::RichRecPhoton*
    PhotonCreatorBase::reconstructPhoton( LHCb::RichRecSegment * segment,
                                          LHCb::RichRecPixel * pixel ) const
    {
      if ( msgLevel(MSG::VERBOSE) )
      {
        verbose() << "Trying photon reco. with segment " << segment->key()
                  << " and pixel " << pixel->key() << " " << pixel->hpdPixelCluster()
                  << endmsg;
      }

      // check photon is possible before proceeding
      if ( !m_photonPredictor->photonPossible(segment, pixel) )
      {
        if ( msgLevel(MSG::VERBOSE) )
        {
          verbose() << "   -> FAILED predictor check -> reject" << endmsg;
        }
        return NULL;
      }
      else if (  msgLevel(MSG::VERBOSE) )
      {
        verbose() << "   -> PASSED predictor check" << endmsg;
      }

      // flag this tool as having been called
      m_hasBeenCalled = true;

      // Form the key for this photon
      const PhotonKey photonKey( pixel->key(), segment->key() );

      // See if this photon already exists
      if ( bookKeep() && m_photonDone[ photonKey ] )
      {
        // return pre-made photon
        return static_cast<LHCb::RichRecPhoton*>( richPhotons()->object(photonKey) );
      }
      else
      {
        // return brand new photon
        return buildPhoton( segment, pixel, photonKey );
      }
    }

    LHCb::RichRecPhotons * PhotonCreatorBase::richPhotons() const
    {

      // is this the first call this event ?
      if ( !m_photons )
      {

        if ( !exist<LHCb::RichRecPhotons>(m_richRecPhotonLocation) )
        {

          // Reinitialise the Photon Container
          m_photons = new LHCb::RichRecPhotons();

          // Register new RichRecPhoton container to Gaudi data store
          put( m_photons, m_richRecPhotonLocation );

        }
        else
        {

          // get photons from TES
          m_photons = get<LHCb::RichRecPhotons>(m_richRecPhotonLocation);
          if ( msgLevel(MSG::DEBUG) )
          {
            debug() << "Found " << m_photons->size()
                    << " pre-existing RichRecPhotons in TES at "
                    << m_richRecPhotonLocation << endmsg;
          }

          // Remake local photon reference map
          if ( bookKeep() )
          {
            for ( LHCb::RichRecPhotons::const_iterator iPhoton = m_photons->begin();
                  iPhoton != m_photons->end();
                  ++iPhoton )
            {
              m_photonDone[ (*iPhoton)->key() ] = true;
            }
          }

        }
      }

      return m_photons;
    }

    const LHCb::RichRecTrack::Photons &
    PhotonCreatorBase::reconstructPhotons( LHCb::RichRecTrack * track ) const
    {
      if ( !track->allPhotonsDone() && track->inUse() )
      {
        debug() << "Reconstructing all photons for track " << track->key() << endmsg;

        // Iterate over segments
        for ( LHCb::RichRecTrack::Segments::iterator segment =
                track->richRecSegments().begin();
              segment != track->richRecSegments().end(); ++segment )
        {
          reconstructPhotons( *segment );
        }

        track->setAllPhotonsDone(true);
      }

      return track->richRecPhotons();
    }

    const LHCb::RichRecSegment::Photons &
    PhotonCreatorBase::reconstructPhotons( LHCb::RichRecSegment * segment ) const
    {
      if ( !segment->allPhotonsDone() && segment->richRecTrack()->inUse() )
      {
        debug() << "Reconstructing all photons for segment " << segment->key() << endmsg;

        // Which Rich
        const Rich::DetectorType rich = segment->trackSegment().rich();

        // get appropriate pixel range
        const bool has1 = segment->hasPhotonsIn(Rich::top);
        const bool has2 = segment->hasPhotonsIn(Rich::bottom);
        IPixelCreator::PixelRange range = ( has1 && has2 ?
                                            pixelCreator()->range(rich) :
                                            has1 ? pixelCreator()->range(rich,Rich::top) :
                                            pixelCreator()->range(rich,Rich::bottom) );
        debug() << " -> Found " << range.size() << " pixels" << endmsg;
        for ( IPixelCreator::PixelRange::const_iterator iPixel = range.begin();
              iPixel != range.end(); ++iPixel )
        {
          reconstructPhoton( segment, *iPixel );
        }

        segment->setAllPhotonsDone(true);
      }

      return segment->richRecPhotons();
    }

    const LHCb::RichRecPixel::Photons &
    PhotonCreatorBase::reconstructPhotons( LHCb::RichRecPixel * pixel ) const
    {
      // Iterate over tracks
      for ( LHCb::RichRecTracks::iterator track =
              trackCreator()->richTracks()->begin();
            track != trackCreator()->richTracks()->end();
            ++track )
      {
        if ( !(*track)->inUse() ) continue;
        reconstructPhotons( *track, pixel );
      }

      return pixel->richRecPhotons();
    }

    // Note to self. Need to review what this method passes back
    LHCb::RichRecTrack::Photons
    PhotonCreatorBase::reconstructPhotons( LHCb::RichRecTrack * track,
                                           LHCb::RichRecPixel * pixel ) const
    {
      LHCb::RichRecTrack::Photons photons;

      // Iterate over segments
      for ( LHCb::RichRecTrack::Segments::iterator segment =
              track->richRecSegments().begin();
            segment != track->richRecSegments().end();
            ++segment )
      {
        LHCb::RichRecPhoton * photon = reconstructPhoton( *segment, pixel );
        if ( photon ) photons.push_back( photon );
      }

      return photons;
    }

    void PhotonCreatorBase::buildCrossReferences( LHCb::RichRecPhoton * photon ) const
    {

      // Pointers to the segment and pixel
      LHCb::RichRecSegment * segment = photon->richRecSegment();
      LHCb::RichRecPixel * pixel     = photon->richRecPixel();

      // Add this pixel to the segment list of pixels that have a reconstructed photon
      segment->addToRichRecPixels( pixel );

      // Add this photon to the segment list of reconstructed photons
      segment->addToRichRecPhotons( photon );

      // Add this photon to the track list of reconstructed photons
      segment->richRecTrack()->addToRichRecPhotons( photon );

      // Add this photon to the pixel list of reconstructed photons
      pixel->addToRichRecPhotons( photon );

      // Add the track to the pixel list of associated tracks (those with valid photons)
      pixel->addToRichRecTracks( segment->richRecTrack() );

      // Add pixel to track list. Need to check if not already there for RICH1
      if ( Rich::Rich1 == segment->trackSegment().rich() )
      {
        LHCb::RichRecTrack::Pixels & tkPixs = segment->richRecTrack()->richRecPixels();
        LHCb::RichRecTrack::Pixels::iterator iPix = std::find( tkPixs.begin(), tkPixs.end(), pixel );
        if ( tkPixs.end() == iPix ) segment->richRecTrack()->addToRichRecPixels( pixel );
      }
      else // RICH2 - only one radiator type so no need to test
      {
        segment->richRecTrack()->addToRichRecPixels( pixel );
      }

    }

    void PhotonCreatorBase::deleteAllCrossReferences() const
    {
      // loop over tracks
      for ( LHCb::RichRecTracks::const_iterator iTk = richTracks()->begin();
            iTk != richTracks()->end(); ++iTk )
      {
        (*iTk)->setRichRecPixels  ( LHCb::RichRecTrack::Pixels()    );
        (*iTk)->setRichRecPhotons ( LHCb::RichRecTrack::Photons()   );
      }
      // loop over segments
      for ( LHCb::RichRecSegments::const_iterator iSeg = richSegments()->begin();
            iSeg != richSegments()->end(); ++iSeg )
      {
        (*iSeg)->setRichRecPixels  ( LHCb::RichRecSegment::Pixels()  );

        (*iSeg)->setRichRecPhotons ( LHCb::RichRecSegment::Photons() );

      }
      // loop over pixels
      for ( LHCb::RichRecPixels::const_iterator iPix = richPixels()->begin();
            iPix != richPixels()->end(); ++iPix )
      {
        (*iPix)->richRecPhotons().clear();
        (*iPix)->richRecTracks().clear();
      }
    }

    bool
    PhotonCreatorBase::checkAngleInRange( LHCb::RichRecSegment * segment,
                                          const double ckTheta ) const
    {
      bool ok = false;

      // Just check overall absolute min - max range
      if ( ckTheta > absMinCKTheta(segment) && ckTheta < absMaxCKTheta(segment) )
      {
        // Finer grained check, to be within tolerence of any mass hypothesis
        for ( Rich::Particles::const_iterator hypo = m_pidTypes.begin();
              hypo != m_pidTypes.end(); ++hypo )
        {
          const double tmpT = m_ckAngle->avgCherenkovTheta( segment, *hypo );
          ok = ( fabs(tmpT-ckTheta) < ckSearchRange(segment,*hypo) );
          if ( ok ) break;
        }

      }
      if ( !ok && msgLevel(MSG::VERBOSE) )
      {
        verbose() << "    -> photon FAILED checkAngleInRange test" << endmsg;
      }
      return ok;
    }

    bool PhotonCreatorBase::checkPhotonProb( LHCb::RichRecPhoton * photon ) const
    {
      // check photon has significant probability to be signal for any
      // hypothesis. If not then reject
      bool keepPhoton = false;
      for ( Rich::Particles::const_iterator hypo = m_pidTypes.begin();
            hypo != m_pidTypes.end(); ++hypo )
      {
        const double predPixSig = m_photonSignal->predictedPixelSignal( photon, *hypo );
        const double minPixSig  = m_minPhotonProb[ photon->richRecSegment()->trackSegment().radiator() ];
        if ( predPixSig > minPixSig )
        {
          keepPhoton = true;
        }
        if ( keepPhoton ) break;
      }

      return keepPhoton;
    }

    void PhotonCreatorBase::InitNewEvent()
    {
      m_hasBeenCalled = false;
      if ( bookKeep() ) m_photonDone.clear();
      m_photons = 0;
      m_photCountLast = m_photCount;
    }

    void PhotonCreatorBase::FinishEvent()
    {
      if ( m_hasBeenCalled ) ++m_Nevts;
      if ( msgLevel(MSG::DEBUG) )
      {
        debug() << "Created " << richPhotons()->size() << " RichRecPhotons : Aerogel="
                << m_photCount[Rich::Aerogel]-m_photCountLast[Rich::Aerogel]
                << " Rich1Gas=" << m_photCount[Rich::Rich1Gas]-m_photCountLast[Rich::Rich1Gas]
                << " Rich2Gas=" << m_photCount[Rich::Rich2Gas]-m_photCountLast[Rich::Rich2Gas] << endmsg;
      }
    }

  } // Rec namespace
} // RICH namespace
