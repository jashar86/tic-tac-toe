#ifndef PRESENTATION_NCURSES_BOARD_RENDERER_HPP
#define PRESENTATION_NCURSES_BOARD_RENDERER_HPP

#include "application/Scoreboard.hpp"
#include "application/Session.hpp"
#include "core/Board.hpp"
#include "core/GameStatus.hpp"

#include <array>
#include <optional>
#include <string_view>

namespace game::view
{

class BoardRenderer
{
public:
    static constexpr int STD_CELL_WIDTH    = 9;
    static constexpr int STD_CELL_HEIGHT   = 3;
    static constexpr int LARGE_CELL_WIDTH  = 13;
    static constexpr int LARGE_CELL_HEIGHT = 7;
    static constexpr int MIN_ROWS = 24;
    static constexpr int MIN_COLS = 80;
    static constexpr int LARGE_BOARD_MIN_ROWS = 40;
    static constexpr int LARGE_BOARD_MIN_COLS = 100;
    static constexpr int COMPACT_TITLE_WIDTH = 45;
    static constexpr int FULL_TITLE_WIDTH = 98;

    [[nodiscard]] static bool checkMinimumTerminalSize();

    [[nodiscard]] static bool useLargeBoard();

    [[nodiscard]] static int getCellWidth();
    [[nodiscard]] static int getCellHeight();
    [[nodiscard]] static int getBoardWidth();
    [[nodiscard]] static int getBoardHeight();

    [[nodiscard]] static bool useCompactTitle();

    [[nodiscard]] static int getBoardStartCol();

    [[nodiscard]] static int getBoardStartRow();

    [[nodiscard]] static int getTitleHeight();

    static void drawTitle();

    static void drawBoard(const core::Board& board, int selectedCell = -1,
                          const std::array<int, 3>* winningCells = nullptr);

    static void drawStatusBar(std::string_view message);

    static void drawScoreboard(const app::Session& session);

    static void drawScoreboardCompact(const app::Scoreboard* scoreboard);

    static void flashCell(int cellIndex, const core::Board& board, int times = 3);

    static void highlightWinningLine(const core::Board& board,
                                      const std::array<int, 3>& winningCells);

    [[nodiscard]] static std::optional<std::array<int, 3>>
        getWinningCells(const core::Board& board, core::GameStatus status);

    static void drawWinnerBanner(core::Marker winner);

    static void drawDrawBanner();

    static void runConfettiAnimation(int durationMs = 1500);

    static void fadeIn(int delayMs = 30);

    static void screenWipe(int durationMs = 200);

    static void drawHelpPanel();

private:
    static void drawCellContent(int row, int col,
                                std::optional<core::Marker> marker,
                                int cellIndex, bool selected, bool winning);

    static void drawCompactTitle();
    static void drawFullTitle();
};

} // namespace game::view

#endif // PRESENTATION_NCURSES_BOARD_RENDERER_HPP
