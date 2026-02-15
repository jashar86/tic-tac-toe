#ifndef PRESENTATION_NCURSES_ISCREEN_HPP
#define PRESENTATION_NCURSES_ISCREEN_HPP

#include "application/ContinuationResult.hpp"

#include <expected>

namespace game::view
{

struct QuitRequested {};

template<typename T>
using ScreenResult = std::expected<T, QuitRequested>;

class IScreen
{
public:
    virtual ~IScreen() = default;

    virtual void draw() = 0;

    [[nodiscard]] virtual ScreenResult<app::ContinuationResult> handleInput() = 0;
};

} // namespace game::view

#endif // PRESENTATION_NCURSES_ISCREEN_HPP
