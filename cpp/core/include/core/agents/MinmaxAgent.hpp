#ifndef GAME_CORE_AGENTS_MINMAXAGENT_HPP
#define GAME_CORE_AGENTS_MINMAXAGENT_HPP

/// \file MinmaxAgent.hpp
/// \brief Optimal play AI agent using minimax algorithm

#include "core/Agent.hpp"

namespace game::core
{

/// \brief AI agent that plays optimally using the minimax algorithm
///
/// Implements the minimax algorithm to find the optimal move. This agent
/// is unbeatable - it will always win or draw, never lose.
class MinmaxAgent : public Agent
{
public:
    /// \brief Calculate the optimal move using minimax
    /// \param board The current board state
    /// \param marker The marker the agent is playing as
    /// \return The optimal position to play
    Position calculateNextMove(const Board& board, Marker marker) override;
};

} // namespace game::core

#endif // GAME_CORE_AGENTS_MINMAXAGENT_HPP
