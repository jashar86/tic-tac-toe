#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/GameLogic.hpp"


namespace game::core
{

class GameLogicTest : public ::testing::Test
{
public:

    using Cell = std::optional<Marker>;

    static constexpr Cell X{std::in_place, Marker::X};
    static constexpr Cell O{std::in_place, Marker::O};
    static constexpr Cell _{};

    void withBoard(std::array<Cell, 9> const& values)
    {
        board = Board{};
        for (int i = 0; i < 9; ++i)
        {
            if (values[i].has_value())
            {
                board.setMarker(Position{i}, values[i].value());
            }
        }
    }

    Board board{};
    
};

std::string marker_to_string(Marker const& marker)
{
    return marker == Marker::X ? "X" : "O";
}

MATCHER_P9(BoardMatches, c1, c2, c3, c4, c5, c6, c7, c8, c9, "")
{
    bool success{true};
    int i{0};
    for(const auto& expected : {c1, c2, c3, c4, c5, c6, c7, c8, c9})
    {
        const auto actual = arg.getMarker(Position{i});
        if (actual != expected)
        {
            *result_listener << "Position " << i << ": " 
                             << "expected " << expected.transform(marker_to_string).value_or("empty")
                             << " but was " << actual.transform(marker_to_string).value_or("empty") << ", "; 
                            
            success = false;
        }
        ++i;
    }
    return success;
};

TEST_F(GameLogicTest, TopRowX)
{
    withBoard({
        X, X, X,
        _, _, _,
        _, _, _
    });

    EXPECT_EQ(checkGameStatus(board), GameStatus::XWins);
}

TEST_F(GameLogicTest, MiddleRowX)
{
    withBoard({
        _, _, _,
        X, X, X,
        _, _, _,
    });
    EXPECT_EQ(checkGameStatus(board), GameStatus::XWins);
}

TEST_F(GameLogicTest, BottomRowX)
{
    withBoard({
        _, _, _,
        _, _, _,
        X, X, X,
    });
    EXPECT_EQ(checkGameStatus(board), GameStatus::XWins);
}

TEST_F(GameLogicTest, LeftCrossX)
{
    withBoard({
        X, _, _,
        _, X, _,
        _, _, X,
    });
    EXPECT_EQ(checkGameStatus(board), GameStatus::XWins);
}

TEST_F(GameLogicTest, RightCrossX)
{
    withBoard({
        _, _, X,
        _, X, _,
        X, _, _,
    });
    EXPECT_EQ(checkGameStatus(board), GameStatus::XWins);
}

TEST_F(GameLogicTest, LeftColumnX)
{
    withBoard({
        X, _, _,
        X, _, _,
        X, _, _,
    });
    EXPECT_EQ(checkGameStatus(board), GameStatus::XWins);
}

TEST_F(GameLogicTest, MiddleColumnX)
{
    withBoard({
        _, X, _,
        _, X, _,
        _, X, _,
    });
    EXPECT_EQ(checkGameStatus(board), GameStatus::XWins);
}

TEST_F(GameLogicTest, RightColumnX)
{
    withBoard({
        _, _, X,
        _, _, X,
        _, _, X,
    });
    EXPECT_EQ(checkGameStatus(board), GameStatus::XWins);
}


TEST_F(GameLogicTest, TopRowO)
{
    withBoard({
        O, O, O,
        _, _, _,
        _, _, _
    });

    EXPECT_EQ(checkGameStatus(board), GameStatus::OWins);
}

TEST_F(GameLogicTest, MiddleRowO)
{
    withBoard({
        _, _, _,
        O, O, O,
        _, _, _,
    });
    EXPECT_EQ(checkGameStatus(board), GameStatus::OWins);
}

TEST_F(GameLogicTest, BottomRowO)
{
    withBoard({
        _, _, _,
        _, _, _,
        O, O, O,
    });
    EXPECT_EQ(checkGameStatus(board), GameStatus::OWins);
}

TEST_F(GameLogicTest, LeftCrossO)
{
    withBoard({
        O, _, _,
        _, O, _,
        _, _, O,
    });
    EXPECT_EQ(checkGameStatus(board), GameStatus::OWins);
}

TEST_F(GameLogicTest, RightCrossO)
{
    withBoard({
        _, _, O,
        _, O, _,
        O, _, _,
    });
    EXPECT_EQ(checkGameStatus(board), GameStatus::OWins);
}

TEST_F(GameLogicTest, LeftColumnO)
{
    withBoard({
        O, _, _,
        O, _, _,
        O, _, _,
    });
    EXPECT_EQ(checkGameStatus(board), GameStatus::OWins);
}

TEST_F(GameLogicTest, MiddleColumnO)
{
    withBoard({
        _, O, _,
        _, O, _,
        _, O, _,
    });
    EXPECT_EQ(checkGameStatus(board), GameStatus::OWins);
}

TEST_F(GameLogicTest, RightColumnO)
{
    withBoard({
        _, _, O,
        _, _, O,
        _, _, O,
    });
    EXPECT_EQ(checkGameStatus(board), GameStatus::OWins);
}

TEST_F(GameLogicTest, Draw)
{
    withBoard({
        X, O, X,
        O, X, O,
        O, X, O,
    });

    EXPECT_EQ(checkGameStatus(board), GameStatus::Draw);
}

TEST_F(GameLogicTest, OneMoveInProgress)
{
    withBoard({
        _, X, _,
        _, _, _,
        O, _, _,
    });

    EXPECT_EQ(checkGameStatus(board), GameStatus::InProgress);
}

TEST_F(GameLogicTest, TwoMovesInProgress)
{
    withBoard({
        _, X, _,
        O, _, X,
        O, _, _,
    });

    EXPECT_EQ(checkGameStatus(board), GameStatus::InProgress);
}

TEST_F(GameLogicTest, ThreeMovesInProgress)
{
    withBoard({
        X, X, O,
        O, _, X,
        O, _, _,
    });

    EXPECT_EQ(checkGameStatus(board), GameStatus::InProgress);
}

TEST_F(GameLogicTest, FourMovesInProgress)
{
    withBoard({
        X, X, O,
        O, X, X,
        O, _, O,
    });

    EXPECT_EQ(checkGameStatus(board), GameStatus::InProgress);
}

TEST_F(GameLogicTest, IsValidMoveByCellValue)
{
    withBoard({
        X, _, O,
        _, X, _,
        O, _, O
    });
    EXPECT_FALSE(isValidMove(board, Position{0}));
    EXPECT_TRUE (isValidMove(board, Position{1}));
    EXPECT_FALSE(isValidMove(board, Position{2}));
    EXPECT_TRUE (isValidMove(board, Position{3}));
    EXPECT_FALSE(isValidMove(board, Position{4}));
    EXPECT_TRUE (isValidMove(board, Position{5}));
    EXPECT_FALSE(isValidMove(board, Position{6}));
    EXPECT_TRUE (isValidMove(board, Position{7}));
    EXPECT_FALSE(isValidMove(board, Position{8}));

    withBoard({
        _, X, _,
        O, _, X,
        _, O, _
    });
    EXPECT_TRUE (isValidMove(board, Position{0}));
    EXPECT_FALSE(isValidMove(board, Position{1}));
    EXPECT_TRUE (isValidMove(board, Position{2}));
    EXPECT_FALSE(isValidMove(board, Position{3}));
    EXPECT_TRUE (isValidMove(board, Position{4}));
    EXPECT_FALSE(isValidMove(board, Position{5}));
    EXPECT_TRUE (isValidMove(board, Position{6}));
    EXPECT_FALSE(isValidMove(board, Position{7}));
    EXPECT_TRUE (isValidMove(board, Position{8}));
}

TEST_F(GameLogicTest, IsValidMoveOutOfBounds)
{
    EXPECT_FALSE(isValidMove(board, Position{9}));
}

TEST_F(GameLogicTest, TakeTurn_FirstMove)
{
    withBoard({
        _, _, _,
        _, _, _,
        _, _, _
    });

    GameState before;
    GameState after;

    before = GameState{board, Marker::O, GameStatus::InProgress};

    // Turn #1
    auto result = takeTurn(before, Position{2});
    ASSERT_TRUE(result.has_value());
    after = result.value();

    EXPECT_THAT(after.getBoard(), BoardMatches(
        _, _, O,
        _, _, _,
        _, _, _
    ));
    EXPECT_EQ(after.getCurrentTurn(), Marker::X);
    EXPECT_EQ(after.getStatus(), GameStatus::InProgress);

    // Turn #2
    before = after;
    result = takeTurn(before, Position{4});\
    ASSERT_TRUE(result.has_value());
    after = result.value();

    EXPECT_THAT(after.getBoard(), BoardMatches(
        _, _, O,
        _, X, _,
        _, _, _
    ));
    EXPECT_EQ(after.getCurrentTurn(), Marker::O);
    EXPECT_EQ(after.getStatus(), GameStatus::InProgress);

    // Turn #3
    before = after;
    result = takeTurn(before, Position{6});\
    ASSERT_TRUE(result.has_value());
    after = result.value();

    EXPECT_THAT(after.getBoard(), BoardMatches(
        _, _, O,
        _, X, _,
        O, _, _
    ));
    EXPECT_EQ(after.getCurrentTurn(), Marker::X);
    EXPECT_EQ(after.getStatus(), GameStatus::InProgress);

    // Turn #4
    before = after;
    result = takeTurn(before, Position{0});\
    ASSERT_TRUE(result.has_value());
    after = result.value();

    EXPECT_THAT(after.getBoard(), BoardMatches(
        X, _, O,
        _, X, _,
        O, _, _
    ));
    EXPECT_EQ(after.getCurrentTurn(), Marker::O);
    EXPECT_EQ(after.getStatus(), GameStatus::InProgress);

    // Turn #5
    before = after;
    result = takeTurn(before, Position{8});\
    ASSERT_TRUE(result.has_value());
    after = result.value();

    EXPECT_THAT(after.getBoard(), BoardMatches(
        X, _, O,
        _, X, _,
        O, _, O
    ));
    EXPECT_EQ(after.getCurrentTurn(), Marker::X);
    EXPECT_EQ(after.getStatus(), GameStatus::InProgress);

    // Turn #6
    before = after;
    result = takeTurn(before, Position{5});\
    ASSERT_TRUE(result.has_value());
    after = result.value();

    EXPECT_THAT(after.getBoard(), BoardMatches(
        X, _, O,
        _, X, X,
        O, _, O
    ));
    EXPECT_EQ(after.getCurrentTurn(), Marker::O);
    EXPECT_EQ(after.getStatus(), GameStatus::InProgress);

    // Turn #7
    before = after;
    result = takeTurn(before, Position{7});\
    ASSERT_TRUE(result.has_value());
    after = result.value();

    EXPECT_THAT(after.getBoard(), BoardMatches(
        X, _, O,
        _, X, X,
        O, O, O
    ));
    EXPECT_EQ(after.getCurrentTurn(), Marker::X);
    EXPECT_EQ(after.getStatus(), GameStatus::OWins);
}

TEST_F(GameLogicTest, TakeTurn_POSITION_OUT_OF_BOUNDS)
{
    GameState state{};
    auto result = takeTurn(state, Position{10});
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), TurnError::POSITION_OUT_OF_BOUNDS);
}

TEST_F(GameLogicTest, TakeTurn_CELL_IS_TAKEN)
{
    Board board{};
    board.setMarker(Position{3}, Marker::X);
    GameState state{board, Marker::O, GameStatus::InProgress};
    auto result = takeTurn(state, Position{3});
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), TurnError::CELL_IS_TAKEN);
}

TEST_F(GameLogicTest, TakeTurn_GAME_NOT_IN_PROGRESS)
{
    GameState state{Board{}, Marker::O, GameStatus::XWins};
    auto result = takeTurn(state, Position{3});
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), TurnError::GAME_NOT_IN_PROGRESS);
}

}