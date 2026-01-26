#include "core/GameState.hpp"

namespace game::core
{

GameState::GameState()
    : board()
    , turn(Marker::X)
    , status(GameStatus::InProgress)
{
}

GameState::GameState(const Board& board, Marker turn, GameStatus status)
    : board(board)
    , turn(turn)
    , status(status)
{
}

const Board& GameState::getBoard() const
{
    return board;
}

Marker GameState::getCurrentTurn() const
{
    return turn;
}

GameStatus GameState::getStatus() const
{
    return status;
}

} // namespace game::core
