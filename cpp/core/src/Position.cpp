#include "core/Position.hpp"

namespace game::core
{

Position::Position(int row, int col)
    : index(row * 3 + col)
{
}

int Position::row() const
{
    return index / 3;
}

int Position::col() const
{
    return index % 3;
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
    return index == other.index;
}

bool Position::operator!=(const Position& other) const
{
    return !(*this == other);
}

} // namespace game::core
