#ifndef GAME_APP_MOCK_SESSION_GENERATOR_HPP
#define GAME_APP_MOCK_SESSION_GENERATOR_HPP

#include "application/SessionGenerator.hpp"
#include "MockPlayer.hpp"

namespace game::app::test
{

/// \brief Mock session generator for testing
class MockSessionGenerator : public SessionGenerator
{
public:
    MockSessionGenerator(std::shared_ptr<MockPlayer> p1, std::shared_ptr<MockPlayer> p2)
        : player1(std::move(p1))
        , player2(std::move(p2))
        , shouldQuit(false)
        , sessionCount(0)
    {
    }

    std::expected<std::unique_ptr<Session>, Quit> startNewSession() override
    {
        ++sessionCount;
        if (shouldQuit)
        {
            return std::unexpected(Quit{});
        }
        // Reset player move indices for new session
        player1->resetMoves();
        player2->resetMoves();
        return std::make_unique<Session>(player1, player2);
    }

    void setQuit(bool quit)
    {
        shouldQuit = quit;
    }

    int getSessionCount() const
    {
        return sessionCount;
    }

private:
    std::shared_ptr<MockPlayer> player1;
    std::shared_ptr<MockPlayer> player2;
    bool shouldQuit;
    int sessionCount;
};

} // namespace game::app::test

#endif // GAME_APP_MOCK_SESSION_GENERATOR_HPP
