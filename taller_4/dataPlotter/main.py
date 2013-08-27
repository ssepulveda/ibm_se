#!/usr/bin/env python2

import sys

from gui import *
from serialmonitorthread import SerialMonitorThread
 
class MainWindow(QtGui.QMainWindow):
    def __init__(self):
        QtGui.QMainWindow.__init__(self)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        # populate combobox
        self.ui.cboxPorts.addItem("/dev/ttyACM0")
        self.ui.cboxPorts.addItem("/dev/ttyUSB0")
        
        self.ui.cboxSpeed.addItems(["4800","9600","57600","115200"])
        
        # configre gui
        self.ui.btnClosePort.setEnabled(False)
        
        # setup timer
        # TODO: try lightweight timer QBasicTimer
        # http://qt-project.org/doc/qt-4.8/widgets-wiggly.html
        self.timer=QtCore.QTimer(self)
        
        # register Qt signals
        QtCore.QObject.connect(self.ui.btnOpenPort, QtCore.SIGNAL('clicked()'), self.openPort)
        QtCore.QObject.connect(self.ui.btnClosePort, QtCore.SIGNAL('clicked()'), self.closePort)
        QtCore.QObject.connect(self.timer, QtCore.SIGNAL('timeout()'), self.updatePlot)
        
        # plot setup
        #self.ui.plt1.setYRange(-2,2)
        #self.ui.plt1.setLabel('left','Acceleration','g')
        self.ui.plt1.showGrid(True,True,.3)
        
        # variables
        self.window=500
        
        self.pulseIntesity=[0.0]*self.window
        self.oxygenDissolved=[0.0]*self.window
        self.pulseSound=[0.0]*self.window
        self.pulseRate=[0.0]*self.window
        self.oxygenSaturation=[0.0]*self.window
        self.searchTimeTooLong='E'
        self.oxygenSaturationDecrease='E'
        self.probeError='E'
        self.searchPulse='E'
        
        self.counter=0
 
    def openPort(self):
        self.smThread=SerialMonitorThread()
        self.smThread.openPort(str(self.ui.cboxPorts.currentText()),57600)
        
        # register signals for thread
        self.connect(self.smThread,QtCore.SIGNAL('newData()'),self.updateData)
        # start thread
        self.smThread.start()
        # disable gui elements
        self.ui.cboxPorts.setEnabled(False)
        self.ui.btnOpenPort.setEnabled(False)
        self.ui.btnClosePort.setEnabled(True)
        # init timer (gui delay)
        self.timer.start(50)

    def closePort(self):
        # stop threads
        self.smThread.close()
        self.timer.stop()
        # enable gui elements
        self.ui.cboxPorts.setEnabled(True)
        self.ui.btnOpenPort.setEnabled(True)
        self.ui.btnClosePort.setEnabled(False)

    def updateData(self):
        data=self.smThread.getData()
        print data
        
        self.pulseIntesity[self.counter]=float(data[0])
        self.oxygenDissolved[self.counter]=float(data[1])
        self.pulseSound[self.counter]=float(data[2])
        self.pulseRate[self.counter]=float(data[3])
        self.oxygenSaturation[self.counter]=float(data[4])
        self.searchTimeTooLong=data[5]
        self.oxygenSaturationDecrease=data[6]
        self.probeError=data[7]
        self.searchPulse=data[8]
        
        # windowed plot
        self.counter+=1
        if self.counter==self.window-1:
            self.counter=0
        
    def updatePlot(self):
        # clear last plots
        self.ui.plt1.clear()
        
        # plot new data
        self.ui.plt1.plot(self.pulseIntesity,pen='r')
                
        # add line in current plotting position
        self.ui.plt1.addLine(x=self.counter)

        # update status variables
        if self.probeError=='K':
            self.ui.lbProbeError.setEnabled(False)
        else:
            self.ui.lbProbeError.setEnabled(True)
        
if __name__ == "__main__":
    app=QtGui.QApplication(sys.argv)
    myapp=MainWindow()
    myapp.show()
    sys.exit(app.exec_())