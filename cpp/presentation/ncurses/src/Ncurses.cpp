#include "presentation/Ncurses.hpp"
#include "application/AgentPlayer.hpp"
#include "application/ContinuationResult.hpp"
#include "application/Session.hpp"
#include "core/GameStatus.hpp"
#include "core/Marker.hpp"

#include <ncurses.h>
#include <array>
#include <memory>
#include <string>
#include <utility>

namespace game::view
{

// ============================================================================
// Color pair IDs
// ============================================================================
namespace colors
{
    constexpr int MARKER_X    = 1;
    constexpr int MARKER_O    = 2;
    constexpr int EMPTY_CELL  = 3;
    constexpr int BOARD_LINES = 4;
    constexpr int TITLE       = 5;
    constexpr int STATUS      = 6;
    constexpr int HIGHLIGHT   = 7;
    constexpr int SCORE       = 8;
} // namespace colors

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
        init_pair(colors::MARKER_X,    COLOR_RED,     -1);
        init_pair(colors::MARKER_O,    COLOR_BLUE,    -1);
        init_pair(colors::EMPTY_CELL,  COLOR_WHITE,   -1);
        init_pair(colors::BOARD_LINES, COLOR_WHITE,   -1);
        init_pair(colors::TITLE,       COLOR_CYAN,    -1);
        init_pair(colors::STATUS,      COLOR_YELLOW,  -1);
        init_pair(colors::HIGHLIGHT,   COLOR_BLACK,   COLOR_WHITE);
        init_pair(colors::SCORE,       COLOR_GREEN,   -1);
    }
}

NcursesContext::~NcursesContext()
{
    endwin();
}

// ============================================================================
// Shared rendering utilities
// ============================================================================

namespace ncurses_ui
{

static constexpr int BOARD_START_ROW = 4;
static constexpr int BOARD_START_COL = 4;
static constexpr int CELL_WIDTH = 7;
static constexpr int CELL_HEIGHT = 3;
static constexpr int MIN_ROWS = 22;
static constexpr int MIN_COLS = 30;

bool checkMinimumTerminalSize()
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

void drawTitle()
{
    attron(COLOR_PAIR(colors::TITLE) | A_BOLD);
    mvprintw(1, BOARD_START_COL, "TIC-TAC-TOE");
    attroff(COLOR_PAIR(colors::TITLE) | A_BOLD);
    mvprintw(2, BOARD_START_COL, "---------------------");
}

static void drawCellContent(int row, int col,
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

void drawBoard(const core::Board& board, int selectedCell)
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

void drawStatusBar(std::string_view message)
{
    int maxY = getmaxy(stdscr);
    attron(COLOR_PAIR(colors::STATUS));
    move(maxY - 2, 0);
    clrtoeol();
    mvprintw(maxY - 2, BOARD_START_COL, "%.*s",
             static_cast<int>(message.size()), message.data());
    attroff(COLOR_PAIR(colors::STATUS));
}

void drawScoreboard(const app::Session& session)
{
    int scoreRow = BOARD_START_ROW + 3 * CELL_HEIGHT + 2;

    attron(COLOR_PAIR(colors::SCORE) | A_BOLD);
    mvprintw(scoreRow, BOARD_START_COL, "SCOREBOARD");
    attroff(COLOR_PAIR(colors::SCORE) | A_BOLD);

    // Note: Player 1 is always X and Player 2 is always O per Session contract
    const auto& sb = session.getScoreboard();
    mvprintw(scoreRow + 1, BOARD_START_COL,
             "%s (X): %d    %s (O): %d    Draws: %d",
             session.getPlayer1()->getName().c_str(), sb.getPlayer1Wins(),
             session.getPlayer2()->getName().c_str(), sb.getPlayer2Wins(),
             sb.getDraws());
}

static void drawScoreboardCompact(const app::Scoreboard* scoreboard)
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

} // namespace ncurses_ui

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
    // Defensive: if no empty cells exist, quit rather than hang
    if (board.isFull())
    {
        return std::unexpected(app::Quit{});
    }

    // Find first empty cell to start selection
    int selectedCell = 0;
    for (int i = 0; i < 9; ++i)
    {
        if (board.isCellEmpty(core::Position{i}))
        {
            selectedCell = i;
            break;
        }
    }

    std::string markerStr = (marker == core::Marker::X) ? "X" : "O";

