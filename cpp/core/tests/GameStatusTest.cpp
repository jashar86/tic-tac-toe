#include "core/GameStatus.hpp"

#include <gtest/gtest.h>

namespace game::core
{

TEST(GameStatusTest, HasInProgressValue)
{
    GameStatus status = GameStatus::InProgress;
    EXPECT_EQ(status, GameStatus::InProgress);
}

TEST(GameStatusTest, HasXWinsValue)
{
    GameStatus status = GameStatus::XWins;
    EXPECT_EQ(status, GameStatus::XWins);
}

TEST(GameStatusTest, HasOWinsValue)
{
    GameStatus status = GameStatus::OWins;
    EXPECT_EQ(status, GameStatus::OWins);
}

TEST(GameStatusTest, HasDrawValue)
{
    GameStatus status = GameStatus::Draw;
    EXPECT_EQ(status, GameStatus::Draw);
}

TEST(GameStatusTest, AllValuesAreDifferent)
{
    EXPECT_NE(GameStatus::InProgress, GameStatus::XWins);
    EXPECT_NE(GameStatus::InProgress, GameStatus::OWins);
    EXPECT_NE(GameStatus::InProgress, GameStatus::Draw);
    EXPECT_NE(GameStatus::XWins, GameStatus::OWins);
    EXPECT_NE(GameStatus::XWins, GameStatus::Draw);
    EXPECT_NE(GameStatus::OWins, GameStatus::Draw);
}

TEST(GameStatusTest, isGameOverReturnsFalseForInProgress)
{
    EXPECT_FALSE(isGameOver(GameStatus::InProgress));
}

TEST(GameStatusTest, isGameOverReturnsTrueForXWins)
{
    EXPECT_TRUE(isGameOver(GameStatus::XWins));
}

TEST(GameStatusTest, isGameOverReturnsTrueForOWins)
{
    EXPECT_TRUE(isGameOver(GameStatus::OWins));
}

TEST(GameStatusTest, isGameOverReturnsTrueForDraw)
{
    EXPECT_TRUE(isGameOver(GameStatus::Draw));
}

} // namespace game::core
