# Configuration example using private databases for a millepede style alignment of the OT
# using 2009 collision data.
#
# example of particular conditions, and magnetic field polarity inversion,
# uncomment to use.
#from Configurables import ( CondDB, CondDBAccessSvc )
#otCalib = CondDBAccessSvc( 'OTCalib' )
#otCalib.ConnectionString ='sqlite_file:/data/work/DataBases/Collision09_OT_QuarterT0s_071209.db/LHCBCOND'
#CondDB().addLayer( otCalib )
#
#from Configurables import UpdateManagerSvc
#UpdateManagerSvc().ConditionsOverride += ["Conditions/Online/LHCb/Magnet/Measured := double Current = 5849.9936523438 ; int Polarity = -1;"]
#
#aligndb = '/data/work/DataBases/TTLayerITLayerOTModules_CollisionsDecember2009.db'
#from Configurables import ( CondDB, CondDBAccessSvc )
#alignCond = CondDBAccessSvc( 'AlignCond' )
#alignCond.ConnectionString = 'sqlite_file:' + aligndb + '/LHCBCOND'
#CondDB().addLayer( alignCond )

from Configurables import ( ATrackSelector, Escher )

ATrackSelector().MinPCut = 0.0 # value is in MeV
ATrackSelector().MinPtCut = 0.0
ATrackSelector().MinChi2Cut = 0.0
ATrackSelector().MaxChi2Cut = 15.0 # chi2perdof
ATrackSelector().Charge = 0
ATrackSelector().MaxPCut = -1 # value = -1 -> no p info available (Boff)
ATrackSelector().MaxPtCut = -1
ATrackSelector().MinITHitCut    = 0
ATrackSelector().MinOTHitCut = 15
ATrackSelector().MinTTHitCut = 0
ATrackSelector().MinEnergyCut = 0.0

Escher().Detectors = ["OT"]
Escher().EvtMax = 100
Escher().AlignmentLevel = "layers"
Escher().Millepede = True
Escher().Kalman = False
Escher().Incident = "GlobalMPedeFit"
Escher().TrackContainer = "Rec/Track/Best"

