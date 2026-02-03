#ifndef GAME_APP_SESSION_GENERATOR_HPP
#define GAME_APP_SESSION_GENERATOR_HPP

/// \file SessionGenerator.hpp
/// \brief Interface for creating new game sessions

#include <expected>
#include <memory>

#include "application/Quit.hpp"
#include "application/Session.hpp"

namespace game::app
{

/// \brief Interface for generating new game sessions
///
/// Implemented by the presentation layer to collect player configuration
/// (names, AI selection) and construct Session objects. This allows
/// the application layer to remain decoupled from UI specifics.
class SessionGenerator
{
public:
    virtual ~SessionGenerator() = default;

    /// \brief Create a new game session
    ///
    /// Prompts the user for player configuration and creates a Session.
    /// Returns Quit if the user wants to exit instead of starting.
    ///
    /// \return A new Session, or Quit if the user requested exit
    virtual std::expected<std::unique_ptr<Session>, Quit> startNewSession() = 0;
};

} // namespace game::app

#endif // GAME_APP_SESSION_GENERATOR_HPP
