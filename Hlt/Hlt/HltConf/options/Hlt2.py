### @file 
 #
 # Hlt exclusive selections options
 #
 # @author P. Koppenburg
 # @date 2006-02-01
###
from Gaudi.Configuration import *
from Configurables import GaudiSequencer
from Configurables import HltSelectionsDecision
# ---------------------------------------------------------------------
#  HLT exclusive Selections
# ---------------------------------------------------------------------
Hlt2 = GaudiSequencer("Hlt2", Context = 'HLT' )
#  just in case
if Hlt2 not in GaudiSequencer("Hlt").Members : GaudiSequencer("Hlt").Members += [ Hlt2 ]
# ---------------------------------------------------------------------
#  Make sure Hlt1 is passed
# ---------------------------------------------------------------------
Hlt2.Members += [ GaudiSequencer("Hlt2Requirements") ]
# ---------------------------------------------------------------------
#  Full reconstruction of all tracks 
# ---------------------------------------------------------------------
from HltConf.HltReco import HltRecoSequence
Hlt2.Members += [ HltRecoSequence ]
# ---------------------------------------------------------------------
#  ProtoParticles and Particles
# ---------------------------------------------------------------------
importOptions( "$HLTCONFROOT/options/Hlt2Particles.py" )
# ---------------------------------------------------------------------
#  Empty Sequencer for MC truth filtering --- filled in HltSelChecker
# ---------------------------------------------------------------------
Hlt2.Members += [ GaudiSequencer("SeqHlt2TruthFilter") ] 
importOptions( "$HLTCONFROOT/options/HltTrackAssociator.py" )
# ---------------------------------------------------------------------
# Initialise Hlt2Shared particles
# ---------------------------------------------------------------------
importOptions( "$HLTSELECTIONSROOT/options/Hlt2Shared.py" )
# ---------------------------------------------------------------------
#  Final selections
# ---------------------------------------------------------------------
Hlt2.Members += [ GaudiSequencer('Hlt2Lines',ModeOR=True,ShortCircuit=False) ] 
importOptions( "$HLTSELECTIONSROOT/options/Hlt2Lines.py" )
