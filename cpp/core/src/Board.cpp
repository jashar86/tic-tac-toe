#include "core/Board.hpp"

#include <algorithm>

namespace game::core
{

Board::Board()
    : cells()
{
    // All cells initialized to std::nullopt by default
}

std::optional<Marker> Board::getMarker(const Position& pos) const
{
    return cells[pos.asIndex()];
}

bool Board::setMarker(const Position& pos, Marker marker)
{
    if (cells[pos.asIndex()].has_value())
    {
        return false;
    }
    cells[pos.asIndex()] = marker;
    return true;
}

bool Board::isEmpty() const
{
    return std::none_of(cells.begin(), cells.end(), [](const auto& cell)
    {
        return cell.has_value();
    });
}

bool Board::isCellEmpty(const Position& pos) const
{
    return !cells[pos.asIndex()].has_value();
}

bool Board::isFull() const
{
    return std::all_of(cells.begin(), cells.end(), [](const auto& cell)
    {
        return cell.has_value();
    });
}

int Board::count(Marker marker) const
{
    return std::count_if(cells.begin(), cells.end(), [marker](const auto& cell)
    {
        return cell.has_value() && cell.value() == marker;
    });
}

std::vector<Position> Board::availablePositions() const
{
    std::vector<Position> positions;
    for (int i = 0; i < 9; ++i)
    {
        if (!cells[i].has_value())
        {
            positions.emplace_back(i);
        }
    }
    return positions;
}

} // namespace game::core
