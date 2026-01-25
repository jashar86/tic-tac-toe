#include "core/Board.hpp"

#include <algorithm>

namespace game::core
{

Board::Board()
    : mCells()
{
    // All mCells initialized to std::nullopt by default
}

std::optional<Marker> Board::getMarker(const Position& pos) const { return mCells[pos.index()]; }

bool Board::setMarker(const Position& pos, Marker marker)
{
    if (mCells[pos.index()].has_value())
    {
        return false;
    }
    mCells[pos.index()] = marker;
    return true;
}

bool Board::isEmpty() const
{
    return std::none_of(mCells.begin(), mCells.end(),
                        [](const auto& cell) { return cell.has_value(); });
}

bool Board::isCellEmpty(const Position& pos) const { return !mCells[pos.index()].has_value(); }

bool Board::isFull() const
{
    return std::all_of(mCells.begin(), mCells.end(),
                       [](const auto& cell) { return cell.has_value(); });
}

int Board::count(Marker marker) const
{
    return std::count_if(mCells.begin(), mCells.end(), [marker](const auto& cell)
                         { return cell.has_value() && cell.value() == marker; });
}

std::vector<Position> Board::availablePositions() const
{
    std::vector<Position> positions;
    for (int i = 0; i < 9; ++i)
    {
        if (!mCells[i].has_value())
        {
            positions.emplace_back(i);
        }
    }
    return positions;
}

} // namespace game::core
