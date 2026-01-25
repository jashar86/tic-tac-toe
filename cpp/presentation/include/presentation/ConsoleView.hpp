#ifndef TICTACTOE_PRESENTATION_CONSOLEVIEW_HPP
#define TICTACTOE_PRESENTATION_CONSOLEVIEW_HPP

/**
 * @file ConsoleView.hpp
 * @brief Console/Terminal UI implementation
 *
 * The ConsoleView class implements the presentation layer interfaces
 * for a terminal-based user interface. This is part of the
 * Presentation layer and depends on the Application layer.
 *
 * TODO: Implement UI interfaces including:
 * - SessionGenerator - Create sessions with player configuration
 * - Player - Handle human input for moves
 * - GameStartListener - Display new game notifications
 * - GameFinishedListener - Display game results
 */

#include "application/Session.hpp"

namespace tictactoe::presentation {

/**
 * @brief Console-based UI for Tic-Tac-Toe
 *
 * Implements all presentation layer interfaces for a terminal UI.
 * Renders the board as ASCII art and accepts keyboard input.
 */
class ConsoleView {
public:
    ConsoleView();
    ~ConsoleView() = default;

    /**
     * @brief Display a welcome message
     */
    void showWelcome() const;

    /**
     * @brief Check if the view is initialized
     * @return true if the view is ready for use
     */
    bool isInitialized() const;
};

} // namespace tictactoe::presentation

#endif // TICTACTOE_PRESENTATION_CONSOLEVIEW_HPP
