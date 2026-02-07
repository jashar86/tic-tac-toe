#include "application/AgentPlayer.hpp"
#include "core/Board.hpp"
#include "core/Marker.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "MockAgent.hpp"

using namespace game::app;
using namespace game::core;

class AgentPlayerTest : public ::testing::Test
{
protected:
    auto makeMockAgent()
    {
        auto agent = std::make_unique<game::core::mocks::MockAgent>();
        mockAgentPtr = agent.get();
        return agent;
    }

    game::core::mocks::MockAgent* mockAgentPtr{nullptr};
};

TEST_F(AgentPlayerTest, GetNameReturnsConfiguredName)
{
    AgentPlayer player("TestBot", makeMockAgent(), std::chrono::milliseconds{0});
    EXPECT_EQ(player.getName(), "TestBot");
}

TEST_F(AgentPlayerTest, DelegatesToAgent)
{
    AgentPlayer player("Bot", makeMockAgent(), std::chrono::milliseconds{0});

    Board board;
    Position expected{4};
    EXPECT_CALL(*mockAgentPtr, calculateNextMove(::testing::_, Marker::X))
        .WillOnce(::testing::Return(expected));

    auto result = player.generateNextMove(board, Marker::X);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), expected);
}

TEST_F(AgentPlayerTest, NeverReturnsQuit)
{
    AgentPlayer player("Bot", makeMockAgent(), std::chrono::milliseconds{0});

    Board board;
    EXPECT_CALL(*mockAgentPtr, calculateNextMove(::testing::_, ::testing::_))
        .Times(3)
        .WillRepeatedly(::testing::Return(Position{0}));

    for (int i = 0; i < 3; ++i)
    {
        auto result = player.generateNextMove(board, Marker::X);
        EXPECT_TRUE(result.has_value());
    }
}

TEST_F(AgentPlayerTest, PassesCorrectMarkerToAgent)
{
    AgentPlayer player("Bot", makeMockAgent(), std::chrono::milliseconds{0});

    Board board;
    EXPECT_CALL(*mockAgentPtr, calculateNextMove(::testing::_, Marker::O))
        .WillOnce(::testing::Return(Position{7}));

    auto result = player.generateNextMove(board, Marker::O);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), Position{7});
}

TEST(CreateAgentPlayerTest, EasyDifficultyCreatesPlayer)
{
    auto player = createAgentPlayer("Easy Bot", AgentDifficulty::Easy,
                                    std::chrono::milliseconds{0});
    ASSERT_NE(player, nullptr);
    EXPECT_EQ(player->getName(), "Easy Bot");

    Board board;
    auto result = player->generateNextMove(board, Marker::X);
    EXPECT_TRUE(result.has_value());
}

TEST(CreateAgentPlayerTest, HardDifficultyCreatesPlayer)
{
    auto player = createAgentPlayer("Hard Bot", AgentDifficulty::Hard,
                                    std::chrono::milliseconds{0});
    ASSERT_NE(player, nullptr);
    EXPECT_EQ(player->getName(), "Hard Bot");

    Board board;
    auto result = player->generateNextMove(board, Marker::X);
    EXPECT_TRUE(result.has_value());
}
