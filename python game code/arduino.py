import serial

class Arduino:

	def __init__(self, port):
		""" Initializes an arduino on a port with timeout 1 listening at 9600 baud
		"""
		self.port = port
                self.ser = serial.Serial(port, 9600)
                self.ser.timeout = 1

	def setPort(self, port):
		""" Sets the port for the arduino
		"""
                self.port = port
                self.ser = serial.Serial(port, 9600)

	def openPort(self):
		""" Opens up the port for listening
		"""
		if self.ser.isOpen() == False:
			self.ser.open()
			return True
		return False

	def closePort(self):
		""" Closes the port
		"""
		if self.ser.isOpen():
			self.ser.close()
			return True
		return False

	def write(self, data):
		""" Writes data to the arduino, ending with the '*' char
		"""
		stringData = str(data) + '*'
		self.ser.write(str.encode(stringData))

	def readAll(self):
		""" Reads the first char sent by the arduino. By default, ser.read() only
			reads a length of 1. Can change in the future if I need more than 1 char
			length communications from the arduino, but for now I just need the player
			column choices and a difficulty choice
		"""
		return self.ser.read()

	def __repr__(self):
		"""	Prints out the info for the arduino
		"""
                return 'Arduino on port', port
