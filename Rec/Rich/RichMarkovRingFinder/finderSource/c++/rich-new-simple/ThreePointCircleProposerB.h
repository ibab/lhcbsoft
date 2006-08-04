
#ifndef LESTER_THREEPOINTCIRCLEPROPOSERB_H
#define LESTER_THREEPOINTCIRCLEPROPOSERB_H

// fwd dec
#include "ThreePointCircleProposerB.fwd" 

// includes
#include <iostream>
#include <map>
#include "NewCircleProposer.h"
#include "Data.h"
#include "Utils/NormalDistribution.h"
#include "HitIndexTriple.h"
#include "RegularCPQuantizer.h"

// declaration
namespace Lester {
class ThreePointCircleProposerB : public NewCircleProposer {
// ThreePointCircleProposerB(const ThreePointCircleProposerB & other) {};
// ThreePointCircleProposerB & operator=(const ThreePointCircleProposerB & other) { return *this; };
// virtual ~ThreePointCircleProposerB() {};
private:
  typedef std::map<HitIndexTriple, double> Cache1;
  mutable Cache1 m_cache1;
  typedef std::map<QuantizedCircleParams, long> VisitCache;
  mutable VisitCache m_visitCache;
  const Data & m_data;
  const RegularCPQuantizer & m_cpQuantizer;
  CircleParams tryToSample() const;
  CircleParams samplePrivate() const;
  double CACHED_probabilityOfBasingACircleOn(const int i1,
					     const int i2,
					     const int i3) const;
  double probabilityOfBasingACircleOn(const int i1,
				      const int i2,
				      const int i3) const;
  double probabilityOfBasingACircleOn(const int i1,
				      const int i2,
				      const int i3,
				      const CircleParams & whatItSmearedTo) const;
  const double m_circleCentreSmearingWidth;
  const double m_radiusSmearingWidth;
  const double m_wanderWidthSq;
  const NormalDistribution m_centreCptSmearer;
public:
  /// The circleCentreSmearingWidth is the width in x (and also in y) by which the circle centre will be smeared out before being returned by the sample method. "radiusSmearingWidth" has the same role. The "...Sq" terminator indicates the square of the quantity has been taken.
  ThreePointCircleProposerB(const Data & d,
			    const RegularCPQuantizer & cpq,
			    const double circleCentreSmearingWidthSq,
			    const double radiusSmearingWidthSq);
  std::ostream & printMeTo(std::ostream & os) const {
    os << "ThreePointCircleProposerB[]";
    return os;
  };
  CircleParams sample() const;
  double probabilityOf(const CircleParams & circleParams) const;
};
};

inline std::ostream & operator<<(std::ostream & os, const Lester::ThreePointCircleProposerB & obj) {
  return obj.printMeTo(os);
};

#endif

