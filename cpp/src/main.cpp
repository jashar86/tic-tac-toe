/// \file main.cpp
/// \brief Tic-Tac-Toe Main Entry Point
///
/// Supports selecting a UI mode via command line arguments:
///   --ui=console   (default) Plain text console UI
///   --ui=ncurses   ncurses-based terminal UI with cursor navigation

#include <iostream>
#include <memory>
#include <optional>
#include <string_view>

#include <application/TicTacToeApp.hpp>
#include <presentation/Console.hpp>
#include <presentation/Ncurses.hpp>

enum class UiMode
{
    Console,
    Ncurses
};

static void printUsage(std::string_view programName)
{
    std::cerr << "Usage: " << programName << " [--ui=console|ncurses]\n"
              << "\n"
              << "Options:\n"
              << "  --ui=console   Plain text console UI (default)\n"
              << "  --ui=ncurses   ncurses terminal UI with cursor navigation\n"
              << "  --help         Show this help message\n";
}

static std::optional<UiMode> parseArgs(int argc, char* argv[])
{
    UiMode mode = UiMode::Console;

    for (int i = 1; i < argc; ++i)
    {
        std::string_view arg{argv[i]};

        if (arg == "--help" || arg == "-h")
        {
            printUsage(argv[0]);
            return std::nullopt;
        }
        else if (arg == "--ui=console")
        {
            mode = UiMode::Console;
        }
        else if (arg == "--ui=ncurses")
        {
            mode = UiMode::Ncurses;
        }
        else
        {
            std::cerr << "Unknown argument: " << arg << "\n\n";
            printUsage(argv[0]);
            return std::nullopt;
        }
    }

    return mode;
}

int main(int argc, char* argv[])
{
    auto mode = parseArgs(argc, argv);
    if (!mode.has_value())
    {
        return 1;
    }

    if (mode.value() == UiMode::Ncurses)
    {
        game::view::NcursesContext ncursesCtx;

        game::app::TicTacToeApp app{
            std::make_shared<game::view::NcursesSessionGenerator>(),
            std::make_shared<game::view::NcursesGameStartListener>(),
            std::make_shared<game::view::NcursesGameFinishedListener>()};
        app.run();
    }
    else
    {
        game::app::TicTacToeApp app{
            std::make_shared<game::view::ConsoleSessionGenerator>(),
            std::make_shared<game::view::ConsoleGameStartListener>(),
            std::make_shared<game::view::ConsoleGameFinishedListener>()};
        app.run();
    }

    return 0;
}
