// $Id: DeITLadder.h,v 1.1 2006-01-26 12:41:19 mneedham Exp $
#ifndef _DeITLadder_H_
#define _DeITLadder_H_

#include <string>
#include <vector>
#include "STDet/DeSTBaseElement.h"

class DeITSector;
class DeITLadder;

/** @class DeITLadder DeITLadder.h "STDet/DeITLadder.h"
 *
 *  IT Station detector element
 *
 *  @author Matthew Needham Matthew.Needham@cern.ch
 */

static const CLID& CLID_DeITLadder = 9205; // needs fixing !

class DeITLadder : public DeSTBaseElement  {

public:

   /** parent type */
   typedef STDetTraits<DeITLadder>::parent parent_type;

   /** child type */
   typedef STDetTraits<DeITLadder>::child child_type;

   /** Constructor */
   DeITLadder ( const std::string& name = "" ) ;

   /** Destructor */
   virtual ~DeITLadder(); 

   /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
   static const CLID& classID(){return CLID_DeITLadder;}
                        
   /**
   * another reference to class identifier
   * @return the class identifier for this class
   */
   const CLID& clID () const;
                                                        
   /** initialization method 
   * @return Status of initialisation
   */
   virtual StatusCode initialize();


   /** check whether contains 
   *  @param channel
   *  @return bool
   */ 
   bool contains(const LHCb::STChannelID aChannel) const; 

   /** identifier */
   unsigned int id() const;

   /** child type */
   DeITLadder::child_type* sector() const;

   /** print to stream */
   std::ostream& printOut( std::ostream& os ) const;
     
   /** print to msgstream */
   MsgStream& printOut( MsgStream& os) const;

private:

   child_type* m_child;
   parent_type* m_parent;
   unsigned int m_id;
};

#include "STDet/DeITSector.h"

inline unsigned int DeITLadder::id() const{
  return m_id;
}

inline DeITLadder::child_type* DeITLadder::sector() const{
  return m_child;
}

inline bool DeITLadder::contains(const LHCb::STChannelID aChannel) const{
  return ((aChannel.detRegion() == elementID().detRegion()) 
         && m_parent->contains(aChannel)) ;
}


/** ouput operator for class DeITLadder
 *  @see DeITLadder
 *  @see MsgStream
 *  @param os reference to STL output stream
 *  @param de reference to DeITLadder object
 */
inline std::ostream& operator<<( std::ostream& os , const DeITLadder* aLadder )
{ return aLadder->printOut( os ); }


/** ouput operator for class DeITLadder
 *  @see DeITLadder
 *  @see MsgStream
 *  @param os reference to MsgStream output stream
 *  @param de reference to DeITLadder object
 */
inline MsgStream& operator<<( MsgStream& os , const DeITLadder* aLadder )
{ return aLadder->printOut( os ); }

#endif // _DeITLadder_H








