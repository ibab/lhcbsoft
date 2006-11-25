// $Id: Math.h,v 1.5 2006-11-25 19:12:55 ibelyaev Exp $
// ============================================================================
// CVS tag $Name: not supported by cvs2svn $ ; version $Revision: 1.5 $ 
// ============================================================================
// $Log: not supported by cvs2svn $
// Revision 1.4  2006/05/02 14:29:09  ibelyaev
//  censored
//
// ============================================================================
#ifndef LOKI_MATH_H 
#define LOKI_MATH_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL 
// ============================================================================
#include <cmath>
// ============================================================================
// LokiCore
// ============================================================================
#include "LoKi/Functions.h"
#include "LoKi/Power.h"
// ============================================================================

// ============================================================================
/** @file
 *
 *  Collection of math-function s for LoKi functions
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
 *  @date 2001-01-23 
 */
// ============================================================================


namespace LoKi 
{
  
  /*  @fn exp 
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  exp ( const LoKi::Function<TYPE>& fun ) 
  { return LoKi::ComposeFunction<TYPE> ( std::exp , fun , "exp" ); }

  /*  @fn sin 
   *  sine for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  sin ( const LoKi::Function<TYPE>& fun ) 
  { return LoKi::ComposeFunction<TYPE> ( std::sin , fun , "sin" ) ; }

  /*  @fn sinh
   *  hypoerbolic sine for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  sinh ( const LoKi::Function<TYPE>& fun ) 
  { return LoKi::ComposeFunction<TYPE> ( std::sinh, fun , "sinh" ) ; }

  /*  @fn cos
   *  cosine for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  cos ( const LoKi::Function<TYPE>& fun ) 
  { return LoKi::ComposeFunction<TYPE>( std::cos , fun , "cos" ) ; }
  
  /*  @fn cosh
   *  hyperbolic cosine for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  cosh ( const LoKi::Function<TYPE>& fun ) 
  { return LoKi::ComposeFunction<TYPE>( std::cosh , fun , "cosh" ) ; }
  
  /*  @fn tan
   *  tan for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  tan ( const LoKi::Function<TYPE>& fun ) 
  { return LoKi::ComposeFunction<TYPE>( std::tan , fun , "tan" ) ; }

  /*  @fn tanh
   *  hyperbolic tangent for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  tanh ( const LoKi::Function<TYPE>& fun ) 
  { return LoKi::ComposeFunction<TYPE>( std::tanh , fun , "tanh" ) ; }
  
  /*  @fn sqrt
   *  sqrt for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  sqrt ( const LoKi::Function<TYPE>& fun ) 
  { return LoKi::ComposeFunction<TYPE>( std::sqrt , fun , "sqrt" ) ; }

  /*  @fn asin 
   *  asine for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  asin ( const LoKi::Function<TYPE>& fun ) 
  { return LoKi::ComposeFunction<TYPE>( std::asin , fun , "asin" ) ; }
  
  /*  @fn acos
   *  acos for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  // inline LoKi::ComposeFunction<TYPE> 
  acos ( const LoKi::Function<TYPE>& fun ) 
  { return LoKi::ComposeFunction<TYPE>( std::acos , fun , "acos" ) ; }
  
  /*  @fn atan
   *  atan for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  atan ( const LoKi::Function<TYPE>& fun ) 
  { return LoKi::ComposeFunction<TYPE>( std::atan , fun , "atan" ) ; }
  
  /*  @fn log
   *  log for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  log ( const LoKi::Function<TYPE>& fun ) 
  { return LoKi::ComposeFunction<TYPE>( std::log , fun , "log" ) ; }
  
  /*  @fn log10
   *  log10 for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  log10 ( const LoKi::Function<TYPE>& fun ) 
  { return LoKi::ComposeFunction<TYPE>( std::log10 , fun , "log10" ) ; }
  
  /*  @fn fabs
   *  abs for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  abs ( const LoKi::Function<TYPE>& fun ) 
  { return LoKi::ComposeFunction<TYPE>( std::fabs , fun , "abs" ) ; }
  
  /*  @fn abs
   *  abs for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  fabs ( const LoKi::Function<TYPE>& fun ) 
  { return LoKi::ComposeFunction<TYPE>( std::fabs , fun , "abs" ) ; }
  
  /*  @fn atan2
   *  atan2 for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  atan2 ( const LoKi::Function<TYPE>& fun1 , 
          const LoKi::Function<TYPE>& fun2 ) 
  { return LoKi::ComposeFunction2<TYPE>( std::atan2 , fun1 , fun2 , "atan2" ); }
  
  /*  @fn atan2
   *  atan2 for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  atan2 ( const LoKi::Function<TYPE>& fun1 , 
          const double                val2 ) 
  { return LoKi::ComposeFunction2<TYPE>( std::atan2 , fun1 , val2 , "atan2" ) ; }
  
  /*  @fn atan2
   *  atan2 for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  atan2 ( const double                val1 , 
          const LoKi::Function<TYPE>& fun2 ) 
  { return LoKi::ComposeFunction2<TYPE>( std::atan2 , val1 , fun2  , "atan2" ) ; }
  
  /*  @fn pow
   *  pow for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  pow ( const LoKi::Function<TYPE>& fun1 , 
        const LoKi::Function<TYPE>& fun2 ) 
  { return LoKi::ComposeFunction2<TYPE>( std::pow , fun1 , fun2  , "pow" ) ; }
  
  /*  @fn pow
   *  pow for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  pow ( const LoKi::Function<TYPE>& fun1 , 
        const double                val2 ) 
  { return LoKi::ComposeFunction2<TYPE>( std::pow , fun1 , val2 , "pow" ) ; }
  
  /*  @fn pow
   *  pow for LoKi function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */      
  template <class TYPE> 
  inline LoKi::FunctionFromFunction<TYPE>
  pow ( const double                val1 , 
        const LoKi::Function<TYPE>& fun2 ) 
  { return LoKi::ComposeFunction2<TYPE>( std::pow , val1 , fun2 , "pow" ) ; }
  
