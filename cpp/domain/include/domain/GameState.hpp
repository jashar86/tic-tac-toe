#ifndef GAME_CORE_GAMESTATE_HPP
#define GAME_CORE_GAMESTATE_HPP

/**
 * @file GameState.hpp
 * @brief Complete game state representation
 */

#include "domain/Board.hpp"
#include "domain/Marker.hpp"
#include "domain/GameStatus.hpp"

namespace game::core {

/**
 * @brief Represents the complete state of a tic-tac-toe game
 *
 * Contains the board, whose turn it is, and the current game status.
 */
class GameState {
public:
    /**
     * @brief Construct initial game state
     *
     * Creates an empty board with X to play and status InProgress.
     */
    GameState();

    /**
     * @brief Construct game state with specific values
     * @param board The board state
     * @param turn Whose turn it is
     * @param status The game status
     */
    GameState(const Board& board, Marker turn, GameStatus status);

    ~GameState() = default;

    /**
     * @brief Get the board
     * @return Const reference to the board
     */
    const Board& board() const;

    /**
     * @brief Get whose turn it is
     * @return The marker of the current player
     */
    Marker currentTurn() const;

    /**
     * @brief Get the game status
     * @return The current game status
     */
    GameStatus status() const;

private:
    Board board_;
    Marker turn_;
    GameStatus status_;
};

} // namespace game::core

#endif // GAME_CORE_GAMESTATE_HPP
