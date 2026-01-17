#include "Board.h"
#include <sstream>

Board::Board() {
    cells = std::vector<std::vector<Player>>(3, std::vector<Player>(3, Player::NONE));
}

bool Board::setCell(int row, int col, Player player) {
    if (!isValidPosition(row, col)) {
        return false;
    }

    if (!isCellEmpty(row, col)) {
        return false;
    }

    cells[row][col] = player;
    return true;
}

Player Board::getCell(int row, int col) const {
    if (!isValidPosition(row, col)) {
        return Player::NONE;
    }
    return cells[row][col];
}

bool Board::isCellEmpty(int row, int col) const {
    if (!isValidPosition(row, col)) {
        return false;
    }
    return cells[row][col] == Player::NONE;
}

bool Board::isValidPosition(int row, int col) const {
    return row >= 0 && row < 3 && col >= 0 && col < 3;
}

void Board::reset() {
    cells = std::vector<std::vector<Player>>(3, std::vector<Player>(3, Player::NONE));
}

bool Board::isFull() const {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (cells[i][j] == Player::NONE) {
                return false;
            }
        }
    }
    return true;
}

Player Board::getWinner() const {
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (cells[i][0] != Player::NONE &&
            cells[i][0] == cells[i][1] &&
            cells[i][1] == cells[i][2]) {
            return cells[i][0];
        }
    }

    // Check columns
    for (int j = 0; j < 3; j++) {
        if (cells[0][j] != Player::NONE &&
            cells[0][j] == cells[1][j] &&
            cells[1][j] == cells[2][j]) {
            return cells[0][j];
        }
    }

    // Check diagonals
    if (cells[0][0] != Player::NONE &&
        cells[0][0] == cells[1][1] &&
        cells[1][1] == cells[2][2]) {
        return cells[0][0];
    }

    if (cells[0][2] != Player::NONE &&
        cells[0][2] == cells[1][1] &&
        cells[1][1] == cells[2][0]) {
        return cells[0][2];
    }

    return Player::NONE;
}

std::string Board::toString() const {
    std::ostringstream oss;
    for (int i = 0; i < 3; i++) {
        oss << " ";
        for (int j = 0; j < 3; j++) {
            // Convert Player enum to display character
            char displayChar = ' ';
            if (cells[i][j] == Player::X) {
                displayChar = 'X';
            } else if (cells[i][j] == Player::O) {
                displayChar = 'O';
            }
            oss << displayChar;
            if (j < 2) oss << " | ";
        }
        oss << "\n";
        if (i < 2) oss << "---|---|---\n";
    }
    return oss.str();
}

std::vector<std::vector<Player>> Board::getCells() const {
    return cells;
}
