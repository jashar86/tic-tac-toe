#ifndef GAME_APP_QUIT_HPP
#define GAME_APP_QUIT_HPP

/// \file Quit.hpp
/// \brief Defines the Quit type for signaling user quit requests

#include <string_view>

namespace game::app
{

/// \brief Sentinel type representing a user's request to quit
///
/// Used in Result types (std::expected) to signal that the user
/// wants to exit the application at any interaction point.
/// \note This class is NOT thread-safe.
struct Quit
{
    /// \brief Default constructor
    constexpr Quit() noexcept = default;

    /// \brief Construct with custom message
    /// \param msg The quit message (must outlive this Quit instance)
    constexpr explicit Quit(std::string_view msg) noexcept : message(msg) {}

    /// \brief Optional message explaining why quit was requested
    std::string_view message = "User requested quit";
};

} // namespace game::app

#endif // GAME_APP_QUIT_HPP
