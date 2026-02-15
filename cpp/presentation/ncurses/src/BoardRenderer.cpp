#include "presentation/ncurses/BoardRenderer.hpp"
#include "presentation/ncurses/Colors.hpp"

#include <ncurses.h>
#include <algorithm>
#include <cstdlib>
#include <vector>

namespace game::view
{

namespace
{

constexpr std::array<std::array<int, 3>, 8> WIN_PATTERNS = {{
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // Rows
    {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // Columns
    {0, 4, 8}, {2, 4, 6}              // Diagonals
}};

} // anonymous namespace

bool BoardRenderer::checkMinimumTerminalSize()
{
    if (LINES < MIN_ROWS || COLS < MIN_COLS)
    {
        clear();
        attron(COLOR_PAIR(colors::STATUS) | A_BOLD);
        mvprintw(LINES / 2 - 1, (COLS - 40) / 2,
                 "Terminal too small!");
        attroff(A_BOLD);
        mvprintw(LINES / 2, (COLS - 40) / 2,
                 "Need at least %dx%d, have %dx%d",
                 MIN_COLS, MIN_ROWS, COLS, LINES);
        mvprintw(LINES / 2 + 2, (COLS - 30) / 2,
                 "Please resize and press any key.");
        attroff(COLOR_PAIR(colors::STATUS));
        refresh();
        getch();
        return false;
    }
    return true;
}

bool BoardRenderer::useCompactTitle()
{
    return COLS < FULL_TITLE_WIDTH + 10;
}

int BoardRenderer::getBoardStartCol()
{
    return (COLS - BOARD_WIDTH) / 2;
}

int BoardRenderer::getBoardStartRow()
{
    return getTitleHeight() + 2;
}

int BoardRenderer::getTitleHeight()
{
    return useCompactTitle() ? 5 : 9;
}

void BoardRenderer::drawCompactTitle()
{
    int startCol = (COLS - COMPACT_TITLE_WIDTH) / 2;

    attron(COLOR_PAIR(colors::TITLE) | A_BOLD);
    mvprintw(1, startCol, " _____ _        _____            _____          ");
    mvprintw(2, startCol, "|_   _(_)      |_   _|          |_   _|         ");
    mvprintw(3, startCol, "  | |  _  ___    | | __ _  ___    | | ___   ___ ");
    mvprintw(4, startCol, "  | | | |/ __|   | |/ _` |/ __|   | |/ _ \\ / _ \\");
    mvprintw(5, startCol, "  | | | | (__    | | (_| | (__    | | (_) |  __/");
    mvprintw(6, startCol, "  \\_/ |_|\\___|   \\_/\\__,_|\\___|   \\_/\\___/ \\___|");
    attroff(COLOR_PAIR(colors::TITLE) | A_BOLD);
}

void BoardRenderer::drawFullTitle()
{
    int startCol = (COLS - FULL_TITLE_WIDTH) / 2;

    attron(COLOR_PAIR(colors::TITLE) | A_BOLD);
    mvprintw(1, startCol, R"( /######## /######  /######        /######## /######   /######        /######## /######  /########)");
    mvprintw(2, startCol, R"(|__  ##__/|_  ##_/ /##__  ##      |__  ##__//##__  ## /##__  ##      |__  ##__//##__  ##| ##_____/)");
    mvprintw(3, startCol, R"(   | ##     | ##  | ##  \__/         | ##  | ##  \ ##| ##  \__/         | ##  | ##  \ ##| ##      )");
    mvprintw(4, startCol, R"(   | ##     | ##  | ##               | ##  | ########| ##               | ##  | ##  | ##| #####   )");
    mvprintw(5, startCol, R"(   | ##     | ##  | ##               | ##  | ##__  ##| ##               | ##  | ##  | ##| ##__/   )");
    mvprintw(6, startCol, R"(   | ##     | ##  | ##    ##         | ##  | ##  | ##| ##    ##         | ##  | ##  | ##| ##      )");
    mvprintw(7, startCol, R"(   | ##    /######|  ######/         | ##  | ##  | ##|  ######/         | ##  |  ######/| ########)");
    mvprintw(8, startCol, R"(   |__/   |______/ \______/          |__/  |__/  |__/ \______/          |__/   \______/ |________/)");
    attroff(COLOR_PAIR(colors::TITLE) | A_BOLD);
}

void BoardRenderer::drawTitle()
{
    if (useCompactTitle())
    {
        drawCompactTitle();
    }
    else
    {
        drawFullTitle();
    }
    refresh();
}

void BoardRenderer::drawCellContent(int row, int col,
                                     std::optional<core::Marker> marker,
                                     int cellIndex, bool selected, bool winning)
{
    int boardCol = getBoardStartCol();
    int boardRow = getBoardStartRow();
    int y = boardRow + 1 + row * (CELL_HEIGHT + 1) + CELL_HEIGHT / 2;
    int x = boardCol + 2 + col * (CELL_WIDTH + 1) + (CELL_WIDTH - 3) / 2;

    if (winning)
    {
        attron(COLOR_PAIR(colors::SCORE) | A_BOLD | A_REVERSE);
    }
    else if (selected)
    {
        attron(COLOR_PAIR(colors::HIGHLIGHT) | A_BOLD);
    }

    if (marker.has_value())
    {
        if (marker.value() == core::Marker::X)
        {
            if (!selected && !winning) attron(COLOR_PAIR(colors::MARKER_X) | A_BOLD);
            mvprintw(y, x, " X ");
            if (!selected && !winning) attroff(COLOR_PAIR(colors::MARKER_X) | A_BOLD);
        }
        else
        {
            if (!selected && !winning) attron(COLOR_PAIR(colors::MARKER_O) | A_BOLD);
            mvprintw(y, x, " O ");
            if (!selected && !winning) attroff(COLOR_PAIR(colors::MARKER_O) | A_BOLD);
        }
    }
    else
    {
        if (!selected && !winning) attron(COLOR_PAIR(colors::EMPTY_CELL) | A_DIM);
        mvprintw(y, x, " %d ", cellIndex + 1);
        if (!selected && !winning) attroff(COLOR_PAIR(colors::EMPTY_CELL) | A_DIM);
    }

    if (winning)
    {
        attroff(COLOR_PAIR(colors::SCORE) | A_BOLD | A_REVERSE);
    }
    else if (selected)
    {
        attroff(COLOR_PAIR(colors::HIGHLIGHT) | A_BOLD);
    }
}

void BoardRenderer::drawBoard(const core::Board& board, int selectedCell,
                               const std::array<int, 3>* winningCells)
{
    int boardCol = getBoardStartCol();
    int boardRow = getBoardStartRow();

    attron(COLOR_PAIR(colors::BOARD_LINES));

    // Draw outer border with double lines
    // Top border
    mvaddch(boardRow, boardCol, ACS_ULCORNER);
    for (int col = 0; col < 3; ++col)
    {
        for (int i = 0; i < CELL_WIDTH; ++i)
        {
            addch(ACS_HLINE);
        }
        if (col < 2) addch(ACS_TTEE);
    }
    addch(ACS_URCORNER);

    // Middle rows
    for (int row = 0; row < 3; ++row)
    {
        // Cell content rows
        for (int cy = 0; cy < CELL_HEIGHT; ++cy)
        {
            int y = boardRow + 1 + row * (CELL_HEIGHT + 1) + cy;
            mvaddch(y, boardCol, ACS_VLINE);
            for (int col = 0; col < 3; ++col)
            {
                for (int i = 0; i < CELL_WIDTH; ++i)
                {
                    addch(' ');
                }
                addch(ACS_VLINE);
            }
        }

        // Horizontal separator (except after last row)
        if (row < 2)
        {
            int y = boardRow + 1 + row * (CELL_HEIGHT + 1) + CELL_HEIGHT;
            mvaddch(y, boardCol, ACS_LTEE);
            for (int col = 0; col < 3; ++col)
            {
                for (int i = 0; i < CELL_WIDTH; ++i)
                {
                    addch(ACS_HLINE);
                }
                if (col < 2)
                {
                    addch(ACS_PLUS);
                }
            }
            addch(ACS_RTEE);
        }
    }

    // Bottom border
    int bottomY = boardRow + 1 + 2 * (CELL_HEIGHT + 1) + CELL_HEIGHT;
    mvaddch(bottomY, boardCol, ACS_LLCORNER);
    for (int col = 0; col < 3; ++col)
    {
        for (int i = 0; i < CELL_WIDTH; ++i)
        {
            addch(ACS_HLINE);
        }
        if (col < 2) addch(ACS_BTEE);
    }
    addch(ACS_LRCORNER);

    attroff(COLOR_PAIR(colors::BOARD_LINES));

    // Draw cell contents
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            int cellIndex = row * 3 + col;
            core::Position pos{cellIndex};
            bool isSelected = (cellIndex == selectedCell);
            bool isWinning = winningCells &&
                std::find(winningCells->begin(), winningCells->end(), cellIndex) != winningCells->end();
            drawCellContent(row, col, board.getMarker(pos), cellIndex, isSelected, isWinning);
        }
    }
}

