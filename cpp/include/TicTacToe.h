#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "Board.h"

class TicTacToe {
private:
    Board board;
    char currentPlayer;

public:
    TicTacToe();

    void displayBoard() const;
    bool makeMove(int row, int col);
    bool isGameOver() const;
    char getWinner() const;
    char getCurrentPlayer() const;
    void reset();
    const Board& getBoard() const;
};

#endif // TICTACTOE_H