  /*  @fn min 
   *  minimum for LoKi functions 
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2004-08-10 
   */
  template <class TYPE>
  inline LoKi::FunctionFromFunction<TYPE>
  min ( const LoKi::Function<TYPE>& fun1 , 
        const LoKi::Function<TYPE>& fun2 ) 
  { return LoKi::Min<TYPE>( fun1 , fun2 ) ; }

  /*  @fn min 
   *  minimum for LoKi functions 
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2004-08-10 
   */
  template <class TYPE>
  inline LoKi::FunctionFromFunction<TYPE>
  min ( const LoKi::Function<TYPE>& fun1 , 
        const LoKi::Function<TYPE>& fun2 , 
        const LoKi::Function<TYPE>& fun3 ) 
  { return LoKi::Min<TYPE>( fun1 , fun2 , fun3 ) ; }

  /*  @fn min 
   *  minimum for LoKi functions 
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2004-08-10 
   */
  template <class TYPE>
  inline LoKi::FunctionFromFunction<TYPE>
  min ( const LoKi::Function<TYPE>& fun1 , 
        const LoKi::Function<TYPE>& fun2 , 
        const LoKi::Function<TYPE>& fun3 , 
        const LoKi::Function<TYPE>& fun4 ) 
  { return LoKi::Min<TYPE>( fun1 , fun2 , fun3 , fun4 ) ; }

  /*  @fn min 
   *  minimum for LoKi functions 
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2004-08-10 
   */
  template <class TYPE>
  inline LoKi::FunctionFromFunction<TYPE>
  min ( const LoKi::Function<TYPE>& fun1 , 
        const double                fun2 ) 
  { return min ( fun1 , LoKi::Constant<TYPE>(fun2) ) ; }

