# C++ Development Container

This directory contains the VS Code devcontainer configuration for the Tic-Tac-Toe C++ project.

## Features

- **Base Image**: Microsoft's official C++ devcontainer (Debian 12)
- **Tools Included**:
  - GCC/G++ compiler
  - CMake 3.x
  - GDB debugger
  - Git
  - C++ development libraries

## VS Code Extensions

The following extensions are automatically installed:
- **C/C++** (ms-vscode.cpptools) - IntelliSense, debugging, and code browsing
- **CMake Tools** (ms-vscode.cmake-tools) - CMake integration
- **CMake** (twxs.cmake) - CMake language support
- **C/C++ Extension Pack** (ms-vscode.cpptools-extension-pack) - Complete C++ development suite

## Usage

### Opening in Devcontainer

1. Install the "Dev Containers" extension in VS Code
2. Open this repository in VS Code
3. When prompted, click "Reopen in Container" (or use Command Palette: "Dev Containers: Reopen in Container")
4. VS Code will build the container and configure the environment

### Building the Project

Use the provided VS Code tasks (Terminal → Run Task):
- **Build All** (Ctrl+Shift+B) - Builds all targets
- **Build TicTacToe** - Builds only the main executable
- **Build Tests** - Builds only the test executable
- **Run Tests** - Builds and runs the test suite
- **CMake Configure** - Runs CMake configuration
- **Clean** - Removes the build directory

### Debugging

Two debug configurations are available (Run → Start Debugging or F5):
1. **Debug TicTacToe** - Debug the main application
2. **Debug Tests** - Debug the test suite

Set breakpoints in your code and use F5 to start debugging.

### Manual Build Commands

If you prefer the command line:

```bash
cd cpp
mkdir -p build
cd build
cmake ..
make

# Run the game
./tictactoe

# Run tests
./board_test
```

## Project Structure

```
cpp/
├── CMakeLists.txt      # CMake build configuration
├── include/            # Header files
│   ├── Board.h
│   ├── Player.h
│   └── TicTacToe.h
├── src/                # Source files
│   ├── Board.cpp
│   ├── main.cpp
│   └── TicTacToe.cpp
└── tests/              # Test files
    └── BoardTest.cpp
```

## Post-Create Command

The devcontainer automatically runs a post-create command that:
1. Creates the build directory
2. Runs CMake configuration
3. Builds all targets

This ensures the project is ready to use immediately after the container starts.
