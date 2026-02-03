#include "application/Scoreboard.hpp"

namespace game::app
{

Scoreboard::Scoreboard()
    : player1Wins(0)
    , player2Wins(0)
    , draws(0)
{
}

int Scoreboard::getPlayer1Wins() const
{
    return player1Wins;
}

int Scoreboard::getPlayer2Wins() const
{
    return player2Wins;
}

int Scoreboard::getDraws() const
{
    return draws;
}

int Scoreboard::getTotalGames() const
{
    return player1Wins + player2Wins + draws;
}

void Scoreboard::recordPlayer1Win()
{
    ++player1Wins;
}

void Scoreboard::recordPlayer2Win()
{
    ++player2Wins;
}

void Scoreboard::recordDraw()
{
    ++draws;
}

void Scoreboard::reset()
{
    player1Wins = 0;
    player2Wins = 0;
    draws = 0;
}

} // namespace game::app