  /*  @fn min 
   *  minimum for LoKi functions 
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2004-08-10 
   */
  template <class TYPE>
  inline LoKi::FunctionFromFunction<TYPE>
  min ( const double                fun1 , 
        const LoKi::Function<TYPE>& fun2 ) 
  { return min ( LoKi::Constant<TYPE>(fun1) , fun2 ) ; }

  /*  @fn max 
   *  maximum for LoKi functions 
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2004-08-10 
   */
  template <class TYPE>
  inline LoKi::FunctionFromFunction<TYPE>
  max ( const LoKi::Function<TYPE>& fun1 , 
        const LoKi::Function<TYPE>& fun2 ) 
  { return LoKi::Max<TYPE>( fun1 , fun2 ) ; }
  
  /*  @fn max 
   *  maximum for LoKi functions 
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2004-08-10 
   */
  template <class TYPE>
  inline LoKi::FunctionFromFunction<TYPE>
  max ( const LoKi::Function<TYPE>& fun1 , 
        const LoKi::Function<TYPE>& fun2 , 
        const LoKi::Function<TYPE>& fun3 ) 
  { return LoKi::Max<TYPE>( fun1 , fun2 , fun3 ) ; }

  /*  @fn max 
   *  maximum for LoKi functions 
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2004-08-10 
   */
  template <class TYPE>
  inline LoKi::FunctionFromFunction<TYPE>
  max ( const LoKi::Function<TYPE>& fun1 , 
        const LoKi::Function<TYPE>& fun2 , 
        const LoKi::Function<TYPE>& fun3 , 
        const LoKi::Function<TYPE>& fun4 ) 
  { return LoKi::Max<TYPE>( fun1 , fun2 , fun3 , fun4 ) ; }
      
  /*  @fn max 
   *  maximum for LoKi functions 
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2004-08-10 
   */
  template <class TYPE>
  inline LoKi::FunctionFromFunction<TYPE>
  max ( const LoKi::Function<TYPE>& fun1 , 
        const double                fun2 ) 
  { return max ( fun1 , LoKi::Constant<TYPE>(fun2) ) ; }

  /*  @fn max
   *  maximum for LoKi functions 
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2004-08-10 
   */
  template <class TYPE>
  inline LoKi::FunctionFromFunction<TYPE>
  max ( const double                fun1 , 
        const LoKi::Function<TYPE>& fun2 ) 
  { return max ( LoKi::Constant<TYPE>(fun1) , fun2 ) ; }

  /** helpful function for creation of comparison object 
   *  @param fun1 function to be applied to the first object to be compared 
   *  @param fun2 function to be applied to the second object to be compared 
   *  @param cmp comparison criteria for result of the function evaluation
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <class TYPE,class CMP>
  inline LoKi::Compare<TYPE,CMP> compare
  ( const LoKi::Function<TYPE>& fun1  , 
    const LoKi::Function<TYPE>& fun2  ,
    const CMP&                  cmp   )
  { return Compare<TYPE,CMP>( fun1 , fun2 , cmp ); }
  
  /** select element form the sequence with minimal value of 
   *  given function.
   *  It is just an extension of the STL <tt>std::min_element</tt> 
   *  standard algorithm 
   *  @code 
   *  SEQUENCE prts = ... ; 
   *  SEQUENCE::iterator particle = 
   *              select_min( prts.begin() , prts.end() , PT ); 
   *  @endcode 
   *  Where <tt>SEQUENCE</tt> could be any container or container-like 
   *  object (like e.g. <tt>LoKi::Range</tt>) of <tt>const Particle*</tt>.
   *  This example illustrates the selection of particle with minimal 
   *  transverse momentum  from "container" <tt>prts</tt>.
   *  @see PT 
   *  @see LoKi::Function
   *  @param first  'begin' iterator of the sequence 
   *  @param last   'end'   iterator of the sequence    
   *  @param fun    function 
   *  @return iterator for the element with minimal value of function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-09-05
   */      
  template<class IT,class TYPE>
  inline IT select_min 
  ( IT first                        , 
    IT last                         , 
    const LoKi::Function<TYPE>& fun )
  {
    return std::min_element 
      ( first , 
        last  , 
        LoKi::compare ( fun                 , 
                        fun                 , 
                        std::less<double>() ) ) ;  
  };
  
