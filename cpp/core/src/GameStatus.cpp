#include "core/GameStatus.hpp"

namespace game::core {

bool isTerminal(GameStatus status) {
    return status != GameStatus::InProgress;
}

} // namespace game::core
