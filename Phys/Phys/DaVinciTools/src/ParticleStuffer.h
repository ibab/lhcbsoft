// $Id: ParticleStuffer.h,v 1.1 2002-03-27 20:35:01 gcorti Exp $
#ifndef PARTICLESTUFFER_H 
#define PARTICLESTUFFER_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/AlgTool.h"

// from DaVinciTools
#include "DaVinciTools/IParticleStuffer.h"

/** @class ParticleStuffer ParticleStuffer.h
 *
 *  @author Paul Colrain
 *  @date   14/03/2002
 */
class ParticleStuffer : public AlgTool, virtual public IParticleStuffer {

public:

  /// Standard constructor
  ParticleStuffer( const std::string& type, 
                   const std::string& name,
                   const IInterface* parent);
  
  /// Desctructor
  virtual ~ParticleStuffer( ){ }; 
  
  /// Fill Composite Particle from Vertex
  StatusCode fillParticle( const Vertex&, Particle&, 
                           const ParticleID& );

  /// Fill Particle from ProtoParticle
  //StatusCode fillParticle( const ProtoParticle&, Particle&, 
  //                         const ParticleID& );

};

#endif // PARTICLESTUFFER_H
