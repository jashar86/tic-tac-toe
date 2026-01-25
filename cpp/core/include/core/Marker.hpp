#ifndef GAME_CORE_MARKER_HPP
#define GAME_CORE_MARKER_HPP

/// \file Marker.hpp
/// \brief Marker enumeration for tic-tac-toe cells

namespace game::core {

/// \brief Represents a player's marker on the board
enum class Marker {
    X,  ///< X marker
    O   ///< O marker
};

/// \brief Get the opponent's marker
/// \param marker The current marker (X or O)
/// \return The opponent's marker
Marker opponent(Marker marker);

} // namespace game::core

#endif // GAME_CORE_MARKER_HPP
