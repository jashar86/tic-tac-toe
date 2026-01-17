# Tic-Tac-Toe (C++)

A command-line implementation of the classic Tic-Tac-Toe game written in C++.

## Project Structure

```
cpp/
├── CMakeLists.txt          # CMake build configuration
├── include/
│   └── TicTacToe.h        # Game class header
├── src/
│   ├── main.cpp           # Entry point
│   └── TicTacToe.cpp      # Game logic implementation
└── README.md              # This file
```

## Requirements

- C++ compiler with C++11 support (g++, clang++, etc.)
- CMake 3.10 or higher

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
g++ -std=c++11 -I include src/main.cpp src/TicTacToe.cpp -o tictactoe
```

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
