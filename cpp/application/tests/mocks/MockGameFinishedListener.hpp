#ifndef GAME_APP_MOCK_GAME_FINISHED_LISTENER_HPP
#define GAME_APP_MOCK_GAME_FINISHED_LISTENER_HPP

#include "application/GameFinishedListener.hpp"

#include <vector>

namespace game::app::test
{

/// \brief Mock game finished listener for testing
class MockGameFinishedListener : public GameFinishedListener
{
public:
    MockGameFinishedListener()
        : defaultResult(ContinuationResult::QUIT)
        , callCount(0)
        , lastStatus(game::core::GameStatus::InProgress)
    {
    }

    ContinuationResult onGameFinished(const Session& session) override
    {
        ++callCount;
        lastStatus = session.getGameState().getStatus();

        // Return from queue if available, otherwise return default
        if (!resultQueue.empty())
        {
            auto r = resultQueue.front();
            resultQueue.erase(resultQueue.begin());
            return r;
        }
        return defaultResult;
    }

    void setDefaultResult(ContinuationResult r)
    {
        defaultResult = r;
    }

    void queueResult(ContinuationResult r)
    {
        resultQueue.push_back(r);
    }

    int getCallCount() const
    {
        return callCount;
    }

    game::core::GameStatus getLastStatus() const
    {
        return lastStatus;
    }

private:
    ContinuationResult defaultResult;
    std::vector<ContinuationResult> resultQueue;
    int callCount;
    game::core::GameStatus lastStatus;
};

} // namespace game::app::test

#endif // GAME_APP_MOCK_GAME_FINISHED_LISTENER_HPP
