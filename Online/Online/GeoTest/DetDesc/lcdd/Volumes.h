#ifndef DETDESC_GEOMETRY_VOLUMES_H
#define DETDESC_GEOMETRY_VOLUMES_H

// Framework include files
#include "DetDesc/Elements.h"
#include "DetDesc/lcdd/Shapes.h"
#include "DetDesc/lcdd/Objects.h"

// C/C++ include files
#include <map>

#include "TGeoShape.h"
// Forward declarations
class TGeoVolume;

/*
 *   DetDesc namespace declaration
 */
namespace DetDesc {

  class IDDescriptor;

  /*
  *   XML namespace declaration
  */
  namespace Geometry  {

    // Forward declarations
    struct LCDD;
    struct Region;
    struct LimitSet;
    struct Material;
    struct Volume;
    struct PhysVol;
    struct Position;
    struct Rotation;
    struct VisAttr;
    struct SensitiveDetector;
  
    /** @class Volume Volume.h  DetDesc/lcdd/Volume.h
     *  
     *  @author  M.Frank
     *  @version 1.0
     */
    struct Volume : public RefElement_type<TGeoVolume>  {
      typedef RefElement_type<TGeoVolume> Base;
      struct Object  {
        Region            Attr_region;
        LimitSet          Attr_limits;
        VisAttr           Attr_vis;
        RefElement_type<TNamed>        Attr_sens_det;
        Object() : Attr_region(), Attr_limits(), Attr_vis(), Attr_sens_det() {}
      };
      /// Default constructor
      Volume() : Base(0) {}

      /// Copy from handle
      Volume(const Volume& v) : Base(v) {}

      /// Copy from arbitrary Element
      template <class T> Volume(const Element_type<T>& v) : Base(v) {}

      /// Constructor to be used when creating a new geometry tree.
      Volume(LCDD& lcdd, const std::string& name);

      /// Constructor to be used when creating a new geometry tree. Also sets materuial and solid attributes
      Volume(LCDD& lcddument, const std::string& name, const Solid& s, const Material& m);

      void setSolid(const Solid& s)  const;
      void setMaterial(const Material& m)  const;
      void addPhysVol(const PhysVol& vol, const Transform& tr)  const;
      void addPhysVol(const PhysVol& vol, const Position& pos)  const;
      void addPhysVol(const PhysVol& vol, const Position& pos, const Rotation& rot)  const;
      void setRegion(const Region& obj)  const;
      void setLimitSet(const LimitSet& obj)  const;
      void setSensitiveDetector(const SensitiveDetector& obj) const;
      void setVisAttributes(const VisAttr& obj) const;
      Solid solid() const;
      Material material() const;
      VisAttr  visAttributes() const;
      RefElement_type<TNamed> sensitiveDetector() const;
      Region region() const;
      /// Auto conversion to underlying ROOT object
      operator TGeoVolume*() const     { return m_element; }
    };

    /** @class PhysVol Volume.h  DetDesc/lcdd/Volume.h
     *  
     *  @author  M.Frank
     *  @version 1.0
     */
    struct PhysVol : RefElement_type<TGeoVolume> {
      /// Constructor to be used when reading the already parsed DOM tree
      template <class T> PhysVol(const Element_type<T>& e) : RefElement_type<TGeoVolume>(e) {}
      /// Add identifier
      PhysVol& addPhysVolID(const std::string& name, int value);
      /// Auto conversion to underlying ROOT object
      operator TGeoVolume*() const     { return m_element; }
    };
  }       /* End namespace Geometry           */
}         /* End namespace DetDesc            */
#endif    /* DETDESC_GEOMETRY_VOLUMES_H       */
