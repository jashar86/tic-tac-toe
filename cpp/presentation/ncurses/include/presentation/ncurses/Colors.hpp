#ifndef PRESENTATION_NCURSES_COLORS_HPP
#define PRESENTATION_NCURSES_COLORS_HPP

namespace game::view::colors
{

// Core game colors
constexpr int MARKER_X    = 1;   // Red - aggressive, active player
constexpr int MARKER_O    = 2;   // Blue - calm, reactive player
constexpr int EMPTY_CELL  = 3;   // Dim white - available cells
constexpr int BOARD_LINES = 4;   // Gray - subtle grid lines

// UI element colors
constexpr int TITLE       = 5;   // Cyan - prominent header
constexpr int STATUS      = 6;   // Yellow - instructions and prompts
constexpr int HIGHLIGHT   = 7;   // Inverse - selected items
constexpr int SCORE       = 8;   // Green - positive reinforcement

// Additional UI colors
constexpr int ERROR       = 9;   // Magenta - errors and warnings
constexpr int SUCCESS     = 10;  // Bright green - success states
constexpr int MENU_ACTIVE = 11;  // Bright cyan - active menu items
constexpr int DIM         = 12;  // Dim - subtle/inactive elements

} // namespace game::view::colors

#endif // PRESENTATION_NCURSES_COLORS_HPP
