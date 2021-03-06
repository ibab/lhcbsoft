"""
High level configuration tools for PhysConf
"""
__version__ = "$Id$"
__author__  = "Patrick Koppenburg <Patrick.Koppenburg@cern.ch>"

from LHCbKernel.Configuration import *
import GaudiKernel.ProcessJobOptions
from Configurables import (EventClockSvc,
                           RecEventTime,
                           TimeDecoderList,
                           OdinTimeDecoder,
                           DecodeRawEvent)

def configureEventTime() :
    """
    Configure EventClockSvc to get event time from RecHeader first
    and then from ODIN in case of failure.
    Returns EventClockSvs()
    Author: Marco Clemencic.
    """
    #turn off setting that's done in DecodeRawEvent first
    DecodeRawEvent().EvtClockBank=""
    ecs = EventClockSvc()
    ecs.addTool(TimeDecoderList, "EventTimeDecoder")
    tdl = ecs.EventTimeDecoder
    tdl.addTool(RecEventTime)
    tdl.addTool(OdinTimeDecoder)
    tdl.Decoders = [tdl.RecEventTime, tdl.OdinTimeDecoder]
    return ecs

class PhysConf(LHCbConfigurableUser) :

    __slots__ = {
           "DataType"          : ''      # Data type
        ,  "Simulation"        : False   # set to True to use SimCond
        ,  "InputType"         : 'DST'   # Data type
        ,  "AllowPIDRerunning" : True    # Allow, under the correct circumstances, PID reconstruction to be rerun (e.g. MuonID)
        ,  "EnableUnpack"      : None    # Enable unpacking of DST.
        ,  "CaloReProcessing"  : False   # Force CaloReco reprocessing
        ,  "UpdateCaloMCLinks" : True    # Update (True) or delete (False) the obsolete CaloHypo->MC Linker in case of calo reprocessing
        ,  "AllowPIDRecalib"   : True    # Allow recalibration of the PID information as required
        ,  "Detectors"         : ['Velo','PuVeto','Rich1','Rich2','TT','IT','OT','Spd','Prs','Ecal','Hcal','Muon','Magnet','Tr']
        ,  "OutputLevel"       : INFO    # The global output level
        }

    __used_configurables__ = (
        'DstConf',
        'CaloDstUnPackConf'   ,
        'CaloProcessor',
        'DecodeRawEvent'
        )

#
# configure reconstruction to be redone
#
    def initSequence(self):
        """
        Init Sequence. Called by master application.
        """
        # only one initialisiation so far
        from Configurables import GaudiSequencer
        init = GaudiSequencer("PhysInitSeq")
        self.configureReco(init)
        return init

