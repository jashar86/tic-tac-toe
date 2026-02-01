#include "core/agents/MinmaxAgent.hpp"

#include "core/GameLogic.hpp"
#include "core/Marker.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace game::core
{

namespace
{

/// \brief Evaluate the board from the perspective of the given marker
/// \return +10 if marker wins, -10 if opponent wins, 0 for draw/in-progress
int evaluate(const Board& board, Marker marker)
{
    GameStatus status = checkGameStatus(board);

    if (status == GameStatus::XWins)
    {
        return (marker == Marker::X) ? 10 : -10;
    }
    if (status == GameStatus::OWins)
    {
        return (marker == Marker::O) ? 10 : -10;
    }
    return 0; // Draw or in progress
}

/// \brief Minimax algorithm with alpha-beta pruning
/// \param board Current board state
/// \param marker The marker we're optimizing for
/// \param isMaximizing True if this is the maximizing player's turn
/// \param currentMarker The marker for the current turn
/// \param depth Current depth (used for preferring faster wins)
/// \return The score of the best move
int minimax(const Board& board, Marker marker, bool isMaximizing,
            Marker currentMarker, int depth)
{
    GameStatus status = checkGameStatus(board);

    // Terminal state check
    if (status != GameStatus::InProgress)
    {
        int score = evaluate(board, marker);
        // Prefer faster wins (subtract depth) and slower losses (add depth)
        if (score > 0)
            return score - depth;
        if (score < 0)
            return score + depth;
        return 0;
    }

    auto available = board.availablePositions();

    if (isMaximizing)
    {
        int bestScore = std::numeric_limits<int>::min();
        for (const auto& pos : available)
        {
            Board newBoard = board.withMove(pos, currentMarker);
            int score = minimax(newBoard, marker, false,
                               opponentOf(currentMarker), depth + 1);
            bestScore = std::max(bestScore, score);
        }
        return bestScore;
    }
    else
    {
        int bestScore = std::numeric_limits<int>::max();
        for (const auto& pos : available)
        {
            Board newBoard = board.withMove(pos, currentMarker);
            int score = minimax(newBoard, marker, true,
                               opponentOf(currentMarker), depth + 1);
            bestScore = std::min(bestScore, score);
        }
        return bestScore;
    }
}

} // anonymous namespace

Position MinmaxAgent::calculateNextMove(const Board& board, Marker marker)
{
    auto available = board.availablePositions();

    if (available.empty())
    {
        throw std::runtime_error("No available positions on the board");
    }

    Position bestMove = available[0];
    int bestScore = std::numeric_limits<int>::min();

    for (const auto& pos : available)
    {
        Board newBoard = board.withMove(pos, marker);
        // After our move, it's the opponent's turn (minimizing)
        int score = minimax(newBoard, marker, false, opponentOf(marker), 1);

        if (score > bestScore)
        {
            bestScore = score;
            bestMove = pos;
        }
    }

    return bestMove;
}

} // namespace game::core
