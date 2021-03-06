import time
import Online.PVSS as PVSS
import Online.Utils as Utils
import Online.SetupParams as Params
import Online.PVSSSystems as Systems

from Online.RunInfoClasses.General import General as General
log       = Utils.log
error     = Utils.error
std       = PVSS.gbl.std

# ===========================================================================
def getNodesFromSlots(slots):
  nodes = {}
  for i in slots:
    nodes[i[:i.find(':')]] = 1
  return nodes.keys()


# =============================================================================
class StorageInfo(General):
  """ @class Storage
  
      Connect to all storage related datapoints in the RunInfo object

      @author  M.Frank
      @version 1.0
  """
  # ===========================================================================
  def __init__(self,manager,name):
    """ Default constructor
        @param  manager       Reference to PVSS ControlManager
        @param  name          Detector/Partition name of the RunInfo datapoint

        @return reference to initialized object
    """
    General.__init__(self,manager,name,complete=1)
  # ===========================================================================
  def doStreaming(self):
    "Access to the use-flag"
    return self.storeFlag.data != 0
  # ===========================================================================
  def numLayer2Slots(self):
    return sum(self.strMult.data)
  # ===========================================================================
  def numLayer1Slots(self):
    return self.nSubFarm.data 
  # ===========================================================================
  def defineTasks(self,partition):
    """
    Define all tasks in the storage layer for a given partition.
    The result is storen in runInfo datapoints for further processing.
    
    """
    recv_slots = partition.recvSlices()
    streams = []
    #
    # Need to change order in this loop to better distribute the tasks
    # in the storage layers:
    #
    num_streams = 0
    for j in xrange(len(self.streams.data)):
      for i in xrange(self.strMult.data[j]):
        streams.append([self.streams.data[j],i])
        num_streams = num_streams + 1

    recvNodes = partition.recvNodesFromSlots()
    dimDns    = self.manager.hostName()
    streamers = []
    dataSources = []
    recvWriters = []
    recvReceivers = []
    recvInfrastructure = []
    opt = '/'+dimDns+'/'+partition.manager.name()+'/'+partition.name+'/'
    cl0 = '/Class0'+opt
    cl1 = '/Class1'+opt
    cl2 = '/Class2'+opt
    for i in xrange(len(recv_slots)):
      slot = recv_slots[i]
      node = slot[:slot.find(':')]
      sub_farm = 'SF%02d'%(i,)
      short_name = sub_farm+'_HLT'
      task = self.name+'_'+node+'_'+short_name
      recvReceivers.append(node+'/'+task+'/'+short_name+'/HLTRec'+cl1+'("'+sub_farm+'",)')
      dataSources.append(sub_farm+'/'+self.name+'_'+sub_farm+'_Sender/'+sub_farm+'_Sender/HLTSend'+cl2+'("'+node+','+task+'",)')

    for j in recvNodes:
      for i in streams:
        type = i[0]
        ident = str(i[1])
        short_name = type+'_'+ident
        sender = self.name+'_'+j+'_WRT'+short_name
        recvWriters.append(j+'/'+sender+'/WRT'+short_name+'/WRT'+type+cl1+'[("'+j+'-d1")]')
      for i in self.rcvInfra.data:
        recvInfrastructure.append(j+'/'+self.name+'_'+j+'_'+i+'/'+i+'/'+i+cl0+'("'+i+'",)')

    if self.storeFlag.data == 0:
      # If we do not store the data, the streaming layer does not have to be instrumented.
      recvWriters = PVSS.StringVector()
    empty = PVSS.StringVector()
    partition.setDataSources(dataSources)
    partition.setRecvInfrastructure(recvInfrastructure)
    partition.setRecvReceivers(recvReceivers)
    partition.setRecvSenders(recvWriters)
    partition.setStreamInfrastructure(empty)
    partition.setStreamReceivers(empty)
    partition.setStreamSenders(empty)
    if partition.saveTasks():
      tasks = partition.collectTasks(tasks={},with_data_sources=0)
      return tasks
    return None
  
# =============================================================================
class StorageInfoCreator:
  def __init__(self):
    pass
  def create(self,rundp_name,partition):
    items = rundp_name.split(':')
    # partition = items[1][:items[1].find('_')]
    mgr = Systems.controlsMgr(items[0])
    return StorageInfo(mgr,partition)
  def showPartition(self,partition,extended=None):
    import Online.Streaming.PartitionInfo as Info
    partition.show(method=Info.showStorage,extended=extended)