    while (true)
    {
        erase();
        ncurses_ui::drawTitle();
        ncurses_ui::drawBoard(board, selectedCell);
        ncurses_ui::drawScoreboardCompact(m_scoreboard);

        std::string status = m_name + " (" + markerStr +
                             ") - Arrows/WASD: move | Enter/Space: place | 1-9: direct | Q: quit";
        ncurses_ui::drawStatusBar(status);

        refresh();

        int ch = getch();

        switch (ch)
        {
            case 'q':
            case 'Q':
                return std::unexpected(app::Quit{});

            case '1': case '2': case '3':
            case '4': case '5': case '6':
            case '7': case '8': case '9':
            {
                int pos = ch - '1';
                if (board.isCellEmpty(core::Position{pos}))
                {
                    return core::Position{pos};
                }
                ncurses_ui::drawStatusBar("Cell is already occupied!");
                refresh();
                napms(600);
                break;
            }

            case KEY_UP:
            case 'w':
            case 'W':
            {
                selectedCell = (selectedCell < 3)
                    ? selectedCell + 6   // wrap to bottom row
                    : selectedCell - 3;
                break;
            }

            case KEY_DOWN:
            case 's':
            case 'S':
            {
                selectedCell = (selectedCell >= 6)
                    ? selectedCell - 6   // wrap to top row
                    : selectedCell + 3;
                break;
            }

            case KEY_LEFT:
            case 'a':
            case 'A':
            {
                int row = selectedCell / 3;
                int col = selectedCell % 3;
                col = (col == 0) ? 2 : col - 1;  // wrap to right column
                selectedCell = row * 3 + col;
                break;
            }

            case KEY_RIGHT:
            case 'd':
            case 'D':
            {
                int row = selectedCell / 3;
                int col = selectedCell % 3;
                col = (col == 2) ? 0 : col + 1;  // wrap to left column
                selectedCell = row * 3 + col;
                break;
            }

            case '\n':
            case ' ':
            case KEY_ENTER:
            {
                if (board.isCellEmpty(core::Position{selectedCell}))
                {
                    return core::Position{selectedCell};
                }
                ncurses_ui::drawStatusBar("Cell is already occupied!");
                refresh();
                napms(600);
                break;
            }

            default:
                break;
        }
    }
}

// ============================================================================
// NcursesGameStartListener
// ============================================================================

app::ContinuationResult NcursesGameStartListener::onGameStarted(const app::Session& session)
{
    clear();
    ncurses_ui::drawTitle();

    int centerRow = LINES / 2;
    int centerCol = COLS / 2;

    attron(COLOR_PAIR(colors::TITLE) | A_BOLD);
    std::string msg = "Game " + std::to_string(session.getScoreboard().getTotalGames() + 1);
    mvprintw(centerRow - 1, centerCol - static_cast<int>(msg.size()) / 2, "%s", msg.c_str());

    // Note: Player 1 is always X and Player 2 is always O per Session contract
    std::string p1 = session.getPlayer1()->getName() + " (X)";
    std::string p2 = session.getPlayer2()->getName() + " (O)";
    std::string vs = p1 + "  vs  " + p2;
    mvprintw(centerRow + 1, centerCol - static_cast<int>(vs.size()) / 2, "%s", vs.c_str());
    attroff(COLOR_PAIR(colors::TITLE) | A_BOLD);

    attron(COLOR_PAIR(colors::STATUS));
    mvprintw(centerRow + 3, centerCol - 14, "Press any key to start (Q to quit)");
    attroff(COLOR_PAIR(colors::STATUS));

    refresh();

    int ch = getch();
    if (ch == 'q' || ch == 'Q')
    {
        return app::ContinuationResult::QUIT;
    }

    return app::ContinuationResult::CONTINUE;
}

// ============================================================================
// NcursesGameFinishedListener
// ============================================================================

