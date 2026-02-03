#ifndef GAME_APP_MOCK_PLAYER_HPP
#define GAME_APP_MOCK_PLAYER_HPP

#include "application/Player.hpp"

#include <string>
#include <vector>

namespace game::app::test
{

/// \brief Mock player for testing
class MockPlayer : public Player
{
public:
    explicit MockPlayer(std::string name, std::vector<int> moves = {})
        : playerName(std::move(name))
        , plannedMoves(std::move(moves))
        , moveIndex(0)
        , shouldQuit(false)
    {
    }

    std::string getName() const override
    {
        return playerName;
    }

    std::expected<game::core::Position, Quit> generateNextMove(
        const game::core::Board& /*board*/,
        game::core::Marker /*marker*/) override
    {
        if (shouldQuit)
        {
            return std::unexpected(Quit{});
        }
        if (moveIndex < plannedMoves.size())
        {
            return game::core::Position(plannedMoves[moveIndex++]);
        }
        return game::core::Position(0);
    }

    void setQuit(bool quit)
    {
        shouldQuit = quit;
    }

    void resetMoves()
    {
        moveIndex = 0;
    }

private:
    std::string playerName;
    std::vector<int> plannedMoves;
    size_t moveIndex;
    bool shouldQuit;
};

} // namespace game::app::test

#endif // GAME_APP_MOCK_PLAYER_HPP
