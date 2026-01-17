#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>

/**
 * @class Board
 * @brief Represents a 3x3 Tic-Tac-Toe game board.
 *
 * The Board class encapsulates all board state and game logic for Tic-Tac-Toe.
 * It manages the 3x3 grid of cells, validates moves, detects win/draw conditions,
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
 *   board.setCell(0, 0, 'X');  // Place X in top-left
 *   board.setCell(1, 1, 'O');  // Place O in center
 *   if (board.hasWinner()) {
 *       char winner = board.getWinner();
 *       // Handle win
 *   }
 * @endcode
 */
class Board {
private:
    std::vector<std::vector<char>> cells;  ///< 3x3 grid storing cell states (' ', 'X', or 'O')
    int moveCount;                         ///< Number of moves made on the board

public:
    /**
     * @brief Constructs a new empty Board.
     *
     * Initializes a 3x3 board with all cells empty (space character).
     * Move count is set to 0.
     */
    Board();

    // Board operations

    /**
     * @brief Sets a cell to the specified player's mark.
     *
     * @param row The row index (0-2)
     * @param col The column index (0-2)
     * @param player The player's mark ('X' or 'O')
     * @return true if the move was successful, false if the position is invalid
     *         or already occupied
     *
     * This method validates the position and checks if the cell is empty before
     * placing the mark. On success, increments the move count.
     */
    bool setCell(int row, int col, char player);

    /**
     * @brief Gets the current value of a cell.
     *
     * @param row The row index (0-2)
     * @param col The column index (0-2)
     * @return The cell value (' ' for empty, 'X', or 'O'), or '\0' if the
     *         position is invalid
     */
    char getCell(int row, int col) const;

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
     * Clears all cells (sets to space character) and resets move count to 0.
     */
    void reset();

    // Game state checks

    /**
     * @brief Checks if there is a winner on the board.
     *
     * @return true if any player has three marks in a row (horizontally,
     *         vertically, or diagonally), false otherwise
     *
     * This checks all possible win conditions:
     * - 3 rows (horizontal)
     * - 3 columns (vertical)
     * - 2 diagonals
     */
    bool hasWinner() const;

    /**
     * @brief Checks if the board is completely filled.
     *
     * @return true if all 9 cells have been filled (moveCount == 9), false otherwise
     */
    bool isFull() const;

    /**
     * @brief Gets the winning player's mark.
     *
     * @return The winning player's mark ('X' or 'O'), or ' ' if there is no winner
     *
     * Note: This method should only be called after hasWinner() returns true.
     * If called when there is no winner, returns ' '.
     */
    char getWinner() const;

    /**
     * @brief Gets the total number of moves made on the board.
     *
     * @return The number of non-empty cells (0-9)
     */
    int getMoveCount() const;

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
    std::vector<std::vector<char>> getCells() const;
};

#endif // BOARD_H
