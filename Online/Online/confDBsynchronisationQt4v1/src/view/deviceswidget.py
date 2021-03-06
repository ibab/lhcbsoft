from PyQt4 import QtGui, QtCore

class DevicesWidget(QtGui.QWidget):
    def __init__(self, devicesInfo, parent = None, parentController = None):
        QtGui.QWidget.__init__(self, parent)
        self.parentController = parentController
        self.gridLayout = QtGui.QGridLayout()
        self.setGeometry(10, 60, 300, 200)
        self.devicesInfo = devicesInfo
        self.createLabels()
        self.createLineEdits()
        self.createButtons()
        self.setLayout(self.gridLayout)
        self.show()
    def setDevicesInfo(self, devicesInfo):
        self.devicesInfo = devicesInfo
        self.updateLineEdits()
        self.update()
    def createLabels(self):
        self.nwodLabel = QtGui.QLabel(str("New devices without DHCP data"), self)
        self.nwodLabel.setFixedSize(200, 20)
        #self.nwodLabel.setMaximumSize(200, 20)
        #self.nwodLabel.setMinimumSize(200, 20)
        self.nwdLabel = QtGui.QLabel(str("New devices with DHCP data"), self)
        self.nwdLabel.setMaximumSize(200, 20)
        self.naLabel = QtGui.QLabel(str("New devices at all"), self)
        self.naLabel.setMaximumSize(200, 20)
        self.cLabel = QtGui.QLabel(str("Changed devices"), self)
        self.cLabel.setMaximumSize(200, 20)
        self.utdLabel = QtGui.QLabel(str("Devices up-to-date"), self)
        self.utdLabel.setMaximumSize(200, 20)
        self.aLabel = QtGui.QLabel(str("Devices at all"), self)
        self.aLabel.setMaximumSize(200, 20)
        self.gridLayout.addWidget(self.nwodLabel, 0, 0)
        self.gridLayout.addWidget(self.nwdLabel, 1, 0)
        self.gridLayout.addWidget(self.naLabel, 2, 0)
        self.gridLayout.addWidget(self.cLabel, 3, 0)
        self.gridLayout.addWidget(self.utdLabel, 4, 0)
        self.gridLayout.addWidget(self.aLabel, 5, 0)
    def createLineEdits(self):
        self.nwodLineEdit = QtGui.QLineEdit(str(self.devicesInfo.NewDevicesWithoutDHCPData), self)
        self.nwodLineEdit.setReadOnly(True)
        self.nwodLineEdit.setMaximumSize(50, 20)
        self.nwdLineEdit = QtGui.QLineEdit(str(self.devicesInfo.NewDevicesWithDHCPData), self)
        self.nwdLineEdit.setReadOnly(True)
        self.nwdLineEdit.setMaximumSize(50, 20)
        self.naLineEdit = QtGui.QLineEdit(str(self.devicesInfo.NewDevicesAtAll), self)
        self.naLineEdit.setReadOnly(True)
        self.naLineEdit.setMaximumSize(50, 20)
        self.cLineEdit = QtGui.QLineEdit(str(self.devicesInfo.ChangedDevices), self)
        self.cLineEdit.setReadOnly(True)
        self.cLineEdit.setMaximumSize(50, 20)
        self.utdLineEdit = QtGui.QLineEdit(str(self.devicesInfo.DevicesUpToDate), self)
        self.utdLineEdit.setReadOnly(True)
        self.utdLineEdit.setMaximumSize(50, 20)
        self.aLineEdit = QtGui.QLineEdit(str(self.devicesInfo.DevicesAtAll), self)
        self.aLineEdit.setReadOnly(True)
        self.aLineEdit.setMaximumSize(50, 20)
        self.gridLayout.addWidget(self.nwodLineEdit, 0, 1)
        self.gridLayout.addWidget(self.nwdLineEdit, 1, 1)
        self.gridLayout.addWidget(self.naLineEdit, 2, 1)
        self.gridLayout.addWidget(self.cLineEdit, 3, 1)
        self.gridLayout.addWidget(self.utdLineEdit, 4, 1)
        self.gridLayout.addWidget(self.aLineEdit, 5, 1)
    def createButtons(self):
        self.insertDevicesButton = QtGui.QPushButton(self)
        self.insertDevicesButton.setText("Insert")
        self.insertDevicesButton.setToolTip("Select and insert new devices \nto be inserted into ConfDB")
        self.insertDevicesButton.setFixedSize(50, 20)
        self.gridLayout.addWidget(self.insertDevicesButton, 2, 2)
        self.connect(self.insertDevicesButton, QtCore.SIGNAL("clicked()"), self.parentController.onInsert)
        ############################################################################
        self.updateDevicesButton = QtGui.QPushButton(self)
        self.updateDevicesButton.setText("Update")
        self.updateDevicesButton.setToolTip("Upate existing devices in ConfDB")
        self.updateDevicesButton.setFixedSize(50, 20)
        self.gridLayout.addWidget(self.updateDevicesButton, 3, 2)
        self.connect(self.updateDevicesButton, QtCore.SIGNAL("clicked()"), self.parentController.onUpdate)
        ############################################################################
        self.deleteDevicesButton = QtGui.QPushButton(self)
        self.deleteDevicesButton.setText("Delete")
        self.deleteDevicesButton.setToolTip("Delete all devices from ConfDB")
        self.deleteDevicesButton.setFixedSize(50, 20)
        self.gridLayout.addWidget(self.deleteDevicesButton, 4, 2)
        self.connect(self.deleteDevicesButton, QtCore.SIGNAL("clicked()"), self.parentController.onDelete)
    def updateLineEdits(self):
        self.nwodLineEdit.setText(str(self.devicesInfo.NewDevicesWithoutDHCPData))
        self.nwdLineEdit.setText(str(self.devicesInfo.NewDevicesWithDHCPData))
        self.naLineEdit.setText(str(self.devicesInfo.NewDevicesAtAll))
        self.cLineEdit.setText(str(self.devicesInfo.ChangedDevices))
        self.utdLineEdit.setText(str(self.devicesInfo.DevicesUpToDate))
        self.aLineEdit.setText(str(self.devicesInfo.DevicesAtAll))