#ifndef PRESENTATION_NCURSES_SCREENS_GAME_RESULT_SCREEN_HPP
#define PRESENTATION_NCURSES_SCREENS_GAME_RESULT_SCREEN_HPP

#include "presentation/ncurses/IScreen.hpp"
#include "application/Session.hpp"

namespace game::view
{

class GameResultScreen : public IScreen
{
public:
    explicit GameResultScreen(const app::Session& session);

    void draw() override;

    [[nodiscard]] ScreenResult<app::ContinuationResult> handleInput() override;

private:
    const app::Session& m_session;
};

} // namespace game::view

#endif // PRESENTATION_NCURSES_SCREENS_GAME_RESULT_SCREEN_HPP
