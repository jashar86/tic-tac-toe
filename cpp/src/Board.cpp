#include "Board.h"
#include <sstream>

Board::Board() : moveCount(0) {
    cells = std::vector<std::vector<char>>(3, std::vector<char>(3, ' '));
}

bool Board::setCell(int row, int col, char player) {
    if (!isValidPosition(row, col)) {
        return false;
    }

    if (!isCellEmpty(row, col)) {
        return false;
    }

    cells[row][col] = player;
    moveCount++;
    return true;
}

char Board::getCell(int row, int col) const {
    if (!isValidPosition(row, col)) {
        return '\0';
    }
    return cells[row][col];
}

bool Board::isCellEmpty(int row, int col) const {
    if (!isValidPosition(row, col)) {
        return false;
    }
    return cells[row][col] == ' ';
}

bool Board::isValidPosition(int row, int col) const {
    return row >= 0 && row < 3 && col >= 0 && col < 3;
}

void Board::reset() {
    cells = std::vector<std::vector<char>>(3, std::vector<char>(3, ' '));
    moveCount = 0;
}

bool Board::hasWinner() const {
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (cells[i][0] != ' ' &&
            cells[i][0] == cells[i][1] &&
            cells[i][1] == cells[i][2]) {
            return true;
        }
    }

    // Check columns
    for (int j = 0; j < 3; j++) {
        if (cells[0][j] != ' ' &&
            cells[0][j] == cells[1][j] &&
            cells[1][j] == cells[2][j]) {
            return true;
        }
    }

    // Check diagonals
    if (cells[0][0] != ' ' &&
        cells[0][0] == cells[1][1] &&
        cells[1][1] == cells[2][2]) {
        return true;
    }

    if (cells[0][2] != ' ' &&
        cells[0][2] == cells[1][1] &&
        cells[1][1] == cells[2][0]) {
        return true;
    }

    return false;
}

bool Board::isFull() const {
    return moveCount == 9;
}

char Board::getWinner() const {
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (cells[i][0] != ' ' &&
            cells[i][0] == cells[i][1] &&
            cells[i][1] == cells[i][2]) {
            return cells[i][0];
        }
    }

    // Check columns
    for (int j = 0; j < 3; j++) {
        if (cells[0][j] != ' ' &&
            cells[0][j] == cells[1][j] &&
            cells[1][j] == cells[2][j]) {
            return cells[0][j];
        }
    }

    // Check diagonals
    if (cells[0][0] != ' ' &&
        cells[0][0] == cells[1][1] &&
        cells[1][1] == cells[2][2]) {
        return cells[0][0];
    }

    if (cells[0][2] != ' ' &&
        cells[0][2] == cells[1][1] &&
        cells[1][1] == cells[2][0]) {
        return cells[0][2];
    }

    return ' ';
}

int Board::getMoveCount() const {
    return moveCount;
}

std::string Board::toString() const {
    std::ostringstream oss;
    for (int i = 0; i < 3; i++) {
        oss << " ";
        for (int j = 0; j < 3; j++) {
            oss << cells[i][j];
            if (j < 2) oss << " | ";
        }
        oss << "\n";
        if (i < 2) oss << "---|---|---\n";
    }
    return oss.str();
}

std::vector<std::vector<char>> Board::getCells() const {
    return cells;
}
