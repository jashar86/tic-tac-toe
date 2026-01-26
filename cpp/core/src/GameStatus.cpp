#include "core/GameStatus.hpp"

namespace game::core
{

bool isGameOver(GameStatus status)
{
    return status != GameStatus::InProgress;
}

} // namespace game::core
