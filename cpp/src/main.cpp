/// \file main.cpp
/// \brief Tic-Tac-Toe Main Entry Point
///
/// This is the main entry point for the Tic-Tac-Toe application.
/// The actual implementation will wire up the presentation layer
/// to run the game.

#include <memory>
#include <presentation/Console.hpp>
#include <application/TicTacToeApp.hpp>

int main()
{
    game::app::TicTacToeApp app{std::make_shared<game::view::ConsoleSessionGenerator>(),
    std::make_shared<game::view::ConsoleGameStartListener>(),
std::make_shared<game::view::ConsoleGameFinishedListener>()};
    app.run();
    return 0;
}
