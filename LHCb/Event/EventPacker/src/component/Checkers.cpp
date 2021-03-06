#include "CheckerBaseAlg.icpp"

#include "Event/PackedMCRichHit.h"
#include "Event/PackedMCRichOpticalPhoton.h"
#include "Event/PackedMCRichSegment.h"
#include "Event/PackedMCRichTrack.h"
#include "Event/PackedMCCaloHit.h"
#include "Event/PackedMCHit.h"
#include "Event/PackedMCRichDigitSummary.h"
#include "Event/PackedMuonPID.h"
#include "Event/PackedRichPID.h"
#include "Event/PackedParticle.h"
#include "Event/PackedVertex.h"
#include "Event/PackedWeightsVector.h"
#include "Event/PackedCaloCluster.h"
#include "Event/PackedFlavourTag.h"
#include "Event/PackedPartToRelatedInfoRelation.h"

template class DataPacking::Check<LHCb::MCRichHitPacker>;
typedef DataPacking::Check<LHCb::MCRichHitPacker> A;
DECLARE_ALGORITHM_FACTORY( A )

template class DataPacking::Check<LHCb::MCRichOpticalPhotonPacker>;
typedef DataPacking::Check<LHCb::MCRichOpticalPhotonPacker> B;
DECLARE_ALGORITHM_FACTORY( B )

template class DataPacking::Check<LHCb::MCRichSegmentPacker>;
typedef DataPacking::Check<LHCb::MCRichSegmentPacker> C;
DECLARE_ALGORITHM_FACTORY( C )

template class DataPacking::Check<LHCb::MCRichTrackPacker>;
typedef DataPacking::Check<LHCb::MCRichTrackPacker> D;
DECLARE_ALGORITHM_FACTORY( D )

template class DataPacking::Check<LHCb::MCPrsHitPacker>;
typedef DataPacking::Check<LHCb::MCPrsHitPacker> Ea;
DECLARE_ALGORITHM_FACTORY( Ea )

template class DataPacking::Check<LHCb::MCSpdHitPacker>;
typedef DataPacking::Check<LHCb::MCSpdHitPacker> Eb;
DECLARE_ALGORITHM_FACTORY( Eb )

template class DataPacking::Check<LHCb::MCEcalHitPacker>;
typedef DataPacking::Check<LHCb::MCEcalHitPacker> Ec;
DECLARE_ALGORITHM_FACTORY( Ec )

template class DataPacking::Check<LHCb::MCHcalHitPacker>;
typedef DataPacking::Check<LHCb::MCHcalHitPacker> Ed;
DECLARE_ALGORITHM_FACTORY( Ed )

template class DataPacking::Check<LHCb::MCVeloHitPacker>;
typedef DataPacking::Check<LHCb::MCVeloHitPacker> F;
DECLARE_ALGORITHM_FACTORY( F )

template class DataPacking::Check<LHCb::MCPuVetoHitPacker>;
typedef DataPacking::Check<LHCb::MCPuVetoHitPacker> G;
DECLARE_ALGORITHM_FACTORY( G )

template class DataPacking::Check<LHCb::MCVPHitPacker>;
typedef DataPacking::Check<LHCb::MCVPHitPacker> Fp;
DECLARE_ALGORITHM_FACTORY( Fp )

template class DataPacking::Check<LHCb::MCTTHitPacker>;
typedef DataPacking::Check<LHCb::MCTTHitPacker> H;
DECLARE_ALGORITHM_FACTORY( H )

template class DataPacking::Check<LHCb::MCUTHitPacker>;
typedef DataPacking::Check<LHCb::MCUTHitPacker> Hu;
DECLARE_ALGORITHM_FACTORY( Hu )

template class DataPacking::Check<LHCb::MCITHitPacker>;
typedef DataPacking::Check<LHCb::MCITHitPacker> I;
DECLARE_ALGORITHM_FACTORY( I )

template class DataPacking::Check<LHCb::MCOTHitPacker>;
typedef DataPacking::Check<LHCb::MCOTHitPacker> J;
DECLARE_ALGORITHM_FACTORY( J )

template class DataPacking::Check<LHCb::MCMuonHitPacker>;
typedef DataPacking::Check<LHCb::MCMuonHitPacker> K;
DECLARE_ALGORITHM_FACTORY( K )

template class DataPacking::Check<LHCb::MCRichDigitSummaryPacker>;
typedef DataPacking::Check<LHCb::MCRichDigitSummaryPacker> L;
DECLARE_ALGORITHM_FACTORY( L )

template class DataPacking::Check<LHCb::RichPIDPacker>;
typedef DataPacking::Check<LHCb::RichPIDPacker> M;
DECLARE_ALGORITHM_FACTORY( M )

template class DataPacking::Check<LHCb::MuonPIDPacker>;
typedef DataPacking::Check<LHCb::MuonPIDPacker> N;
DECLARE_ALGORITHM_FACTORY( N )

template class DataPacking::Check<LHCb::ParticlePacker>;
typedef DataPacking::Check<LHCb::ParticlePacker> O;
DECLARE_ALGORITHM_FACTORY( O )

template class DataPacking::Check<LHCb::VertexPacker>;
typedef DataPacking::Check<LHCb::VertexPacker> P;
DECLARE_ALGORITHM_FACTORY( P )

template class DataPacking::Check<LHCb::WeightsVectorPacker>;
typedef DataPacking::Check<LHCb::WeightsVectorPacker> Q;
DECLARE_ALGORITHM_FACTORY( Q )

template class DataPacking::Check<LHCb::CaloClusterPacker>;
typedef DataPacking::Check<LHCb::CaloClusterPacker> R;
DECLARE_ALGORITHM_FACTORY( R )

template class DataPacking::Check<LHCb::MCFTHitPacker>;
typedef DataPacking::Check<LHCb::MCFTHitPacker> S_;
DECLARE_ALGORITHM_FACTORY( S_ )

template class DataPacking::Check<LHCb::MCSLHitPacker>;
typedef DataPacking::Check<LHCb::MCSLHitPacker> T;
DECLARE_ALGORITHM_FACTORY( T )

template class DataPacking::Check<LHCb::FlavourTagPacker>;
typedef DataPacking::Check<LHCb::FlavourTagPacker> U;
DECLARE_ALGORITHM_FACTORY( U )

template class DataPacking::Check<LHCb::MCHCHitPacker>;
typedef DataPacking::Check<LHCb::MCHCHitPacker> V;
DECLARE_ALGORITHM_FACTORY( V )

template class DataPacking::Check<LHCb::MCBcmHitPacker>;
typedef DataPacking::Check<LHCb::MCBcmHitPacker> Va;
DECLARE_ALGORITHM_FACTORY( Va )

template class DataPacking::Check<LHCb::MCBlsHitPacker>;
typedef DataPacking::Check<LHCb::MCBlsHitPacker> Vb;
DECLARE_ALGORITHM_FACTORY( Vb )

template class DataPacking::Check<LHCb::RelatedInfoRelationsPacker>;
typedef DataPacking::Check<LHCb::RelatedInfoRelationsPacker> W;
DECLARE_ALGORITHM_FACTORY( W )
