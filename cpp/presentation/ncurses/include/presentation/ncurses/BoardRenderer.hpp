#ifndef PRESENTATION_NCURSES_BOARD_RENDERER_HPP
#define PRESENTATION_NCURSES_BOARD_RENDERER_HPP

#include "application/Scoreboard.hpp"
#include "application/Session.hpp"
#include "core/Board.hpp"

#include <string_view>

namespace game::view
{

class BoardRenderer
{
public:
    static constexpr int BOARD_START_ROW = 10;
    static constexpr int BOARD_START_COL = 4;
    static constexpr int CELL_WIDTH = 7;
    static constexpr int CELL_HEIGHT = 3;
    static constexpr int MIN_ROWS = 22;
    static constexpr int MIN_COLS = 30;

    [[nodiscard]] static bool checkMinimumTerminalSize();

    static void drawTitle();

    static void drawBoard(const core::Board& board, int selectedCell = -1);

    static void drawStatusBar(std::string_view message);

    static void drawScoreboard(const app::Session& session);

    static void drawScoreboardCompact(const app::Scoreboard* scoreboard);

private:
    static void drawCellContent(int row, int col,
                                std::optional<core::Marker> marker,
                                int cellIndex, bool selected);
};

} // namespace game::view

#endif // PRESENTATION_NCURSES_BOARD_RENDERER_HPP
