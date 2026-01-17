#include <gtest/gtest.h>
#include "Board.h"
#include "Player.h"

class BoardTest : public ::testing::Test {
protected:
    Board board;
};

// Test initialization
TEST_F(BoardTest, InitialStateIsEmpty) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_EQ(board.getCell(i, j), Player::NONE);
            EXPECT_TRUE(board.isCellEmpty(i, j));
        }
    }
    EXPECT_EQ(board.getWinner(), Player::NONE);
    EXPECT_FALSE(board.isFull());
}

// Test position validation
TEST_F(BoardTest, ValidPositionCheck) {
    EXPECT_TRUE(board.isValidPosition(0, 0));
    EXPECT_TRUE(board.isValidPosition(1, 1));
    EXPECT_TRUE(board.isValidPosition(2, 2));

    EXPECT_FALSE(board.isValidPosition(-1, 0));
    EXPECT_FALSE(board.isValidPosition(0, -1));
    EXPECT_FALSE(board.isValidPosition(3, 0));
    EXPECT_FALSE(board.isValidPosition(0, 3));
    EXPECT_FALSE(board.isValidPosition(-1, -1));
    EXPECT_FALSE(board.isValidPosition(3, 3));
}

// Test setting cells
TEST_F(BoardTest, SetCellSuccess) {
    EXPECT_TRUE(board.setCell(0, 0, Player::X));
    EXPECT_EQ(board.getCell(0, 0), Player::X);

    EXPECT_TRUE(board.setCell(1, 1, Player::O));
    EXPECT_EQ(board.getCell(1, 1), Player::O);
}

TEST_F(BoardTest, SetCellInvalidPosition) {
    EXPECT_FALSE(board.setCell(-1, 0, Player::X));
    EXPECT_FALSE(board.setCell(0, 3, Player::X));
}

TEST_F(BoardTest, SetCellAlreadyOccupied) {
    EXPECT_TRUE(board.setCell(0, 0, Player::X));
    EXPECT_FALSE(board.setCell(0, 0, Player::O));
    EXPECT_EQ(board.getCell(0, 0), Player::X);
}

// Test getting cells
TEST_F(BoardTest, GetCellInvalidPosition) {
    EXPECT_EQ(board.getCell(-1, 0), Player::NONE);
    EXPECT_EQ(board.getCell(0, 3), Player::NONE);
    EXPECT_EQ(board.getCell(3, 3), Player::NONE);
}

// Test isEmpty
TEST_F(BoardTest, IsCellEmpty) {
    EXPECT_TRUE(board.isCellEmpty(0, 0));

    board.setCell(0, 0, Player::X);
    EXPECT_FALSE(board.isCellEmpty(0, 0));

    EXPECT_FALSE(board.isCellEmpty(-1, 0));
    EXPECT_FALSE(board.isCellEmpty(3, 3));
}

// Test win detection - rows
TEST_F(BoardTest, WinDetectionRow0) {
    board.setCell(0, 0, Player::X);
    board.setCell(0, 1, Player::X);
    EXPECT_EQ(board.getWinner(), Player::NONE);

    board.setCell(0, 2, Player::X);
    EXPECT_EQ(board.getWinner(), Player::X);
}

TEST_F(BoardTest, WinDetectionRow1) {
    board.setCell(1, 0, Player::O);
    board.setCell(1, 1, Player::O);
    board.setCell(1, 2, Player::O);
    EXPECT_EQ(board.getWinner(), Player::O);
}

TEST_F(BoardTest, WinDetectionRow2) {
    board.setCell(2, 0, Player::X);
    board.setCell(2, 1, Player::X);
    board.setCell(2, 2, Player::X);
    EXPECT_EQ(board.getWinner(), Player::X);
}

// Test win detection - columns
TEST_F(BoardTest, WinDetectionColumn0) {
    board.setCell(0, 0, Player::X);
    board.setCell(1, 0, Player::X);
    board.setCell(2, 0, Player::X);
    EXPECT_EQ(board.getWinner(), Player::X);
}

TEST_F(BoardTest, WinDetectionColumn1) {
    board.setCell(0, 1, Player::O);
    board.setCell(1, 1, Player::O);
    board.setCell(2, 1, Player::O);
    EXPECT_EQ(board.getWinner(), Player::O);
}

