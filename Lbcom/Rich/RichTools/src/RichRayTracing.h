
//-----------------------------------------------------------------------------
/** @file RichRayTracing.h
 *
 *  Header file for tool : Rich::RayTracing
 *
 *  CVS History :
 *  $Id: RichRayTracing.h,v 1.33 2007-08-09 16:00:25 jonrob Exp $
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2004-03-29
 */
//-----------------------------------------------------------------------------

#ifndef RICHTOOLS_RICHRAYTRACING_H
#define RICHTOOLS_RICHRAYTRACING_H 1

// from Gaudi
#include "GaudiKernel/ToolFactory.h"

// Base class and interface
#include "RichKernel/RichHistoToolBase.h"
#include "RichKernel/IRichRayTracing.h"

// RichKernel
#include "RichKernel/IRichMirrorSegFinder.h"
#include "RichKernel/IRichRefractiveIndex.h"
#include "RichKernel/IRichRadiatorTool.h"
#include "RichKernel/BoostArray.h"
#include "RichKernel/RichGeomPhoton.h"

// Math
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// Kernel
#include "Kernel/RichSide.h"
#include "Kernel/RichSmartID.h"
#include "Kernel/RichTraceMode.h"

// RichDet
#include "RichDet/DeRich.h"
#include "RichDet/DeRichSphMirror.h"
#include "RichDet/DeRichHPDPanel.h"
#include "RichDet/Rich1DTabProperty.h"
#include "RichDet/DeRichBeamPipe.h"

// Units
#include "GaudiKernel/SystemOfUnits.h"

namespace Rich
{

  //-----------------------------------------------------------------------------
  /** @class RayTracing RichRayTracing.h
   *
   *  Rich detector tool which traces photons to the photodetectors.
   *
   *  Mirror segmentation is takaen into account.
   *
   *  @author Antonis Papanestis
   *  @author Chris Jones
   *  @date   2003-11-04
   *
   *  @todo Check if it is neccessary to check for intersections other than those
   *  from emission point to spherical mirror reflection point ?
   */
  //-----------------------------------------------------------------------------

  class RayTracing : public Rich::HistoToolBase,
                     virtual public IRayTracing
  {

  public: // Methods for Gaudi Framework

    /// Standard constructor
    RayTracing( const std::string& type,
                const std::string& name,
                const IInterface* parent);

    virtual ~RayTracing( ); ///< Destructor

    // Initialization of the tool after creation
    virtual StatusCode initialize();

    // Finalization of the tool before deletion
    virtual StatusCode finalize  ();

  public: // methods (and doxygen comments) inherited from interface

    /// For a given detector, raytraces a given direction from a given point to
    /// the photo detectors. Returns the result in the form of a RichGeomPhoton
    LHCb::RichTraceMode::RayTraceResult
    traceToDetector( const Rich::DetectorType rich,
                     const Gaudi::XYZPoint& startPoint,
                     const Gaudi::XYZVector& startDir,
                     LHCb::RichGeomPhoton& photon,
                     const LHCb::RichTraceMode mode = LHCb::RichTraceMode(),
                     const Rich::Side forcedSide    = Rich::top,
                     const double photonEnergy      = 0 ) const;

    /// For a given detector, raytraces a given direction from a given point to
    /// the photo detectors.
    LHCb::RichTraceMode::RayTraceResult
    traceToDetector( const Rich::DetectorType rich,
                     const Gaudi::XYZPoint& startPoint,
                     const Gaudi::XYZVector& startDir,
                     Gaudi::XYZPoint& hitPosition,
                     const LHCb::RichTraceMode mode = LHCb::RichTraceMode(),
                     const Rich::Side forcedSide    = Rich::top,
                     const double photonEnergy      = 0 ) const;

    /// Raytraces from a point in the detector panel back to the spherical mirror
    /// returning the mirror intersection point and the direction a track would
    /// have in order to hit that point in the detector panel.
    StatusCode traceBackFromDetector ( const Gaudi::XYZPoint& startPoint,
                                       const Gaudi::XYZVector& startDir,
                                       Gaudi::XYZPoint& endPoint,
                                       Gaudi::XYZVector& endDir ) const;

    /// Intersection a given direction, from a given point with a given plane.
    StatusCode intersectPlane( const Gaudi::XYZPoint& position,
                               const Gaudi::XYZVector& direction,
                               const Gaudi::Plane3D& plane,
                               Gaudi::XYZPoint& intersection ) const;

    /// Reflect a given direction off a spherical mirror. Can be used for intersection.
    StatusCode reflectSpherical ( Gaudi::XYZPoint& position,
                                  Gaudi::XYZVector& direction,
                                  const Gaudi::XYZPoint& CoC,
                                  const double radius ) const;

    /// Ray trace from given position in given direction off flat mirrors
    StatusCode reflectFlatPlane ( Gaudi::XYZPoint& position,
                                  Gaudi::XYZVector& direction,
                                  const Gaudi::Plane3D& plane ) const;

  private: // methods

    /// Ray trace from given position in given direction off both mirrors
    bool reflectBothMirrors ( const Rich::DetectorType rich,
                              Gaudi::XYZPoint& position,
                              Gaudi::XYZVector& direction,
                              LHCb::RichGeomPhoton& photon,
                              const LHCb::RichTraceMode mode,
                              const Rich::Side fSide ) const;

    /// Access the DeRich beam pipe objects, creating as needed on demand
    inline const DeRichBeamPipe* deBeam( const Rich::DetectorType rich ) const
    {
      if ( !m_deBeam[rich] )
      {
        m_deBeam[rich] = getDet<DeRichBeamPipe>( Rich::Rich1 == rich ?
                                                 DeRichBeamPipeLocation::Rich1BeamPipe :
                                                 DeRichBeamPipeLocation::Rich2BeamPipe );
      }
      return m_deBeam[rich];
    }

  private: // data

    /// Refractive index tool
    const IRefractiveIndex * m_refIndex;

    /// Rich1 and Rich2 pointers
    std::vector< const DeRich* > m_rich;

    /// photodetector panels per rich
    typedef boost::array<DeRichHPDPanel*, 2> HPDPanelsPerRich;
    /// photodetector for each rich
    boost::array<HPDPanelsPerRich, Rich::NRiches> m_photoDetPanels;

    std::vector<int> m_sphMirrorSegRows;
    std::vector<int> m_sphMirrorSegCols;
    std::vector<int> m_secMirrorSegRows;
    std::vector<int> m_secMirrorSegCols;

    /// Mirror segment finder tool
    const IMirrorSegFinder* m_mirrorSegFinder;

    /// Flag to to ignore secondary mirrors (useful for test beam work)
    bool m_ignoreSecMirrs;

    /// RICH beampipe object for each RICH detector
    mutable std::vector<const DeRichBeamPipe*> m_deBeam;

    /// aerogel exit plane (for refraction correction)
    Gaudi::Plane3D m_aeroExitPlane;

    /// Vector normal to aerogel exit plane (for refraction correction)
    Gaudi::XYZVector m_aeroNormVect;

    /// z point for plane
    double m_minZaero;

  };

}

#endif // RICHTOOLS_RICHRAYTRACING_H
