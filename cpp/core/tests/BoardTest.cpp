#include "core/Board.hpp"

#include "core/Marker.hpp"
#include "core/Position.hpp"

#include <gtest/gtest.h>

namespace game::core
{

class BoardTest : public ::testing::Test
{
protected:
    Board board;
};

// Empty board tests
TEST_F(BoardTest, NewBoardIsEmpty)
{
    EXPECT_TRUE(board.isEmpty());
}

TEST_F(BoardTest, NewBoardHasAllEmptyCells)
{
    for (int i = 0; i < 9; ++i)
    {
        EXPECT_FALSE(board.getMarker(Position(i)).has_value());
    }
}

TEST_F(BoardTest, NewBoardIsNotFull)
{
    EXPECT_FALSE(board.isFull());
}

// Setting markers
TEST_F(BoardTest, SetMarkerOnEmptyCell)
{
    Position pos(0);
    ASSERT_FALSE(board.getMarker(pos));
    board = board.withMove(pos, Marker::X);
    EXPECT_EQ(board.getMarker(pos).value(), Marker::X);
}

TEST_F(BoardTest, BoardNotEmptyAfterSettingMarker)
{
    board = board.withMove(Position(4), Marker::X);
    EXPECT_FALSE(board.isEmpty());
}

// Cell emptiness check
TEST_F(BoardTest, CellIsEmptyByDefault)
{
    EXPECT_TRUE(board.isCellEmpty(Position(0)));
}

TEST_F(BoardTest, CellNotEmptyAfterSettingMarker)
{
    Position pos(0);
    board = board.withMove(pos, Marker::X);
    EXPECT_FALSE(board.isCellEmpty(pos));
}

// Full board
TEST_F(BoardTest, BoardIsFullWhenAllCellsFilled)
{
    // Fill the board
    board =  board.withMove(Position(0), Marker::X)
                  .withMove(Position(1), Marker::O)
                  .withMove(Position(2), Marker::X)
                  .withMove(Position(3), Marker::O)
                  .withMove(Position(4), Marker::X)
                  .withMove(Position(5), Marker::O)
                  .withMove(Position(6), Marker::X)
                  .withMove(Position(7), Marker::O)
                  .withMove(Position(8), Marker::X);

    EXPECT_TRUE(board.isFull());
}

TEST_F(BoardTest, BoardNotFullWithOneMissing)
{
    board = board.withMove(Position(0), Marker::X)
                 .withMove(Position(1), Marker::O)
                 .withMove(Position(2), Marker::X)
                 .withMove(Position(3), Marker::O)
                 .withMove(Position(4), Marker::X)
                 .withMove(Position(5), Marker::O)
                 .withMove(Position(6), Marker::X)
                 .withMove(Position(7), Marker::O);
    // Position 8 left empty

    EXPECT_FALSE(board.isFull());
}

// Marker counting
TEST_F(BoardTest, CountZeroOnEmptyBoard)
{
    EXPECT_EQ(board.count(Marker::X), 0);
    EXPECT_EQ(board.count(Marker::O), 0);
}

TEST_F(BoardTest, CountAfterPlacingMarkers)
{
    board = board.withMove(Position(0), Marker::X)
                 .withMove(Position(1), Marker::X)
                 .withMove(Position(2), Marker::O);

    EXPECT_EQ(board.count(Marker::X), 2);
    EXPECT_EQ(board.count(Marker::O), 1);
}

// Available positions
TEST_F(BoardTest, AllPositionsAvailableOnEmptyBoard)
{
    auto available = board.availablePositions();
    ASSERT_EQ(available.size(), 9u);

    // Verify all positions 0-8 are returned
    for (int i = 0; i < 9; ++i)
    {
        EXPECT_EQ(available[i].asIndex(), i);
    }
}

TEST_F(BoardTest, AvailablePositionsDecreaseAfterPlacing)
{
    board = board.withMove(Position(0), Marker::X)
                 .withMove(Position(4), Marker::O);

    auto available = board.availablePositions();
    ASSERT_EQ(available.size(), 7u);

    // Verify the correct positions are returned (1, 2, 3, 5, 6, 7, 8)
    std::vector<int> expectedIndices = {1, 2, 3, 5, 6, 7, 8};
    for (size_t i = 0; i < available.size(); ++i)
    {
        EXPECT_EQ(available[i].asIndex(), expectedIndices[i]);
    }
}

TEST_F(BoardTest, NoAvailablePositionsOnFullBoard)
{
    for (int i = 0; i < 9; ++i)
    {
        board = board.withMove(Position(i), (i % 2 == 0) ? Marker::X : Marker::O);
    }

    auto available = board.availablePositions();
    EXPECT_EQ(available.size(), 0u);
}

// Row/col access
TEST_F(BoardTest, SetAndGetByRowCol)
{
    board = board.withMove(Position(1, 2), Marker::O);
    EXPECT_TRUE(board.getMarker(Position(1, 2)).has_value());
    EXPECT_EQ(board.getMarker(Position(1, 2)).value(), Marker::O);
    EXPECT_EQ(board.getMarker(Position(5)).value(), Marker::O); // index 5 = row 1, col 2
}

} // namespace game::core
