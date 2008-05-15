#include <ostream>
#include <boost/functional/hash.hpp>
#include "GaudiKernel/Property.h"

class stringKey {

public:

    stringKey(const std::string& str) : m_str(str), m_hash( hasher(str) ) 
    { m_str.declareUpdateHandler( &stringKey::updateHandler,this); }
    const std::string& str() const { return m_str.value();}
    bool operator==(const stringKey& rhs) const ;
    bool operator!=(const stringKey& rhs) const ;
    bool operator<(const stringKey& rhs) const ;
    std::ostream& print(std::ostream & o ) const ;

    // yes, this should work, as the real string is carried along as reference, 
    // and survives copy-construction of properties!!!...
    // hopefully, so does the updatehandler...
    StringProperty& property() { return m_str; }
    void updateHandler(Property& prop) { 
        std::cout <<" updating " << m_str << " -> " << prop.toString() << std::endl;  
        m_str = prop.toString();
        m_hash = hasher(str());
    }

// transitional functionality -- to be deleted when no longer needed...
    stringKey() : m_hash( 0 ) { }
    bool empty() const { return str().empty(); }

private:
    bool valid() const { return m_hash!=0 && !str().empty() && hasher(str())==m_hash; }

    static boost::hash<std::string> hasher;

    StringProperty m_str;
    // note: NEVER use m_hash for anything stored in files, as it is NOT 
    //       guaranteed that the hashing scheme will remain the same 
    //       during the lifetime of LHCb.
    // note: the only reason m_hash exists is to increase the speed
    //       of searching for keys in (sorted) lists,maps,...
    // note: hence you do NOT get access to it... period.
    size_t m_hash;
};

inline std::ostream& operator<< (std::ostream& os, const stringKey& k) 
{ return k.print(os); }


inline bool stringKey::operator==(const stringKey& rhs) const 
{ assert(valid()); return m_hash==rhs.m_hash && str() == rhs.str();}

inline bool stringKey::operator!=(const stringKey& rhs) const 
{ assert(valid()); return !operator==(rhs);}

inline bool stringKey::operator<(const stringKey& rhs) const 
{ assert(valid());
  return ( m_hash == rhs.m_hash ) ? ( str() < rhs.str() )
                                  : ( m_hash < rhs.m_hash ); 
}

inline std::ostream& stringKey::print(std::ostream & o ) const 
{ assert(valid()); 
  return  o << "stringKey(" << str() << ")"; 
}

