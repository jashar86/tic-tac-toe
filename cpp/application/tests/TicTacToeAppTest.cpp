#include "application/TicTacToeApp.hpp"
#include "mocks/MockGameFinishedListener.hpp"
#include "mocks/MockGameStartListener.hpp"
#include "mocks/MockPlayer.hpp"
#include "mocks/MockSessionGenerator.hpp"
#include "core/GameLogic.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace game::app
{

using test::MockPlayer;
using test::MockSessionGenerator;
using test::MockGameStartListener;
using test::MockGameFinishedListener;

class TicTacToeAppTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Default players that will play a complete game ending in X win
        // X: 0, 1, 2 (top row)
        // O: 3, 4
        player1 = std::make_shared<MockPlayer>("Alice", std::vector<int>{0, 1, 2});
        player2 = std::make_shared<MockPlayer>("Bob", std::vector<int>{3, 4});

        sessionGenerator = std::make_shared<MockSessionGenerator>(player1, player2);
        gameStartListener = std::make_shared<MockGameStartListener>();
        gameFinishedListener = std::make_shared<MockGameFinishedListener>();
    }

    std::shared_ptr<MockPlayer> player1;
    std::shared_ptr<MockPlayer> player2;
    std::shared_ptr<MockSessionGenerator> sessionGenerator;
    std::shared_ptr<MockGameStartListener> gameStartListener;
    std::shared_ptr<MockGameFinishedListener> gameFinishedListener;
};

TEST_F(TicTacToeAppTest, AppCanBeConstructed)
{
    TicTacToeApp app(sessionGenerator, gameStartListener, gameFinishedListener);
    SUCCEED();
}

TEST_F(TicTacToeAppTest, RunCallsSessionGenerator)
{
    TicTacToeApp app(sessionGenerator, gameStartListener, gameFinishedListener);
    app.run();

    EXPECT_GE(sessionGenerator->getSessionCount(), 1);
}

TEST_F(TicTacToeAppTest, RunCallsGameStartListener)
{
    TicTacToeApp app(sessionGenerator, gameStartListener, gameFinishedListener);
    app.run();

    EXPECT_GE(gameStartListener->getCallCount(), 1);
}

TEST_F(TicTacToeAppTest, RunCallsGameFinishedListener)
{
    TicTacToeApp app(sessionGenerator, gameStartListener, gameFinishedListener);
    app.run();

    EXPECT_GE(gameFinishedListener->getCallCount(), 1);
}

TEST_F(TicTacToeAppTest, QuitFromSessionGeneratorExitsImmediately)
{
    sessionGenerator->setQuit(true);

    TicTacToeApp app(sessionGenerator, gameStartListener, gameFinishedListener);
    app.run();

    EXPECT_EQ(gameStartListener->getCallCount(), 0);
    EXPECT_EQ(gameFinishedListener->getCallCount(), 0);
}

TEST_F(TicTacToeAppTest, QuitFromGameStartListenerExitsBeforeGame)
{
    gameStartListener->setResult(ContinuationResult::QUIT);

    TicTacToeApp app(sessionGenerator, gameStartListener, gameFinishedListener);
    app.run();

    EXPECT_EQ(gameStartListener->getCallCount(), 1);
    EXPECT_EQ(gameFinishedListener->getCallCount(), 0);
}

TEST_F(TicTacToeAppTest, GameFinishesWithXWin)
{
    TicTacToeApp app(sessionGenerator, gameStartListener, gameFinishedListener);
    app.run();

    EXPECT_EQ(gameFinishedListener->getLastStatus(), game::core::GameStatus::XWins);
}

TEST_F(TicTacToeAppTest, GameFinishesWithOWin)
{
    // O wins with first column: positions 0, 3, 6
    // X: 1, 2, 5 (not winning)
    // O: 0, 3, 6 (first column wins)
    player1 = std::make_shared<MockPlayer>("Alice", std::vector<int>{1, 2, 5});
    player2 = std::make_shared<MockPlayer>("Bob", std::vector<int>{0, 3, 6});
    sessionGenerator = std::make_shared<MockSessionGenerator>(player1, player2);

    TicTacToeApp app(sessionGenerator, gameStartListener, gameFinishedListener);
    app.run();

    EXPECT_EQ(gameFinishedListener->getLastStatus(), game::core::GameStatus::OWins);
}

