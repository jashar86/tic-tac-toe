#include "presentation/ncurses/BoardRenderer.hpp"
#include "presentation/ncurses/Colors.hpp"

#include <ncurses.h>

namespace game::view
{

bool BoardRenderer::checkMinimumTerminalSize()
{
    if (LINES < MIN_ROWS || COLS < MIN_COLS)
    {
        clear();
        mvprintw(0, 0, "Terminal too small! Need at least %dx%d, have %dx%d.",
                 MIN_COLS, MIN_ROWS, COLS, LINES);
        mvprintw(1, 0, "Please resize your terminal and press any key.");
        refresh();
        getch();
        return false;
    }
    return true;
}

void BoardRenderer::drawTitle()
{
    attron(COLOR_PAIR(colors::TITLE) | A_BOLD);
    mvprintw(1, (COLS/2) - 98/2, R"( /######## /######  /######        /######## /######   /######        /######## /######  /########)");
    mvprintw(2, (COLS/2) - 98/2, R"(|__  ##__/|_  ##_/ /##__  ##      |__  ##__//##__  ## /##__  ##      |__  ##__//##__  ##| ##_____/)");
    mvprintw(3, (COLS/2) - 98/2, R"(   | ##     | ##  | ##  \__/         | ##  | ##  \ ##| ##  \__/         | ##  | ##  \ ##| ##      )");
    mvprintw(4, (COLS/2) - 98/2, R"(   | ##     | ##  | ##               | ##  | ########| ##               | ##  | ##  | ##| #####   )");
    mvprintw(5, (COLS/2) - 98/2, R"(   | ##     | ##  | ##               | ##  | ##__  ##| ##               | ##  | ##  | ##| ##__/   )");
    mvprintw(6, (COLS/2) - 98/2, R"(   | ##     | ##  | ##    ##         | ##  | ##  | ##| ##    ##         | ##  | ##  | ##| ##      )");
    mvprintw(7, (COLS/2) - 98/2, R"(   | ##    /######|  ######/         | ##  | ##  | ##|  ######/         | ##  |  ######/| ########)");
    mvprintw(8, (COLS/2) - 98/2, R"(   |__/   |______/ \______/          |__/  |__/  |__/ \______/          |__/   \______/ |________/)");
    attroff(COLOR_PAIR(colors::TITLE) | A_BOLD);
    refresh();
}

void BoardRenderer::drawCellContent(int row, int col,
                                     std::optional<core::Marker> marker,
                                     int cellIndex, bool selected)
{
    int y = BOARD_START_ROW + row * CELL_HEIGHT + 1;
    int x = BOARD_START_COL + col * CELL_WIDTH + 2;

    if (selected)
    {
        attron(COLOR_PAIR(colors::HIGHLIGHT) | A_BOLD);
    }

    if (marker.has_value())
    {
        if (marker.value() == core::Marker::X)
        {
            if (!selected) attron(COLOR_PAIR(colors::MARKER_X) | A_BOLD);
            mvprintw(y, x, " X ");
            if (!selected) attroff(COLOR_PAIR(colors::MARKER_X) | A_BOLD);
        }
        else
        {
            if (!selected) attron(COLOR_PAIR(colors::MARKER_O) | A_BOLD);
            mvprintw(y, x, " O ");
            if (!selected) attroff(COLOR_PAIR(colors::MARKER_O) | A_BOLD);
        }
    }
    else
    {
        if (!selected) attron(COLOR_PAIR(colors::EMPTY_CELL) | A_DIM);
        mvprintw(y, x, " %d ", cellIndex + 1);
        if (!selected) attroff(COLOR_PAIR(colors::EMPTY_CELL) | A_DIM);
    }

    if (selected)
    {
        attroff(COLOR_PAIR(colors::HIGHLIGHT) | A_BOLD);
    }
}

void BoardRenderer::drawBoard(const core::Board& board, int selectedCell)
{
    // Draw horizontal lines
    attron(COLOR_PAIR(colors::BOARD_LINES));
    for (int row = 0; row < 3; ++row)
    {
        int y = BOARD_START_ROW + row * CELL_HEIGHT;
        for (int col = 0; col < 3; ++col)
        {
            int x = BOARD_START_COL + col * CELL_WIDTH;
            for (int cx = 0; cx < CELL_WIDTH; ++cx)
            {
                mvaddch(y, x + cx, ACS_HLINE);
                mvaddch(y + CELL_HEIGHT, x + cx, ACS_HLINE);
            }
            for (int cy = 0; cy <= CELL_HEIGHT; ++cy)
            {
                mvaddch(y + cy, x, ACS_VLINE);
                mvaddch(y + cy, x + CELL_WIDTH, ACS_VLINE);
            }
        }
    }

    // Draw intersection characters
    for (int row = 0; row <= 3; ++row)
    {
        for (int col = 0; col <= 3; ++col)
        {
            int y = BOARD_START_ROW + row * CELL_HEIGHT;
            int x = BOARD_START_COL + col * CELL_WIDTH;
            mvaddch(y, x, ACS_PLUS);
        }
    }
    attroff(COLOR_PAIR(colors::BOARD_LINES));

    // Draw cell contents
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            int cellIndex = row * 3 + col;
            core::Position pos{cellIndex};
            bool isSelected = (cellIndex == selectedCell);
            drawCellContent(row, col, board.getMarker(pos), cellIndex, isSelected);
        }
    }
}

void BoardRenderer::drawStatusBar(std::string_view message)
{
    int maxY = getmaxy(stdscr);
    attron(COLOR_PAIR(colors::STATUS));
    move(maxY - 2, 0);
    clrtoeol();
    mvprintw(maxY - 2, BOARD_START_COL, "%.*s",
             static_cast<int>(message.size()), message.data());
    attroff(COLOR_PAIR(colors::STATUS));
}

void BoardRenderer::drawScoreboard(const app::Session& session)
{
    int scoreRow = BOARD_START_ROW + 3 * CELL_HEIGHT + 2;

    attron(COLOR_PAIR(colors::SCORE) | A_BOLD);
    mvprintw(scoreRow, BOARD_START_COL, "SCOREBOARD");
    attroff(COLOR_PAIR(colors::SCORE) | A_BOLD);

    const auto& sb = session.getScoreboard();
    mvprintw(scoreRow + 1, BOARD_START_COL,
             "%s (X): %d    %s (O): %d    Draws: %d",
             session.getPlayer1()->getName().c_str(), sb.getPlayer1Wins(),
             session.getPlayer2()->getName().c_str(), sb.getPlayer2Wins(),
             sb.getDraws());
}

void BoardRenderer::drawScoreboardCompact(const app::Scoreboard* scoreboard)
{
    if (!scoreboard) return;

    int scoreRow = BOARD_START_ROW + 3 * CELL_HEIGHT + 2;
    attron(COLOR_PAIR(colors::SCORE) | A_BOLD);
    mvprintw(scoreRow, BOARD_START_COL, "SCORE");
    attroff(COLOR_PAIR(colors::SCORE) | A_BOLD);

    attron(COLOR_PAIR(colors::SCORE));
    mvprintw(scoreRow, BOARD_START_COL + 6,
             "P1: %d  P2: %d  Draws: %d",
             scoreboard->getPlayer1Wins(),
             scoreboard->getPlayer2Wins(),
             scoreboard->getDraws());
    attroff(COLOR_PAIR(colors::SCORE));
}

} // namespace game::view
