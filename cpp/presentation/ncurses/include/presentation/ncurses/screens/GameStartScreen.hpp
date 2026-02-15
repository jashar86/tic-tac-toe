#ifndef PRESENTATION_NCURSES_SCREENS_GAME_START_SCREEN_HPP
#define PRESENTATION_NCURSES_SCREENS_GAME_START_SCREEN_HPP

#include "presentation/ncurses/IScreen.hpp"
#include "application/Session.hpp"

namespace game::view
{

class GameStartScreen : public IScreen
{
public:
    explicit GameStartScreen(const app::Session& session);

    void draw() override;

    [[nodiscard]] ScreenResult<app::ContinuationResult> handleInput() override;

private:
    const app::Session& m_session;
};

} // namespace game::view

#endif // PRESENTATION_NCURSES_SCREENS_GAME_START_SCREEN_HPP
