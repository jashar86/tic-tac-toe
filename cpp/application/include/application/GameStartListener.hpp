#ifndef GAME_APP_GAME_START_LISTENER_HPP
#define GAME_APP_GAME_START_LISTENER_HPP

/// \file GameStartListener.hpp
/// \brief Interface for handling game start events

#include "application/ContinuationResult.hpp"
#include "application/Session.hpp"

namespace game::app
{

/// \brief Interface for handling game start events
///
/// Implemented by the presentation layer to display game start
/// information, animations, or sounds. This is a hook that gets
/// called when a new game begins within a session.
class GameStartListener
{
public:
    virtual ~GameStartListener() = default;

    /// \brief Called when a new game starts
    ///
    /// The presentation layer can use this to show initial game state,
    /// display player names, or trigger animations.
    ///
    /// \param session The current session with game state
    /// \return How the application should proceed
    virtual ContinuationResult onGameStarted(const Session& session) = 0;
};

} // namespace game::app

#endif // GAME_APP_GAME_START_LISTENER_HPP