  /** select element form the sequence with maximal value of function
   *  It is just an extension of the STL <tt>std::min_element</tt> 
   *  standard algorithm 
   *  @code 
   *  SEQUENCE prts = ... ; 
   *  SEQUENCE::iterator particle = 
   *              select_max( prts.begin() , prts.end() , CL ); 
   *  @endcode 
   *  Where <tt>SEQUENCE</tt> could be any container or container-like 
   *  object (like e.g. <tt>LoKi::Range</tt>) of <tt>const Particle*</tt>.
   *  This example illustrates the selection of particle with maximal 
   *  convidence level from "container" <tt>prts</tt>.
   *  @see CL
   *  @see LoKi::Function
   *  @param first  'begin' iterator of the sequence 
   *  @param last   'end'   iterator of the sequence    
   *  @param fun    function 
   *  @return iterator for the element with maximal value of function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-09-05
   */      
  template<class IT,class TYPE>
  inline IT select_max
  ( IT first                        , 
    IT last                         , 
    const LoKi::Function<TYPE>& fun )
  {
    return std::max_element 
      ( first , 
        last  , 
        LoKi::compare ( fun                 , 
                        fun                 , 
                        std::less<double>() ) ) ;  
  };
  
  /** select element form the sequence with minimal value of function
   *
   *  It is just an extension of the STL <tt>std::min_element</tt> 
   *  standard algorithm 
   *  @code 
   *  SEQUENCE prts = ... ; 
   *  const Particle* particle = select_min( prts , PT ); 
   *  @endcode 
   *  Where <tt>SEQUENCE</tt> could be any container or container-like 
   *  object (like e.g. <tt>LoKi::Range</tt>) of <tt>const Particle*</tt>.
   *  This example illustrates the selection of particle with minimal 
   *  transverse momentum  from "container" <tt>prts</tt>.
   *  @see PT 
   *  @see LoKi::Function
   *  @param sequence sequence/container/range 
   *  @param fun      function 
   *  @return element with minimal value of function  
   *  @attention be careful with empty ranges/containers/sequences 
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-09-05
   */      
  template<class SEQUENCE , class TYPE>
  inline typename SEQUENCE::value_type select_min
  ( SEQUENCE& sequence  , const LoKi::Function<TYPE>& fun )
  {
    typedef typename SEQUENCE::value_type Type ;
    if ( sequence.begin() == sequence.end() ) { return Type (); }
    return *select_min( sequence.begin() , sequence.end() , fun );
  };
  
  /** select element form the sequence with maximal value of function
   *
   *  It is just an extension of the STL <tt>std::min_element</tt> 
   *  standard algorithm 
   *  @code 
   *  SEQUENCE prts = ... ; 
   *  const Particle* particle = select_max( prts , CL ); 
   *  @endcode 
   *  Where <tt>SEQUENCE</tt> could be any container or container-like 
   *  object (like e.g. <tt>LoKi::Range</tt>) of <tt>const Particle*</tt>.
   *  This example illustrates the selection of particle with maximal 
   *  confidence level from "container" <tt>prts</tt>.
   *  @see CL
   *  @see LoKi::Function
   *  @param sequence sequence/container/range 
   *  @param fun      function 
   *  @return element with maximal value of function  
   *  @attention be careful with empty ranges/containers/sequences 
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-09-05
   */      
  template<class SEQUENCE , class TYPE>
  inline typename SEQUENCE::value_type select_max
  ( SEQUENCE& sequence  , const LoKi::Function<TYPE>& fun )
  {
    typedef typename SEQUENCE::value_type Type ;
    if ( sequence.begin() == sequence.end() ) { return Type () ; }
    return *select_max( sequence.begin() , sequence.end() , fun );
  };
  
