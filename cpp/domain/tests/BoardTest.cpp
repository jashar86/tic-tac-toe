#include <gtest/gtest.h>
#include "domain/Board.hpp"
#include "domain/Position.hpp"
#include "domain/Marker.hpp"

namespace game::core {

class BoardTest : public ::testing::Test {
protected:
    Board board;
};

// Empty board tests
TEST_F(BoardTest, NewBoardIsEmpty) {
    EXPECT_TRUE(board.isEmpty());
}

TEST_F(BoardTest, NewBoardHasAllEmptyCells) {
    for (int i = 0; i < 9; ++i) {
        EXPECT_EQ(board.getMarker(Position(i)), Marker::Empty);
    }
}

TEST_F(BoardTest, NewBoardIsNotFull) {
    EXPECT_FALSE(board.isFull());
}

// Setting markers
TEST_F(BoardTest, SetMarkerOnEmptyCell) {
    Position pos(0);
    EXPECT_TRUE(board.setMarker(pos, Marker::X));
    EXPECT_EQ(board.getMarker(pos), Marker::X);
}

TEST_F(BoardTest, SetMarkerOnOccupiedCellFails) {
    Position pos(0);
    board.setMarker(pos, Marker::X);
    EXPECT_FALSE(board.setMarker(pos, Marker::O));
    EXPECT_EQ(board.getMarker(pos), Marker::X);
}

TEST_F(BoardTest, BoardNotEmptyAfterSettingMarker) {
    board.setMarker(Position(4), Marker::X);
    EXPECT_FALSE(board.isEmpty());
}

// Cell emptiness check
TEST_F(BoardTest, CellIsEmptyByDefault) {
    EXPECT_TRUE(board.isCellEmpty(Position(0)));
}

TEST_F(BoardTest, CellNotEmptyAfterSettingMarker) {
    Position pos(0);
    board.setMarker(pos, Marker::X);
    EXPECT_FALSE(board.isCellEmpty(pos));
}

// Full board
TEST_F(BoardTest, BoardIsFullWhenAllCellsFilled) {
    // Fill the board
    board.setMarker(Position(0), Marker::X);
    board.setMarker(Position(1), Marker::O);
    board.setMarker(Position(2), Marker::X);
    board.setMarker(Position(3), Marker::O);
    board.setMarker(Position(4), Marker::X);
    board.setMarker(Position(5), Marker::O);
    board.setMarker(Position(6), Marker::X);
    board.setMarker(Position(7), Marker::O);
    board.setMarker(Position(8), Marker::X);

    EXPECT_TRUE(board.isFull());
}

TEST_F(BoardTest, BoardNotFullWithOneMissing) {
    board.setMarker(Position(0), Marker::X);
    board.setMarker(Position(1), Marker::O);
    board.setMarker(Position(2), Marker::X);
    board.setMarker(Position(3), Marker::O);
    board.setMarker(Position(4), Marker::X);
    board.setMarker(Position(5), Marker::O);
    board.setMarker(Position(6), Marker::X);
    board.setMarker(Position(7), Marker::O);
    // Position 8 left empty

    EXPECT_FALSE(board.isFull());
}

// Marker counting
TEST_F(BoardTest, CountZeroOnEmptyBoard) {
    EXPECT_EQ(board.count(Marker::X), 0);
    EXPECT_EQ(board.count(Marker::O), 0);
    EXPECT_EQ(board.count(Marker::Empty), 9);
}

TEST_F(BoardTest, CountAfterPlacingMarkers) {
    board.setMarker(Position(0), Marker::X);
    board.setMarker(Position(1), Marker::X);
    board.setMarker(Position(2), Marker::O);

    EXPECT_EQ(board.count(Marker::X), 2);
    EXPECT_EQ(board.count(Marker::O), 1);
    EXPECT_EQ(board.count(Marker::Empty), 6);
}

// Available positions
TEST_F(BoardTest, AllPositionsAvailableOnEmptyBoard) {
    auto available = board.availablePositions();
    EXPECT_EQ(available.size(), 9u);
}

TEST_F(BoardTest, AvailablePositionsDecreaseAfterPlacing) {
    board.setMarker(Position(0), Marker::X);
    board.setMarker(Position(4), Marker::O);

    auto available = board.availablePositions();
    EXPECT_EQ(available.size(), 7u);
}

TEST_F(BoardTest, NoAvailablePositionsOnFullBoard) {
    for (int i = 0; i < 9; ++i) {
        board.setMarker(Position(i), (i % 2 == 0) ? Marker::X : Marker::O);
    }

    auto available = board.availablePositions();
    EXPECT_EQ(available.size(), 0u);
}

// Row/col access
TEST_F(BoardTest, SetAndGetByRowCol) {
    board.setMarker(Position(1, 2), Marker::O);
    EXPECT_EQ(board.getMarker(Position(1, 2)), Marker::O);
    EXPECT_EQ(board.getMarker(Position(5)), Marker::O);  // index 5 = row 1, col 2
}

} // namespace game::core
