#include "core/GameState.hpp"

#include "core/Board.hpp"
#include "core/GameStatus.hpp"
#include "core/Marker.hpp"
#include "core/Position.hpp"

#include <gtest/gtest.h>

namespace game::core
{

class GameStateTest : public ::testing::Test
{
protected:
    GameState state;
};

// Initial state
TEST_F(GameStateTest, InitialBoardIsEmpty)
{
    EXPECT_TRUE(state.board().isEmpty());
}

TEST_F(GameStateTest, InitialTurnIsX)
{
    EXPECT_EQ(state.currentTurn(), Marker::X);
}

TEST_F(GameStateTest, InitialStatusIsInProgress)
{
    EXPECT_EQ(state.status(), GameStatus::InProgress);
}

// Board access
TEST_F(GameStateTest, BoardReturnsCorrectReference)
{
    const Board& board = state.board();
    EXPECT_TRUE(board.isEmpty());
}

// Construction with parameters
TEST_F(GameStateTest, ConstructWithBoard)
{
    Board board;
    board.setMarker(Position(0), Marker::X);

    GameState customState(board, Marker::O, GameStatus::InProgress);

    EXPECT_FALSE(customState.board().isEmpty());
    EXPECT_TRUE(customState.board().getMarker(Position(0)).has_value());
    EXPECT_EQ(customState.board().getMarker(Position(0)).value(), Marker::X);
    EXPECT_EQ(customState.currentTurn(), Marker::O);
    EXPECT_EQ(customState.status(), GameStatus::InProgress);
}

TEST_F(GameStateTest, ConstructWithXWinsStatus)
{
    Board     board;
    GameState winState(board, Marker::X, GameStatus::XWins);

    EXPECT_EQ(winState.status(), GameStatus::XWins);
}

TEST_F(GameStateTest, ConstructWithOWinsStatus)
{
    Board     board;
    GameState winState(board, Marker::O, GameStatus::OWins);

    EXPECT_EQ(winState.status(), GameStatus::OWins);
}

TEST_F(GameStateTest, ConstructWithDrawStatus)
{
    Board     board;
    GameState drawState(board, Marker::X, GameStatus::Draw);

    EXPECT_EQ(drawState.status(), GameStatus::Draw);
}

} // namespace game::core
