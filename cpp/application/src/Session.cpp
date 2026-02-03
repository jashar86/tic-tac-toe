#include "application/Session.hpp"

namespace game::app
{

Session::Session(std::shared_ptr<Player> p1, std::shared_ptr<Player> p2)
    : player1(std::move(p1))
    , player2(std::move(p2))
    , scoreboard()
    , gameState()
{
}

std::shared_ptr<Player> Session::getPlayer1() const
{
    return player1;
}

std::shared_ptr<Player> Session::getPlayer2() const
{
    return player2;
}

Scoreboard& Session::getScoreboard()
{
    return scoreboard;
}

const Scoreboard& Session::getScoreboard() const
{
    return scoreboard;
}

const game::core::GameState& Session::getGameState() const
{
    return gameState;
}

void Session::setGameState(const game::core::GameState& state)
{
    gameState = state;
}

void Session::resetGame()
{
    gameState = game::core::GameState();
}

bool Session::isActive() const
{
    return gameState.getStatus() == game::core::GameStatus::InProgress;
}

std::shared_ptr<Player> Session::getCurrentPlayer() const
{
    return gameState.getCurrentTurn() == game::core::Marker::X ? player1 : player2;
}

} // namespace game::app