app::ContinuationResult NcursesGameFinishedListener::onGameFinished(const app::Session& session)
{
    clear();
    ncurses_ui::drawTitle();
    ncurses_ui::drawBoard(session.getGameState().getBoard(), -1);

    int resultRow = ncurses_ui::BOARD_START_ROW + 3 * ncurses_ui::CELL_HEIGHT + 2;

    switch (session.getGameState().getStatus())
    {
        case core::GameStatus::XWins:
            attron(COLOR_PAIR(colors::MARKER_X) | A_BOLD);
            mvprintw(resultRow, ncurses_ui::BOARD_START_COL,
                     "%s (X) WINS!", session.getPlayer1()->getName().c_str());
            attroff(COLOR_PAIR(colors::MARKER_X) | A_BOLD);
            break;
        case core::GameStatus::OWins:
            attron(COLOR_PAIR(colors::MARKER_O) | A_BOLD);
            mvprintw(resultRow, ncurses_ui::BOARD_START_COL,
                     "%s (O) WINS!", session.getPlayer2()->getName().c_str());
            attroff(COLOR_PAIR(colors::MARKER_O) | A_BOLD);
            break;
        case core::GameStatus::Draw:
            attron(COLOR_PAIR(colors::STATUS) | A_BOLD);
            mvprintw(resultRow, ncurses_ui::BOARD_START_COL, "It's a DRAW!");
            attroff(COLOR_PAIR(colors::STATUS) | A_BOLD);
            break;
        default:
            break;
    }

    ncurses_ui::drawScoreboard(session);

    int promptRow = resultRow + 4;
    attron(COLOR_PAIR(colors::STATUS));
    mvprintw(promptRow, ncurses_ui::BOARD_START_COL,
             "Play again? (Y)es / (N)o");
    attroff(COLOR_PAIR(colors::STATUS));

    refresh();

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

// ============================================================================
// NcursesSessionGenerator
// ============================================================================

namespace
{

enum class PlayerType
{
    Human,
    CpuEasy,
    CpuHard
};

struct PlayerTypeOption
{
    const char* label;
    PlayerType type;
};

static constexpr std::array<PlayerTypeOption, 3> PLAYER_TYPE_OPTIONS = {{
    {"Human",            PlayerType::Human},
    {"Computer (Easy)",  PlayerType::CpuEasy},
    {"Computer (Hard)",  PlayerType::CpuHard}
}};

/// \brief Draw a player type selection menu and return the chosen type, or Quit
static std::expected<PlayerType, app::Quit>
selectPlayerType(int playerNumber, std::string_view markerLabel)
{
    int selected = 0;

    while (true)
    {
        erase();
        ncurses_ui::drawTitle();

        int menuRow = ncurses_ui::BOARD_START_ROW + 1;
        int menuCol = ncurses_ui::BOARD_START_COL;

        attron(COLOR_PAIR(colors::TITLE) | A_BOLD);
        mvprintw(menuRow, menuCol, "Player %d (%.*s) - Select type:",
                 playerNumber,
                 static_cast<int>(markerLabel.size()), markerLabel.data());
        attroff(COLOR_PAIR(colors::TITLE) | A_BOLD);

        for (int i = 0; i < static_cast<int>(PLAYER_TYPE_OPTIONS.size()); ++i)
        {
            int row = menuRow + 2 + i;
            if (i == selected)
            {
                attron(COLOR_PAIR(colors::HIGHLIGHT) | A_BOLD);
                mvprintw(row, menuCol, " > %s", PLAYER_TYPE_OPTIONS[i].label);
                attroff(COLOR_PAIR(colors::HIGHLIGHT) | A_BOLD);
            }
            else
            {
                mvprintw(row, menuCol, "   %s", PLAYER_TYPE_OPTIONS[i].label);
            }
        }

        ncurses_ui::drawStatusBar("Up/Down: select | Enter/1-3: confirm | Q: quit");
        refresh();

        int ch = getch();
        switch (ch)
        {
            case KEY_UP:
            case 'w':
            case 'W':
                selected = (selected == 0)
                    ? static_cast<int>(PLAYER_TYPE_OPTIONS.size()) - 1
                    : selected - 1;
                break;

            case KEY_DOWN:
            case 's':
            case 'S':
                selected = (selected == static_cast<int>(PLAYER_TYPE_OPTIONS.size()) - 1)
                    ? 0
                    : selected + 1;
                break;

            case '1':
                return PLAYER_TYPE_OPTIONS[0].type;
            case '2':
                return PLAYER_TYPE_OPTIONS[1].type;
            case '3':
                return PLAYER_TYPE_OPTIONS[2].type;

            case '\n':
            case ' ':
            case KEY_ENTER:
                return PLAYER_TYPE_OPTIONS[selected].type;

            case 'q':
            case 'Q':
                return std::unexpected(app::Quit{});

            default:
                break;
        }
    }
}

static std::shared_ptr<app::Player>
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
    if (!ncurses_ui::checkMinimumTerminalSize())
    {
        return std::unexpected(app::Quit{});
    }

    auto p1Type = selectPlayerType(1, "X");
    if (!p1Type.has_value())
    {
        return std::unexpected(app::Quit{});
    }

    auto p2Type = selectPlayerType(2, "O");
    if (!p2Type.has_value())
    {
        return std::unexpected(app::Quit{});
    }

    // Create session first with temporary players, then inject scoreboard.
    // We create the session with placeholder players, then replace them
    // so NcursesPlayer instances can get the scoreboard reference.
    auto p1 = createPlayer(p1Type.value(), 1, nullptr);
    auto p2 = createPlayer(p2Type.value(), 2, nullptr);
    auto session = std::make_unique<app::Session>(p1, p2);

    // Inject scoreboard reference into NcursesPlayer instances.
    // Safe: the Session owns the Scoreboard and outlives all generateNextMove calls.
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
