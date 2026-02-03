#include "application/Scoreboard.hpp"

namespace game::app
{

int Scoreboard::getPlayer1Wins() const noexcept
{
    return player1Wins;
}

int Scoreboard::getPlayer2Wins() const noexcept
{
    return player2Wins;
}

int Scoreboard::getDraws() const noexcept
{
    return draws;
}

int Scoreboard::getTotalGames() const noexcept
{
    return player1Wins + player2Wins + draws;
}

void Scoreboard::recordPlayer1Win() noexcept
{
    ++player1Wins;
}

void Scoreboard::recordPlayer2Win() noexcept
{
    ++player2Wins;
}

void Scoreboard::recordDraw() noexcept
{
    ++draws;
}

void Scoreboard::reset() noexcept
{
    player1Wins = 0;
    player2Wins = 0;
    draws = 0;
}

} // namespace game::app
