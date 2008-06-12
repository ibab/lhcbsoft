#!/usr/bin/env gaudirun.py
# =============================================================================
# $Id: LoKi_Bs2PsiPhi.py,v 1.1 2008-06-12 10:44:01 ibelyaev Exp $ 
# =============================================================================
## @file
#  The configuration file to run LoKi_Bs2PsiPhi example
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
from  Configurables import LoKi_Bs2PsiPhi as PsiPhi

importOptions( "$DAVINCIROOT/options/DaVinciCommon.opts" )
importOptions( "$COMMONPARTICLESROOT/options/StandardKaons.opts" )
importOptions( "$COMMONPARTICLESROOT/options/StandardMuons.opts" )

## configure our own algorithm: 
alg = PsiPhi('PsiPhi')
alg.addTool ( PhysDesktop() )
alg.PhysDesktop.InputLocations = ['Phys/StdTightKaons' , 'Phys/StdTightMuons']
alg.PP2MCs = [ 'Relations/Rec/ProtoP/Charged' ]

## confgure the application itself:
appMgr = ApplicationMgr( EvtMax = 1000 )
appMgr.TopAlg += [ alg ]

## printout frequency
EventSelector ( PrintFreq = 10 ) 

## histograms:
HistogramPersistencySvc ( OutputFile = "PsiPhi_Histos.root" )

## input data:
importOptions ( os.environ['LOKIEXAMPLEDATA'] )

# =============================================================================
# The END
# =============================================================================
