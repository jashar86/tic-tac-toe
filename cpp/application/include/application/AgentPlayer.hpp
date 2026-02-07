#ifndef GAME_APP_AGENT_PLAYER_HPP
#define GAME_APP_AGENT_PLAYER_HPP

#include "application/Player.hpp"

#include <chrono>
#include <memory>
#include <string>

#include "core/Agent.hpp"

namespace game::app
{

/// \brief Adapts a core::Agent to the Player interface
///
/// Wraps an AI agent so it can participate in games through the same
/// Player interface used by human players. The agent's calculateNextMove
/// is delegated to and the result is wrapped in std::expected (agents
/// never quit, so the result is always a valid Position).
///
/// An optional think delay makes CPU moves visible to human observers.
class AgentPlayer : public Player
{
public:
    AgentPlayer(std::string name, std::unique_ptr<core::Agent> agent,
                std::chrono::milliseconds thinkDelay = std::chrono::milliseconds{500});

    ~AgentPlayer() override = default;

    [[nodiscard]] std::string getName() const override { return m_name; }

    [[nodiscard]] std::expected<core::Position, Quit> generateNextMove(
        const core::Board& board, core::Marker marker) override;

private:
    std::string m_name;
    std::unique_ptr<core::Agent> m_agent;
    std::chrono::milliseconds m_thinkDelay;
};

/// \brief Difficulty levels for AI opponents
enum class AgentDifficulty
{
    Easy, ///< Random moves
    Hard  ///< Optimal minimax play
};

/// \brief Create an AgentPlayer with the given difficulty
///
/// Encapsulates the mapping from difficulty enum to concrete core::Agent type.
/// This keeps the presentation layer from depending on core agent implementations.
[[nodiscard]] std::shared_ptr<Player> createAgentPlayer(
    std::string name, AgentDifficulty difficulty,
    std::chrono::milliseconds thinkDelay = std::chrono::milliseconds{500});

} // namespace game::app

#endif // GAME_APP_AGENT_PLAYER_HPP