void BoardRenderer::drawStatusBar(std::string_view message)
{
    int maxY = getmaxy(stdscr);
    attron(COLOR_PAIR(colors::STATUS));
    move(maxY - 2, 0);
    clrtoeol();
    int startCol = (COLS - static_cast<int>(message.size())) / 2;
    if (startCol < 0) startCol = 0;
    mvprintw(maxY - 2, startCol, "%.*s",
             static_cast<int>(message.size()), message.data());
    attroff(COLOR_PAIR(colors::STATUS));
}

void BoardRenderer::drawScoreboard(const app::Session& session)
{
    int boardRow = getBoardStartRow();
    int scoreRow = boardRow + BOARD_HEIGHT + 1;
    int centerCol = COLS / 2;

    attron(COLOR_PAIR(colors::SCORE) | A_BOLD);
    const char* title = "SCOREBOARD";
    mvprintw(scoreRow, centerCol - 5, "%s", title);
    attroff(A_BOLD);

    const auto& sb = session.getScoreboard();
    std::string p1Name = session.getPlayer1()->getName();
    std::string p2Name = session.getPlayer2()->getName();

    // Calculate bar widths (max 10 chars)
    int maxWins = std::max({sb.getPlayer1Wins(), sb.getPlayer2Wins(), sb.getDraws(), 1});
    int p1BarLen = (sb.getPlayer1Wins() * 10) / maxWins;
    int p2BarLen = (sb.getPlayer2Wins() * 10) / maxWins;
    int drawBarLen = (sb.getDraws() * 10) / maxWins;

    // Player 1 score
    attron(COLOR_PAIR(colors::MARKER_X));
    mvprintw(scoreRow + 2, centerCol - 20, "%-12s", p1Name.c_str());
    for (int i = 0; i < 10; ++i)
    {
        addch(i < p1BarLen ? ACS_CKBOARD : ACS_BULLET);
    }
    printw(" %d", sb.getPlayer1Wins());
    attroff(COLOR_PAIR(colors::MARKER_X));

    // Player 2 score
    attron(COLOR_PAIR(colors::MARKER_O));
    mvprintw(scoreRow + 3, centerCol - 20, "%-12s", p2Name.c_str());
    for (int i = 0; i < 10; ++i)
    {
        addch(i < p2BarLen ? ACS_CKBOARD : ACS_BULLET);
    }
    printw(" %d", sb.getPlayer2Wins());
    attroff(COLOR_PAIR(colors::MARKER_O));

    // Draws
    attron(COLOR_PAIR(colors::STATUS));
    mvprintw(scoreRow + 4, centerCol - 20, "%-12s", "Draws");
    for (int i = 0; i < 10; ++i)
    {
        addch(i < drawBarLen ? ACS_CKBOARD : ACS_BULLET);
    }
    printw(" %d", sb.getDraws());
    attroff(COLOR_PAIR(colors::STATUS));

    attroff(COLOR_PAIR(colors::SCORE));
}

