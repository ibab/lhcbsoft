// $Id: TaggerPionSameTool.h,v 1.6 2006-10-24 10:16:45 jpalac Exp $
#ifndef USER_TAGGERPIONSAMETOOL_H 
#define USER_TAGGERPIONSAMETOOL_H 1

// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/ToolFactory.h"
// from Event
#include "Event/ProtoParticle.h"
#include "Event/FlavourTag.h"

#include "Kernel/ITagger.h"
#include "Kernel/IGeomDispCalculator.h"
#include "INNetTool.h"

/** @class TaggerPionSameTool TaggerPionSameTool.h 
 *
 *  Tool to tag the B flavour with a PionSame Tagger
 *
 *  @author Marco Musy
 *  @date   30/06/2005
 */

class TaggerPionSameTool : public GaudiTool, 
	virtual public ITagger {

public: 
  /// Standard constructor
  TaggerPionSameTool( const std::string& type,
		      const std::string& name,
		      const IInterface* parent );
  virtual ~TaggerPionSameTool( ); ///< Destructor
  StatusCode initialize();    ///<  initialization
  StatusCode finalize  ();    ///<  finalization

  //-------------------------------------------------------------
  virtual LHCb::Tagger tag( const LHCb::Particle*, const LHCb::RecVertex*,
			    std::vector<const LHCb::Vertex*>&, LHCb::
			    Particle::ConstVector&);
  //-------------------------------------------------------------

private:
  void calcIP( const LHCb::Particle* , const LHCb::Vertex* , double& , double& );
  void calcIP( const LHCb::Particle* , const LHCb::RecVertex* , double& , double& );
  double pol2( double x, double a0, double a1 );

  double m_ProbMin;
  IGeomDispCalculator *m_Geom;
  INNetTool* m_nnet;
  std::string m_CombinationTechnique, m_NeuralNetName;

  //properties 
  double m_Pt_cut_pionS;
  double m_P_cut_pionS;
  double m_IP_cut_pionS;
  double m_dQcut_pionS;
  double m_lcs_pSu;
  double m_AverageOmega;
};
//===============================================================//
#endif // USER_TAGGERPIONSAMETOOL_H
