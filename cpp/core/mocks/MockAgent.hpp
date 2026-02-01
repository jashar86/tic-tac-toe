#ifndef GAME_CORE_MOCKS_MOCKAGENT_HPP
#define GAME_CORE_MOCKS_MOCKAGENT_HPP

/// \file MockAgent.hpp
/// \brief Mock implementation of Agent for testing

#include "core/Agent.hpp"

#include <gmock/gmock.h>

namespace game::core::mocks
{

/// \brief Mock Agent for unit testing
///
/// Uses Google Mock to create a mock implementation of the Agent interface.
/// Allows tests to set expectations and return values for calculateNextMove.
class MockAgent : public Agent
{
public:
    MOCK_METHOD(Position, calculateNextMove, (const Board& board, Marker marker), (override));
};

} // namespace game::core::mocks

#endif // GAME_CORE_MOCKS_MOCKAGENT_HPP
