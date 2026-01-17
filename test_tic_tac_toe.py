"""
Unit tests for Tic-Tac-Toe game
"""

import unittest
from tic_tac_toe import TicTacToe


class TestTicTacToe(unittest.TestCase):
    """Test cases for TicTacToe class"""

    def setUp(self):
        """Set up a new game before each test"""
        self.game = TicTacToe()

    def test_initialization(self):
        """Test that the game initializes correctly"""
        self.assertEqual(self.game.current_player, 'X')
        self.assertEqual(len(self.game.board), 3)
        self.assertEqual(len(self.game.board[0]), 3)
        for row in self.game.board:
            for cell in row:
                self.assertEqual(cell, ' ')

    def test_valid_move(self):
        """Test making a valid move"""
        result = self.game.make_move(0, 0)
        self.assertTrue(result)
        self.assertEqual(self.game.board[0][0], 'X')
        self.assertEqual(self.game.current_player, 'O')

    def test_invalid_move_out_of_bounds(self):
        """Test that moves out of bounds are rejected"""
        self.assertFalse(self.game.make_move(-1, 0))
        self.assertFalse(self.game.make_move(0, -1))
        self.assertFalse(self.game.make_move(3, 0))
        self.assertFalse(self.game.make_move(0, 3))

    def test_invalid_move_occupied_cell(self):
        """Test that moves to occupied cells are rejected"""
        self.game.make_move(0, 0)
        result = self.game.make_move(0, 0)
        self.assertFalse(result)

    def test_player_alternation(self):
        """Test that players alternate correctly"""
        self.assertEqual(self.game.current_player, 'X')
        self.game.make_move(0, 0)
        self.assertEqual(self.game.current_player, 'O')
        self.game.make_move(1, 1)
        self.assertEqual(self.game.current_player, 'X')

    def test_row_win(self):
        """Test winning by completing a row"""
        self.game.make_move(0, 0)  # X
        self.game.make_move(1, 0)  # O
        self.game.make_move(0, 1)  # X
        self.game.make_move(1, 1)  # O
        self.game.make_move(0, 2)  # X wins
        self.assertEqual(self.game.check_winner(), 'X')

    def test_column_win(self):
        """Test winning by completing a column"""
        self.game.make_move(0, 0)  # X
        self.game.make_move(0, 1)  # O
        self.game.make_move(1, 0)  # X
        self.game.make_move(1, 1)  # O
        self.game.make_move(2, 0)  # X wins
        self.assertEqual(self.game.check_winner(), 'X')

    def test_diagonal_win(self):
        """Test winning by completing a diagonal"""
        self.game.make_move(0, 0)  # X
        self.game.make_move(0, 1)  # O
        self.game.make_move(1, 1)  # X
        self.game.make_move(0, 2)  # O
        self.game.make_move(2, 2)  # X wins
        self.assertEqual(self.game.check_winner(), 'X')

    def test_anti_diagonal_win(self):
        """Test winning by completing the anti-diagonal"""
        self.game.make_move(0, 2)  # X
        self.game.make_move(0, 0)  # O
        self.game.make_move(1, 1)  # X
        self.game.make_move(0, 1)  # O
        self.game.make_move(2, 0)  # X wins
        self.assertEqual(self.game.check_winner(), 'X')

    def test_no_winner_yet(self):
        """Test that no winner is declared when game is in progress"""
        self.game.make_move(0, 0)  # X
        self.game.make_move(1, 1)  # O
        self.assertIsNone(self.game.check_winner())

    def test_is_board_full(self):
        """Test detecting a full board"""
        self.assertFalse(self.game.is_board_full())

        # Fill the board
        moves = [
            (0, 0), (0, 1), (0, 2),
            (1, 1), (1, 0), (1, 2),
            (2, 1), (2, 0), (2, 2)
        ]
        for row, col in moves:
            self.game.make_move(row, col)

        self.assertTrue(self.game.is_board_full())

    def test_tie_game(self):
        """Test a game that ends in a tie"""
        # Create a tie scenario
        moves = [
            (0, 0), (0, 1), (0, 2),  # X O X
            (1, 1), (1, 0), (1, 2),  # O X O
            (2, 1), (2, 0), (2, 2)   # O X X
        ]
        for row, col in moves:
            self.game.make_move(row, col)

        self.assertIsNone(self.game.check_winner())
        self.assertTrue(self.game.is_board_full())
        self.assertTrue(self.game.is_game_over())

    def test_game_over_with_winner(self):
        """Test that game is over when there's a winner"""
        self.game.make_move(0, 0)  # X
        self.game.make_move(1, 0)  # O
        self.game.make_move(0, 1)  # X
        self.game.make_move(1, 1)  # O
        self.game.make_move(0, 2)  # X wins
        self.assertTrue(self.game.is_game_over())

    def test_reset(self):
        """Test resetting the game"""
        self.game.make_move(0, 0)
        self.game.make_move(1, 1)
        self.game.reset()

        self.assertEqual(self.game.current_player, 'X')
        for row in self.game.board:
            for cell in row:
                self.assertEqual(cell, ' ')

    def test_get_board(self):
        """Test getting a copy of the board"""
        board = self.game.get_board()
        self.assertEqual(len(board), 3)

        # Modify the copy and ensure original is unchanged
        board[0][0] = 'X'
        self.assertEqual(self.game.board[0][0], ' ')


if __name__ == '__main__':
    unittest.main()
