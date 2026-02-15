#include "presentation/ncurses/screens/GameResultScreen.hpp"
#include "presentation/ncurses/BoardRenderer.hpp"
#include "presentation/ncurses/Colors.hpp"
#include "core/GameStatus.hpp"

#include <ncurses.h>
#include <string>

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

    const auto& board = m_session.getGameState().getBoard();
    auto status = m_session.getGameState().getStatus();
    auto winningCells = BoardRenderer::getWinningCells(board, status);

    // Draw board with winning cells highlighted
    if (winningCells.has_value())
    {
        BoardRenderer::drawBoard(board, -1, &winningCells.value());
    }
    else
    {
        BoardRenderer::drawBoard(board);
    }

    int boardRow = BoardRenderer::getBoardStartRow();
    int resultRow = boardRow + BoardRenderer::BOARD_HEIGHT + 1;
    int centerCol = COLS / 2;

    std::string resultMsg;
    int colorPair = colors::STATUS;

    switch (status)
    {
        case core::GameStatus::XWins:
            resultMsg = m_session.getPlayer1()->getName() + " (X) WINS!";
            colorPair = colors::MARKER_X;
            break;
        case core::GameStatus::OWins:
            resultMsg = m_session.getPlayer2()->getName() + " (O) WINS!";
            colorPair = colors::MARKER_O;
            break;
        case core::GameStatus::Draw:
            resultMsg = "It's a DRAW!";
            colorPair = colors::STATUS;
            break;
        default:
            break;
    }

    // Display result message centered
    attron(COLOR_PAIR(colorPair) | A_BOLD);
    mvprintw(resultRow, centerCol - static_cast<int>(resultMsg.size()) / 2,
             "%s", resultMsg.c_str());
    attroff(COLOR_PAIR(colorPair) | A_BOLD);

    // Scoreboard
    BoardRenderer::drawScoreboard(m_session);

    // Play again prompt
    int promptRow = resultRow + 6;
    attron(COLOR_PAIR(colors::STATUS));
    const char* prompt = "Play again? (Y)es / (N)o";
    mvprintw(promptRow, centerCol - 12, "%s", prompt);
    attroff(COLOR_PAIR(colors::STATUS));

    refresh();

    // Animate winning line if there is one
    if (winningCells.has_value())
    {
        BoardRenderer::highlightWinningLine(board, winningCells.value());
    }
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
