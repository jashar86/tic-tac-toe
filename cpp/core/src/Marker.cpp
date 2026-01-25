#include "core/Marker.hpp"

namespace game::core {

Marker opponent(Marker marker) {
    return (marker == Marker::X) ? Marker::O : Marker::X;
}

} // namespace game::core
