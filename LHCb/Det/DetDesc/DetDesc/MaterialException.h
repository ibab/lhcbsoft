// $Header: /afs/cern.ch/project/cvs/reps/lhcb/Det/DetDesc/DetDesc/MaterialException.h,v 1.4 2003-01-17 14:03:38 sponce Exp $
#ifndef    DETDESC_MATERIALEXCEPTION_H
#define    DETDESC_MATERIALEXCEPTION_H
/// GaudiKernel
#include "GaudiKernel/GaudiException.h"
//// STL
#include <iostream>
#include <exception>
//
class Material;
class MSgStream;
///

///
class MaterialException: public GaudiException
{
  ///
public:
  ///
  MaterialException( const std::string    & message     , 
                     const Material       * mat     = 0 );
  ///
  MaterialException( const std::string    &  message    ,
                     const GaudiException &  Exception  ,  
                     const Material       *  mat    = 0 );
  ///
  MaterialException( const MaterialException& );
  ///
  virtual ~MaterialException() throw();
  ///
  virtual std::ostream& printOut ( std::ostream& os = std::cerr ) const;
  virtual MsgStream&    printOut ( MsgStream&    os             ) const;
  ///
  virtual inline GaudiException* clone() const {
    return new MaterialException( *this );
  } 
  ///
private:
  ///
  const Material* m_me_mat;
  ///
}; 
///

#endif  // DETDESC_MATERIALEXCEPTION_H
