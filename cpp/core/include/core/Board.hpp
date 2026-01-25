#ifndef GAME_CORE_BOARD_HPP
#define GAME_CORE_BOARD_HPP

/// \file Board.hpp
/// \brief Tic-Tac-Toe board representation

#include "core/Marker.hpp"
#include "core/Position.hpp"

#include <array>
#include <optional>
#include <vector>

namespace game::core
{

/// \brief Represents a 3x3 Tic-Tac-Toe board
///
/// The board is the core data structure for the game state.
/// It provides methods to query and modify cell contents.
/// Empty cells are represented as std::nullopt.
class Board
{
public:
    /// \brief Construct an empty board
    Board();

    ~Board() = default;

    /// \brief Get the marker at a position
    /// \param pos The position to query
    /// \return The marker at that position, or std::nullopt if empty
    std::optional<Marker> getMarker(const Position& pos) const;

    /// \brief Set a marker at a position
    /// \param pos The position to set
    /// \param marker The marker to place
    /// \return true if successful, false if cell was occupied
    bool setMarker(const Position& pos, Marker marker);

    /// \brief Check if the board is empty
    /// \return true if all cells are empty
    bool isEmpty() const;

    /// \brief Check if a specific cell is empty
    /// \param pos The position to check
    /// \return true if the cell is empty
    bool isCellEmpty(const Position& pos) const;

    /// \brief Check if the board is full
    /// \return true if all cells are occupied
    bool isFull() const;

    /// \brief Count markers of a specific type
    /// \param marker The marker type to count
    /// \return The number of cells with that marker
    int count(Marker marker) const;

    /// \brief Get all available (empty) positions
    /// \return Vector of empty positions
    std::vector<Position> availablePositions() const;

private:
    std::array<std::optional<Marker>, 9> mCells;
};

} // namespace game::core

#endif // GAME_CORE_BOARD_HPP
