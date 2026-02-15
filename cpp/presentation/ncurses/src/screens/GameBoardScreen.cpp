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

    // Draw turn indicator panel
    int boardCol = BoardRenderer::getBoardStartCol();
    int boardRow = BoardRenderer::getBoardStartRow();
    int panelWidth = 20;
    int panelLeft = boardCol + BoardRenderer::BOARD_WIDTH + 3;

    // Only draw side panel if there's room
    if (panelLeft + panelWidth < COLS)
    {
        int colorPair = (m_marker == core::Marker::X) ? colors::MARKER_X : colors::MARKER_O;
        std::string markerStr = (m_marker == core::Marker::X) ? "X" : "O";

        // Panel border
        attron(COLOR_PAIR(colorPair));
        mvaddch(boardRow, panelLeft, ACS_ULCORNER);
        for (int i = 1; i < panelWidth - 1; ++i) addch(ACS_HLINE);
        addch(ACS_URCORNER);

        for (int row = 1; row < 6; ++row)
        {
            mvaddch(boardRow + row, panelLeft, ACS_VLINE);
            mvaddch(boardRow + row, panelLeft + panelWidth - 1, ACS_VLINE);
        }

        mvaddch(boardRow + 6, panelLeft, ACS_LLCORNER);
        for (int i = 1; i < panelWidth - 1; ++i) addch(ACS_HLINE);
        addch(ACS_LRCORNER);

        // YOUR TURN header
        attron(A_BOLD | A_REVERSE);
        mvprintw(boardRow + 1, panelLeft + 2, "   YOUR TURN   ");
        attroff(A_REVERSE);

        // Player name
        mvprintw(boardRow + 3, panelLeft + 2, "%-16s", m_playerName.c_str());

        // Large marker indicator
        attroff(COLOR_PAIR(colorPair));
        attron(COLOR_PAIR(colorPair) | A_BOLD);
        mvprintw(boardRow + 5, panelLeft + 7, "( %s )", markerStr.c_str());
        attroff(COLOR_PAIR(colorPair) | A_BOLD);
    }

    // Draw scoreboard
    BoardRenderer::drawScoreboardCompact(m_scoreboard);

    // Status bar with controls
    std::string status = "Arrows/WASD: move | Enter/Space: place | 1-9: direct | Q: quit";
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
            // Flash the occupied cell to show it's taken
            BoardRenderer::flashCell(pos, m_board, 2);
            beep();
            BoardRenderer::drawStatusBar("That cell is taken! Choose another.");
            refresh();
            napms(300);
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
            // Flash the occupied cell to show it's taken
            BoardRenderer::flashCell(m_selectedCell, m_board, 2);
            beep();
            BoardRenderer::drawStatusBar("That cell is taken! Choose another.");
            refresh();
            napms(300);
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
