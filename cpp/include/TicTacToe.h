#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <vector>
#include <string>

class TicTacToe {
private:
    std::vector<std::vector<char>> board;
    char currentPlayer;
    int movesCount;

    bool checkWin() const;
    bool checkDraw() const;

public:
    TicTacToe();

    void displayBoard() const;
    bool makeMove(int row, int col);
    bool isGameOver() const;
    char getWinner() const;
    char getCurrentPlayer() const;
    void reset();
};

#endif // TICTACTOE_H
