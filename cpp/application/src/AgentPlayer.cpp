#include "application/AgentPlayer.hpp"
#include "core/agents/MinmaxAgent.hpp"
#include "core/agents/RandomAgent.hpp"

#include <thread>

namespace game::app
{

AgentPlayer::AgentPlayer(std::string name, std::unique_ptr<core::Agent> agent,
                         std::chrono::milliseconds thinkDelay)
    : m_name(std::move(name))
    , m_agent(std::move(agent))
    , m_thinkDelay(thinkDelay)
{
}

std::expected<core::Position, Quit>
AgentPlayer::generateNextMove(const core::Board& board, core::Marker marker)
{
    if (m_thinkDelay.count() > 0)
    {
        std::this_thread::sleep_for(m_thinkDelay);
    }
    return m_agent->calculateNextMove(board, marker);
}

std::shared_ptr<Player> createAgentPlayer(
    std::string name, AgentDifficulty difficulty,
    std::chrono::milliseconds thinkDelay)
{
    std::unique_ptr<core::Agent> agent;
    switch (difficulty)
    {
        case AgentDifficulty::Easy:
            agent = std::make_unique<core::RandomAgent>();
            break;
        case AgentDifficulty::Hard:
            agent = std::make_unique<core::MinmaxAgent>();
            break;
    }
    return std::make_shared<AgentPlayer>(std::move(name), std::move(agent), thinkDelay);
}

} // namespace game::app