#
# configure reconstruction to be redone
#
    def configureReco(self,init):
        """
        Configure Reconstruction to be redone
        """

        ## CaloReco & CaloPIDs on-demand
        clusters= [ 'Digits' , 'Clusters' ]
        from Configurables  import CaloProcessor
        caloProc = CaloProcessor( EnableOnDemand = True, OutputLevel = self.getProp("OutputLevel") )  
        caloProc.RecList = clusters
        # ---
        if self.getProp('DataType')=='Upgrade' :
            caloProc.NoSpdPrs=True

        ## General unpacking
        from Configurables import DstConf
        if self.isPropertySet('EnableUnpack') :
            DstConf().setProp('EnableUnpack',self.getProp('EnableUnpack'))

        ## unpack Calo Hypos ?
        from Configurables import CaloDstUnPackConf
        unpack = CaloDstUnPackConf()
        hypos= [ 'Photons',  'MergedPi0s', 'SplitPhotons' ,'Electrons' ]  # CaloHypos
        if self.isPropertySet('EnableUnpack') and "Reconstruction" in self.getProp('EnableUnpack') :
            unpack.setProp('Enable',True)
        else :
            caloProc.RecList +=  hypos  # enable caloHypos onDemand

        # Reprocess explicitely the full calo sequence in the init sequence ?
        inputtype = self.getProp('InputType').upper()
        if ( self.getProp("CaloReProcessing") and inputtype != 'MDST' ) :
            caloProc.RecList =  clusters + hypos
            caloSeq = caloProc.sequence() # apply the CaloProcessor configuration
            cSeq = GaudiSequencer( 'CaloReProcessing' )
            cSeq.Members += [ caloSeq ]
            init.Members += [cSeq]
            unpack.setProp('Enable',False)
            # update CaloHypo->MC Linker 
            if self.getProp('Simulation') :
                log.info("CaloReprocessing : obsolete CaloHypo2MC Links is updated")
                from Configurables import (TESCheck,EventNodeKiller,CaloHypoMCTruth)
                caloMCLinks = ["Link/Rec/Calo"]
                caloMCSeq=GaudiSequencer("cleanCaloMCLinks")
                checkCaloMCLinks=TESCheck("checkCaloMCLinks")
                checkCaloMCLinks.Inputs=caloMCLinks
                checkCaloMCLinks.Stop=False
                killCaloMCLinks = EventNodeKiller ( "killCaloMCLinks" )
                killCaloMCLinks.Nodes = caloMCLinks
                caloMCSeq.Members=[checkCaloMCLinks,killCaloMCLinks]
                init.Members += [ caloMCSeq ]
                update = self.getProp("UpdateCaloMCLinks") 
                if  update :
                    redoCaloMCLinks = CaloHypoMCTruth("recreteCaloMCLinks")
                    init.Members += [ redoCaloMCLinks ]
        else :
            caloProc.applyConf()
            if inputtype != 'MDST' :
                log.info("CaloReProcessing cannot be processed on reduced (m)DST data")

        # For backwards compatibility with MC09, we need the following to rerun
        # the Muon Reco on old data. To be removed AS SOON as this backwards compatibility
        # is no longer needed
        if ( self.getProp("DataType") == 'MC09' and inputtype != 'MDST' and
             self.getProp("AllowPIDRerunning") and inputtype != 'RDST' ) :

            from Configurables import DataObjectVersionFilter, MuonRec, TESCheck
            from MuonID import ConfiguredMuonIDs

            rerunPIDSeq = GaudiSequencer("ReRunMuonPID")
            init.Members += [rerunPIDSeq]

            # Check data version, to see if this is needed or not
            rerunPIDSeq.Members += [ DataObjectVersionFilter( "MuonPIDVersionCheck",
                                                              DataObjectLocation = "/Event/Rec/Muon/MuonPID",
                                                              MaxVersion = 0 ) ]
            # Check raw event is available
            rerunPIDSeq.Members += [ TESCheck( "TESCheckRawEvent",
                                               Inputs = ["DAQ/RawEvent"],
                                               Stop = False ) ]
            # Run Muon PID
            cm = ConfiguredMuonIDs.ConfiguredMuonIDs(data=self.getProp("DataType"))
            rerunPIDSeq.Members += [ MuonRec(), cm.getMuonIDSeq() ]

            # If muon PID has rerun, need to re make the Combined DLLS...
            from Configurables import ( ChargedProtoParticleAddMuonInfo,
                                        ChargedProtoCombineDLLsAlg )
            rerunPIDSeq.Members += [ ChargedProtoParticleAddMuonInfo("CProtoPAddNewMuon"),
                                     ChargedProtoCombineDLLsAlg("CProtoPCombDLLNewMuon") ]

        # Compatibility with pre-2011 data, where Rec/Summary and Trigger/RawEvent are missing
        import PhysConf.CheckMissingTESData as DataCheck
        DataCheck.checkForMissingData()

