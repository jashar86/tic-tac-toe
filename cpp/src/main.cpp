#include "TicTacToe.h"
#include "Player.h"
#include <iostream>
#include <limits>

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

char playerToChar(Player player) {
    if (player == Player::X) return 'X';
    if (player == Player::O) return 'O';
    return ' ';
}

int main() {
    TicTacToe game;
    char playAgain;

    std::cout << "=================================\n";
    std::cout << "   Welcome to Tic-Tac-Toe!\n";
    std::cout << "=================================\n";

    do {
        game.reset();

        std::cout << "\nBoard positions:\n";
        std::cout << " 1 | 2 | 3 \n";
        std::cout << "---|---|---\n";
        std::cout << " 4 | 5 | 6 \n";
        std::cout << "---|---|---\n";
        std::cout << " 7 | 8 | 9 \n\n";

        while (!game.isGameOver()) {
            game.displayBoard();

            std::cout << "Player " << playerToChar(game.getCurrentPlayer()) << "'s turn\n";
            std::cout << "Enter position (1-9): ";

            int position;
            if (!(std::cin >> position)) {
                clearInput();
                std::cout << "Invalid input! Please enter a number.\n";
                continue;
            }

            if (position < 1 || position > 9) {
                std::cout << "Invalid position! Please enter a number between 1 and 9.\n";
                continue;
            }

            int row = (position - 1) / 3;
            int col = (position - 1) % 3;

            if (!game.makeMove(row, col)) {
                std::cout << "That position is already taken! Try again.\n";
            }
        }

        game.displayBoard();

        Player winner = game.getWinner();
        if (winner != Player::NONE) {
            std::cout << "Player " << playerToChar(winner) << " wins! Congratulations!\n";
        } else {
            std::cout << "It's a draw!\n";
        }

        std::cout << "\nPlay again? (y/n): ";
        std::cin >> playAgain;

    } while (playAgain == 'y' || playAgain == 'Y');

    std::cout << "\nThanks for playing!\n";
    return 0;
}
