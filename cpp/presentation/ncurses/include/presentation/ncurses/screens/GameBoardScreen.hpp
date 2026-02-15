#ifndef PRESENTATION_NCURSES_SCREENS_GAME_BOARD_SCREEN_HPP
#define PRESENTATION_NCURSES_SCREENS_GAME_BOARD_SCREEN_HPP

#include "presentation/ncurses/IScreen.hpp"
#include "application/Scoreboard.hpp"
#include "core/Board.hpp"
#include "core/Marker.hpp"
#include "core/Position.hpp"

#include <expected>
#include <string>

namespace game::view
{

class GameBoardScreen : public IScreen
{
public:
    GameBoardScreen(const core::Board& board, core::Marker marker,
                    std::string playerName, const app::Scoreboard* scoreboard);

    void draw() override;

    [[nodiscard]] ScreenResult<app::ContinuationResult> handleInput() override;

    [[nodiscard]] ScreenResult<core::Position> run();

private:
    [[nodiscard]] int findFirstEmptyCell() const;
    void moveSelection(int direction);

    const core::Board& m_board;
    core::Marker m_marker;
    std::string m_playerName;
    const app::Scoreboard* m_scoreboard;
    int m_selectedCell{0};
    core::Position m_selectedPosition{0};
    bool m_positionSelected{false};
};

} // namespace game::view

#endif // PRESENTATION_NCURSES_SCREENS_GAME_BOARD_SCREEN_HPP
