
//-----------------------------------------------------------------------------
/** @file RichPixelCreatorFromAllMCRichHits.cpp
 *
 *  Implementation file for RICH reconstruction tool : Rich::Rec::PixelCreatorFromAllMCRichHits
 *
 *  CVS Log :-
 *  $Id: RichPixelCreatorFromAllMCRichHits.cpp,v 1.1 2009-07-31 11:57:12 jonrob Exp $
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   15/09/2003
 */
//-----------------------------------------------------------------------------

// from Gaudi
#include "GaudiKernel/ToolFactory.h"

// local
#include "RichPixelCreatorFromAllMCRichHits.h"

// All code is in general Rich reconstruction namespace
using namespace Rich::Rec::MC;

//-----------------------------------------------------------------------------

DECLARE_TOOL_FACTORY( PixelCreatorFromAllMCRichHits );

// Standard constructor
PixelCreatorFromAllMCRichHits::
PixelCreatorFromAllMCRichHits( const std::string& type,
                               const std::string& name,
                               const IInterface* parent )
  : Rich::Rec::PixelCreatorBase ( type, name, parent ),
    m_mcTool                    ( NULL               )
{
  // book keeping cannot work with this tool
  setProperty( "DoBookKeeping", false );
}

StatusCode PixelCreatorFromAllMCRichHits::initialize()
{
  // Sets up various tools and services
  const StatusCode sc = Rich::Rec::PixelCreatorBase::initialize();
  if ( sc.isFailure() ) { return sc; }

  // book keeping cannot work with this tool
  if ( bookKeep() )
    return Error( "Pixel Book-keeping cannot work with this pixel creator" );

  // Acquire instances of tools
  acquireTool( "RichMCTruthTool", m_mcTool, 0, true );

  return sc;
}

LHCb::RichRecPixel *
PixelCreatorFromAllMCRichHits::buildPixel( const Rich::HPDPixelCluster& cluster ) const
{
  // build hits for each pixel in the cluster
  for ( LHCb::RichSmartID::Vector::const_iterator iS = cluster.smartIDs().begin();
        iS != cluster.smartIDs().end(); ++iS )
  {
    buildPixel( *iS );
  }
  // always return NULL ...
  return NULL;
}

LHCb::RichRecPixel *
PixelCreatorFromAllMCRichHits::buildPixel( const LHCb::RichSmartID& id ) const
{

  if ( msgLevel(MSG::VERBOSE) )
  {
    verbose() << "Making pixels for " << id << endmsg;
  }

  // First run base class method to produce reconstructed pixel
  LHCb::RichRecPixel * pixel = Rich::Rec::PixelCreatorBase::buildPixel(id);

  // Get MC hits
  const SmartRefVector<LHCb::MCRichHit>& mcRichHits = m_mcTool->mcRichHits( id );
  if ( !mcRichHits.empty() )
  {

    // Loop over hits
    for ( SmartRefVector<LHCb::MCRichHit>::const_iterator iHit = mcRichHits.begin();
          iHit != mcRichHits.end(); ++iHit )
    {
      // Is this a true CK photon ?
      const LHCb::MCRichOpticalPhoton * mcPhot = m_mcTool->mcOpticalPhoton( *iHit );
      if ( mcPhot )
      {
        // clone the pixel for this hit
        LHCb::RichRecPixel * pix = new LHCb::RichRecPixel(*pixel);
        // Update coordinates with cheated info
        pix->setGlobalPosition( mcPhot->hpdQWIncidencePoint() );
        pix->setLocalPosition( smartIDTool()->globalToPDPanel(pix->globalPosition()) );
        // set the corrected local positions
        geomTool()->setCorrLocalPos(pix,id.rich());
        // save this pixel
        savePixel( pix );
        // some printout
        if ( msgLevel(MSG::VERBOSE) )
        {
          verbose() << " -> MC cheated global pos " << pix->globalPosition() << endmsg;
        }
      }
    }

    // delete the original
    delete pixel;
    pixel = NULL;

  }

  // return
  return pixel;
}
