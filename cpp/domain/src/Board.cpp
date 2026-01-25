#include "domain/Board.hpp"
#include <algorithm>

namespace game::core {

Board::Board() : cells_() {
    cells_.fill(Marker::Empty);
}

Marker Board::getMarker(const Position& pos) const {
    return cells_[pos.index()];
}

bool Board::setMarker(const Position& pos, Marker marker) {
    if (cells_[pos.index()] != Marker::Empty) {
        return false;
    }
    cells_[pos.index()] = marker;
    return true;
}

bool Board::isEmpty() const {
    return std::all_of(cells_.begin(), cells_.end(),
                       [](Marker m) { return m == Marker::Empty; });
}

bool Board::isCellEmpty(const Position& pos) const {
    return cells_[pos.index()] == Marker::Empty;
}

bool Board::isFull() const {
    return std::none_of(cells_.begin(), cells_.end(),
                        [](Marker m) { return m == Marker::Empty; });
}

int Board::count(Marker marker) const {
    return std::count(cells_.begin(), cells_.end(), marker);
}

std::vector<Position> Board::availablePositions() const {
    std::vector<Position> positions;
    for (int i = 0; i < 9; ++i) {
        if (cells_[i] == Marker::Empty) {
            positions.emplace_back(i);
        }
    }
    return positions;
}

} // namespace game::core
