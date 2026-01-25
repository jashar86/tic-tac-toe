#ifndef GAME_CORE_POSITION_HPP
#define GAME_CORE_POSITION_HPP

/// \file Position.hpp
/// \brief Board position representation

namespace game::core
{

/// \brief Represents a position on the 3x3 board
///
/// Positions can be specified either by index (0-8) or by row/col (0-2, 0-2).
/// The mapping is:
///   0 | 1 | 2
///   ---------
///   3 | 4 | 5
///   ---------
///   6 | 7 | 8
class Position
{
public:
    /// \brief Construct position from linear index
    /// \param index Linear index (0-8)
    explicit Position(int index);

    /// \brief Construct position from row and column
    /// \param row Row index (0-2)
    /// \param col Column index (0-2)
    Position(int row, int col);

    /// \brief Get linear index
    /// \return Index (0-8)
    int index() const;

    /// \brief Get row
    /// \return Row (0-2)
    int row() const;

    /// \brief Get column
    /// \return Column (0-2)
    int col() const;

    /// \brief Check if an index is valid
    /// \param index The index to check
    /// \return true if index is in range [0, 8]
    static bool isValidIndex(int index);

    /// \brief Check if row/col coordinates are valid
    /// \param row The row to check
    /// \param col The column to check
    /// \return true if both are in range [0, 2]
    static bool isValidRowCol(int row, int col);

    /// \brief Equality comparison
    bool operator==(const Position& other) const;

    /// \brief Inequality comparison
    bool operator!=(const Position& other) const;

private:
    int mIndex;
};

} // namespace game::core

#endif // GAME_CORE_POSITION_HPP
