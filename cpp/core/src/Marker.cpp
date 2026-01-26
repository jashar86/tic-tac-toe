#include "core/Marker.hpp"

namespace game::core
{

Marker opponentOf(Marker marker)
{
    return (marker == Marker::X) ? Marker::O : Marker::X;
}

} // namespace game::core
