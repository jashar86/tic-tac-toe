#ifndef GAME_APP_QUIT_HPP
#define GAME_APP_QUIT_HPP

/// \file Quit.hpp
/// \brief Defines the Quit type for signaling user quit requests

namespace game::app
{

/// \brief Tag type representing a user's request to quit
///
/// Used in Result types (std::expected) to signal that the user
/// wants to exit the application at any interaction point.
/// The type provides semantic clarity - std::expected<T, Quit>
/// documents that user exit is a first-class concern for the operation.
struct Quit
{
    constexpr Quit() noexcept = default;
};

} // namespace game::app

#endif // GAME_APP_QUIT_HPP
