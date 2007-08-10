import Online.PVSS as PVSS
import Online.Utils as Utils
log         = Utils.log
error       = Utils.error
std         = PVSS.gbl.std
DataPoint   = PVSS.DataPoint
printSlots  = Utils.printSlots

# =============================================================================
class StreamingDescriptor(object):
  """ @class StreamingDescriptor
      This class describes the shared streaming control structure.
      
      @author  M.Frank
      @version 1.0
  """
  # ===========================================================================
  def __init__(self, manager, name):
    """ Default constructor
        @param  manager       Reference to PVSS ControlManager
        @param  name          Name of the Streaming control datapoint

        @return reference to initialized object
    """
    self.manager    = manager
    self.name       = name
    self.inUse      = self.get('InUse')
    self.active     = self.get('ActivePartitions')
    self.recvNodes  = self.get('RecvNodes')
    self.recvSlices = self.get('RecvSlices')
    self.strmNodes  = self.get('StreamNodes')
    self.strmSlices = self.get('StreamSlices')
    self.reader     = self.manager.devReader()
    self.writer     = self.manager.devWriter()

  # ===========================================================================
  def get(self,name):
    """ Return datapoint in Streaming control structure
        @param internal datapoint name
        
        @return Initialized PVSS datapoint
    """
    return DataPoint(self.manager,DataPoint.original(self.name+'.'+name))
    
  # ===========================================================================
  def partitionName(self,which):
    return self.name+'_Partition_%02X'%(which,)

  # ===========================================================================
  def load(self):
    self.reader.add(self.inUse)
    self.reader.add(self.active)
    self.reader.add(self.recvNodes)
    self.reader.add(self.recvSlices)
    self.reader.add(self.strmNodes)
    self.reader.add(self.strmSlices)
    if self.reader.execute():
      return self
    return None

  # ===========================================================================
  def show(self, extended=None):
    rnodes = self.recvNodes.data
    snodes = self.strmNodes.data
    rslice = self.recvSlices.data
    sslice = self.strmSlices.data
    active = self.active.data
    inuse  = self.inUse.data
    log('Active partitions:'+str(len(active)))
    if len(active) > 0:
      printSlots(active,'          ',1)
    printSlots(rnodes,'Receiving nodes ('+str(len(rslice))+' slices left):',99,5)
    printSlots(rslice,'          ',5)
    printSlots(snodes,'Streaming nodes ('+str(len(sslice))+' slices left):',99,5)
    printSlots(sslice,'          ',5)

  # ===========================================================================
  def getPartition(self, partition):
    "Select partition by allocation name."
    actor = PVSS.DpVectorActor(self.manager)
    typ   = self.manager.typeMgr().type('StreamPartition')
    actor.lookup(DataPoint.original(self.name+'_Partition*.Name'),typ)
    for i in actor.container: self.reader.add(i)
    if self.load():
      for i in actor.container:
        nam = i.name()
        nam = nam[nam.find(':')+1:nam.find('.')]
        id  = int(nam[len(nam)-2:],16)
        if i.data == partition:
          return (DataPoint(self.manager,i.name()), nam, id)
      return None
    return None
