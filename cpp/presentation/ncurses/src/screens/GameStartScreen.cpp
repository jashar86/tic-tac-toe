#include "presentation/ncurses/screens/GameStartScreen.hpp"
#include "presentation/ncurses/BoardRenderer.hpp"
#include "presentation/ncurses/Colors.hpp"

#include <ncurses.h>
#include <string>

namespace game::view
{

GameStartScreen::GameStartScreen(const app::Session& session)
    : m_session(session)
{
}

void GameStartScreen::draw()
{
    clear();
    BoardRenderer::drawTitle();

    int centerRow = LINES / 2;
    int centerCol = COLS / 2;

    attron(COLOR_PAIR(colors::TITLE) | A_BOLD);
    std::string msg = "Game " + std::to_string(m_session.getScoreboard().getTotalGames() + 1);
    mvprintw(centerRow - 1, centerCol - static_cast<int>(msg.size()) / 2, "%s", msg.c_str());

    std::string p1 = m_session.getPlayer1()->getName() + " (X)";
    std::string p2 = m_session.getPlayer2()->getName() + " (O)";
    std::string vs = p1 + "  vs  " + p2;
    mvprintw(centerRow + 1, centerCol - static_cast<int>(vs.size()) / 2, "%s", vs.c_str());
    attroff(COLOR_PAIR(colors::TITLE) | A_BOLD);

    attron(COLOR_PAIR(colors::STATUS));
    mvprintw(centerRow + 3, centerCol - 14, "Press any key to start (Q to quit)");
    attroff(COLOR_PAIR(colors::STATUS));

    refresh();
}

ScreenResult<app::ContinuationResult> GameStartScreen::handleInput()
{
    int ch = getch();
    if (ch == 'q' || ch == 'Q')
    {
        return std::unexpected(QuitRequested{});
    }
    return app::ContinuationResult::CONTINUE;
}

} // namespace game::view