void BoardRenderer::drawScoreboardCompact(const app::Scoreboard* scoreboard)
{
    if (!scoreboard) return;

    int boardRow = getBoardStartRow();
    int scoreRow = boardRow + BOARD_HEIGHT + 1;
    int centerCol = COLS / 2;

    attron(COLOR_PAIR(colors::SCORE) | A_BOLD);
    mvprintw(scoreRow, centerCol - 15, "SCORE");
    attroff(A_BOLD);

    mvprintw(scoreRow, centerCol - 8,
             "P1: %d  P2: %d  Draws: %d",
             scoreboard->getPlayer1Wins(),
             scoreboard->getPlayer2Wins(),
             scoreboard->getDraws());
    attroff(COLOR_PAIR(colors::SCORE));
}

void BoardRenderer::flashCell(int cellIndex, const core::Board& board, int times)
{
    int row = cellIndex / 3;
    int col = cellIndex % 3;
    auto marker = board.getMarker(core::Position{cellIndex});

    for (int i = 0; i < times; ++i)
    {
        // Flash on (highlighted)
        drawCellContent(row, col, marker, cellIndex, true, false);
        refresh();
        napms(80);

        // Flash off (normal)
        drawCellContent(row, col, marker, cellIndex, false, false);
        refresh();
        napms(80);
    }
}

