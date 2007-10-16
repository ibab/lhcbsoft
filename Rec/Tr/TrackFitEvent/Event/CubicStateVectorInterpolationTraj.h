// $Id: CubicStateVectorInterpolationTraj.h,v 1.1 2007-10-16 12:17:35 wouter Exp $
#ifndef LHCbKernel_CubicStateVectorInterpolationTraj_H
#define LHCbKernel_CubicStateVectorInterpolationTraj_H 1

// Includes
#include "Event/State.h"
#include "Event/StateVector.h"
#include "GaudiKernel/PhysicalConstants.h"
#include "Kernel/Trajectory.h"

namespace LHCb 
{
  /** @class CubicStateVectorInterpolationTraj CubicStateVectorInterpolationTraj.h
   * 
   * Cubic interpolating between two state vectors. Used for
   * TrackTraj. Eventuall, we should turn this into a trajectory.
   *
   * @author Wouter Hulsbergen
   * @date   10/10/2007
   * 
   */
  class CubicStateVectorInterpolationTraj : public Trajectory {
  public:
    typedef Gaudi::XYZPoint Point ;
    typedef Gaudi::XYZVector Vector ;

    /// Default constructor. Does nothing.
    CubicStateVectorInterpolationTraj() : Trajectory(0,0), m_zbegin(0), m_zend(0) {}
 
    /// Constructor from two states of state-vectors
    template<class StateT>
    CubicStateVectorInterpolationTraj( const StateT& begin,
                                   const StateT& end ) : Trajectory(0,0) { init(begin,end) ; }

    /// Constructor from single state(vector) and B-field
    template<class StateT>
    CubicStateVectorInterpolationTraj( const StateT& state,
                                   const Gaudi::XYZVector& bfield ) : Trajectory(0,0) { init(state,bfield) ; }

    /// Initialize with two states 
    template<class StateT>
    void init( const StateT& begin, const StateT& end ) ;

    /// Initialize with singel state and B field
    template<class StateT>
    void init( const StateT& state, const Gaudi::XYZVector& bfield ) ;
    

  public: // trajectory interface

    /// Clone this trajectory
    std::auto_ptr<Trajectory> clone() const { return std::auto_ptr<Trajectory>(new CubicStateVectorInterpolationTraj(*this)) ; }
    /// Position at location z
    Point position( double z ) const { return Point(x(z),y(z),z) ; }
    /// First derivative of position to z
    Vector direction( double z ) const { return Vector(tx(z),ty(z),1) ; }
    /// Second derivative of position to z
    Vector curvature( double z ) const { return Vector(omegax(z),omegay(z),0) ; }
    /// Distance in z until the deviation from the linear
    /// approximation differs from this trajectory by a given tolerance.
    double distTo1stError( double z, double tolerance, int pathDirection = +1 ) const ;
    /// Distance in z until the deviation from the quadratic
    /// approximation differs from this trajectory by a given tolerance.
    double distTo2ndError( double z, double tolerance, int pathDirection = +1 ) const ;
    /// Create a parabolic approximation to the trajectory
    void expansion(  double z, Gaudi::XYZPoint& p, Gaudi::XYZVector& dp, Gaudi::XYZVector& ddp  ) const ;
    /// Arclength between 2 z -locations (not yet imlpemented)
    double arclength(double z1, double z2) const ;
    /// Estimate for expansion parameter 'z' closest to point
    double muEstimate(const Point& p) const ;

  public: // other public methods
    /// Return x(z)
    double x(double z) const { return polyeval(z-m_zbegin,m_cx) ; }
    /// Return dx/dz
    double tx(double z) const { return poly1stderiveval(z-m_zbegin,m_cx) ; }
    /// Return d2x/dz2
    double omegax(double z) const { return poly2ndderiveval(z-m_zbegin,m_cx) ; }
    /// Return y(z)
    double y(double z) const { return polyeval(z-m_zbegin,m_cy) ; }
    /// Return dy/dz
    double ty(double z) const { return poly1stderiveval(z-m_zbegin,m_cy) ; }
    /// Return d2y/dz2
    double omegay(double z) const { return poly2ndderiveval(z-m_zbegin,m_cy) ; }
    
    /// Return the z of the begin point
    double zbegin() const { return m_zbegin ; }
    /// Return the z of the end point
    double zend()   const { return m_zend ; }
    
    /// qop at position z
    double qop(double z) const ;
    /// momentum vector at position z
    Gaudi::XYZVector momentum(double z) const { return std::abs(1/qop(z)) * Gaudi::XYZVector(tx(z),ty(z),1).Unit() ; }
    
  private:
    void interpolate1D( double za, double xa, double txa, double zb, double xb, double txb,
			double coeff[4] ) const ;
    double polyeval( double dz, const double c[4]) const { return c[0] + dz*(c[1] + dz * (c[2] + dz*c[3])) ; }
    double poly1stderiveval( double dz, const double c[4]) const { return c[1] + dz*(2*c[2] + dz*3*c[3]) ; }
    double poly2ndderiveval( double dz, const double c[4]) const { return 2*c[2] + dz*6*c[3] ; }
    
