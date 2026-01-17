#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include "Player.h"

/**
 * @class Board
 * @brief Represents a 3x3 Tic-Tac-Toe game board.
 *
 * The Board class encapsulates board state for Tic-Tac-Toe.
 * It manages the 3x3 grid of cells, validates moves, detects win conditions,
 * and provides board representation. This class is designed to be independent of
 * game flow logic, making it suitable for AI player implementation.
 *
 * Board coordinates use (row, col) indexing where both are 0-based:
 * - Valid ranges: row ∈ [0,2], col ∈ [0,2]
 * - (0,0) is top-left, (2,2) is bottom-right
 *
 * Example usage:
 * @code
 *   Board board;
 *   board.setCell(0, 0, Player::X);  // Place X in top-left
 *   board.setCell(1, 1, Player::O);  // Place O in center
 *   Player winner = board.getWinner();
 *   if (winner != Player::NONE) {
 *       // Handle win
 *   }
 * @endcode
 */
class Board {
private:
    std::vector<std::vector<Player>> cells;  ///< 3x3 grid storing cell states

public:
    /**
     * @brief Constructs a new empty Board.
     *
     * Initializes a 3x3 board with all cells set to Player::NONE.
     */
    Board();

    // Board operations

    /**
     * @brief Sets a cell to the specified player's mark.
     *
     * @param row The row index (0-2)
     * @param col The column index (0-2)
     * @param player The player (Player::X or Player::O)
     * @return true if the move was successful, false if the position is invalid
     *         or already occupied
     *
     * This method validates the position and checks if the cell is empty before
     * placing the mark.
     */
    bool setCell(int row, int col, Player player);

    /**
     * @brief Gets the current value of a cell.
     *
     * @param row The row index (0-2)
     * @param col The column index (0-2)
     * @return The cell value (Player::NONE for empty, Player::X, or Player::O)
     */
    Player getCell(int row, int col) const;

    /**
     * @brief Checks if a cell is empty.
     *
     * @param row The row index (0-2)
     * @param col The column index (0-2)
     * @return true if the cell is empty and the position is valid, false otherwise
     */
    bool isCellEmpty(int row, int col) const;

    /**
     * @brief Validates if a position is within board boundaries.
     *
     * @param row The row index to validate
     * @param col The column index to validate
     * @return true if 0 <= row < 3 AND 0 <= col < 3, false otherwise
     */
    bool isValidPosition(int row, int col) const;

    /**
     * @brief Resets the board to initial empty state.
     *
     * Clears all cells (sets to Player::NONE).
     */
    void reset();

    // Board state queries

    /**
     * @brief Checks if the board is completely filled.
     *
     * @return true if all 9 cells have been filled, false otherwise
     */
    bool isFull() const;

    /**
     * @brief Gets the winning player.
     *
     * @return The winning player (Player::X or Player::O), or Player::NONE if no winner
     *
     * This checks all possible win conditions:
     * - 3 rows (horizontal)
     * - 3 columns (vertical)
     * - 2 diagonals
     */
    Player getWinner() const;

    // Board representation

    /**
     * @brief Converts the board to a string representation.
     *
     * @return A formatted string showing the board state with grid lines
     *
     * Example output:
     * @code
     *  X | O |
     * ---|---|---
     *  X | O |
     * ---|---|---
     *  X |   |
     * @endcode
     */
    std::string toString() const;

    /**
     * @brief Gets a copy of the internal cell grid.
     *
     * @return A 3x3 vector of vectors containing the current board state
     *
     * Useful for AI algorithms that need to analyze the board state.
     * Returns a copy to prevent external modification of internal state.
     */
    std::vector<std::vector<Player>> getCells() const;
};

#endif // BOARD_H
