class Board:

	def __init__(self, height, width):
		""" Constructor for the Board class
		"""
		self.height = height
		self.width = width
		self.slots = [[' '] * self.width for row in range(self.height)]
		self.checker_count = {'X': 0, 'O', 0}

	def __repr__(self):
		""" Returns a string representation for a Board object.
		"""
		s = ''         # begin with an empty string

		# add one row of slots at a time
		for row in range(self.height):
		    s += '|'   # one vertical bar at the start of the row

		    for col in range(self.width):
		        s += self.slots[row][col] + '|'

		    s += '\n'  # newline at the end of the row

		# Add code here for the hyphens at the bottom of the board
		# and the numbers underneath it.
		s += '-' * 2 * self.width + '-' + '\n'
		for i in range(self.width):
			s += ' ' + str(i % 10)

		return s

	def add_checker(self, checker, col):
		""" Adds an x or o checker to a specific column
		"""
		assert(checker == 'X' or checker == 'O')
		assert(0 <= col < self.width)

		i = len(self.slots) - 1
		while True:
			if self.slots[i][col] == ' ':
				break
			i -= 1
		self.slots[i][col] = checker
		self.checkercount[checker] += 1

	def reset(self):
		""" Sets all the slots to contain a space character
		"""
		self.__init__(self.width, self.height)

	def add_checkers(self, colnums):
		""" takes in a string of column numbers and places alternating
		    checkers in those columns of the called Board object, 
		    starting with 'X'.
		"""
		checker = 'X'	# start by playing 'X'

		for col_str in colnums:
			col = int(col_str)
			if 0 <= col < self.width:
				self.add_checker(checker, col)

			# switch to the other checker
			if checker == 'X':
				checker = 'O'
			else:
				checker = 'X'

	def can_add_to(self, col):
		""" returns True if it is valid to place a checker in col
		"""
		if 0 <= col < self.width:
			if self.slots[0][col] == ' ':
				return True
		return False

	def is_full(self):
		""" returns True if all slots are filled
		"""
		for i in range(self.width):
			if self.can_add_to(i):
				return False
		return True

	def remove_checker(self, col):
		""" removes the top checker from col
		"""
		for i in range(self.height):
			if self.slots[i][col] != ' ':
				self.slots[i][col] = ' '
				break

	def is_win_for(self, checker):
		""" returns true if there are four consecutive slots of
			the parameter check
		"""
		assert(checker == 'X' or checker == 'O')

		return self.is_horizontal_win(checker) or \
			self.is_vertical_win(checker) or \
			self.is_down_diagonal_win(checker) or \
			self.is_up_diagonal_win(checker)

	def is_horizontal_win(self, checker):
		""" Checks for a horizontal win for the specified checker.
		"""
		for row in range(self.height):
			for col in range(self.width - 3):
				# Check if the next four columns in this row
				# contain the specified checker.
				if self.slots[row][col] == checker and \
					self.slots[row][col + 1] == checker and \
					self.slots[row][col + 2] == checker and \
					self.slots[row][col + 3] == checker:
					return True

		# if we make it here, there were no horizontal wins
		return False

	def is_vertical_win(self, checker):
		""" Checks for a vertical win for the specified checker.
		"""
		for col in range(self.width):
			for row in range(self.height - 3):
				if self.slots[row][col] == checker and \
					self.slots[row + 1][col] == checker and \
					self.slots[row + 2][col] == checker and \
					self.slots[row + 3][col] == checker:
					return True
		return False


	def is_down_diagonal_win(self, checker):
		""" Checks for a down diagonal win for the specified checker.
		"""
		for col in range(self.width):
			for row in range(self.height - 4):
				if self.slots[row][col] == checker:
					if col < self.width - 3:
						if self.slots[row + 1][col + 1] == checker and \
							self.slots[row + 2][col + 2] == checker and \
							self.slots[row + 3][col + 3] == checker:
							return True
					if col > 2:
						if self.slots[row + 1][col - 1] == checker and \
							self.slots[row + 2][col - 2] == checker and \
							self.slots[row + 3][col - 3] == checker:
							return True
		return False

	def is_up_diagonal_win(self, checker):
		""" Checks for an up diagonal win for the specified checker.
		"""
		for col in range(self.width):
			for row in range(3, self.height):
				if self.slots[row][col] == checker:
					if col < self.width - 3:
						if self.slots[row - 1][col + 1] == checker and \
							self.slots[row - 2][col + 2] == checker and \
							self.slots[row - 3][col + 3] == checker:
							return True
					if col > 2:
						if self.slots[row - 1][col - 1] == checker and \
							self.slots[row - 2][col - 2] == checker and \
							self.slots[row - 3][col - 3] == checker:
							return True
		return False