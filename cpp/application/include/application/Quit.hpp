#ifndef GAME_APP_QUIT_HPP
#define GAME_APP_QUIT_HPP

/// \file Quit.hpp
/// \brief Defines the Quit type for signaling user quit requests

namespace game::app
{

/// \brief Sentinel type representing a user's request to quit
///
/// Used in Result types (std::expected) to signal that the user
/// wants to exit the application at any interaction point.
struct Quit
{
    /// \brief Optional message explaining why quit was requested
    const char* message = "User requested quit";
};

} // namespace game::app

#endif // GAME_APP_QUIT_HPP
