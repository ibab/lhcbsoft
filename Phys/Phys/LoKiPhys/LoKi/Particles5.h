// $Id$
// ============================================================================
#ifndef LOKI_PARTICLES5_H 
#define LOKI_PARTICLES5_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKiCore 
// ============================================================================
#include "LoKi/Constants.h"
// ============================================================================
// LoKiPhys 
// ============================================================================
#include "LoKi/PhysTypes.h"
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
 *  @date 2006-02-20 
 */
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  namespace Particles 
  {
    // ========================================================================
    /** @class InTree
     *  The trivial predicate which evaluates to true 
     *  if there is at least one particle in the decay 
     *  tree of the given particle which satisfies the 
     *  certain criteria
     *
     *  The function uses the algorithm LoKi::PhysAlgs::found 
     *
     *  @see LoKi::PhysAlgs::found 
     *  @see LoKi::Cuts::INTREE 
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    class GAUDI_API InTree : public LoKi::PhysTypes::Cuts 
    {
    public:
      // ======================================================================
      /** standard constructor 
       *  @param cut cut to be applied 
       */
      InTree  ( const LoKi::PhysTypes::Cuts& cut ) ;
      /// copy constructor 
      InTree  ( const InTree& right ) ;
      /// MANDATORY: virtual destructor 
      virtual ~InTree (){};
      /// MANDATORY: clone function ("virtual constructor")
      virtual  InTree*       clone() const { return new InTree(*this) ;}
      /// MANDATORY: the only one essential method 
      virtual  result_type   operator() ( argument p ) const ;
      /// OPTIONAL: the specific printout 
      virtual std::ostream& fillStream( std::ostream& s ) const ;
      // ======================================================================
    private:
      // ======================================================================
      // default constructor is disabled 
      InTree () ;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::PhysTypes::Cut m_cut ;
      // ======================================================================
    };
    // ========================================================================    
    /** @class NinTree
     *  The simple funtion which evaluates the number 
     *  of particle in decay tree which satisfies the certan criteria 
     * 
     *  The function uses the algorithm LoKi::PhysAlgs::count_if 
     *
     *  @see LoKi::PhysAlgs::count_if 
     *  @see LoKi::Cuts::NINTREE 
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    class GAUDI_API NinTree : public LoKi::PhysTypes::Func 
    {
    public:
      // ======================================================================
      /** standard constructor 
       *  @param cut cut to be applied 
       */
      NinTree  ( const LoKi::PhysTypes::Cuts& cut ) ;
      /// copy constructor 
      NinTree  ( const NinTree& right ) ;
      /// destructor 
      virtual ~NinTree (){} ;
      /// MANDATORY: clone function (virtual destructor)
      virtual  NinTree* clone() const { return new NinTree(*this) ; }
      /// MANDATORY: the only one essential method 
      virtual  result_type   operator() ( argument p ) const ;
      /// OPTIONAL: the specific printout 
      virtual std::ostream& fillStream( std::ostream& s ) const ;
      // ======================================================================
    private:
      // ======================================================================
      // the default constructor is disabled 
      NinTree () ;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::PhysTypes::Cut m_cut ;
      // ======================================================================
    };
    // ========================================================================    
    /** @class SumTree
     *
     *  The simple function which accumulated the 
     *  value of certain function over the decay 
     *  tree of the particle for the articles which 
     *  satisfy the certain selection criteria
     *  Accumulation is done throught summation
     *
     *  The function uses the algorithm LoKi::PhysAlgs::accumulate
     *
     *  @see LoKi::PhysAlgs::accumulate
     *  @see LoKi::Cuts::SUMTREE 
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    class GAUDI_API SumTree : public LoKi::PhysTypes::Func 
    {
    public:
      // ======================================================================
      /** standard constructor 
       *  @param fun the function to be evaluated 
       *  @param cut cut to be applied 
       *  @param res initial value 
       */
      SumTree
      ( const LoKi::PhysTypes::Func& fun       ,
        const LoKi::PhysTypes::Cuts& cut       ,
        const double                 res = 0.0 ) ;
      /** standard constructor 
       *  @param fun the function to be evaluated 
       *  @param cut cut to be applied 
       *  @param res initial value 
       */
      SumTree
      ( const LoKi::PhysTypes::Cuts& cut       ,
        const LoKi::PhysTypes::Func& fun       ,
        const double                 res = 0.0 ) ;
      /// copy constructor 
      SumTree ( const SumTree& right ) ;
      /// MANDATORY: virtual destructor 
      virtual ~SumTree (){} ;
      /// MANDATORY: clone function (virtual destructor)
      virtual  SumTree* clone() const { return new SumTree(*this) ; }
      /// MANDATORY: the only one essential method 
      virtual  result_type   operator() ( argument p ) const ;
      /// OPTIONAL: the specific printout 
      virtual std::ostream& fillStream( std::ostream& s ) const ;
      // ======================================================================
    private:
      // ======================================================================
      // the default constructor is disabled 
      SumTree() ;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::PhysTypes::Fun m_fun ;
      LoKi::PhysTypes::Cut m_cut ;
      double               m_res ;
      // ======================================================================
    };
    // ========================================================================
    /** @class MultTree
     *
     *  The simple function which accumulated the 
     *  value of certain function over the decay 
     *  tree of the particle for the articles which 
     *  satisfy the certain selection criteria
     *  Accumulation is done throught summation
     *
     *  The function uses the algorithm LoKi::PhysAlgs::multiplicate
     *
     *  @see LoKi::PhysAlgs::multuplicate
     *  @see LoKi::Cuts::MULTTREE 
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    class GAUDI_API MultTree : public LoKi::PhysTypes::Func 
    {
    public:
      // ======================================================================
      /** standard constructor 
       *  @param fun the function to be evaluated 
       *  @param cut cut to be applied 
       *  @param res initial value 
       */
      MultTree
      ( const LoKi::PhysTypes::Func& fun       ,
        const LoKi::PhysTypes::Cuts& cut       ,
        const double                 res = 1.0 ) ;
      /** standard constructor 
       *  @param fun the function to be evaluated 
       *  @param cut cut to be applied 
       *  @param res initial value 
       */
      MultTree
      ( const LoKi::PhysTypes::Cuts& cut       ,
        const LoKi::PhysTypes::Func& fun       ,
        const double                 res = 1.0 ) ;
      /// copy constructor 
      MultTree ( const MultTree& right ) ;
      /// MANDATORY: virtual destructor 
      virtual ~MultTree (){} ;
      /// MANDATORY: clone function (virtual destructor)
      virtual  MultTree* clone() const { return new MultTree(*this) ; }
      /// MANDATORY: the only one essential method 
      virtual  result_type   operator() ( argument p ) const ;
      /// OPTIONAL: the specific printout 
      virtual std::ostream& fillStream( std::ostream& s ) const ;
      // ======================================================================
    private:
      // ======================================================================
      // the default constructor is disabled 
      MultTree() ;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::PhysTypes::Fun m_fun ;
      LoKi::PhysTypes::Cut m_cut ;
      double               m_res ;
      // ======================================================================
    };
    // ========================================================================
    /** @class MinTree
     *
     *  The trivial algorithm which scans the decay 
     *  tree of the particle and searches for the the 
     *  minimal value for some functions for
     *  particles which satisfy the certain criteria 
     *
     *  The function uses the algorithm LoKi::PhysAlgs::min_value 
     *
     *  @see LoKi::PhysAlgs::min_value  
     *  @see LoKi::Cuts::MINTREE 
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    class GAUDI_API MinTree : public LoKi::PhysTypes::Func 
    {
    public:
      // ======================================================================
      /** standard constructor 
       *  @param fun the function to be evaluated 
       *  @param cut cut to be applied 
       *  @param res initial value 
       */
      MinTree
      ( const LoKi::PhysTypes::Func& fun  ,
        const LoKi::PhysTypes::Cuts& cut  ,
        const double                 res  = 
        LoKi::Constants::PositiveInfinity ) ;
      /** standard constructor 
       *  @param fun the function to be evaluated 
       *  @param cut cut to be applied 
       *  @param res initial value 
       */
      MinTree
      ( const LoKi::PhysTypes::Cuts& cut ,
        const LoKi::PhysTypes::Func& fun ,
        const double                 res = 
        LoKi::Constants::PositiveInfinity ) ;
      /// copy constructor 
      MinTree ( const MinTree& right ) ;
      /// MANDATORY: virtual destructor 
      virtual ~MinTree (){} ;
      /// MANDATORY: clone function (virtual destructor)
      virtual  MinTree* clone() const { return new MinTree(*this) ; }
      /// MANDATORY: the only one essential method 
      virtual  result_type   operator() ( argument p ) const ;
      /// OPTIONAL: the specific printout 
      virtual std::ostream& fillStream( std::ostream& s ) const ;
      // ======================================================================
    private:
      // ======================================================================
      // the default constructor is disabled 
      MinTree() ;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::PhysTypes::Fun m_fun ;
      LoKi::PhysTypes::Cut m_cut ;
      double               m_res ;
      // ======================================================================
    };
    // ========================================================================
    /** @class MinTree
     *
     *  The trivial algorithm which scans the decay 
     *  tree of the particle and searches for the the 
     *  minimal value for some functions for
     *  particles which satisfy the certain criteria 
     *
     *  The function uses the algorithm LoKi::PhysAlgs::max_value 
     *
     *  @see LoKi::PhysAlgs::max_value  
     *  @see LoKi::Cuts::MAXTREE 
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    class GAUDI_API MaxTree : public LoKi::PhysTypes::Func 
    {
    public:
      // ======================================================================
      /** standard constructor 
       *  @param fun the function to be evaluated 
       *  @param cut cut to be applied 
       *  @param res initial value 
       */
      MaxTree
      ( const LoKi::PhysTypes::Func& fun  ,
        const LoKi::PhysTypes::Cuts& cut  ,
        const double                 res  = 
        LoKi::Constants::NegativeInfinity ) ;
      /** standard constructor 
       *  @param fun the function to be evaluated 
       *  @param cut cut to be applied 
       *  @param res initial value 
       */
      MaxTree
      ( const LoKi::PhysTypes::Cuts& cut ,
        const LoKi::PhysTypes::Func& fun ,
        const double                 res = 
        LoKi::Constants::NegativeInfinity ) ;
      /// copy constructor 
      MaxTree ( const MaxTree& right ) ;
      /// MANDATORY: virtual destructor 
      virtual ~MaxTree (){} ;
      /// MANDATORY: clone function (virtual destructor)
      virtual  MaxTree* clone() const { return new MaxTree(*this) ; }
      /// MANDATORY: the only one essential method 
      virtual  result_type   operator() ( argument p ) const ;
      /// OPTIONAL: the specific printout 
      virtual std::ostream& fillStream( std::ostream& s ) const ;
      // ======================================================================
    private:
      // ======================================================================
      // the default constructor is disabled 
      MaxTree() ;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::PhysTypes::Fun m_fun ;
      LoKi::PhysTypes::Cut m_cut ;
      double               m_res ;
      // ======================================================================
    };
    // ========================================================================
    /** @class InGeneration 
     *
     *  Simple meta-functor which checks the presence of the certain particles
     *  at the given level of decay tree structure.
     *  
     *  The concept belongs to Patrick KOPPENBURG (Patrick.Koppenburg@cern.ch)
     *
     *  @see LoKi::Cuts::INGENERATION
     *  @see LoKi::Algs::find_if 
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-03-28
     */
    class GAUDI_API InGeneration : public LoKi::PhysTypes::Cuts 
    {
    public:
      // ======================================================================
      /// the constructor from the predicate and the generation level
      InGeneration 
      ( const LoKi::PhysTypes::Cuts& cut   , 
        const unsigned int           level ) ;      
      /// the constructor from the generation level and the prediate 
      InGeneration 
      ( const unsigned int           level ,
        const LoKi::PhysTypes::Cuts& cut   ) ;
      /// MANDATORY: virtual destructor 
      virtual ~InGeneration(){} ;
      /// MANDATORY: clone method ("virtual constructor") ;
      virtual  InGeneration* clone () const ;
      /// MANDATORY: the only one essential method 
      virtual result_type operator() ( argument p ) const ;
      /// OPTIONAL: the specific printout 
      virtual std::ostream& fillStream( std::ostream& s ) const ;
      // ======================================================================
    private:
      // ======================================================================
      /// the default constructor is disabled 
      InGeneration(); // the default constructor is disabled 
      // ======================================================================
    private:
      // ======================================================================
      /// the predicate to be checked 
      LoKi::PhysTypes::Cut  m_cut   ; // the predicate to be checked
      /// the generation level 
      unsigned int          m_level ; // the generation level 
      // ======================================================================
    };
    // ========================================================================
    /** @class NinGeneration 
     *
     *  Simple meta-functor which checks the presence of the certain particles
     *  at the given level of decay tree structure.
     *  
     *  The concept belongs to Patrick KOPPENBURG (Patrick.Koppenburg@cern.ch)
     *
     *  @see LoKi::Cuts::NINGENERATION
     *  @see LoKi::Algs::count_if 
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-03-28
     */
    class GAUDI_API NinGeneration : public LoKi::PhysTypes::Func 
    {
    public:
      // ======================================================================
      /// the constructor from the predicate and the generation level
      NinGeneration 
      ( const LoKi::PhysTypes::Cuts& cut   , 
        const unsigned int           level ) ;      
      /// the constructor from the generation level and the prediate 
      NinGeneration 
      ( const unsigned int           level ,
        const LoKi::PhysTypes::Cuts& cut   ) ;
      /// MANDATORY: virtual destructor 
      virtual ~NinGeneration(){} ;
      /// MANDATORY: clone method ("virtual constructor") ;
      virtual  NinGeneration* clone () const ;
      /// MANDATORY: the only one essential method 
      virtual result_type operator() ( argument p ) const ;
      /// OPTIONAL: the specific printout 
      virtual std::ostream& fillStream( std::ostream& s ) const ;
      // ======================================================================
    private:
      // ======================================================================
      /// the default constructor is disabled 
      NinGeneration(); // the default constructor is disabled 
      // ======================================================================
    private:
      // ======================================================================
      /// the predicate to be checked 
      LoKi::PhysTypes::Cut  m_cut   ; // the predicate to be checked
      /// the generation level 
      unsigned int          m_level ; // the generation level 
      // ======================================================================
    };
    // ========================================================================    
  } //                                         end of namespace LoKi::Particles
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // LOKI_PARTICLES4_H
// ============================================================================
