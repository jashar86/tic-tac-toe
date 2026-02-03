#include "application/TicTacToeApp.hpp"

#include "core/GameLogic.hpp"

namespace game::app
{

TicTacToeApp::TicTacToeApp(std::shared_ptr<SessionGenerator> sessGen,
                           std::shared_ptr<GameStartListener> startListener,
                           std::shared_ptr<GameFinishedListener> finishListener)
    : sessionGenerator(std::move(sessGen))
    , gameStartListener(std::move(startListener))
    , gameFinishedListener(std::move(finishListener))
{
}

void TicTacToeApp::run()
{
    bool running = true;

    while (running)
    {
        // Welcome stage: create a new session
        auto sessionResult = sessionGenerator->startNewSession();
        if (!sessionResult.has_value())
        {
            // User quit from welcome screen
            return;
        }

        auto session = std::move(sessionResult.value());
        bool sessionActive = true;

        while (sessionActive)
        {
            // Game Start stage
            auto startResult = gameStartListener->onGameStarted(*session);
            if (startResult == ContinuationResult::QUIT)
            {
                return;
            }
            if (startResult == ContinuationResult::RESET)
            {
                sessionActive = false;
                continue;
            }

            // Player Turns stage
            if (!playTurns(*session))
            {
                // Player quit during game
                return;
            }

            // Update scoreboard with game result
            updateScoreboard(*session);

            // Game Finished stage
            auto finishResult = gameFinishedListener->onGameFinished(*session);

            switch (finishResult)
            {
            case ContinuationResult::CONTINUE:
                // Play again - reset the game state
                session->resetGame();
                break;

            case ContinuationResult::RESET:
                // Return to welcome screen (new session)
                sessionActive = false;
                break;

            case ContinuationResult::QUIT:
                // Exit the application
                return;
            }
        }
    }
}

bool TicTacToeApp::playTurns(Session& session)
{
    while (session.isActive())
    {
        auto currentPlayer = session.getCurrentPlayer();
        auto currentState = session.getGameState();

        // Get the next move from the current player
        auto moveResult = currentPlayer->generateNextMove(
            currentState.getBoard(), currentState.getCurrentTurn());

        if (!moveResult.has_value())
        {
            // Player quit
            return false;
        }

        // Apply the move
        auto turnResult = game::core::takeTurn(currentState, moveResult.value());
        if (turnResult.has_value())
        {
            session.setGameState(turnResult.value());
        }
        // If turn was invalid, the game state doesn't change and we try again
        // In a real implementation, we might want to handle this differently
    }

    return true;
}

void TicTacToeApp::updateScoreboard(Session& session)
{
    auto status = session.getGameState().getStatus();

    switch (status)
    {
    case game::core::GameStatus::XWins:
        session.getScoreboard().recordPlayer1Win();
        break;

    case game::core::GameStatus::OWins:
        session.getScoreboard().recordPlayer2Win();
        break;

    case game::core::GameStatus::Draw:
        session.getScoreboard().recordDraw();
        break;

    case game::core::GameStatus::InProgress:
        // Should not happen, but handle gracefully
        break;
    }
}

} // namespace game::app
