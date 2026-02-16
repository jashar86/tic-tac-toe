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
    // Brief transition wipe
    BoardRenderer::screenWipe(150);

    clear();
    BoardRenderer::drawTitle();

    int centerRow = LINES / 2;
    int centerCol = COLS / 2;
    int gameNum = m_session.getScoreboard().getTotalGames() + 1;

    // Game number with decorative border
    std::string gameLabel = "GAME " + std::to_string(gameNum);
    int boxWidth = static_cast<int>(gameLabel.size()) + 8;
    int boxLeft = centerCol - boxWidth / 2;

    attron(COLOR_PAIR(colors::SCORE) | A_BOLD);
    mvaddch(centerRow - 3, boxLeft, ACS_ULCORNER);
    for (int i = 1; i < boxWidth - 1; ++i) addch(ACS_HLINE);
    addch(ACS_URCORNER);
    mvaddch(centerRow - 2, boxLeft, ACS_VLINE);
    mvprintw(centerRow - 2, centerCol - static_cast<int>(gameLabel.size()) / 2,
             "%s", gameLabel.c_str());
    mvaddch(centerRow - 2, boxLeft + boxWidth - 1, ACS_VLINE);
    mvaddch(centerRow - 1, boxLeft, ACS_LLCORNER);
    for (int i = 1; i < boxWidth - 1; ++i) addch(ACS_HLINE);
    addch(ACS_LRCORNER);
    attroff(COLOR_PAIR(colors::SCORE) | A_BOLD);

    // Player matchup
    std::string p1 = m_session.getPlayer1()->getName();
    std::string p2 = m_session.getPlayer2()->getName();

    // Player 1 (X) - left aligned from center
    attron(COLOR_PAIR(colors::MARKER_X) | A_BOLD);
    mvprintw(centerRow + 1, centerCol - static_cast<int>(p1.size()) - 6,
             "%s (X)", p1.c_str());
    attroff(COLOR_PAIR(colors::MARKER_X) | A_BOLD);

    // VS
    attron(COLOR_PAIR(colors::TITLE) | A_BOLD);
    mvprintw(centerRow + 1, centerCol - 2, " vs ");
    attroff(COLOR_PAIR(colors::TITLE) | A_BOLD);

    // Player 2 (O) - right aligned from center
    attron(COLOR_PAIR(colors::MARKER_O) | A_BOLD);
    mvprintw(centerRow + 1, centerCol + 2, "%s (O)", p2.c_str());
    attroff(COLOR_PAIR(colors::MARKER_O) | A_BOLD);

    // Current score summary if not first game
    if (gameNum > 1)
    {
        const auto& sb = m_session.getScoreboard();
        attron(COLOR_PAIR(colors::STATUS) | A_DIM);
        std::string scoreMsg = "Current: " + p1 + " " + std::to_string(sb.getPlayer1Wins()) +
                               " - " + std::to_string(sb.getPlayer2Wins()) + " " + p2;
        if (sb.getDraws() > 0)
        {
            scoreMsg += " (Draws: " + std::to_string(sb.getDraws()) + ")";
        }
        mvprintw(centerRow + 3, centerCol - static_cast<int>(scoreMsg.size()) / 2,
                 "%s", scoreMsg.c_str());
        attroff(COLOR_PAIR(colors::STATUS) | A_DIM);
    }

    // Start prompt
    attron(COLOR_PAIR(colors::STATUS));
    const char* prompt = "Press any key to start (Q to quit)";
    mvprintw(centerRow + 5, centerCol - 17, "%s", prompt);
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
