#include "core/agents/RandomAgent.hpp"

#include <stdexcept>

namespace game::core
{

RandomAgent::RandomAgent()
    : rng(std::random_device{}())
{
}

RandomAgent::RandomAgent(unsigned int seed)
    : rng(seed)
{
}

Position RandomAgent::calculateNextMove(const Board& board, Marker /*marker*/)
{
    auto available = board.availablePositions();

    if (available.empty())
    {
        throw std::runtime_error("No available positions on the board");
    }

    std::uniform_int_distribution<size_t> dist(0, available.size() - 1);
    size_t index = dist(rng);

    return available[index];
}

} // namespace game::core
