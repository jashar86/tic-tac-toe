#include "presentation/ncurses/screens/GameResultScreen.hpp"
#include "presentation/ncurses/BoardRenderer.hpp"
#include "presentation/ncurses/Colors.hpp"
#include "core/GameStatus.hpp"

#include <ncurses.h>

namespace game::view
{

GameResultScreen::GameResultScreen(const app::Session& session)
    : m_session(session)
{
}

void GameResultScreen::draw()
{
    clear();
    BoardRenderer::drawTitle();
    BoardRenderer::drawBoard(m_session.getGameState().getBoard());

    int resultRow = BoardRenderer::BOARD_START_ROW + 3 * BoardRenderer::CELL_HEIGHT + 2;

    switch (m_session.getGameState().getStatus())
    {
        case core::GameStatus::XWins:
            attron(COLOR_PAIR(colors::MARKER_X) | A_BOLD);
            mvprintw(resultRow, BoardRenderer::BOARD_START_COL,
                     "%s (X) WINS!", m_session.getPlayer1()->getName().c_str());
            attroff(COLOR_PAIR(colors::MARKER_X) | A_BOLD);
            break;
        case core::GameStatus::OWins:
            attron(COLOR_PAIR(colors::MARKER_O) | A_BOLD);
            mvprintw(resultRow, BoardRenderer::BOARD_START_COL,
                     "%s (O) WINS!", m_session.getPlayer2()->getName().c_str());
            attroff(COLOR_PAIR(colors::MARKER_O) | A_BOLD);
            break;
        case core::GameStatus::Draw:
            attron(COLOR_PAIR(colors::STATUS) | A_BOLD);
            mvprintw(resultRow, BoardRenderer::BOARD_START_COL, "It's a DRAW!");
            attroff(COLOR_PAIR(colors::STATUS) | A_BOLD);
            break;
        default:
            break;
    }

    BoardRenderer::drawScoreboard(m_session);

    int promptRow = resultRow + 4;
    attron(COLOR_PAIR(colors::STATUS));
    mvprintw(promptRow, BoardRenderer::BOARD_START_COL,
             "Play again? (Y)es / (N)o");
    attroff(COLOR_PAIR(colors::STATUS));

    refresh();
}

ScreenResult<app::ContinuationResult> GameResultScreen::handleInput()
{
    while (true)
    {
        int ch = getch();
        if (ch == 'y' || ch == 'Y')
        {
            return app::ContinuationResult::CONTINUE;
        }
        else if (ch == 'n' || ch == 'N' || ch == 'q' || ch == 'Q')
        {
            return app::ContinuationResult::QUIT;
        }
    }
}

} // namespace game::view