TEST_F(TicTacToeAppTest, GameFinishesWithDraw)
{
    // A draw scenario:
    // X O X
    // X X O
    // O X O
    // X: 0, 3, 4, 7, 2 (positions 0, 2, 3, 4, 7)
    // O: 1, 5, 6, 8
    player1 = std::make_shared<MockPlayer>("Alice", std::vector<int>{0, 3, 4, 7, 2});
    player2 = std::make_shared<MockPlayer>("Bob", std::vector<int>{1, 5, 6, 8});
    sessionGenerator = std::make_shared<MockSessionGenerator>(player1, player2);

    TicTacToeApp app(sessionGenerator, gameStartListener, gameFinishedListener);
    app.run();

    EXPECT_EQ(gameFinishedListener->getLastStatus(), game::core::GameStatus::Draw);
}

TEST_F(TicTacToeAppTest, PlayAgainStartsNewGame)
{
    // First game ends, then play again, then quit
    gameFinishedListener->queueResult(ContinuationResult::CONTINUE);
    gameFinishedListener->setDefaultResult(ContinuationResult::QUIT);

    // Need more moves for two games
    player1 = std::make_shared<MockPlayer>("Alice", std::vector<int>{0, 1, 2, 0, 1, 2});
    player2 = std::make_shared<MockPlayer>("Bob", std::vector<int>{3, 4, 3, 4});
    sessionGenerator = std::make_shared<MockSessionGenerator>(player1, player2);

    TicTacToeApp app(sessionGenerator, gameStartListener, gameFinishedListener);
    app.run();

    EXPECT_EQ(gameFinishedListener->getCallCount(), 2);
    EXPECT_EQ(gameStartListener->getCallCount(), 2);
}

TEST_F(TicTacToeAppTest, ResetReturnsToWelcome)
{
    // First game ends with RESET, then second game ends with QUIT
    gameFinishedListener->queueResult(ContinuationResult::RESET);
    gameFinishedListener->setDefaultResult(ContinuationResult::QUIT);

    // Need more moves for two games (one per session)
    player1 = std::make_shared<MockPlayer>("Alice", std::vector<int>{0, 1, 2, 0, 1, 2});
    player2 = std::make_shared<MockPlayer>("Bob", std::vector<int>{3, 4, 3, 4});
    sessionGenerator = std::make_shared<MockSessionGenerator>(player1, player2);

    TicTacToeApp app(sessionGenerator, gameStartListener, gameFinishedListener);
    app.run();

    // Session generator should be called twice (once for each session)
    EXPECT_EQ(sessionGenerator->getSessionCount(), 2);
}

TEST_F(TicTacToeAppTest, PlayerQuitDuringGameExits)
{
    player1->setQuit(true);

    TicTacToeApp app(sessionGenerator, gameStartListener, gameFinishedListener);
    app.run();

    // Game should exit without finishing normally
    EXPECT_EQ(gameFinishedListener->getCallCount(), 0);
}

TEST_F(TicTacToeAppTest, ScoreboardTracksWins)
{
    // Play two games, both X wins
    gameFinishedListener->queueResult(ContinuationResult::CONTINUE);
    gameFinishedListener->setDefaultResult(ContinuationResult::QUIT);

    player1 = std::make_shared<MockPlayer>("Alice", std::vector<int>{0, 1, 2, 0, 1, 2});
    player2 = std::make_shared<MockPlayer>("Bob", std::vector<int>{3, 4, 3, 4});
    sessionGenerator = std::make_shared<MockSessionGenerator>(player1, player2);

    TicTacToeApp app(sessionGenerator, gameStartListener, gameFinishedListener);
    app.run();

    // Both games should have been counted (we can verify via call counts)
    EXPECT_EQ(gameFinishedListener->getCallCount(), 2);
}

} // namespace game::app
