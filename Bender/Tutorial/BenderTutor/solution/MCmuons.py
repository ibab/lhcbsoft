#!/usr/bin/env python
# =============================================================================
# $Id: MCmuons.py,v 1.4 2004-11-25 12:10:35 ibelyaev Exp $
# =============================================================================
# CVS tag $Name: not supported by cvs2svn $ 
# =============================================================================

# =============================================================================
# @file
#
# "Solution"-file for 'MCmuons.py' example (Bender Tutorial)
#
# @author Vanya BELYAEV  belyaev@lapp.in2p3.fr
# @date   2004-10-12
# =============================================================================

# import everything from BENDER
from bendermodule import *

# =============================================================================
# define the primitive algorithm 
# =============================================================================
class MCMuons(Algo):
    def analyse( self ) :
        
        # get *ALL* Monte Carlo muons 
        mcmu   = self.mcselect ( tag = 'mcmu'   , cuts = 'mu+' == MCABSID )

        # get all beauty hadrons 
        beauty  = self.mcselect( tag = 'beauty' , cuts = BEAUTY )
        
        # from selected muons select the muons from beauty
        muFromB = self.mcselect ( tag    = 'muFromCharm'         ,
                                  source = mcmu                  ,
                                  cuts   = FROMMCTREE( beauty )  )

        # get muons produce in vicinity of  primary vertex
        muFromPV = self.mcselect ( tag    = 'MuFromPV'                    ,
                                   source = muFromB                       ,
                                   cuts   = MCVXFUN( MCVZ ) < ( 20 * cm ) )
        
        print '  MCMuons:         '                  , \
              '  \tTotal            ' , mcmu.size()    , \
              ', \tFrom B-tree:     ' , muFromB.size() , \
              ', \tProduced near PV ' , muFromPV.size()
        
        return SUCCESS
# =============================================================================

# =============================================================================
# The configuration of the job 
# =============================================================================
def configure() :
    
    gaudi.config ( files = ['$BENDERTUTOROPTS/BenderTutor.opts' ] )
    
    # modify/update the configuration:
    
    # 1) create the algorithm
    alg = MCMuons( 'McMu' )
    
    # 2) replace the list of top level algorithm by only *THIS* algorithm
    gaudi.setAlgorithms( [ alg ] ) 

    return SUCCESS
# =============================================================================

# =============================================================================
# The control flow 
# =============================================================================
if __name__ == '__main__' :

    # job configuration
    configure()

    # event loop 
    gaudi.run(50)

    # for the interactive mode it is better to comment the last line
    gaudi.exit()
# =============================================================================

# =============================================================================
# $Log: not supported by cvs2svn $
# =============================================================================
# The END 
# =============================================================================
