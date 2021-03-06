#ifndef PRVUTTRACK_H
#define PRVUTTRACK_H 1

// Include files
#include "Event/Track.h"

#include "PrKernel/PrUTHit.h"


  /** @class PrVUTTrack PrVUTTrack.h
   *  Holds the candidate track while building it.
   *  Based on the original implementation by Olivier Callot.
   *
   *  @author Luis Fernandez
   *  @date   2005-07-27
   *  Taken over by Mariusz Witek and modified to have single version on-line off-line
   *  @date   2006-10-07
   */

  class PrVUTTrack {

  public:
    /// Standard constructor
    PrVUTTrack( LHCb::Track* tr ) {
      m_track  = tr;
      const LHCb::State& trState = tr->hasStateAt(LHCb::State::LastMeasurement) ?
        *(tr->stateAt(LHCb::State::LastMeasurement)) :
        (tr->closestState(LHCb::State::EndVelo)) ;

      m_origin = trState.position( );
      m_slope  = trState.slopes();
      m_clusters.reserve(4); // max of 1 per layer
      m_qOverP = 0.;
      m_chi2PerDoF=0.;
      
    };

    virtual ~PrVUTTrack( ) {}; ///< Destructor

    LHCb::Track* track()       const { return m_track; }

    float xAtZ( float z ) const {
      return m_origin.x() + m_slope.x() * ( z - m_origin.z() ) ;
    }

    float yAtZ( float z ) const {
      return m_origin.y() + m_slope.y() * ( z - m_origin.z() ) ;
    }
    
    float slopeY() const { return m_slope.y(); }

    void storeHit(PrUTHit* hit){
        m_clusters.push_back(hit);
    }

    PrUTHits& clusters() { return m_clusters;}

    void setChi2PerDoF(float chi2) { m_chi2PerDoF = chi2; }
    float chi2PerDoF() { return m_chi2PerDoF; }

    void setQOverP(float qp) { m_qOverP = qp; }
    float qOverP() { return m_qOverP; }

    void setXUT(float x) { m_xUT = x; }
    float xUT() { return m_xUT; }

    void setXSlopeUT(float tx) { m_txUT = tx; }
    float xSlopeUT() { return m_txUT; }

  protected:
    
  private:
    LHCb::Track*  m_track;
    Gaudi::XYZPoint m_origin;
    Gaudi::XYZVector m_slope;
    PrUTHits m_clusters;
    float m_chi2PerDoF;
    float m_qOverP;
    float m_xUT;
    float m_txUT;
  };

#endif // PRVUTTRACK_H

