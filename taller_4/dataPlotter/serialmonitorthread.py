import serial

from PyQt4 import QtCore
from basethread import BaseThread

class SerialMonitorThread(BaseThread):
	def __init__(self):
		BaseThread.__init__(self)

	def openPort(self,port,baud):
		# configure serial port
		try:
			self.ser=serial.Serial(
								port=port,
								baudrate=baud,
								stopbits=serial.STOPBITS_ONE,
								bytesize=serial.EIGHTBITS,
								timeout=0.05)
			# clean input buffer
			self.ser.flushInput()
			print "SerialMonitorThread(): Serial port %s opened" % port
		except serial.SerialException as se:
			print "SerialMonitorThread().openPort(): Unable to open port %s" % port
			print se
			
	def run(self):
		while not self.exiting: 
			try:
				if self.ser.inWaiting()>0:
					raw=self.ser.readline().strip()
					# process data only if have a minimal length
					if(len(raw)>=15):
						#self.data=map(float, raw.split(','))
						self.data=raw.split(',')
						self.emit(QtCore.SIGNAL('newData()'))
			except serial.SerialException as s:
				print "SerialMonitorThread().run(): "+s
	
	def getData(self):
		return self.data

	def close(self):
		self.ser.close()
		self.exiting=True
		print "SerialMonitorThread(): Serial port closed"