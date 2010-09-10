#!/usr/bin/env python
# =============================================================================
# $Id: DataAccess.py,v 1.15 2010-09-10 09:33:32 ibelyaev Exp $ 
# =============================================================================
# $URL$ 
# =============================================================================
## @file solutions/DataAccess.py
#
#  Simple example that illustrates the access to data (tracks) and loop
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
#  with the campain of Dr.O.Callot et al.: 
#  ``No Vanya's lines are allowed in LHCb/Gaudi software.''
#
#  @author Vanya BELYAEV ibelyaev@physics.syr.edu
#  @date 2006-10-12
#
#  Last modification $Date: 2010-09-10 09:33:32 $
#                 by $Author: ibelyaev $
# =============================================================================
"""
Simple example that illustrates the access to data (tracks) and loop

This file is a part of BENDER project:
``Python-based Interactive Environment for Smart and Friendly Physics Analysis''

The project has been designed with the kind help from
Pere MATO and Andrey TSAREGORODTSEV. 

And it is based on the 
LoKi project: ``C++ ToolKit for Smart and Friendly Physics Analysis''

By usage of this code one clearly states the disagreement 
with the campain of Dr.O.Callot et al.: 
``No Vanya's lines are allowed in LHCb/Gaudi software.''

Last modification $Date: 2010-09-10 09:33:32 $
               by $Author: ibelyaev $
"""
# =============================================================================
__author__  = " Vanya BELYAEV Ivan.Belyaev@nikhef.nl "
__date__    = " 2006-10-12 " 
__version__ = " Version $Revision: 1.15 $ "
# =============================================================================
## import everything from BENDER
from Bender.Main import *
# =============================================================================
## @class GetData
#  Simple algorithm to access the data 
class GetData(Algo):

    # =========================================================================
    ## the main method: analyse the event
    def analyse( self ) :
        """
        the main method: analyse the event
        """
        
        ## get and print the event number 
        hdr  = self.get( 'Header' )
        if hdr :
            print ' Event/Run #%d/%d ' % ( hdr.evtNum() , hdr.runNum() )
            
        ## get tracks 
        tracks = self.get ( 'Rec/Track/Best' )
        print ' # of tracks : ' , tracks.size() 
            
        ## for the first five  tracks print PT 
        i = 0
        for track in tracks :
            if i >= 5 : break
            print ' Track, pt: ', track.pt() 
            
        return SUCCESS
# =============================================================================

# =============================================================================
## The configuration of the job
def configure( inputdata , catalogs = [] ) :
    """
    The configuration of the job
    """
    from Configurables import DaVinci
    DaVinci ( DataType   = '2010' ,
              Lumi       = False  )

    ## get/create Application Manager
    gaudi = appMgr()

    setData ( inputdata , catalogs )
    
    # modify/update the configuration:
    
    ## create the algorithm
    alg = GetData( 'GetData' )
    
    ## replace the list of top level algorithms by
    #  new list, which contains only *THIS* algorithm
    gaudi.setAlgorithms( [ alg ] )
    
    return SUCCESS 
# =============================================================================
    
# =============================================================================
## Job steering 
if __name__ == '__main__' :
    
    print '*'*120
    print                      __doc__
    print ' Author  : %s ' %   __author__    
    print ' Version : %s ' %   __version__
    print ' Date    : %s ' %   __date__
    print '*'*120
    
    
    ## job configuration
    inputdata = [
        '/castor/cern.ch/grid/lhcb/data/2010/DST/00005848/0000/00005848_00000001_1.V0.dst' ,
        '/castor/cern.ch/grid/lhcb/data/200/DST/00005848/0000/00005848_00000002_1.V0.dst' 
        ]
    
    configure( inputdata )
    
    ## event loop 
    run(10)

    tracks = get('/Event/Rec/Track/Best')
    print '# TRACKS : ', tracks.size() 

    
    
# =============================================================================
# The END
# =============================================================================
