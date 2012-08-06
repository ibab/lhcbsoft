// Include files


// from Gaudi
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiKernel/SmartIF.h"

// from Event
#include "Event/Track.h"
#include "Linker/LinkerWithKey.h"

#include "Event/MCParticle.h"
#include "Event/MCVertex.h"

// local
#include "PrTrackAssociator.h"

DECLARE_ALGORITHM_FACTORY( PrTrackAssociator )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  PrTrackAssociator::PrTrackAssociator( const std::string& name,
                                          ISvcLocator* pSvcLocator ) :
    GaudiAlgorithm( name, pSvcLocator )
{
  declareProperty( "RootOfContainers",  m_rootOfContainers = "/Event/Rec/Track" );
  declareProperty( "SingleContainer",   m_singleContainer  = "" );
  declareProperty( "FractionOK"    ,    m_fractionOK       = 0.70 );
}

//=============================================================================
// Destructor
//=============================================================================
PrTrackAssociator::~PrTrackAssociator() {}

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode PrTrackAssociator::initialize() {

  // Mandatory initialization of GaudiAlgorithm
  StatusCode sc = GaudiAlgorithm::initialize();
  if( sc.isFailure() ) { return sc; }

  if ( "" != m_singleContainer ) {
    info() << "Processing only the container " << m_singleContainer << endmsg;
  } else {
    info() << "Processing all tracks containers in " << m_rootOfContainers << endmsg;
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode PrTrackAssociator::execute() {

  // Retrieve the MCParticles
  LHCb::MCParticles* mcParts = get<LHCb::MCParticles> ( LHCb::MCParticleLocation::Default );
  // Get the LHCbID to MCParticle linker
  LinkedTo<LHCb::MCParticle> idLinker( evtSvc(), msgSvc(), "Pr/LHCbID" );

  std::vector<std::string> trackContainers;
  if ( "" != m_singleContainer ) {
    trackContainers.push_back( m_singleContainer );
  } else {
    //== Scan the root directory and make a list of containers to process.
    DataObject* root = get<DataObject*>( m_rootOfContainers );
    unsigned int trackContainerId = LHCb::Track::classID() + 0x60000;
    SmartIF<IDataManagerSvc> mgr( eventSvc() );
    typedef std::vector<IRegistry*> Leaves;
    Leaves leaves;
    StatusCode sc = mgr->objectLeaves( root, leaves );
    if ( sc ) {
      for ( Leaves::const_iterator iL = leaves.begin(); leaves.end() != iL; ++iL ) {
        const std::string& id = (*iL)->identifier();
        DataObject* tmp(NULL);
        sc = eventSvc()->findObject( id, tmp );
        if ( sc && NULL != tmp ) {
          if ( tmp->clID() == trackContainerId ) {
            trackContainers.push_back( id );
          }
        }
      }
    }
  }

  for ( std::vector<std::string>::iterator itS = trackContainers.begin();
        trackContainers.end() != itS; ++itS ) {

    // Retrieve the Tracks
    LHCb::Tracks* tracks = get<LHCb::Tracks> ( *itS );

    // Create the Linker table from Track to MCParticle
    // Linker table is stored in "Link/" + m_linkerOutTable
    // Sorted by decreasing weight, so first retrieved has highest weight
    LinkerWithKey<LHCb::MCParticle,LHCb::Track> myLinker( evtSvc(), msgSvc(), *itS );

    // Loop over the Tracks
    LHCb::Tracks::const_iterator it;
    for( it = tracks->begin(); tracks->end() != it; ++it ) {
      const LHCb::Track* tr = *it;
      m_total.reset();
      m_truthCounters.clear();

      // Loop over the LHCbIDs of the Track
      int nMeas = 0;
      for( std::vector<LHCb::LHCbID>::const_iterator iId = tr->lhcbIDs().begin();
           tr->lhcbIDs().end() != iId; ++iId ) {

        if( (*iId).isVelo() || (*iId).isVeloPix() || (*iId).isVL() ) {
          ++nMeas;
          m_total.nVelo += 1.;       // Count number of Velo hits
          LHCb::MCParticle* part = idLinker.first( (*iId).lhcbID() );
          while( 0 != part ) {
            if( mcParts == part->parent() ) incrementVelo( part  );
            part = idLinker.next();
          }

        } else if ( (*iId).isTT() ) {
          ++nMeas;
          m_total.nTT += 1.;       // Count number of TT hits
          LHCb::MCParticle* part = idLinker.first( (*iId).lhcbID() );
          while( 0 != part ) {
            if( mcParts == part->parent() ) incrementTT( part  );
            part = idLinker.next();
          }

        } else if ( (*iId).isIT() || (*iId).isOT() || (*iId).isFT() ) {
          ++nMeas;
          m_total.nT += 1.;       // Count number of T hits
          LHCb::MCParticle* part = idLinker.first( (*iId).lhcbID() );
          while( 0 != part ) {
            if( mcParts == part->parent() ) incrementT( part  );
            part = idLinker.next();
          }
        }
      }

      // If the Track has total # Velo hits > 2 AND total # T hits > 2, cumul mother and daughter
      if( ( 2 < m_total.nVelo ) &&
          ( 2 < m_total.nT    ) ) {
        for ( std::vector<TruthCounter>::iterator it1 = m_truthCounters.begin();
              m_truthCounters.end() != it1; ++it1 ) {
          if ( (*it1).nT < m_fractionOK * m_total.nT ) continue;  // not match in T
          const LHCb::MCVertex* vOrigin = (*it1).particle->originVertex();
          while( 0 != vOrigin ) {
            const LHCb::MCParticle* mother = vOrigin->mother();
            if( 0 == mother ) break;  // end of ancestors
            for ( std::vector<TruthCounter>::iterator it2 = m_truthCounters.begin();
                  m_truthCounters.end() != it2; ++it2 ) {
              if( (*it2).nVelo < m_fractionOK * m_total.nVelo ) continue;  // not matching in Velo

              if( mother == (*it2).particle ) {

                debug() << "  *** Particle " << (*it1).particle->key()
                        << "[" << (*it1).particle->particleID().pid()
                        << "] (" << (*it1).nVelo << "," << (*it1).nTT << "," << (*it1).nT << ")"
                        << " is daughter of " << (*it2).particle->key()
                        << "[" << (*it2).particle->particleID().pid()
                        << "] (" << (*it2).nVelo << "," << (*it2).nTT << "," << (*it2).nT << ")"
                        << ". Merge hits to tag both." << endreq;

                //== Daughter hits are added to mother.
                (*it2).nVelo += (*it1).nVelo;
                (*it2).nTT   += (*it1).nTT;
                (*it2).nT    += (*it1).nT;

                //== Mother hits overwrite Daughter hits
                (*it1).nVelo = (*it2).nVelo;
                (*it1).nTT   = (*it2).nTT;
                (*it1).nT    = (*it2).nT;
              }
            }
            // Go one step into the past of family line, so find grandmother
            vOrigin = mother->originVertex();
          }
        }
      }

      //===============================================================
      // Association definition
      // Velo matching:
      //      * either less than 2 hits in total (no Velo on track)
      //      * or at least 'm_fractionOK' of the Velo hits are associated to this MCParticle.
      // TT matching:
      //      * this MC particle has at least nTT-2 hits.
      //      * or this track has both Velo and T stations: TT not used for long tracks.
      // T matching:
      //      * either less than 2 hits in total (no T station hits on track)
      //      * or at least 'm_fractionOK' of the T station hits are associated to this MCParticle.
      //
      // A MCParticle matches if all 4 criteria are OK.
      //===============================================================

      double ratio;
      for ( std::vector<TruthCounter>::iterator it = m_truthCounters.begin();
            m_truthCounters.end() != it; ++it ) {
        bool veloOK = true;
        if( 2 < m_total.nVelo ) {
          veloOK = false;
          ratio = (*it).nVelo / m_total.nVelo;
          if( m_fractionOK <= ratio ) { veloOK = true; }
        }

        bool tOK = true;
        if( 2 < m_total.nT ) {
          tOK = false;
          ratio =  (*it).nT / m_total.nT;
          if( m_fractionOK <= ratio ) { tOK = true; }
        }

        bool ttOK = (*it).nTT > m_total.nTT - 2;
        if( 2 < m_total.nVelo && 2 < m_total.nT ) { ttOK = true; }

        debug() << "Track " << tr->key()
                << " MC "   << (*it).particle->key()
                << " Velo " << (*it).nVelo << "/" << m_total.nVelo
                << " TT "   << (*it).nTT   << "/" << m_total.nTT
                << " T "    << (*it).nT    << "/" << m_total.nT
                << endreq;

        //=== Decision. Fill Linker
        if( veloOK && tOK && ttOK ) {
          ratio = ( (*it).nVelo + (*it).nTT + (*it).nT ) / nMeas;
          myLinker.link( tr, (*it).particle, ratio );
        }
      }
    } // End loop over Tracks
  }

  return StatusCode::SUCCESS;
}
//=========================================================================
// Increment the Velo count.
//=========================================================================
void PrTrackAssociator::incrementVelo( const LHCb::MCParticle* part ) {

  for ( std::vector<TruthCounter>::iterator it = m_truthCounters.begin();
        m_truthCounters.end() != it; ++it ) {
    if( (*it).particle == part ) {
      (*it).nVelo += 1.;
      return;
    }
  }
  TruthCounter tmp;
  tmp.particle = part;
  tmp.nVelo    = 1.;
  m_truthCounters.push_back( tmp );
}
//=========================================================================
// Increment the TT count.
//=========================================================================
void PrTrackAssociator::incrementTT( const LHCb::MCParticle* part ) {

  for ( std::vector<TruthCounter>::iterator it = m_truthCounters.begin();
        m_truthCounters.end() != it; ++it ) {
    if( (*it).particle == part ) {
      (*it).nTT += 1.;
      return;
    }
  }
  TruthCounter tmp;
  tmp.particle = part;
  tmp.nTT    = 1.;
  m_truthCounters.push_back( tmp );
}
//=========================================================================
// Increment the T count.
//=========================================================================
void PrTrackAssociator::incrementT( const LHCb::MCParticle* part ) {

  for ( std::vector<TruthCounter>::iterator it = m_truthCounters.begin();
        m_truthCounters.end() != it; ++it ) {
    if( (*it).particle == part ) {
      (*it).nT += 1.;
      return;
    }
  }
  TruthCounter tmp;
  tmp.particle = part;
  tmp.nT    = 1.;
  m_truthCounters.push_back( tmp );
}
//=========================================================================
