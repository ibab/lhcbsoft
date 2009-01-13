# =============================================================================
# $Id: HltHadronLines.py,v 1.3 2009-01-13 15:35:38 graven Exp $
# =============================================================================
## @file
#  Configuration of Hadron Lines
#  @author Gerhard Raven Gerhard.Raven@nikhef.nl
#  @date 2008-08-25
# =============================================================================
"""
 script to configure Hadron trigger lines
"""
# =============================================================================
__author__  = "Gerhard Raven Gerhard.Raven@nikhef.nl"
__version__ = "CVS Tag $Name: not supported by cvs2svn $, $Revision: 1.3 $"
# =============================================================================

from Gaudi.Configuration import * 
from LHCbKernel.Configuration import *

from Configurables import GaudiSequencer
from Configurables import HltTrackUpgrade

from HltConf.HltLine import Hlt1Line   as Line
from HltConf.HltLine import bindMembers
from HltConf.HltLine import Hlt1Member as Member
from HltConf.HltLine import Hlt1Tool   as Tool
from HltConf.HltLine import hlt1Lines  
from HltConf.HltL0Candidates import *

# here get a Hlt1 line 
def _getLine(name):
    line = filter( lambda x : x._name == name ,  hlt1Lines() )
    if line : return line[0]
    print "ERROR:  no line with name ", name
    return None



