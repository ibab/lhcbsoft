// $Id: Trajectory.h,v 1.11 2006-04-06 14:06:09 ebos Exp $
#ifndef LHCbKernel_Trajectory_H
#define LHCbKernel_Trajectory_H 1

// Includes

// from Kernel/LHCbDefinitions
#include "Kernel/Point3DTypes.h"
#include "Kernel/Vector3DTypes.h"
#include "Kernel/GenericMatrixTypes.h"
#include "Kernel/PhysicalConstants.h"
#include <utility>
#include <memory>

namespace LHCb 
{

  /** @class Trajectory Trajectory.h
   *
   * This is the base class for the trajectory classes. 
   *
   * @author Edwin Bos, Jeroen van Tilburg, Eduardo Rodrigues
   * @date   01/12/2005
   * 
   */
  
  class Trajectory {
    
  public:
    typedef std::pair<double,double> Range;

    /// Clone a trajectory...
    virtual std::auto_ptr<Trajectory> clone() const = 0;

    /// Constructor taking the values of arclength
    /// at the begin and at the end of the trajectory
    Trajectory( double begin, double end ) 
    {
      m_range = Range(begin,end);
    }
    
    /// Point on the trajectory at arclength from the starting point
    virtual Gaudi::XYZPoint position( double arclength ) const = 0;

    /// Beginpoint of the Trajectory
    Gaudi::XYZPoint beginPoint() const { return position( beginRange() ); }

    /// Endpoint of the Trajectory
    Gaudi::XYZPoint endPoint() const { return position( endRange() ); }
    
    /// First derivative of the trajectory at arclength from the starting point
    virtual Gaudi::XYZVector direction( double arclength ) const = 0;
    
    /// Second derivative of the trajectory at arclength from the starting point
    virtual Gaudi::XYZVector curvature( double arclength ) const = 0;
    
    /// Create a parabolic approximation to the trajectory
    /// at arclength from the starting point
    virtual void expansion( double arclength,
                            Gaudi::XYZPoint& p,
                            Gaudi::XYZVector& dp,
                            Gaudi::XYZVector& ddp ) const = 0;
    
    /// Determine the distance in arclenghts to the
    /// closest point on the trajectory to a given point
    virtual double arclength( const Gaudi::XYZPoint& ) const = 0;
    
    /// Number of arclengths until deviation of the trajectory from the expansion
    /// reaches the given tolerance.
    virtual double distTo1stError( double arclength,
                                   double tolerance, 
                                   int pathDirection = +1 ) const = 0;
    
    /// Number of arclengths until deviation of the trajectory from the expansion
    /// reaches the given tolerance.
    virtual double distTo2ndError( double arclength,
                                   double tolerance, 
                                   int pathDirection = +1 ) const = 0;
    
    /// Range in arclength w.r.t. the starting point
    /// over which the trajectory is valid
    Range range() const { return m_range; }
    
    /// Maximal valid value of arclength according to range()
    double endRange() const { return range().second; }
    
    /// Minimal valid value of arclength according to range()
    double beginRange() const { return range().first; }
    
    /// Restrict arclength to the valid range of the trajectory
    double restrictToRange( double arclength ) const 
    {
      Range r = range(); 
      return std::max( r.first, std::min(arclength, r.second ) );
    }
    
    /// Length of trajectory
    double length() const { Range r = range(); return r.second-r.first; };

  private:

    Range m_range;
    
  }; // class Trajectory
  
} // namespace LHCb

#endif /// LHCbKernel_Trajectory_H
