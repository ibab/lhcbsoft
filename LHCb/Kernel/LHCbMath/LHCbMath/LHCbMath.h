// $Id: LHCbMath.h,v 1.10 2010-01-03 08:41:17 ibelyaev Exp $ 
// ============================================================================
/** @file
 *
 *  Collection of math related functions for general use in LHCb
 *
 *  CVS Log :-
 *  $Id: LHCbMath.h,v 1.10 2010-01-03 08:41:17 ibelyaev Exp $
 *
 *  @author Juan PALACIOS
 *  @date   2005-11-21
 */
// ============================================================================
#ifndef LHCBMATH_LHCBMATH_H 
#define LHCBMATH_LHCBMATH_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL 
// ============================================================================
#include <cmath>
#include <algorithm>
#include <functional>
#include <vector>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Lomont.h"
// ============================================================================
// Boost 
// ============================================================================
#include "boost/call_traits.hpp"
#include "boost/integer_traits.hpp"
#include "boost/static_assert.hpp"
#include "boost/numeric/conversion/converter.hpp"
// ============================================================================
namespace LHCb 
{
  // ==========================================================================
  /** @namespace LHCb::Math
   *  Collection of math related functions for general use in LHCb
   */
  namespace Math 
  {
    /// Parameters for numerical calculations (M.Needham)
    static const double hiTolerance    = 1e-40;
    static const double lowTolerance   = 1e-20;
    static const double looseTolerance = 1e-5;
    static const double     sqrt_12 = 3.4641016151377546; // sqrt(12.)
    static const double inv_sqrt_12 = 0.2886751345948129; // 1./sqrt(12.)
    // ========================================================================
    /** @var mULPS_float
     *  "tolerance" parameter for "Lomont"-compare of floating point numbers.
     *  It corresponds to relative ("Knuth/GLS") tolerance of about ~6*10^-6
     *  for values in excess of 10^-37.
     *
     *  @see Gaudi::Math::lomont_compare_float 
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-01-02
     */
    const unsigned short mULPS_float = 100 ;
    // ========================================================================
    /** @var mULPS_float_low
     *  "Low-tolerance" parameter for "Lomont"-compare of floating point numbers.
     *  It corresponds to relative ("Knuth/GLS") tolerance of about ~6*10^-5
     *  for values in excess of 10^-37.
     *
     *  @see Gaudi::Math::lomont_compare_float 
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-01-02
     */
    const unsigned short mULPS_float_low = 1000 ;
    // =========================================================================
    /** @var mULPS_double
     *  "tolerance" parameter for "Lomont"-compare of floating point numbers.
     *  It corresponds to relative ("Knuth/GLS") tolerance of about ~6*10^-13
     *  for values in excess of 10^-304.
     *  @see Gaudi::Math::lomont_compare_double
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-01-02
     */
    const unsigned int mULPS_double = 1000 ;
    // ========================================================================
    /** @struct abs_less 
     *  comparison by absolute value 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-08-17
     */
    template <class TYPE>
    struct abs_less : std::binary_function<TYPE,TYPE,TYPE>
    {
      inline TYPE operator() 
        ( typename boost::call_traits<const TYPE>::param_type v1 ,
          typename boost::call_traits<const TYPE>::param_type v2 ) const 
      {
        return m_eval ( std::fabs( v1 ) , std::fabs( v2 ) ) ; 
      }
      /// evaluator: 
      std::less<TYPE> m_eval ;
    } ;
    // ========================================================================
    /** @struct abs_greater
     *  comparison by absolute value 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-08-17
     */
    template <class TYPE>
    struct abs_greater : std::binary_function<TYPE,TYPE,TYPE>
    {
      inline TYPE operator() 
      ( typename boost::call_traits<const TYPE>::param_type v1 ,
        typename boost::call_traits<const TYPE>::param_type v2 ) const 
      {
        return m_eval ( std::fabs( v1 ) , std::fabs( v2 ) ) ; 
      }
      /// evaluator: 
      std::greater<TYPE> m_eval ;
    } ;
    // ========================================================================
    /** return "min_by_abs"
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-08-17
     */        
    template <class TYPE> 
    inline TYPE absMin ( TYPE v1 , TYPE v2 ) 
    { return std::min ( std::fabs ( v1 ) , std::fabs ( v2 ) ) ; }
    // ========================================================================
    /** return  "max_by_abs"
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-08-17
     */
    template <class TYPE> 
    inline TYPE absMax ( TYPE v1 , TYPE v2 ) 
    { return std::max ( std::fabs ( v1 ) , std::fabs ( v2 ) ) ; }
    // ========================================================================
    /** compare two double numbers with relative precision 'epsilon'
     *
     *  Essentially it is a wrapper to gsl_fcmp function from GSL library
     *  See D.E.Knuth, "Seminumerical Algorithms", section 4.2.2
     *
     *  @param value1  (INPUT) the first value 
     *  @param value2  (INPUT) the second value 
     *  @param epsilon (INPUT) the (relative) precision 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-27
     */
    GAUDI_API
    bool knuth_equal_to_double
    ( const double value1           ,
      const double value2           ,
      const double epsilon = 1.0e-6 ) ;
    // ========================================================================
    /** compare two double numbers with precision 'mULPS'
     *  @param value1 (INPUT) the first value 
     *  @param value2 (INPUT) the second value 
     *  @param mULPS  (INPUT) the precision 
     *  @see Gaudi::Math::lomont_compare_double 
     *  @see LHCb::Math::mULPS_double 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-27
     */
    inline bool equal_to_double
    ( const double value1                      ,
      const double value2                      ,
      const unsigned int mULPS = mULPS_double  ) 
    { return Gaudi::Math::lomont_compare_double ( value1 , value2 , mULPS ) ; }
    // ========================================================================
    /** @struct Equal_To
     *  helper structure for comparison of floating values
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-27
     */
    template <class TYPE>
    struct Equal_To : public std::binary_function<TYPE,TYPE,bool>
    {
      typedef typename boost::call_traits<const TYPE>::param_type T ;
      /// comparison
      inline bool operator() ( T v1 , T v2 ) const
      {
        std::equal_to<TYPE> cmp ;
        return cmp ( v1 , v2 ) ;
      }
    } ;
    // ========================================================================
    /// partial specialization for const-types
    template <class TYPE>
    struct Equal_To<const TYPE>: public Equal_To<TYPE>{} ;
    // ========================================================================
    /// partial specialization for references
    template <class TYPE>
    struct Equal_To<TYPE&>: public Equal_To<TYPE>{} ;
    // ========================================================================
    /** explicit specialization for doubles
     *  @see LHCb::Math::mULPS_double 
     */
    template <>
    struct Equal_To<double>
    {
    public:
      // ======================================================================
      /// constructor
      Equal_To ( const unsigned int eps = mULPS_double ) : m_cmp ( eps ) {}
      /// comparison:
      inline bool operator() ( const double v1 , const double v2 ) const
      { return m_cmp ( v1 , v2 ) ; }
      // ======================================================================
    private :
      // ======================================================================
      Equal_To ( const double /* eps */ ) ;
      // ======================================================================
    private :
      // ======================================================================
      /// evaluator 
      Gaudi::Math::Lomont<double> m_cmp ;                      // the evalautor 
      // ======================================================================
    };
    // ========================================================================
    /** explicit specialization for long doubles
     *  @see LHCb::Math::mULPS_double 
     */
    template <>
    struct Equal_To<long double>
    {
    public:
      // ======================================================================
      /// constructor
      Equal_To ( const unsigned int eps = mULPS_double ) : m_cmp ( eps ) {}
      /// comparison:
      inline bool operator() 
      ( const long double v1 ,
        const long double v2 ) const
      { 
        return  m_cmp ( static_cast<double> ( v1 ) , 
                        static_cast<double> ( v2 ) ) ; 
      }
      // ======================================================================
    private :
      // ======================================================================
      /// constructor
      Equal_To ( const long double /* eps */ ) ;
      // ======================================================================
    private :
      // ======================================================================
      /// the evaluator 
      Equal_To<double> m_cmp ;                                 // the evaluator 
      // ======================================================================
    };
    // ========================================================================
    /** explicit specialization for floats
     *  @see LHCb::Math::mULPS_float
     *  @see Gaudi::Math::Lomont
     *  @see Gaudi::Math::Lomont<float>
     */
    template <>
    struct Equal_To<float>
    {
    public:
      // ======================================================================
      /** constructor
       *  @see LHCb::Math::mULPS_float
       */
      Equal_To ( const unsigned short eps =  mULPS_float ) : m_cmp ( eps ) {}
      /// comparison:
      inline bool operator() ( const float v1 , const float v2 ) const
      { return m_cmp( v1 , v2 ) ; }
      // ======================================================================
    private:
      // ======================================================================      
      /// constructor
      Equal_To ( const float /* eps */ ) ;
      // ======================================================================
    private :
      // ======================================================================
      /// the evaluator 
      Gaudi::Math::Lomont<float> m_cmp ;                       // the evaluator
      // ======================================================================
    } ;
    // ========================================================================
    /** specialisation for vectors 
     *  @see LHCb::Math::mULPS_float
     *  @see Gaudi::Math::Lomont
     *  @see Gaudi::Math::Lomont<float>
     */
    template <>
    struct Equal_To<std::vector<float> > 
    {
    public:
      // ======================================================================
      /** constructor
       *  @see LHCb::Math::mULPS_float
       */
      Equal_To ( const unsigned short eps  = mULPS_float ) : m_cmp ( eps ) {}
      /// comparison:
      inline bool operator() ( const std::vector<float>& v1 , 
                               const std::vector<float>& v2 ) const
      {
        return v1.size() == v2.size() && 
          std::equal ( v1.begin () , v1.end () , v2.begin () , m_cmp ) ;
      }      
      // ======================================================================
    private:
      // ======================================================================
      Equal_To<float> m_cmp ;
      // ======================================================================
    } ;
    // ========================================================================
    /** specialisation for vectors 
     *  @see LHCb::Math::mULPS_double
     *  @see Gaudi::Math::Lomont
     *  @see Gaudi::Math::Lomont<double>
     */
    template <>
    struct Equal_To<std::vector<double> > 
    {
    public:
      // ======================================================================
      /** constructor
       *  @see LHCb::Math::mULPS_double
       */
      Equal_To ( const unsigned int eps  = mULPS_double ) : m_cmp ( eps ) {}
      // ======================================================================
      /// comparison:
      inline bool operator() ( const std::vector<double>& v1 , 
                               const std::vector<double>& v2 ) const
      {
        return v1.size() == v2.size() && 
          std::equal ( v1.begin () , v1.end () , v2.begin () , m_cmp ) ;
      }      
      /// comparison:
      inline bool operator() ( const std::vector<double>& v1 , 
                               const std::vector<float>&  v2 ) const
      {
        return v1.size() == v2.size() && 
          std::equal ( v1.begin () , v1.end () , v2.begin () , m_cmp ) ;
      }      
      /// comparison:
      inline bool operator() ( const std::vector<double>& v1 , 
                               const std::vector<int>&    v2 ) const
      {
        return v1.size() == v2.size() && 
          std::equal ( v1.begin () , v1.end () , v2.begin () , m_cmp ) ;
      }      
      /// comparison:
      inline bool operator() ( const std::vector<double>&       v1 , 
                               const std::vector<unsigned int>& v2 ) const
      {
        return v1.size() == v2.size() && 
          std::equal ( v1.begin () , v1.end () , v2.begin () , m_cmp ) ;
      }      
      /// comparison:
      inline bool operator() ( const std::vector<float>&  v1 , 
                               const std::vector<double>& v2 ) const
      {
        return v1.size() == v2.size() && 
          std::equal ( v1.begin () , v1.end () , v2.begin () , m_cmp ) ;
      }      
      /// comparison:
      inline bool operator() ( const std::vector<int>&    v1 , 
                               const std::vector<double>& v2 ) const
      {
        return v1.size() == v2.size() && 
          std::equal ( v1.begin () , v1.end () , v2.begin () , m_cmp ) ;
      }      
      /// comparison:
      inline bool operator() ( const std::vector<unsigned int>& v1 , 
                               const std::vector<double>&       v2 ) const
      {
        return v1.size() == v2.size() && 
          std::equal ( v1.begin () , v1.end () , v2.begin () , m_cmp ) ;
      }      
      // ======================================================================
    private:
      // ======================================================================
      /// the evaluator 
      Equal_To<double> m_cmp ;                                 // the evaluator 
      // ======================================================================
    } ;  
    // ========================================================================
    /** round to nearest integer, rounds half integers to nearest even integer 
     *  It is just a simple wrapper around boost::numeric::converter 
     *  @author Vanya BELYAEV Ivan.BElyaev
     */
    inline long round ( const double x ) 
    {
      typedef boost::numeric::RoundEven<double> Rounder ;
      typedef boost::numeric::make_converter_from 
        <double,
        boost::numeric::silent_overflow_handler,
        Rounder>::to<long>::type Converter ;
      return Converter::convert ( x ) ; 
    }
    // ========================================================================
    /** round to nearest integer, rounds half integers to nearest even integer 
     *  It is just a simple wrapper around boost::numeric::converter 
     *  @author Vanya BELYAEV Ivan.BElyaev
     */
    inline long round ( const float  x ) 
    {
      typedef boost::numeric::RoundEven<float> Rounder ;
      typedef boost::numeric::make_converter_from 
        <float,
        boost::numeric::silent_overflow_handler,
        Rounder>::to<long>::type Converter ;
      return Converter::convert ( x ) ; 
    }    
    // ========================================================================    
    /** check if the double value is actually equal to the integer value  
     *  @param val value to be compared with the integer 
     *  @param ref the reference integer number 
     *  @param mULPS the precision 
     *  @see Gaudi::Math::lomont_compare_double 
     *  @see LHCb::Math::mULPS_double
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-17
     */        
    inline bool equal_to_int 
    ( const double       val                  , 
      const int          ref                  , 
      const unsigned int mULPS = mULPS_double ) 
    {
      const double tmp = ref ;
      return Gaudi::Math::lomont_compare_double ( val , tmp , mULPS ) ;
    }
    // ========================================================================
    /** check if the double value is actually equal to the integer value  
     *  @param ref the reference integer  number 
     *  @param val value to be compared with the integer 
     *  @param mULPS the precision 
     *  @see Gaudi::Math::lomont_compare_double 
     *  @see LHCb::Math::mULPS_double
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-17
     */        
    inline bool equal_to_int 
    ( const int          ref                  , 
      const double       val                  , 
      const unsigned int mULPS = mULPS_double ) 
    { 
      return equal_to_int ( val , ref , mULPS ) ; 
    }
    // ========================================================================
    /** check if the double value is actually equal to the unsigned integer value  
     *  @param val value to be compared with the unsigned integer 
     *  @param ref the reference unsigned integer number 
     *  @param mULPS the precision 
     *  @see Gaudi::Math::lomont_compare_double 
     *  @see LHCb::Math::mULPS_double
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-17
     */        
    inline bool equal_to_uint 
    ( const double       val                  , 
      const unsigned int ref                  , 
      const unsigned int mULPS = mULPS_double ) 
    {
      const double tmp = ref ;
      return Gaudi::Math::lomont_compare_double ( val , tmp , mULPS ) ;
    }
    // ========================================================================
    /** check if the double value is actually equal to the integer value  
     *  @param val value to be compared with the unsigned integer 
     *  @param ref the reference unsigned integer number 
     *  @param mULPS the precision 
     *  @see Gaudi::Math::lomont_compare_double 
     *  @see LHCb::Math::mULPS_double
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-17
     */        
    inline bool equal_to_uint 
    ( const unsigned int ref                  , 
      const double       val                  ,
      const unsigned int mULPS = mULPS_double ) 
    {
      return equal_to_uint ( val , ref , mULPS ) ; 
    }
    // ========================================================================
  } //                                              end of namespace LHCb::Math 
  // ==========================================================================
} //                                                      end of namespace LHCb 
// ============================================================================
// The END 
// ============================================================================
#endif // LHCBMATH_LHCBMATH_H
// ============================================================================
