#
#   Stripping selections test job
#
#   @author A. Poluektov
#   @date 2009-09-29
#

from Gaudi.Configuration import *
from StrippingConf.Configuration import StrippingConf

from StrippingSelections import StreamBmuon, StreamHadron, StreamJpsi, StreamDstar, StreamLambda, StreamBelectron

MessageSvc().Format = "% F%60W%S%7W%R%T %0W%M"

#
# Stripping job configuration
#

sc = StrippingConf()
sc.appendStream( StreamBmuon.stream )
sc.appendStream( StreamHadron.stream )
sc.appendStream( StreamJpsi.stream )
sc.appendStream( StreamDstar.stream )
sc.appendStream( StreamLambda.stream )
sc.appendStream( StreamBelectron.stream )

from Configurables import EventTuple, TupleToolSelResults

tag = EventTuple("TagCreator")
tag.EvtColsProduce = True
tag.ToolList = [ "TupleToolEventInfo", "TupleToolRecoStats", "TupleToolSelResults"  ]
tag.addTool(TupleToolSelResults)

tag.TupleToolSelResults.Selections = sc.selections()  # Add the list of stripping selections to TagCreator

from Configurables import DaVinci

DaVinci().appendToMainSequence( [ sc.sequence() ] )   # Append the stripping selection sequence to DaVinci
DaVinci().appendToMainSequence( [ tag ] )             # Append the TagCreator to DaVinci
DaVinci().EvtMax = 1000                        # Number of events
DaVinci().DataType = "2008"
DaVinci().Simulation   = True
DaVinci().ETCFile = "etc.root"                 # ETC file name
DaVinci().HistogramFile = "TestStripping.root"

importOptions("$STRIPPINGSELECTIONSROOT/tests/2008-InclBJpsiMuMu.py")     # Data file
