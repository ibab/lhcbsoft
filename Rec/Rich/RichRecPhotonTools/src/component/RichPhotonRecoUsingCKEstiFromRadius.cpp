
//-----------------------------------------------------------------------------
/** @file RichPhotonRecoUsingCKEstiFromRadius.cpp
 *
 * Implementation file for class : Rich::Rec::PhotonRecoUsingCKEstiFromRadius
 *
 * @author Chris Jones   Christopher.Rob.Jones@cern.ch
 * @date 2003-11-14
 */
//-----------------------------------------------------------------------------

// local
#include "RichPhotonRecoUsingCKEstiFromRadius.h"

// All code is in general Rich reconstruction namespace
using namespace Rich::Rec;

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PhotonRecoUsingCKEstiFromRadius::
PhotonRecoUsingCKEstiFromRadius( const std::string& type,
                                 const std::string& name,
                                 const IInterface* parent )
  : PhotonRecoBase       ( type, name, parent       ),
    m_minCalibRingRadius ( Rich::NRadiatorTypes, -1 )
{
  // Job options
  declareProperty( "MinFracCKTheta", m_minFracCKtheta = { 1.0, 0.05, 0.05 } );
  declareProperty( "RejectAmbiguousPhotons", m_rejAmbigPhots = false );
  //                                                           Aero  R1Gas R2Gas
  declareProperty( "UseRingInterpolation", m_useRingInterp = { true, true, true } );

  // Corrections for the intrinsic biases
  //                  Aerogel    Rich1Gas    Rich2Gas
  m_ckBiasCorrs = {   0.0,      -6.687e-5,   1.787e-6   };

  //setProperty( "OutputLevel", 1 );
}

//=============================================================================
// Destructor
//=============================================================================
PhotonRecoUsingCKEstiFromRadius::~PhotonRecoUsingCKEstiFromRadius() { }

//=============================================================================
// Initialisation.
//=============================================================================
StatusCode PhotonRecoUsingCKEstiFromRadius::initialize()
{
  // initialise base class
  const StatusCode sc = PhotonRecoBase::initialize();
  if ( sc.isFailure() ) return sc;

  // Load used tools
  acquireTool( "RichCherenkovAngle", m_ckAngle       );

  // Setup PID types to consider
  const IParticleProperties * richPartProp = nullptr;
  acquireTool( "RichParticleProperties", richPartProp );
  m_pidTypes = richPartProp->particleTypes();
  _ri_debug << "Particle types considered for calibration points = " 
            << m_pidTypes << endmsg;
  release(richPartProp);

  _ri_debug << "Min CK theta fraction = " << m_minFracCKtheta << endmsg;

  return sc;
}

