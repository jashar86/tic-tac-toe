#include "core/agents/RandomAgent.hpp"

#include "core/Board.hpp"
#include "core/Marker.hpp"
#include "core/Position.hpp"

#include <gtest/gtest.h>

#include <set>
#include <stdexcept>

namespace game::core
{

class RandomAgentTest : public ::testing::Test
{
protected:
    Board board;
};

TEST_F(RandomAgentTest, ReturnsValidPositionOnEmptyBoard)
{
    RandomAgent agent(42);

    Position pos = agent.calculateNextMove(board, Marker::X);

    EXPECT_GE(pos.asIndex(), 0);
    EXPECT_LE(pos.asIndex(), 8);
    EXPECT_TRUE(board.isCellEmpty(pos));
}

TEST_F(RandomAgentTest, ReturnsOnlyAvailablePosition)
{
    // Fill all but one position
    board = board.withMove(Position(0), Marker::X)
                 .withMove(Position(1), Marker::O)
                 .withMove(Position(2), Marker::X)
                 .withMove(Position(3), Marker::O)
                 .withMove(Position(4), Marker::X)
                 .withMove(Position(5), Marker::O)
                 .withMove(Position(6), Marker::X)
                 .withMove(Position(7), Marker::O);
    // Position 8 is the only available spot

    RandomAgent agent(42);

    Position pos = agent.calculateNextMove(board, Marker::X);

    EXPECT_EQ(pos.asIndex(), 8);
}

TEST_F(RandomAgentTest, ThrowsWhenBoardIsFull)
{
    board = board.withMove(Position(0), Marker::X)
                 .withMove(Position(1), Marker::O)
                 .withMove(Position(2), Marker::X)
                 .withMove(Position(3), Marker::O)
                 .withMove(Position(4), Marker::X)
                 .withMove(Position(5), Marker::O)
                 .withMove(Position(6), Marker::X)
                 .withMove(Position(7), Marker::O)
                 .withMove(Position(8), Marker::X);

    RandomAgent agent(42);

    EXPECT_THROW(agent.calculateNextMove(board, Marker::X), std::runtime_error);
}

TEST_F(RandomAgentTest, SameSeedProducesSameResult)
{
    RandomAgent agent1(12345);
    RandomAgent agent2(12345);

    Position pos1 = agent1.calculateNextMove(board, Marker::X);
    Position pos2 = agent2.calculateNextMove(board, Marker::X);

    EXPECT_EQ(pos1.asIndex(), pos2.asIndex());
}

TEST_F(RandomAgentTest, DifferentSeedsCanProduceDifferentResults)
{
    // Run multiple times with different seeds and verify we get variety
    std::set<int> positions;
    for (unsigned int seed = 0; seed < 100; ++seed)
    {
        RandomAgent agent(seed);
        Position pos = agent.calculateNextMove(board, Marker::X);
        positions.insert(pos.asIndex());
    }

    // With 100 different seeds on an empty board (9 positions),
    // we should see more than 1 unique position selected
    EXPECT_GT(positions.size(), 1u);
}

TEST_F(RandomAgentTest, OnlySelectsFromAvailablePositions)
{
    // Mark some positions as taken
    board = board.withMove(Position(0), Marker::X)
                 .withMove(Position(2), Marker::O)
                 .withMove(Position(4), Marker::X);

    // Run many times to ensure we never select taken positions
    for (unsigned int seed = 0; seed < 100; ++seed)
    {
        RandomAgent agent(seed);
        Position pos = agent.calculateNextMove(board, Marker::O);

        EXPECT_NE(pos.asIndex(), 0) << "Seed " << seed << " selected taken position 0";
        EXPECT_NE(pos.asIndex(), 2) << "Seed " << seed << " selected taken position 2";
        EXPECT_NE(pos.asIndex(), 4) << "Seed " << seed << " selected taken position 4";
        EXPECT_TRUE(board.isCellEmpty(pos)) << "Seed " << seed << " selected non-empty cell";
    }
}

TEST_F(RandomAgentTest, WorksWithBothMarkers)
{
    RandomAgent agent(42);

    Position posX = agent.calculateNextMove(board, Marker::X);
    EXPECT_TRUE(board.isCellEmpty(posX));

    // Reset the agent with same seed to get same behavior
    RandomAgent agent2(42);
    Position posO = agent2.calculateNextMove(board, Marker::O);
    EXPECT_TRUE(board.isCellEmpty(posO));

    // Same seed should give same result regardless of marker
    EXPECT_EQ(posX.asIndex(), posO.asIndex());
}

TEST_F(RandomAgentTest, ConsecutiveCallsProduceDifferentResults)
{
    RandomAgent agent(42);

    // Make multiple calls and track results
    std::set<int> positions;
    Board currentBoard;

    for (int i = 0; i < 9; ++i)
    {
        Position pos = agent.calculateNextMove(currentBoard, Marker::X);
        positions.insert(pos.asIndex());
        currentBoard = currentBoard.withMove(pos, Marker::X);
    }

    // Should have selected all 9 positions exactly once
    EXPECT_EQ(positions.size(), 9u);
}

TEST_F(RandomAgentTest, DefaultConstructorWorks)
{
    // Just verify it doesn't crash and returns a valid position
    RandomAgent agent;

    Position pos = agent.calculateNextMove(board, Marker::X);

    EXPECT_GE(pos.asIndex(), 0);
    EXPECT_LE(pos.asIndex(), 8);
}

} // namespace game::core
