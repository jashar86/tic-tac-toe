#ifndef GAME_CORE_GAMESTATUS_HPP
#define GAME_CORE_GAMESTATUS_HPP

/**
 * @file GameStatus.hpp
 * @brief Game status enumeration
 */

namespace game::core {

/**
 * @brief Represents the current state of the game
 */
enum class GameStatus {
    InProgress,  ///< Game is still ongoing
    XWins,       ///< X has won the game
    OWins,       ///< O has won the game
    Draw         ///< Game ended in a draw
};

/**
 * @brief Check if the game has ended
 * @param status The current game status
 * @return true if the game has ended (win or draw)
 */
bool isTerminal(GameStatus status);

} // namespace game::core

#endif // GAME_CORE_GAMESTATUS_HPP
