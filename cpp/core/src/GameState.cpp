#include "core/GameState.hpp"

namespace game::core
{

GameState::GameState()
    : mBoard()
    , mTurn(Marker::X)
    , mStatus(GameStatus::InProgress)
{
}

GameState::GameState(const Board& board, Marker turn, GameStatus status)
    : mBoard(board)
    , mTurn(turn)
    , mStatus(status)
{
}

const Board& GameState::board() const { return mBoard; }

Marker GameState::currentTurn() const { return mTurn; }

GameStatus GameState::status() const { return mStatus; }

} // namespace game::core
