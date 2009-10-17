from HltSettings.Physics_10000Vis_1000L0_40Hlt1_Apr09 import Physics_10000Vis_1000L0_40Hlt1_Apr09
class Physics_10000Vis_1000L0_40Hlt1_HadronicHlt2_Jul09( Physics_10000Vis_1000L0_40Hlt1_Apr09 ) :
    """
    Threshold settings for Hlt1 and 2 in nominal conditions (hadronic scenario)
    
    WARNING :: DO NOT EDIT WITHOUT PERMISSION OF THE AUTHORS
    
    @author P.Koppenburg, G. Raven
    @date 2009-08-06
    """
    
    def Thresholds(self) :
        """
        Returns a dictionary of cuts
        """
        
        from Hlt2Lines.Hlt2TopologicalLines  import Hlt2TopologicalLinesConf
    
        # pick up inherited thresholds, and update them...
        d = Physics_10000Vis_1000L0_40Hlt1_Apr09.Thresholds(self)
        # switch the Hlt2 pass through line back to normal
        from Hlt2Lines.Hlt2CommissioningLines import Hlt2CommissioningLinesConf
        d.update( { Hlt2CommissioningLinesConf : { 'Prescale' : { 'Hlt2PassThrough' : 0.0001 
                                                                , 'Hlt2Forward'     : 0. # first needs a check that /Hlt/Track/Long actually exists..
                                                                , 'Hlt2DebugEvent'  : 0.0001
                                                                } 
                                                 } } )
        # Phi thresholds
        from Phi_Nominal import PhiHigh_Nominal
        d.update( PhiHigh_Nominal().Thresholds() )
        # Remaining
        d.update(  { Hlt2TopologicalLinesConf  : { 'ComRobAllTrkPtLL' : 400.0
                                               , 'ComRobAllTrkPVIPLL' : 0.025
                                               , 'ComRobPairMinDocaUL' : 0.075
                                               , 'ComRobTrkMaxPtLL' : 1500.0
                                               , 'ComTFVtxPVDispChi2LL' : 169.0
                                               , 'RobustPointingUL' : 0.24
                                               , 'TFPointUL' : 0.28
                                               , 'CharmRobustPointUL' : 0.04
                                               , 'CharmTFPointUL' : 0.04
                                               , 'Prescale' : { 'Hlt2TopoTF4BodyCharmSignal' : 0.90
                                                              , 'Hlt2TopoTF3BodyCharmSignal' : 0.90
                                                              , 'Hlt2TopoTF2BodyCharmSignal' : 0.90 } 
                                               }
                     })
        #
        # presently muon line rates are controlled by ActiveHlt2Lines below
        #
        return d 
    
######################################################################################################
    #
    # Active Lines
    #
    def ActiveHlt2Lines(self) :
        """
        Returns a list of active lines
        """
        
        list =  [ 'Hlt2Topo2BodySA',
                  #'Hlt2Topo4BodySA',
                  'Hlt2Topo3BodySA',
                  #'Hlt2TopoTF4BodySA',
                  'Hlt2TopoTF3BodySA',
                  'Hlt2TopoTF2BodySA',
                  #'Hlt2TopoTF4BodyReq4Yes',
                  #'Hlt2TopoTF4BodyReq3Yes',
                  #'Hlt2TopoTF4BodyReq2Yes',
                  #'Hlt2TopoTF3BodyReq4Yes',
                  'Hlt2TopoTF3BodyReq3Yes',
                  'Hlt2TopoTF3BodyReq2Yes',
                  #'Hlt2TopoTF2BodyReq4Yes',
                  'Hlt2TopoTF2BodyReq3Yes',
                  'Hlt2TopoTF2BodyReq2Yes' ] 
##
## charm : leave out 4-body lines
##
        list.extend( [ 'Hlt2Topo3BodyCharmSA',
                       'Hlt2Topo2BodyCharmSA',
                       #'Hlt2Topo4BodyCharmSA',
                       'Hlt2TopoTF2BodyCharmSignal',
                       'Hlt2TopoTF3BodyCharmSignal',
                       #'Hlt2TopoTF4BodyCharmSignal',
                       'Hlt2TopoTF2BodyCharmWideMass',
                       'Hlt2TopoTF3BodyCharmWideMass'
                       #'Hlt2TopoTF4BodyCharmWideMass'
                       ] )
##
## (di)muons : Hadronic list
##
        list.extend( [ 'Hlt2IncMuTrackLow',
                       'Hlt2UnbiasedJPsiLow',
                       'Hlt2UnbiasedPsi2S',
                       'Hlt2UnbiasedBmm' ] )
##
## phi : whole list
##
        list.extend( [ 'Hlt2IncPhi',
                       'Hlt2IncPhiRobust',
                       'Hlt2IncPhiTF',
                       'Hlt2IncPhiSidebands' ])
##
## exclusive : whole list
##
        from Exclusive_Nominal import Exclusive_Nominal
        list.extend( Exclusive_Nominal().ActiveHlt2Lines() )
##
## commissioning : whole list
##
        from Commissioning_Nominal import Commissioning_Nominal
        list.extend( Commissioning_Nominal().ActiveHlt2Lines() )
        
        return list
    
    
    
