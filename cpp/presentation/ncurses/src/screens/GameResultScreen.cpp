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
    // Brief pause before showing result (builds anticipation)
    napms(400);

    // Screen wipe transition
    BoardRenderer::screenWipe(150);

    const auto& board = m_session.getGameState().getBoard();
    auto status = m_session.getGameState().getStatus();
    auto winningCells = BoardRenderer::getWinningCells(board, status);

    // Show celebration banner first
    if (status == core::GameStatus::XWins)
    {
        BoardRenderer::drawWinnerBanner(core::Marker::X);
        refresh();
        BoardRenderer::runConfettiAnimation(1200);
    }
    else if (status == core::GameStatus::OWins)
    {
        BoardRenderer::drawWinnerBanner(core::Marker::O);
        refresh();
        BoardRenderer::runConfettiAnimation(1200);
    }
    else if (status == core::GameStatus::Draw)
    {
        BoardRenderer::drawDrawBanner();
        refresh();
        napms(800);
    }

    // Clear and show final result screen
    clear();
    BoardRenderer::drawTitle();

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
    int resultRow = boardRow + BoardRenderer::getBoardHeight() + 1;
    int centerCol = COLS / 2;

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
