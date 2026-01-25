#ifndef TICTACTOE_DOMAIN_BOARD_H
#define TICTACTOE_DOMAIN_BOARD_H

/**
 * @file Board.h
 * @brief Tic-Tac-Toe board representation
 *
 * The Board class represents the 3x3 game board. This is part of the
 * Domain layer and has no external dependencies.
 *
 * TODO: Implement full board logic including:
 * - Cell access (get/set markers)
 * - Available positions query
 * - Board state validation
 */

namespace tictactoe::domain {

/**
 * @brief Represents a 3x3 Tic-Tac-Toe board
 *
 * The board is the core data structure for the game state.
 * It provides methods to query and modify cell contents.
 */
class Board {
public:
    Board();
    ~Board() = default;

    /**
     * @brief Check if the board is empty
     * @return true if all cells are empty
     */
    bool isEmpty() const;
};

} // namespace tictactoe::domain

#endif // TICTACTOE_DOMAIN_BOARD_H
