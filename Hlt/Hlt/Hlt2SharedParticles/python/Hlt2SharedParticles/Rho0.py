### @file
#
#  Standard rho(770)0
#
##
from Gaudi.Configuration import *
from Configurables import CombineParticles
from HltLine.HltLine import bindMembers
from Hlt2SharedParticles.GoodParticles import GoodPions

__all__ = ('Rho0')

Hlt2SharedRho0 = CombineParticles("Hlt2SharedRho0")
Hlt2SharedRho0.Output = 'Hlt2/Hlt2SharedRho0/Particles'
Hlt2SharedRho0.Inputs  = [ GoodPions.outputSelection() ]
Hlt2SharedRho0.DecayDescriptor = "[rho(770)0 -> pi+ pi-]cc"
Hlt2SharedRho0.DaughtersCuts =  { "pi+" : "TRUE" ,
                                  "pi-" : "TRUE" }
Hlt2SharedRho0.CombinationCut = "(ADAMASS('rho(770)0')<100*MeV)"
Hlt2SharedRho0.MotherCut = "(BPVVDZ>0) & (VFASPF(VCHI2PDOF)<9) & (BPVDIRA>0.95) & (BPVVDCHI2>25)"

Rho0 = bindMembers( None, [ GoodPions, Hlt2SharedRho0 ] )
