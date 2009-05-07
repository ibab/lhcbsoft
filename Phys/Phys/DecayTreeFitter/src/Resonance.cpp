#include <iomanip>
#include <algorithm>

#include "DecayTreeFitter/VtkResonance.h"

namespace vtxtreefit
{

  extern int vtxverbose ;

  Resonance::Resonance(const LHCb::Particle& bc, const ParticleBase* mother, 
		       bool forceFitAll)
    : InternalParticle(bc,mother,forceFitAll)
  {
  }

  Resonance::~Resonance() {}

  ErrCode Resonance::initPar1(FitParams* fitparams)
  {
    ErrCode status ;
    for(daucontainer::const_iterator it = begin() ; it != end() ; ++it) 
      status |= (*it)->initPar1(fitparams) ;
    return status ;
  }

  ErrCode Resonance::initPar2(FitParams* fitparams)
  {
    ErrCode status ;
    for(daucontainer::const_iterator it = begin() ; it != end() ; ++it) 
      status |= (*it)->initPar2(fitparams) ;
    initMom( fitparams ) ;
    return status ;
  }

  std::string Resonance::parname(int index) const
  {
    return ParticleBase::parname(index+4) ;
  }

}