  /** select element form the sequence with minimal value of 
   *  given function 
   *  It is just an extension of the STL <tt>std::min_element</tt> 
   *  standard algorithm with additional conditions 
   *  @code 
   *  SEQUENCE prts = ... ; 
   *  SEQUENCE::iterator particle = 
   *              select_min( prts.begin() , 
   *                          prts.end()   , 
   *                          PT           , 
   *                          P > 10 * GeV ); 
   *  @endcode 
   *  Where <tt>SEQUENCE</tt> could be any container or container-like 
   *  object (like e.g. <tt>LoKi::Range</tt>) of <tt>const Particle*</tt>.
   *  This example illustrates the selection of particle with minimal 
   *  transverse momentum  from "container" <tt>prts</tt>. Only 
   *  particles with momentum in excess of 10 GeV are considered.
   *
   *  @see PT 
   *  @see LoKi::Function
   *  @param first  'begin' iterator of the sequence 
   *  @param last   'end'   iterator of the sequence    
   *  @param fun    function 
   *  @param cut    predicate which need to be satisfied 
   *  @return iterator for the element with minimal value of function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2005-03-09
   */      
  template<class IT,class TYPE>
  inline IT select_min
  ( IT first                         , 
    IT last                          , 
    const LoKi::Function<TYPE>&  fun , 
    const LoKi::Predicate<TYPE>& cut )
  {
    // empty sequence 
    if ( first == last ) { return last ; }
    
    typedef std::less<double>        LESS ;
    typedef LoKi::Compare<TYPE,LESS> CMP  ;
    const CMP cmp = CMP ( fun , fun , LESS() )  ;
    
    IT result = last ;
    for (  ; first != last ; ++first ) 
    {
      if ( !cut( *first )           ) {                  continue ; }
      if ( last == result           ) { result = first ; continue ; }
      if ( cmp ( *first , *result ) ) { result = first ;            }  
    }
    
    return result ;
  };
  
  /** select element form the sequence with maximal value of 
   *  given function 
   *
   *  It is just an extension of the STL <tt>std::max_element</tt> 
   *  standard algorithm with additional conditions 
   *
   *  @code 
   *  SEQUENCE prts = ... ; 
   *  SEQUENCE::iterator particle = 
   *              select_max( prts.begin() , 
   *                          prts.end()   , 
   *                          PT           , 
   *                          P > 10 * GeV ); 
   *  @endcode 
   *
   *  Where <tt>SEQUENCE</tt> could be any container or container-like 
   *  object (like e.g. <tt>LoKi::Range</tt>) of <tt>const Particle*</tt>.
   *  This example illustrates the selection of particle with maximal
   *  transverse momentum  from "container" <tt>prts</tt>. Only 
   *  particles with momentum in excess of 10 GeV are considered.
   *
   *  @see PT 
   *  @see LoKi::Function
   *  @param first  'begin' iterator of the sequence 
   *  @param last   'end'   iterator of the sequence    
   *  @param fun    function 
   *  @param cut    predicate which need to be satisfied 
   *  @return iterator for the element with minimal value of function
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2005-03-09
   */      
  template<class IT,class TYPE>
  inline IT select_max
  ( IT first                         , 
    IT last                          , 
    const LoKi::Function<TYPE>&  fun , 
    const LoKi::Predicate<TYPE>& cut )
  {
    // empty sequence 
    if ( first == last ) { return last ; }
    
    typedef std::less<double>        LESS ;
    typedef LoKi::Compare<TYPE,LESS> CMP  ;
    const CMP cmp = CMP ( fun , fun , LESS() )  ;
    
    IT result = last ;
    for (  ; first != last ; ++first ) 
    {
      if ( !cut( *first )           ) {                  continue ; }
      if ( last == result           ) { result = first ; continue ; }
      if ( cmp ( *result , *first ) ) { result = first ;            }  
    }
    
    return result ;
  };

} // end of namespace LoKi

// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_MATH_H
// ============================================================================
