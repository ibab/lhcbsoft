
//--------------------------------------------------------------------------------
/** @file RichMap.h
 *
 *  Header file for utility has map for the RICH : Rich::Map
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   2005-01-11
 */
//--------------------------------------------------------------------------------

#ifndef RICHKERNEL_RICHMAP_H
#define RICHKERNEL_RICHMAP_H 1

// STL
#include <map>
#include <ostream>

namespace Rich
{

  //--------------------------------------------------------------------------------
  /** @class Map RichMap.h RichKernel/RichMap.h
   *
   *  A utility class providing a standard std::map like object.
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   2005-01-11
   */
  //--------------------------------------------------------------------------------

  template < typename KEY, 
             typename VALUE, 
             typename COMPARE = std::less<KEY>,
             typename ALLOC   = std::allocator< std::pair<const KEY,VALUE> >
             >
  class Map : public std::map < KEY , VALUE , COMPARE , ALLOC >
  {
  public:
    /// Operator overloading for ostream
    friend inline std::ostream& operator << ( std::ostream& str ,
                                              const Map<KEY,VALUE,COMPARE,ALLOC> & m )
    {
      str << "[";
      for ( const auto& i : m ) { str << " (" << i.first << "," << i.second << ")"; }
      return str << " ]";
    }
  };

}

#endif // RICHKERNEL_RICHMAP_H
