# $Id: $
# Test your line(s) of the stripping

from Gaudi.Configuration import *
from Configurables import SelDSTWriter, DaVinci
from StrippingConf.Configuration import StrippingConf

# Tighten Trk Chi2 to <3
from CommonParticles.Utils import DefaultTrackingCuts
DefaultTrackingCuts().Cuts  = { "Chi2Cut" : [ 0, 3 ],
                                "CloneDistCut" : [5000, 9e+99 ] }
# Now build the stream
from StrippingConf.StrippingStream import StrippingStream
Stream = StrippingStream("TEST")

from StrippingSelections import StrippingPhiToKSKS
confPhiToKSKS = StrippingPhiToKSKS.PhiToKSKSAllLinesConf("PhiToKSKS", StrippingPhiToKSKS.confdict)
Stream.appendLines( confPhiToKSKS.lines() )

from Configurables import  ProcStatusCheck
filterBadEvents =  ProcStatusCheck()

# Configure the stripping using the same options as in Reco06-Stripping10
sc = StrippingConf( Streams = [ Stream ],
                    MaxCandidates = 2000,
                    AcceptBadEvents = False,
                    BadEventSelection = filterBadEvents )

from Configurables import CondDB
CondDB().IgnoreHeartBeat = True

# Remove the microbias and beam gas etc events before doing the tagging step
regexp = "HLT_PASS_RE('Hlt1(?!ODIN)(?!L0)(?!Lumi)(?!Tell1)(?!MB)(?!NZS)(?!Velo)(?!BeamGas)(?!Incident).*Decision')"
from Configurables import LoKi__HDRFilter
filterHLT = LoKi__HDRFilter("FilterHLT",Code = regexp )

MessageSvc().Format = "% F%60W%S%7W%R%T %0W%M"

from Configurables import AuditorSvc, ChronoAuditor
AuditorSvc().Auditors.append( ChronoAuditor("Chrono") )

from Configurables import StrippingReport
sr = StrippingReport(Selections = sc.selections())
sr.OnlyPositive = False
sr.ReportFrequency = 1000

from Configurables import AlgorithmCorrelationsAlg
ac = AlgorithmCorrelationsAlg(Algorithms = sc.selections())

from Configurables import CondDB
CondDB().IgnoreHeartBeat = True

from Configurables import FilterDesktop

StdParticles = []
#StdParticles.append("Phys/StdNoPIDsProtons")
#StdParticles.append("Phys/StdNoPIDsPions")
#StdParticles.append("Phys/StdNoPIDsKaons")
#StdParticles.append("Phys/StdNoPIDsMuons")
#StdParticles.append("Phys/StdAllLoosePions")
#StdParticles.append("Phys/StdLoosePions")
StdParticles.append("Phys/StdLooseKaons")
#StdParticles.append("Phys/StdLooseProtons")
StdParticles.append("Phys/StdLooseKsLL")
#StdParticles.append("Phys/StdLooseKsDD")
#StdParticles.append("Phys/StdLooseLambdaLL")
#StdParticles.append("Phys/StdLooseLambdaDD")
#StdParticles.append("Phys/StdLooseResolvedPi0")
#StdParticles.append("Phys/StdLooseMergedPi0")
#StdParticles.append("Phys/StdLooseDstarWithD02KPi")
MakePionsEtc = FilterDesktop('MakePionsEtc')
MakePionsEtc.Inputs=StdParticles
MakePionsEtc.Code="ALL"

DaVinci().PrintFreq = 10000
DaVinci().HistogramFile = 'DV_stripping_histos.root'
DaVinci().EvtMax = 100000
DaVinci().EventPreFilters = [ filterHLT ]
DaVinci().appendToMainSequence( [MakePionsEtc] )
#DaVinci().appendToMainSequence( [KsLL_SEQ])
DaVinci().appendToMainSequence( [ sc.sequence() ] )
DaVinci().appendToMainSequence( [ sr ] )
#DaVinci().appendToMainSequence( [ ac ] )
DaVinci().DataType = "2012"
DaVinci().InputType = 'SDST'
#DaVinci().DDDBtag  = "head-20120413"
#DaVinci().CondDBtag = "head-20120420"
DaVinci().DDDBtag  = "dddb-20120831"
DaVinci().CondDBtag = "cond-20121008"


#importOptions("$STRIPPINGSELECTIONSROOT/tests/data/Reco13c_Run124134.py")
#importOptions("$STRIPPINGSELECTIONSROOT/tests/data/Reco14_2011Data_MagDn.py")
importOptions("$STRIPPINGSELECTIONSROOT/tests/data/Reco14_Run125113.py")
#importOptions("$STRIPPINGSELECTIONSROOT/Reco13e.py")
