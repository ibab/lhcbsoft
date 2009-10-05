# $Id: StrippingBs2PhiPhi.py,v 1.4 2009-10-05 15:20:47 schleich Exp $

__author__ = 'Sebastian Schleich'
__date__ = '05/10/2009'
__version__ = '$Revision: 1.4 $'

'''
Bs->PhiPhi stripping selection
'''

from Gaudi.Configuration import *
from StrippingConf.StrippingLine import StrippingLine, StrippingMember, StrippingTool
from Configurables import FilterDesktop, CombineParticles #, OfflineVertexFitter, PhysDesktop
import GaudiKernel.SystemOfUnits as Units


Bs2PhiPhiLooseDetachedPhi2KK = StrippingMember ( CombineParticles
                          , "Bs2PhiPhiLooseDetachedPhi2KK"
                          , InputLocations = ["StdNoPIDsKaons"]
                          , DecayDescriptor = "phi(1020) -> K+ K-"
                          , DaughtersCuts = {"K+": "((PIDK-PIDpi)>-7.0)&(MIPCHI2DV(PRIMARY)>4)"}
                          , CombinationCut = "(AM < 1100.*MeV)"
                          , MotherCut = "(VFASPF(VCHI2) < 25.0)"
                          )

Bs2PhiPhiPhi2KK = StrippingMember ( FilterDesktop
                , "Bs2PhiPhiPhi2KK"
                , InputLocations = ["%Bs2PhiPhiLooseDetachedPhi2KK"]
                , Code = "(ADMASS('phi(1020)') < 20.*MeV)"
                )

Bs2PhiPhi = StrippingMember( CombineParticles
          , "Bs2PhiPhi"
          , DecayDescriptor =  "[B_s0 -> phi(1020) phi(1020)]cc"
          , InputLocations  = [ "%Bs2PhiPhiPhi2KK" ]
          , DaughtersCuts   = {"phi(1020)" : "PT>650*MeV"}
          , CombinationCut  = "(ADAMASS('B_s0')<300*MeV)&(ACHILD(PT,1)*ACHILD(PT,2)>3.2*GeV*GeV)"
          , MotherCut       = "(VFASPF(VCHI2)<40)"
          , ReFitPVs        = True
          )

Bs2PhiPhiLine = StrippingLine('Bs2PhiPhiLine'
              , prescale = 1
              , postscale = 1
              , algos = [Bs2PhiPhiLooseDetachedPhi2KK, Bs2PhiPhiPhi2KK, Bs2PhiPhi]
              , stream = 'Hadron'
              )

Bs2PhiPhiWideLine = Bs2PhiPhiLine.clone('Bs2PhiPhiWideLine'
                  , prescale = 0.05
                  , postscale = 1
                  , FilterDesktopBs2PhiPhiPhi2KK = {"Code": "MM < 1100.*MeV"}
                  )

