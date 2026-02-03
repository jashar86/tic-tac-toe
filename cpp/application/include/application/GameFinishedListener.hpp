#ifndef GAME_APP_GAME_FINISHED_LISTENER_HPP
#define GAME_APP_GAME_FINISHED_LISTENER_HPP

/// \file GameFinishedListener.hpp
/// \brief Interface for handling game finished events

#include "application/ContinuationResult.hpp"
#include "application/Session.hpp"

namespace game::app
{

/// \brief Interface for handling game finished events
///
/// Implemented by the presentation layer to display game results,
/// victory screens, and updated statistics. This is a hook that gets
/// called when a game ends (win or draw).
class GameFinishedListener
{
public:
    virtual ~GameFinishedListener() = default;

    /// \brief Called when a game ends
    ///
    /// The presentation layer can use this to show the result,
    /// update displayed statistics, and prompt for next action.
    ///
    /// \param session The current session with final game state and scoreboard
    /// \return How the application should proceed
    virtual ContinuationResult onGameFinished(const Session& session) = 0;
};

} // namespace game::app

#endif // GAME_APP_GAME_FINISHED_LISTENER_HPP
