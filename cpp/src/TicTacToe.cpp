#include "TicTacToe.h"
#include <iostream>

TicTacToe::TicTacToe() : currentPlayer(Player::X) {
}

void TicTacToe::displayBoard() const {
    std::cout << "\n" << board.toString() << "\n";
}

bool TicTacToe::makeMove(int row, int col) {
    if (!board.setCell(row, col, currentPlayer)) {
        return false;
    }

    if (!isGameOver()) {
        currentPlayer = (currentPlayer == Player::X) ? Player::O : Player::X;
    }

    return true;
}

bool TicTacToe::isGameOver() const {
    return board.getWinner() != Player::NONE || board.isFull();
}

Player TicTacToe::getWinner() const {
    return board.getWinner();
}

Player TicTacToe::getCurrentPlayer() const {
    return currentPlayer;
}

void TicTacToe::reset() {
    board.reset();
    currentPlayer = Player::X;
}

const Board& TicTacToe::getBoard() const {
    return board;
}
