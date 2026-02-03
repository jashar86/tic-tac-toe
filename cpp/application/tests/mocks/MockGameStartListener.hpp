#ifndef GAME_APP_MOCK_GAME_START_LISTENER_HPP
#define GAME_APP_MOCK_GAME_START_LISTENER_HPP

#include "application/GameStartListener.hpp"

namespace game::app::test
{

/// \brief Mock game start listener for testing
class MockGameStartListener : public GameStartListener
{
public:
    MockGameStartListener()
        : result(ContinuationResult::CONTINUE)
        , callCount(0)
    {
    }

    ContinuationResult onGameStarted(const Session& /*session*/) override
    {
        ++callCount;
        return result;
    }

    void setResult(ContinuationResult r)
    {
        result = r;
    }

    int getCallCount() const
    {
        return callCount;
    }

private:
    ContinuationResult result;
    int callCount;
};

} // namespace game::app::test

#endif // GAME_APP_MOCK_GAME_START_LISTENER_HPP
