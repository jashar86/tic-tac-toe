#include "application/TicTacToeApp.hpp"

#include "core/GameLogic.hpp"

namespace game::app
{

namespace
{
constexpr int MAX_INVALID_MOVE_RETRIES = 10;
} // namespace

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
    while (true)
    {
        auto session = createSession();
        if (!session.has_value())
        {
            return;
        }

        bool continueToNewSession = runSessionLoop(**session);
        if (!continueToNewSession)
        {
            return;
        }
    }
}

std::optional<std::unique_ptr<Session>> TicTacToeApp::createSession()
{
    auto sessionResult = sessionGenerator->startNewSession();
    if (!sessionResult.has_value())
    {
        return std::nullopt;
    }
    return std::move(sessionResult.value());
}

bool TicTacToeApp::runSessionLoop(Session& session)
{
    while (true)
    {
        auto gameResult = playSingleGame(session);
        if (!gameResult.has_value())
        {
            return false; // Quit during game
        }

        switch (gameResult.value())
        {
        case ContinuationResult::CONTINUE:
            session.resetGame();
            break;

        case ContinuationResult::RESET:
            return true; // Start new session

        case ContinuationResult::QUIT:
            return false;
        }
    }
}

std::optional<ContinuationResult> TicTacToeApp::playSingleGame(Session& session)
{
    auto startResult = gameStartListener->onGameStarted(session);
    if (startResult == ContinuationResult::QUIT)
    {
        return std::nullopt;
    }
    if (startResult == ContinuationResult::RESET)
    {
        return ContinuationResult::RESET;
    }

    if (!playTurns(session))
    {
        return std::nullopt;
    }

    updateScoreboard(session);
    return gameFinishedListener->onGameFinished(session);
}

bool TicTacToeApp::playTurns(Session& session)
{
    while (session.isActive())
    {
        if (!getValidMoveFromCurrentPlayer(session))
        {
            return false;
        }
    }
    return true;
}

bool TicTacToeApp::getValidMoveFromCurrentPlayer(Session& session)
{
    auto currentPlayer = session.getCurrentPlayer();
    auto currentState = session.getGameState();

    for (int attempt = 0; attempt < MAX_INVALID_MOVE_RETRIES; ++attempt)
    {
        auto moveResult = currentPlayer->generateNextMove(
            currentState.getBoard(), currentState.getCurrentTurn());

        if (!moveResult.has_value())
        {
            return false;
        }

        auto turnResult = game::core::takeTurn(currentState, moveResult.value());
        if (turnResult.has_value())
        {
            session.setGameState(turnResult.value());
            return true;
        }
    }

    return false;
}

void TicTacToeApp::updateScoreboard(Session& session)
{
    switch (session.getGameState().getStatus())
    {
    case game::core::GameStatus::XWins:
        session.recordPlayer1Win();
        break;

    case game::core::GameStatus::OWins:
        session.recordPlayer2Win();
        break;

    case game::core::GameStatus::Draw:
        session.recordDraw();
        break;

    case game::core::GameStatus::InProgress:
        break;
    }
}

} // namespace game::app
