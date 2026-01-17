#include "TicTacToe.h"
#include <iostream>

TicTacToe::TicTacToe() : currentPlayer('X') {
}

void TicTacToe::displayBoard() const {
    std::cout << "\n" << board.toString() << "\n";
}

bool TicTacToe::makeMove(int row, int col) {
    if (!board.setCell(row, col, currentPlayer)) {
        return false;
    }

    if (!isGameOver()) {
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    return true;
}

bool TicTacToe::isGameOver() const {
    return board.hasWinner() || board.isFull();
}

char TicTacToe::getWinner() const {
    return board.getWinner();
}

char TicTacToe::getCurrentPlayer() const {
    return currentPlayer;
}

void TicTacToe::reset() {
    board.reset();
    currentPlayer = 'X';
}

const Board& TicTacToe::getBoard() const {
    return board;
}
