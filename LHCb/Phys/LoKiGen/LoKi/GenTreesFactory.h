// $Id$
// ============================================================================
#ifndef LOKI_GENTREESFACTORY_H 
#define LOKI_GENTREESFACTORY_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/iTree.h"
#include "LoKi/Trees.h"
// ============================================================================
// forward declarations 
// ============================================================================
namespace HepMC { class GenParticle ; }
// ============================================================================
/** @file LoKi/GenTreesFactory.h
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
 *
 *                    $Revision$
 *  Last modification $Date$
 *                 by $Author$
 */
// ============================================================================
namespace Decays 
{
  // ==========================================================================
  namespace Parsers  { class Tree ; }
  // ==========================================================================
  namespace Trees
  {
    // ========================================================================
    /** "Factory" to create the proper Tree from the full description 
     *   @param tree       (OUTPUT) the constructed tree 
     *   @param mother     (INPUT)  the mother particle 
     *   @param oscillated (INPUT)  the oscillation flag 
     *   @param arrow      (INPUT)  the type of arrow 
     *   @param daughters  (INPUT)  the list of daughter trees 
     *   @param inclusive  (INPUT)  the flag for inclusive 
     *   @param optional   (INPUT)  the list of "optional" particles 
     *   @param stream     (OUTPUT) the stream to report errors 
     *   @return StatusCode 
     *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *   @date   2009-05-06 
     */     
    StatusCode factory 
    ( Decays::Trees::Types_<const HepMC::GenParticle*>::Tree&           tree       ,
      const Decays::iNode&                                              mother     , 
      const Decays::Trees::Oscillation&                                 oscillated ,
      const Decays::Trees::Arrow&                                       arrow      ,
      const Decays::Trees::Types_<const HepMC::GenParticle*>::TreeList& daughters  , 
      const bool                                                        inclusive  ,
      const Decays::Trees::Types_<const HepMC::GenParticle*>::TreeList& optional   ,
      std::ostream&                                                     stream     ) ;
      // ========================================================================
    /** "Factory" to create the proper Tree from "short" descriptor
     *   @param tree       (OUTPUT) the constructed tree 
     *   @param mother     (INPUT)  the mother particle 
     *   @param oscillated (INPUT)  the oscillation flag 
     *   @return status code 
     *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *   @date   2009-05-06 
     */     
    StatusCode factory 
    ( Decays::Trees::Types_<const HepMC::GenParticle*>::Tree& tree       ,
      const Decays::iNode&                                    mother     ,
      const Decays::Trees::Oscillation&                       oscillated ) ;
    // ========================================================================
    /** "Factory" to create the proper Tree from the parsed tree 
     *   @param tree       (OUTPUT) the constructed tree 
     *   @param parsed     (INPUT)  the parsed tree  
     *   @param stream     (OUTPUT) the stream to report errors 
     *   @return status code  
     *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *   @date   2009-05-22 
     */     
    StatusCode factory 
    ( Decays::Trees::Types_<const HepMC::GenParticle*>::Tree& tree   ,
      const Decays::Parsers::Tree&                            parsed ,
      std::ostream&                                           stream ) ;
    // ========================================================================
  } //                                           end of namespace Decays::Trees
  // ==========================================================================
} //                                                    end of namespace Decays 
// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_GENTREESFACTORY_H
// ============================================================================
