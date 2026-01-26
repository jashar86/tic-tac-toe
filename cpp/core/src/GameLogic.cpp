#include "core/GameLogic.hpp"

namespace game::core
{

namespace
{
    bool hasThreeInARow(Board const& board, Marker const& marker)
    {
        return false;
    }

}

GameStatus checkGameStatus(Board const& board)
{
    if (hasThreeInARow(board, Marker::X))
    {
        return GameStatus::XWins;
    } 
    else if (hasThreeInARow(board, Marker::O))
    {
        return GameStatus::OWins;
    }
    else if (board.isFull())
    {
        return GameStatus::Draw;
    }
    return GameStatus::InProgress;
}

}