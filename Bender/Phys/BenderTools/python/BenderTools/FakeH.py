#!/usr/bin/env python
# -*- coding: utf-8 -*-
# =============================================================================
# @file BenderTools/FakeH.py
#
#  Helper utulities to substitute PID for the basic particle
#  @code
#
#  mass = DTF_FUN ( M , True , 'J/psi(1S)') ## calculate the mass with constrains
#  for B in container :
#
#
#      mass_B  = mass ( B )  ## calcualte the mass with all constraints
#      
#      pion = B(1)  ## get the first daughter
#      with FakeKaon ( pion ) :
#         mass_fake = mass ( B ) ## calculate the mass after pion -> kaon change
#  
#  @endcode
#  @thanks Wouter    HULSBERGEN
#  @thanks Alexander BARANOV 
#
#  This file is a part of 
#  <a href="http://cern.ch/lhcb-comp/Analysis/Bender/index.html">Bender project</a>
#  <b>``Python-based Interactive Environment for Smart and Friendly 
#   Physics Analysis''</b>
#
#  The package has been designed with the kind help from
#  Pere MATO and Andrey TSAREGORODTSEV. 
#  And it is based on the 
#  <a href="http://cern.ch/lhcb-comp/Analysis/LoKi/index.html">LoKi project:</a>
#  ``C++ ToolKit for Smart and Friendly Physics Analysis''
#
#  By usage of this code one clearly states the disagreement 
#  with the smear campaign of Dr.O.Callot et al.: 
#  ``No Vanya's lines are allowed in LHCb/Gaudi software.''
#
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date   2014-05-12
#
#                   $Revision: 169835 $
# Last modification $Date: 2014-03-09 14:25:30 +0100 (Sun, 09 Mar 2014) $
#                by $Author: ibelyaev $
# =============================================================================
"""
Helper utulities to substitute PID for the basic particle

#  @code
#
#  mass = DTF_FUN ( M , True , 'J/psi(1S)') ## calculate the mass with constrains
#  for B in container :
#
#
#      mass_B  = mass ( B )  ## calcualte the mass with all constraints
#      
#      pion = B(1)  ## get the first daughter
#      with FakeKaon ( pion ) :
#         mass_fake = mass ( B ) ## calculate the mass after pion -> kaon change
#  
#  @endcode

Thanks Wouter    HULSBERGEN and Alexander BARANOV



This file is a part of BENDER project:
    ``Python-based Interactive Environment for Smart and Friendly Physics Analysis''

The project has been designed with the kind help from Pere MATO and Andrey TSAREGORODTSEV. 

And it is based on the LoKi project:
    ``C++ ToolKit for Smart and Friendly Physics Analysis''

By usage of this code one clearly states the disagreement 
with the smear campain of Dr.O.Callot et al.: 
    ``No Vanya's lines are allowed in LHCb/Gaudi software.''

"""
# =============================================================================
__version__ = "$Revision: 169835 $"
__author__  = "Vanya BELYAEV Ivan.Belyaev@itep.ru"
__date__    = "2014-05-12"
# =============================================================================
__all__= (
    ##
    'FakeH'        , ## generic classes (requires configuration)
    #
    'FakeKaon'     , ## replace charged particle with kaon ID 
    'FakePion'     , ## replace charged particle with pion ID 
    'FakeProton'   , ## replace charged particle with proton ID 
    'FakeElectron' , ## replace charged particle with electron ID 
    'FakeMuon'     , ## replace charged particle with muon ID
    #
    'FakeK'        , ## replace charged particle with kaon ID 
    'FakePi'       , ## replace charged particle with pion ID 
    'FakeP'        , ## replace charged particle with proton ID 
    'FakeE'        , ## replace charged particle with electron ID 
    'FakeMu'       , ## replace charged particle with muon ID
    ##
    'validatePIDs' , ## helper function to validate hardcorded PIDs
    ## 
    )
# =============================================================================
## logging
# =============================================================================
from Bender.Logger import getLogger 
logger = getLogger( __name__ )
# ==============================================================================
from   LoKiCore.basic  import cpp, LHCb, LoKi 
import PartProp.decorators 
# ==============================================================================
## @class FakeH
#  Helper class to substitute PID for the basic particle
#  @code
#
#  mass = DTF_FUN ( M , True , 'J/psi(1S)') ## calculate the mass with constrains
#  for B in container :
#
#
#      mass_B  = mass ( B )  ## calcualte the mass with all constraints
#      
#      pion = B(1)  ## get the first daughter
#      with FakeKaon ( pion ) :
#         mass_fake = mass ( B ) ## calculate the mass after pion -> kaon change
#  
#  @endcode
#  @attention DecayTreeFitter is needed after!
#  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
#  @thanks Wouter    HULSBERGEN
#  @thanks Alexander BARANOV 
#  @date 2014-05-12
class FakeH ( object ) :
    """
    Helper class to substitute PID for the basic particle
    #
    #  mass = DTF_FUN ( M , True , 'J/psi(1S)') ## calculate the mass with constrains
    #  for B in container :
    #
    #
    #      mass_B  = mass ( B )  ## calcualte the mass with all constraints
    #      
    #      pion = B(1)  ## get the first daughter
    #      with FakeKaon ( pion ) :
    #         mass_fake = mass ( B ) ## calculate the mass after pion -> kaon change
    #   
    """
    ## constructor
    def __init__  ( self , particle , _mapping ) :

        self._particle = particle
        self._oldpid   = None
        
        if particle : 
            _pid           = particle.particleID() 
            self._newpid   = _mapping.get( _pid , None )

    def __enter__ ( self ) :
            
        if self._particle and self._newpid  :
            _pid = self._particle.paricleID()
            self._oldpid   = LHC.Particle ( _pid ) 
            self._particle.setParticleID ( self._newpid )
            
    def __exit__  ( self, *_ ) :

        if self._particle and self._oldpid :
            self._particle.setParticleID ( self._oldpid )


