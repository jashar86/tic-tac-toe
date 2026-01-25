#include "core/GameState.hpp"

namespace game::core {

GameState::GameState()
    : board_()
    , turn_(Marker::X)
    , status_(GameStatus::InProgress) {}

GameState::GameState(const Board& board, Marker turn, GameStatus status)
    : board_(board)
    , turn_(turn)
    , status_(status) {}

const Board& GameState::board() const {
    return board_;
}

Marker GameState::currentTurn() const {
    return turn_;
}

GameStatus GameState::status() const {
    return status_;
}

} // namespace game::core
