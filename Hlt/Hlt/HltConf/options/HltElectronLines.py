#!/usr/bin/env gaudirun.py
# =============================================================================
# $Id: HltElectronLines.py,v 1.7 2008-12-08 12:30:03 graven Exp $
# =============================================================================
## @file
#  Configuration of Photon Lines
#  @author Gerhard Raven Gerhard.Raven@nikhef.nl
#  @date 2008-09-24
# =============================================================================
'''
 script to configure Electron trigger lines
'''
# =============================================================================
__author__  = 'Gerhard Raven Gerhard.Raven@nikhef.nl'
__version__ = 'CVS Tag $Name: not supported by cvs2svn $, $Revision: 1.7 $'
# =============================================================================

from Gaudi.Configuration import * 

from Configurables import GaudiSequencer
from Configurables import HltL0CaloPrepare
from Configurables import HltTrackUpgrade
from Configurables import EcalSeedForVeloMatch

from HltConf.HltLine import Hlt1Line   as Line
from HltConf.HltLine import Hlt1Member as Member
from HltConf.HltLine import bindMembers
from HltConf.HltLine import Hlt1Tool   as Tool
from HltConf.HltLine import hlt1Lines, addHlt1Prop, rmHlt1Prop 

HltL0CaloPrepare('L0ElectronDecision').addTool(  EcalSeedForVeloMatch('EcalSeedForVeloMatch') )

prepElectron =  bindMembers( 'PrepElectron', 
  [ HltL0CaloPrepare('L0ElectronDecision' , CaloType = 'Electron', MinEt = 2600.0)
  , GaudiSequencer('Hlt1RecoRZVeloSequence')
  , Member ('TF', 'RZVelo'  # // select RZVelo tracks with an IP and matched to L0Calo
           , InputSelection     = 'RZVelo'
           , FilterDescriptor = [ 'Ecal2DChi2_L0ElectronDecision,<,4.' ]
           , HistogramUpdatePeriod = 0
           , HistoDescriptor = {  'Ecal2DChi2'     : ('Ecal2DChi2',0.,20.,100), 'Ecal2DChi2Best' : ('Ecal2DChi2Best',0.,20.,100) }
           )
  , Member ( 'TU', 'Velo', RecoName = 'Velo') # // Velo reconstruction of selected RZVelo
  , Member ( 'TF', 'Velo' # // Select Velo tracks with an IP and matched to L0Calo
           , HistogramUpdatePeriod = 0
           , FilterDescriptor = [ 'IP_PV2D,||[],0.1,3.', 'Ecal3DChi2_L0ElectronDecision,<,4.' ]
           , HistoDescriptor = { 'IP' :           ('IP',-1.,3.,400), 'IPBest' :       ('IPBest',-1.,3.,400), 'Ecal3DChi2':    ('Ecal3DChi2',0.,20.,100), 'Ecal3DChi2Best':('Ecal3DChi2Best',0.,20.,100) }
           )
  , Member ( 'TU', 'Forward' # // Make forward the selected velo tracks
           , RecoName = 'Forward'
           )
  , Member ( 'TF','Forward' # // Select forward track with a given Pt
           , FilterDescriptor = ['PT,>,1300.']
           , HistogramUpdatePeriod = 0
           , HistoDescriptor = { 'PT'     : ('PT',0.,6000.,100), 'PTBest' : ('PTBest',0.,6000.,100) }
           )
  ])


Line ('SingleElectron'
     ,  L0DU = "L0_CHANNEL('Electron')"
     , algos = 
     [ prepElectron
     , Member( 'TF','Decision'
             , OutputSelection = '%Decision'
             , InputSelection  = prepElectron.outputSelection()
             , FilterDescriptor = ['PT,>,3000.']
             , HistogramUpdatePeriod = 0
             , HistoDescriptor = { 'PT' : ('PT',0.,8000.,100), 'PTBest' : ('PTBest',0.,8000.,100)}
             )
     ])


Line( 'ElectronTrack' 
    ,  L0DU = "L0_CHANNEL('Electron')"
    , algos = 
    [ prepElectron
    , HltTrackUpgrade( 'Hlt1RecoVelo' )
    , Member ( 'TF', 'CompanionVelo' # select velo tracks as companions 
             , InputSelection    = 'Velo'
             , HistogramUpdatePeriod = 0
             , FilterDescriptor = [ 'IP_PV2D,||[],0.1,3.', 'DOCA_'+prepElectron.outputSelection()+',<,0.2' ]
             , HistoDescriptor = { 'IP': ( 'IP',-1.,.3,600), 'IPBest':   ( 'IPBest',-1.,3.,600), 'DOCA':     ( 'DOCA',0.,1.,100), 'DOCABest': ( 'DOCABest',0.,1.,100) }
             )
    , Member ( 'TU', 'CompanionForward' # // Do forward the companion tracks
             , RecoName = 'Forward'
             )
    , Member ( 'TF', 'CompanionForward' # // // Select companion tracks if they have a fiven Pt
             , FilterDescriptor = ['PT,>,1000.']
             , HistogramUpdatePeriod = 0
             , HistoDescriptor = { 'PT'     : ('PT',0.,8000.,100), 'PTBest' : ('PTBest',0.,8000.,100)}
             )
    , Member ( 'VM2', 'DiElectron' # // // Make vertices with the forward companion tracks 
             , InputSelection1   = prepElectron.outputSelection()
             , InputSelection2   = '%TFCompanionForward'
             , FilterDescriptor = ['DOCA,<,0.2']
             , HistogramUpdatePeriod = 0
             , HistoDescriptor = { 'DOCA' :     ('DOCA',0.,1.,100), 'DOCABest' : ('DOCABest',0.,0.5,100) }
             )
    , Member ( 'VF', 'Decision' # // select vertices if Pt, poiting, and distance
             , OutputSelection = '%Decision'
             , FilterDescriptor = [ 'VertexPointing_PV2D,<,0.5', 'VertexDz_PV2D,>,0.' ]
             , HistogramUpdatePeriod = 0
             , HistoDescriptor = { 'VertexPointing'    : ('VertexPointing',0.,1.,100), 'VertexPointingBest': ('VertexPointingBest',0.,1.,100), 'VertexDz'          : ('VertexDz',-10.,50.,100), 'VertexDzBest'      : ('VertexDzBest',-10.,50.,100) }
             )
    ])
