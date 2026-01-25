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

TEST(GameStatusTest, IsTerminalReturnsFalseForInProgress)
{
    EXPECT_FALSE(isTerminal(GameStatus::InProgress));
}

TEST(GameStatusTest, IsTerminalReturnsTrueForXWins) { EXPECT_TRUE(isTerminal(GameStatus::XWins)); }

TEST(GameStatusTest, IsTerminalReturnsTrueForOWins) { EXPECT_TRUE(isTerminal(GameStatus::OWins)); }

TEST(GameStatusTest, IsTerminalReturnsTrueForDraw) { EXPECT_TRUE(isTerminal(GameStatus::Draw)); }

} // namespace game::core
