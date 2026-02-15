#include "presentation/Ncurses.hpp"
#include "presentation/ncurses/BoardRenderer.hpp"
#include "presentation/ncurses/Colors.hpp"
#include "presentation/ncurses/screens/GameBoardScreen.hpp"
#include "presentation/ncurses/screens/GameResultScreen.hpp"
#include "presentation/ncurses/screens/GameStartScreen.hpp"
#include "presentation/ncurses/screens/PlayerSelectionScreen.hpp"
#include "application/AgentPlayer.hpp"
#include "application/Session.hpp"

#include <ncurses.h>
#include <memory>
#include <string>
#include <utility>

namespace game::view
{

// ============================================================================
// NcursesContext - RAII for ncurses init/teardown
// ============================================================================

NcursesContext::NcursesContext()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors())
    {
        start_color();
        use_default_colors();

        // Core game colors
        init_pair(colors::MARKER_X,    COLOR_RED,     -1);
        init_pair(colors::MARKER_O,    COLOR_BLUE,    -1);
        init_pair(colors::EMPTY_CELL,  COLOR_WHITE,   -1);
        init_pair(colors::BOARD_LINES, COLOR_WHITE,   -1);

        // UI element colors
        init_pair(colors::TITLE,       COLOR_CYAN,    -1);
        init_pair(colors::STATUS,      COLOR_YELLOW,  -1);
        init_pair(colors::HIGHLIGHT,   COLOR_BLACK,   COLOR_WHITE);
        init_pair(colors::SCORE,       COLOR_GREEN,   -1);

        // Additional UI colors
        init_pair(colors::ERROR,       COLOR_MAGENTA, -1);
        init_pair(colors::SUCCESS,     COLOR_GREEN,   -1);
        init_pair(colors::MENU_ACTIVE, COLOR_CYAN,    -1);
        init_pair(colors::DIM,         COLOR_WHITE,   -1);
    }
}

NcursesContext::~NcursesContext()
{
    endwin();
}

// ============================================================================
// NcursesPlayer
// ============================================================================

NcursesPlayer::NcursesPlayer(std::string name)
    : m_name(std::move(name))
{
}

std::expected<core::Position, app::Quit>
NcursesPlayer::generateNextMove(const core::Board& board, core::Marker marker)
{
    if (board.isFull())
    {
        return std::unexpected(app::Quit{});
    }

    GameBoardScreen screen(board, marker, m_name, m_scoreboard);
    auto result = screen.run();
    if (!result.has_value())
    {
        return std::unexpected(app::Quit{});
    }
    return result.value();
}

// ============================================================================
// NcursesGameStartListener
// ============================================================================

app::ContinuationResult NcursesGameStartListener::onGameStarted(const app::Session& session)
{
    GameStartScreen screen(session);
    screen.draw();
    auto result = screen.handleInput();
    if (!result.has_value())
    {
        return app::ContinuationResult::QUIT;
    }
    return result.value();
}

// ============================================================================
// NcursesGameFinishedListener
// ============================================================================

app::ContinuationResult NcursesGameFinishedListener::onGameFinished(const app::Session& session)
{
    GameResultScreen screen(session);
    screen.draw();
    auto result = screen.handleInput();
    return result.value_or(app::ContinuationResult::QUIT);
}

// ============================================================================
// NcursesSessionGenerator
// ============================================================================

namespace
{

std::shared_ptr<app::Player>
createPlayer(PlayerType type, int playerNumber, const app::Scoreboard* scoreboard)
{
    std::string suffix = " (P" + std::to_string(playerNumber) + ")";

    switch (type)
    {
        case PlayerType::Human:
        {
            auto player = std::make_shared<NcursesPlayer>(
                "Player " + std::to_string(playerNumber));
            player->setScoreboard(scoreboard);
            return player;
        }
        case PlayerType::CpuEasy:
            return app::createAgentPlayer(
                "CPU Easy" + suffix, app::AgentDifficulty::Easy);
        case PlayerType::CpuHard:
            return app::createAgentPlayer(
                "CPU Hard" + suffix, app::AgentDifficulty::Hard);
    }
    std::unreachable();
}

} // anonymous namespace

std::expected<std::unique_ptr<app::Session>, app::Quit>
NcursesSessionGenerator::startNewSession()
{
    if (!BoardRenderer::checkMinimumTerminalSize())
    {
        return std::unexpected(app::Quit{});
    }

    PlayerSelectionScreen p1Screen(1, "X");
    auto p1Type = p1Screen.run();
    if (!p1Type.has_value())
    {
        return std::unexpected(app::Quit{});
    }

    PlayerSelectionScreen p2Screen(2, "O");
    auto p2Type = p2Screen.run();
    if (!p2Type.has_value())
    {
        return std::unexpected(app::Quit{});
    }

    auto p1 = createPlayer(p1Type.value(), 1, nullptr);
    auto p2 = createPlayer(p2Type.value(), 2, nullptr);
    auto session = std::make_unique<app::Session>(p1, p2);

    if (auto* ncP1 = dynamic_cast<NcursesPlayer*>(p1.get()))
    {
        ncP1->setScoreboard(&session->getScoreboard());
    }
    if (auto* ncP2 = dynamic_cast<NcursesPlayer*>(p2.get()))
    {
        ncP2->setScoreboard(&session->getScoreboard());
    }

    return session;
}

} // end namespace game::view