void BoardRenderer::highlightWinningLine(const core::Board& board,
                                          const std::array<int, 3>& winningCells)
{
    for (int flash = 0; flash < 3; ++flash)
    {
        // Draw winning cells highlighted
        for (int cellIndex : winningCells)
        {
            int row = cellIndex / 3;
            int col = cellIndex % 3;
            auto marker = board.getMarker(core::Position{cellIndex});
            drawCellContent(row, col, marker, cellIndex, false, true);
        }
        refresh();
        napms(200);

        // Draw winning cells normal
        for (int cellIndex : winningCells)
        {
            int row = cellIndex / 3;
            int col = cellIndex % 3;
            auto marker = board.getMarker(core::Position{cellIndex});
            drawCellContent(row, col, marker, cellIndex, false, false);
        }
        refresh();
        napms(100);
    }

    // Leave them highlighted
    for (int cellIndex : winningCells)
    {
        int row = cellIndex / 3;
        int col = cellIndex % 3;
        auto marker = board.getMarker(core::Position{cellIndex});
        drawCellContent(row, col, marker, cellIndex, false, true);
    }
    refresh();
}

std::optional<std::array<int, 3>>
BoardRenderer::getWinningCells(const core::Board& board, core::GameStatus status)
{
    if (status != core::GameStatus::XWins && status != core::GameStatus::OWins)
    {
        return std::nullopt;
    }

    core::Marker winner = (status == core::GameStatus::XWins)
        ? core::Marker::X : core::Marker::O;

    for (const auto& pattern : WIN_PATTERNS)
    {
        bool allMatch = true;
        for (int cellIndex : pattern)
        {
            auto marker = board.getMarker(core::Position{cellIndex});
            if (!marker.has_value() || marker.value() != winner)
            {
                allMatch = false;
                break;
            }
        }
        if (allMatch)
        {
            return pattern;
        }
    }

    return std::nullopt;
}

