#!/usr/bin/env python
# =============================================================================
# $Id: RCSelect.py,v 1.13 2009-03-04 12:56:49 ibelyaev Exp $ 
# =============================================================================
"""
'Solution'-file for 'RCselect.py' example (Bender Tutorial)
""" 
# =============================================================================
# @file
#
# "Solution"-file for 'RCselect.py' example (Bender Tutorial)
#
# @author Vanya BELYAEV ibelyaev@physics.syr.edu
# @date   2004-10-12
# =============================================================================
__author__  = ' Vanya BELYAEV  Ivan.Belyaev@nikhef.nl '
__version__ = ' CVS tag $Name: not supported by cvs2svn $, version $Revision: 1.13 $  '  
# =============================================================================
## import everything from BENDER
from Bender.Main import *
# =============================================================================
## @class RCselect
#  simple demo-algorithm 
class RCSelect(Algo):
    """
    The simple demo-algorithm
    """
    ## the main analysis algorithm 
    def analyse( self ) :
        """
        The main analysis algorithm
        """        
        ## select muons for J/Psi reconstruction 
        muons = self.select( "mu" , ( "mu+" == ABSID ) & ( PT > 500  ) )
        if muons.empty() : return SUCCESS
        
        ## select kaons for Phi reconstruction
        kaons = self.select( "K"  , ( "K+" == ABSID ) & ( PIDK > 0.0  ) )
        if kaons.empty() : return SUCCESS

        ## delta mass cut fro J/psi  
        dmPsi = ADMASS('J/psi(1S)') < 50
        
        psis = self.loop ( 'mu mu' , 'J/psi(1S)' )
        for psi in psis :
            m = psi.mass(1,2) / 1000 
            if not 2.5 < m < 3.5     : continue
            # neutral combination?
            if not 0 == SUMQ( psi )  : continue 
            # check the chi2 of the vertex fit
            if not 0 <= VCHI2( psi ) < 49 : continue
            
            self.plot( M ( psi ) / 1000  ,
                       " dimuon invariant mass " ,
                       2.5 , 3.5 )

            if not dmPsi( psi ) : continue
            psi.save( 'psi' )
            
        # delta mass cut for phi 
        dmPhi = ADMASS('phi(1020)') < 20 
            
        phis = self.loop( 'K K' , 'phi(1020)' )
        for phi in phis :
            if phi.mass( 1 ,2 ) > 1050     : continue 
            # neutral combination ?
            if not 0 == SUMQ ( phi )       : continue
            # check the chi2 of the vertex fit
            if not 0 <= VCHI2( phi ) < 49 : continue
            
            self.plot ( M(phi) / 1000 ,
                        " dikaon invariant mass " ,
                        1.0 , 1.050 )

            if not dmPhi( phi ) : continue
            phi.save('phi')

        # delta mass cut for Bs 
        dmBs = ADMASS('B_s0') < 100      
        bs = self.loop( 'psi phi' , 'B_s0' )
        for B in bs :
            mass = B.mass(1,2) / 1000 
            if not 4.5 < mass < 6.5      : continue
            if not 0 <= VCHI2( B ) <  49 : continue
            
            self.plot ( M(B) / 1000 ,
                        " psi phi invarinat mass " ,
                        5.0 , 6.0 )
            
            if not dmBs ( B ) : continue
            B.save('Bs')
            
        Bs = self.selected('Bs')
        if not Bs.empty() : self.setFilterPassed ( True )
        
        return SUCCESS 
    
# =============================================================================
## Job configuration:
def configure() :
    """
    The Job configuration
    """ 
    
    from Configurables import DaVinci
    
    DaVinci (
        DataType   = 'DC06'     , # default  
        Simulation = True       ,
        HltType    = '' ) 
   
    from Gaudi.Configuration import HistogramPersistencySvc
    HistogramPersistencySvc ( OutputFile = 'RCselect_histos.root' ) 


    ## get/create application manager
    gaudi = appMgr() 
    
    # modify/update the configuration:
        
    # 1) create the algorithm
    alg = RCSelect( 'RCSelect' )
    
    # 2) add the algorithm to the list of top-level algorithms 
    # gaudi.addAlgorithm( alg ) 
    gaudi.setAlgorithms( [alg] ) 
    
    # 3) configure algorithm
    desktop = gaudi.tool('RCSelect.PhysDesktop')
    desktop.InputLocations = [
        'Phys/StdTightKaons' , 
        'Phys/StdTightMuons' ]
    
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
    run(5000)

# =============================================================================
# The END 
# =============================================================================
