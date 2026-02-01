#include "core/GameLogic.hpp"

namespace game::core
{

namespace
{

/// \brief indices for each set of 3 cells that wins the game 
constexpr int WINNING_LINES[8][3] = {
    {0, 1, 2}, // top row
    {3, 4, 5}, // middle row
    {6, 7, 8}, // bottom row
    {0, 4, 8}, // left cross
    {2, 4, 6}, // right cross
    {0, 3, 6}, // left column
    {1, 4, 7}, // middle column
    {2, 5, 8}  // right column
};

/// \brief Returns if a cell in the board matches the given marker
/// \param board - where to look up cell value
/// \param index - cell index to look up value
/// \param marker - value to compare against
/// \return true if the given marker is at the index in the board, false otherwise
inline bool matches(Board const& board, int index, Marker const& marker)
{
    const auto cell_value = board.getMarker(Position{index});
    if (cell_value)
    {
        return cell_value.value() == marker;
    }
    else 
    {
        return false;
    }
}

/// \brief Checks if there is a winning line in the board for the given marker
/// \param board - current game board to check
/// \param marker - marker to check
/// \return true if any 3 cells in a row have the given marker, false otherwise
bool hasThreeInARow(Board const& board, Marker const& marker)
{
    for (auto const& line : WINNING_LINES)
    {
        if (matches(board, line[0], marker)
            && matches(board, line[1], marker)
            && matches(board, line[2], marker))
        {
            return true;
        }
    }

    return false;
}

} // end namespace

std::expected<GameState, TurnError> takeTurn(GameState const& prior, Position const& position)
{
    if(!Position::isValidIndex(position.asIndex()))
    {
        return std::unexpected(TurnError::POSITION_OUT_OF_BOUNDS);
    }

    if (isGameOver(prior.getStatus()))
    {
        return std::unexpected(TurnError::GAME_NOT_IN_PROGRESS);
    }

    if (prior.getBoard().getMarker(position).has_value())
    {
        return std::unexpected(TurnError::CELL_IS_TAKEN);
    }

    const auto next   = prior.getBoard().withMove(position, prior.getCurrentTurn());
    const auto turn   = opponentOf(prior.getCurrentTurn());
    const auto status = checkGameStatus(next);
    return GameState{next, turn, status};
}

bool isValidMove(Board const& board, Position const& position)
{
    if (!Position::isValidIndex(position.asIndex()))
    {
        return false;
    }
    return board.isCellEmpty(position);
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

} // end namespace game::core