void BoardRenderer::drawWinnerBanner(core::Marker winner)
{
    int centerCol = COLS / 2;
    int bannerRow = LINES / 2 - 4;

    int colorPair = (winner == core::Marker::X) ? colors::MARKER_X : colors::MARKER_O;

    attron(COLOR_PAIR(colorPair) | A_BOLD);

    if (winner == core::Marker::X)
    {
        // Large X WINS! ASCII art
        mvprintw(bannerRow,     centerCol - 20, " __  __  __        __ ___ _   _  ___  _ ");
        mvprintw(bannerRow + 1, centerCol - 20, " \\ \\/ /  \\ \\      / /|_ _| \\ | |/ __|| |");
        mvprintw(bannerRow + 2, centerCol - 20, "  \\  /    \\ \\ /\\ / /  | ||  \\| |\\__ \\|_|");
        mvprintw(bannerRow + 3, centerCol - 20, "  /  \\     \\ V  V /   | || |\\  ||__) |_ ");
        mvprintw(bannerRow + 4, centerCol - 20, " /_/\\_\\     \\_/\\_/   |___|_| \\_||___/(_)");
    }
    else
    {
        // Large O WINS! ASCII art
        mvprintw(bannerRow,     centerCol - 20, "  ___   __        __ ___ _   _  ___  _ ");
        mvprintw(bannerRow + 1, centerCol - 20, " / _ \\  \\ \\      / /|_ _| \\ | |/ __|| |");
        mvprintw(bannerRow + 2, centerCol - 20, "| | | |  \\ \\ /\\ / /  | ||  \\| |\\__ \\|_|");
        mvprintw(bannerRow + 3, centerCol - 20, "| |_| |   \\ V  V /   | || |\\  ||__) |_ ");
        mvprintw(bannerRow + 4, centerCol - 20, " \\___/     \\_/\\_/   |___|_| \\_||___/(_)");
    }

    attroff(COLOR_PAIR(colorPair) | A_BOLD);
}

void BoardRenderer::drawDrawBanner()
{
    int centerCol = COLS / 2;
    int bannerRow = LINES / 2 - 3;

    attron(COLOR_PAIR(colors::STATUS) | A_BOLD);

    mvprintw(bannerRow,     centerCol - 18, "  ____ _____  _    _     _____ __  __    _  _____ _____ ");
    mvprintw(bannerRow + 1, centerCol - 18, " / ___|_   _|/ \\  | |   | ____|  \\/  |  / \\|_   _| ____|");
    mvprintw(bannerRow + 2, centerCol - 18, " \\___ \\ | | / _ \\ | |   |  _| | |\\/| | / _ \\ | | |  _|  ");
    mvprintw(bannerRow + 3, centerCol - 18, "  ___) || |/ ___ \\| |___| |___| |  | |/ ___ \\| | | |___ ");
    mvprintw(bannerRow + 4, centerCol - 18, " |____/ |_/_/   \\_|_____|_____|_|  |_/_/   \\_|_| |_____|");

    attroff(COLOR_PAIR(colors::STATUS) | A_BOLD);
}

void BoardRenderer::runConfettiAnimation(int durationMs)
{
    static const char confettiChars[] = "*+o.#@%&";
    static const int confettiColors[] = {
        colors::MARKER_X, colors::MARKER_O, colors::TITLE,
        colors::SCORE, colors::STATUS
    };

    int numConfetti = 40;
    int frames = durationMs / 50;

    // Store confetti positions
    struct Confetti {
        int x, y;
        int dy;
        char ch;
        int color;
    };

    std::vector<Confetti> particles;
    particles.reserve(numConfetti);

    // Initialize confetti at random positions at top
    for (int i = 0; i < numConfetti; ++i)
    {
        Confetti c;
        c.x = rand() % COLS;
        c.y = rand() % 3;
        c.dy = 1 + rand() % 2;
        c.ch = confettiChars[rand() % (sizeof(confettiChars) - 1)];
        c.color = confettiColors[rand() % 5];
        particles.push_back(c);
    }

    for (int frame = 0; frame < frames; ++frame)
    {
        // Draw confetti
        for (auto& c : particles)
        {
            if (c.y >= 0 && c.y < LINES - 2)
            {
                attron(COLOR_PAIR(c.color) | A_BOLD);
                mvaddch(c.y, c.x, c.ch);
                attroff(COLOR_PAIR(c.color) | A_BOLD);
            }

            // Move confetti down
            c.y += c.dy;

            // Respawn at top if fell off bottom
            if (c.y >= LINES - 2)
            {
                c.y = 0;
                c.x = rand() % COLS;
                c.ch = confettiChars[rand() % (sizeof(confettiChars) - 1)];
            }
        }

        refresh();
        napms(50);

        // Clear confetti positions for next frame (but don't erase the whole screen)
        for (const auto& c : particles)
        {
            if (c.y - c.dy >= 0 && c.y - c.dy < LINES - 2)
            {
                mvaddch(c.y - c.dy, c.x, ' ');
            }
        }
    }
}

