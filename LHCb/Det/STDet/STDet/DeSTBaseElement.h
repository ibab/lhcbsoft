// $Id: DeSTBaseElement.h,v 1.3 2006-01-26 09:54:29 mneedham Exp $
#ifndef _DeSTBaseElement_H_
#define _DeSTBaseElement_H_

#include <string>
#include <vector>

#include "STDet/STDetTraits.h"
#include "Kernel/STChannelID.h"
#include "DetDesc/DetectorElement.h"
#include "GaudiKernel/GaudiException.h"

class DeSTBaseElement : public DetectorElement  {

public:

  /** Constructor */
  DeSTBaseElement ( const std::string& name = "" ) ;

  /** Destructor */
  virtual ~DeSTBaseElement(); 
 
  /** initialization method 
  * @return Status of initialisation
  */
  virtual StatusCode initialize();

  /** check whether point is inside element
  * @param point 
  * @return true if inside
  */
  bool isInside(const Gaudi::XYZPoint& point) const;

  /** transform global to local point
  * @param global point 
  * @return local point
  */
  Gaudi::XYZPoint toLocal(const Gaudi::XYZPoint& point) const;

  /** transform global to local point
  * @param local point 
  * @return global point
  */
  Gaudi::XYZPoint toGlobal(const Gaudi::XYZPoint& point) const;

  /** detector element id  - for experts only !*/
  LHCb::STChannelID elementID() const;

  /** set detector element id  - for experts only !*/
  void setElementID(const LHCb::STChannelID& chanID);

  /** get the parent of the detector element 
  @return parenttype */
  template <typename TYPE>
  typename STDetTraits<TYPE>::parent* getParent() const;

  /** retrieve the children
  * @return children  
  */
  template <typename TYPE>
  std::vector<typename STDetTraits<TYPE>::child*> getChildren();

private:

  LHCb::STChannelID m_elementID;
 
};

#include "DetDesc/IGeometryInfo.h"

inline bool DeSTBaseElement::isInside(const Gaudi::XYZPoint& point) const{
  return this->geometry()->isInside(point);
}

inline Gaudi::XYZPoint DeSTBaseElement::toLocal(const Gaudi::XYZPoint& point) const{
  return this->geometry()->toLocal(point);
}

inline Gaudi::XYZPoint DeSTBaseElement::toGlobal(const Gaudi::XYZPoint& point) const{
  return this->geometry()->toGlobal(point);
}

template <typename TYPE>
inline typename STDetTraits<TYPE>::parent* DeSTBaseElement::getParent() const{

  typedef typename STDetTraits<TYPE>::parent parentType;
  parentType* parent = dynamic_cast<parentType*>(this->parentIDetectorElement());
  if (parent == 0) {
    throw GaudiException ("Orphaned detector element", "DeSTBaseElement", 
                           StatusCode::FAILURE);
  }
  return parent;
}

inline LHCb::STChannelID DeSTBaseElement::elementID() const{
  return m_elementID;
}

inline void DeSTBaseElement::setElementID(const LHCb::STChannelID& chanID){
  m_elementID = chanID;
}

template<typename TYPE>
inline std::vector<typename STDetTraits<TYPE>::child*> DeSTBaseElement::getChildren(){

  typedef typename STDetTraits<TYPE>::child cType;
  std::vector<cType*> childVector;
  IDetectorElement::IDEContainer::const_iterator iChild;
  for (iChild = this->childBegin(); this->childEnd() != iChild; ++iChild) {
    cType* aChild = dynamic_cast<cType*>(*iChild);
    if (aChild !=0) childVector.push_back(aChild);
  } // iStation

  if (childVector.empty()) {
    throw GaudiException ("Sterile detector element", "DeSTBaseElement", 
                           StatusCode::FAILURE);
  }

  return childVector;
}

#endif
