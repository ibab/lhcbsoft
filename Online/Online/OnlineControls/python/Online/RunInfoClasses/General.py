import time
import Online.PVSS as PVSS
import Online.Utils as Utils
log       = Utils.log
error     = Utils.error
std       = PVSS.gbl.std
DataPoint = PVSS.DataPoint
debug     = None


## "L0DU VELO PUS VELOA VELOC ST TT IT OT OTA OTC RICH RICH1 RICH2 CALO PS ECAL HCAL MUON MUONA MUONC TDET";
## "0    -1   1   2     3     -1 5  6  -1 7   8   -1   4     9     -1   10 11   12   -1   13    14    15";

RO_BIT_L0DU   = 0
RO_BIT_PUS    = 1
RO_BIT_VELOA  = 2
RO_BIT_VELOC  = 3
RO_BIT_TT     = 5
RO_BIT_IT     = 6
RO_BIT_OTA    = 7
RO_BIT_OTC    = 8
RO_BIT_RICH1  = 4
RO_BIT_RICH2  = 9
RO_BIT_PS     = 10
RO_BIT_ECAL   = 11
RO_BIT_HCAL   = 12
RO_BIT_MUONA  = 13
RO_BIT_MUONC  = 14
RO_BIT_TDET   = 15

_detectors = {
  'L0DU':  1<<RO_BIT_L0DU,
  'VELO': (1<<RO_BIT_PUS)+(1<<RO_BIT_VELOA)+(1<<RO_BIT_VELOC),
  'PUS':   1<<RO_BIT_PUS,
  'VELOA': 1<<RO_BIT_VELOA,
  'VELOC': 1<<RO_BIT_VELOC,
  'ST':   (1<<RO_BIT_TT)+(1<<RO_BIT_IT),
  'TT':    1<<RO_BIT_TT,
  'IT':    1<<RO_BIT_IT,
  'OT':   (1<<RO_BIT_OTA)+(1<<RO_BIT_OTC),
  'OTA':   1<<RO_BIT_OTA,
  'OTC':   1<<RO_BIT_OTC,
  'RICH': (1<<RO_BIT_RICH1)+(1<<RO_BIT_RICH2),
  'RICH1': 1<<RO_BIT_RICH1,
  'RICH2': 1<<RO_BIT_RICH2,
  'CALO': (1<<RO_BIT_PS)+(1<<RO_BIT_ECAL)+(1<<RO_BIT_HCAL),
  'PS':    1<<RO_BIT_PS,
  'ECAL':  1<<RO_BIT_ECAL,
  'HCAL':  1<<RO_BIT_HCAL,
  'MUON': (1<<RO_BIT_MUONA)+(1<<RO_BIT_MUONC),
  'MUONA': 1<<RO_BIT_MUONA,
  'MUONC': 1<<RO_BIT_MUONC,
  'TDET':  1<<RO_BIT_TDET
  }

# =============================================================================
def _getDetectorsInReadout(partid):
  """Retrieve all detectors participating in the readout corresponding
     to a given active partition identifier.
  """
  dets = []
  for name,id in _detectors.items():
    if ( (partid&id)==id ):
      #print 'Detector '+name+' is in RO',id,partid,(partid&id)
      dets.append((name,id,))
    #else:
      #print 'Detector '+name+' is NOT in RO',id,partid,(partid&id)
  return dets

# =============================================================================
def _isDetectorInReadout(partid, name):
  """Retrieve information if a given subdetector is participating in the readout
     corresponding to a given active partition identifier.
  """
  for n,id in _detectors.items():
    if ( (partid&id)==id and name==n):
      return (n,id,)
    elif ( (partid&id) != 0 ):
      return (name,id,)
  return None
  
