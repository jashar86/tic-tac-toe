#include "TicTacToe.h"
#include <iostream>

TicTacToe::TicTacToe() : currentPlayer('X'), movesCount(0) {
    board = std::vector<std::vector<char>>(3, std::vector<char>(3, ' '));
}

void TicTacToe::displayBoard() const {
    std::cout << "\n";
    for (int i = 0; i < 3; i++) {
        std::cout << " ";
        for (int j = 0; j < 3; j++) {
            std::cout << board[i][j];
            if (j < 2) std::cout << " | ";
        }
        std::cout << "\n";
        if (i < 2) std::cout << "---|---|---\n";
    }
    std::cout << "\n";
}

bool TicTacToe::makeMove(int row, int col) {
    if (row < 0 || row > 2 || col < 0 || col > 2) {
        return false;
    }

    if (board[row][col] != ' ') {
        return false;
    }

    board[row][col] = currentPlayer;
    movesCount++;

    if (!isGameOver()) {
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    return true;
}

bool TicTacToe::checkWin() const {
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (board[i][0] != ' ' &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2]) {
            return true;
        }
    }

    // Check columns
    for (int j = 0; j < 3; j++) {
        if (board[0][j] != ' ' &&
            board[0][j] == board[1][j] &&
            board[1][j] == board[2][j]) {
            return true;
        }
    }

    // Check diagonals
    if (board[0][0] != ' ' &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2]) {
        return true;
    }

    if (board[0][2] != ' ' &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0]) {
        return true;
    }

    return false;
}

bool TicTacToe::checkDraw() const {
    return movesCount == 9 && !checkWin();
}

bool TicTacToe::isGameOver() const {
    return checkWin() || checkDraw();
}

char TicTacToe::getWinner() const {
    if (checkWin()) {
        return currentPlayer;
    }
    return ' ';
}

char TicTacToe::getCurrentPlayer() const {
    return currentPlayer;
}

void TicTacToe::reset() {
    board = std::vector<std::vector<char>>(3, std::vector<char>(3, ' '));
    currentPlayer = 'X';
    movesCount = 0;
}