TEST_F(BoardTest, WinDetectionColumn2) {
    board.setCell(0, 2, Player::X);
    board.setCell(1, 2, Player::X);
    board.setCell(2, 2, Player::X);
    EXPECT_EQ(board.getWinner(), Player::X);
}

// Test win detection - diagonals
TEST_F(BoardTest, WinDetectionMainDiagonal) {
    board.setCell(0, 0, Player::X);
    board.setCell(1, 1, Player::X);
    board.setCell(2, 2, Player::X);
    EXPECT_EQ(board.getWinner(), Player::X);
}

TEST_F(BoardTest, WinDetectionAntiDiagonal) {
    board.setCell(0, 2, Player::O);
    board.setCell(1, 1, Player::O);
    board.setCell(2, 0, Player::O);
    EXPECT_EQ(board.getWinner(), Player::O);
}

// Test no winner
TEST_F(BoardTest, NoWinnerIncomplete) {
    board.setCell(0, 0, Player::X);
    board.setCell(0, 1, Player::X);
    board.setCell(1, 1, Player::O);
    EXPECT_EQ(board.getWinner(), Player::NONE);
}

// Test draw
TEST_F(BoardTest, DrawDetection) {
    // Set up a draw scenario
    // X O X
    // X O O
    // O X X
    board.setCell(0, 0, Player::X);
    board.setCell(0, 1, Player::O);
    board.setCell(0, 2, Player::X);
    board.setCell(1, 0, Player::X);
    board.setCell(1, 1, Player::O);
    board.setCell(1, 2, Player::O);
    board.setCell(2, 0, Player::O);
    board.setCell(2, 1, Player::X);

    EXPECT_FALSE(board.isFull());
    board.setCell(2, 2, Player::X);

    EXPECT_TRUE(board.isFull());
    EXPECT_EQ(board.getWinner(), Player::NONE);
}

// Test board is full with a winner
TEST_F(BoardTest, FullBoardWithWinner) {
    // X X X
    // O O X
    // O X O
    board.setCell(0, 0, Player::X);
    board.setCell(0, 1, Player::X);
    board.setCell(0, 2, Player::X);
    board.setCell(1, 0, Player::O);
    board.setCell(1, 1, Player::O);
    board.setCell(1, 2, Player::X);
    board.setCell(2, 0, Player::O);
    board.setCell(2, 1, Player::X);
    board.setCell(2, 2, Player::O);

    EXPECT_TRUE(board.isFull());
    EXPECT_EQ(board.getWinner(), Player::X);
}

// Test reset
TEST_F(BoardTest, ResetBoard) {
    board.setCell(0, 0, Player::X);
    board.setCell(1, 1, Player::O);
    board.setCell(2, 2, Player::X);

    board.reset();

    EXPECT_EQ(board.getWinner(), Player::NONE);
    EXPECT_FALSE(board.isFull());

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_EQ(board.getCell(i, j), Player::NONE);
        }
    }
}

// Test toString
TEST_F(BoardTest, ToStringEmpty) {
    std::string expected = "   |   |  \n---|---|---\n   |   |  \n---|---|---\n   |   |  \n";
    EXPECT_EQ(board.toString(), expected);
}

TEST_F(BoardTest, ToStringWithMoves) {
    board.setCell(0, 0, Player::X);
    board.setCell(1, 1, Player::O);
    board.setCell(2, 2, Player::X);

    std::string result = board.toString();
    EXPECT_NE(result.find('X'), std::string::npos);
    EXPECT_NE(result.find('O'), std::string::npos);
}

// Test getCells
TEST_F(BoardTest, GetCells) {
    board.setCell(0, 0, Player::X);
    board.setCell(1, 1, Player::O);

    auto cells = board.getCells();
    EXPECT_EQ(cells.size(), 3);
    EXPECT_EQ(cells[0].size(), 3);
    EXPECT_EQ(cells[0][0], Player::X);
    EXPECT_EQ(cells[1][1], Player::O);
    EXPECT_EQ(cells[2][2], Player::NONE);
}