# =============================================================================
class General:
  """ @class General
  
      Connect to all general information related datapoints in the RunInfo object

      @author  M.Frank
      @version 1.0
  """
  # ===========================================================================
  def __init__(self, manager, name):
    """ Default constructor
        @param  manager       Reference to PVSS ControlManager
        @param  name          Detector/Partition name of the RunInfo datapoint

        @return reference to initialized object
    """
    self.manager     = manager
    self.name        = name
    self.devMgr      = self.manager.deviceMgr()
    self.reader      = self.manager.devReader()
    self.runTyp      = self.dp('general.runType')
    self.partID      = self.dp('general.activePartId')
    self.partition   = self.dp('general.partName')

    self.outputLvl   = None
    self.acceptFrac  = None
    self.tae         = None
    dpn = self.manager.name()+':'+self.name+'_RunInfo.general.outputLevel'
    if self.devMgr.exists(dpn):
      self.outputLvl = self.dp('general.outputLevel')
    dpn = self.manager.name()+':'+self.name+'_RunInfo.general.acceptRate'
    if self.devMgr.exists(dpn):
      self.acceptFrac = self.dp('general.acceptRate')
    dpn = self.manager.name()+':'+self.name+'_RunInfo.general.TAE'
    if self.devMgr.exists(dpn):
      self.tae = self.dp('general.TAE')

    self.nSubFarm    = self.dp('HLTFarm.nSubFarms')
    self.storageDir  = self.dp('Storage.dataDirectory')
    self.storeFlag   = self.dp('Storage.storeFlag')
    self.rcvInfra    = self.dp('Storage.recvInfrastructure')
    self.strInfra    = self.dp('Storage.streamInfrastructure')
    self.streams     = self.dp('Storage.streamTypes')
    self.strMult     = self.dp('Storage.streamMultiplicity')

    self.monFlag     = self.dp('MonFarm.monFlag')
    self.monTypes    = self.dp('MonFarm.monTypes')
    self.monStreams  = self.dp('MonFarm.monStreams')
    self.monMult     = self.dp('MonFarm.monMultiplicity')
    self.monInfra    = self.dp('MonFarm.monInfrastructure')
    self.relayInfra  = self.dp('MonFarm.relayInfrastructure')

    self.tell1Boards = self.dp('SubDetectors.tell1List')

    # General run info 
    self.reader.add(self.runTyp)
    self.reader.add(self.partID)
    self.reader.add(self.partition)
    if self.outputLvl is not None:
      self.reader.add(self.outputLvl)
    if self.acceptFrac is not None:
      self.reader.add(self.acceptFrac)
    if self.tae is not None:
      self.reader.add(self.tae)

    # HLT information
    self.reader.add(self.nSubFarm)

    # Storage information
    self.reader.add(self.storageDir)
    self.reader.add(self.storeFlag)
    self.reader.add(self.streams)
    self.reader.add(self.strMult)
    self.reader.add(self.rcvInfra)
    self.reader.add(self.strInfra)
    # Monitoring information
    self.reader.add(self.monFlag)
    self.reader.add(self.monTypes)
    self.reader.add(self.monMult)
    self.reader.add(self.monStreams)
    self.reader.add(self.monInfra)
    self.reader.add(self.relayInfra)
    # Subdetector information
    self.reader.add(self.tell1Boards)    

  # ===========================================================================
  def dp(self,name):
    "Access internal datapoint of the datapoint structure."
    return DataPoint(self.manager,DataPoint.original(self.name+'_RunInfo.'+name))

  # ===========================================================================
  def _setDataItem(self, datapoint, save=None):
    datapoint.data = partID
    if save: return self._save(datapoint)
    return self

  # ===========================================================================
  def _dataItem(self, datapoint):
    if datapoint.data is None:
      self.load()
    return datapoint.data

  # ===========================================================================
  def load(self):
    "Load the datapoints assigned to the reader."
    if self.reader.execute(1,1):
      return self
    error('Failed to load run information for:'+self.name)
    return None
        
  # ===========================================================================
  def _save(self,datapoint,deviceIO=None):
    """Save single datapoint. If the deviceIO structure is present only
       add the datapoint to the pending transaction
    """
    if deviceIO is not None:
      deviceIO.add(datapoint)
      return self
    wr = self.manager.devWriter()
    wr.add(self.partID)
    if wr.execute(0,1) is not None:
      return self
    return None

  # ===========================================================================
  def showGeneral(self):
    "Show all information from the RunInfo structure."
    log('Run type:                    '+str(self.runTyp.data))
    log('Partition ID                 '+str(self.partID.data))
    if self.storeFlag>0:
      log('Data storage is enabled')
      for i in xrange(len(self.streams.data)):
        fmt = 'Data stream:                 %-20s with multiplicity:%d'
        log(fmt%(self.streams.data[i],self.strMult.data[i]))
    s = 'Receive infrastructure:      '
    for i in xrange(len(self.recvInfra.data)):
      s = s + self.recvInfra.data[i]
    log(s)
    if self.storeFlag>0:
      s = 'Streaming infrastructure:    '
      for i in xrange(len(self.recvInfra.data)):
        s = s + self.recvInfra.data[i]
      log(s)

  # ===========================================================================
  def show(self):
    "Show all information from the Storage structure."
    if self.load():
      self.showStreams()
      self.showMonitors()
      self.showTell1Boards()
      self.showGeneral()
      return
    error('Failed to load RunInfo for partition:'+self.name)

  # ===========================================================================
  def showTell1Boards(self):
    "Show TELL1 boards related information of the RunInfo datapoint."
    t1 = self.tell1Boards
    log('Number of TELL1 boards:%d'%len(t1))
    s = ''
    for i in xrange(len(t1)):
      s = s + ' %-12s'%(t1[i],)
      if i>0 and ((i%5) == 0 or i==(len(t1)-1)):
        log('->  '+s)
        s = ''

  # ===========================================================================
  def showStreams(self):
    "Show output stream related information of the RunInfo datapoint."
    streams = self.streams.data
    multiplicity = self.strMult.data
    log('Output to %d logical streams'%len(self.streams.data))
    for i in xrange(len(streams)):
      log(' -> Data stream:%-24s with  mutiplicity:%d'%(streams[i],multiplicity[i]))

  # ===========================================================================
  def showMonitors(self):
    "Show monitor stream related information of the RunInfo datapoint."
    streams = self.monTypes.data
    multiplicity = self.monMult.data
    log('Output to %d logical streams'%len(streams))
    for i in xrange(len(streams)):
      log(' -> Data stream:%-24s with  mutiplicity:%d'%(streams[i],multiplicity[i]))
      
  # ===========================================================================
  def isDetectorUsed(self,detector):
    "Check if detector identified by its name is in use by the selected partition."
    return 1

  # ===========================================================================
  def detectorsInReadout(self):
    """Retrieve all detectors participating in the readout corresponding
       to a given active partition identifier.
    """
    if self.partID.data is None: self.load()
    return _getDetectorsInReadout(self.partID.data)

  # ===========================================================================
  def isDetectorInReadout(self, name):
    """Retrieve information if a given subdetector is participating in the readout
       corresponding to a given active partition identifier.
    """
    if self.partID.data is None: self.load()
    return _isDetectorInReadout(self.partID.data,name)
  
  # ===========================================================================
  def setPartitionID(self,partID,save=0):
    "Update active partition identifier."
    return self._setDataItem(self.partID,partID,save)

  # ===========================================================================
  def partitionID(self):
    "Access to the active partition identifier."
    return self._dataItem(self.partID)

  # ===========================================================================
  def setRunType(self,runType,save=0):
    "Update the run type identifier."
    return self._setDataItem(self.runTyp,runType,save)

  # ===========================================================================
  def runType(self):
    "Access to the run type identifier."
    return self._dataItem(self.runTyp)

  # ===========================================================================
  def outputLevel(self):
    "Access output level from RunInfo."
    if self.outputLvl:
      if self.outputLvl.data is None: self.load()
      return int(self.outputLvl.data)
    return 4

  # ===========================================================================
  def acceptRate(self):
    "Access Accept rate in percent from RunInfo."
    if self.acceptFrac:
      if self.acceptFrac.data is None: self.load()
      return int(self.acceptFrac.data)
    return 100

  # ===========================================================================
  def TAE(self):
    "Access TAE flag from run info."
    if self.tae:
      if self.tae.data is None: self.load()
      return int(self.tae.data)
    return 0

# =============================================================================
def create(rundp_name):
    items = rundp_name.split(':')
    mgr = Systems.controlsMgr(items[0])
    partition = items[1][:items[1].find('_')]
    return General(mgr,partition)
