#include "core/agents/MinmaxAgent.hpp"

#include "core/Board.hpp"
#include "core/GameLogic.hpp"
#include "core/Marker.hpp"
#include "core/Position.hpp"

#include <gtest/gtest.h>
#include <stdexcept>

namespace game::core
{

class MinmaxAgentTest : public ::testing::Test
{
protected:
    Board board;
    MinmaxAgent agent;
};

// =============================================================================
// Basic functionality
// =============================================================================

TEST_F(MinmaxAgentTest, ReturnsValidPositionOnEmptyBoard)
{
    Position pos = agent.calculateNextMove(board, Marker::X);

    EXPECT_GE(pos.asIndex(), 0);
    EXPECT_LE(pos.asIndex(), 8);
    EXPECT_TRUE(board.isCellEmpty(pos));
}

TEST_F(MinmaxAgentTest, ReturnsOnlyAvailablePosition)
{
    // Fill all but position 4
    board = board.withMove(Position(0), Marker::X)
                 .withMove(Position(1), Marker::O)
                 .withMove(Position(2), Marker::X)
                 .withMove(Position(3), Marker::O)
                 .withMove(Position(5), Marker::O)
                 .withMove(Position(6), Marker::X)
                 .withMove(Position(7), Marker::O)
                 .withMove(Position(8), Marker::X);

    Position pos = agent.calculateNextMove(board, Marker::X);

    EXPECT_EQ(pos.asIndex(), 4);
}

// =============================================================================
// Winning moves - Agent should take a winning move when available
// =============================================================================

TEST_F(MinmaxAgentTest, TakesWinningMoveInRow)
{
    // X has positions 0 and 1, position 2 wins
    board = board.withMove(Position(0), Marker::X)
                 .withMove(Position(1), Marker::X)
                 .withMove(Position(3), Marker::O)
                 .withMove(Position(4), Marker::O);

    Position pos = agent.calculateNextMove(board, Marker::X);

    EXPECT_EQ(pos.asIndex(), 2);
}

TEST_F(MinmaxAgentTest, TakesWinningMoveInColumn)
{
    // X has positions 0 and 3, position 6 wins
    board = board.withMove(Position(0), Marker::X)
                 .withMove(Position(3), Marker::X)
                 .withMove(Position(1), Marker::O)
                 .withMove(Position(4), Marker::O);

    Position pos = agent.calculateNextMove(board, Marker::X);

    EXPECT_EQ(pos.asIndex(), 6);
}

TEST_F(MinmaxAgentTest, TakesWinningMoveInDiagonal)
{
    // X has positions 0 and 4, position 8 wins
    board = board.withMove(Position(0), Marker::X)
                 .withMove(Position(4), Marker::X)
                 .withMove(Position(1), Marker::O)
                 .withMove(Position(2), Marker::O);

    Position pos = agent.calculateNextMove(board, Marker::X);

    EXPECT_EQ(pos.asIndex(), 8);
}

// =============================================================================
// Blocking moves - Agent should block opponent's winning move
// =============================================================================

TEST_F(MinmaxAgentTest, BlocksOpponentWinningMoveInRow)
{
    // O has positions 0 and 1, X must block at 2
    board = board.withMove(Position(0), Marker::O)
                 .withMove(Position(1), Marker::O)
                 .withMove(Position(4), Marker::X);

    Position pos = agent.calculateNextMove(board, Marker::X);

    EXPECT_EQ(pos.asIndex(), 2);
}

TEST_F(MinmaxAgentTest, BlocksOpponentWinningMoveInColumn)
{
    // O has positions 1 and 4, X must block at 7
    board = board.withMove(Position(1), Marker::O)
                 .withMove(Position(4), Marker::O)
                 .withMove(Position(0), Marker::X);

    Position pos = agent.calculateNextMove(board, Marker::X);

    EXPECT_EQ(pos.asIndex(), 7);
}

TEST_F(MinmaxAgentTest, BlocksOpponentWinningMoveInDiagonal)
{
    // O has positions 2 and 4, X must block at 6
    board = board.withMove(Position(2), Marker::O)
                 .withMove(Position(4), Marker::O)
                 .withMove(Position(0), Marker::X);

    Position pos = agent.calculateNextMove(board, Marker::X);

    EXPECT_EQ(pos.asIndex(), 6);
}

// =============================================================================
// Optimal play - Agent should prefer winning over blocking
// =============================================================================

TEST_F(MinmaxAgentTest, PrefersWinningOverBlocking)
{
    // X can win at position 2 (row 0), O threatens at position 6
    // X should take the win, not block
    board = board.withMove(Position(0), Marker::X)
                 .withMove(Position(1), Marker::X)
                 .withMove(Position(3), Marker::O)
                 .withMove(Position(4), Marker::O);
    // X has 0,1 -> can win at 2
    // O has 3,4 -> threatens at 5

    Position pos = agent.calculateNextMove(board, Marker::X);

    EXPECT_EQ(pos.asIndex(), 2);
}

// =============================================================================
// Playing as O
// =============================================================================

TEST_F(MinmaxAgentTest, TakesWinningMoveAsO)
{
    // O has positions 3 and 4, position 5 wins
    board = board.withMove(Position(0), Marker::X)
                 .withMove(Position(3), Marker::O)
                 .withMove(Position(1), Marker::X)
                 .withMove(Position(4), Marker::O)
                 .withMove(Position(8), Marker::X);

    Position pos = agent.calculateNextMove(board, Marker::O);

    EXPECT_EQ(pos.asIndex(), 5);
}

TEST_F(MinmaxAgentTest, BlocksOpponentAsO)
{
    // X has positions 0 and 1, O must block at 2
    board = board.withMove(Position(0), Marker::X)
                 .withMove(Position(4), Marker::O)
                 .withMove(Position(1), Marker::X);

    Position pos = agent.calculateNextMove(board, Marker::O);

    EXPECT_EQ(pos.asIndex(), 2);
}

// =============================================================================
// Error handling
// =============================================================================

TEST_F(MinmaxAgentTest, ThrowsWhenBoardIsFull)
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

