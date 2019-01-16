import random 
from player import Player
from arduino import Arduino

class AIPlayer(Player):

	def __init__(self, checker, ard, tiebreak, lookahead):
		""" Constructor for AIPlayer
		"""
		assert(checker == 'X' or checker == 'O')
		assert(tiebreak == 'LEFT' or tiebreak == 'RIGHT' or tiebreak == 'RANDOM')
		assert(lookahead >= 0)

		super().__init__(checker, ard)
		self.tiebreak = tiebreak
		self.lookahead = lookahead

	def __repr__(self):
		""" Returns a string version of AIPlayer
		"""
		return super().__repr__() + ' (' + self.tiebreak + ', ' + str(self.lookahead) + ')'

	def max_score_column(self, scores):
		""" Returns the index of the column with the maximum score
		"""
		max_score = max(scores)
		max_indices = [i for i in range(len(scores)) if scores[i] == max_score]

		if self.tiebreak == 'LEFT':
			return max_indices[0]
		if self.tiebreak == 'RIGHT':
			return max_indices[len(max_indices) - 1]
		return random.choice(max_indices)

	def scores_for(self, board):
		""" determines the called AIPlayer's scores for the columns
		"""
		scores = [50] * board.width

		for col in range(board.width):
			if board.slots[0][col] != ' ':
				scores[col] = -1
			elif board.is_win_for(self.checker):
				scores[col] = 100
			elif board.is_win_for(self.opponent_checker()):
				scores[col] = 0
			elif self.lookahead == 0:
				scores[col] = 50
			else:
				board.add_checker(self.checker, col)
				opp = AIPlayer(self.opponent_checker(), self.ard, 'LEFT', self.lookahead - 1)
				opp_scores = opp.scores_for(board)
				if 100 in opp_scores:
					scores[col] = 0
				elif 50 not in opp_scores:
					scores[col] = 100
				else:
					scores[col] = 50
				board.remove_checker(col)
		return scores

	def next_move(self, board):
		""" Returns the AIPlayer's judgment of the best possible move
		"""
		self.num_moves += 1
		col = self.max_score_column(self.scores_for(board))
		self.ard.write(col)
		return col