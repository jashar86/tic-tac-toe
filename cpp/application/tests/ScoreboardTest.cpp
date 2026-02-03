#include "application/Scoreboard.hpp"

#include <gtest/gtest.h>

namespace game::app
{

class ScoreboardTest : public ::testing::Test
{
protected:
    Scoreboard scoreboard;
};

TEST_F(ScoreboardTest, NewScoreboardHasZeroPlayer1Wins)
{
    EXPECT_EQ(scoreboard.getPlayer1Wins(), 0);
}

TEST_F(ScoreboardTest, NewScoreboardHasZeroPlayer2Wins)
{
    EXPECT_EQ(scoreboard.getPlayer2Wins(), 0);
}

TEST_F(ScoreboardTest, NewScoreboardHasZeroDraws)
{
    EXPECT_EQ(scoreboard.getDraws(), 0);
}

TEST_F(ScoreboardTest, RecordPlayer1WinIncrementsCounter)
{
    scoreboard.recordPlayer1Win();
    EXPECT_EQ(scoreboard.getPlayer1Wins(), 1);
}

TEST_F(ScoreboardTest, RecordPlayer2WinIncrementsCounter)
{
    scoreboard.recordPlayer2Win();
    EXPECT_EQ(scoreboard.getPlayer2Wins(), 1);
}

TEST_F(ScoreboardTest, RecordDrawIncrementsCounter)
{
    scoreboard.recordDraw();
    EXPECT_EQ(scoreboard.getDraws(), 1);
}

TEST_F(ScoreboardTest, MultipleWinsAccumulate)
{
    scoreboard.recordPlayer1Win();
    scoreboard.recordPlayer1Win();
    scoreboard.recordPlayer2Win();
    scoreboard.recordDraw();
    scoreboard.recordDraw();
    scoreboard.recordDraw();

    EXPECT_EQ(scoreboard.getPlayer1Wins(), 2);
    EXPECT_EQ(scoreboard.getPlayer2Wins(), 1);
    EXPECT_EQ(scoreboard.getDraws(), 3);
}

TEST_F(ScoreboardTest, GetTotalGamesReturnsSum)
{
    scoreboard.recordPlayer1Win();
    scoreboard.recordPlayer2Win();
    scoreboard.recordDraw();

    EXPECT_EQ(scoreboard.getTotalGames(), 3);
}

TEST_F(ScoreboardTest, ResetClearsAllCounters)
{
    scoreboard.recordPlayer1Win();
    scoreboard.recordPlayer2Win();
    scoreboard.recordDraw();

    scoreboard.reset();

    EXPECT_EQ(scoreboard.getPlayer1Wins(), 0);
    EXPECT_EQ(scoreboard.getPlayer2Wins(), 0);
    EXPECT_EQ(scoreboard.getDraws(), 0);
}

} // namespace game::app
