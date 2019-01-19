from board import Board
from arduino import Arduino

class Player:

	def __init__(self, checker, ard):
		""" Constructor for the Player class
		"""
		self.checker = checker
		self.num_moves = 0
		self.ard = ard

	def __repr__(self):
		""" Returns the string of the player
		"""
		return 'Player ' + self.checker

	def opponent_checker(self):
		""" Returns the string of the opponent player
		"""
		if self.checker == 'X':
			return 'O'
		return 'X'

	def next_move(self, board):
		""" Accepts a Board object and returns the column where
			the player wants to make their next move
		"""
		self.num_moves += 1
		col = -1
		count = 0
		while True:
			read = self.ard.readAll()
			# Clear any junk left in the serial by making sure it is at least
			# the second request
			if len(read) > 0 and count > 0:
				col = int(bytes.decode(read)[0])
				return col
			count+=1
