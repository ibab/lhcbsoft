#!/usr/bin/env python
# =============================================================================
"""
Standard 'Hello, world!' example.
No way to avoid it!
"""
# =============================================================================
# @file
#
# "Solution"-file for 'Hello,World!' example (Bender Tutorial)
#
# @author Vanya BELYAEV ibelyaev@physics.syr.edu
# @date   2004-10-12
# =============================================================================
__author__  = ' Vanya BELYAEV  Ivan.Belyaev@nikhef.nl '
__version__ = ' CVS tag $Name: not supported by cvs2svn $, version $Revision: 1.11 $  '  
# =============================================================================
## import everything from BENDER
from Bender.Main import *
# =============================================================================
## @class HelloWorld
#  simple Python/Bender class for classical 'Hello,World!' example
class HelloWorld(Algo):
    """
    The most trivial algorithm
    """

    ## the main 'analysis' method 
    def analyse( self ) :
        """
        The main 'analysis' method
        """
        
        ## use Python printout:
        print 'Hello, world!'
        
        ## use Gaudi printout:
        self.Print( 'Hello, World! (using Gaudi)')
        
        return SUCCESS
# =============================================================================

# =============================================================================
## The configuration of the job
def configure() :
    

    ## get some external configuration
    importOptions ( '$DAVINCIROOT/options/DaVinciCommon.opts' )

    ## get/create application manager
    gaudi = appMgr() 
    
    # modify/update the configuration:
    
    # (1) create the algorithm
    alg = HelloWorld( 'Hello' )
    
    # (2) replace the list of top level algorithm by
    #     new list, which contains only *THIS* algorithm
    gaudi.setAlgorithms( [ alg ] )
    
    ## redefine input files 
    evtSel = gaudi.evtSel()
    import LoKiExample.Bs2Jpsiphi_mm_data as data 
    evtSel.open( data.Files ) 
    
    return SUCCESS 
# =============================================================================

# =============================================================================
## Job steering 
if __name__ == '__main__' :

    ## job configuration
    configure()

    ## event loop 
    run(50)
        
# =============================================================================
# The END
# =============================================================================


