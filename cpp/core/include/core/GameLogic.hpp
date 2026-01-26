#ifndef GAME_CORE_GAMELOGIC_HPP
#define GAME_CORE_GAMELOGIC_HPP

#include "core/GameStatus.hpp"
#include "core/Board.hpp"

namespace game::core
{

/// \brief Determines the status of the game given the current pieces on the board
/// \param board - the game board representing all of the moves
GameStatus checkGameStatus(Board const& board);

}

#endif /// GAME_CORE_GAMELOGIC_HPP