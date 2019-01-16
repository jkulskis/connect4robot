from board import Board
from player import Player
from aiPlayer import AIPlayer
from arduino import Arduino
import random
import time

def communicate(port):
    """ Loops so that the program can listen for when the arduino wants to start a
        game. Lets the user cancel a game or change difficulties. Initializes the Arduino class.
        port: port on which the arduino is connected
    """
    ard = Arduino(port)
    ard.openPort()
    while True:
        difficulty = ''
        while True:
            read = ard.readAll()
            if len(read) > 0:
                difficulty = bytes.decode(read)
                print(difficulty)
                if difficulty in ['e', 'i', 'h']: 
                    break
        connect_four(ard, difficulty)


            

def connect_four(ard, difficulty):
    """ Plays a game of Connect Four with the given port for ardication
        and returns the Board object as it looks at the end of the game.
        inputs: port is the port at which the arduino is connected
    """

    #Make sure the arduino port is open
    ard = ard
    player1 = Player('X', ard)
    if difficulty == 'e':
        player2 = AIPlayer('O', ard, 'RANDOM', 3)
    elif difficulty == 'i':
        player2 = AIPlayer('O', ard, 'RANDOM', 4)
    elif difficulty == 'h':
        player2 = AIPlayer('O', ard, 'RANDOM', 5)

    # Make sure one player is 'X' and one player is 'O'.
    if player1.checker not in 'XO' or player2.checker not in 'XO' \
       or player1.checker == player2.checker:
        print('need one X player and one O player.')
        return None

    print('Welcome to Connect Four!')
    print()
    board = Board(6, 7)
    print(board)
    
    while True:
        if process_move(player1, board) == True:
            if board.is_full():
                ard.write("tie")
            else:
                ard.write("loss")
            return board

        if process_move(player2, board) == True:
            if board.is_full():
                ard.write("tie")
            else:
                ard.write("win")
            return board

def process_move(player, board):
    """ Processes a single move by a player
    """
    #a
    print('Player', player.checker + "'s", 'turn')
    #b
    col = player.next_move(board)
    #c
    board.add_checker(player.checker, col)
    #d
    print('\n' + str(board))
    #e
    if board.is_win_for(player.checker):
        print('Player', player.checker, 'wins in', player.num_moves, 'moves.Congratulations!')
        return True
    elif board.is_full():
        print("It's a tie!")
        return True
    else:
        return False