#
# Data on demand
#
    def dataOnDemand(self):
        """
        dataOnDemand service
        """
        dataOnDemand = DataOnDemandSvc()

        dataOnDemand.NodeMap['/Event/Rec']            = 'DataObject'
        dataOnDemand.NodeMap['/Event/Rec/Muon']       = 'DataObject'
        dataOnDemand.NodeMap['/Event/Rec/Rich']       = 'DataObject'
        dataOnDemand.NodeMap['/Event/Phys']           = 'DataObject'
        dataOnDemand.NodeMap['/Event/Relations/Phys'] = 'DataObject'

        # raw event
        DecodeRawEvent().DataOnDemand=True
        #require L0 just in case I need to do L0 decoding
        importOptions( "$L0TCK/L0DUConfig.opts" )
        
        # ANN PID recalibration
        if self.getProp("AllowPIDRecalib") :

            from Gaudi.Configuration import appendPostConfigAction

            datatype = self.getProp("DataType")

            # Default settings for calibration is off
            recoRegex = "v43r2(.*)"

            # If Run I data, perform Reco14 recalibration on the fly
            if ( datatype == '2009' or
                 datatype == '2010' or
                 datatype == '2011' or
                 datatype == '2012' or
                 datatype == '2013' or
                 datatype == '2015' ) :

                def _ANNPIDReCalib_() :

                    from Configurables import ( DstConf, DataOnDemandSvc,
                                                ChargedProtoANNPIDConf,
                                                ChargedProtoParticleMapper,
                                                ApplicationVersionFilter )

                    # Sequence to fill
                    annPIDSeq = GaudiSequencer("ANNPIDSeq")

                    # Only rerun on Reco14 (Run1) and Reco15 (2015) samples
                    recoRegex = "(v43r2|v47r9|v48r2)(.*)"
                    annPIDSeq.Members += [
                        ApplicationVersionFilter( name = "ANNPIDRecoVersionFilter",
                                                  HeaderLocation = "Rec/Header",
                                                  VersionRegex = recoRegex ) ]

                    # ANN PID Configurable
                    annPIDConf = ChargedProtoANNPIDConf("ReDoANNPID")

                    # Configure Configurable for recalibration of the DST charged protos
                    annPIDConf.DataType = self.getProp("DataType")
                    annPIDConf.RecoSequencer = annPIDSeq
                    annPIDConf.applyConf()

                    # Update the DoD sequence to run this at the end
                    chargedLoc = "/Event/Rec/ProtoP/Charged"
                    if chargedLoc in DataOnDemandSvc().AlgMap.keys() :
                        chargedSeq = DataOnDemandSvc().AlgMap[chargedLoc]
                        chargedSeq.Members += [annPIDSeq]

                    # Now for uDSTs. Update the DoD mappers to run a custom one
                    # for charged Protos, and includes the recalibration
                    cppmapper = ChargedProtoParticleMapper("UnpackChargedPPsMapper")
                    # Clone the settings from the DST configurable
                    cppmapper.ANNPIDTune = annPIDConf.tune(annPIDConf.DataType)
                    cppmapper.TrackTypes = annPIDConf.TrackTypes
                    cppmapper.PIDTypes   = annPIDConf.PIDTypes
                    # Again, only rerun the ANNPID on Reco14 data
                    cppmapper.VersionRegex = recoRegex
                    # Update the DoD mapper lists
                    DataOnDemandSvc().NodeMappingTools = [cppmapper] + DataOnDemandSvc().NodeMappingTools
                    DataOnDemandSvc().AlgMappingTools  = [cppmapper] + DataOnDemandSvc().AlgMappingTools

                # Append post config action
                appendPostConfigAction( _ANNPIDReCalib_ )

#
# LoKi
#
    def loki(self) :
        """
        Define loki service
        """
        from Configurables import LoKiSvc
        lokiService = LoKiSvc()
        ApplicationMgr().ExtSvc +=  [ lokiService ]

#
# Main configuration
#
    def __apply_configuration__(self):
        """
        Apply configuration for Phys
        """
        log.info("Applying Phys configuration")
        self.dataOnDemand()
        self.loki()
        if 'DST' in  self.getProp('InputType').upper() :
            log.info('PhysConf: Configuring EventClockSvc to use RecHeader')
            configureEventTime()
