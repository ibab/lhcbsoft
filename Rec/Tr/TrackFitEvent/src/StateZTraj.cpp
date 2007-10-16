// $Id: StateZTraj.cpp,v 1.1 2007-10-16 12:17:35 wouter Exp $
// Include files

// local
#include "Event/StateZTraj.h"

namespace LHCb
{
  void StateZTraj::expansion( double z, Gaudi::XYZPoint& p, Gaudi::XYZVector& dp, Gaudi::XYZVector& ddp ) const 
  {
    p.SetX(   x(z)  ) ;
    p.SetY(   y(z)  ) ;
    p.SetZ(   z     ) ;
    dp.SetX(  tx(z) ) ;
    dp.SetY(  ty(z) ) ;
    dp.SetZ(  1     ) ;
    ddp.SetX( omegax(z) ) ;
    ddp.SetY( omegay(z) ) ;
    ddp.SetZ( 0         ) ;
  }

  double StateZTraj::muEstimate( const Gaudi::XYZPoint& p ) const 
  {
    Gaudi::XYZVector dir = direction(p.z()) ;
    Gaudi::XYZVector dx  = p - position(p.z()) ;
    double dir2 = dir.mag2() ;
    double det  = dir2 - curvature(p.z()).Dot(dx) ;
    if(det<=0) det = dir2 ;
    return p.z() + dx.Dot(dir)/det ;
  }
  
  StateZTraj::Parameters StateZTraj::parameters() const
  {
    return Parameters( m_cx[0], m_cy[0], m_cx[1], m_cy[1], m_qOverP );
  }

  StateZTraj& StateZTraj::operator+=(const Parameters& /*delta*/)
  {
    // to implement this we need the full b-field. 
    std::cout << "StateZTraj::operator+= not yet implemented." << std::endl ;
    return *this ;
  }
  

  StateZTraj::Derivative StateZTraj::derivative( double z ) const
  {
    double tx = m_cx[1] ;
    double ty = m_cy[1] ;
    double omegax = m_cx[2] ;
    double omegay = m_cy[2] ;
    double n  = std::sqrt(1 + tx*tx + ty*ty ) ;
    double dndtx = tx/n ;
    double dndty = ty/n ;
    double dz = z - m_z ;

    Derivative deriv;
    deriv(0,0) = 1;
    deriv(0,2) = dz + 0.5 * dz * dz * omegax/n * dndtx ;
    deriv(0,3) = 0.5 * dz * dz * (omegax/n * dndty + n * m_qOverP * Gaudi::Units::c_light * m_Bz ) ;
    deriv(0,4) = 0.5 * dz * dz * omegax/m_qOverP ;
    
    deriv(1,1) = 1;
    deriv(1,2) = 0.5 * dz * dz * (omegax/n * dndtx - n * m_qOverP * Gaudi::Units::c_light * m_Bz ) ;
    deriv(1,3) = dz + 0.5 * dz * dz * omegax/n * dndty ;
    deriv(1,4) = 0.5 * dz * dz * omegay/m_qOverP ;

    return deriv;        
  }
}
