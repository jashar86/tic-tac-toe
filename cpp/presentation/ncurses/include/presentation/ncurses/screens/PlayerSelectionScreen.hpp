#ifndef PRESENTATION_NCURSES_SCREENS_PLAYER_SELECTION_SCREEN_HPP
#define PRESENTATION_NCURSES_SCREENS_PLAYER_SELECTION_SCREEN_HPP

#include "presentation/ncurses/IScreen.hpp"

#include <expected>
#include <string_view>

namespace game::view
{

enum class PlayerType
{
    Human,
    CpuEasy,
    CpuHard
};

class PlayerSelectionScreen : public IScreen
{
public:
    PlayerSelectionScreen(int playerNumber, std::string_view markerLabel);

    void draw() override;

    [[nodiscard]] ScreenResult<app::ContinuationResult> handleInput() override;

    [[nodiscard]] ScreenResult<PlayerType> run();

    [[nodiscard]] PlayerType getSelectedType() const { return m_selectedType; }

private:
    void showConfirmation();

    int m_playerNumber;
    std::string_view m_markerLabel;
    int m_selectedIndex{0};
    PlayerType m_selectedType{PlayerType::Human};
    bool m_isFirstDraw{true};
};

} // namespace game::view

#endif // PRESENTATION_NCURSES_SCREENS_PLAYER_SELECTION_SCREEN_HPP
