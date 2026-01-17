# Tic-Tac-Toe (C++)

A command-line implementation of the classic Tic-Tac-Toe game written in C++.

## Project Structure

```
cpp/
├── CMakeLists.txt          # CMake build configuration
├── include/
│   ├── Board.h            # Board class header (game board logic)
│   └── TicTacToe.h        # Game class header (game flow)
├── src/
│   ├── main.cpp           # Entry point
│   ├── Board.cpp          # Board logic implementation
│   └── TicTacToe.cpp      # Game logic implementation
├── tests/
│   └── BoardTest.cpp      # Google Test unit tests for Board class
└── README.md              # This file
```

## Architecture

The game uses a modular design with clear separation of concerns:

- **Board**: Manages the game board state, move validation, and win/draw detection. This class is optimized for future AI player implementation.
- **TicTacToe**: Handles game flow, player turns, and user interaction.

## Requirements

- C++ compiler with C++11 support (g++, clang++, etc.)
- CMake 3.14 or higher
- Internet connection (for first build to download Google Test)

## Building

### Using CMake

```bash
cd cpp
mkdir build
cd build
cmake ..
make
```

### Using g++ directly

```bash
cd cpp
g++ -std=c++11 -I include src/main.cpp src/TicTacToe.cpp src/Board.cpp -o tictactoe
```

## Testing

The project includes comprehensive unit tests using Google Test framework.

### Building and Running Tests

```bash
cd cpp/build
cmake ..
make
ctest --output-on-failure
```

Or run the test executable directly for more detailed output:

```bash
./board_test
```

### Test Coverage

The Board class has comprehensive test coverage including:
- Initial state validation
- Position validation
- Cell operations (get, set, isEmpty)
- Win detection (all rows, columns, and diagonals)
- Draw detection
- Board reset
- Move counting
- Invalid input handling

## Running

After building, run the executable:

```bash
# If using CMake
./build/tictactoe

# If using g++ directly
./tictactoe
```

## How to Play

1. The game is played on a 3x3 grid
2. Players take turns placing their mark (X or O)
3. Enter a position number (1-9) corresponding to:
   ```
   1 | 2 | 3
   ---|---|---
   4 | 5 | 6
   ---|---|---
   7 | 8 | 9
   ```
4. The first player to get three marks in a row (horizontally, vertically, or diagonally) wins
5. If all 9 positions are filled without a winner, the game is a draw
6. After each game, you can choose to play again

## Features

- Two-player gameplay (Player X vs Player O)
- Input validation
- Win detection (rows, columns, diagonals)
- Draw detection
- Play multiple rounds
- Clean, readable console interface
- Modular architecture with separate Board class (optimized for future AI implementation)
- Comprehensive unit tests using Google Test
- CMake build system with integrated testing