//-------------------------------------------------------------------------
//  reconstruct a photon from track segment and pixel
//-------------------------------------------------------------------------
StatusCode
PhotonRecoUsingCKEstiFromRadius::
reconstructPhoton ( const LHCb::RichRecSegment * segment,
                    const LHCb::RichRecPixel * pixel,
                    LHCb::RichGeomPhoton& gPhoton ) const
{
  // return status. Default is OK until proved otherwise
  StatusCode sc = StatusCode::SUCCESS;

  // track segment
  const auto & trSeg = segment->trackSegment();

  // Detector information
  const auto radiator = trSeg.radiator();
  // This tool is not optimised for Aerogel. So return failure
  if ( UNLIKELY( Rich::Aerogel == radiator ) )
  { return Warning("Ray tracing photon reconstruction not available for Aerogel"); }
  const auto side     = pixel->panel().panel();

  // Emission point to use for photon reconstruction
  auto & emissionPoint = gPhoton.emissionPoint();
  emissPoint()->emissionPoint( segment, pixel, emissionPoint );

  // Cannot set these yet - Could be done if needed, but at what CPU cost ?
  //Gaudi::XYZPoint & sphReflPoint = gPhoton.sphMirReflectionPoint();
  //Gaudi::XYZPoint & secReflPoint = gPhoton.flatMirReflectionPoint();

  // fraction of segment path length accessible to the photon
  // cannot determine this here so set to 1
  const float fraction(1);

  // Projected track position in corrected local coords on the detector plane
  const auto & segPSide = segment->pdPanelHitPointCorrectedLocal(side);

  // Pixel position in corrected local coords on detector plane
  const auto & pixPRad = pixel->radCorrLocalPositions().position(radiator);

  // x,y differences
  const auto diff_x = ( segPSide.x() - pixPRad.x() );
  const auto diff_y = ( segPSide.y() - pixPRad.y() );
  // The track - pixel seperation
  const auto track_pix_sep = std::sqrt( diff_x*diff_x + diff_y*diff_y );

  // estimate phi from these hits
  // use full atan2
  //const auto phiCerenkov = (float)( Gaudi::Units::pi + std::atan2(diff_y,diff_x) );
  // Use VDT
  const auto phiCerenkov = (float) ( Gaudi::Units::pi + vdt::fast_atan2f(diff_y,diff_x) );

  // Find the best ring and points to use as the calibration data
  const LHCb::RichRecRing *ring{nullptr}, *last_ring{nullptr};
  const LHCb::RichRecPointOnRing *best_point{nullptr};
  float sep_diff2 = std::numeric_limits<float>::max();
  float sep_calib{0}, last_sep_calib{0}, thetaCerenkov{-1};
  bool unambigPhoton{false}, last_unambigPhoton{false};
  bool pointOutsideLargestRing{false};
  for ( const auto pid : m_pidTypes )
  {

    // Load the ring and select the point for this PID type
    const auto * ring_tmp = massHypoRingCreator()->massHypoRing( segment, pid );

    // Did we find a ring (i.e. above threshold)
    if ( ring_tmp )
    {
      // check how close to threshold the ring is. Do not use if too close.
      if ( ring_tmp->radius() > minCalibRingRadius(radiator) )
      {

        // select the calibration point to use from this ring
        const auto points = ring_tmp->getPointsClosestInAzimuth(phiCerenkov);

        // if found see if this point is better than the last one tried
        if ( points.first && points.second &&
             sameSide( radiator, pixPRad, points.first->localPosition()  ) &&
             sameSide( radiator, pixPRad, points.second->localPosition() ) )
        {
          // get distance to each calibration point in phi
          const auto lD = fabs( points.first->azimuth()  - phiCerenkov );
          const auto hD = fabs( points.second->azimuth() - phiCerenkov );
          const auto Dinv = 1.0 / ( lD + hD );

          // corrected interpolated local calibration point position (x,y)
          const auto& lP    = points.first ->localPosition();
          const auto& hP    = points.second->localPosition();
          const auto calp_x = ( ( lP.x() * hD ) + ( hP.x() * lD ) ) * Dinv;
          const auto calp_y = ( ( lP.y() * hD ) + ( hP.y() * lD ) ) * Dinv;

          // pixel - calibration point seperation ^ 2
          const auto pix_calib_sep2 = ( std::pow( pixPRad.x() - calp_x, 2 ) +
                                        std::pow( pixPRad.y() - calp_y, 2 ) );
          const bool calibIsCloser = ( pix_calib_sep2 < sep_diff2 );

          // Is this point a better calibration point to use ?
          if ( m_useRingInterp[radiator] || calibIsCloser )
          {
            // update decision variable
            sep_diff2 = pix_calib_sep2;

            // First ring found ?
            const bool firstFoundRing = ( ring == nullptr );

            // Update best point pointer
            best_point = ( lD < hD ? points.first : points.second );

            // Update calibration distance
            last_sep_calib = sep_calib;
            sep_calib = std::sqrt( std::pow( segPSide.x() - calp_x, 2 ) +
                                   std::pow( segPSide.y() - calp_y, 2 ) );

            // Update best ring and pixel pointers
            last_ring = ring;
            ring      = ring_tmp;

            // Ambiguous photon test, based on the mirrors used by both calibration points.
            // Not quite the same as what is done in the Quartic tool (which tests
            // reconstruction from the start and end of the track segment) but its better
            // than doing nothing.
            last_unambigPhoton = unambigPhoton;
            unambigPhoton =
              ( ( points.first->primaryMirror()   == points.second->primaryMirror()   ) &&
                ( points.first->secondaryMirror() == points.second->secondaryMirror() ) );

            // Is the data point outside the first ring ?
            if ( firstFoundRing ) { pointOutsideLargestRing = track_pix_sep > sep_calib; }

            // If interpolation is enabled, and we are between rings, use them
            if ( m_useRingInterp[radiator]       &&
                 !firstFoundRing                 &&
                 track_pix_sep >=      sep_calib &&
                 track_pix_sep <  last_sep_calib  )
            {
              // Interpolate between the two rings, then break out as this is the best we can do
              thetaCerenkov = 
                ( ( (      ring->radius() * ( track_pix_sep - last_sep_calib ) ) +
                    ( last_ring->radius() * ( sep_calib     - track_pix_sep  ) ) ) / 
                  ( sep_calib - last_sep_calib ) );
              break;
            }
            else
            {
              // Update CK theta value just using this ring
              thetaCerenkov = ring->radius() * ( track_pix_sep / sep_calib );
              // If this is the first found ring, and the point is further away than
              // the calibration point, do not bother searching other (smaller) rings
              // as they are only going to get further away.
              if ( firstFoundRing && pointOutsideLargestRing ) { break; }
            }

            // // If we have two rings, try and interpolate between them
            // if ( m_useRingInterp[radiator] && ring && last_ring )
            // {
            //   // Interpolate between the two rings
            //   thetaCerenkov = 
            //     ( ( (      ring->radius() * ( track_pix_sep - last_sep_calib ) ) +
            //         ( last_ring->radius() * ( sep_calib     - track_pix_sep  ) ) ) / 
            //       ( sep_calib - last_sep_calib ) );
            //   // If data point is between the last two rings, or data point is outside the largest
            //   // ring, break out as this is the optimal combination to use for interpolation
            //   if ( pointOutsideLargestRing || 
            //        ( track_pix_sep >= sep_calib &&
            //          track_pix_sep <  last_sep_calib ) ) { break; }
            // }
            // else
            // {
            //   // Update CK theta value just using this ring
            //   thetaCerenkov = ring->radius() * ( track_pix_sep / sep_calib );
            // }

          }
          else if ( ring )
          {
            // a ring is already found, and we are getting further away, so break out ...
            break;
          }
          
        } // point found

      } // ring saturation check

    }
    else
    {
      // no ring was found. This means we are below threshold so there is no
      // point in searching other heavier rings, so break out.
      break;
    }

  } // loop over PID types

  // If a good calibration point was found, use its info to fill the photon
  if ( ring )
  {

    // check for ambiguous photons ?
    const bool unambig = unambigPhoton && last_unambigPhoton;
    if ( UNLIKELY( !unambig && m_rejAmbigPhots ) )
    {
      // Calibration points used different mirrors, so reject
      //_ri_verbo << "Ambiguous photon -> reject" << endmsg;
      sc = StatusCode::FAILURE;
    }
    else
    {

      // Add bias correction to CK theta value
      thetaCerenkov += ckThetaCorrection(radiator);

      // ------------------------------------------------------------------------
      // Set (remaining) photon parameters
      // ------------------------------------------------------------------------
      gPhoton.setCherenkovTheta         ( thetaCerenkov            );
      gPhoton.setCherenkovPhi           ( phiCerenkov              );
      gPhoton.setActiveSegmentFraction  ( fraction                 );
      gPhoton.setDetectionPoint         ( pixel->globalPosition()  );
      gPhoton.setSmartID                ( pixel->hpdPixelCluster().primaryID() );
      gPhoton.setUnambiguousPhoton      ( unambig                  );
      gPhoton.setPrimaryMirror          ( best_point->primaryMirror()   );
      gPhoton.setSecondaryMirror        ( best_point->secondaryMirror() );
      // ------------------------------------------------------------------------

      // Print the photon
      //_ri_verbo << "Created photon " << gPhoton << endmsg;

    }

  }
  else
  {
    //_ri_verbo << "No closest point found !" << endmsg;
    sc = StatusCode::FAILURE;
  }

  // Return final status
  return sc;
}

//=============================================================================
// Declaration of the Algorithm Factory
//=============================================================================

DECLARE_TOOL_FACTORY ( PhotonRecoUsingCKEstiFromRadius )

//=============================================================================

