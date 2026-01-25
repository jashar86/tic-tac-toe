#ifndef GAME_APP_SESSION_HPP
#define GAME_APP_SESSION_HPP

/// \file Session.hpp
/// \brief Game session management
///
/// The Session class manages a game session including players,
/// scoreboard, and current game state. This is part of the
/// Application layer and depends on the Core layer.

#include "core/Board.hpp"

namespace game::app {

/// \brief Manages a game session with two players
///
/// A session tracks the players, their scores across multiple games,
/// and the current game being played.
class Session {
public:
    Session();
    ~Session() = default;

    /// \brief Check if the session is active
    /// \return true if a game is currently in progress
    bool isActive() const;
};

} // namespace game::app

#endif // GAME_APP_SESSION_HPP
