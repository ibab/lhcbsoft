from Gaudi.Configuration import *
from Configurables import ( DaVinci, GaudiSequencer )
from Configurables import ( ANNGlobalPID__ChargedProtoANNPIDTrainingTuple,
                            ANNGlobalPID__ChargedProtoANNPIDTupleTool )

pidtuple = ANNGlobalPID__ChargedProtoANNPIDTrainingTuple("ANNPID")
pidtuple.addTool( ANNGlobalPID__ChargedProtoANNPIDTupleTool, name = "Tuple" )
pidtuple.Tuple.NTupleLUN = "ANNPIDTUPLE"
NTupleSvc().Output += ["ANNPIDTUPLE DATAFILE='ProtoPIDANN.tuples.root' TYP='ROOT' OPT='NEW'"]
   
DaVinci().UserAlgorithms += [ pidtuple ]

DaVinci().EvtMax     = -1
DaVinci().PrintFreq  = 1000
#DaVinci().SkipEvents = 100000

DaVinci().InputType = 'DST' 

DaVinci().DataType = "2011"
DaVinci().Simulation = True

EventSelector().Input = [
    "DATAFILE='PFN:/usera/jonesc/NFS/data/MC/MC11a/DST/Inc-B/00013171_00000001_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'",
    "DATAFILE='PFN:/usera/jonesc/NFS/data/MC/MC11a/DST/Inc-B/00013171_00000002_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'",
    "DATAFILE='PFN:/usera/jonesc/NFS/data/MC/MC11a/DST/Inc-B/00013171_00000003_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'",
    "DATAFILE='PFN:/usera/jonesc/NFS/data/MC/MC11a/DST/Inc-B/00013171_00000004_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'",
    "DATAFILE='PFN:/usera/jonesc/NFS/data/MC/MC11a/DST/Inc-B/00013171_00000005_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'",
    "DATAFILE='PFN:/usera/jonesc/NFS/data/MC/MC11a/DST/Inc-B/00013171_00000006_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'",
    "DATAFILE='PFN:/usera/jonesc/NFS/data/MC/MC11a/DST/Inc-B/00013171_00000007_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'",
    "DATAFILE='PFN:/usera/jonesc/NFS/data/MC/MC11a/DST/Inc-B/00013171_00000008_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'",
    "DATAFILE='PFN:/usera/jonesc/NFS/data/MC/MC11a/DST/Inc-B/00013171_00000009_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'",
    "DATAFILE='PFN:/usera/jonesc/NFS/data/MC/MC11a/DST/Inc-B/00013171_00000010_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'"
    ]

## EventSelector().Input=[
##     "DATAFILE='LFN:/lhcb/MC/MC11a/ALLSTREAMS.DST/00013171/0000/00013171_00000009_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'",
##     "DATAFILE='LFN:/lhcb/MC/MC11a/ALLSTREAMS.DST/00013171/0000/00013171_00000010_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'",
##     "DATAFILE='LFN:/lhcb/MC/MC11a/ALLSTREAMS.DST/00013171/0000/00013171_00000002_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'",
##     "DATAFILE='LFN:/lhcb/MC/MC11a/ALLSTREAMS.DST/00013171/0000/00013171_00000004_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'",
##     "DATAFILE='LFN:/lhcb/MC/MC11a/ALLSTREAMS.DST/00013171/0000/00013171_00000006_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'",
##     "DATAFILE='LFN:/lhcb/MC/MC11a/ALLSTREAMS.DST/00013171/0000/00013171_00000005_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'",
##     "DATAFILE='LFN:/lhcb/MC/MC11a/ALLSTREAMS.DST/00013171/0000/00013171_00000001_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'",
##     "DATAFILE='LFN:/lhcb/MC/MC11a/ALLSTREAMS.DST/00013171/0000/00013171_00000003_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'",
##     "DATAFILE='LFN:/lhcb/MC/MC11a/ALLSTREAMS.DST/00013171/0000/00013171_00000008_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'",
##     "DATAFILE='LFN:/lhcb/MC/MC11a/ALLSTREAMS.DST/00013171/0000/00013171_00000007_1.allstreams.dst' TYP='POOL_ROOTTREE' OPT='READ'"
##     ]