  private:
    double m_zbegin ;   ///< z-position of first state
    double m_zend ;     ///< z-position of second state
    double m_cx[4] ;    ///< parameters of x(z)
    double m_cy[4] ;    ///< parameters of y(z)
    double m_qopbegin ; ///< q/p at first state
    double m_qopend ;   ///< q/p at second state
  } ;

  /*************************************************************************************************/
  // inline functions
  /*************************************************************************************************/

  template<class StateT> void  
  CubicStateVectorInterpolationTraj::init( const StateT& begin,
                                       const StateT& end ) 
  {
    m_zbegin = begin.z() ;
    m_zend   = end.z() ;
    m_qopbegin = begin.qOverP() ;
    m_qopend   = end.qOverP() ;
    interpolate1D(begin.z(), begin.x(), begin.tx(), end.z(), end.x(), end.tx(), m_cx) ;
    interpolate1D(begin.z(), begin.y(), begin.ty(), end.z(), end.y(), end.ty(), m_cy) ;
  }
  
  template<class StateT> void  
  CubicStateVectorInterpolationTraj::init( const StateT& state,
                                       const Gaudi::XYZVector& bfield)
  {
    m_zbegin   = m_zend = state.z() ;
    m_qopbegin = m_qopend = state.qOverP() ;
    double n = std::sqrt( 1 + state.tx()*state.tx() + state.ty()*state.ty()) ;
    m_cx[0] = state.x() ;
    m_cx[1] = state.tx() ;
    m_cx[2] = n * Gaudi::Units::c_light * m_qopbegin * ( -bfield.y() + state.ty()* bfield.z() ) ;
    m_cx[3] = 0 ;
    m_cy[0] = state.y() ;
    m_cy[1] = state.ty() ;
    m_cy[2] = n * Gaudi::Units::c_light * m_qopbegin * (  bfield.x() - state.tx()* bfield.z() ) ;
    m_cy[3] = 0 ;
  }
  
  inline double CubicStateVectorInterpolationTraj::distTo1stError( double z, double tolerance, int pathDirection ) const 
  {
    double deriv = poly2ndderiveval(z-m_zbegin,m_cx) ;
    // validity range of linear approximation cannot be larger than that of parabolic!
    return std::min( deriv!=0 ? std::sqrt(std::abs(2*tolerance/deriv)) :10*Gaudi::Units::km,
                     distTo2ndError(z,tolerance,pathDirection)) ;
  }
  
  inline double CubicStateVectorInterpolationTraj::distTo2ndError( double /*z*/, double tolerance, int /*pathDirection*/ ) const 
  {
    // need to replace this with a sensible check on size of c3
    return m_cx[3]!=0 ? std::pow(std::abs(tolerance/m_cx[3]), 1.0/3.0) : 10*Gaudi::Units::km ;
  }
  
  inline double CubicStateVectorInterpolationTraj::qop(double z) const 
  { 
    return z<=m_zbegin ? m_qopbegin : 
      ( z>=m_zend ? m_qopend : m_qopbegin + (m_qopend-m_qopbegin)*(z-zbegin())/(zend()-zbegin()) ) ; 
  }
  
  inline void CubicStateVectorInterpolationTraj::expansion( double z, Gaudi::XYZPoint& p, Gaudi::XYZVector& dp,
                                                        Gaudi::XYZVector& ddp ) const {
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

  inline void CubicStateVectorInterpolationTraj::interpolate1D( double za, double xa, double txa, 
                                                                double zb, double xb, double txb,
                                                                double c[4] ) const
  {
    c[0] = xa ;
    c[1] = txa ;
    double d  = zb-za ;
    c[2] = (3*(xb-xa)-d*(2*txa+txb))/(d*d) ;
    c[3] = (txb-txa-2*c[2]*d)/(3*d*d) ;
  
    //     // we want to prevent strange wiggle. a straneg wiggle is a zero
    //     // in the 2nd derivative. let's call ifs two coeffs b0 and b1:
    //     double b1 = 2*3*c[3] ;
    //     if( fabs(b1) > 0 ) {
    //       double b0 = 2*c[2] ;
    //       double zzero = - b0 / b1 ;
    //       if( zzero > za && zzero < zb ) {
    //         // pol3 interpolation gives strange wiggle. let's go 1 order
    //         // down. this uses the constraints on xa and xb and an
    //         // 'equally wrong' cnstraint for txa and txb,
    //         c[0] = xa ;
    //         c[1] = (txa-txb)/2 + (xb-xa)/d ;
    //         c[2] = (xb-xa-c[1]*d)/(d*d) ;
    //         c[3] = 0 ;
    //       }
    //     }
  }
}
#endif
