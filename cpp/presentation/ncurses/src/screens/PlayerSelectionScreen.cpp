#include "presentation/ncurses/screens/PlayerSelectionScreen.hpp"
#include "presentation/ncurses/BoardRenderer.hpp"
#include "presentation/ncurses/Colors.hpp"

#include <ncurses.h>
#include <array>

namespace game::view
{

namespace
{

struct PlayerTypeOption
{
    const char* icon;
    const char* label;
    const char* description;
    PlayerType type;
};

static constexpr std::array<PlayerTypeOption, 3> PLAYER_TYPE_OPTIONS = {{
    {"[H]", "Human",            "You control this player",           PlayerType::Human},
    {"[E]", "Computer (Easy)",  "Makes random moves",                 PlayerType::CpuEasy},
    {"[X]", "Computer (Hard)",  "Plays optimally - never loses",      PlayerType::CpuHard}
}};

} // anonymous namespace

PlayerSelectionScreen::PlayerSelectionScreen(int playerNumber, std::string_view markerLabel)
    : m_playerNumber(playerNumber)
    , m_markerLabel(markerLabel)
{
}

void PlayerSelectionScreen::draw()
{
    // Screen wipe transition when entering player 2 selection
    if (m_isFirstDraw && m_playerNumber > 1)
    {
        BoardRenderer::screenWipe(150);
    }
    m_isFirstDraw = false;

    erase();
    BoardRenderer::drawTitle();

    int centerCol = COLS / 2;
    int menuRow = BoardRenderer::getTitleHeight() + 2;

    // Progress indicator: filled dot for current/done, empty for pending
    {
        attron(COLOR_PAIR(colors::STATUS));
        std::string progress = "Player Setup:  ";
        progress += (m_playerNumber >= 1) ? "(1)" : " 1 ";
        progress += "  ";
        progress += (m_playerNumber >= 2) ? "(2)" : " 2 ";
        mvprintw(menuRow, centerCol - static_cast<int>(progress.size()) / 2,
                 "%s", progress.c_str());
        attroff(COLOR_PAIR(colors::STATUS));
        menuRow += 2;
    }

    // Header styled with the player's marker color
    int markerColor = (m_markerLabel == "X") ? colors::MARKER_X : colors::MARKER_O;
    attron(COLOR_PAIR(markerColor) | A_BOLD);
    std::string header = "Player " + std::to_string(m_playerNumber) + " (" +
                         std::string(m_markerLabel) + ") - Select type:";
    mvprintw(menuRow, centerCol - static_cast<int>(header.size()) / 2, "%s", header.c_str());
    attroff(COLOR_PAIR(markerColor) | A_BOLD);

    // Draw menu box
    int boxWidth = 40;
    int boxLeft = centerCol - boxWidth / 2;
    int boxTop = menuRow + 2;

    attron(COLOR_PAIR(colors::BOARD_LINES));
    mvaddch(boxTop, boxLeft, ACS_ULCORNER);
    for (int i = 1; i < boxWidth - 1; ++i) addch(ACS_HLINE);
    addch(ACS_URCORNER);

    for (int row = 0; row < static_cast<int>(PLAYER_TYPE_OPTIONS.size()) * 2 + 1; ++row)
    {
        mvaddch(boxTop + 1 + row, boxLeft, ACS_VLINE);
        mvaddch(boxTop + 1 + row, boxLeft + boxWidth - 1, ACS_VLINE);
    }

    int boxBottom = boxTop + static_cast<int>(PLAYER_TYPE_OPTIONS.size()) * 2 + 2;
    mvaddch(boxBottom, boxLeft, ACS_LLCORNER);
    for (int i = 1; i < boxWidth - 1; ++i) addch(ACS_HLINE);
    addch(ACS_LRCORNER);
    attroff(COLOR_PAIR(colors::BOARD_LINES));

    // Draw options
    for (int i = 0; i < static_cast<int>(PLAYER_TYPE_OPTIONS.size()); ++i)
    {
        int optionRow = boxTop + 2 + i * 2;
        const auto& option = PLAYER_TYPE_OPTIONS[i];

        if (i == m_selectedIndex)
        {
            attron(COLOR_PAIR(colors::HIGHLIGHT) | A_BOLD);
            mvprintw(optionRow, boxLeft + 2, " > %s %s",
                     option.icon, option.label);
            attroff(COLOR_PAIR(colors::HIGHLIGHT) | A_BOLD);

            // Show description for selected item
            attron(COLOR_PAIR(colors::STATUS) | A_DIM);
            mvprintw(optionRow + 1, boxLeft + 6, "%s", option.description);
            attroff(COLOR_PAIR(colors::STATUS) | A_DIM);
        }
        else
        {
            attron(COLOR_PAIR(colors::EMPTY_CELL));
            mvprintw(optionRow, boxLeft + 2, "   %s %s",
                     option.icon, option.label);
            attroff(COLOR_PAIR(colors::EMPTY_CELL));
        }
    }

    BoardRenderer::drawStatusBar("Up/Down: select | Enter/1-3: confirm | Q: quit");
    refresh();
}

ScreenResult<app::ContinuationResult> PlayerSelectionScreen::handleInput()
{
    int ch = getch();
    switch (ch)
    {
        case KEY_UP:
        case 'w':
        case 'W':
            m_selectedIndex = (m_selectedIndex == 0)
                ? static_cast<int>(PLAYER_TYPE_OPTIONS.size()) - 1
                : m_selectedIndex - 1;
            return app::ContinuationResult::CONTINUE;

        case KEY_DOWN:
        case 's':
        case 'S':
            m_selectedIndex = (m_selectedIndex == static_cast<int>(PLAYER_TYPE_OPTIONS.size()) - 1)
                ? 0
                : m_selectedIndex + 1;
            return app::ContinuationResult::CONTINUE;

        case '1':
            m_selectedType = PLAYER_TYPE_OPTIONS[0].type;
            return app::ContinuationResult::QUIT;

        case '2':
            m_selectedType = PLAYER_TYPE_OPTIONS[1].type;
            return app::ContinuationResult::QUIT;

        case '3':
            m_selectedType = PLAYER_TYPE_OPTIONS[2].type;
            return app::ContinuationResult::QUIT;

        case '\n':
        case ' ':
        case KEY_ENTER:
            m_selectedType = PLAYER_TYPE_OPTIONS[m_selectedIndex].type;
            return app::ContinuationResult::QUIT;

        case 'q':
        case 'Q':
            return std::unexpected(QuitRequested{});

        default:
            return app::ContinuationResult::CONTINUE;
    }
}

void PlayerSelectionScreen::showConfirmation()
{
    int markerColor = (m_markerLabel == "X") ? colors::MARKER_X : colors::MARKER_O;

    const char* selectedLabel = "Unknown";
    for (const auto& opt : PLAYER_TYPE_OPTIONS)
    {
        if (opt.type == m_selectedType)
        {
            selectedLabel = opt.label;
            break;
        }
    }

    std::string confirmMsg = "Player " + std::to_string(m_playerNumber) +
                             " (" + std::string(m_markerLabel) + "): " +
                             selectedLabel + "  [READY]";

    int centerRow = LINES / 2;
    int centerCol = COLS / 2;
    int msgCol = centerCol - static_cast<int>(confirmMsg.size()) / 2;

    // Two quick flashes to draw attention
    for (int flash = 0; flash < 2; ++flash)
    {
        attron(COLOR_PAIR(markerColor) | A_BOLD | A_REVERSE);
        mvprintw(centerRow, msgCol, "%s", confirmMsg.c_str());
        attroff(COLOR_PAIR(markerColor) | A_BOLD | A_REVERSE);
        refresh();
        napms(200);

        attron(COLOR_PAIR(markerColor) | A_BOLD);
        mvprintw(centerRow, msgCol, "%s", confirmMsg.c_str());
        attroff(COLOR_PAIR(markerColor) | A_BOLD);
        refresh();
        napms(120);
    }

    // Hold the confirmed state briefly before transitioning
    attron(COLOR_PAIR(markerColor) | A_BOLD | A_REVERSE);
    mvprintw(centerRow, msgCol, "%s", confirmMsg.c_str());
    attroff(COLOR_PAIR(markerColor) | A_BOLD | A_REVERSE);
    refresh();
    napms(450);
}

ScreenResult<PlayerType> PlayerSelectionScreen::run()
{
    while (true)
    {
        draw();
        auto result = handleInput();
        if (!result.has_value())
        {
            return std::unexpected(QuitRequested{});
        }
        if (result.value() == app::ContinuationResult::QUIT)
        {
            showConfirmation();
            return m_selectedType;
        }
    }
}

} // namespace game::view
