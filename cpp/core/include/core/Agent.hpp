#ifndef GAME_CORE_AGENT_HPP
#define GAME_CORE_AGENT_HPP

/// \file Agent.hpp
/// \brief Agent interface for AI opponents

#include "core/Board.hpp"
#include "core/Marker.hpp"
#include "core/Position.hpp"

namespace game::core
{

/// \brief Interface for AI agents that can play tic-tac-toe
///
/// AI opponents implement this interface, enabling easy substitution of strategies.
/// Agents are stateless—given the same board and marker, they produce the same move
/// (deterministic agents) or a move from the same distribution (stochastic agents).
class Agent
{
public:
    virtual ~Agent() = default;

    /// \brief Calculate the next move for the given board state
    /// \param board The current board state
    /// \param marker The marker the agent is playing as (X or O)
    /// \return The position where the agent wants to place its marker
    virtual Position calculateNextMove(const Board& board, Marker marker) = 0;
};

} // namespace game::core

#endif // GAME_CORE_AGENT_HPP
