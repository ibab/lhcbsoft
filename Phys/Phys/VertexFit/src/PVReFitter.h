
#ifndef PVREFITTER_H 
#define PVREFITTER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "Kernel/IPVReFitter.h"            // Interface

class ITrackExtrapolator;

using namespace LHCb;

typedef ROOT::Math::SMatrix<double, 10, 10,
                            ROOT::Math::MatRepSym<double,10> > SymMatrix10x10;

typedef ROOT::Math::SMatrix<double, 8, 8,
                            ROOT::Math::MatRepSym<double,8> > SymMatrix8x8;


/** @class PVReFitter PVReFitter.h
 *  
 *  reimplementation of the PVReFitter for DC06
 *
 *  @author Yuehong Xie
 *  @date   2006-06-08
 */

class PVReFitter : public GaudiTool, virtual public IPVReFitter {
public: 
  /// Standard constructor
  PVReFitter( const std::string& type, 
              const std::string& name,
              const IInterface* parent);
  
  StatusCode initialize();

  /// refit PV
  virtual StatusCode reFit(LHCb::VertexBase* );

  /// remove track used for a (B) LHCb::Particle and refit PV
  virtual StatusCode remove(LHCb::Particle*,  LHCb::VertexBase*);

  virtual ~PVReFitter( ); ///< Destructor

private:
  
  StatusCode fitPV(LHCb::RecVertex*, LHCb::Track::Vector & );
  void getFinalTracks(LHCb::Particle*, LHCb::Track::ConstVector &);
  StatusCode seedPV(LHCb::RecVertex*,
                    LHCb::Track*, 
                    LHCb::Track* );
  StatusCode  addTr(LHCb::RecVertex*,
                    LHCb::Track* );
  LHCb::State stateAtFirstMeas(LHCb::Track*);

private:
      
  int m_maxIter;
  double m_maxDeltaChi2;
  double m_maxDeltaZ;
  
  ITrackExtrapolator* m_fullExtrapolator;
  std::string m_fullExtrapolatorName;
  ITrackExtrapolator* m_veloExtrapolator;
  std::string m_veloExtrapolatorName;

};
#endif // PVREFITTER_H