void BoardRenderer::fadeIn(int delayMs)
{
    // Simple fade-in effect by progressively revealing the screen
    refresh();
    napms(delayMs);
}

void BoardRenderer::screenWipe(int durationMs)
{
    // Wipe effect: lines slide in from left to right
    int frameDelay = durationMs / LINES;
    if (frameDelay < 5) frameDelay = 5;

    attron(COLOR_PAIR(colors::TITLE));
    for (int row = 0; row < LINES; ++row)
    {
        // Wipe this row with a gradient
        for (int step = 0; step < 3; ++step)
        {
            int startCol = (COLS * step) / 3;
            int endCol = (COLS * (step + 1)) / 3;
            for (int col = startCol; col < endCol; ++col)
            {
                mvaddch(row, col, ' ');
            }
        }
        if (row % 3 == 0)
        {
            refresh();
            napms(frameDelay);
        }
    }
    attroff(COLOR_PAIR(colors::TITLE));
    clear();
    refresh();
}

void BoardRenderer::drawHelpPanel()
{
    int panelWidth = 44;
    int panelHeight = 14;
    int panelTop = (LINES - panelHeight) / 2;
    int panelLeft = (COLS - panelWidth) / 2;

    // Draw panel background
    attron(COLOR_PAIR(colors::BOARD_LINES));
    for (int row = 0; row < panelHeight; ++row)
    {
        mvhline(panelTop + row, panelLeft, ' ', panelWidth);
    }

    // Draw border
    mvaddch(panelTop, panelLeft, ACS_ULCORNER);
    mvhline(panelTop, panelLeft + 1, ACS_HLINE, panelWidth - 2);
    mvaddch(panelTop, panelLeft + panelWidth - 1, ACS_URCORNER);

    for (int row = 1; row < panelHeight - 1; ++row)
    {
        mvaddch(panelTop + row, panelLeft, ACS_VLINE);
        mvaddch(panelTop + row, panelLeft + panelWidth - 1, ACS_VLINE);
    }

    mvaddch(panelTop + panelHeight - 1, panelLeft, ACS_LLCORNER);
    mvhline(panelTop + panelHeight - 1, panelLeft + 1, ACS_HLINE, panelWidth - 2);
    mvaddch(panelTop + panelHeight - 1, panelLeft + panelWidth - 1, ACS_LRCORNER);
    attroff(COLOR_PAIR(colors::BOARD_LINES));

    // Title
    attron(COLOR_PAIR(colors::TITLE) | A_BOLD);
    mvprintw(panelTop + 1, panelLeft + (panelWidth - 8) / 2, "CONTROLS");
    attroff(COLOR_PAIR(colors::TITLE) | A_BOLD);

    // Help content
    int contentLeft = panelLeft + 3;
    int row = panelTop + 3;

    attron(COLOR_PAIR(colors::STATUS));
    mvprintw(row++, contentLeft, "Navigation:");
    attroff(COLOR_PAIR(colors::STATUS));

    mvprintw(row++, contentLeft + 2, "Arrow keys / WASD   Move cursor");
    mvprintw(row++, contentLeft + 2, "1-9                 Jump to cell");

    row++;
    attron(COLOR_PAIR(colors::STATUS));
    mvprintw(row++, contentLeft, "Actions:");
    attroff(COLOR_PAIR(colors::STATUS));

    mvprintw(row++, contentLeft + 2, "Enter / Space       Place marker");
    mvprintw(row++, contentLeft + 2, "Q                   Quit game");
    mvprintw(row++, contentLeft + 2, "?                   Toggle this help");

    row++;
    attron(COLOR_PAIR(colors::DIM) | A_DIM);
    mvprintw(row, panelLeft + (panelWidth - 22) / 2, "Press any key to close");
    attroff(COLOR_PAIR(colors::DIM) | A_DIM);

    refresh();
}

} // namespace game::view
