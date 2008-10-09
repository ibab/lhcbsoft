#!/usr/bin/env python
# =============================================================================
"""
'Solution'-file for 'DataAccess/GetData' example (Bender Tutorial)
"""
# =============================================================================
## @file
#
#  "Solution"-file for 'DataAccess/GetData' example (Bender Tutorial)
#
#  @author Vanya BELYAEV  belyaev@lapp.in2p3.fr
#  @date   2004-10-12
# =============================================================================
__author__  = ' Vanya BELYAEV  Ivan.Belyaev@nikhef.nl '
__version__ = ' CVS tag $Name: not supported by cvs2svn $, version $Revision: 1.11 $  '  
# =============================================================================
## import everything from BENDER
from Bender.MainMC import *
# =============================================================================
## @class GetData
#  Simple algorithm to access the data 
class GetData(Algo):
    # =========================================================================
    ## standard constructor 
    def __init__ ( self , name = 'GetData' ) :
        """
        Standard constructor
        """
        ## construct the base class 
        return Algo.__init__ ( self , name )

    # =========================================================================
    ## the main method: analyse the event
    def analyse( self ) :
        """ the main method: analyse the event """

        ## get and print the event number 
        hdr  = self.get( 'Header' )
        if hdr :
            print ' Event/Run #%d/%d ' %( hdr.evtNum() , hdr.runNum() )
        
        ## get all MC particles 
        mcps = self.get( 'MC/Particles' )
        print ' # of MC-particles : ' , mcps.size() 
        
        ## get all MC collisisons 
        colls = self.get( 'Gen/Collisions')
        for c in colls :
            print 'Collision:' , `c` 
            
        ## for the first 5 particles print PID information
        i = 0
        for mcp in mcps :
            if i >= 5 : break
            print " MCParticle name is '%s' " % mcp.name()            
            print ' The decay chain is  (decay only)    : ', mcp.decay ( True  ) 
            print ' The decay chain is  (+interactions) : ', mcp.decay ( False ) 
            print ' MCParticle info: ', `mcp`
            i +=1 
            
        return SUCCESS
# =============================================================================

# =============================================================================
## The configuration of the job
def configure( **args ) :
    """
    The configuration of the job
    """
    
    ## general configuration :
    importOptions ( '$DAVINCIROOT/options/DaVinciCommon.opts' )

    ## get/create Application Manager
    gaudi = appMgr()
    
    # modify/update the configuration:
    
    ## create the algorithm
    alg = GetData( 'GetData' )
    
    ## replace the list of top level algorithms by
    #  new list, which contains only *THIS* algorithm
    gaudi.setAlgorithms( [ alg ] )
    
    ## define input data files :
    #  get the Event Selector from Gaudi
    evtSel = gaudi.evtSel()
    
    ## configure Event Selector
    import LoKiExample.Bs2Jpsiphi_mm_data as data 
    evtSel.open( data.Files ) 
    
    return SUCCESS 
# =============================================================================
    
# =============================================================================
## Job steering 
if __name__ == '__main__' :
    """
    Job steering
    """
    
    ## job configuration
    configure()
    
    ## event loop 
    run(10)
    
# =============================================================================
# The END
# =============================================================================
