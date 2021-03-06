# Config for performance testing 
from Gaudi.Configuration import *
from Configurables import NoPIDsParticleMaker
from CommonParticles.StdAllNoPIDsPions import StdAllNoPIDsPions

importOptions("$BRUNELROOT/options/Brunel-Default.py")
importOptions("$PRCONFIGOPTS/Brunel/PR-COLLISION12-Beam4000GeV-VeloClosed-MagDown.py")


from Gaudi.Configuration import FileCatalog, EventSelector
from Configurables import Brunel
Brunel().EvtMax=10

pmaker =  NoPIDsParticleMaker ( 'StdNoPIDsPions' , Particle = 'pion'  )


def postConfig():

    from Configurables import JsonConverter
    conv = JsonConverter()
    #conv.OutputLevel = DEBUG
    from Configurables import StoreExplorerAlg
    storeExp = StoreExplorerAlg("StoreExplorer1")
    storeExp.Load = 1
    storeExp.PrintFreq = 1

    #GaudiSequencer("PhysicsSeq").Members += [ storeExp,  conv ]
    GaudiSequencer("PhysicsSeq").Members += [ pmaker, storeExp, conv ]



appendPostConfigAction(postConfig)
