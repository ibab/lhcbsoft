#!/usr/bin/env python
# =============================================================================
## @file LoKiGen/decorators.py
#  The set of basic decorator for objects from LoKiGen library
#
#  This file is a part of LoKi project - 
#   'C++ ToolKit  for Smart and Friendly Physics Analysis'
#
#  The package has been designed with the kind help from
#  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
#  contributions and advices from G.Raven, J.van Tilburg, 
#  A.Golutvin, P.Koppenburg have been used in the design.
#
#  @author Vanya BELYAEV ibelyaev@physics.syr.edu
# =============================================================================
"""
The set of basic decorators for obejcts from LoKiGen library

This file is a part of LoKi project - 
'C++ ToolKit  for Smart and Friendly Physics Analysis'

The package has been designed with the kind help from
Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
contributions and advices from G.Raven, J.van Tilburg, 
A.Golutvin, P.Koppenburg have been used in the design.
"""
# =============================================================================
__author__ = "Vanya BELYAEV ibelyaev@physics.syr.edu" 
# =============================================================================

from   LoKiGen.functions   import *
from   LoKiGen.HepMC       import *

_name = __name__ 
# =============================================================================
## make the decoration of all objects fomr this module
def _decorate ( name = _name  ) :
    """ Make the decoration of all objects fomr this module """
    import LoKiCore.decorators as _LoKiCore
    _gp  = 'const HepMC::GenParticle*'
    _gv  = 'const HepMC::GenVertex*'
    _vgp = std.vector ( _gp      )
    _vgv = std.vector ( _gv      )
    _vd  = std.vector ( 'double' )
    _decorated  = _LoKiCore.getAndDecorateFunctions (
        name                                             , ## module name 
        LoKi.Functor('const HepMC::GenParticle*,double') , ## the base 
        LoKi.Dicts.FunCalls(HepMC.GenParticle)           , ## call-traits 
        LoKi.Dicts.FuncOps(_gp,_gp)                      ) ## operators
    _decorated |= _LoKiCore.getAndDecorateFunctions  (
        name                                             , ## module name 
        LoKi.Functor('const HepMC::GenVertex*,double')   , ## the base 
        LoKi.Dicts.FunCalls(HepMC.GenVertex)             , ## call-traits 
        LoKi.Dicts.FuncOps(_gv,_gv)                      ) ## operators
    _decorated |=  _LoKiCore.getAndDecoratePredicates (
        name                                             , ## module name 
        LoKi.Functor('const HepMC::GenParticle*,bool')   , ## the base 
        LoKi.Dicts.CutCalls(HepMC.GenParticle)           , ## call-traits 
        LoKi.Dicts.CutsOps(_gp,_gp)                      ) ## operators 
    _decorated |= _LoKiCore.getAndDecoratePredicates (
        name                                             , ## module name 
        LoKi.Functor('const HepMC::GenVertex*,bool')     , ## the base 
        LoKi.Dicts.CutCalls(HepMC.GenVertex)             , ## call-traits 
        LoKi.Dicts.CutsOps(_gv,_gv)                      ) ## the operators
    ## functional:
    _decorated |= _LoKiCore.getAndDecorateMaps       (
        name                                             , ## module name
        LoKi.Functor( _vgp , _vd )                       , ## the base
        LoKi.Dicts.MapsOps(_gp)                          ) ## stremers
    _decorated |= _LoKiCore.getAndDecorateMaps       (
        name                                             , ## module name
        LoKi.Functor( _vgv , _vd )                       , ## the base
        LoKi.Dicts.MapsOps(_gv)                          ) ## stremers    
    _decorated |= _LoKiCore.getAndDecorateMaps       (
        name                                             , ## module name
        LoKi.Functor( _vgp , _vgp )                      , ## the base
        LoKi.Dicts.PipeOps(_gp,_gp)                      ) ## stremers
    _decorated |= _LoKiCore.getAndDecorateMaps       (
        name                                             , ## module name
        LoKi.Functor( _vgv , _vgv )                      , ## the base
        LoKi.Dicts.PipeOps(_gv,_gv)                      ) ## stremers    
    _decorated |= _LoKiCore.getAndDecorateMaps       (
        name                                             , ## module name
        LoKi.Functor( _vgp , 'double' )                  , ## the base
        LoKi.Dicts.FunValOps(_gp)                        ) ## stremers
    _decorated |= _LoKiCore.getAndDecorateMaps       (
        name                                             , ## module name
        LoKi.Functor( _vgv , 'double' )                  , ## the base
        LoKi.Dicts.FunValOps(_gv)                        ) ## stremers    
    _decorated |= _LoKiCore.getAndDecorateMaps       (
        name                                             , ## module name
        LoKi.Functor( _vgp , _gp )                       , ## the base
        LoKi.Dicts.ElementOps(_gp,_gp)                   ) ## stremers
    _decorated |= _LoKiCore.getAndDecorateMaps       (
        name                                             , ## module name
        LoKi.Functor( _vgv , _gv )                       , ## the base
        LoKi.Dicts.ElementOps(_gv,_gv)                   ) ## stremers    
    print "8 ",len(_decorated)    
    # decorate pids (Comparison with strings, integers and ParticleID objects:
    for t in ( GID , GABSID ) :
        t = type ( t ) 
        _LoKiCore.decoratePID ( t , LoKi.Dicts.PIDOps ( t ) )
        _decorated.add( t ) 
    ## 
    return _decorated                            ## RETURN

# =============================================================================
## perform the decoration 
_decorated = _decorate ()                         ## ATTENTION 
# =============================================================================

# =============================================================================
if '__main__' == __name__ :
    print 'Number of properly decorated types: %s'%len(_decorated)
    for o in _decorated : print o
        
# =============================================================================
# The END
# =============================================================================
