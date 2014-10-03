// $Id: RelInfoBs2MuMuBIsolations.h 
#ifndef RELINFOBS2MUMUBISOLATIONS_H 
#define RELINFOBS2MUMUBISOLATIONS_H 1

#include "GaudiKernel/ToolFactory.h"
#include "Kernel/RelatedInfoNamed.h"
#include "Kernel/IRelatedInfoTool.h"
#include "Event/RelatedInfoMap.h"
#include "GaudiAlg/GaudiTool.h"
#include "CaloUtils/CaloParticle.h"
#include "Kernel/IDistanceCalculator.h"
#include "Kernel/IParticleCombiner.h"
#include "Kernel/IDVAlgorithm.h"
#include <Kernel/GetIDVAlgorithm.h>
#include "Kernel/IVertexFit.h"
#include "Kernel/IPVReFitter.h"
#include "Event/RecVertex.h"


/** @class RelInfoBs2MuMuBIsolations RelInfoBs2MuMuBIsolations.h

 * 
 *  @author Fatima Soomro
 *  @date   2014-08-08
 */

class IDistanceCalculator;
class IDVAlgorithm;
class IPVReFitter;

class RelInfoBs2MuMuBIsolations : public GaudiTool, virtual public IRelatedInfoTool{
 public: 
  /// Standard constructor
  RelInfoBs2MuMuBIsolations( const std::string& type, 
              const std::string& name,
              const IInterface* parent );

  virtual StatusCode initialize();    ///< Algorithm initialization

  virtual StatusCode calculateRelatedInfo( const LHCb::Particle*, const LHCb::Particle*);
  
  virtual LHCb::RelatedInfoMap* getInfo(void);

  virtual std::string infoPath(void);

  virtual ~RelInfoBs2MuMuBIsolations( ); ///< Destructor

  
protected:


private:

  IDVAlgorithm* m_dva;
  const IDistanceCalculator* m_dist;
  const IParticleCombiner* m_combiner;

  bool isTrackInDecay(const LHCb::Track* track);
  void saveDecayParticles( const LHCb::Particle *top);

  StatusCode CDFIsolation(const LHCb::Particle* , 
			  const LHCb::Particle* );
  
  StatusCode OtherB(const LHCb::Particle* ,
		    const LHCb::Particle* );

  
  
  LHCb::RelatedInfoMap m_map;
  std::vector<std::string> m_variables;
  std::vector<short int> m_keys;
  std::vector<const LHCb::Particle*> m_decayParticles;
  LHCb::RecVertex::Container * m_vertices ;
  
  std::string m_TracksPath;
  std::string m_ParticlePath;
  std::string m_PVInputLocation;	

  double m_pvdis_h;
  double m_pvdis;
  double m_svdis_h;
  double m_svdis;
  int m_tracktype;
  double m_ips;
  double m_fc;
  double m_angle;
  double m_doca_iso;

  double m_clone_cut;
  double m_ghost_cut;
  double m_trchi2_cut;
  bool m_makeTrackCuts;
  bool m_IsoTwoBody;

  // //// Variables that are calculated and stored 
 
  double m_CDFIso;
  double m_otherB_mag;
  double m_otherB_angle;
  double m_otherB_boost_mag;
  double m_otherB_boost_angle;
  int otherBtracks;
  

  /** IsHltGood: Defines whether or not a particle is not isolating according to the
      BsMuMu Roadmap, using an Hlt criterium                                                                                                                                                             
  */
  void IsHltGood(Gaudi::XYZPoint o, Gaudi::XYZVector p, Gaudi::XYZPoint o_mu,Gaudi::XYZVector p_mu, Gaudi::XYZPoint PV, bool& hltgood, double& fc);
  
  /** pointer: given two particles and the vector between the PV and vertex defined                                                                                                                                   *  by them, it returns :                                                                                                                                                                                 *   fc=\frac{\left|\vec{P_{\mu}}+\vec{P_{tr}}\right|\cdot\alpha^{\mu+tr,PV}}                                                                                                                             *   {\left|\vec{P_{\mu}}+\vec{P_{tr}}\right|\cdot\alpha^{\mu+tr,PV}+P_{T\mu}                                                                                                                             *   +P_{Ttr}}                                                                                                                                                                                            *  where $P_{T\mu}$ and  $P_{Ttr}$ are the transverse momenta (with respect                                                                                                                              *  to the beam line) of the two tracks.                                                                                                                                                                  */
  double pointer (Gaudi::XYZVector vtx, Gaudi::XYZVector P_tr,  Gaudi::XYZVector P_mu);
  
  double gettheta(const LHCb::Particle* vdau1, const LHCb::Particle* vdau2);
  double getphi(const LHCb::Particle* vdau1, const LHCb::Particle* vdau2);
  
  double ratio( double p1, double p2);
  
  double IsClose(const LHCb::Particle* p1, const LHCb::Particle* p2);

  /** InCone: given two tracks returns the closest point, the doca and the angle                                                                                                                                       between the tracks                                                                                                                                                                                    */
  void InCone(Gaudi::XYZPoint o1, Gaudi::XYZVector p1,Gaudi::XYZPoint o2,
              Gaudi::XYZVector p2, Gaudi::XYZPoint& vtx, double& doca, double& angle);

  /** Closest_point: given two vectors and two application points finds for the two relevant                                                                                                                                *  straight lines the closest points along the lines and the middle one                                                                                                                                  *  between the such points                                                                                                                                                                               */
  void closest_point(Gaudi::XYZPoint o,Gaudi::XYZVector p, Gaudi::XYZPoint o_mu,Gaudi::XYZVector p_mu, 
                     Gaudi::XYZPoint& close1, Gaudi::XYZPoint& close2, Gaudi::XYZPoint& vertex, bool& fail);
  
  /// return the arcosine between two vectors
  double arcosine (Gaudi::XYZVector, Gaudi::XYZVector);

  
};



#endif // RELINFOBS2MUMUBISOLATIONS_H
