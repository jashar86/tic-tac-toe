#ifndef GAME_CORE_GAMELOGIC_HPP
#define GAME_CORE_GAMELOGIC_HPP

#include <expected>

#include "core/GameState.hpp"
#include "core/GameStatus.hpp"
#include "core/Board.hpp"

namespace game::core
{

/// \brief Enumeration of errors that arise while taking a turn 
enum class TurnError
{
    POSITION_OUT_OF_BOUNDS, ///< Position value is out of bounds [0, 8]
    CELL_IS_TAKEN,          ///< Chosen position is already occupied
    GAME_NOT_IN_PROGRESS    ///< Trying to take a turn after the game has finished
};

/// \brief Provides the next game state after making the move.  
/// \param prior - Game state before taking the turn
/// \param position - Board position for the turn
/// \return Updated state or TurnError if problem is found
std::expected<GameState, TurnError> takeTurn(GameState const& prior, Position const& position);

/// \brief Returns true if the given position is a valid move to make 
///        for the given board 
/// \param board - used to check the move
/// \param position - position to check
/// \return true if position is valid and the cell is empty on the board
bool isValidMove(Board const& board, Position const& position);

/// \brief Determines the status of the game given the current pieces on the board
/// \param board - the game board representing all of the moves
GameStatus checkGameStatus(Board const& board);

}

#endif /// GAME_CORE_GAMELOGIC_HPP