class HltHadronLinesConf(LHCbConfigurableUser) :
    #--------------------------------
    #
    # (J.A. Hernando 17/12/08)
    # External selection to be provided
    #    1) AllL0Hadrons (L0 Candidates above a given threshold i.e 2000 MeV)
    #    2) RZVelo, PV2D, Velo tracks
    # Replace opts-file to set the opts for the fast fit by configurables

    # steering variables
    __slots__ = { 'L0Channel'               : "Hadron" # 'HadronNoGlob'
                , 'HadL0_EtCut'             : 3500.
                , 'HadL0_MinEtCut'          : 2500.
                , 'SingleHadron_PtCut'      : 5000.
                , 'HadMain_IPCut'           : 0.1
                , 'HadMain_PtCut'           : 2500.
                , 'HadMain_TrackFitChi2Cut' : 10.
                , 'HadVertex_DOCACut'       : 0.2
                , 'HadVertex_DZCut'         : 0.
                , 'HadVertex_MinIPCut'      : 0.1
                , 'HadVertex_MinPtCut'      : 1000.
                , 'HadVertex_PointingCut'   : 0.4
                , 'SoftDiHadron'            : False
                , 'HadL0_SoftEtCut'         : 2500. 
                , 'HadMain_SoftPtCut'       : 1500. 
                }

    def __apply_configuration__(self) : 
        importOptions('$HLTCONFROOT/options/HltRecoSequence.py')
        importOptions('$HLTCONFROOT/options/Hlt1HadFitTracks.opts')

        # depending variables
        #------------------------------------
        L0Channel   = self.getProp('L0Channel')
        HadL0_EtCut = self.getProp('HadL0_EtCut')
        if ( L0Channel == "HadronNoGlob"): HadL0_EtCut = 4000.


        # Single Hadron Alley
        #-----------------------------------
        Line ( 'SingleHadron' 
               , L0DU  = "L0_CHANNEL('"+L0Channel+"')"
               , algos = [
            convertL0Candidates('AllHadron')
            , Member("TF","L0Hadrons", FilterDescriptor = ["L0ET,>,"+str(HadL0_EtCut)])
            , GaudiSequencer('Hlt1RecoRZVeloSequence')
            , Member ( 'TF' ,'RZVelo'
                       , InputSelection = 'RZVelo'
                       , FilterDescriptor = ['Calo2DChi2_%TFL0Hadrons,<,4'])
            , Member ( 'TU', 'Velo',  RecoName = 'Velo')
            , Member ( 'TF', '1Velo', FilterDescriptor = [ 'IP_PV2D,||>,'+str(self.getProp('HadMain_IPCut'))])
            , Member ( 'TF', '2Velo', FilterDescriptor = [ 'Calo3DChi2_%TFL0Hadrons,<,4' ])
            , Member ( 'TM' , 'VeloCalo'
                       , InputSelection1 = '%TF2Velo'
                       , InputSelection2 = '%TFL0Hadrons'
                       , MatchName = 'VeloCalo' , MaxQuality = 4.
                       )
            , Member ( 'TU', 'GuidedForward'
                       , InputSelection  = '%TMVeloCalo'
                       , RecoName = 'GuidedForward'
                       , HistogramUpdatePeriod = 1
                       )
            , Member ( 'TF' , 'GuidedForward' , FilterDescriptor = ['PT,>,'+str(self.getProp('SingleHadron_PtCut'))])
            , Member ( 'TU' , 'FitTrack' ,      RecoName = "FitTrack")
            , Member ( 'TF' , '1FitTrack' ,     FilterDescriptor = ["FitIP_PV2D,||>,"+str(self.getProp('HadMain_IPCut'))])
            , Member ( 'TF' , '2FitTrack'
                       , OutputSelection = '%Decision'
                       , FilterDescriptor = ["FitChi2OverNdf,<,"+str(self.getProp('HadMain_TrackFitChi2Cut'))])
            ])
               

        Line ('DiHadron'
              , L0DU  = "L0_CHANNEL('"+L0Channel+"')"
              , algos =  [
            convertL0Candidates('AllHadron')
            , Member("TF","L0Hadrons", FilterDescriptor = ["L0ET,>,"+str(self.getProp('HadL0_EtCut'))])
            , GaudiSequencer('Hlt1RecoRZVeloSequence')
            , Member ( 'TF' ,'RZVelo'
                       , InputSelection = 'RZVelo'
                       , FilterDescriptor = ['Calo2DChi2_%TFL0Hadrons,<,4'])
            , Member ( 'TU', 'Velo',   RecoName = 'Velo')
            , Member ( 'TF', '1Velo',  FilterDescriptor = [ 'IP_PV2D,||>,'+str(self.getProp('HadMain_IPCut'))] )
            , Member ( 'TF', '2Velo',  FilterDescriptor = [ 'Calo3DChi2_%TFL0Hadrons,<,4' ])
            , Member ( 'TM' , 'VeloCalo'
                       , InputSelection1 = '%TF2Velo'
                       , InputSelection2 = '%TFL0Hadrons'
                       , MatchName = 'VeloCalo' , MaxQuality = 4.
                       )
            , Member ( 'TU', 'GuidedForward'
                       , InputSelection  = '%TMVeloCalo'
                       , RecoName = 'GuidedForward'
                       , HistogramUpdatePeriod = 1
                       )        
            , Member ( 'TF', 'GuidedForward' , FilterDescriptor = ['PT,>,'+str(self.getProp('HadMain_PtCut'))])
            , HltTrackUpgrade( 'Hlt1RecoVelo' )
            , Member ( 'TF', '1Velo2'
                       , InputSelection = HltTrackUpgrade('Hlt1RecoVelo')
                       , FilterDescriptor = [ 'IP_PV2D,||>,'+str(self.getProp('HadMain_IPCut'))])
            , Member ( 'TF', '2Velo2'
                       , InputSelection = HltTrackUpgrade('Hlt1RecoVelo')
                       , FilterDescriptor = ['MatchIDsFraction_%TFGuidedForward,<,0.9' ])
            , Member ( 'VM2', 'Velo'
                       , InputSelection1 = '%TFGuidedForward'
                       , InputSelection2 = '%TF2Velo2'
                       , FilterDescriptor = [ 'DOCA,<,'+str(self.getProp('HadVertex_DOCACut'))])
            , Member ( 'VF', 'Velo2',      FilterDescriptor = [ 'VertexDz_PV2D,>,'+str(self.getProp('HadVertex_DZCut'))])
            , Member ( 'VU', 'Forward',    RecoName = 'Forward')
            , Member ( 'VF', '1Forward',   FilterDescriptor = [ 'VertexMinPT,>,'+str(self.getProp('HadVertex_MinPtCut'))])
            , Member ( 'VF', '2Forward',   FilterDescriptor = [ 'VertexPointing_PV2D,<,'+str(self.getProp('HadVertex_PointingCut'))])
            , Member ( 'VU', 'FitTrack',   RecoName = 'FitTrack')
            , Member ( 'VF', '1FitTrack',  FilterDescriptor = [ 'FitVertexMinIP_PV2D,||>,'+str(self.getProp('HadVertex_MinIPCut'))])
            , Member ( 'VF', '2FitTrack'
                     , OutputSelection = "%Decision"
                     , FilterDescriptor = ['FitVertexMaxChi2OverNdf,<,'+str(self.getProp('HadMain_TrackFitChi2Cut')) ])
            ])



        if (self.getProp('SoftDiHadron')):
            _getLine("DiHadron").clone("SoftDiHadron"
                                       , L0DU = "L0_CHANNEL('"+",".join([L0Channel]+L0Channels())+"') "
                                       , TFL0Hadrons     = { "FilterDescriptor": ["L0ET,>,"+str(self.getProp('HadL0_SoftEtCut'))] }
                                       , TFGuidedForward = { "FilterDescriptor": ["PT,>,"+str(self.getProp('HadMain_SoftPtCut'))] }
                                       )
