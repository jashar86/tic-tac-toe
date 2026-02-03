#ifndef GAME_APP_PLAYER_HPP
#define GAME_APP_PLAYER_HPP

/// \file Player.hpp
/// \brief Defines the Player interface for game participants

#include <expected>
#include <string>

#include "application/Quit.hpp"
#include "core/Board.hpp"
#include "core/Marker.hpp"
#include "core/Position.hpp"

namespace game::app
{

/// \brief Interface for game participants (humans or AI)
///
/// The Player interface abstracts human input and AI computation
/// behind a common contract. This allows the application layer
/// to coordinate turns without knowing the implementation details.
class Player
{
public:
    virtual ~Player() = default;

    /// \brief Get the player's display name
    /// \return The player's name
    virtual std::string getName() const = 0;

    /// \brief Generate the next move for this player
    /// \param board The current board state
    /// \param marker The marker this player is using (X or O)
    /// \return The chosen position, or Quit if user wants to exit
    virtual std::expected<game::core::Position, Quit> generateNextMove(
        const game::core::Board& board,
        game::core::Marker marker) = 0;
};

} // namespace game::app

#endif // GAME_APP_PLAYER_HPP
