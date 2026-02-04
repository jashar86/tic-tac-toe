#include "application/Session.hpp"
#include "MockPlayer.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace game::app
{

using test::MockPlayer;

class SessionTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        player1 = std::make_shared<MockPlayer>("Alice");
        player2 = std::make_shared<MockPlayer>("Bob");
    }

    std::shared_ptr<Player> player1;
    std::shared_ptr<Player> player2;
};

TEST_F(SessionTest, SessionStoresPlayers)
{
    Session session(player1, player2);

    EXPECT_EQ(session.getPlayer1()->getName(), "Alice");
    EXPECT_EQ(session.getPlayer2()->getName(), "Bob");
}

TEST_F(SessionTest, NewSessionHasEmptyScoreboard)
{
    Session session(player1, player2);

    EXPECT_EQ(session.getScoreboard().getPlayer1Wins(), 0);
    EXPECT_EQ(session.getScoreboard().getPlayer2Wins(), 0);
    EXPECT_EQ(session.getScoreboard().getDraws(), 0);
}

TEST_F(SessionTest, NewSessionHasInitialGameState)
{
    Session session(player1, player2);

    EXPECT_TRUE(session.getGameState().getBoard().isEmpty());
    EXPECT_EQ(session.getGameState().getCurrentTurn(), game::core::Marker::X);
    EXPECT_EQ(session.getGameState().getStatus(), game::core::GameStatus::InProgress);
}

TEST_F(SessionTest, NewSessionIsActive)
{
    Session session(player1, player2);

    EXPECT_TRUE(session.isActive());
}

TEST_F(SessionTest, CanRecordGameOutcomes)
{
    Session session(player1, player2);

    session.recordPlayer1Win();
    session.recordDraw();

    EXPECT_EQ(session.getScoreboard().getPlayer1Wins(), 1);
    EXPECT_EQ(session.getScoreboard().getDraws(), 1);
}

TEST_F(SessionTest, CanUpdateGameState)
{
    Session session(player1, player2);

    auto board = game::core::Board().withMove(game::core::Position(0), game::core::Marker::X);
    game::core::GameState newState(board, game::core::Marker::O, game::core::GameStatus::InProgress);

    session.setGameState(newState);

    EXPECT_FALSE(session.getGameState().getBoard().isEmpty());
    EXPECT_EQ(session.getGameState().getCurrentTurn(), game::core::Marker::O);
}

TEST_F(SessionTest, ResetGameClearsBoard)
{
    Session session(player1, player2);

    auto board = game::core::Board().withMove(game::core::Position(0), game::core::Marker::X);
    game::core::GameState newState(board, game::core::Marker::O, game::core::GameStatus::InProgress);
    session.setGameState(newState);

    session.resetGame();

    EXPECT_TRUE(session.getGameState().getBoard().isEmpty());
    EXPECT_EQ(session.getGameState().getCurrentTurn(), game::core::Marker::X);
}

TEST_F(SessionTest, ResetGamePreservesScoreboard)
{
    Session session(player1, player2);
    session.recordPlayer1Win();

    session.resetGame();

    EXPECT_EQ(session.getScoreboard().getPlayer1Wins(), 1);
}

TEST_F(SessionTest, GetCurrentPlayerReturnsPlayer1WhenXTurn)
{
    Session session(player1, player2);

    EXPECT_EQ(session.getCurrentPlayer(), player1);
}

TEST_F(SessionTest, GetCurrentPlayerReturnsPlayer2WhenOTurn)
{
    Session session(player1, player2);

    auto board = game::core::Board().withMove(game::core::Position(0), game::core::Marker::X);
    game::core::GameState newState(board, game::core::Marker::O, game::core::GameStatus::InProgress);
    session.setGameState(newState);

    EXPECT_EQ(session.getCurrentPlayer(), player2);
}

TEST_F(SessionTest, SessionIsNotActiveWhenGameFinished)
{
    Session session(player1, player2);

    game::core::GameState finishedState(
        game::core::Board(), game::core::Marker::X, game::core::GameStatus::Draw);
    session.setGameState(finishedState);

    EXPECT_FALSE(session.isActive());
}

} // namespace game::app
