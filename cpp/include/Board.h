#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>

class Board {
private:
    std::vector<std::vector<char>> cells;
    int moveCount;

public:
    Board();

    // Board operations
    bool setCell(int row, int col, char player);
    char getCell(int row, int col) const;
    bool isCellEmpty(int row, int col) const;
    bool isValidPosition(int row, int col) const;
    void reset();

    // Game state checks
    bool hasWinner() const;
    bool isFull() const;
    char getWinner() const;
    int getMoveCount() const;

    // Board representation
    std::string toString() const;
    std::vector<std::vector<char>> getCells() const;
};

#endif // BOARD_H
