#include "presentation/Console.hpp"
#include "application/ContinuationResult.hpp"
#include "application/Session.hpp"
#include "core/GameStatus.hpp"
#include "core/Marker.hpp"

#include <iostream>
#include <memory>

namespace game::view 
{

namespace 
{
std::string to_string(std::optional<core::Marker> const& marker, char default_val=' ')
{
    if (marker)
    {
        return (marker.value() == core::Marker::X) ? "\033[31mX\033[0m" : "\033[34mO\033[0m"; 
    }
    else 
    {
        return std::string{"\033[90m"} + default_val + "\033[0m";
    }
}

void printRow(core::Board const& board, int row)
{
    std::cout << to_string(board.getMarker(core::Position{row, 0}), '1' + (row*3)) 
        << " ┃ "
        << to_string(board.getMarker(core::Position{row, 1}), '2' + (row*3))
        << " ┃ "
        << to_string(board.getMarker(core::Position{row, 2}), '3' + (row*3))
        << '\n';   
}

void printBoard(core::Board const& board)
{
    static constexpr const char* const ROW_DIVIDER = "━━╋━━━╋━━";
    printRow(board, 0);
    std::cout << ROW_DIVIDER << '\n';
    printRow(board, 1);
    std::cout << ROW_DIVIDER << '\n';
    printRow(board, 2);
    std::cout << std::endl;
}
}

game::view::ConsolePlayer::ConsolePlayer(std::string name)
    : m_name(std::move(name))
{}

std::expected<core::Position, app::Quit> 
game::view::ConsolePlayer::generateNextMove(
        const core::Board& board, 
        core::Marker marker)
{
    printBoard(board);

    char choice{' '};
    while (choice != 'q')
    {        
        std::cout << "Select a move for " << to_string(marker) << " [1-9] (q to quit): ";
        std::cin >> choice;
        if (choice >= '1' && choice <= '9')
        {
            int position = static_cast<int>(choice) - static_cast<int>('1');
            return core::Position{position};
        }
        else if (choice != 'q')
        {
            std::cout << "Invalid option!" << std::endl;
        }
    }
    return std::unexpected(app::Quit{});        
}

app::ContinuationResult ConsoleGameStartListener::onGameStarted(const app::Session& session)
{
    std::ignore = session;
    std::cout << "\n\n\n     STARTING!\n\n\n" << std::endl; 
    return app::ContinuationResult::CONTINUE;
}

app::ContinuationResult ConsoleGameFinishedListener::onGameFinished(const app::Session& session)
{
    printBoard(session.getGameState().getBoard());
    
    switch (session.getGameState().getStatus())
    {
        case core::GameStatus::OWins: std::cout << to_string(core::Marker::O) << " Wins!" << std::endl;
            break;
        case core::GameStatus::XWins: std::cout << to_string(core::Marker::X) << " Wins!" << std::endl;
            break;
        case core::GameStatus::Draw: std::cout << "Draw!" << std::endl;
            break;
        default:
            std::cout << "Invalid state?" << std::endl;
            break;
    }

    const auto& scoreboard = session.getScoreboard();
    std::cout << "\n\n"
        << session.getPlayer1()->getName() << ": " << scoreboard.getPlayer1Wins()
        << "\t"
        << session.getPlayer2()->getName() << ": " << scoreboard.getPlayer2Wins()
        << "\t"
        << "Draws: " << scoreboard.getDraws()
        << "\n\n" << std::endl;

    std::cout << "Play again [y/n]? ";
    char choice{'n'};
    std::cin >> choice;

    if (choice == 'y')
    {
        return app::ContinuationResult::CONTINUE;
    }
    else
    {
        return app::ContinuationResult::QUIT;
    }
}

std::expected<std::unique_ptr<app::Session>, app::Quit> ConsoleSessionGenerator::startNewSession()
{
    return std::make_unique<app::Session>(std::make_shared<ConsolePlayer>("Player 1"),
    std::make_shared<ConsolePlayer>("Player 2"));
}

} // end namespace game::view
