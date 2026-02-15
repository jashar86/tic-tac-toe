#ifndef GAME_VIEW_NCURSES_HPP
#define GAME_VIEW_NCURSES_HPP

#include "application/ContinuationResult.hpp"
#include "application/GameFinishedListener.hpp"
#include "application/GameStartListener.hpp"
#include "application/Player.hpp"
#include "application/Scoreboard.hpp"
#include "application/SessionGenerator.hpp"

#include <string>

namespace game::view
{

/// \brief RAII wrapper for ncurses initialization and teardown
class NcursesContext
{
public:
    NcursesContext();
    ~NcursesContext();

    NcursesContext(const NcursesContext&) = delete;
    NcursesContext& operator=(const NcursesContext&) = delete;
    NcursesContext(NcursesContext&&) = delete;
    NcursesContext& operator=(NcursesContext&&) = delete;
};

class NcursesPlayer : public game::app::Player
{
public:
    explicit NcursesPlayer(std::string name);

    ~NcursesPlayer() override = default;

    [[nodiscard]] std::string getName() const override { return m_name; }

    [[nodiscard]] std::expected<core::Position, app::Quit> generateNextMove(
        const core::Board& board, core::Marker marker) override;

    void setScoreboard(const app::Scoreboard* scoreboard) { m_scoreboard = scoreboard; }

private:
    std::string m_name;
    const app::Scoreboard* m_scoreboard{nullptr};
};

class NcursesGameStartListener : public app::GameStartListener
{
public:
    ~NcursesGameStartListener() noexcept override = default;

    app::ContinuationResult onGameStarted(const app::Session& session) override;
};

class NcursesGameFinishedListener : public app::GameFinishedListener
{
public:
    ~NcursesGameFinishedListener() noexcept override = default;

    app::ContinuationResult onGameFinished(const app::Session& session) override;
};

class NcursesSessionGenerator : public app::SessionGenerator
{
public:
    ~NcursesSessionGenerator() noexcept override = default;

    std::expected<std::unique_ptr<app::Session>, app::Quit> startNewSession() override;
};

} // namespace game::view

#endif // GAME_VIEW_NCURSES_HPP
