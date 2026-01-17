"""
Tic-Tac-Toe game implementation
"""


class TicTacToe:
    """A simple Tic-Tac-Toe game"""

    def __init__(self):
        """Initialize an empty 3x3 board"""
        self.board = [[' ' for _ in range(3)] for _ in range(3)]
        self.current_player = 'X'

    def make_move(self, row, col):
        """
        Make a move on the board

        Args:
            row: Row index (0-2)
            col: Column index (0-2)

        Returns:
            bool: True if move was successful, False otherwise
        """
        if not (0 <= row < 3 and 0 <= col < 3):
            return False

        if self.board[row][col] != ' ':
            return False

        self.board[row][col] = self.current_player
        self.current_player = 'O' if self.current_player == 'X' else 'X'
        return True

    def check_winner(self):
        """
        Check if there's a winner

        Returns:
            str: 'X' or 'O' if there's a winner, None otherwise
        """
        # Check rows
        for row in self.board:
            if row[0] == row[1] == row[2] != ' ':
                return row[0]

        # Check columns
        for col in range(3):
            if self.board[0][col] == self.board[1][col] == self.board[2][col] != ' ':
                return self.board[0][col]

        # Check diagonals
        if self.board[0][0] == self.board[1][1] == self.board[2][2] != ' ':
            return self.board[0][0]

        if self.board[0][2] == self.board[1][1] == self.board[2][0] != ' ':
            return self.board[0][2]

        return None

    def is_board_full(self):
        """
        Check if the board is full

        Returns:
            bool: True if board is full, False otherwise
        """
        for row in self.board:
            if ' ' in row:
                return False
        return True

    def is_game_over(self):
        """
        Check if the game is over

        Returns:
            bool: True if game is over, False otherwise
        """
        return self.check_winner() is not None or self.is_board_full()

    def get_board(self):
        """
        Get the current board state

        Returns:
            list: 3x3 board representation
        """
        return [row[:] for row in self.board]

    def reset(self):
        """Reset the game to initial state"""
        self.board = [[' ' for _ in range(3)] for _ in range(3)]
        self.current_player = 'X'


def print_board(board):
    """Print the board in a human-readable format"""
    print("\n")
    for i, row in enumerate(board):
        print(" " + " | ".join(row))
        if i < 2:
            print("-----------")
    print("\n")


if __name__ == "__main__":
    game = TicTacToe()
    print("Tic-Tac-Toe Game")
    print("Player X starts")

    while not game.is_game_over():
        print_board(game.get_board())
        print(f"Player {game.current_player}'s turn")

        try:
            row = int(input("Enter row (0-2): "))
            col = int(input("Enter column (0-2): "))

            if game.make_move(row, col):
                winner = game.check_winner()
                if winner:
                    print_board(game.get_board())
                    print(f"Player {winner} wins!")
                    break
            else:
                print("Invalid move! Try again.")
        except (ValueError, KeyboardInterrupt):
            print("\nGame ended.")
            break

    if game.is_board_full() and not game.check_winner():
        print_board(game.get_board())
        print("It's a tie!")
