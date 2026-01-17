#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "Board.h"

/**
 * @class TicTacToe
 * @brief Manages the game flow and player turns for Tic-Tac-Toe.
 *
 * The TicTacToe class handles the high-level game logic including player turns,
 * move processing, game state management, and user interaction. It uses the Board
 * class to manage the actual board state and game rules.
 *
 * The game alternates between two players: 'X' (who goes first) and 'O'.
 * After each valid move, the current player switches unless the game ends.
 *
 * Example usage:
 * @code
 *   TicTacToe game;
 *   game.displayBoard();
 *   if (game.makeMove(0, 0)) {  // X plays at (0,0)
 *       // Move successful
 *   }
 *   if (game.isGameOver()) {
 *       char winner = game.getWinner();
 *       if (winner != ' ') {
 *           // Winner found
 *       } else {
 *           // Draw
 *       }
 *   }
 * @endcode
 */
class TicTacToe {
private:
    Board board;           ///< The game board
    char currentPlayer;    ///< Current player's mark ('X' or 'O')

public:
    /**
     * @brief Constructs a new TicTacToe game.
     *
     * Initializes an empty board with 'X' as the starting player.
     */
    TicTacToe();

    /**
     * @brief Displays the current board state to console.
     *
     * Prints a formatted representation of the board with grid lines
     * to standard output.
     */
    void displayBoard() const;

    /**
     * @brief Attempts to make a move for the current player.
     *
     * @param row The row index (0-2)
     * @param col The column index (0-2)
     * @return true if the move was successful, false if invalid or cell occupied
     *
     * On success, places the current player's mark at the specified position
     * and switches to the other player (unless the game ends). On failure,
     * the current player remains unchanged.
     */
    bool makeMove(int row, int col);

    /**
     * @brief Checks if the game has ended.
     *
     * @return true if there is a winner or the board is full (draw), false otherwise
     */
    bool isGameOver() const;

    /**
     * @brief Gets the winning player.
     *
     * @return The winning player's mark ('X' or 'O'), or ' ' if no winner
     *
     * Should be called after isGameOver() returns true to determine if there
     * was a winner or a draw.
     */
    char getWinner() const;

    /**
     * @brief Gets the current player's mark.
     *
     * @return 'X' or 'O' indicating whose turn it is
     */
    char getCurrentPlayer() const;

    /**
     * @brief Resets the game to initial state.
     *
     * Clears the board and sets the current player back to 'X'.
     * Use this to start a new game.
     */
    void reset();

    /**
     * @brief Gets a reference to the internal board.
     *
     * @return Const reference to the Board object
     *
     * Useful for accessing board state without modifying it, such as
     * for AI analysis or game state inspection.
     */
    const Board& getBoard() const;
};

#endif // TICTACTOE_H
