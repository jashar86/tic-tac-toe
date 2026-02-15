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
    const char* label;
    PlayerType type;
};

static constexpr std::array<PlayerTypeOption, 3> PLAYER_TYPE_OPTIONS = {{
    {"Human",            PlayerType::Human},
    {"Computer (Easy)",  PlayerType::CpuEasy},
    {"Computer (Hard)",  PlayerType::CpuHard}
}};

} // anonymous namespace

PlayerSelectionScreen::PlayerSelectionScreen(int playerNumber, std::string_view markerLabel)
    : m_playerNumber(playerNumber)
    , m_markerLabel(markerLabel)
{
}

void PlayerSelectionScreen::draw()
{
    erase();
    BoardRenderer::drawTitle();

    int menuRow = BoardRenderer::BOARD_START_ROW + 1;
    int menuCol = BoardRenderer::BOARD_START_COL;

    attron(COLOR_PAIR(colors::TITLE) | A_BOLD);
    mvprintw(menuRow, menuCol, "Player %d (%.*s) - Select type:",
             m_playerNumber,
             static_cast<int>(m_markerLabel.size()), m_markerLabel.data());
    attroff(COLOR_PAIR(colors::TITLE) | A_BOLD);

    for (int i = 0; i < static_cast<int>(PLAYER_TYPE_OPTIONS.size()); ++i)
    {
        int row = menuRow + 2 + i;
        if (i == m_selectedIndex)
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
            return m_selectedType;
        }
    }
}

} // namespace game::view
