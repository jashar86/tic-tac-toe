#ifndef GAME_CORE_MARKER_HPP
#define GAME_CORE_MARKER_HPP

/**
 * @file Marker.hpp
 * @brief Marker enumeration for tic-tac-toe cells
 */

namespace game::core {

/**
 * @brief Represents the possible states of a cell on the board
 */
enum class Marker {
    Empty,  ///< Cell is unoccupied
    X,      ///< Cell contains X marker
    O       ///< Cell contains O marker
};

/**
 * @brief Get the opponent's marker
 * @param marker The current marker (X or O)
 * @return The opponent's marker
 * @note Behavior undefined for Empty marker
 */
Marker opponent(Marker marker);

} // namespace game::core

#endif // GAME_CORE_MARKER_HPP
