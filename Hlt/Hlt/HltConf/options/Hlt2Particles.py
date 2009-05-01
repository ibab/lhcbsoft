## @file 
#
#  Hlt exclusive selections particle making
#
#  @author P. Koppenburg
#  @date 2006-02-01

from Gaudi.Configuration import importOptions
from Configurables import GaudiSequencer
from Configurables import NumberOfTracksFilter
from Configurables import HltInsertTrackErrParam
from Configurables import InSpdAcceptanceAlg, InPrsAcceptanceAlg, InEcalAcceptanceAlg, InHcalAcceptanceAlg, InBremAcceptanceAlg
from Configurables import ElectronMatchAlg, BremMatchAlg, PhotonMatchAlg
from Configurables import Track2SpdEAlg, Track2PrsEAlg, Track2EcalEAlg, Track2HcalEAlg
from Configurables import EcalChi22ID, BremChi22ID, ClusChi22ID
from Configurables import MuonID
from Configurables import ChargedProtoPAlg, ChargedProtoCombineDLLsAlg, NeutralProtoPAlg


##
##---------------------------------------------------------------------
## Charged ProtoParticles
##---------------------------------------------------------------------
SeqHlt2Charged = GaudiSequencer('SeqHlt2Charged'
                               , MeasureTime = True
                               , IgnoreFilterPassed = False)

SeqHlt2Particles = GaudiSequencer('SeqHlt2Particles'
                                 , MeasureTime = True 
                                 , IgnoreFilterPassed = True) # do all 

SeqHlt2Particles.Members += [ SeqHlt2Charged ]

GaudiSequencer('Hlt2').Members += [ SeqHlt2Particles ]

#
# @todo TEMPORARY kill huge events
#
NumberOfTracksFilter =  NumberOfTracksFilter()
NumberOfTracksFilter.TrackLocations = [ "Hlt/Track/Forward" ]
NumberOfTracksFilter.MaxTracks = 1000 
SeqHlt2Charged.Members += [ NumberOfTracksFilter ]

#
# MC truth associated tracks
#
SeqHlt2Charged.Members += [ GaudiSequencer('SeqTrueSignalTracks') ] #  debug
#
# Hacking of errors
#
HltInsertTrackErrParam = HltInsertTrackErrParam()
HltInsertTrackErrParam.InputLocation = "Hlt/Track/Forward" 
SeqHlt2Charged.Members += [ HltInsertTrackErrParam ]


## -------------------------
## Calo Reco & PIDs --------
## -------------------------
SeqHlt2Particles.Members += [ GaudiSequencer('HltRecoCALOSeq') ]

## Options for Calo reconstruction
importOptions("$CALORECOROOT/options/HltCaloSeq.opts")

##/ @todo temporary : redefine HLT track location to "Hlt/Track/ForwardCleaned"
InSpdAcceptanceAlg('HltInSPD').Inputs   =  [ "Hlt/Track/Forward" ]
InPrsAcceptanceAlg('HltInPRS').Inputs   =  [ "Hlt/Track/Forward" ]
InEcalAcceptanceAlg('HltInECAL').Inputs =  [ "Hlt/Track/Forward" ]
InHcalAcceptanceAlg('HltInHCAL').Inputs =  [ "Hlt/Track/Forward" ]
InBremAcceptanceAlg('HltInBREM').Inputs =  [ "Hlt/Track/Forward" ]
ElectronMatchAlg('HltElectronMatch').Tracks =["Hlt/Track/Forward"]
BremMatchAlg('HltBremMatch').Tracks =      ["Hlt/Track/Forward"]
PhotonMatchAlg('HltClusterMatch').Tracks = ["Hlt/Track/Forward" ]
Track2SpdEAlg('HltSpdE').Inputs =          ["Hlt/Track/Forward" ]
Track2PrsEAlg('HltPrsE').Inputs =          ["Hlt/Track/Forward" ]
Track2EcalEAlg('HltEcalE').Inputs =        ["Hlt/Track/Forward" ]
Track2HcalEAlg('HltHcalE').Inputs =        ["Hlt/Track/Forward" ]
EcalChi22ID('HltEcalChi22ID').Tracks =     ["Hlt/Track/Forward"]
BremChi22ID('HltBremChi22ID').Tracks =    ["Hlt/Track/Forward"]
ClusChi22ID('HltClusChi22ID').Tracks =    ["Hlt/Track/Forward"]