# ===============================================================================
# | pi-               |         -211 |  -1  |   139.6 MeV | 
# | pi+               |          211 |   1  |   139.6 MeV |
# | K-                |         -321 |  -1  |   493.7 MeV |
# | K+                |          321 |   1  |   493.7 MeV |
# | e+                |          -11 |   1  |   0.511 MeV |
# | e-                |           11 |  -1  |   0.511 MeV |
# | mu+               |          -13 |   1  |   105.7 MeV |
# | mu-               |           13 |  -1  |   105.7 MeV |
# | p~-               |        -2212 |  -1  |   938.3 MeV |
# | p+                |         2212 |   1  |   938.3 MeV |
# ===============================================================================
_particles_ = {
    'pi+' : LHCb.ParticleID (   211 ) ,
    'pi-' : LHCb.ParticleID (  -211 ) ,
    'K+'  : LHCb.ParticleID (   321 ) ,
    'K-'  : LHCb.ParticleID (  -321 ) ,
    'p+'  : LHCb.ParticleID (  2212 ) ,
    'p~-' : LHCb.ParticleID ( -2212 ) ,
    'K-'  : LHCb.ParticleID (  -321 ) ,
    'e+'  : LHCb.ParticleID (   -11 ) ,
    'e-'  : LHCb.ParticleID (    11 ) ,
    'mu+' : LHCb.ParticleID (   -13 ) ,
    'mu-' : LHCb.ParticleID (    13 ) ,
    }
# ===============================================================================
## helper local function to buld proper maps accoring to the charge 
def _make_pids_ ( name ) :
    """Helper local function to buld proper maps accoring to the charge"""
    ## 
    result = {}
    pid1   = _particles_ [ name ]
    for k in _particles_ :
        pid2 = _particles_ [k]
        if pid2.threeCharge() == pid1.threeCharge() : result [ pid2 ] = pid1

    #    
    return result 
# =============================================================================
_to_kaon      =          _make_list_ ( 'K+'  )
_to_kaon      . update ( _make_list_ ( 'K-'  ) ) 
_to_pion      =          _make_list_ ( 'pi+' )
_to_pion      . update ( _make_list_ ( 'pi-' ) ) 
_to_proton    =          _make_list_ ( 'p+'  )
_to_proton    . update ( _make_list_ ( 'p~-' ) ) 
_to_electron  =          _make_list_ ( 'e+'  )
_to_electron  . update ( _make_list_ ( 'e-'  ) ) 
_to_muon      =          _make_list_ ( 'mu+' )
_to_muon      . update ( _make_list_ ( 'mu-' ) )
# =============================================================================
class FakeKaon     (FakeH) :
    def __init__ ( self , particle ) : FakeH.__init__( self, particle , _to_kaon     )
class FakePion     (FakeH) :
    def __init__ ( self , particle ) : FakeH.__init__( self, particle , _to_pion     )
class FakeProton   (FakeH) :
    def __init__ ( self , particle ) : FakeH.__init__( self, particle , _to_proton   )
class FakeElectron (FakeH) :
    def __init__ ( self , particle ) : FakeH.__init__( self, particle , _to_electron )
class FakeMuon     (FakeH) :
    def __init__ ( self , particle ) : FakeH.__init__( self, particle , _to_muon     )
# ==============================================================================
FakeK  = FakeKaon
FakePi = FakePion
FakeP  = FakeProton
FakeE  = FakeElectron
FakeMu = FakeMuon 
# ==============================================================================
## it is highly recommended to call this function in run-time 
def validatePIDs ( parts = _particles_ ) :
    """
    Try to validate the hardcoded PIDs
    It is highly recommended to call this function in run-time 
    """
    ppsv = LoKi.Services.instance().ppSvc() 
    if not ppsvc :
        logger.error("Unable to validate, Particle properties service is invalid ")
        return False

    _error = False 
    for key in parts :
        pid1 = parts [key] 
        pid2 = LoKi.Particles.pidFromName ( key )
        
        if pid1.pid() != pid2.pid() :
            logger.error("Unable to validate %s  %s %s "
                         % ( key , pid1.pid() , pid2.pid() ) )
            _error = True 

    return _error

# =============================================================================
if '__main__' == __name__ :

    print 80*'*'
    print __doc__
    print ' Author  : ' , __author__
    print ' Version : ' , __version__
    print ' Date    : ' , __date__    
    print ' Symbols : ' , __all__    
    print 80*'*'
    print ' Particle -> KAON     :' , _to_kaon
    print ' Particle -> PION     :' , _to_pion
    print ' Particle -> PROTON   :' , _to_proton
    print ' Particle -> ELECTRON :' , _to_electron
    print ' Particle -> MUON     :' , _to_muon
    
    
# =============================================================================
# The END 
# =============================================================================
