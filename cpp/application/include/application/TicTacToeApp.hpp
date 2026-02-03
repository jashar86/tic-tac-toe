#ifndef GAME_APP_TIC_TAC_TOE_APP_HPP
#define GAME_APP_TIC_TAC_TOE_APP_HPP

/// \file TicTacToeApp.hpp
/// \brief Main application class that orchestrates the game flow

#include <memory>

#include "application/GameFinishedListener.hpp"
#include "application/GameStartListener.hpp"
#include "application/SessionGenerator.hpp"

namespace game::app
{

/// \brief Main application orchestrator for Tic-Tac-Toe
///
/// TicTacToeApp manages the application lifecycle using a finite state machine
/// with stages: Welcome -> GameStart -> PlayerTurns -> GameFinished.
/// It uses dependency injection for all UI interactions, making it
/// decoupled from any specific presentation layer.
class TicTacToeApp
{
public:
    /// \brief Construct the application with its dependencies
    /// \param sessionGenerator Creates new game sessions (from presentation layer)
    /// \param gameStartListener Handles game start events (from presentation layer)
    /// \param gameFinishedListener Handles game end events (from presentation layer)
    TicTacToeApp(std::shared_ptr<SessionGenerator> sessionGenerator,
                 std::shared_ptr<GameStartListener> gameStartListener,
                 std::shared_ptr<GameFinishedListener> gameFinishedListener);

    ~TicTacToeApp() = default;

    /// \brief Run the application main loop
    ///
    /// Starts at the Welcome stage and processes transitions until
    /// the user requests to quit.
    void run();

private:
    /// \brief Execute player turns until the game ends
    /// \param session The current session
    /// \return true if turns completed normally, false if quit requested
    bool playTurns(Session& session);

    /// \brief Update the scoreboard based on game result
    /// \param session The session with completed game
    void updateScoreboard(Session& session);

    std::shared_ptr<SessionGenerator> sessionGenerator;
    std::shared_ptr<GameStartListener> gameStartListener;
    std::shared_ptr<GameFinishedListener> gameFinishedListener;
};

} // namespace game::app

#endif // GAME_APP_TIC_TAC_TOE_APP_HPP
