from Gaudi.Configuration import *
from Configurables import LHCbApp, CondDB, DDDBConf

dataType="2011"

DDDBConf(DataType=dataType)
CondDB(IgnoreHeartBeat=True)

if "UseLatestTags" in CondDB().__slots__:
    CondDB().UseLatestTags=[dataType, True]
else:
    CondDB().useLatestTags(DataType=dataType, OnlyGlobalTags=True)
