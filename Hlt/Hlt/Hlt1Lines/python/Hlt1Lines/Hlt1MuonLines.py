from Gaudi.Configuration import * 
from HltLine.HltLinesConfigurableUser import *
import re

class Hlt1MuonLinesConf( HltLinesConfigurableUser ):
    # steering variables
    __slots__ = {  'DoTiming'                 : False
                 , 'SingleMuonHighPT_P'       : 8000.
                 , 'SingleMuonHighPT_PT'      : 4800.
                 , 'SingleMuonHighPT_TrChi2'  :    4.
                 , 'SingleMuonHighPT_TrNTHits'   : 0 #OFF
                 , 'SingleMuonHighPT_Velo_NHits' : 0 #OFF
                 , 'SingleMuonHighPT_Velo_Qcut'  : 999 #OFF
                 , 'SingleMuonHighPT_GEC'     : 'Loose'
                 , 'SingleMuonNoIP_P'         : 6000.
                 , 'SingleMuonNoIP_PT'        : 1300.
                 , 'SingleMuonNoIP_TrChi2'    :    4.
                 , 'SingleMuonNoIP_TrNTHits'   : 16
                 , 'SingleMuonNoIP_Velo_NHits' : 9
                 , 'SingleMuonNoIP_Velo_Qcut'  : 3
                 , 'SingleMuonNoIP_GEC'       : 'Loose'
                 , 'DiMuonLowMass_VxDOCA'     :  0.2
                 , 'DiMuonLowMass_VxChi2'     :   25.
                 , 'DiMuonLowMass_P'          : 6000.
                 , 'DiMuonLowMass_PT'         :  200.
                 , 'DiMuonLowMass_TrChi2'     :    4.
                 , 'DiMuonLowMass_M'          : 1000.
                 , 'DiMuonLowMass_IPChi2'     :    9.
                 , 'DiMuonLowMass_GEC'        : 'Loose'
                 , 'DiMuonHighMass_VxDOCA'    :  0.2
                 , 'DiMuonHighMass_VxChi2'    :   25.
                 , 'DiMuonHighMass_P'         : 6000.
                 , 'DiMuonHighMass_PT'        :  200.
                 , 'DiMuonHighMass_TrChi2'    :    4.
                 , 'DiMuonHighMass_M'         : 2900.
                 , 'DiMuonHighMass_GEC'       : 'Loose'
                 , 'DiMuonSoft_VxDOCA'     :  0.2
                 , 'DiMuonSoft_VxChi2'     :   25.
                 , 'DiMuonSoft_P'          : 3000.
                 , 'DiMuonSoft_PT'         :    0.
                 , 'DiMuonSoft_TrChi2'     :    2.
                 , 'DiMuonSoft_M'          :    0.
                 , 'DiMuonSoft_IPChi2'     :    9.
                 , 'DiMuonSoft_GEC'        : 'Loose'
                 , 'MultiMuonNoIP_P'          : 6000.
                 , 'MultiMuonNoIP_PT'         :  500.
                 , 'MultiMuonNoIP_TrChi2'     :    4.
                 , 'MultiMuonNoIP_GT'         :  2.5
                 , 'MultiMuonNoIP_GEC'        : 'Loose'
                 , 'MultiMuonNoIP_TrackType'  : 'Long'
        
                 , 'L0Channels'               : { 'SingleMuonHighPT' : ( 'Muon', ),
                                                  'SingleMuonNoIP'   : ( 'Muon', ),
                                                  'DiMuonLowMass'    : ( 'Muon', 'DiMuon' ),
                                                  'DiMuonSoft'       : None,
                                                  'DiMuonHighMass'   : ( 'Muon', 'DiMuon' ),
                                                  'MultiMuonNoIP'    : ( 'Muon', 'DiMuon' )}
                 , 'Priorities'               : { 'SingleMuonHighPT' : 7,
                                                  'SingleMuonNoIP'   : 6,
                                                  'DiMuonLowMass'    : 5,
                                                  'DiMuonHighMass'   : 4,
                                                  'MultiMuonNoIP'    : 8,
                                                  'DiMuonSoft'       : 9,
                                                  }
                 }
    
    def localise_props( self, prefix ):
        ps = self.getProps()
        props = dict( [ ( key.split( '_', 1 )[ 1 ], ps[ key ] ) for key in ps if key.find( prefix ) != -1 ] )
        props[ 'name' ] = prefix
        return props

    def do_timing( self, unit ):
        from Configurables import LoKi__HltUnit as HltUnit
        if not isinstance( unit, HltUnit ):
            return unit
        reco = set()
        for entry in unit.Preambulo:
            s = entry.split( '=' )
            if s[ 0 ].find( 'PV3D' ) != -1 or s[ 0 ].find( 'GEC' ) != -1: continue
            if len( s ) > ( 1 ):
                reco.add( s[ 0 ].strip() )
        name = unit.name()[ 4 : unit.name().find( 'Streamer' ) ]
        code = unit.Code
        for step in reco:
            sub = " ( timer( '%s_%s' )" % ( name, step ) + ' % ' +  step + ' ) '
            code = re.sub( '\\s+%s\\s+' % step, sub, code )
        unit.Code = code
        return unit

    def singleMuon_preambulo( self, properties ):
        from HltTracking.Hlt1Tracking import ( TrackCandidates, IsMuon,
                                               FitTrack, MatchVeloTTMuon,
                                               VeloTTCandidates,
                                               ComplementForward )
        return [ TrackCandidates( properties[ 'name' ] ),
                 IsMuon, ComplementForward, MatchVeloTTMuon,
                 VeloTTCandidates( properties[ 'name' ] ),
                 FitTrack ]

    def singleMuon_streamer( self, properties ):
        # SingleMuon with LongTracks (TrackCandidates) 
        from Hlt1Lines.Hlt1GECs import Hlt1GECUnit
        from Configurables import LoKi__HltUnit as HltUnit
        from HltTracking.Hlt1Tracking import TrackCandidatesAlgos
        unit = HltUnit(
            'Hlt1%(name)sStreamer' % properties,
            ##OutputLevel = 1 ,
            Preambulo = self.singleMuon_preambulo( properties ),
            Code = """
            VeloTTCandidates
            >>  MatchVeloTTMuon
            >>  tee  ( monitor( TC_SIZE > 0, '# MatchMuon', LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE , 'nMatched' , LoKi.Monitoring.ContextSvc ) )
            >>  ComplementForward
            >>  tee  ( monitor( TC_SIZE > 0, '# Complement', LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE , 'nComp' , LoKi.Monitoring.ContextSvc ) )
            >>  FitTrack
            >>  tee  ( monitor( TC_SIZE > 0, '# pass fit', LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE , 'nFitted' , LoKi.Monitoring.ContextSvc ) )
            >>  ( ( TrPT > %(PT)s * MeV ) & ( TrP  > %(P)s  * MeV ) )
            >>  tee  ( monitor( TC_SIZE , 'n after P/PT' , LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE > 0, '# pass P/PT', LoKi.Monitoring.ContextSvc ) )
            >>  ( TrCHI2PDOF < %(TrChi2)s )
            >>  IsMuon
            >>  tee  ( monitor( TC_SIZE > 0, '# pass IsMuon', LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE , 'nIsMuon' , LoKi.Monitoring.ContextSvc ) )
            >>  SINK( 'Hlt1%(name)sDecision' )
            >> ~TC_EMPTY
            """ % properties
            )
        gec = properties[ 'GEC' ]
        return [ Hlt1GECUnit( gec ),
                 TrackCandidatesAlgos(properties['name']),
                 unit ]

    def diMuon_preambulo( self, properties ):
        ## define some "common" preambulo 
        preambulo = self.singleMuon_preambulo( properties ) + \
             [ "VertexConf = LoKi.Hlt1.VxMakerConf( %(VxDOCA)f * mm, %(VxChi2)f )" % properties,
               "MakeDiMuons = TC_VXMAKE4( '', VertexConf )",
               "from LoKiPhys.decorators import RV_MASS" ]
        return preambulo

    def diMuon_streamer( self, properties ):
        # DiMuon with LongTracks (TrackCandidates) 
        from Hlt1Lines.Hlt1GECs import Hlt1GECUnit
        from HltTracking.Hlt1Tracking import TrackCandidatesAlgos
        from Configurables import LoKi__HltUnit as HltUnit
        unit = HltUnit(
            'Hlt1%(name)sStreamer' % properties,
            ##OutputLevel = 1 ,
            Preambulo = self.diMuon_preambulo( properties ),
            Code = """
            VeloTTCandidates
            >>  MatchVeloTTMuon
            >>  tee  ( monitor( TC_SIZE > 0, '# MatchMuon', LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE , 'nMatched' , LoKi.Monitoring.ContextSvc ) )
            >>  ComplementForward
            >>  tee  ( monitor( TC_SIZE > 0, '# Complement', LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE , 'nComp' , LoKi.Monitoring.ContextSvc ) )
            >>  FitTrack
            >>  tee  ( monitor( TC_SIZE > 0, '# pass fit', LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE , 'nFitted' , LoKi.Monitoring.ContextSvc ) )
            >>  ( ( TrPT > %(PT)s * MeV ) & ( TrP  > %(P)s  * MeV ) )
            >>  tee  ( monitor( TC_SIZE , 'n after P/PT' , LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE > 0, '# pass P/PT', LoKi.Monitoring.ContextSvc ) )
            >>  ( TrCHI2PDOF < %(TrChi2)s )
            >>  IsMuon
            >>  tee  ( monitor( TC_SIZE > 0, '# pass IsMuon', LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE , 'nIsMuon' , LoKi.Monitoring.ContextSvc ) )
            >>  MakeDiMuons
            >>  tee  ( monitor( TC_SIZE > 0, '# pass vertex', LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE , 'nVertices' , LoKi.Monitoring.ContextSvc ) )
            >>  ( RV_MASS ( 'mu+' , 'mu-' ) > %(M)s * MeV ) 
            >>  tee  ( monitor( TC_SIZE > 0, '# pass mass', LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE , 'nDiMuons' , LoKi.Monitoring.ContextSvc ) )
            >>  SINK( 'Hlt1%(name)sDecision' )
            >> ~TC_EMPTY
            """ % properties
            )
        gec = properties[ 'GEC' ]
        return [ Hlt1GECUnit( gec ),
                 TrackCandidatesAlgos(properties['name']),
                 unit ]

    def diMuonDetached_streamer( self, properties ):
        from Hlt1Lines.Hlt1GECs import Hlt1GECUnit
        from HltTracking.Hlt1Tracking import TrackCandidatesAlgos
        from Configurables import LoKi__HltUnit as HltUnit
        from HltTracking.HltPVs import PV3D
        unit = HltUnit(
            'Hlt1%(name)sStreamer' % properties,
            ##OutputLevel = 1 ,
            Preambulo = self.diMuon_preambulo( properties ),
            Code = """
            VeloTTCandidates
            >>  MatchVeloTTMuon
            >>  tee  ( monitor( TC_SIZE > 0, '# MatchMuon', LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE , 'nMatched' , LoKi.Monitoring.ContextSvc ) )
            >>  ComplementForward
            >>  tee  ( monitor( TC_SIZE > 0, '# Complement', LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE , 'nComp' , LoKi.Monitoring.ContextSvc ) )
            >>  FitTrack
            >>  tee  ( monitor( TC_SIZE > 0, '# pass fit', LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE , 'nFitted' , LoKi.Monitoring.ContextSvc ) )
            >>  ( ( TrPT > %(PT)s * MeV ) & ( TrP  > %(P)s  * MeV ) )
            >>  tee  ( monitor( TC_SIZE , 'n after P/PT' , LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE > 0, '# pass P/PT', LoKi.Monitoring.ContextSvc ) )
            >>  ( ( TrCHI2PDOF < %(TrChi2)s ) & ( Tr_HLTMIPCHI2( 'PV3D' ) > %(IPChi2)s ) )
            >>  tee  ( monitor( TC_SIZE , 'n after Tr/IPChi2' , LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE > 0, '# pass Tr/IPChi2', LoKi.Monitoring.ContextSvc ) )
            >>  IsMuon
            >>  tee  ( monitor( TC_SIZE > 0, '# pass IsMuon', LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE , 'nIsMuon' , LoKi.Monitoring.ContextSvc ) )
            >>  MakeDiMuons
            >>  tee  ( monitor( TC_SIZE > 0, '# pass vertex', LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE , 'nVertices' , LoKi.Monitoring.ContextSvc ) )
            >>  ( RV_MASS ( 'mu+' , 'mu-' ) > %(M)s * MeV ) 
            >>  tee  ( monitor( TC_SIZE > 0, '# pass mass', LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE , 'nDiMuons' , LoKi.Monitoring.ContextSvc ) )
            >>  SINK( 'Hlt1%(name)sDecision' )
            >> ~TC_EMPTY
            """ % properties
            )
        gec = properties[ 'GEC' ]

        return [ Hlt1GECUnit( gec ),
                 PV3D('Hlt1'),
                 TrackCandidatesAlgos(properties['name']),
                 unit ]

    def multiMuon_streamer( self, properties ):
        from Hlt1Lines.Hlt1GECs import Hlt1GECUnit
        from Configurables import LoKi__HltUnit as HltUnit
        unit = HltUnit(
            'Hlt1%(name)sStreamer' % properties,
            ##OutputLevel = 1 ,
            Preambulo = self.singleMuon_preambulo( properties ),
            Code = """
            TrackCandidates
            >>  FitTrack
            >>  tee  ( monitor( TC_SIZE > 0, '# pass fit', LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE , 'nFitted' , LoKi.Monitoring.ContextSvc ) )
            >>  ( ( TrPT > %(PT)s * MeV ) & ( TrP  > %(P)s  * MeV ) )
            >>  tee  ( monitor( TC_SIZE , 'n after P/PT' , LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE > 0, '# pass P/PT', LoKi.Monitoring.ContextSvc ) )
            >>  ( TrCHI2PDOF < %(TrChi2)s )
            >>  tee  ( monitor( TC_SIZE , 'n after TrChi2' , LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE > 0, '# pass TrChi2', LoKi.Monitoring.ContextSvc ) )
            >>  IsMuon
            >>  tee  ( monitor( TC_SIZE > 0, '# pass IsMuon', LoKi.Monitoring.ContextSvc ) )
            >>  tee  ( monitor( TC_SIZE , 'nIsMuon' , LoKi.Monitoring.ContextSvc ) )
            >>  SINK( 'Hlt1%(name)sDecision' )
            >>  TC_SIZE > %(GT)s
            """ % properties
            )
        gec = properties[ 'GEC' ]
        return [ Hlt1GECUnit( gec ), unit ]
        
    def build_line( self, name, streamer ):
        from HltLine.HltLine import Hlt1Line
        algos = [ self.do_timing( unit ) if self.getProp('DoTiming') else unit for unit in streamer( self.localise_props( name ) ) ]
        priorities = self.getProp( "Priorities" )
        priority = priorities[ name ] if name in priorities else None
        line = Hlt1Line(
            name,
            prescale  = self.prescale,
            postscale = self.postscale,
            priority  = priority,
            L0DU = "|".join( [ "L0_CHANNEL('%s')" % l0 for l0 in self.L0Channels[ name ] ] )  if self.L0Channels[name] else None, 
            algos = algos
            )

    def __apply_configuration__( self ) : 
         ## Create the lines
        to_build = [ ( 'SingleMuonHighPT', self.singleMuon_streamer ),
                     ( 'SingleMuonNoIP',   self.singleMuon_streamer ),
                     ( 'DiMuonLowMass',    self.diMuonDetached_streamer ),
                     ( 'DiMuonHighMass',   self.diMuon_streamer ),
                     ( 'DiMuonSoft',    self.diMuonDetached_streamer ),
                     ( 'MultiMuonNoIP',    self.multiMuon_streamer ) ]
        for line, streamer in to_build:
            self.build_line( line, streamer )

        from HltLine.HltLine import Hlt1Line   as Line
        # The pass-through NoPV line
        Hlt1NoPVMuonPassThrough = Line( 'NoPVPassThrough'
                                        , prescale = self.prescale
                                        , L0DU = "L0_CHANNEL_RE('.*lowMult')"
                                        , ODIN = "~jbit( ODIN_EVTTYP,0 )"
                                        , postscale = self.postscale)