##---------------------------------------------------------------------
## MuonID
##---------------------------------------------------------------------
importOptions("$MUONIDROOT/options/MuonID.py")

HltMuonID = MuonID().clone('HltMuonID')
HltMuonID.TrackLocation = "Hlt/Track/Forward" 
HltMuonID.MuonIDLocation = "Hlt/Muon/MuonPID" 
HltMuonID.MuonTrackLocation = "Hlt/Track/Muon"

HltMuonIDSeq = GaudiSequencer('HltMuonIDSeq')
HltMuonIDSeq.Members += [ "MuonRec", HltMuonID ]

SeqHlt2Particles.Members += [ HltMuonIDSeq ]

##----------------------------------------------------------------------------------------------------
## C.Jones : OLD RICH options
## Disabled by default. To enable uncomment the lines below
##----------------------------------------------------------------------------------------------------

## Global algorithm, using all three radiators
###include "$RICHHLTSYSOPTS/GlobalPID_AllRads.opts"

## Global algorithm, using only the gas radiators (faster, but no low P PID)
###include "$RICHHLTSYSOPTS/GlobalPID_GasRads.opts"

## Local algorithm (fastest)
###include "$RICHHLTSYSOPTS/LocalPID.opts"

## temporary, to use the cleaned forward tracks
##ToolSvc.HLT.RichTrackCreator.TracksLocation = "Hlt/Track/Forward"

## Explicitly run the RICH reco sequence
##SeqHlt2Charged.Members += { "GaudiSequencer/HltRICHReco" }
## ... or on-demand only when really needed
##DataOnDemandSvc.Algorithms += {"DATA='/Event/Rec/Rich/HltPIDs' 
##                              TYPE='GaudiSequencer/HltRICHReco'"}
##-----------------------------------------------------------------

##---------------------------------------------------------------------
## C.Jones : RICH options
## Disabled by default. To enable uncomment the lines below
##---------------------------------------------------------------------
## #include "$HLTCONFROOT/options/Hlt2Rich.py"
##CreateHLTTracks.OutputLevel = 1
##ToolSvc.HLT_RichTrackCreator.OutputLevel  = 1
##---------------------------------------------------------------------

##---------------------------------------------------------------------
## ChargedProtoPAlg
##---------------------------------------------------------------------
Hlt2ChargedProtoPAlg = ChargedProtoPAlg('Hlt2ChargedProtoPAlg')
Hlt2ChargedProtoPAlg.InputTrackLocation = "Hlt/Track/Forward"  #  @todo Correct this
Hlt2ChargedProtoPAlg.OutputProtoParticleLocation = "Hlt/ProtoP/Charged" 
##Hlt2ChargedProtoPAlg.InputRichPIDLocation = "Rec/Rich/HltPIDs"
Hlt2ChargedProtoPAlg.InputMuonPIDLocation = "Hlt/Muon/MuonPID"
## Calo PID
Hlt2ChargedProtoPAlg.UseCaloSpdPID = True 
Hlt2ChargedProtoPAlg.UseCaloPrsPID = True 
Hlt2ChargedProtoPAlg.UseCaloEcalPID = True 
Hlt2ChargedProtoPAlg.UseCaloHcalPID = True 
Hlt2ChargedProtoPAlg.UseCaloBremPID = True 
##Hlt2ChargedProtoPAlg.UseRichPID = False  // Protos will NOT have any RICH information - HltRichPIDsKaons will not work
Hlt2ChargedProtoPAlg.UseRichPID = True     ## Use this to add RICH info to the HLT protos, needed for HltRichPIDsKaons
Hlt2ChargedProtoPAlg.UseMuonPID = True 
Hlt2ChargedProtoPAlg.UseVeloPID = False 
SeqHlt2Particles.Members += [ Hlt2ChargedProtoPAlg ]

Hlt2ChargedProtoCombDLL = ChargedProtoCombineDLLsAlg('Hlt2ChargedProtoCombDLL')
Hlt2ChargedProtoCombDLL.ProtoParticleLocation = "Hlt/ProtoP/Charged" 
SeqHlt2Particles.Members += [ Hlt2ChargedProtoCombDLL ]


##---------------------------------------------------------------------
## NeutralProtoPAlg
##---------------------------------------------------------------------
SeqHlt2Particles.Members += [ NeutralProtoPAlg('HltNeutralProtoPAlg') ]


