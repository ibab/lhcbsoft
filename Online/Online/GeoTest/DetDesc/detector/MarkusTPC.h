#include "DetDesc/compact/Detector.h"

namespace DetDesc {

  struct TPCData;

  struct MarkusTPC : public Geometry::Subdetector {
    typedef TPCData Object;
    MarkusTPC(const Geometry::RefElement_type<TNamed>& e) : Geometry::Subdetector(e) {}
    double feature() const;
  };
}
