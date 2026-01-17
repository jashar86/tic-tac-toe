#ifndef PLAYER_H
#define PLAYER_H

/**
 * @enum Player
 * @brief Represents the player types in Tic-Tac-Toe.
 *
 * Used to identify cell states and player marks on the board.
 * NONE represents an empty cell.
 */
enum class Player {
    NONE,  ///< Empty cell
    X,     ///< Player X
    O      ///< Player O
};

#endif // PLAYER_H
