#!/usr/bin/env gaudirun.py
# =============================================================================
# $Id: LoKi_MCMuon.py,v 1.3 2008-07-01 11:12:10 ibelyaev Exp $ 
# =============================================================================
## @file
#  The configuration file to run LoKi_MCMuon example
#
#   This file is a part of LoKi project - 
#     "C++ ToolKit  for Smart and Friendly Physics Analysis"
#
#  The package has been designed with the kind help from
#  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
#  contributions and advices from G.Raven, J.van Tilburg, 
#  A.Golutvin, P.Koppenburg have been used in the design.
#
#  By usage of this code one clearly states the disagreement 
#  with the campain of Dr.O.Callot et al.: 
#  "No Vanya's lines are allowed in LHCb/Gaudi software."
#
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date   2008-06-07
# =============================================================================
import os 
from  Gaudi.Configuration import * 
from  Configurables import PhysDesktop
from  Configurables import LoKi__MCMuons as MCMuons

importOptions( "$DAVINCIROOT/options/DaVinciCommon.opts" )
importOptions( "$COMMONPARTICLESROOT/options/StandardMuons.opts" )

## configure our own algorithm: 
alg = MCMuons('MCMuon')
alg.addTool ( PhysDesktop() )
alg.PhysDesktop.InputLocations = ['Phys/StdTightMuons']
alg.NTupleLUN = "MCMU"
alg.PP2MCs = [ 'Relations/Rec/ProtoP/Charged' ]

## confgure the application itself:
appMgr = ApplicationMgr( EvtMax = 1000 )
appMgr.TopAlg += [ alg ]

## printout frequency
EventSelector ( PrintFreq = 100 ) 

## histograms:
HistogramPersistencySvc ( OutputFile = "MCMuon_Histos.root" )

## n-Tuples
NTupleSvc ( Output = [ "MCMU DATAFILE='MCMuon_Tuples.root' TYP='ROOT' OPT='NEW'" ] ) ;

## input data:
importOptions ( os.environ['LOKIEXAMPLEDATA'] )

# =============================================================================
# The END
# =============================================================================
