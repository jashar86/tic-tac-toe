#include "presentation/ncurses/screens/GameBoardScreen.hpp"
#include "presentation/ncurses/BoardRenderer.hpp"
#include "presentation/ncurses/Colors.hpp"

#include <ncurses.h>

namespace game::view
{

GameBoardScreen::GameBoardScreen(const core::Board& board, core::Marker marker,
                                   std::string playerName, const app::Scoreboard* scoreboard)
    : m_board(board)
    , m_marker(marker)
    , m_playerName(std::move(playerName))
    , m_scoreboard(scoreboard)
    , m_selectedCell(findFirstEmptyCell())
{
}

int GameBoardScreen::findFirstEmptyCell() const
{
    for (int i = 0; i < 9; ++i)
    {
        if (m_board.isCellEmpty(core::Position{i}))
        {
            return i;
        }
    }
    return 0;
}

void GameBoardScreen::draw()
{
    erase();
    BoardRenderer::drawTitle();
    BoardRenderer::drawBoard(m_board, m_selectedCell);
    BoardRenderer::drawScoreboardCompact(m_scoreboard);

    std::string markerStr = (m_marker == core::Marker::X) ? "X" : "O";
    std::string status = m_playerName + " (" + markerStr +
                         ") - Arrows/WASD: move | Enter/Space: place | 1-9: direct | Q: quit";
    BoardRenderer::drawStatusBar(status);

    refresh();
}

ScreenResult<app::ContinuationResult> GameBoardScreen::handleInput()
{
    int ch = getch();

    switch (ch)
    {
        case 'q':
        case 'Q':
            return std::unexpected(QuitRequested{});

        case '1': case '2': case '3':
        case '4': case '5': case '6':
        case '7': case '8': case '9':
        {
            int pos = ch - '1';
            if (m_board.isCellEmpty(core::Position{pos}))
            {
                m_selectedPosition = core::Position{pos};
                m_positionSelected = true;
                return app::ContinuationResult::QUIT;
            }
            BoardRenderer::drawStatusBar("Cell is already occupied!");
            refresh();
            napms(600);
            break;
        }

        case KEY_UP:
        case 'w':
        case 'W':
            m_selectedCell = (m_selectedCell < 3)
                ? m_selectedCell + 6
                : m_selectedCell - 3;
            break;

        case KEY_DOWN:
        case 's':
        case 'S':
            m_selectedCell = (m_selectedCell >= 6)
                ? m_selectedCell - 6
                : m_selectedCell + 3;
            break;

        case KEY_LEFT:
        case 'a':
        case 'A':
        {
            int row = m_selectedCell / 3;
            int col = m_selectedCell % 3;
            col = (col == 0) ? 2 : col - 1;
            m_selectedCell = row * 3 + col;
            break;
        }

        case KEY_RIGHT:
        case 'd':
        case 'D':
        {
            int row = m_selectedCell / 3;
            int col = m_selectedCell % 3;
            col = (col == 2) ? 0 : col + 1;
            m_selectedCell = row * 3 + col;
            break;
        }

        case '\n':
        case ' ':
        case KEY_ENTER:
        {
            if (m_board.isCellEmpty(core::Position{m_selectedCell}))
            {
                m_selectedPosition = core::Position{m_selectedCell};
                m_positionSelected = true;
                return app::ContinuationResult::QUIT;
            }
            BoardRenderer::drawStatusBar("Cell is already occupied!");
            refresh();
            napms(600);
            break;
        }

        default:
            break;
    }

    return app::ContinuationResult::CONTINUE;
}

ScreenResult<core::Position> GameBoardScreen::run()
{
    while (true)
    {
        draw();
        auto result = handleInput();
        if (!result.has_value())
        {
            return std::unexpected(QuitRequested{});
        }
        if (m_positionSelected)
        {
            return m_selectedPosition;
        }
    }
}

} // namespace game::view
