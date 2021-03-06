// $Id$
// ============================================================================
#ifndef LOKI_OBJECTS_H 
#define LOKI_OBJECTS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL 
// ============================================================================
#include <utility>
// ============================================================================
// LoKi 
// ============================================================================
#include "LoKi/valid.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/call_traits.hpp"
// ============================================================================
/** @file
 *
 *  This file is a part of LoKi project - 
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
 *  contributions and advices from G.Raven, J.van Tilburg, 
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-02-06 
 */
// ============================================================================
namespace LoKi 
{
  // ==========================================================================
  /** @namespace LoKi::Objects Objects.h LoKi/Objects.h
   *
   *  Collection of useful small objects for LoKi 
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2006-02-08
   */
  namespace Objects
  { 
    // ========================================================================
    /** @struct _ALL
     *  trivial class/functor/predicate which always evaluated to "true"
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-02-08
     */
    template <class TYPE>
    struct _ALL 
    {
      // ======================================================================
      /// type for the argument 
      typedef typename boost::call_traits<TYPE>::param_type ARG ;  
      inline bool operator() ( ARG /* arg */ ) const { return true   ; }
      // ======================================================================
    } ;
    // ========================================================================
    /** @struct _NONE
     *  trivial class/functor/predicate which always evaluated to "false"
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-02-08
     */
    template <class TYPE>
    struct _NONE 
    {
      // ======================================================================
      /// type for the argument 
      typedef typename boost::call_traits<TYPE>::param_type ARG ;  
      inline bool operator() ( ARG /* arg */ ) const { return false  ; }
      // ======================================================================
    } ;
    // ========================================================================
    /** @struct _ONE
     *  trivial class/functor/predicate which always evaluated to "1"
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-02-08
     */
    template <class TYPE>
    struct _ONE 
    {
      // ======================================================================
      /// type for the argument 
      typedef typename boost::call_traits<TYPE>::param_type ARG ;  
      inline int operator() ( ARG /* arg */ ) const { return 1 ; }
      // ======================================================================
    };  
    // ========================================================================
    /** @struct _ZERO
     *  trivial class/functor/predicate which always evaluated to "0"
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-02-08
     */
    template <class TYPE>
    struct _ZERO
    {
      // ======================================================================
      /// type for the argument 
      typedef typename boost::call_traits<TYPE>::param_type ARG ;  
      inline int operator() ( ARG /* arg */ ) const { return 0 ; }
      // ======================================================================
    };  
    // ========================================================================
    /** @struct _VALID
     *  trivial class/functor/predicate which checks the validity of object
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-02-08
     */
    template <class TYPE> 
    struct _VALID 
    {
      // ======================================================================
      /// type for the argument 
      typedef typename boost::call_traits<TYPE>::param_type ARG ;  
      inline bool  operator() ( ARG arg ) const { return LoKi::valid ( arg ) ; }
      // ======================================================================
    } ;
    // ========================================================================
    /** @var _ALL_
     *  helper objects which allows the efficient implementation 
     *  of selection algorithms 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    const _ALL<const void*> _ALL_     = _ALL<const void*>  () ;
    // ========================================================================
    /** @var _NONE_
     *  helper objects which allows the efficient implementation 
     *  of selection algorithms 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    const _NONE<const void*> _NONE_   = _NONE<const void*>  () ;
    // ========================================================================
    /** @var _ONE_
     *  helper objects which allows the efficient implementation 
     *  of selection algorithms 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    // ========================================================================
    const _ONE<const void*> _ONE_     = _ONE<const void*>  () ;
    // ========================================================================
    /** @var _ZERO_
     *  helper objects which allows the efficient implementation 
     *  of selection algorithms 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    // ========================================================================
    const _ZERO<const void*> _ZERO_   = _ZERO<const void*>  () ;
    // ========================================================================
    /** @var _VALID_
     *  helper objects which allows the efficient implementation 
     *  of selection algorithms 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    const _VALID<const void*> _VALID_ = _VALID<const void*> () ;
    // ========================================================================
  } //                                       end of the namespace LoKi::Objects
  // ==========================================================================
} //                                                  end of the namespace LoKi
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // LOKI_OBJECTS_H
// ============================================================================
