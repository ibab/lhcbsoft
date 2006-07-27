
#ifndef LESTER_RICH_PARAMS_H
#define LESTER_RICH_PARAMS_H

namespace Lester {

  class RichParams;

};

//#include "MarkovChainSampler/Ntuple.h"

#include "CircleParams.h"
#include <iostream>
#include <vector>
#include "CLHEP/Random/RandPoisson.h"
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/RandExponential.h"
#include "Rich.h"
#include "Constants.h"
#include "ProbabilityUtils.h"
#include "RichPriors.h"
#include "GenericRingFinder/GenericResults.h"

namespace Lester {
  
  class RichParams /*:
		     public Ntuple<7,double>*/ {
  public:
    typedef std::vector<CircleParams> Circs;
    Circs circs;
    //static const bool backgroundIsVariable = false;
    double meanBackground;
  public:
     void fill(GenRingF::GenericResults & results) const {
       results.rings.clear();
       unsigned int i=0;
       for (Circs::const_iterator it = circs.begin();
	   it!=circs.end();
	   ++it) {
	results.rings.push_back(GenRingF::GenericRing(i++, it->centre().x(), it->centre().y(), it->radius()));
      };
      results.meanBackground = meanBackground;
    };
   RichParams(const GenRingF::GenericResults & results) {
      for (std::list<GenRingF::GenericRing>::const_iterator it = results.rings.begin();
	   it!=results.rings.end();
	   ++it) {
	circs.push_back(CircleParams(Hep2Vector(it->x(), it->y()), it->radius()));
      };
      meanBackground = results.meanBackground;
    };
  public:
    inline double priorProbability() const {
      double ans = 1;

      // in principle I should have
      //        ans *= Lester::poissonProb(rp.circs.size(),meanNumberOfRings);
      // followed by
      //        ans *= factorial(rp.circs.size()) 
      // to accound for a combinatorial factor in what is about to follow.
      // however this involves a needless double (cancelling) evaluation of n!,
      // where n=rp.circs.size(), so instead I write the following faster combination
      // of the two:
      
      // actually I have just realised that contrary to the comments above, since in my case the order DOES count, as I don't always sort my circles into some order, I should NOT multiply by the factorial factor mentioned above, so I should just stick  to using the poisson factor.  As a result, I comment out the three lines below and replace them by the one four below!
      //const double n = static_cast<double>(rp.circs.size());
      //const double mu = meanNumberOfRings;
      //ans *= exp(n*log(mu)-mu);  /* times n! divided by n! */
      // SUPERSEDED BY NEXT LINE      ans *= poissonProb(circs.size(),meanNumberOfRings);
      ans *= RichPriors::priorProbabilityOfNumberOfCircles(circs.size());
      //std::cout << "FLUM = " << RichPriors::priorProbabilityOfNumberOfCircles(circs.size()) << std::endl;
      for (Circs::const_iterator it = circs.begin();
	   it!=circs.end();
	   it++) {
	const CircleParams & c = *it;
	const double contribution = c.priorProbability();
	ans *= contribution;
	//std::cout << "GROM = " << contribution << std::endl;
      };

      // and now the part I left out for ages (by accident!) which ensures that we compare hypotheses with different numbers of circles on an equivalent footing.  This insertion is somewhat guessed, so would be a good idea to get a statistician to confirm what I am doing.

      // Basically, for every circle I multiply the prior by one over the expectation (under the circles prior) of the circles prior itself!

      const double two = 2;
      const double strangeFactor = (two*MathsConstants::sqrtPi);

      const double oneOnCharacteristicAreaForCentresWandering = strangeFactor*strangeFactor*(Constants::circleCenXSig*Constants::circleCenYSig);
      const double oneOnCharacteristicLengthForRadiiWandering = strangeFactor*(Constants::circleRadiusSigmaAboutMean);
      const double characteristicThingForOneCircle 
	= oneOnCharacteristicAreaForCentresWandering
	* oneOnCharacteristicLengthForRadiiWandering;

      const double characteristicThingForAllCircles 
	= pow(characteristicThingForOneCircle,circs.size());

      ans *= characteristicThingForAllCircles;

      //std::cout << "FLUMBIC " << characteristicThingForAllCircles<<std::endl;

      //std::cout << "With n= " << circs.size() << " prob= " << ans<<std::endl; 
      return ans;
    };

    bool operator==(const RichParams & other) const {
      return 
	circs==other.circs && 
	meanBackground == other.meanBackground;
    };
    RichParams() : meanBackground(Constants::backgroundMeanParameter) {
      // Beware! Variables are randomized!
      //jokeSetRandom();
    };
    std::ostream & printMeTo(std::ostream & os) const {
      int i=0;
      os << "RichParams[" << std::endl;
      for(Circs::const_iterator it=circs.begin();
	  it!=circs.end();
	  ++it) {
	os << "circ "<<i<<"="<<*it<<std::endl;
	++i;
      };
      os << ";SB="<<meanBackground<<"]";

      return os;
    };
    double sigmaMu() const {
      double ans=0;
      for (Circs::const_iterator it=circs.begin();
	   it!=circs.end();
	   ++it) {
	ans += it->mu();
      };
      ans += meanBackground;
      return ans;
    };
    double prob(const Hep2Vector & p) const {
      double ans=0;
      double sigmaMu=0;
      for (Circs::const_iterator it=circs.begin();
	   it!=circs.end();
	   ++it) {
	const double mu = it->mu();
	sigmaMu+= mu;
	ans += mu*RichPriors::priorProbabilityOfHitDueToCircle(p,*it);
      };
      sigmaMu += meanBackground;
      ans += meanBackground*RichPriors::priorProbabilityOfHitDueToBackground(p);
      return ans/sigmaMu;      
    };
    void jokeSetRandom /* deprecated! */ () {
      circs.clear();
      const int n=RichPriors::sampleFromNumberOfCirclesDistribution();
      for (int i=0; i<n; i++) {
	CircleParams circ;
	circ.jokeSetRandom();
	circs.push_back(circ);
      };
    };
    RichParams jitterSymm1() const {
      // Jitter ALL circles
      RichParams rp;
      for (Circs::const_iterator it=circs.begin();
	   it!=circs.end();
	   ++it) {
	rp.circs.push_back(it->jitterSymm1());
      };
      return rp;
    };
    //
    //inline double suggestJitteredBg() const {
    //assert(backgroundIsVariable); // it makes no sense to have called this routine otherwise!
    //const double trialnewbg = (meanBackground+0.3*RandGauss::shoot());
    //const double ans = ((trialnewbg>0)?trialnewbg:meanBackground);
    //return ans;
    //};
    //
    RichParams jitterSymm2() const {
      // Jitter SOME circles and or the bakckground
      const int siz = circs.size();
      const int sizPlusOptionalOne = siz; // the 1 is for the background!
      if (sizPlusOptionalOne>0) {
	const int numToJitter = lestersRandomNumberToJitter1(sizPlusOptionalOne);
	RichParams rp=*this;
	const std::vector<int> toJitter 
	  = getSetOfMDifferentIntsFromN(sizPlusOptionalOne, numToJitter);
	//std::cerr<<"j[" << numToJitter<<"]";
	for (int i=0; i<numToJitter; ++i) {
	  const int toJit = toJitter[i];
	  
	  /*
	    if (backgroundIsVariable) {
	    if (toJit<siz) {
	    // toJit represents a circle
	    rp.circs[toJit]=(rp.circs[toJit].jitterSymm1());
	    } else {
	    // toJit represents "the background"
	    assert(toJit==siz);
	    rp.meanBackground = rp.suggestJitteredBg();
	    };
	    } else {
	  */
	  // toJit ALWAYS represents a circle

	  assert(toJit>=0 && toJit<siz);
	  rp.circs[toJit]=(rp.circs[toJit].jitterSymm1());	    

	  /* }; */
	};      
	return rp;
      } else {
	return *this; // LESTER STRANGE
      };
    };
#ifdef LESTER_USE_GRAPHICS
    void draw(BasicCanvasInterface & canvas) const {
      for (Circs::const_iterator it=circs.begin();
	   it!=circs.end();
	   ++it) {
	it->draw(canvas);
      };
    };
#endif
  };


};

std::ostream & operator<<(std::ostream & os, const Lester::RichParams & rp);

#endif
