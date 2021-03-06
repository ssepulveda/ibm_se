# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'gui.ui'
#
# Created: Tue Aug 27 11:00:50 2013
#      by: PyQt4 UI code generator 4.10.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        MainWindow.resize(800, 600)
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.gridLayout_2 = QtGui.QGridLayout(self.centralwidget)
        self.gridLayout_2.setObjectName(_fromUtf8("gridLayout_2"))
        self.gridLayout = QtGui.QGridLayout()
        self.gridLayout.setObjectName(_fromUtf8("gridLayout"))
        self.btnOpenPort = QtGui.QPushButton(self.centralwidget)
        self.btnOpenPort.setObjectName(_fromUtf8("btnOpenPort"))
        self.gridLayout.addWidget(self.btnOpenPort, 0, 2, 1, 1)
        self.btnClosePort = QtGui.QPushButton(self.centralwidget)
        self.btnClosePort.setObjectName(_fromUtf8("btnClosePort"))
        self.gridLayout.addWidget(self.btnClosePort, 0, 3, 1, 1)
        self.cboxPorts = QtGui.QComboBox(self.centralwidget)
        self.cboxPorts.setObjectName(_fromUtf8("cboxPorts"))
        self.gridLayout.addWidget(self.cboxPorts, 0, 0, 1, 1)
        self.cboxSpeed = QtGui.QComboBox(self.centralwidget)
        self.cboxSpeed.setObjectName(_fromUtf8("cboxSpeed"))
        self.gridLayout.addWidget(self.cboxSpeed, 0, 1, 1, 1)
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.plt1 = PlotWidget(self.centralwidget)
        self.plt1.setObjectName(_fromUtf8("plt1"))
        self.verticalLayout.addWidget(self.plt1)
        self.gridLayout.addLayout(self.verticalLayout, 2, 0, 1, 4)
        self.lbProbeError = QtGui.QLabel(self.centralwidget)
        self.lbProbeError.setEnabled(False)
        font = QtGui.QFont()
        font.setPointSize(20)
        self.lbProbeError.setFont(font)
        self.lbProbeError.setAlignment(QtCore.Qt.AlignCenter)
        self.lbProbeError.setObjectName(_fromUtf8("lbProbeError"))
        self.gridLayout.addWidget(self.lbProbeError, 1, 0, 1, 1)
        self.lbSearchPulse = QtGui.QLabel(self.centralwidget)
        self.lbSearchPulse.setEnabled(False)
        font = QtGui.QFont()
        font.setPointSize(20)
        self.lbSearchPulse.setFont(font)
        self.lbSearchPulse.setAlignment(QtCore.Qt.AlignCenter)
        self.lbSearchPulse.setObjectName(_fromUtf8("lbSearchPulse"))
        self.gridLayout.addWidget(self.lbSearchPulse, 1, 1, 1, 1)
        self.lbSeachTimeTooLong = QtGui.QLabel(self.centralwidget)
        self.lbSeachTimeTooLong.setEnabled(False)
        font = QtGui.QFont()
        font.setPointSize(20)
        self.lbSeachTimeTooLong.setFont(font)
        self.lbSeachTimeTooLong.setAlignment(QtCore.Qt.AlignCenter)
        self.lbSeachTimeTooLong.setObjectName(_fromUtf8("lbSeachTimeTooLong"))
        self.gridLayout.addWidget(self.lbSeachTimeTooLong, 1, 2, 1, 1)
        self.lbOxygenSaturationDecrease = QtGui.QLabel(self.centralwidget)
        self.lbOxygenSaturationDecrease.setEnabled(False)
        font = QtGui.QFont()
        font.setPointSize(20)
        self.lbOxygenSaturationDecrease.setFont(font)
        self.lbOxygenSaturationDecrease.setAlignment(QtCore.Qt.AlignCenter)
        self.lbOxygenSaturationDecrease.setObjectName(_fromUtf8("lbOxygenSaturationDecrease"))
        self.gridLayout.addWidget(self.lbOxygenSaturationDecrease, 1, 3, 1, 1)
        self.gridLayout_2.addLayout(self.gridLayout, 0, 0, 1, 1)
        self.plt2 = PlotWidget(self.centralwidget)
        self.plt2.setObjectName(_fromUtf8("plt2"))
        self.gridLayout_2.addWidget(self.plt2, 1, 0, 1, 1)
        self.plt3 = PlotWidget(self.centralwidget)
        self.plt3.setObjectName(_fromUtf8("plt3"))
        self.gridLayout_2.addWidget(self.plt3, 2, 0, 1, 1)
        self.plt4 = PlotWidget(self.centralwidget)
        self.plt4.setObjectName(_fromUtf8("plt4"))
        self.gridLayout_2.addWidget(self.plt4, 3, 0, 1, 1)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 19))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName(_fromUtf8("menuFile"))
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(MainWindow)
        self.statusbar.setObjectName(_fromUtf8("statusbar"))
        MainWindow.setStatusBar(self.statusbar)
        self.actionExit = QtGui.QAction(MainWindow)
        self.actionExit.setObjectName(_fromUtf8("actionExit"))
        self.menuFile.addAction(self.actionExit)
        self.menubar.addAction(self.menuFile.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(_translate("MainWindow", "Pulse Oxymeter", None))
        self.btnOpenPort.setText(_translate("MainWindow", "Open Port", None))
        self.btnClosePort.setText(_translate("MainWindow", "Close Port", None))
        self.cboxPorts.setToolTip(_translate("MainWindow", "<html><head/><body><p>Select IMU source</p></body></html>", None))
        self.lbProbeError.setText(_translate("MainWindow", "Probe Error", None))
        self.lbSearchPulse.setText(_translate("MainWindow", "Searching Pulse", None))
        self.lbSeachTimeTooLong.setText(_translate("MainWindow", "Searching too long", None))
        self.lbOxygenSaturationDecrease.setText(_translate("MainWindow", "Saturation Decrease", None))
        self.menuFile.setTitle(_translate("MainWindow", "File", None))
        self.actionExit.setText(_translate("MainWindow", "Exit", None))

from pyqtgraph import PlotWidget
