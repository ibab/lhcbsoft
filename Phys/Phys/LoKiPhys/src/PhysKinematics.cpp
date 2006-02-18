// $Id: PhysKinematics.cpp,v 1.2 2006-02-18 18:09:15 ibelyaev Exp $
// ============================================================================
// CVS tag $Name: not supported by cvs2svn $, version $Revision: 1.2 $
// ============================================================================
// $Log: not supported by cvs2svn $
// Revision 1.1.1.1  2006/02/17 19:17:26  ibelyaev
// New package: "Phys"-part of restructurized LoKi project  
// 
// ============================================================================
// Include files
// ============================================================================
// Event 
// ============================================================================
#include "Event/Particle.h"
// ============================================================================
// LoKiCore
// ============================================================================
#include "LoKi/Constants.h"
#include "LoKi/Report.h"
// ============================================================================
// LoKiPhys 
// ============================================================================
#include "LoKi/PhysKinematics.h"
// ============================================================================

// ============================================================================
/** @file
 *
 * Implementation file for functions from the file LoKi/PhysKinematics.h
 *
 *  This file is a part of LoKi project - 
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
 *  contributions and advices from G.Raven, J.van Tilburg, 
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  By usage of this code one clearly states the disagreement 
 *  with the campain of Dr.O.Callot et al.: 
 *  "No Vanya's lines are allowed in LHCb/Gaudi software."
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-02-06 
 */
// ============================================================================

// ============================================================================
/** @fn mass 
 *  trivial function to evaluate the mass LHCb::Particle
 *  @param  p particle 
 *  @return invariant mass 
 *  @see LHCb::Particle 
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-01-17
 */
// ============================================================================
double LoKi::Kinematics::mass 
( const LHCb::Particle* p ) 
{
  //
  if ( 0 == p ) 
  {
    LoKi::Report::Error
      ("mass(LHCb::Particle*),invaild argument 1,return 'InvalidMass'") ;
    return LoKi::Constants::InvalidMass ;
  }
  //
  return mass ( p->momentum() ) ;
} ;
// ============================================================================
/** @fn mass 
 *  trivial function to evaluate the mass LHCb::Particle
 *  @param  p1 the first particle 
 *  @param  p2 the second particle 
 *  @return invariant mass 
 *  @see LHCb::Particle 
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-01-17
 */
// ============================================================================
double LoKi::Kinematics::mass 
( const LHCb::Particle* p1 , 
  const LHCb::Particle* p2 ) 
{
  //
  if ( 0 == p1 ) 
  {
    LoKi::Report::Error
      ("mass(2*LHCb::Particle*),invaild argument 1,return 'InvalidMass'") ;
    return LoKi::Constants::InvalidMass ;
  }
  //
  if ( 0 == p2 ) 
  {
    LoKi::Report::Error
      ("mass(2*LHCb::Particle*),invaild argument 2,return 'InvalidMass'") ;
    return LoKi::Constants::InvalidMass ;
  }
  //
  return mass ( p1->momentum() , 
                p2->momentum() ) ;
} ;
// ============================================================================
/** @fn mass 
 *  trivial function to evaluate the mass LHCb::Particle
 *  @param  p1 the first particle 
 *  @param  p2 the third particle 
 *  @param  p3 the second particle 
 *  @return invariant mass 
 *  @see LHCb::Particle 
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-01-17
 */
// ============================================================================
double LoKi::Kinematics::mass 
( const LHCb::Particle* p1 , 
  const LHCb::Particle* p2 ,
  const LHCb::Particle* p3 ) 
{
  //
  if ( 0 == p1 ) 
  {
    LoKi::Report::Error
      ("mass(3*LHCb::Particle*),invaild argument 1,return 'InvalidMass'") ;
    return LoKi::Constants::InvalidMass ;
  }
  //  
  if ( 0 == p2 ) 
  {
    LoKi::Report::Error
      ("mass(3*LHCb::Particle*),invaild argument 2,return 'InvalidMass'") ;
    return LoKi::Constants::InvalidMass ;
  }
  //
  if ( 0 == p3 ) 
  {
    LoKi::Report::Error
      ("mass(3*LHCb::Particle*),invaild argument 3,return 'InvalidMass'") ;
    return LoKi::Constants::InvalidMass ;
  }
  //
  return mass ( p1->momentum() , 
                p2->momentum() ,
                p3->momentum() ) ;
} ;
// ============================================================================
/** @fn mass 
 *  trivial function to evaluate the mass LHCb::Particle
 *  @param  p1 the first particle 
 *  @param  p2 the second particle 
 *  @param  p3 the third  particle 
 *  @param  p3 the fourth  particle 
 *  @return invariant mass 
 *  @see LHCb::Particle 
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-01-17
 */
// ============================================================================
double LoKi::Kinematics::mass
( const LHCb::Particle* p1 , 
  const LHCb::Particle* p2 ,
  const LHCb::Particle* p3 ,
  const LHCb::Particle* p4 ) 
{
  //
  if ( 0 == p1 ) 
  {
    LoKi::Report::Error
      ("mass(4*LHCb::Particle*),invaild argument 1,return 'InvalidMass'") ;
    return LoKi::Constants::InvalidMass ;
  }
  //
  if ( 0 == p2 ) 
  {
    LoKi::Report::Error
      ("mass(4*LHCb::Particle*),invaild argument 2,return 'InvalidMass'") ;
    return LoKi::Constants::InvalidMass ;
  }
  //
  if ( 0 == p3 ) 
  {
    LoKi::Report::Error
      ("mass(4*LHCb::Particle*),invaild argument 3,return 'InvalidMass'") ;
    return LoKi::Constants::InvalidMass ;
  }
  //
  if ( 0 == p4 ) 
  {
    LoKi::Report::Error
      ("mass(4*LHCb::Particle*),invaild argument 4,return 'InvalidMass'") ;
    return LoKi::Constants::InvalidMass ;
  }  
  //
  return mass ( p1->momentum() , 
                p2->momentum() ,
                p3->momentum() , 
                p4->momentum() ) ;
} ;
// ============================================================================

// ============================================================================
// The END 
// ============================================================================