    EXPECT_THROW(agent.calculateNextMove(board, Marker::X), std::runtime_error);
}

// =============================================================================
// Optimal play verification - Agent vs Agent always draws
// =============================================================================

TEST_F(MinmaxAgentTest, SelfPlayAlwaysResultsInDraw)
{
    // Two optimal players should always draw
    MinmaxAgent agentX;
    MinmaxAgent agentO;

    Board gameBoard;
    Marker currentPlayer = Marker::X;

    while (checkGameStatus(gameBoard) == GameStatus::InProgress)
    {
        Position move = (currentPlayer == Marker::X)
            ? agentX.calculateNextMove(gameBoard, Marker::X)
            : agentO.calculateNextMove(gameBoard, Marker::O);

        gameBoard = gameBoard.withMove(move, currentPlayer);
        currentPlayer = opponentOf(currentPlayer);
    }

    EXPECT_EQ(checkGameStatus(gameBoard), GameStatus::Draw);
}

// =============================================================================
// Fork handling - Agent should create or block forks
// =============================================================================

TEST_F(MinmaxAgentTest, HandlesEarlyGameOptimally)
{
    // X at corner 0, O at center 4
    // Any move by X leads to draw with optimal play, so verify
    // the agent chooses a valid position
    board = board.withMove(Position(0), Marker::X)
                 .withMove(Position(4), Marker::O);

    Position pos = agent.calculateNextMove(board, Marker::X);

    // Verify the agent picks a valid empty cell
    EXPECT_TRUE(board.isCellEmpty(pos));

    // Play out the rest of the game with optimal agents
    // and verify it results in a draw
    Board gameBoard = board.withMove(pos, Marker::X);
    Marker currentPlayer = Marker::O;
    MinmaxAgent optimalOpponent;

    while (checkGameStatus(gameBoard) == GameStatus::InProgress)
    {
        Position move = optimalOpponent.calculateNextMove(gameBoard, currentPlayer);
        gameBoard = gameBoard.withMove(move, currentPlayer);
        currentPlayer = opponentOf(currentPlayer);
    }

    // X should at least draw (never lose) with optimal play
    EXPECT_NE(checkGameStatus(gameBoard), GameStatus::OWins);
}

TEST_F(MinmaxAgentTest, BlocksFork)
{
    // Classic fork setup: X at corners 0 and 8, O at center
    // O must block by taking an edge, not a corner
    board = board.withMove(Position(0), Marker::X)
                 .withMove(Position(4), Marker::O)
                 .withMove(Position(8), Marker::X);

    Position pos = agent.calculateNextMove(board, Marker::O);

    // O must take an edge (1, 3, 5, or 7) to block the fork
    EXPECT_TRUE(pos.asIndex() == 1 || pos.asIndex() == 3 ||
                pos.asIndex() == 5 || pos.asIndex() == 7);
}

} // namespace game::core
