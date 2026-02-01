#ifndef GAME_CORE_AGENTS_RANDOMAGENT_HPP
#define GAME_CORE_AGENTS_RANDOMAGENT_HPP

/// \file RandomAgent.hpp
/// \brief Random strategy AI agent

#include "core/Agent.hpp"

#include <optional>
#include <random>

namespace game::core
{

/// \brief AI agent that selects moves at random
///
/// Implements the simplest possible strategy: randomly select any available cell.
/// Useful for testing and as a trivial opponent.
///
/// The random number generator can optionally be seeded for reproducible behavior
/// in tests.
class RandomAgent : public Agent
{
public:
    /// \brief Construct a RandomAgent with a random seed
    RandomAgent();

    /// \brief Construct a RandomAgent with a specific seed
    /// \param seed The seed for the random number generator
    explicit RandomAgent(unsigned int seed);

    /// \brief Calculate the next move by randomly selecting an available position
    /// \param board The current board state
    /// \param marker The marker the agent is playing as (unused, but required by interface)
    /// \return A randomly selected available position
    /// \throws std::runtime_error if the board is full (no available moves)
    Position calculateNextMove(const Board& board, Marker marker) override;

private:
    std::mt19937 rng;
};

} // namespace game::core

#endif // GAME_CORE_AGENTS_RANDOMAGENT_HPP
