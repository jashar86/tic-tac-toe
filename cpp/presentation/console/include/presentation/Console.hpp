#ifndef GAME_VIEW_CONSOLE_HPP
#define GAME_VIEW_CONSOLE_HPP

#include "application/ContinuationResult.hpp"
#include "application/GameFinishedListener.hpp"
#include "application/GameStartListener.hpp"
#include "application/Player.hpp"
#include "application/SessionGenerator.hpp"

#include <stdexcept>
#include <string>

namespace game::view 
{

class ConsolePlayer : public game::app::Player
{
public:

    explicit ConsolePlayer(std::string name);

    virtual ~ConsolePlayer() = default;

    /// \brief Get the player's display name
    /// \return The player's name
    [[nodiscard]] std::string getName() const override { return m_name; }

    /// \brief Generate the next move for this player
    /// \param board The current board state
    /// \param marker The marker this player is using (X or O)
    /// \return The chosen position, or Quit if user wants to exit
    [[nodiscard]] std::expected<core::Position, app::Quit> generateNextMove(
        const core::Board& board, core::Marker marker) override;

private:

    std::string m_name; ///< Name of the player
};

class ConsoleGameStartListener : public app::GameStartListener
{
public:
    ~ConsoleGameStartListener() noexcept override = default;

    app::ContinuationResult onGameStarted(const app::Session& session) override;
};

class ConsoleGameFinishedListener : public app::GameFinishedListener
{
public:
    ~ConsoleGameFinishedListener() noexcept override = default;

    app::ContinuationResult onGameFinished(const app::Session& session) override;
};

class ConsoleSessionGenerator : public app::SessionGenerator
{
public:
    ~ConsoleSessionGenerator() noexcept override = default;
    
    std::expected<std::unique_ptr<app::Session>, app::Quit> startNewSession() override;
};

}

#endif // GAME_VIEW_CONSOLE_HPP