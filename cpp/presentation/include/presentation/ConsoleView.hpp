#ifndef GAME_VIEW_CONSOLEVIEW_HPP
#define GAME_VIEW_CONSOLEVIEW_HPP

/// \file ConsoleView.hpp
/// \brief Console/Terminal UI implementation
///
/// The ConsoleView class implements the presentation layer interfaces
/// for a terminal-based user interface. This is part of the
/// Presentation layer and depends on the Application layer.

#include "application/Session.hpp"

namespace game::view {

/// \brief Console-based UI for Tic-Tac-Toe
///
/// Implements all presentation layer interfaces for a terminal UI.
/// Renders the board as ASCII art and accepts keyboard input.
class ConsoleView {
public:
    ConsoleView();
    ~ConsoleView() = default;

    /// \brief Display a welcome message
    void showWelcome() const;

    /// \brief Check if the view is initialized
    /// \return true if the view is ready for use
    bool isInitialized() const;
};

} // namespace game::view

#endif // GAME_VIEW_CONSOLEVIEW_HPP
