#include "core/Position.hpp"

namespace game::core
{

Position::Position(int index)
    : mIndex(index)
{
}

Position::Position(int row, int col)
    : mIndex(row * 3 + col)
{
}

int Position::index() const
{
    return mIndex;
}

int Position::row() const
{
    return mIndex / 3;
}

int Position::col() const
{
    return mIndex % 3;
}

bool Position::isValidIndex(int index)
{
    return index >= 0 && index < 9;
}

bool Position::isValidRowCol(int row, int col)
{
    return row >= 0 && row < 3 && col >= 0 && col < 3;
}

bool Position::operator==(const Position& other) const
{
    return mIndex == other.mIndex;
}

bool Position::operator!=(const Position& other) const
{
    return !(*this == other);
}

} // namespace game::core
