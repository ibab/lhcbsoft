// $Id: IBestParticles.h,v 1.2 2004-07-28 14:15:23 pkoppenb Exp $
#ifndef DAVINCITOOLS_IBESTPARTICLES_H 
#define DAVINCITOOLS_IBESTPARTICLES_H 1

// Include files
// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "Event/Particle.h"

static const InterfaceID IID_IBestParticles ( "IBestParticles", 1, 0 );

/** @class IBestParticles IBestParticles.h DaVinciTools/IBestParticles.h
 *  
 *
 *  @author Patrick KOPPENBURG
 *  @date   2004-07-15
 */
class IBestParticles : virtual public IAlgTool {
public: 

  // Return the interface ID
  static const InterfaceID& interfaceID() { return IID_IBestParticles; }
  virtual StatusCode Candidates( const ParticleVector & input,
                                 ParticleVector& output) = 0;
  //  virtual StatusCode Candidates( const Particles & input,
  //                                 Particles& output) = 0;
  virtual StatusCode BestCandidate( ParticleVector& input,
                                    Particle* output) = 0;
  //  virtual StatusCode BestCandidate( Particles& input,
  //                                   Particle* output) = 0;

protected:

private:

};
#endif // DAVINCITOOLS_IBESTPARTICLES_H
